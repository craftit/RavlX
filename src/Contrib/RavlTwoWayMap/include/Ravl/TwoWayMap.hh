/******************************************************************************
 * $Id$
 * This file is used in conjunction with RAVL, Recognition And Vision Library
 * Copyright (C) 2006, OmniPerception Ltd.
 * This code may be redistributed under the terms of the GNU Lesser
 * General Public License (LGPL). See the gpl.licence file for details or
 * see http://www.gnu.org/copyleft/lesser.html
 ******************************************************************************/
#ifndef RAVLN_FACEN_TWOWAYMAP_HH
#define RAVLN_FACEN_TWOWAYMAP_HH

#include <map>
#include <vector>

namespace RavlN {
  /**
   @author Alexey Kostin
   */

//------------------------------------------------------------------------------
  template<class Type>
  class TwoWayMapC
  {
  public:
    TwoWayMapC()
    {
    }

    //! map Val1 to Val2 return true if succeeded
    bool MapForward(const Type &Val1, unsigned int &Val2, bool Add);

    //! map Val1 to Val2 return true if succeeded
    bool MapForward(const Type &Val1, unsigned int &Val2, bool Add) const;

    //! reverse map Val2 to Val1. return true if OK
    bool MapBackward(Type &Val1, unsigned int Val2) const;

    //! size of map table
    unsigned int Size() const
    {
      return (unsigned int) backwardMap.size();
    }

    //! forward map. add to the map if not mapped already
    unsigned int Forward(const Type &Val1)
    {
      unsigned int v2;
      return MapForward(Val1, v2, true) ? v2 : -1;
    }

    //! backward map. Return Type() if not mapped value
    const Type& Backward(unsigned int Val2) const;

    //! check if Val1 is mapped
    bool IsMapped(Type &Val1) const;
  private:
    std::map<Type, unsigned int> forwardMap;
    std::vector<Type> backwardMap;
    Type emptyVal1;
  };

//------------------------------------------------------------------------------
  template<class Type>
  const Type& TwoWayMapC<Type>::Backward(unsigned int Val2) const
  {
    return (Val2 < backwardMap.size()) ? backwardMap[Val2] : emptyVal1;
  }

//------------------------------------------------------------------------------
  template<class Type>
  bool TwoWayMapC<Type>::MapForward(const Type &Val1, unsigned int &Val2, bool Add)
  {
    typename std::map<Type, unsigned int>::const_iterator forwardIt = forwardMap.find(Val1);

    if (forwardIt == forwardMap.end()) {
      if (Add) {
        //insert new pair
        Val2 = (unsigned int) backwardMap.size();
        forwardMap[Val1] = Val2;
        backwardMap.push_back(Val1);
        return true;
      } else
        return false;
    } else {
      //return existing ID
      Val2 = forwardIt->second;
      return true;
    }
  }

//------------------------------------------------------------------------------
  template<class Type>
  bool TwoWayMapC<Type>::MapForward(const Type &Val1, unsigned int &Val2, bool Add) const
  {
    typename std::map<Type, unsigned int>::const_iterator forwardIt = forwardMap.find(Val1);

    if (forwardIt == forwardMap.end()) {
      return false;
    } else {
      //return existing ID
      Val2 = forwardIt->second;
      return true;
    }
  }

//------------------------------------------------------------------------------
  template<class Type>
  bool TwoWayMapC<Type>::MapBackward(Type &Val1, unsigned int Val2) const
  {
    if (Val2 >= backwardMap.size())
      return false;
    Val1 = backwardMap[Val2];
    return true;
  }

//------------------------------------------------------------------------------
//! check if Val1 is mapped
  template<class Type>
  bool TwoWayMapC<Type>::IsMapped(Type &Val1) const
  {
    return forwardMap.find(Val1) != forwardMap.end();
  }
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

}

#endif
