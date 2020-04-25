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

#include "Ravl/PatternRec/ArffIO.hh"
#include "Ravl/PatternRec/SampleIter.hh"

#include <vector>
#include <algorithm>
#include <cstdio>

struct ScoreC
{
  ScoreC(float Score, char Label) : score(Score), label(Label)
    {}

  float score;
  char label;
};

bool CompareScores(const ScoreC& V1, const ScoreC& V2)
{
  return V1.score < V2.score;
}

class ScoresC
{
public:
  ScoresC()
    { numPos = numNeg = 0; sorted = false; }

  void Add(float Score, char Label)
    { scores.push_back(ScoreC(Score, Label)); sorted = false;
      (Label > 0 ? numPos : numNeg)++; }

  float GetTER();

  bool sorted;
  std::vector<ScoreC> scores;
  unsigned int numPos, numNeg;
};

float ScoresC::GetTER()
{
  if(!sorted)
    std::sort(scores.begin(), scores.end(), CompareScores);

  unsigned int numFA = numNeg, numFR = 0;

  for(std::vector<ScoreC>::iterator it = scores.begin(); it != scores.end(); ++it)
  {
    if(it->label > 0)
    {
      numFR++;
    }
    else
    {
      numFA--;
    }

    if(float(numFA) / float(numNeg) <= float(numFR) / float(numPos))
    {
      printf("threshold:%g\n", it->score);
      break;
    }
  }
  return float(numFA) / float(numNeg) + float(numFR) / float(numPos);
}

using namespace RavlN;
using namespace RavlN;

int main(int Argc, char **Argv)
{
  OptionC option(Argc, Argv);

  FilenameC arffName = option.String("i", "", "Filename of the arff file");
  FilenameC clsName = option.String("c", "", "Classifier name");
  StringC trimFeatures   = option.String("trFeat", "", "Features to use (0 - do not use, 1 - use)");
  StringC bfname = option.String("bf", "", "Binary file");

  //FilenameC resName = option.String("r", "", "Resulting file");

  option.Check();

  try
  {
    DateC startTime(true);

    //load classifier
    Classifier2C classifier;
    {
      IStreamC inCls(clsName);
      if(!inCls)
      {
        cerr << "Failed to open file:" << clsName << endl;
        throw ExceptionC("Can't open classifier file.");
      }
      inCls >> classifier;
    }
    //cout << classifier << endl;

    SampleC<VectorC> vecs;
    SampleC<SByteT> labs;
    if(!arffName.IsEmpty())
    {
      //load arff
      LoadArff(arffName, vecs, labs);
      if(!bfname.IsEmpty())
      {
        cout << "Saving data to binary file\n";
        //BinOStreamC bf(bfname);
        //bf << vecs << labs;
        FILE *f = fopen(bfname.chars(), "wb");
        if(f == NULL)
          throw ExceptionC("Can't open binary file");

        int size = vecs.Size();
        fwrite(&size, sizeof(int), 1, f);

        if(size != 0)
        {
          int numFeatures = vecs[0].Size();
          fwrite(&numFeatures, sizeof(int), 1, f);
          SArray1dC<float> buf(numFeatures);


          SampleIterC<SByteT> itLab(labs);
          int i = 0;
          for(SampleIterC<VectorC> itVec(vecs); itLab && itVec; itLab++, itVec++, i++)
          {
            const RealT* srcPtr = itVec->ReferenceElm();
            float* dstPtr = buf.ReferenceElm();
            float* endDstPtr = dstPtr + numFeatures;
            while(dstPtr < endDstPtr)
            {
              *dstPtr++ = float(*srcPtr++);
            }

            fwrite(buf.ReferenceElm(), sizeof(float), numFeatures, f);
            fputc(*itLab, f);
            if((i % 1000) == 0)
              printf("%i   \r", i);
          }
        }

        fclose(f);
        cout << "done\n";
      }
    }
    else
    {
      if(!bfname.IsEmpty())
      {
        cout << "Reading data from binary file\n";
        //BinIStreamC bf(bfname);
        //bf >> vecs;
        //bf >> labs;
        FILE *f = fopen(bfname.chars(), "rb");
        if(f == NULL)
          throw ExceptionC("Can't open binary file");

        int size;
        fread(&size, sizeof(int), 1, f);
        cout << "size:" << size << endl;

        if(size != 0)
        {
          int numFeatures;
          fread(&numFeatures, sizeof(int), 1, f);
          cout << "numFeatures:" << numFeatures << endl;
          SArray1dC<float> buf(numFeatures);

          for(int i = 0; i < size; i++)
          {
            fread(buf.ReferenceElm(), sizeof(float), numFeatures, f);
            SByteT lab = SByteT(fgetc(f));

            VectorC newVec(numFeatures);
            const float* srcPtr = buf.ReferenceElm();
            RealT* dstPtr = newVec.ReferenceElm();
            RealT* endDstPtr = dstPtr + numFeatures;
            while(dstPtr < endDstPtr)
            {
              *dstPtr++ = RealT(*srcPtr++);
            }

            vecs.Append(newVec);
            labs.Append(lab);
            if((i % 1000) == 0)
              printf("%i   \r", i);
          }
        }

        fclose(f);
        cout << "done\n";
      }
    }

    if(!trimFeatures.IsEmpty())
    {
      cout << "Trimming features\n";
      TrimFeatures(vecs, trimFeatures.chars());
    }

    int num1 = 0, num2 = 0;
    int numErr1 = 0, numErr2 = 0;
    int numClassified1 = 0, numClassified2 = 0;
    int numZeros = 0;

    ScoresC scores;

    SampleIterC<SByteT> itLab(labs);
    int i = 0;
    for(SampleIterC<VectorC> itVec(vecs); itLab && itVec; itLab++, itVec++, i++)
    {
      if((i % 100) == 0)
        printf("%i   \r", i);

      const RealT res = classifier.Classify2(*itVec);
      scores.Add(res, *itLab);

      if(res > 0.)
        numClassified1++;
      else if(res < 0.)
        numClassified2++;
      else
        numZeros++;

      if(*itLab > 0)
      {
        num1++;
        if(res <= 0.)
          numErr1++;
      }
      else
      {
        num2++;
        if(res >= 0.)
          numErr2++;
      }
    }

    cout << "time:" << DateC(true).Double() - startTime.Double() << endl;
    cout << "Objects of c1:" << num1 << endl;
    cout << "Objects of c2:" << num2 << endl;
    cout << "Total objects:" << num1 + num2 << endl;
    printf("Classified as c1:%i\n", numClassified1);
    printf("Classified as c2:%i\n", numClassified2);
    printf("Errors 1:%i\n", numErr1);
    printf("Errors 2:%i\n", numErr2);
    printf("Error rate1:%g\n", num1 > 0 ? float(numErr1) / float(num1) : 0);
    printf("Error rate2:%g\n", num2 > 0 ? float(numErr2) / float(num2) : 0);
    printf("Total Error rate:%g\n",
           (num1 > 0 ? float(numErr1) / float(num1) : 0) +
           (num2 > 0 ? float(numErr2) / float(num2) : 0));
    printf("Num zeros:%i\n", numZeros);
    printf("Equal total error rate: %g\n", scores.GetTER());

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


