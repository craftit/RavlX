// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2001-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlCore

#include "Ravl/VectorUtils.hh"
#include "Ravl/Stream.hh"
#include "Ravl/SArray1d.hh"
#include "Ravl/Array2d.hh"
#include "Ravl/SysLog.hh"
#include "Ravl/Random.hh"

template<class DataT> int TestDot();
template<class DataT> int TestQuad();
template<class DataT> int TestMatMulVec();
int testConvolveKernel();
int testConvolveKernelQuad();
int testFastTanh();


int main() {
  int ln;
  if((ln = TestDot<float>()) != 0) {
    std::cerr << "Test failed on line " << ln << "\n";
    return ln;
    }
  if((ln = TestDot<double>()) != 0) {
    std::cerr << "Test failed on line " << ln << "\n";
    return ln;
  }
  if((ln = TestQuad<double>()) != 0) {
    std::cerr << "Test failed on line " << ln << "\n";
    return ln;
  }
  if((ln = TestQuad<float>()) != 0) {
    std::cerr << "Test failed on line " << ln << "\n";
    return ln;
  }
  if((ln = TestMatMulVec<float>()) != 0) {
    std::cerr << "Test failed on line " << ln << "\n";
    return ln;
  }
  if((ln = TestMatMulVec<double>()) != 0) {
    std::cerr << "Test failed on line " << ln << "\n";
    return ln;
  }
  if((ln = testConvolveKernel()) != 0) {
    std::cerr << "Test failed on line " << ln << "\n";
    return ln;
  }
  if((ln = testConvolveKernelQuad()) != 0) {
    std::cerr << "Test failed on line " << ln << "\n";
    return ln;
  }
  if((ln = testFastTanh()) != 0) {
    std::cerr << "Test failed on line " << ln << "\n";
    return ln;
  }
  std::cout << "Test passed ok. \n";
  return 0;
}

template<class DataT>
int TestDot() {
  RavlN::SArray1dC<DataT> data(32);
  for(int i = 0;i < 32;i++)
    data[i] = i;
  
  for(int l = 0;l <= 16;l++) {
    for(int i = 0;i < 16;i++) {
      for(int j = 0;j < 16;j++) {
        // Compute true answer
        DataT trueProduct = 0;
        for(int k = 0;k < l;k++)
          trueProduct += data[k+i] * data[k+j];
        // Compute dot product.
        DataT dp = RavlBaseVectorN::DotProduct(&data[i],&data[j],l);
        DataT diff = trueProduct - dp;
        if(RavlN::Abs(diff) > 1e-12) {
          std::cerr << "Failed l=" << l << " i=" << i << " j=" << j << " True=" << trueProduct << " Sum=" << dp << "\n";
          return __LINE__;
        }
      }
    }
  }
  return 0;
  
}

template<class DataT>
int TestQuad() {
  RavlN::SArray1dC<DataT> data(32);
  for(int i = 0;i < 32;i++)
    data[i] = i;
  
  for(int l = 0;l <= 16;l++) {
    for(int i = 0;i < 16;i++) {
      for(int j = 0;j < 16;j++) {
        for(int j2 = 0;j2 < 16;j2++) {
          // Compute true answer
          DataT trueDotAndResult = 0;
          for(int k = 0;k < l;k++)
            trueDotAndResult += data[k+i] * (data[k+j] + data[k+j2]*data[k+i]);
          // Compute dot and add.
          DataT qp = RavlBaseVectorN::QuadProduct(&data[i],&data[j],&data[j2],l);
          DataT diff = trueDotAndResult - qp;
          if(RavlN::Abs(diff) > 1e-12) {
            std::cerr << "Failed l=" << l << " i=" << i << " j=" << j << " j2=" << j2 << " True=" << trueDotAndResult << " Sum=" << qp << " Diff= " << diff << "\n";
            return __LINE__;
          }
        }
      }
    }
  }
  return 0;
  
}

template<typename DataT>
void RefMatrixMulVector(const DataT *matrix, 
                        const DataT *vec, // Must be 'col' entries
                        unsigned int rows,
                        unsigned int cols,         
                        int stride,         // Stride of matrix.
                        DataT *result       // Must have 'rows' entries
                        ) 
{
  RavlAssert(rows > 0);
  RavlAssert(cols > 0);
  const DataT *rowStart = matrix;
  for(unsigned int i = 0;i < rows;i++,rowStart += stride) {
    DataT accum = rowStart[0]*vec[0];
    for(unsigned int j = 1;j < cols;j++)
      accum += rowStart[j]*vec[j];
    result[i] = accum;
  }
}


template<class DataT> 
int TestMatMulVec() {
  
  RavlN::SArray1dC<DataT> vec(128);
  for(unsigned int i = 0;i < vec.Size();i++)
    vec[i] = i+1;
  RavlN::SArray1dC<DataT> refResult(128);
  RavlN::SArray1dC<DataT> testResult(128);
  
  // Change matrix sizes.
  for(int n = 1;n < 32;n += 3) {
    for(int m = 1;m < 32;m += 3) {
      RavlN::Array2dC<DataT> mat(n,m);
      for(int i = 0;i < n;i++)
        for(int j = 0;j < m;j++) 
          mat[i][j] = i % 5 + j % 11 + 1;
      
      // Matrix Start offsets.
      for(int i = 0;i < n;i++) {
        for(int j = 0;j < m;j++) {
          
          // Vector start offsets.
          for(int k = 0;k < 5;k++) {
            for(int q = 0;q < 5;q++) {
              int rows = n-i;
              int cols = m-j;
              RavlAssert(rows > 0 && cols > 0);
              RefMatrixMulVector(&(mat[i][j]),&vec[k],rows,cols,mat.Stride(),&refResult[q]);
              RavlBaseVectorN::MatrixMulVector(&(mat[i][j]),&vec[k],rows,cols,mat.Stride(),&testResult[q]);
              DataT sum = 0;
              for(int r = 0;r < rows;r++)
                sum += RavlN::Abs(testResult[q+r] - refResult[q+r]);
              if(sum > 0.000001) {
                std::cerr << "Test failed. Sum=" << sum << "\n";
                std::cerr << "n=" << n << " m=" << m << " i=" << i << " j=" << j << " k=" << k << " q=" << q <<"\n";
                RavlAssert(0);
                return __LINE__;
              }
            }
          }
        }
      }
      
      //std::cerr << ".";
    }
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
              kernel[r][c] = (r-c) * (r-c);
            }

          //create matrix
          for(size_t r = 0; r < matrRows; r++)
            for(size_t c = 0; c < matrCols; c++) {
              matrix[r][c] = (r + c) / 2.;
            }


          for(size_t posRow = 20-2; posRow <= 25; posRow += 1) {
            //cerr << "posRow:" << posRow << std::endl;
            for(size_t posCol = 10-2; posCol <= 15; posCol += 1) {
              //cerr << "posCol:" << posCol << std::endl;

              //compute old way
              float resOld = 0.f;
              for(size_t r = 0; r < kernRows; r++) {
                //std::cerr << "r:" << r << "  kernRows:" << kernRows << std::endl;
                for(size_t c = 0; c < kernCols; c++) {
                  //std::cerr << "c:" << c << "  kernCols:" << kernCols << std::endl;
                  //std::cerr << resOld << "   " << matrix[r+posRow][c+posCol] << "   " << kernel[r][c] << std::endl;
                  resOld += matrix[r+posRow][c+posCol] * kernel[r][c];
                }
              }

              float resNew = 111;
              RavlBaseVectorN::ConvolveKernel(&(matrix[posRow][posCol]), &(kernel[0][0]), kernRows, kernCols, matrCols*sizeof(float), &resNew);

              if(RavlN::Abs(resNew - resOld) > (resNew + resOld) * 1e-5) {
                std::cerr << "ress: " << resOld << "    " << resNew << "    " << resOld - resNew << std::endl;
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

int testConvolveKernelQuad() {
  for(size_t matrRows = 128-2*13; matrRows <= 200; matrRows += 13) {
    //cerr << "matrRows:" << matrRows << std::endl;
    for(size_t matrCols = 128-2*13; matrCols <= 200; matrCols += 13) {
      //cerr << "matrCols:" << matrCols << std::endl;
      for(size_t kernRows = 32-2*5; kernRows <= 64; kernRows += 5) {
        //cerr << "kernRows:" << kernRows << std::endl;
        for(size_t kernCols = 32-2*5; kernCols <= 64; kernCols += 5) {
          //cerr << "kernCols:" << kernCols << std::endl;
          float matrix[matrRows][matrCols];
          float kernel1[kernRows][kernCols];
          float kernel2[kernRows][kernCols];

          //create kernel
          for(size_t r = 0; r < kernRows; r++)
            for(size_t c = 0; c < kernCols; c++) {
              kernel1[r][c] = (r-c) * (r-c);
              kernel2[r][c] = r * c / 100.f;
            }

          //create matrix
          for(size_t r = 0; r < matrRows; r++)
            for(size_t c = 0; c < matrCols; c++) {
              matrix[r][c] = (r + c) / 2.;
            }


          for(size_t posRow = 20-2; posRow <= 25; posRow += 1) {
            //cerr << "posRow:" << posRow << std::endl;
            for(size_t posCol = 10-2; posCol <= 15; posCol += 1) {
              //cerr << "posCol:" << posCol << std::endl;

              //compute old way
              float resOld = 0.f;
              for(size_t r = 0; r < kernRows; r++) {
                //std::cerr << "r:" << r << "  kernRows:" << kernRows << std::endl;
                for(size_t c = 0; c < kernCols; c++) {
                  //std::cerr << "c:" << c << "  kernCols:" << kernCols << std::endl;
                  //std::cerr << resOld << "   " << matrix[r+posRow][c+posCol] << "   " << kernel[r][c] << std::endl;
                  float matVal = matrix[r+posRow][c+posCol];
                  resOld += matVal * (kernel1[r][c] + matVal * kernel2[r][c]);
                }
              }

              float resNew = 111;
              RavlBaseVectorN::ConvolveKernelQuad(&(matrix[posRow][posCol]), &(kernel1[0][0]), &(kernel2[0][0]), kernRows, kernCols, matrCols*sizeof(float), &resNew);

              if(RavlN::Abs(resNew - resOld) > (resNew + resOld) * 1e-5) {
                std::cerr << "ress: " << resOld << "    " << resNew << "    " << resOld - resNew << std::endl;
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

int testFastTanh()
{
  const int maxSize = 2000;
  RavlN::SArray1dC<float> dataIn(maxSize+1000);
  RavlN::SArray1dC<float> dataOut(maxSize+1000);
  for(int i = 0;i < dataIn.Size();i++) {
    dataIn[i] = (RavlN::Random1() - 0.5) * 10.0;
  }
  {
    double maxErr = 0;
    for(int j = 0;j < 10;j++) {
      for(int i = 0;i < maxSize;i++) {
        RavlBaseVectorN::FastTanh(&dataOut[j],&dataIn[0],i);
      }
      for(int i = 0;i < maxSize;i++) {
        for(int k = 0;k < i;k++) {
          double val = RavlN::Tanh(dataIn[k]);
          double err = RavlN::Abs(dataOut[j+k] - val);
          if(err > maxErr)
            maxErr = err;
        }
      }
    }
    RavlDebug("Max error: %f ",maxErr);
  }
  return 0;
}
