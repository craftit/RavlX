// This file is used in conjunction with RAVL, Recognition And Vision Library
// Copyright (C) 2004, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU
// General Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
//! lib=ArffIO

#include "Ravl/Option.hh"
#include "Ravl/OS/Date.hh"
#include "Ravl/OS/Filename.hh"

#include "Ravl/PatternRec/ArffIO.hh"
#include "Ravl/PatternRec/SampleIter.hh"


using namespace RavlN;
using namespace RavlN;

int main(int Argc, char **Argv)
{
  OptionC option(Argc, Argv);

  FilenameC inName = option.String("i", "", "Filename of the arff file");
  FilenameC outName = option.String("o", "", "Filename of the arff file");
  IntT newNum1 = option.Int("n1", 0, "Number of objects in first class (0 - use all)");
  IntT newNum2 = option.Int("n2", 0, "Number of objects in second class (0 - use all)");

  option.Check();

  try
  {
    DateC startTime(true);

    SampleC<VectorC> vecs;
    SampleC<SByteT> labs;
    if(!inName.IsEmpty())
    {
      //load arff
      LoadArff(inName, vecs, labs);
    }
    else
    {
      throw ExceptionC("No input data");
    }

    int oldNum1 = 0, oldNum2 = 0;
    for(SampleIterC<SByteT> itLab(labs); itLab; itLab++)
    {
      (*itLab > 0 ? oldNum1 : oldNum2)++;
    }
    cout << "Objects of c1:" << oldNum1 << endl;
    cout << "Objects of c2:" << oldNum2 << endl;

    SArray1dC<VectorC*> oldVecs1(oldNum1);
    SArray1dC<VectorC*> oldVecs2(oldNum2);

    cout << "Sorting objects\n";
    {
      SampleIterC<SByteT> itLab(labs);
      int i = 0;
      int i1 = 0, i2 = 0;
      for(SampleIterC<VectorC> itVec(vecs); itLab && itVec; itLab++, itVec++, i++)
      {
        ((*itLab > 0) ? oldVecs1[i1++] : oldVecs2[i2++]) = &(*itVec);
      }
    }

    cout << "Generating new set\n";
    cout << "RAND_MAX:" << RAND_MAX << endl;
    SampleC<VectorC> newVecs;
    SampleC<SByteT> newLabs;
    if(newNum1 == 0)
    {
      for(SizeT i = 0; i < oldVecs1.Size(); i++)
      {
        newVecs.Append(*oldVecs1[i]);
        newLabs.Append(1);
      }
    }
    else
    {
      for(int i = 0; i < newNum1; i++)
      {
        int id = rand() % oldNum1;
        newVecs.Append(*oldVecs1[id]);
        newLabs.Append(1);
      }
    }
    if(newNum2 == 0)
    {
      for(SizeT i = 0; i < oldVecs2.Size(); i++)
      {
        newVecs.Append(*oldVecs2[i]);
        newLabs.Append(-1);
      }
    }
    else
    {
      for(int i = 0; i < newNum2; i++)
      {
        int id = rand() % oldNum2;
        newVecs.Append(*oldVecs2[id]);
        newLabs.Append(-1);
      }
    }

    cout << "Saving file\n";

    SaveArff(outName, newVecs, newLabs);

    cout << "time:" << DateC(true).Double() - startTime.Double() << endl;
  }
  catch(ExceptionC &exc)
  {
    cout << "Exception!\n" << exc.Text() << endl;
  }
  catch( ... )
  {
    cout << "Unknown exception\n";
  }
  return 0;
}


