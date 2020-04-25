// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLHISTOGRAM_HEADER
#define RAVLHISTOGRAM_HEADER 1
//////////////////////////////////////////////////
//! rcsid="$Id$"
//! author="Charles Galambos"
//! example=exHistogram.cc
//! docentry="Ravl.API.Math.Statistics.Histogram"
//! lib=RavlMath
//! file="Ravl/Math/Statistics/Histogram/Histogram.hh"

#include "Ravl/StdMath.hh"
#include "Ravl/Hash.hh"
#include "Ravl/HashIter.hh"
#include "Ravl/IntC.hh"

namespace RavlN { 


  //! userlevel=Normal
  //: Create a histogram from discrete values. 
  // <p>Use the class template <code>KeyT</code> to create a histogram from any sort of discrete data type, for example <code>int, short, char, IntT, Index2dC</code> (for a 2D histogram), even <code>bool</code> .....</p>

  // <p>Note that <code>HistogramC</code> is based on <code>HashC</code>, in order to avoid problems of unknown range and data type.  If your application is time-critical, it will probably be worth while creating a class specifically for your data type.</p>

  template<class KeyT>
  class HistogramC 
    : public HashC<KeyT,UIntC>
  {
  public:
    HistogramC()
      : total(0)
      {}
    //: Default constructor.
    
    UIntT Vote(const KeyT &bin) { 
      total++; 
      return ++HashC<KeyT,UIntC>::operator[](bin); 
    }
    //: Vote for 'bin'
    // returns the count of the bin after voting.

    UIntT Vote(const KeyT &bin,UIntT noVotes) { 
      total += noVotes; 
      return HashC<KeyT,UIntC>::operator[](bin) += noVotes;
    }
    //: Vote with 'noVotes' for 'bin'
    // returns the count of the bin after voting.
    
    UIntT Unvote(const KeyT &bin) { 
      total--; 
      return --HashC<KeyT,UIntC>::operator[](bin); 
    }
    //: Unvote for 'bin'
    // returns the count of the bin after unvoting.
    
    UIntT Total() const
    { return total; }
    //: Sum of all the bins.
    
    RealT Information() const;
    //: Calculate the amount of information represented by the histogram.
    // This is also known as the entropy of the histogram.
    
    RealT Energy() const;
    //: Calculate the energy represented by the original signal.
    
    UIntT SumIntersection(const HistogramC<KeyT> &oth) const;
    //: Return the sum of the intersection between the two histograms.
    
    UIntT LargestDifference(const HistogramC<KeyT> &oth,KeyT &lKey,IntT &ldiff);
    //: Return key with largest difference between this and 'oth'.
    // Returns the number of keys with an equal difference count. <p>
    // The key is assigned to lKey, and the maximum difference to ldiff.
    
    IntT Peak(KeyT &key);
    //: Find key with largest number of votes.
    // Returns the margin to the next largest key
    // If -1 is returned no peak was found, the table was empty or
    // all count were zero.
    
    UIntT Size() const
    { return HashC<KeyT,UIntC>::Size(); }
    //: Number of key's in the histogram.
    // Note: This is NOT the total number of votes.
  protected:
    UIntT total; // Total of count in all bins

     template<class K> friend
    istream &operator>>(istream &in,HistogramC<K> &obj);
  };
  
  template<class KeyT>
  RealT HistogramC<KeyT>::Information() const {
    RealT totalp = 0;
    for(HashIterC<KeyT,UIntC> it(*this);it;it++) {
      RealT prob = (RealT) it.Data() / total;
      totalp += -prob * Log2(prob);
    }
    return totalp;
  }
  
  template<class KeyT>
  RealT HistogramC<KeyT>::Energy() const {
    RealT sum = 0;
    for(HashIterC<KeyT,UIntC> it(*this);it;it++)
      sum += Pow((RealT) it.Data() / total,2);
    return sum;
  }
  
  template<class KeyT>
  UIntT HistogramC<KeyT>::SumIntersection(const HistogramC<KeyT> &oth) const {
    UIntT ret = 0;
    if(Size() > oth.Size())
      return oth.SumIntersection(*this); // Turn it around, its faster.
    UIntC v;
    for(HashIterC<KeyT,UIntC> it(*this);it;it++) {
      if(!oth.Lookup(it.Key(),v))
	continue;
      ret += Min(v,it.Data());
    }
    return ret;
  }
  
  template<class KeyT>
  UIntT HistogramC<KeyT>::LargestDifference(const HistogramC<KeyT> &oth,KeyT &lKey,IntT &ldiff) {
    IntT maxDiff = -1;
    UIntT matchCount =0;
    for(HashIterC<KeyT,UIntC> it1(*this);it1;it1++) {
      UIntC oc = 0;
      oth.Lookup(it1.Key(),oc); // If not found oc will be 0
      IntT diff = Abs((IntT) oc - (IntT) it1.Data());
      if(diff < maxDiff)
	continue;
      if(diff == maxDiff) {
	matchCount++;
	continue;
      }
      matchCount = 1;
      maxDiff = diff;
      lKey = it1.Key();
    }
    for(HashIterC<KeyT,UIntC> it2(oth);it2;it2++) {
      UIntC oc = 0;
      if(this->Lookup(it2.Key(),oc)) 
	continue; // Only interested in elements in oth only.
      IntT diff = oc;
      if(diff < maxDiff)
	continue;
      if(diff == maxDiff) {
	matchCount++;
	continue;
      }
      matchCount = 1;
      maxDiff = diff;
      lKey = it2.Key();      
    }
    ldiff = maxDiff;
    return matchCount;
  }
  
  template<class KeyT>
  IntT HistogramC<KeyT>::Peak(KeyT &key) {
    IntT diff = -1;
    UIntT max = 0;
    for(HashIterC<KeyT,UIntC> it(*this);it;it++) {
      if(*it > max) {
	diff = *it - max;
	key = it.Key();
	max = *it ; 
      } else if(*it == max)
	diff = 0; // More than one key with this count.
    }
    return diff;
  }


  template<class KeyT>
  inline istream &operator>>(istream &in,HistogramC<KeyT> &obj) {
    HashC<KeyT,UIntC> hash(in);
    obj.Move(hash);
    obj.total = 0;
    for (HashIterC<KeyT,UIntC> i(obj); i; ++i)  obj.total += *i;
    return in;
  }

}

#endif
