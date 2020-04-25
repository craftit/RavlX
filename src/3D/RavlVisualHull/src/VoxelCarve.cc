// Ravl includes
#include "Ravl/SArray1d.hh"
#include "Ravl/SDArray1d.hh"
#include "Ravl/IndexRange2d.hh"
#include "Ravl/Array2dIter3.hh"
#include "Ravl/SArray1dIter.hh"

// MVTHullN includes
#include "Ravl/VisualHull/VoxelCarve.hh"

namespace RavlN { namespace VisualHullN {

  using Ravl3DN::PinholeCameraArrayC;
  using RavlImageN::ImageC;
  using VoxelsN::ByteVoxelOctreeC;
  using VoxelsN::VoxelOctreeNodeC;

// CARVING //////////////////////////////////////////////////////////////

// Carve an Octree voxel set
void
CarveVoxels(ByteVoxelOctreeC& voxels,
            const PinholeCameraArrayC& cameras,
            const SArray1dC< ImageC<ByteT> >& silimages,
            IntT reprojError,
            ByteT silthresh,
            IntT minViews,
            bool conservative,
            bool subdivide,
            RealT threshold)
{
    // Call through with a null initialised range
    CarveVoxels(voxels, cameras, silimages, RealRange3dC(), reprojError, silthresh,
                minViews, conservative, subdivide, threshold);
}

// Carve an Octree voxel set
// Restrict reconstruction to a given volume range
void
CarveVoxels(ByteVoxelOctreeC& voxels,
            const PinholeCameraArrayC& cameras,
            const SArray1dC< ImageC<ByteT> >& silimages,
            const RealRange3dC& range,
            IntT reprojError,
            ByteT silthresh,
            IntT minViews,
            bool conservative,
            bool subdivide,
            RealT threshold)
{
    // Call through converting single error value in to a list of error values
    SArray1dC<IntT> reprojErrors(silimages.Size());
    reprojErrors.Fill(reprojError);
    CarveVoxels(voxels, cameras, silimages, RealRange3dC(), reprojErrors, silthresh,
                minViews, conservative, subdivide, threshold);
}

// Carve an Octree voxel set
// Restrict reconstruction to a given volume range with an array of error values to use
void
CarveVoxels(ByteVoxelOctreeC& voxels,
            const PinholeCameraArrayC& cameras,
            const SArray1dC< ImageC<ByteT> >& silimages,
            const RealRange3dC& range,
            SArray1dC<IntT> reprojErrors,
            ByteT silthresh,
            IntT minViews,
            bool conservative,
            bool subdivide,
            RealT threshold)
{
    VoxelCarveC carve(voxels, cameras, silimages, reprojErrors, range,
                      minViews, threshold, silthresh, conservative, subdivide);
    carve.OctreeCarve();
}

// Carve a Probablistic Octree voxel set
void
CarveVoxelsProbablistic(ByteVoxelOctreeC& voxels,
                        const PinholeCameraArrayC& cameras,
                        const SArray1dC< ImageC<ByteT> >& silimages,
                        const RealRange3dC& range,
                        ByteT silthresh,
                        IntT minViews)
{
    SArray1dC<IntT> reprojErrors(silimages.Size());
    reprojErrors.Fill(5);
    VoxelCarveC carve(voxels, cameras, silimages, reprojErrors, range,
                      minViews, 1.0, silthresh, false, true);
    carve.SetProb(true);
    carve.OctreeCarve();
}

// VoxelCarveC //////////////////////////////////////////////////////////////
VoxelCarveC::VoxelCarveC(ByteVoxelOctreeC voxels,
                    PinholeCameraArrayC cameras,
                    SArray1dC< ImageC<ByteT> > silimages,
                    SArray1dC<IntT> reprojErrors,
                    RealRange3dC vol,
                    IntT minViews,
                    RealT threshold,
                    ByteT silthresh,
                    bool conservative,
                    bool subdivide)
    :m_cameras(cameras),
     m_silimages(silimages),
     m_reprojErrors(reprojErrors),
     m_vol(vol),
     m_minViews(minViews),
     m_threshold(threshold),
     m_silthresh(silthresh),
     m_conservative(conservative),
     m_subdivide(subdivide),
     m_consensus(m_threshold < 1.0),
     m_probablistic(false)
{
    SetVoxels(voxels);
}

// Carve an octree node
void VoxelCarveC::CarveNode(VoxelOctreeNodeC<ByteT> node, int childIndex)
{
    // Calculate extents
    SArray1dC< Vector3dC > extents = CalculateNodeExtents(node, childIndex);

    // Check range
    if (m_vol.IsValid()) {
        RealRange3dC node_range(extents[0],0.0);
        for (IntT i=1; i<8; i++) {
            node_range.Involve(extents[i]);
        }
        if (!m_vol.IsOverlapping(node_range)) {
            // Node is entirely outside of reconstruction volume. Cull node.
            node.Value() = 0;
            return;
        }
    }

    RealT overlap(0.0);
    if (m_probablistic) {
        if (node.Level() < m_voxels.Depth()) {
            overlap = CalculateDepth(extents);
        }
        else {
            overlap = CalculateProbablisticDepth(extents);
        }
    }
    else if (m_consensus) {
        overlap = CalculateConsensusDepth(extents);
    }
    else if (m_conservative) {
        overlap = CalculateConservativeDepth(extents);
    }
    else {
        overlap = CalculateDepth(extents);
    }

    node.Value() = overlap * 255.0;
    // Set the depth at the voxel centroid according to the minimum overlap
    // Do not use node value in this check as small values may give a value of 0
    // here.
    if (overlap)
    {
        // YOU COULD SPEED THIS UP BY ONLY SUBDIVIDING WHERE THERE IS PARTIAL OCCUPANCY
        // BUT IT IS POSSIBLE THAT THERE MAY BE A SURFACE PLANE THAT FALLS EXACTLY BETWEEN
        // TWO NODES THAT ARE NOT AT THE DEEPEST LEVEL IN WHICH CASE YOU WOULD NOT SUBDIVIDE
        // AND YOU WOULD NOT HAVE ANY NODES AT THE FINEST LEVEL TO DEFINE THE SURFACE
        if (node.Level() < m_voxels.Depth()) {
            if (node.Value() < 255 || m_probablistic) {
                // Carve children
                node.Expand(node.Value());
                for (IntT i=0; i<8; i++) {
                    CarveNode(node.Child(i));
                }
            }
            else {
                // Expand all children down to deepest level
                StackC< VoxelOctreeNodeC<ByteT> > stack;
                stack.Push( node );
                while (!stack.IsEmpty()) {
                    VoxelOctreeNodeC<ByteT> top = stack.Pop();
                    top.Expand(node.Value());
                    // Add children if they need to be expanded
                    if (top.Level() < m_voxels.Depth()-1) {
                        for (IntT i=0; i<8; i++) {
                            stack.Push( top.Child(i) );
                        }
                    }
                }
            }
        }
        else if (node.Level() == m_voxels.Depth())
        {
            if (m_subdivide) {
                if (node.Value() < 255 || m_probablistic) {
                    // Carve children
                    RealT sum = 0.0;
                    node.Expand(node.Value());
                    for (IntT i=0; i<8; i++) {
                        CarveNode(node.Child(i), i);
                        sum += (RealT)node.Child(i).Value();
                    }
                    node.Value() = (ByteT)(sum / 8.0);
                    node.Collapse();
                }
            }
        }
    }
}

RealT VoxelCarveC::DetermineOverlap(const SArray1dC<Vector3dC> &extents, IntT cam_index,
                                    const ImageC<IntT> &int_image, ClipState &clipState)
{
    // Find the image plane footprint for the node
    IndexRange2dC range;
    for (IntT i=0; i<8; i++)
    {
        Vector2dC imgPt;
        m_cameras[cam_index].Project(imgPt, extents[i]);
        Index2dC pix((IntT)(imgPt.Y()+0.5),(IntT)(imgPt.X()+0.5));
        i==0 ? range = IndexRange2dC(pix,0) : range.Involve(pix);
    }

    // Determine the true voxel area
    // This is still used to determine the pixel overlap for a voxel even when the voxel is expanded
    // The maximum voxel overlap should really be found up to a shuffle for the possible reprojection error
    // A simpler more conservative overlap is found using the unexpanded voxel area
    // i.e the voxel can be shifted and distorted to cover the maximum amount of pixels
    // Expand the pixel range by the reprojection error
    // Normalising this by the wrong size is causing errors for thin objects.
    RealT voxarea = (range.Rows() * range.Cols());
    // We need to expand the pixel range left and up by one pixel to compute the
    // silhouette overlap using the integral image representation
    range.RowRange().Min()--;
    range.ColRange().Min()--;

    IndexRange2dC clipped_range = range;
    clipped_range.ClipBy(int_image.Frame());
    IndexRange2dC expanded_range = range.Expand(m_reprojErrors[cam_index]);
    expanded_range.ClipBy(int_image.Frame());

    // Check to see if the voxel is clipped
    RealT clippedPixels(0);
    if (clipped_range.IsEmpty()) {
        clipState = FullyClipped;
        return 0;
    }
    if (clipped_range !=  range) {
        clipState = Clipped;
        RealT clip_voxarea = ((clipped_range.Rows() - 1)  * (clipped_range.Cols() - 1));
        clippedPixels = voxarea - clip_voxarea;
    }
    else {
        clipState = NotClipped;
    }

    // Check the occupancy
    RealT pixcount =
       int_image[expanded_range.BottomRight()] +
       int_image[expanded_range.TopLeft()] -
       int_image[expanded_range.TopRight()] -
       int_image[expanded_range.BottomLeft()];

    RealT overlap = (pixcount + clippedPixels) / voxarea;

    if (overlap > 1.0) {
        overlap = 1.0;
    }

    return overlap;
}

RealT VoxelCarveC::CalculateDepth(const SArray1dC<Vector3dC> &extents)
{
    // Determine the overlap with the images
    RealT minoverlap = 1e36;
    IntT numViews = 0;
    for (SizeT cam_index(0); cam_index < m_cameras.Size(); cam_index++) {
        ClipState clipped(NotClipped);
        RealT overlap = DetermineOverlap(extents, cam_index, m_intimages[cam_index], clipped);
        if (clipped == NotClipped) {
            if(overlap == 0.0) {
                // Carve if we are certain that there is no foreground for this voxel
                // i.e a clipped voxel can have foreground pixels outside the camera frame
                return 0.0;
            }
        }
        if(overlap != 0.0) {
            minoverlap = Min(overlap, minoverlap);
            numViews++;
        }
    }

    // Carve any points that are not positively seen by enough cameras
    if (numViews < m_minViews) {
        return 0.0;
    }

    return minoverlap;
}

RealT VoxelCarveC::CalculateConservativeDepth(const SArray1dC<Vector3dC> &extents)
{
    // Determine the overlap with the images
    RealT minoverlap = 1e36;
    IntT numViews = 0;
    for (SizeT cam_index(0); cam_index < m_cameras.Size(); cam_index++) {
        ClipState clipped(NotClipped);
        RealT overlap = DetermineOverlap(extents, cam_index, m_intimages[cam_index], clipped);
        RealT consoverlap = DetermineOverlap(extents, cam_index, m_cons_intimages[cam_index], clipped);
        if (clipped == NotClipped) {
            if(consoverlap == 0.0) {
                // Carve if we are certain that there is no foreground for this voxel
                // i.e a clipped voxel can have foreground pixels outside the camera frame
                return 0.0;
            }
        }
        if(overlap != 0.0) {
            numViews++;
        }
        if(consoverlap != 0.0) {
            minoverlap = Min(consoverlap, minoverlap);
        }
    }

    // Carve any points that are not positively seen by enough cameras
    if (numViews < m_minViews) {
        return 0.0;
    }

    return minoverlap;
}

static bool RealGrEq(const RealT &f1, const RealT &f2)
{
   return f1 >= f2;
}

RealT VoxelCarveC::CalculateConsensusDepth(const SArray1dC<Vector3dC> &extents)
{
    // Determine the overlap with the images
    RealT numVotes = 0.0;
    IntT numViews = 0;

    SArray1dC<RealT> occupancies(m_cameras.Size());
    occupancies.Fill(1.0);
    for (SizeT cam_index(0); cam_index < m_cameras.Size(); cam_index++) {
        ClipState clipped(NotClipped);
        RealT overlap = DetermineOverlap(extents, cam_index, m_intimages[cam_index], clipped);
        RealT consoverlap = DetermineOverlap(extents, cam_index, m_cons_intimages[cam_index], clipped);
        if (clipped == NotClipped || (clipped == Clipped && consoverlap != 0.0)) {
            if (overlap != 0.0) {
                numViews ++;
            }
            occupancies[numVotes] = consoverlap;
            numVotes += 1.0;
        }
    }

    // Carve any points that are not positively seen by enough cameras
    if (numVotes < m_minViews || numViews < m_minViews) {
        return 0.0;
    }

    occupancies.QuickSort(RealGrEq);
    int index = Round(m_threshold * numVotes) + (m_cameras.Size()-1 - numVotes);

    return occupancies[index];
}

RealT VoxelCarveC::CalculateProbablisticDepth(const SArray1dC< Vector3dC > &extents)
{
    float P_D = 0.9;
    float P_FA = 0.2;
    int W = 1;

    float occ = 1.;
    float unocc = 1.;
    for (SizeT cam_index(0); cam_index < m_cameras.Size(); cam_index++) {
        Vector2dC imgPt;
        m_cameras[cam_index].Project(imgPt, extents.Sum() / extents.Size());
        Index2dC pix((IntT)(imgPt.Y()+0.5),(IntT)(imgPt.X()+0.5));
        IndexRange2dC window = IndexRange2dC(pix,0);
        for ( Array2dIterC<ByteT> itSil(m_silimages[cam_index],window); itSil; ++itSil)
        {
            IndexRange2dC s = m_silimages[cam_index].Frame();
            float WS = float(W*W);
            float pfo = (((WS-1.)/WS) * 0.5) + ((1./WS) * P_D);
            float pbo = 1. - pfo;
            float pfu = (((WS-1.)/WS) * 0.5) + ((1./WS) * P_FA); //0.5 * (P_D + P_FA));
            float pbu = 1. - pfu;
            float pib = 0.5;
            // pib = mahal(I[p],bg[p].mean,bg[p].variance)
            // Need to do this rather than clipping as all pixels need to be considered
            if (s.Contains(itSil.Index()))
            {
                if (itSil.Data() == 0)
                {
                    pib = 0.9;
                }
                else
                {
                    pib = 0.1;
                }
            }
            float pif = 1. - pib;
            occ *= (pfo * pif) + (pbo * pib);
            unocc *= (pfu * pif) + (pbu * pib);
        }
    }
    float prob = (occ / (occ + unocc))-0.4;
    if (prob < 0.0) {
        return 0.0;
    }
    else if (prob > 1.0) {
        return 1.0;
    }
    else {
        return prob;
    }
}

SArray1dC< Vector3dC > VoxelCarveC::CalculateNodeExtents(const VoxelOctreeNodeC<ByteT> &node, int childIndex)
{
    // Define the corners of the node in world space
    const IndexRange3dC& rng = node.Range();
    Vector3dC x = m_iDir*((RealT)(rng.IRange().Size()+1)*m_voxels.VoxelSize());
    Vector3dC y = m_jDir*((RealT)(rng.JRange().Size()+1)*m_voxels.VoxelSize());
    Vector3dC z = m_kDir*((RealT)(rng.KRange().Size()+1)*m_voxels.VoxelSize());
    SArray1dC< Vector3dC > extents(8);
    extents[0] = m_voxels.WorldPosition(Vector3dC((RealT)rng.MinI()-0.5,
          (RealT)rng.MinJ()-0.5,(RealT)rng.MinK()-0.5));
    if (childIndex != -1) {
        // Halve the lengths of the cube and offset appropriately
        x /= 2.0;
        y /= 2.0;
        z /= 2.0;
        if (childIndex & 1) {
            extents[0] += x;
        }
        if (childIndex & 2) {
            extents[0] += y;
        }
        if (childIndex & 4) {
            extents[0] += z;
        }
    }
    extents[1] = extents[0] + x;
    extents[2] = extents[0] + y;
    extents[3] = extents[1] + y;
    extents[4] = extents[0] + z;
    extents[5] = extents[4] + x;
    extents[6] = extents[4] + y;
    extents[7] = extents[5] + y;
    return extents;
}

//: The Voxel Carving algorithm
void VoxelCarveC::OctreeCarve()
{
    // Clear out the voxel set for this carving session
    m_voxels.OccupiedThreshold() = 127;
    m_voxels.Clear();

    // Construct the integral images
    m_intimages = SArray1dC< ImageC<IntT> >(m_silimages.Size());
    m_cons_intimages = SArray1dC< ImageC<IntT> >(m_silimages.Size());
    for (SizeT i(0); i<m_silimages.Size(); i++)
    {
        m_intimages[i] = ConstructIntegralImage(m_silimages[i], m_silthresh);
        m_cons_intimages[i] = ConstructIntegralImage(m_silimages[i], 0);
    }

    // Carve the root node
    CarveNode(m_voxels.Root());
}

// Construct an integral image from an image thresholding at a given value
ImageC<IntT> VoxelCarveC::ConstructIntegralImage(const ImageC<ByteT> &silhouette, ByteT silthresh)
{
    ImageC<IntT> integral_image(silhouette.Frame());
    Array2dIter2C<IntT, ByteT> image_it(integral_image,silhouette);
    // First row
    IntT rsum = 0;
    do {
        rsum += (image_it.Data2() > silthresh) ? 1 : 0;
        image_it.Data1() = rsum;
    } while (image_it.Next());
    // Subsequent rows
    Array2dIterC<IntT> prev_row_it(integral_image);
    do {
        rsum = 0;
        do {
            rsum += (image_it.Data2() > silthresh) ? 1 : 0;
            image_it.Data1() = rsum + prev_row_it.Data();
            prev_row_it++;
        } while (image_it.Next());
    } while (image_it);
    return integral_image;
}

// Store and set up Voxel related information
void VoxelCarveC::SetVoxels(const ByteVoxelOctreeC &voxels)
{
    m_voxels = voxels;
    m_iDir = Vector3dC(m_voxels.R().TMul( Vector3dC(1,0,0) )).Unit();
    m_jDir = Vector3dC(m_voxels.R().TMul( Vector3dC(0,1,0) )).Unit();
    m_kDir = Vector3dC(m_voxels.R().TMul( Vector3dC(0,0,1) )).Unit();
}

}}

