// This file is used in conjunction with RAVL, Recognition And Vision Library
// Copyright (C) 2004, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU
// General Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
#include "Ravl/PatternRec/ArffIO.hh"
#include "Ravl/PatternRec/SampleIter.hh"
#include <string.h>
#include <stdio.h>
//! lib=ArffIO

namespace RavlN
{
//---------------------------------------------------------------------------
void SaveArff(const StringC& FileName, const SampleC<VectorC> &TrVec,
              const SampleC<SByteT> &TrLab)
{
  const char* classLab[2] = {"c1", "c2"};
  FILE* f = fopen(FileName.chars(), "w+");
  int numFeatures = (int)(TrVec[0].Size());
  fprintf(f, "%% Number of objects:  %i\n", (int)(TrVec.Size()));
  fprintf(f, "%% Number of features: %i\n", numFeatures);
  fprintf(f, "@relation test\n\n");


  //attributes
  fprintf(f, "@attribute class {%s,%s}\n", classLab[0], classLab[1]);
  for(int j = 0; j < (int)(TrVec[0].Size()); j++)
  {
    fprintf(f, "@attribute f%03i real\n", j);
  }

  //data
  fprintf(f, "\n@data\n");

  for(int i = 0; i < (int)(TrVec.Size()); i++)
  {
    const VectorC& vec = TrVec[i];
    if(numFeatures != (int)(vec.Size()))
      cerr << "Wrong vector size for index:" << i << endl;
    else
    {
      fprintf(f, "%s", TrLab[i] > 0 ? classLab[0] : classLab[1]);
      for(int j = 0; j < (int)(vec.Size()); j++)
      {
        fprintf(f, ",%g", vec[j]);
      }
      fprintf(f, "\n");
      if(i % 1000 == 0)
        printf("%i\r", i);
    }
  }
  fclose(f);
}

//---------------------------------------------------------------------------
void LookForAt(FILE *F, char &Buff, int &LineNo)
{
  while(1)
  {
    Buff = fgetc(F);
    //cout << "At " << Buff << endl;
    if(Buff == 10) //skip empty lines
    {
      LineNo++;
      continue;
    }
    if(Buff == 13)
      continue;
    if(Buff == '@')
      break;
    if(Buff == '%') // skip comments
    {
      while(1)
      {
        Buff = fgetc(F);
        //cout << "At c " << Buff << endl;
        if(Buff == 10)
        {
          LineNo++;
          break;
        }
        if(Buff == EOF)
          throw "EOF in reading comment";
      }
      continue;
    }
    throw "Wrong character while looking for @";
  }
}

//---------------------------------------------------------------------------
bool CheckReadWord(FILE *F, char& Buff, const char* Word)
{
  for(int i = 0; i < int(strlen(Word)); i++)
  {
    Buff = fgetc(F);
    //cout << "Cr " << Buff << endl;
    if(Buff != Word[i])
    {
      return false;
    }
  }
  return true;
}

//---------------------------------------------------------------------------
bool ReadWord(FILE *F, char& Buff, char* Word, int MaxLen, char Delim)
{
  int i = 0;
  for(; i < MaxLen - 1; i++)
  {
    Buff = fgetc(F);
    if(Buff == EOF)
      return false;
    //cout << "RW " << Buff << endl;
    if(Buff != Delim && Buff != 10)
    {
      Word[i] = Buff;
    }
    else
      break;
  }
  Word[i] = 0;
  return true;
}

//---------------------------------------------------------------------------
void LoadArff(const StringC& FileName, SampleC<VectorC> &TrVec,
              SampleC<SByteT> &TrLab)
{
  TrVec = SampleC<VectorC>();
  TrLab = SampleC<SByteT>();
  SampleC<int> attributes; //indices of attributes to be read
  int classAttribute = -1;
  const char* relationWord = "relation ";
  const char* attributeWord = "attribute ";
  const char* classWord = "class ";
  const char* realWord = "real";
  const char* ataWord = "ata";
  char buff = ' ';
  char wordBuf[100];
  char label1[100], label2[100];
  int lineNo = 1;
  FILE* f = fopen(FileName.chars(), "r");
  try
  {
    if(!f)
    {
      cerr << "Can't open file:" << FileName << endl;
      exit(-1);
    }

    //looking for @relation
    LookForAt(f, buff, lineNo);
    if(!CheckReadWord(f, buff, relationWord))
      throw "Can't read relations word";
    if(!fgets(wordBuf, 100, f))
      throw "Can't read relation";
    lineNo++;
    //cout << "Relation:" << wordBuf;

    //read attributes
    int numAttr = 0;
    while(1)
    {
      LookForAt(f, buff, lineNo);
      if(!CheckReadWord(f, buff, attributeWord))
        break;

      bool attrNameRead = false;
      if(classAttribute < 0)
      {
        if(CheckReadWord(f, buff, classWord))
        {
          buff = fgetc(f);
          //cout << buff << endl;
          if(buff == EOF)
            throw "Unexpected EOF";
          if(buff != '{')
            throw "Class must be nominal";
          int i = 0;
          for(;i < 99; i++)
          {
            buff = fgetc(f);
            if(buff == EOF)
              throw "Unexpected EOF";
            //cout << buff << endl;
            if(buff == ',')
              break;
            label1[i] = buff;
          }
          label1[i] = 0;

          i = 0;
          for(;i < 99; i++)
          {
            buff = fgetc(f);
            if(buff == EOF)
              throw "Unexpected EOF";
            //cout << buff << endl;
            if(buff == '}')
              break;
            label2[i] = buff;
          }
          label2[i] = 0;

          //finish line
          while(1)
          {
            buff = fgetc(f);
            //cout << buff << endl;
            if(buff == 10)
            {
              lineNo++;
              break;
            }
            if(buff == EOF)
              throw "Unexpected EOF";
          }
          //cout << "Found class attribute\nl1:" << label1 << "  l2:" << label2
          //      << "  numAttr:" << numAttr << endl;
          classAttribute = numAttr++;
          continue;
        }
        attrNameRead = true;
      }

      if(!attrNameRead) //look for non space
      {
        while(1)
        {
          if(buff != 32)
            break;
          if(buff == EOF)
            throw "Unexpected EOF";
          buff = fgetc(f);
          //cout << buff << endl;
        }
      }

      //skip name of attribute
      while(1)
      {
        if(buff == 32)
          break;
        if(buff == EOF)
          throw "Unexpected EOF";
        buff = fgetc(f);
        //cout << buff << endl;
      }

      if(CheckReadWord(f, buff, realWord))
      {
        //cout << "Found real attr at line:" << lineNo << "  numAttr:" << numAttr << endl;
        attributes.Append(numAttr++);
        continue;
      }

      //finish line
      while(1)
      {
        buff = fgetc(f);
        cout << buff << endl;
        if(buff == 10)
        {
          lineNo++;
          break;
        }
        if(buff == EOF)
          throw "Unexpected EOF";
      }
      cout << "Skipping attribute at line:" << lineNo << "  numAttr:" << numAttr << endl;
      numAttr++;
    }

    cout << "numAttributes:" << attributes.Size() << "  classAttr:" << classAttribute << endl;
    if(attributes.Size() == 0 || classAttribute < 0)
      throw "Error reading attributes;";

    if(buff != 'd' || !CheckReadWord(f, buff, ataWord))
      throw "error reading @data token";
    //finish line
    while(1)
    {
      buff = fgetc(f);
      //cout << buff << endl;
      if(buff == 10)
      {
        lineNo++;
        break;
      }
      if(buff == EOF)
        throw "Unexpected EOF";
    }

    //read data
    int numObj = 0;
    while(1)
    {
      int numAttr = -1;
      int attrID = 0;
      bool classRead = false;
      int attrRead = 0;
      VectorC vec(attributes.Size());
      while(1)
      {
        if(!ReadWord(f, buff, wordBuf, 100, ','))
          break;
        numAttr++;
        if(numAttr == classAttribute)
        {
          if(strcmp(label1, wordBuf) == 0)
            TrLab.Append(1);
          else if(strcmp(label2, wordBuf) == 0)
            TrLab.Append(-1);
          else
            throw "Wrong class label";
          classRead = true;
        }
        else
        {
          if(attributes[attrID] == numAttr)
          {
            vec[attrID++] = atof(wordBuf);
            attrRead++;
          }
        }
        if(buff == 10)
        {
          lineNo++;
          break;
        }
      }
      if(!classRead && attrRead == 0)
        break;
      if(!classRead || attrRead != int(attributes.Size()) )
        throw "Error reading data line";
      TrVec.Append(vec);
      numObj++;
      if((numObj % 1000) == 0)
        printf("%i   \r", numObj);
    }

    cout << "numObj:" << numObj << endl;

    fclose(f);
  }
  catch(const char* Str)
  {
    fclose(f);
    cerr << "Error reading file:" << FileName << endl;
    cerr << "In line:" << lineNo << "   buff:";
    if(buff > ' ')
      cerr << buff;
    else
      cerr << int(buff);
    cerr << endl;
    cerr << Str << endl;
    exit(-1);
  }
}
//---------------------------------------------------------------------------
void TrimFeatures(SampleC<VectorC> &Vecs, const char *Mask)
{
  if(Vecs.Size() == 0)
    return;

  unsigned int numFeatures = Vecs[0].Size();

  unsigned int numResFeatures = 0;
  for(unsigned int i = 0; i < numFeatures; i++)
  {
    if(Mask[i] == 0)
      throw "Mask is too short";

    if(Mask[i] > '0')
      numResFeatures++;
  }

  for(SampleIterC<VectorC> itVec(Vecs); itVec; itVec++)
  {
    VectorC& oldVec = *itVec;

    if(oldVec.Size() != numFeatures)
      throw "Wrong number of features";

    VectorC newVec(numResFeatures);
    int curFeature = 0;
    for(unsigned int i = 0; i < numFeatures; i++)
    {
      if(Mask[i] > '0')
      {
        newVec[curFeature] = oldVec[i];
        curFeature++;
      }
    }
    oldVec = newVec;
  }
}
//---------------------------------------------------------------------------
};
