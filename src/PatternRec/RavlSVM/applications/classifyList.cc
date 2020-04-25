// This file is used in conjunction with RAVL, Recognition And Vision Library
// Copyright (C) 2004, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU
// General Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
//! lib=RavlSVM

//! author="Alexey Kostin"

#include "Ravl/PatternRec/Classifier2.hh"

#include "Ravl/Option.hh"
#include "Ravl/OS/Date.hh"
#include "Ravl/OS/Filename.hh"

#include <cstdio>

using namespace RavlN;
using namespace RavlN;

int main(int Argc, char **Argv)
{
  OptionC option(Argc, Argv);

  FilenameC listName = option.String("l", "", "Filename of the list of objects");
  FilenameC clsName = option.String("c", "", "Classificator name");
  FilenameC resName = option.String("r", "", "Resulting file");

  option.Check();

  FILE *inList = NULL, *outList = NULL, *vectorFile = NULL;
  try
  {
    DateC startTime(true);
    cout << "Current time:" << startTime.Double() << endl;

    //load classifier
    ClassifierC classifier;
    {
      IStreamC inCls(clsName);
      if(!inCls)
      {
        cerr << "Failed to open file:" << clsName << endl;
        throw ExceptionOperationFailedC("Can't open file.");
      }
      inCls >> classifier;
    }

    inList = fopen(listName.chars(), "r");
    if(!inList)
    {
      cerr << "Failed to open file:" << listName << endl;
      throw ExceptionOperationFailedC("Can't open file.");
    }

    outList = fopen(resName.chars(), "w");
    if(!outList)
    {
      cerr << "Failed to open file:" << resName << endl;
      throw ExceptionOperationFailedC("Can't open file.");
    }

    int numFirst = 0, numSecond = 0;

    char buff[1024];
    char filename[1024];
    while(!feof(inList))
    {
      if(!fgets(buff, 1024, inList))
        break;

      //extract filename
      int len = 0;
      while(len < 1023)
      {
        char val = buff[len];
        if(val <= ' ')
          break;
        filename[len] = val;
        len++;
      }
      filename[len] = 0;

      //remove eol from buff
      while(len < 1023)
      {
        char val = buff[len];
        if(val == 0 || val == '\r' || val == '\n')
          break;
      }
      buff[len] = 0;

      //cout << "len:" << len << endl;

      //cout << "fn:" << filename << endl;

      printf("%s    \r", filename);

      //read vector
      vectorFile = fopen(filename, "r");
      if(!vectorFile)
      {
        cerr << "Failed to open file:" << filename << endl;
        throw ExceptionOperationFailedC("Can't open file.");
      }
      int vecLen;
      if(1 != fscanf(vectorFile, "%i", &vecLen))
        throw ExceptionOperationFailedC("Can't read vector file.");

      VectorC vec(vecLen);

      for(int i = 0; i < vecLen; i++)
      {
        float val;
        if(1 != fscanf(vectorFile, "%f", &val))
          throw ExceptionOperationFailedC("Can't read vector file.");
        vec[i] = val;
      }

      fclose(vectorFile);
      vectorFile = NULL;

      //cout << "vec len:" << vec.Size() << endl;

      //cout << "class:" << classifier.Classify(vec) << endl;
      int vecClass = int(classifier.Classify(vec));

      if(vecClass == 0)
      {
        numFirst++;
      }
      else
      {
        numSecond++;
      }

      fprintf(outList, "%s %i\n", buff, vecClass);
    }

    fclose(outList);
    outList = NULL;

    fclose(inList);
    inList = NULL;

    cout << "End time:" << DateC(true).Double() << endl;
    cout << "time:" << DateC(true).Double() - startTime.Double() << endl;
    cout << "Total objects:" << numFirst + numSecond << endl;
    cout << "Objects of first class:" << numFirst << endl;
    cout << "Objects of second class:" << numSecond << endl;
  }
  catch( ExceptionOperationFailedC &exc )
  {
    if(vectorFile)
    {
      fclose(vectorFile);
      vectorFile = NULL;
    }
    if(inList)
    {
      fclose(inList);
      inList = NULL;
    }
    if(outList)
    {
      fclose(outList);
      outList = NULL;
    }
    cout << "Exception!\n" << exc.Text() << endl;
  }
  catch( ... )
  {
    if(vectorFile)
    {
      fclose(vectorFile);
      vectorFile = NULL;
    }
    if(inList)
    {
      fclose(inList);
      inList = NULL;
    }
    if(outList)
    {
      fclose(outList);
      outList = NULL;
    }
    cout << "Unknown exception\n";
  }
  return 0;
}


