// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
/////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/LinearAlgebra/General/MatrixInverse.cc"

#include "Ravl/Matrix.hh"
#include "Ravl/SArray1d.hh"
#include "Ravl/CCMath.hh"
#include "Ravl/Vector.hh"
#include "Ravl/LAHooks.hh"

#define DODEBUG 0
#if DODEBUG 
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {
  
  
  //: Calculate the inverse of this matrix.
  
  MatrixC MatrixC::Inverse() const {
    MatrixC ret = Copy();
    if(!ret.InverseIP()) {
      // Could throw an exception ?
      return MatrixC(); 
    }
    return ret;
  }

  //: Do a pseudo inverse 
  // Uses singular value decomposition to decompose the matrix, and sets
  // the singular values smaller than 'thesh' to zero.
  
  MatrixC MatrixC::PseudoInverse(RealT thresh) const {
    MatrixC U, V;
    ONDEBUG(cerr << "*this=" << *this << "\n");
    VectorC D=SVD(*this,U,V);
    ONDEBUG(cerr << "U=" << U << "\n");
    //ONDEBUG(cerr << "V=" << V << "\n");
    ONDEBUG(cerr << "V=" << V.Size1() << " " << V.Size2() << "\n");
    ONDEBUG(cerr << "D=" << D << "\n");
    // Invert diagonal
    RealT aver = D.Sum() / D.Size();
    for(SArray1dIterC<RealT> it(D);it;it++) {
      if(IsSmall(*it,aver,thresh)) {
	//cerr << "Index " << it.Index() << " is small. \n";
	*it = 0;
      } else
	*it = 1 / *it;
    }
    MatrixC md(V.Size2(),U.Size2());
    md.Fill(0);
    int x = Min(D.Size(),md.Size1(),md.Size2());
    for(int i = 0;i < x;i++)
      md[i][i] = D[i]; // Set all the diagonal elements we've got.
    return (V * md).MulT(U);
  }
  
  bool MatrixC::InverseIP(RealT &det) {
    return Inverse_IP_hook(*this, det);
  }
  

  /*
   *------------------------------------------------------------------------
   *
   *		The most general (Gauss-Jordan) matrix inverse
   *
   * This method works for any matrix (which of course must be square and
   * non-singular). Use this method only if none of specialised algorithms
   * (for symmetric, tridiagonal, etc) matrices isn't applicable/available.
   * Also, the matrix to invert has to be _well_ conditioned:
   * Gauss-Jordan eliminations (even with pivoting) perform poorly for
   * near-singular matrices (e.g., Hilbert matrices).
   *
   * The method inverts matrix inplace and returns the determinant if
   * determ_ptr was specified as not nil. determinant will be exactly zero
   * if the matrix turns out to be (numerically) singular. If determ_ptr is
   * nil and matrix happens to be singular, throw up.
   *
   * The algorithm perform in place Gauss-Jordan eliminations with
   * full pivoting. It was adapted from my Algol-68 "translation" (ca 1986)
   * of the FORTRAN code described in
   * Johnson, K. Jeffrey, "Numerical methods in chemistry", New York,
   * N.Y.: Dekker, c1980, 503 pp, p.221
   *
   * Note, since it's much more efficient to perform operations on matrix
   * columns rather than matrix rows (due to the layout of elements in the
   * matrix), the present method implements a "transposed" algorithm.
   *
   */
  
  struct PivotC { int row, col; };
  
  //: Calculate the inverse of this matrix.
  
  bool InverseIP_GaussJordan(MatrixC & m, RealT &det) {
    RavlAlwaysAssertMsg(m.Cols() == m.Rows(),"MatrixC::InverseIP(), Matrix must be square to invert ");
    
    if(m.IsContinuous())
      return minv(&m[0][0],m.Rows()) == 0; // ccmath routine.
    
    // TODO:- Should either copy matrix to continuous memory,
    // or make a version of the ccmath routine that will work
    // with separate rows?
    // ----------------------------------------------------------
    
    // This can cope with non-continuous matrixes, but its
    // a little slower and less accurate.  I figure this 
    // is quicker than coping the matrix ??
    // I'm not sure I like doing this.
    
    double determinant = 1;
    const double singularity_tolerance = 1e-35;
    
    // Locations of pivots (indices start with 0)
    
    SArray1dC<PivotC> pivots(m.Cols());
    SArray1dC<bool> was_pivoted(m.Rows());
    was_pivoted.Fill(false);
    
    PivotC * pivotp;
    const PivotC *endofrow = &(&pivots[0])[m.Rows()];
    for(pivotp = &pivots[0]; pivotp < endofrow; pivotp++){
      unsigned int prow = 0,pcol = 0;		// Location of a pivot to be
      
      {					// Look through all non-pivoted Cols()
	RealT max_value = 0;		// (and Rows()) for a pivot (max elem)
	for(unsigned int j=0; j<m.Rows(); j++)
	  if( !was_pivoted[j] ) {
	    RealT * cp =0;
	    unsigned int k = 0;
	    RealT curr_value = 0;
	    for(k=0,cp=m[j].DataStart(); k<m.Cols(); k++,cp++)
	      if( !was_pivoted[k] && (curr_value = fabs(*cp)) > max_value )
		max_value = curr_value, prow = k, pcol = j;
	  }
	if( max_value < singularity_tolerance ) {
	  //cerr << "Matrix turns out to be singular: can't invert";
	  return 0;
	}
	pivotp->row = prow;
	pivotp->col = pcol;
      }
      
      if( prow != pcol ) {			// Swap prow-th and pcol-th columns to
	                                       // bring the pivot to the diagonal
	RealT * cr = m[prow].DataStart();
	RealT * cc = m[pcol].DataStart();
	for(unsigned int k=0; k<m.Cols(); k++) {
	  RealT temp = *cr; *cr++ = *cc; *cc++ = temp;
	}
      }
      was_pivoted[prow] = true;
      
      {					// Normalise the pivot column and
	RealT * pivot_cp = m[prow].DataStart();
	double pivot_val = pivot_cp[prow];	// pivot is at the diagonal
	determinant *= pivot_val;		// correct the determinant
	pivot_cp[prow] = true;
	for(unsigned int k=0; k<m.Cols(); k++)
	  *pivot_cp++ /= pivot_val;
      }
      
      {					// Perform eliminations
	RealT * pivot_rp = m[0].DataStart() + prow;	// pivot row
	for(unsigned int k=0; k<m.Cols(); k++, pivot_rp += m.Cols())
	  if( k != prow ) {
	    double temp = *pivot_rp;
	    *pivot_rp = 0;
	    RealT * pivot_cp = m[prow].DataStart();	// pivot column
	    RealT * elim_cp  = m[k].DataStart();		// elimination column
	      for(unsigned int l=0; l<m.Cols(); l++)
		*elim_cp++ -= temp * *pivot_cp++;
	  }
      }
    }
    
    int no_swaps = 0;		// Swap exchanged *Rows()* back in place
    for(pivotp = &pivots[m.Rows()-1]; pivotp >= &pivots[0]; pivotp--)
      if( pivotp->row != pivotp->col ) {
	no_swaps++;
	RealT * rp = m[0].DataStart() + pivotp->row;
	RealT * cp = m[0].DataStart() + pivotp->col;
	for(unsigned int k=0; k<m.Rows(); k++, rp += m.Cols(), cp += m.Cols()) {
	  RealT temp = *rp; *rp = *cp; *cp = temp;
	}
      }
    
    det = (no_swaps & 1 ? -determinant : determinant );
    return (det != 0);
  }

}
