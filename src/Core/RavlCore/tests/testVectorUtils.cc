// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/Base/testTFVector.cc"
//! docentry="Ravl.API.Core.Tuples"
//! userlevel=Develop
//! author="Charles Galambos"
//! date="24/01/2001"

#include "Ravl/VectorUtils.hh"
#include "Ravl/Stream.hh"

using namespace RavlN;

int testSimple() {
  const float buf1[4] = { 1,2,3,4 };
  const float buf2[4] = { 1,3,5,7 };

  float value = RavlBaseVectorN::DotProduct(buf1, buf2, 2);
  if(Abs(value - 7.0) > 1e-9) return __LINE__;


  return 0;
}

template<class DataT>
int testDotProduct(DataT Threshold) {
  const size_t bufferLen = 32;
  DataT buf1[bufferLen];
  DataT buf2[bufferLen];
  for(size_t i = 0; i < bufferLen; i++) {
    buf1[i] = (i % 253)/255.0;
    buf2[i] = (i*i % 253) / 255.0;
  }

  for(size_t testSize = 1;testSize < bufferLen;testSize++) {
    //calculate sums old slow way
    DataT sum00_00 = 0.;
    for(size_t i = 0; i < testSize; i++) {
      sum00_00 += buf1[i] * buf2[i];
    }

    DataT sum01_10 = 0.;
    for(size_t i = 0; i < testSize-1; i++) {
      sum01_10 += buf1[i+1] * buf2[i];
    }

    DataT sum10_01 = 0.;
    for(size_t i = 0; i < testSize-1; i++) {
      sum10_01 += buf1[i] * buf2[i+1];
    }

    DataT sum00_11 = 0.;
    for(size_t i = 0; i < testSize-1; i++) {
      sum00_11 += buf1[i] * buf2[i];
    }

    DataT sum11_00 = 0.;
    for(size_t i = 0; i < testSize-1; i++) {
      sum11_00 += buf1[i+1] * buf2[i+1];
    }
    //test fast way

    DataT fast00_00 = RavlBaseVectorN::DotProduct(buf1, buf2, testSize);
    if(!IsSmall(RavlN::Abs(fast00_00 - sum00_00),1.0f,Threshold)) {
      std::cerr << "Diff=" << RavlN::Abs(fast00_00 - sum00_00) << "\n";
      return __LINE__;
    }

    DataT fast01_10 = RavlBaseVectorN::DotProduct(buf1+1, buf2, testSize-1);
    if(!IsSmall(RavlN::Abs(fast01_10 - sum01_10),1.0f,Threshold)) {
      std::cerr << "Diff=" << RavlN::Abs(fast01_10 - sum01_10) << "\n";
      return __LINE__;
    }

    double fast10_01 = RavlBaseVectorN::DotProduct(buf1, buf2+1, testSize-1);
    if(!IsSmall(RavlN::Abs(fast10_01 - sum10_01),1.0f,Threshold)) return __LINE__;

    double fast00_11 = RavlBaseVectorN::DotProduct(buf1, buf2, testSize-1);
    if(!IsSmall(RavlN::Abs(fast00_11 - sum00_11),1.0f,Threshold)) return __LINE__;

    double fast11_00 = RavlBaseVectorN::DotProduct(buf1+1, buf2+1, testSize-1);
    if(!IsSmall(RavlN::Abs(fast11_00 - sum11_00),1.0f,Threshold)) return __LINE__;
  }
  return 0;
}

int testConvolveKernel() {
  for(size_t matrRows = 128-2*13; matrRows <= 200; matrRows += 13) {
    //cerr << "matrRows:" << matrRows << std::endl;
    for(size_t matrCols = 128-2*13; matrCols <= 200; matrCols += 13) {
      //cerr << "matrCols:" << matrCols << std::endl;
      for(size_t kernRows = 32-2*5; kernRows <= 64; kernRows += 5) {
        //cerr << "kernRows:" << kernRows << std::endl;
        for(size_t kernCols = 32-2*5; kernCols <= 64; kernCols += 5) {
          //cerr << "kernCols:" << kernCols << std::endl;
          float matrix[matrRows][matrCols];
          float kernel[kernRows][kernCols];

          //create kernel
          for(size_t r = 0; r < kernRows; r++)
            for(size_t c = 0; c < kernCols; c++) {
              kernel[r][c] = (((r-c) * (r-c)) % 253)/255.0;
            }

          //create matrix
          for(size_t r = 0; r < matrRows; r++)
            for(size_t c = 0; c < matrCols; c++) {
              matrix[r][c] = ((r + c)%253) / 255.0;
            }


          for(size_t posRow = 20-2; posRow <= 25; posRow += 1) {
            //cerr << "posRow:" << posRow << std::endl;
            for(size_t posCol = 10-2; posCol <= 15; posCol += 1) {
              //cerr << "posCol:" << posCol << std::endl;

              //compute old way
              float resOld = 0.f;
              for(size_t r = 0; r < kernRows; r++)
                for(size_t c = 0; c < kernCols; c++) {
                  resOld += matrix[r+posRow][c+posCol] * kernel[r][c];
                }

              float resNew = 111;
              RavlBaseVectorN::ConvolveKernel(&(matrix[posRow][posCol]), &(kernel[0][0])  , kernRows, kernCols, matrCols*sizeof(float), &resNew);

              if(!IsSmall(RavlN::Abs(resNew - resOld),resNew,1e-5)) {
                std::cerr << "Diff=" << RavlN::Abs(resNew - resOld) << " New=" << resNew << " " << resOld << "\n";
                return __LINE__;
              }
            }
          }
        }
      }
    }
  }
  return 0;
}

int main(int nargs,char **argv) {
  int ln;
  if((ln = testSimple()) != 0) {
    std::cerr << "Error line :" << ln << "\n";
    return 1;
  }
  if((ln = testDotProduct<float>(2e-7)) != 0) {
    std::cerr << "Error 'float' line :" << ln << "\n";
    return 1;
  }
  if((ln = testDotProduct<double>(5e-16)) != 0) {
    std::cerr << "Error 'double' line :" << ln << "\n";
    return 1;
  }
  if((ln = testConvolveKernel()) != 0) {
    std::cerr << "Error line :" << ln << "\n";
    return 1;
  }
  std::cerr <<"Test passed ok. \n";
  return 0;
}


