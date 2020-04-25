// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLLOGIC_NLPTYPES_HEADER
#define RAVLLOGIC_NLPTYPES_HEADER 1
///////////////////////////////////////////////////////
//! author="Charles Galambos"
//! date="16/12/1996"
//! docentry="Ravl.API.Logic.Planning"
//! userlevel=Develop
//! file="Ravl/Logic/NonLinearPlanner/NLPTypes.hh"
//! lib=RavlLogicNLP

#include "Ravl/DList.hh"
#include "Ravl/BDAGraph.hh"
#include "Ravl/Logic/NLPCausalLink.hh"
#include "Ravl/Logic/NLPStep.hh"
#include "Ravl/IntC.hh"
#include "Ravl/BGraphNode.hh"

namespace RavlLogicN {
  using namespace RavlN;
  
  class NLPAgendaOpenGoalC;
  class NLPAgendaThreatC;
  class NLPCausalLinkC;
  class NLPAgendaC;
  class NLPAgendaItemC;
  class NLPActionC;
  class NLPStepInfC;
  class VLNonLinearPlanC;
  
  //typedef VLPlanLinearC<LiteralC,StateC,MinTermC> VLPlanLinearStepC;
  typedef UIntC StepListIDC;
  
  typedef BGraphNodeHC<NLPStepC,NLPCausalLinkC> NLPStepNodeT;
  typedef BGraphEdgeHC<NLPStepC,NLPCausalLinkC> NLPCausalLinkT;
  typedef BGraphEdgeHC<NLPStepC,NLPCausalLinkC> NLPOrderLinkT;
  typedef BGraphAdjIterC<NLPStepC,NLPCausalLinkC> NLPAdjLinkT;
  typedef BDAGraphC<NLPStepC,NLPCausalLinkC> NLPPlanT;
  
  typedef IntT PriorityT;  
}


#endif
