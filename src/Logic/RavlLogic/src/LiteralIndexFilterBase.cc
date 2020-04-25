// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlLogic
//! file="Ravl/Logic/Index/LiteralIndexFilterBase.cc"

#include "Ravl/Logic/LiteralIndexFilterBase.hh"
#include "Ravl/Logic/Unify.hh"
#include "Ravl/Logic/LiteralIndexIter.hh"
#include "Ravl/Logic/LiteralIndexLeafIter.hh"
#include "Ravl/Logic/LiteralIndexLeafVarIter.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlLogicN {

  //: Construct a new filter.
  
  LiteralIndexFilterBaseBodyC::LiteralIndexFilterBaseBodyC(const LiteralIndexBaseC &nindex,const LiteralC &nfilter) 
    : binds(true),
      filter(nfilter),
      index(nindex)
  { 
    initialMark = binds.Mark();
    ONDEBUG(cerr << "LiteralIndexFilterBaseBodyC::LiteralIndexFilterBaseBodyC(), @" << ((void *) this) << "  Called. Filter [" << nfilter << "]\n");
    First(); 
    ONDEBUG(cerr << "LiteralIndexFilterBaseBodyC::LiteralIndexFilterBaseBodyC(), @" << ((void *) this) << " Complete. First=" << IsElm() << "\n"); 
  }
  
  //: Construct a new filter with variable bindings.
  
  LiteralIndexFilterBaseBodyC::LiteralIndexFilterBaseBodyC(const LiteralIndexBaseC &nindex,const LiteralC &nfilter,BindSetC &nbinds)
    : binds(nbinds),
      filter(nfilter),
      index(nindex)
  { 
    initialMark = binds.Mark();
    ONDEBUG(cerr << "LiteralIndexFilterBaseBodyC::LiteralIndexFilterBaseBodyC(), Called. Filter [" << nfilter << "]\n");
    First(); 
    ONDEBUG(cerr << "LiteralIndexFilterBaseBodyC::LiteralIndexFilterBaseBodyC(), First=" << IsElm() << "\n"); 
  }


  //: Goto first data element.
  // returns true if next element is valid.
  
  bool LiteralIndexFilterBaseBodyC::First() {
    RavlAssert(filter.IsValid());
    RavlAssert(index.IsValid());
    stack.Empty();
    binds.Undo(initialMark);
    ONDEBUG(cerr << "LiteralIndexFilterBaseBodyC::First(), Filter=" << filter <<" IsGrounded=" << filter.IsGrounded() << "\n");
    IntT bindLoop = 0;
    
    // Filtering with a simple variable ?
    while(filter.IsVariable()) {
      LiteralC newUn;
      // See if filter is bound to anything.
      if(!binds.Resolve(filter,newUn) || bindLoop > 1000) {
	if(bindLoop > 1000) {
	  // FIXME:- Check more intelligently for a loop. with a set of visited variables ?
	  // This shouldn't happen, but issue a warning and make at a guess at what we should do.
	  cerr << "LiteralIndexFilterBaseBodyC::First(), ERROR: Probably a bind loop detected. Iterating all values. \n";
	  RavlAssert(0); // In debug mode flag a problem.
	}
	// We're filtering on a unbound variable.
	ONDEBUG(cerr << "LiteralIndexFilterBaseBodyC::First(), Filter on var. \n");
	LiteralIndexLeafIterC it(index); // Just iterate through the index.
	stack.Push(LiteralIndexFilterChoicePointC(index.Body().root,
                                                  binds.Mark(),
                                                  filter,
                                                  (LiteralMapIterC<LiteralIndexElementC> &) it)
                   );
	if(it) {
	  at = it.MappedData();
	  ONDEBUG(cerr << "LiteralIndexFilterBaseBodyC::First(), Filter on var, solutions found. Data='" << Data().Key().Name() << "'\n");
	  return true;
	}
	ONDEBUG(cerr << "LiteralIndexFilterBaseBodyC::First(), Filter on var, no solutions. \n");
	return false; // No values to iteratate through!
      }
      filter = newUn; // Replace filter with bound value.
      bindLoop++; // Increment loop detection counter.
    }
    
    // Check if the operation is trivial.
    if(filter.IsGrounded()) {
      if(index.IsGrounded()) {
        LiteralIndexLeafC *ptr = index.Lookup(filter);
	at = ptr != 0 ? *ptr : LiteralIndexLeafC(); 
	ONDEBUG(cerr << "LiteralIndexFilterBaseBodyC::First(), Grounded filter [" << filter << "], just doing a lookup. Data=" << (Data().IsValid() ? Data().Key().Name() : "NULL") << " \n");
	return at.IsValid();
      }
      if(!filter.IsTuple()) {
	// We need to go through all simple variables, plus the exact match.
        LiteralIndexLeafC *leafPtr = index.Lookup(filter);
	LiteralMapIterC<LiteralIndexElementC> newit;
	if(leafPtr != 0 && leafPtr->IsValid()) // Is there an exact match ?
	  newit = LiteralIndexLeafVarIterC(index,filter,*leafPtr); // Just iterate through the index.
	else
	  newit = LiteralIndexLeafIterC(index,true); // Just iterate through the index var's.
	stack.Push(LiteralIndexFilterChoicePointC(index.Body().root,binds.Mark(),filter,newit));
	if(newit) {
	  at = newit.MappedData();
	  ONDEBUG(cerr << "LiteralIndexFilterBaseBodyC::First(), Filter on lit, solutions found. Data='" << Data().Key().Name() << "'\n");
	  return true;
	}
	ONDEBUG(cerr << "LiteralIndexFilterBaseBodyC::First(), Filter on lit, no solutions. \n");
	return false; // No values to iteratate through!
      }
    }
    // Setup for Next().
    ONDEBUG(cerr << "LiteralIndexFilterBaseBodyC::First(), Setup filter on [" << filter << "] \n");
    // Find first valid solution.
    if(ExploreMatches(index.Body().root)) {
      ONDEBUG(cerr << "LiteralIndexFilterBaseBodyC::First(), Setup filter on [" << filter << "] Data='" << Data().Key().Name() << "'\n");
      
      return true;
    }
    return Next();
  }
  
  
  //: Expore matches.
  
  bool LiteralIndexFilterBaseBodyC::ExploreMatches(LiteralIndexElementC place) {
    // Follow up on rest of args.
    LiteralIndexElementC next; 
    LiteralMapIterC<LiteralIndexElementC> filterIter;
    LiteralC nextvar; // Holder for next variable to bind if there is one.
    ONDEBUG(UIntT loopDetect = 0);
    while(place.IsValid()) {
#if DODEBUG      
      if(loopDetect++ > 1000) {
        index.Dump(std::cerr);
        RavlAssert(0);
      }
#endif
      ONDEBUG(cerr << "LiteralIndexFilterBaseBodyC::ExploreMatches(), Filtering. @" << (void *) this << " Place=@" << place.Hash() << "\n");
      BindMarkT mark = binds.Mark();
      filterIter = place.Filter(filter,
                                next,
                                binds,
                                nextvar);
      
      if(filterIter.IsValid() && filterIter.IsElm()) { // We have a valid iterator to data ?
        ONDEBUG(cerr << "LiteralIndexFilterBaseBodyC::ExploreMatches(), Pushing onto stack @" << (void *) this << " Stack Depth=" << stack.Size() << "\n");
        stack.Push(LiteralIndexFilterChoicePointC(place,
                                                  mark,
                                                  nextvar,
                                                  filterIter));
        
        place = filterIter.MappedData();
        RavlAssert(nextvar.IsValid());
        if(filterIter.Data().IsVariable())
          binds.Bind(filterIter.Data(),nextvar);
        else {
          if(nextvar.IsVariable())
            binds.Bind(nextvar,filterIter.Data());
        }
        //next.Invalidate();
        continue;
      }
      
      if(!next.IsValid()) {
	// Either this is a deadend or we've found a solution.
	at = place;
	if(at.IsValid()) {
	  ONDEBUG(cerr << "LiteralIndexFilterBaseBodyC::ExploreMatches(), Checking match between " << filter << " and " << at.Key() << " \n");
	  // We've got a leaf!
	  if(Unify(filter,at.Key(),binds)) {
	    ONDEBUG(cerr << "LiteralIndexFilterBaseBodyC::ExploreMatches(), Found match! \n");
	    return true; 
	  }
	}
      }
      
      RavlAssert(place != next);
      place = next;
    }
    ONDEBUG(cerr << "LiteralIndexFilterBaseBodyC::ExploreMatches(), Out of choices, have to backtrack. \n");
    return false;
  }
  

  //: Goto next data element.
  // returns true if next element is valid.
  
  bool LiteralIndexFilterBaseBodyC::Next() {
    ONDEBUG(cerr << "LiteralIndexFilterBaseBodyC::Next(), Called. Filter : '" << filter << "' \n");
    while(!stack.IsEmpty()) {
      // Back track.
      binds.Undo(stack.Top().BindMark());
      if(!stack.Top().Iter().IsValid()) { // Recursed back to the initial point ?
	stack.DelTop(); // Empty stack.
	break; // Leave.
      }
      if(!stack.Top().Iter().Next()) {
	stack.DelTop(); // Backup some more.
	continue;
      }
      // Bind new value to var. (Var maybe null for the root node.)
      RavlAssert(stack.Top().Iter()); // Next should have returns false if this isn't true.
      LiteralC &var =stack.Top().Var();
      if(var.IsValid()) {
	ONDEBUG(cerr << "LiteralIndexFilterBaseBodyC::Next(), Binding " << var << " to " << stack.Top().Iter().Data() << " \n");
	if(var.IsVariable()) {
	  binds.Bind(var,stack.Top().Iter().Data());
	} else {
	  if(stack.Top().Iter().Data().IsVariable())	    
	    binds.Bind(stack.Top().Iter().Data(),var);
	}
      }
      if(ExploreMatches(stack.Top().Iter().MappedData())) {
	ONDEBUG(cerr << "LiteralIndexFilterBaseBodyC::Next(), Return= '" << Data().Key().Name() << "'\n");
	return true;
      }
    }
    ONDEBUG(cerr << "LiteralIndexFilterBaseBodyC::Next(), Failed to find solution. \n");
    at.Invalidate();
    return false;
  }
  
  
}
