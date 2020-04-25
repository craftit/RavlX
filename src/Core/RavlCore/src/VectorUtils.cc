// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2001-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#include "Ravl/VectorUtils.hh"
#include "Ravl/Stream.hh"
//! lib=RavlCore

namespace RavlBaseVectorN {

  using namespace RavlN;
  
  static double BaseDotProductD(const double* v1, const double* v2, size_t size) {
    double sum = 0.0;
    for(size_t i=0;i < size;i++)
      sum += v1[i] * v2[i];
    return sum;  
  }

  static float BaseDotProductF(const float* v1, const float* v2, size_t size) {
    float sum = 0.0;
    for(size_t i=0;i < size;i++)
      sum += v1[i] * v2[i];
    return sum;
  }
  
  static double BaseQuadProductD(const double *Data, const double *Weights1,
                               const double *Weights2, size_t Size) {
    double retVal = 0.;
    const double* wPtr = Weights1;
    const double* w2Ptr = Weights2;
    const double* const ewPtr = wPtr + Size;
    const double* dPtr = Data;
    while(wPtr != ewPtr)
      {
        const double val = *dPtr++;
        retVal += val * (*wPtr++ + *w2Ptr++ * val);
      }
    return retVal;
  }

  static float BaseQuadProductF(const float *Data, const float *Weights1,
				const float *Weights2, size_t Size) {
    float retVal = 0.;
    const float* wPtr = Weights1;
    const float* w2Ptr = Weights2;
    const float* const ewPtr = wPtr + Size;
    const float* dPtr = Data;
    while(wPtr != ewPtr) {
      const float val = *dPtr++;
      retVal += val * (*wPtr++ + *w2Ptr++ * val);
    }
    return retVal;
  }
  
  static void BaseReal2ByteD(unsigned char * byteData, const double *realData, size_t size) {
    for (int i=size; i>0; --i) {
      *(byteData++) = (*realData >= 255.0) ? 255 : (*realData<=0.0) ? 0 : ((unsigned char) (*realData + 0.5));
      realData++;
    }
  }
  
  static void BaseReal2ByteF(unsigned char * byteData, const float *realData, size_t size) {
    for (int i=size; i>0; --i) {
      *(byteData++) = (*realData >= 255.0) ? 255 : (*realData<=0.0) ? 0 : ((unsigned char) (*realData + 0.5));
      realData++;
    }
  }
  
  
  static void MatrixMulVectorF(const float *matrix, 
                               const float *vec, // Must be 'col' entries
                               UIntT rows,
                               UIntT cols,         
                               IntT stride,        // Stride of matrix.
                               float *result       // Must have 'rows' entries
                               ) 
  {
    RavlAssert(rows > 0);
    RavlAssert(cols > 0);
    const float *rowStart = matrix;
    for(unsigned int i = 0;i < rows;i++,rowStart += stride) {
      float accum = rowStart[0]*vec[0];
      for(unsigned int j = 1;j < cols;j++)
        accum += rowStart[j]*vec[j];
      result[i] = accum;
    }
  }

  static void MatrixMulVectorAddF(const float *matrix,
                               const float *vec, // Must be 'col' entries
                               UIntT rows,
                               UIntT cols,
                               IntT stride,        // Stride of matrix.
                               float *result       // Must have 'rows' entries
                               )
  {
    RavlAssert(rows > 0);
    RavlAssert(cols > 0);
    const float *rowStart = matrix;
    for(unsigned int i = 0;i < rows;i++,rowStart += stride) {
      float accum = rowStart[0]*vec[0];
      for(unsigned int j = 1;j < cols;j++)
        accum += rowStart[j]*vec[j];
      result[i] += accum;
    }
  }

  static void MatrixMulVectorD(const double *matrix, 
                               const double *vec, // Must be 'col' entries
                               UIntT rows,
                               UIntT cols,         
                               IntT stride,         // Stride of matrix.
                               double *result       // Must have 'rows' entries
                               ) 
  {
    RavlAssert(rows > 0);
    RavlAssert(cols > 0);
    const double *rowStart = matrix;
    for(unsigned int i = 0;i < rows;i++,rowStart += stride) {
      double accum = rowStart[0]*vec[0];
      for(unsigned int j = 1;j < cols;j++)
        accum += rowStart[j]*vec[j];
      result[i] = accum;
    }
  }
  
  static void MatrixTMulVectorF(const float *matrix,
                                const float *vec, // Must be 'col' entries
                                UIntT rows,
                                UIntT cols,
                                IntT stride,        // Stride of matrix.
                                float *result       // Must have 'rows' entries
                                )
  {
    unsigned int i = 0;
    for(;i < cols;i++)
      result[i] = 0;
    const float *rowStart = matrix;
    for(i = 0;i < rows; i++) {
      const float dat = vec[i];
      for(unsigned int j = 0;j < cols;j++)
        result[j] += rowStart[j] * dat;
      rowStart += stride;
    }
  }

  static void MatrixTMulVectorD(const double *matrix, 
                                const double *vec, // Must be 'col' entries
                                UIntT rows,
                                UIntT cols,         
                                IntT stride,         // Stride of matrix.
                                double *result       // Must have 'rows' entries
                                ) 
  {
    unsigned int i = 0;
    for(;i < cols;i++)
      result[i] = 0;
    const double *rowStart = matrix;
    for(i = 0;i < rows; i++) {
      const double dat = vec[i];
      for(unsigned int j = 0;j < cols;j++)
        result[j] += rowStart[j] * dat;
      rowStart += stride;
    }
  }


  static void ConvolveKernelF(const float *matrix,
                              const float *kernel,size_t rows,size_t cols,
                              int matrixByteStride,float *result)
  {
    float ret = 0;
    const float *vi = matrix;
    const float *vk = kernel;
    for(size_t i = 0;i < rows;i++) {
      const float *vir = vi;
      for(size_t j = 0;j < cols;j++)
        ret += *(vk++) * *(vir++);
      vi = reinterpret_cast<const float *>(reinterpret_cast<const char *>(vi) + matrixByteStride);
    }
    *result = ret;
  }

  static void ConvolveKernelQuadF(const float *matrix,
                                  const float *kernel1, const float *kernel2,
                                  size_t rows, size_t cols,
                                  int matrixByteStride, float *result)
  {
    float ret = 0;
    const float *vi = matrix;
    const float *vk1 = kernel1;
    const float *vk2 = kernel2;
    for(size_t i = 0;i < rows;i++) {
      const float *vir = vi;
      for(size_t j = 0;j < cols;j++) {
        ret += *vir * (*(vk1++) + *(vk2++) * *vir);
        vir++;
      }
      vi = reinterpret_cast<const float *>(reinterpret_cast<const char *>(vi) + matrixByteStride);
    }
    *result = ret;
  }

  // The following code is based on fast_sse_tanh by lubomir i. ivanov, neolit123 [at] gmail

  static const float g_TANH_RANGE = 5.f;
  static const float g_TANH_CLAMP = 1.f;
  static const float g_TANH_K0 = 21.f;
  static const float g_TANH_K1 = 210.f;
  static const float g_TANH_K2 = 1260.f;
  static const float g_TANH_K3 = 4725.f;
  static const float g_TANH_K4 = 10395.f;

  float fast_tanh(const float x)
  {
    const float s = x*x;
    float d;

    if      (x < -g_TANH_RANGE)
      return -g_TANH_CLAMP;
    else if (x > g_TANH_RANGE)
      return g_TANH_CLAMP;

    d =     (s*(s*(s + g_TANH_K1) + g_TANH_K3) + g_TANH_K4);
    return  (x*(s*(g_TANH_K0*s + g_TANH_K2) + g_TANH_K4)) / d;
  }

  static void FastTanhF(float *out,const float *in,size_t len)
  {
    for(size_t i= 0;i < len;i++) {
      out[i] = fast_tanh(in[i]);
    }
  }

  
  double (*g_DotProductD)(const double*, const double*, size_t) = &BaseDotProductD;
  float (*g_DotProductF)(const float*, const float*, size_t) = &BaseDotProductF;
  double (*g_QuadProductD)(const double*, const double*, const double*, size_t) = &BaseQuadProductD;
  float (*g_QuadProductF)(const float*, const float*, const float*, size_t) = &BaseQuadProductF;
  void (*g_Real2ByteD)(unsigned char*, const double*, size_t) = &BaseReal2ByteD;
  void (*g_Real2ByteF)(unsigned char*, const float*, size_t) = &BaseReal2ByteF;
  void (*g_MatrixMulVectorD)(const double *,const double *,UIntT ,UIntT ,IntT ,double *) = &MatrixMulVectorD;
  void (*g_MatrixMulVectorF)(const float *,const float *,UIntT ,UIntT ,IntT ,float *) = &MatrixMulVectorF;
  void (*g_MatrixMulVectorAddF)(const float *,const float *,UIntT ,UIntT ,IntT ,float *) = &MatrixMulVectorAddF;
  
  void (*g_MatrixTMulVectorD)(const double *,const double *,unsigned int ,unsigned int ,int ,double *) = &MatrixTMulVectorD;
  void (*g_MatrixTMulVectorF)(const float *,const float *,unsigned int ,unsigned int ,int ,float *) = &MatrixTMulVectorF;
  
  void (*g_ConvolveKernelF)(const float *matrix,const float *kernel,size_t rows,size_t cols,int matrixByteStride,float *result) = &ConvolveKernelF;
  void (*g_ConvolveKernelQuadF)(const float *matrix,const float *kernel1,const float *kernel2,size_t rows,size_t cols,int matrixByteStride,float *result) = &ConvolveKernelQuadF;

  void (*g_fastTanhF)(float*, const float*, size_t) = &FastTanhF;

}
