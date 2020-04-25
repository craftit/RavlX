// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlImageProc
//! author="Charles Galambos"
//! file="Ravl/Image/Processing/Filters/HomomorphicFilter.cc"

#include "Ravl/Image/HomomorphicFilter.hh"
#include "Ravl/Array2dIter2.hh"
#include "Ravl/Array2dIter.hh"
#include "Ravl/Math.hh"
#include "Ravl/BinStream.hh"

namespace RavlImageN {
#if RAVL_COMPILER_MIPSPRO
  static Slice1dC<ComplexC*> fixSGIBug1;
  static Slice1dIter2C<ComplexC*,ComplexC> fixSGIBug2;
  static SArray1dC<ComplexC*> fixSGIBug3(1);
  static SArray2dC<double> stupidSGIbugfix(1,1);
#endif
  

  //: Constructor.
  
  HomomorphicFilterBodyC::HomomorphicFilterBodyC(RealT nSigma,RealT nDepth,const Index2dC &size) 
    : sigma(nSigma),
      depth(nDepth),
      m_cache(100)
  {
    if(size != Index2dC(0,0)) {
      Tuple3C<FFT2dC,FFT2dC,ImageC<RealT> > filter;
      // The constructor can only be called by one thread so no locking required.
      InitFilter(size,filter);
      m_cache.Insert(size,filter);
    }
  }
  
  
  //: Setup the filter.
  bool HomomorphicFilterBodyC::InitFilter(const Index2dC &size,Tuple3C<FFT2dC,FFT2dC,ImageC<RealT> > &filterSetup)
  {
    FFT2dC fwd(size,false);
    FFT2dC bkw(size,true);
    ImageC<RealT> filter(size[0],size[1]);
    Index2dC v(0,0);
    RealT sp = (sigma * RavlConstN::sqrt2Pi);
    RealT mod = depth * sp;
    for(Array2dIterC<RealT> it(filter);it;it++) {
      Index2dC at = it.Index();
      RealT dist = Sqrt(at.SqrEuclidDistance(v));
      RealT g = Exp(-0.5 * Sqr(dist/sigma)) / sp;
      *it = 1 - g * mod;
      //cerr << "Index=" << at << " Val=" << *it << "\n";
    }
    filterSetup = Tuple3C<FFT2dC,FFT2dC,ImageC<RealT> > (fwd, bkw, filter);
    //cerr<<"inserted, cache size is now :"<<cache.Size()<<endl;
    return true;
  }
  
  //: Initialise a filter.
  bool HomomorphicFilterBodyC::UseFilter(const Index2dC &size,Tuple3C<FFT2dC,FFT2dC, ImageC<RealT> > &filter) {
    // Take a read lock while we check the cache.
    RWLockHoldC lock(m_lock, RWLOCK_READONLY);
    if(m_cache.Lookup(size,filter))
      return true; // Found it!
    // We need to build a filter, so we need to change to a write lock.
    lock.Unlock();
    // Build the required filter. We do this without a lock to avoid interfering with
    // other threads.
    if(!InitFilter(size,filter))
      return false;
    // We're ready to update.
    lock.LockWr();
    // Its possible another thread may have added the filter we just computed, so
    // check before proceeding.
    if(m_cache.Lookup(size,filter))
      return true;
    // Add the new filter.
    m_cache.Insert(size,filter);
    return true;
  }


  //: Apply filter to image.
  ImageC<float> HomomorphicFilterBodyC::Apply(const ImageC<float> &img) {
    Index2dC size(img.Frame().Range1().Size(),img.Frame().Range2().Size());
    Tuple3C<FFT2dC,FFT2dC, ImageC<RealT> > filterSet;
    if(!UseFilter(size,filterSet)) {
      // We failed to generate a filter !?
      return ImageC<float>();
    }
    // FIXME:- Do more of this in the float domain.
    ImageC<RealT> work(img.Frame());
    for(Array2dIter2C<RealT,float> it(work,img);it;it++)
      it.Data1() = Log(it.Data2() + 1);
    SArray2dC<RealT> sret(work.SArray2d(true));
    SArray2dC<ComplexC> fdom = filterSet.Data1().Apply(sret);
    for(Array2dIter2C<ComplexC,RealT> fit(fdom,filterSet.Data3());fit;fit++)
      fit.Data1() *= fit.Data2();
    SArray2dC<ComplexC> result = filterSet.Data2().Apply(fdom);
    ImageC<float> retf(img.Frame());
    for(Array2dIter2C<ComplexC,float> rit(result,retf);rit;rit++)
      rit.Data2() = Exp(rit.Data1().Mag()) - 1;
    return retf;
  }

  
  ImageC<RealT> HomomorphicFilterBodyC::Apply(const ImageC<RealT> &img) {
    ImageC<RealT> ret(img.Frame());
    Index2dC size(img.Frame().Range1().Size(),img.Frame().Range2().Size());
    Tuple3C<FFT2dC,FFT2dC, ImageC<RealT> > filterSet;
    if(!UseFilter(size,filterSet)) {
      // We failed to generate a filter !?
      return ImageC<RealT>();
    }
    //cerr<<"Homo::Apply initialised- sigma:"<<Sigma()<<" depth:"<<Depth()<<" size:"<<filterSet.Data3().Frame()<<endl;
    for(Array2dIter2C<RealT,RealT> it(ret,img);it;it++)
      it.Data1() = Log(it.Data2() + 1);
    SArray2dC<RealT> sret(ret.SArray2d(true));
    SArray2dC<ComplexC> fdom = filterSet.Data1().Apply(sret);
    for(Array2dIter2C<ComplexC,RealT> fit(fdom,filterSet.Data3());fit;fit++)
      fit.Data1() *= fit.Data2();
    SArray2dC<ComplexC> result = filterSet.Data2().Apply(fdom);
    for(Array2dIter2C<ComplexC,RealT> rit(result,ret);rit;rit++)
      rit.Data2() = Exp(rit.Data1().Mag()) - 1;
    return ret;
  }

  std::ostream &operator<<(std::ostream &s,const HomomorphicFilterC &homom) {
    UIntT v = 0; // stream version no.
    s << v << ' ' << homom.Sigma() << ' ' << homom.Depth();
    return s;
  }
  //: Write to a stream.

  std::istream &operator>>(std::istream &s,HomomorphicFilterC &homom) {
    UIntT v;
    RealT sigma,depth;
    s >> v;
    if(v > 0)
      throw ExceptionOutOfRangeC("Bad version number in stream. ");
    s >> sigma >> depth;
    homom = HomomorphicFilterC(sigma,depth);
    return s;
  }
  //: Read from a stream.
  
  BinOStreamC &operator<<(BinOStreamC &s,const HomomorphicFilterC &homom) {
    UIntT v = 0; // stream version no.
    s << v << homom.Sigma() << homom.Depth();
    return s;
  }
  //: Write to a binary stream.
  
  BinIStreamC &operator>>(BinIStreamC &s,HomomorphicFilterC &homom) {
    UIntT v;
    RealT sigma,depth;
    s >> v;
    if(v > 0)
      throw ExceptionOutOfRangeC("Bad version number in stream. ");
    s >> sigma >> depth;
    homom = HomomorphicFilterC(sigma,depth);
    return s;
  }
  //: Read from a binary stream.


}
