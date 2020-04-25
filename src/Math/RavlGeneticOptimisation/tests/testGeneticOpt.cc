// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2012, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlGeneticOptimisation


#include "Ravl/Genetic/Genome.hh"
#include "Ravl/Genetic/GenomeConst.hh"
#include "Ravl/Genetic/GenomeClass.hh"
#include "Ravl/Genetic/GenomeMeta.hh"
#include "Ravl/Genetic/GenomeList.hh"
#include "Ravl/Genetic/GenomeShare.hh"
#include "Ravl/Genetic/GeneTypeWeightedMeta.hh"
#include "Ravl/Genetic/GeneTypeFloatGauss.hh"
#include "Ravl/Genetic/GeneTypeBool.hh"
#include "Ravl/Genetic/GeneFactory.hh"
#include "Ravl/Genetic/PopulationState.hh"
#include "Ravl/Genetic/EvaluateFitness.hh"

#include "Ravl/VirtualConstructor.hh"
#include "Ravl/UnitTest.hh"
#include "Ravl/SysLog.hh"
#include "Ravl/Point2d.hh"

int testGenomeIO();
int testGeneIntIO();
int testGeneFloatIO();
int testGeneBoolIO();
int testGeneFloatGaussIO();
int testGeneClassIO();
int testGeneTypeEnumIO();
int testGeneTypeMetaIO();
int testGeneTypeWeightedMetaIO();
int testGeneTypeListIO();
int testPopulationStateIO();
int testFitnessFunctionIO();
int testGeneTypeShare();

int main(int nargs,char **argv)
{

  RavlN::SysLogOpen("testGeneticOpt",false,true,false,-1,true);
#if 1
  RAVL_RUN_TEST(testGeneIntIO());
  RAVL_RUN_TEST(testGeneFloatIO());
  RAVL_RUN_TEST(testGeneFloatGaussIO());
  RAVL_RUN_TEST(testGeneBoolIO());
  RAVL_RUN_TEST(testGeneClassIO());
  RAVL_RUN_TEST(testGeneTypeEnumIO());
  RAVL_RUN_TEST(testGeneTypeMetaIO());
  RAVL_RUN_TEST(testGeneTypeWeightedMetaIO());
  RAVL_RUN_TEST(testGeneTypeListIO());
  RAVL_RUN_TEST(testGeneTypeShare());
  RAVL_RUN_TEST(testGenomeIO());
#endif
  RAVL_RUN_TEST(testPopulationStateIO());

  RavlInfo("Test completed ok. ");
  return 0;
}

using RavlN::GeneticN::GenomeC;
using RavlN::GeneticN::GeneC;
using RavlN::GeneticN::GeneTypeC;
using RavlN::GeneticN::GeneTypeIntC;
using RavlN::GeneticN::GeneIntC;
using RavlN::GeneticN::GeneTypeBoolC;
using RavlN::GeneticN::GeneBoolC;
using RavlN::GeneticN::GeneTypeFloatC;
using RavlN::GeneticN::GeneFloatC;
using RavlN::GeneticN::GeneTypeClassC;
using RavlN::GeneticN::GeneClassC;
using RavlN::GeneticN::GeneTypeEnumC;
using RavlN::GeneticN::GeneTypeMetaC;
using RavlN::GeneticN::GeneTypeListC;
using RavlN::GeneticN::GeneListC;
using RavlN::GeneticN::GeneTypeWeightedMetaC;
using RavlN::GeneticN::GeneTypeClassShareC;
using RavlN::GeneticN::GeneClassShareC;
using RavlN::GeneticN::GeneTypeFloatGaussC;
using RavlN::IntT;

int testGeneIntIO()
{
  GeneTypeIntC::RefT geneType = new GeneTypeIntC("igloo",1,10);
  GeneTypeIntC::RefT geneTypeRL;

  if(!TestBinStreamIO(geneType,geneTypeRL))
    return __LINE__;

  RAVL_TEST_EQUALS(geneType->Name(),geneTypeRL->Name());
  RAVL_TEST_EQUALS(geneType->Min(),geneTypeRL->Min());
  RAVL_TEST_EQUALS(geneType->Max(),geneTypeRL->Max());

  GeneIntC::RefT gene = new GeneIntC(*geneType,5);
  GeneIntC::RefT geneRL;

  if(!TestBinStreamIO(gene,geneRL))
    return __LINE__;

  RAVL_TEST_EQUALS(gene->Value(),geneRL->Value());
  RAVL_TEST_EQUALS(gene->Type().Name(),geneType->Name());

  return 0;
}

int testGeneBoolIO()
{
  GeneTypeBoolC::RefT geneType = new GeneTypeBoolC("igloo",0.76);
  GeneTypeBoolC::RefT geneTypeRL;

  if(!TestBinStreamIO(geneType,geneTypeRL))
    return __LINE__;

  RAVL_TEST_EQUALS(geneType->Name(),geneTypeRL->Name());
  RAVL_TEST_EQUALS(geneType->Bias(),geneTypeRL->Bias());

  GeneBoolC::RefT gene = new GeneBoolC(*geneType,true);
  GeneBoolC::RefT geneRL;

  if(!TestBinStreamIO(gene,geneRL))
    return __LINE__;

  RAVL_TEST_EQUALS(gene->Value(),geneRL->Value());
  RAVL_TEST_EQUALS(gene->Type().Name(),geneType->Name());

  return 0;
}

int testGeneFloatIO()
{
  GeneTypeFloatC::RefT geneType = new GeneTypeFloatC("bannana",1.0,10.0);
  GeneTypeFloatC::RefT geneTypeRL;

  if(!TestBinStreamIO(geneType,geneTypeRL))
    return __LINE__;

  RAVL_TEST_EQUALS(geneType->Name(),geneTypeRL->Name());
  RAVL_TEST_EQUALS(geneType->Min(),geneTypeRL->Min());
  RAVL_TEST_EQUALS(geneType->Max(),geneTypeRL->Max());

  GeneFloatC::RefT gene = new GeneFloatC(*geneType,5.0);
  GeneFloatC::RefT geneRL;

  if(!TestBinStreamIO(gene,geneRL))
    return __LINE__;

  RAVL_TEST_EQUALS(gene->Value(),geneRL->Value());
  RAVL_TEST_EQUALS(gene->Type().Name(),geneType->Name());

  return 0;
}

int testGeneFloatGaussIO() {
  GeneTypeFloatGaussC::RefT geneType = new GeneTypeFloatGaussC("flup",-21,32,5,1,RavlN::GeneticN::GeneTypeFloatGaussC::FoldUp);
  GeneTypeFloatGaussC::RefT geneTypeRL;

  if(!TestBinStreamIO(geneType,geneTypeRL))
    return __LINE__;

  RAVL_TEST_EQUALS(geneType->FoldMode(),geneTypeRL->FoldMode());
  RAVL_TEST_ALMOST_EQUALS(geneType->Width(),geneTypeRL->Width(),1e-6);
  RAVL_TEST_ALMOST_EQUALS(geneType->Offset(),geneTypeRL->Offset(),1e-6);

  return 0;
}

int testGeneClassIO()
{
  GeneTypeClassC::RefT geneType = new GeneTypeClassC(typeid(RavlN::GeneticN::GeneFactoryC));
  GeneTypeClassC::RefT geneTypeRL;

  GeneTypeIntC::RefT geneTypeVal = new GeneTypeIntC("igloo",1,10);
  GeneTypeIntC::RefT geneTypeVal2 = new GeneTypeIntC("flood",1,10);
  geneType->AddComponent("hello",*geneTypeVal);
  geneType->AddComponent("fellow",*geneTypeVal);
  geneType->AddComponent("another",*geneTypeVal2);

  if(!TestBinStreamIO(geneType,geneTypeRL))
    return __LINE__;

  if(geneType->TypeInfo() != geneTypeRL->TypeInfo())
    return __LINE__;

  RAVL_TEST_EQUALS(geneType->TypeName(),geneTypeRL->TypeName());
  RAVL_TEST_EQUALS(std::string(geneType->TypeInfo().name()),std::string(geneTypeRL->TypeInfo().name()));

  GeneTypeC::ConstRefT geneTypeValRL1;
  GeneTypeC::ConstRefT geneTypeValRL2;
  GeneTypeC::ConstRefT geneTypeValRL3;
  RAVL_TEST_TRUE(geneType->LookupComponent("hello",geneTypeValRL1));
  RAVL_TEST_TRUE(geneType->LookupComponent("fellow",geneTypeValRL2));
  RAVL_TEST_TRUE(geneType->LookupComponent("another",geneTypeValRL3));
  RAVL_TEST_EQUALS(geneTypeValRL1,geneTypeValRL2);
  RAVL_TEST_NOT_EQUALS(geneTypeValRL2,geneTypeValRL3);

  GeneTypeIntC::RefT geneTypeIntValRL = dynamic_cast<const GeneTypeIntC *>(geneTypeValRL1.BodyPtr());
  RAVL_TEST_TRUE(geneTypeIntValRL.IsValid());
  RAVL_TEST_EQUALS(geneTypeIntValRL->Name(),geneTypeVal->Name());

  GeneTypeIntC::RefT geneTypeIntValRL3 = dynamic_cast<const GeneTypeIntC *>(geneTypeValRL3.BodyPtr());
  RAVL_TEST_TRUE(geneTypeIntValRL3.IsValid());
  RAVL_TEST_EQUALS(geneTypeIntValRL3->Name(),geneTypeVal2->Name());

  GeneClassC::RefT gene = new GeneClassC(*geneType);
  GeneClassC::RefT geneRL;

  GeneIntC::RefT geneInt1 = new GeneIntC(*geneTypeVal,1);
  GeneIntC::RefT geneInt2 = new GeneIntC(*geneTypeVal,2);
  GeneIntC::RefT geneInt3 = new GeneIntC(*geneTypeVal2,3);
  gene->SetComponent("hello",*geneInt1);
  gene->SetComponent("fellow",*geneInt2);
  gene->SetComponent("another",*geneInt3);


  if(!TestBinStreamIO(gene,geneRL))
    return __LINE__;

  RAVL_TEST_EQUALS(gene->Type().Name(),geneType->Name());

  GeneC::ConstRefT geneRL1;
  GeneC::ConstRefT geneRL2;
  GeneC::ConstRefT geneRL3;
  RAVL_TEST_TRUE(gene->GetComponent("hello",geneRL1));
  RAVL_TEST_TRUE(gene->GetComponent("fellow",geneRL2));
  RAVL_TEST_TRUE(gene->GetComponent("another",geneRL3));

  GeneIntC::ConstRefT geneIntRL1 = dynamic_cast<const GeneIntC *>(geneRL1.BodyPtr());
  GeneIntC::ConstRefT geneIntRL2 = dynamic_cast<const GeneIntC *>(geneRL2.BodyPtr());
  GeneIntC::ConstRefT geneIntRL3 = dynamic_cast<const GeneIntC *>(geneRL3.BodyPtr());

  RAVL_TEST_TRUE(geneIntRL1.IsValid());
  RAVL_TEST_TRUE(geneIntRL2.IsValid());
  RAVL_TEST_TRUE(geneIntRL3.IsValid());

  RAVL_TEST_EQUALS(geneIntRL1->Value(),geneInt1->Value());
  RAVL_TEST_EQUALS(geneIntRL2->Value(),geneInt2->Value());
  RAVL_TEST_EQUALS(geneIntRL3->Value(),geneInt3->Value());

  return 0;
}

int testGeneTypeEnumIO()
{
  GeneTypeIntC::RefT geneType = new GeneTypeIntC("igloo",1,10);
  GeneIntC::RefT gene = new GeneIntC(*geneType,5);

  std::vector<GeneC::ConstRefT> values;
  values.push_back(gene.BodyPtr());

  GeneTypeEnumC::RefT geneTypeEnum = new GeneTypeEnumC("test",values);
  GeneTypeEnumC::RefT geneTypeEnumRL;

  if(!TestBinStreamIO(geneTypeEnum,geneTypeEnumRL))
    return __LINE__;

  RAVL_TEST_EQUALS(geneTypeEnum->Name(),geneTypeEnumRL->Name());
  RAVL_TEST_EQUALS(geneTypeEnum->Values().size(),geneTypeEnumRL->Values().size());
  RAVL_TEST_EQUALS(geneTypeEnum->Values()[0]->Name(),geneTypeEnumRL->Values()[0]->Name());

  return 0;
}

int testGeneTypeMetaIO()
{
  GeneTypeIntC::RefT geneTypeInt = new GeneTypeIntC("igloo",1,10);
  GeneTypeFloatC::RefT geneTypeFloat = new GeneTypeFloatC("bannana",1.0,10.0);

  std::vector<GeneTypeC::ConstRefT> types;
  types.push_back(geneTypeInt.BodyPtr());
  types.push_back(geneTypeFloat.BodyPtr());

  GeneTypeMetaC::RefT geneTypeMeta = new GeneTypeMetaC("groovy",types);
  GeneTypeMetaC::RefT geneTypeMetaRL;

  if(!TestBinStreamIO(geneTypeMeta,geneTypeMetaRL))
    return __LINE__;

  RAVL_TEST_EQUALS(geneTypeMeta->Name(),geneTypeMetaRL->Name());

  return 0;
}

int testGeneTypeWeightedMetaIO()
{
  std::vector<GeneTypeC::ConstRefT> types;
  std::vector<float> weights;

  GeneTypeWeightedMetaC::RefT geneTypeMetaE = new GeneTypeWeightedMetaC("empty",types,weights);
  GeneTypeWeightedMetaC::RefT geneTypeMetaERL;

  //! Check IO on an empty class works.
  if(!TestBinStreamIO(geneTypeMetaE,geneTypeMetaERL))
    return __LINE__;

  RAVL_TEST_EQUALS(geneTypeMetaE->Name(),geneTypeMetaERL->Name());


  GeneTypeIntC::RefT geneTypeInt = new GeneTypeIntC("igloo",1,10);
  GeneTypeFloatC::RefT geneTypeFloat = new GeneTypeFloatC("bannana",1.0,10.0);

  types.push_back(geneTypeInt.BodyPtr());
  types.push_back(geneTypeFloat.BodyPtr());
  weights.push_back(0.3);
  weights.push_back(0.7);

  GeneTypeWeightedMetaC::RefT geneTypeMeta = new GeneTypeWeightedMetaC("groovy",types,weights);
  GeneTypeWeightedMetaC::RefT geneTypeMetaRL;

  if(!TestBinStreamIO(geneTypeMeta,geneTypeMetaRL))
    return __LINE__;

  RAVL_TEST_EQUALS(geneTypeMeta->Name(),geneTypeMetaRL->Name());
  RAVL_TEST_EQUALS(geneTypeMetaRL->Weights().size(),weights.size());
  RAVL_TEST_EQUALS(geneTypeMetaRL->Types().size(),weights.size());
  RAVL_TEST_EQUALS(geneTypeMetaRL->Types()[0]->Name(),geneTypeInt->Name());
  RAVL_TEST_EQUALS(geneTypeMetaRL->Types()[1]->Name(),geneTypeFloat->Name());

  return 0;
}

static RavlN::VirtualConstructorInstC<GeneTypeListC<IntT> > vc_function_GeneTypeListC_IntT("RavlN::GeneticN::GeneTypeListC<IntT>");

int testGeneTypeListIO()
{
  GeneTypeIntC::RefT geneTypeInt = new GeneTypeIntC("igloo",1,10);
  GeneTypeListC<IntT>::RefT geneType = new GeneTypeListC<IntT>("fella",*geneTypeInt,10);
  GeneTypeListC<IntT>::RefT geneTypeRL;

  if(!TestBinStreamIO(geneType,geneTypeRL))
    return __LINE__;

  RAVL_TEST_EQUALS(geneType->Name(),geneTypeRL->Name());
  RAVL_TEST_EQUALS(geneType->MaxLength(),geneTypeRL->MaxLength());
  RAVL_TEST_EQUALS(geneType->ContentType().Name(),geneTypeRL->ContentType().Name());


  std::vector<GeneC::ConstRefT> values;

  GeneIntC::RefT geneVal1 = new GeneIntC(*geneType,3);
  GeneIntC::RefT geneVal2 = new GeneIntC(*geneType,4);
  values.push_back(geneVal1.BodyPtr());
  values.push_back(geneVal2.BodyPtr());

  GeneListC::RefT gene = new GeneListC(*geneType,values);
  GeneListC::RefT geneRL;

  if(!TestBinStreamIO(gene,geneRL))
    return __LINE__;

  RAVL_TEST_EQUALS(gene->List().size(),geneRL->List().size());
  GeneIntC::RefT geneRL1 = dynamic_cast<const GeneIntC *>(gene->List()[0].BodyPtr());
  RAVL_TEST_TRUE(geneRL1.IsValid());
  GeneIntC::RefT geneRL2 = dynamic_cast<const GeneIntC *>(gene->List()[1].BodyPtr());
  RAVL_TEST_TRUE(geneRL2.IsValid());
  RAVL_TEST_EQUALS(geneRL1->Value(),geneVal1->Value());
  RAVL_TEST_EQUALS(geneRL2->Value(),geneVal2->Value());

  return 0;
}

int testGeneTypeShare() {
  GeneTypeClassShareC::RefT geneType = new GeneTypeClassShareC(typeid(int),true);
  GeneTypeClassShareC::RefT geneTypeRL;

  if(!TestBinStreamIO(geneType,geneTypeRL))
    return __LINE__;

  RAVL_TEST_EQUALS(geneType->IsMustConnect(),geneTypeRL->IsMustConnect());

  const float vals[] = {10,20};
  RavlN::TFVectorC<float,2> at(vals);
  GeneClassShareC::RefT gene = new GeneClassShareC(*geneType,at,10.0);
  GeneClassShareC::RefT geneRL;

  if(!TestBinStreamIO(gene,geneRL))
    return __LINE__;

  RAVL_TEST_ALMOST_EQUALS(gene->Strength(),geneRL->Strength(),1e-6);
  RAVL_TEST_ALMOST_EQUALS(gene->Position()[0],gene->Position()[0],1e-6);
  RAVL_TEST_ALMOST_EQUALS(gene->Position()[1],gene->Position()[1],1e-6);
  return 0;
}

int testGenomeIO()
{
  GeneTypeIntC::RefT geneType = new GeneTypeIntC("igloo",1,10);
  GeneIntC::RefT gene = new GeneIntC(*geneType,5);
  GenomeC::RefT genome = new GenomeC(*gene);

  GenomeC::RefT genomeReconstructed;

  if(!TestBinStreamIO(genome,genomeReconstructed))
    return __LINE__;

  RAVL_TEST_EQUALS(genome->Age(),genomeReconstructed->Age());
  RAVL_TEST_EQUALS(genome->RootGene().Type().Name(),geneType->Name());

  return 0;
}

int testPopulationStateIO()
{
  using RavlN::GeneticN::PopulationStateC;
  PopulationStateC::RefT pop = new PopulationStateC();
  PopulationStateC::RefT popRL;
  if(!TestBinStreamIO(pop,popRL))
    return __LINE__;

  return 0;
}

int testFitnessFunctionIO()
{
  using RavlN::GeneticN::PopulationStateC;
  PopulationStateC::RefT pop = new PopulationStateC();
  PopulationStateC::RefT popRL;
  if(!TestBinStreamIO(pop,popRL))
    return __LINE__;

  return 0;
}
