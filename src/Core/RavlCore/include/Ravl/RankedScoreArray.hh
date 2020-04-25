// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2010, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_RANKEDSCOREARRAY_HEADER
#define RAVL_RANKEDSCOREARRAY_HEADER 1
//! lib=RavlCore
//! userlevel=Normal
//! author="Charles Galambos"
//! docentry="Ravl.API.Math"
//! date="11/02/2010"

#include "Ravl/StdConst.hh"
#include "Ravl/Tuple2.hh"
#include "Ravl/Assert.hh"
#include "Ravl/Stream.hh"
#include "Ravl/SArray1d.hh"
#include <math.h>

#ifdef WIN32
#include <limits>
#define INFINITY numeric_limits<float>::infinity( ) 
#endif

namespace RavlN {

  //: Keep a ranked array of results.
  // KeyT = score value, higher values preferred over smaller ones.

  template<typename KeyT,typename DataT>
  class RankedScoreArrayC
  {
  public:
    RankedScoreArrayC(SizeT n = 1, const KeyT &Threshold = KeyT(-INFINITY))
      : N(n),
        m_results(n >= 1 ? n : SizeT(1)),
        m_worseAcceptableScore(Threshold)
    {
      RavlAssert(n > 0);
      Reset();
    }
    //: Constructor.
    // n-Number of results to track.

    void SetMaxSize(SizeT n) {
      RavlAssert(n > 0);
      if(n < 1)
        n = 1;
      N = n;
      m_results = SArray1dC<Tuple2C<KeyT,DataT> >(n);
      Reset();
    }
    //: Set maximum number of entries to collect.
    // This will reset the contents of the array.

    void Reset() {
      m_used = 0;
      m_results[0].Data1() = m_worseAcceptableScore;
    }
    //: Clear array

    void Reset(const KeyT &worstScoreToAccept) {
      m_used = 0;
      m_results[0].Data1() = worstScoreToAccept;
      m_worseAcceptableScore = worstScoreToAccept;
    }
    //: Reset with with a minimum Score to accept.


    bool Include(const KeyT &score,const DataT &data) {
      if(m_used < N) // Have we filled the list yet?
        return AddFill(score,data);
      if(score <= m_results[0].Data1())
        return false;
      // Add pushing worst out.
      SizeT j = 1;
      for(;j < N && m_results[j].Data1() < score;j++)
        m_results[j-1] = m_results[j];
      m_results[j-1].Data1() = score;
      m_results[j-1].Data2() = data;
      return true;
    }
    //: Test a candidate for inclusion into the list, and add it if passes
    //: Returns true if worse candidate has been replaced.

    KeyT WorstAcceptedScore() const
    { return m_results[0].Data1(); }
    //: Access worst accepted Score.

    SizeT Size() const
    { return m_used; }
    //: Access number of entries.

    const Tuple2C<KeyT,DataT> &Result(SizeT nth) const {
      RavlAssert(nth < m_used);
      return m_results[nth];
    }
    //: Access nth results.
    // Results are ordered worst (smallest score) to best (largest).

    const Tuple2C<KeyT,DataT> &operator[](SizeT nth) const
    { return Result(nth); }
    //: Access result.

    const DataT &BestResult() const {
      RavlAssert(m_used > 0);
      return m_results[m_used-1].Data2();
    }
    //: Access best result.

    const KeyT &BestScore() const {
      RavlAssert(m_used > 0);
      return m_results[m_used-1].Data1();
    }
    //: Access best result.

    bool IsEmpty() const
    { return m_used == 0; }
    //: Test if result set is empty.
    
    void Merge(const RankedScoreArrayC<KeyT,DataT> &array) {
      for(SizeT j = 0; j < array.m_used; j++) {
        const Tuple2C<KeyT,DataT> &val = array.m_results[j];
        Include(val.Data1(), val.Data2());
      }
    }
    //: Merge another set of results with this one.

    void Dump(std::ostream &out) const {
      out << "RankedScoreArray, " << m_used << " Entries. \n";
      for(UIntT i = 0;i < m_used;i++)
        out << " " << m_results[i] << "\n";
    }
    //: Dump to stream in human readable form.
    
    const SizeT& GetMaxSize() const
    { return N; }
    //: Get maximum number of results.

    const KeyT& GetWorstAcceptableScore() const
    { return m_worseAcceptableScore; }
    //: Get the current worst

    SArray1dC<Tuple2C<KeyT,DataT> > SArray()
    { return m_results.From(0,m_used); }
    //: Get results as array.
    
  protected:
    bool AddFill(const KeyT &score,const DataT &data) {
      if(score < m_worseAcceptableScore)
        return false;
      RavlAssert(m_used < N);
      SizeT j = 0;

      // Find where to put new entry.
      for(;j < m_used && m_results[j].Data1() < score;j++);

      // Make a space for the new entry.
      for(SizeT k = m_used;k > j;k--)
        m_results[k] = m_results[k-1];

      // Put in the  new entry.
      m_results[j].Data1() = score;
      m_results[j].Data2() = data;
      m_used++;
      return j == 0;
    }
    //: Add to a partially filled list.
    //: Returns true if current worse has been replaced.

    SizeT m_used;
    SizeT N;
    SArray1dC<Tuple2C<KeyT,DataT> > m_results;
    KeyT m_worseAcceptableScore;
  };

}

#endif
