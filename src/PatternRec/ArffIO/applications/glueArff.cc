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
#include "Ravl/DLIter.hh"


using namespace RavlN;
using namespace RavlN;

int main(int Argc, char **Argv)
{
  OptionC option(Argc, Argv);

  DListC<StringC> inNames = option.List("i", "", "Filenames to glue together");
  FilenameC outName = option.String("o", "", "Filename of the arff file");

  option.Check();

  try
  {
    DateC startTime(true);

    SampleC<VectorC> vecs;
    SampleC<SByteT> labs;
    if(inNames.Size() > 0)
    {
      //load arff
      for(DLIterC<StringC> it(inNames); it; it++)
      {
        SampleC<VectorC> vecs_l;
        SampleC<SByteT> labs_l;
        LoadArff(*it, vecs_l, labs_l);
        vecs.Append(vecs_l);
        labs.Append(labs_l);
      }
    }
    else
    {
      throw ExceptionC("No input data");
    }

    int num1 = 0, num2 = 0;
    for(SampleIterC<SByteT> itLab(labs); itLab; itLab++)
    {
      (*itLab > 0 ? num1 : num2)++;
    }
    cout << "Objects of c1:" << num1 << endl;
    cout << "Objects of c2:" << num2 << endl;

    cout << "Saving file\n";

    SaveArff(outName, vecs, labs);

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


