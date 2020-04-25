// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2001-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlCore

#include "Ravl/VectorUtils.hh"
#include <emmintrin.h>
#include "Ravl/Stream.hh"
#include "Ravl/CPUID.hh"

namespace RavlBaseVectorN {
  
  using namespace RavlN;
  
  template<typename DataT>
  static inline bool Is16ByteAligned(const DataT *data) 
  { return (((unsigned long int) data) & 0xf) == 0; }
  
  // FIXME:- In both these routines if vectors have the same alignment we could process 
  // the first few entries then process as aligned .
  
  static double SSEDotProductD(const double* v1, const double* v2, size_t size) {
    double dsum = 0.0;
    if(size < 8) {
      // For small vectors all the messing about is not worth it.
      for(unsigned int i=size; i>0; --i)
        dsum += *v1++ * *v2++;
      return dsum;
    }
    const double* const ewPtr = v1 + (size & ~0x1);
    __m128d sum = _mm_setzero_pd();
    if(Is16ByteAligned(v1)) {  // v1 16-byte aligned ?
      if(Is16ByteAligned(v2)) {// v2 16-byte aligned ?
        while(v1 != ewPtr) {
          sum = _mm_add_pd(sum, _mm_mul_pd(_mm_load_pd(v2), _mm_load_pd(v1)));
          v2 += 2;
          v1 += 2;
        }
      }
      else {
        while(v1 != ewPtr) {
          sum = _mm_add_pd(sum, _mm_mul_pd(_mm_loadu_pd(v2), _mm_load_pd(v1)));
          v2 += 2;
          v1 += 2;
        }
      }
    }
    else {
      if(Is16ByteAligned(v2)) {// v2 16-byte aligned ?
        while(v1 != ewPtr) {
          sum = _mm_add_pd(sum, _mm_mul_pd(_mm_load_pd(v2), _mm_loadu_pd(v1)));
          v2 += 2;
          v1 += 2;
        }
      }
      else {
        while(v1 != ewPtr) {
          sum = _mm_add_pd(sum, _mm_mul_pd(_mm_loadu_pd(v2), _mm_loadu_pd(v1)));
          v2 += 2;
          v1 += 2;
        }
      }
    }

    if(size & 1) { // Odd length ?
      sum = _mm_add_pd(sum, _mm_mul_sd(_mm_load_sd(v2++), _mm_load_sd(v1++)));
    }
    // Sum the two accumulators together.
    sum = _mm_add_sd(sum,_mm_shuffle_pd(sum,sum,_MM_SHUFFLE2(0,1)));
    _mm_store_sd(&dsum, sum);
    return dsum;
  }
  
  static float SSEDotProductF(const float *v1,const float *v2,size_t n) {
    if(n < 12) {
      // Not worth using this code for small vectors.
      float sum = 0;
      for(unsigned int i=n; i>0; --i)
        sum += *v1++ * *v2++;
      return sum;
    }
    UIntT quadLen = (n & ~0x3);
    const float* const ewPtr = v1 + quadLen;
    __m128 sum = _mm_setzero_ps ();
    
    
    if(Is16ByteAligned(v1)) {  // v1 16-byte aligned ?
      if(Is16ByteAligned(v2)) {// v2 16-byte aligned ?
        while(v1 != ewPtr) {
          sum = _mm_add_ps(sum,_mm_mul_ps(_mm_load_ps(v1),_mm_load_ps(v2)));
          v1 += 4;
          v2 += 4;
        }
      } else {
        while(v1 != ewPtr) {
          sum = _mm_add_ps(sum,_mm_mul_ps(_mm_load_ps(v1),_mm_loadu_ps(v2)));
          v1 += 4;
          v2 += 4;
        }
      }
    } else {
      if(Is16ByteAligned(v2)) {// v2 16-byte aligned ?
        while(v1 != ewPtr) {
          sum = _mm_add_ps(sum,_mm_mul_ps(_mm_loadu_ps(v1),_mm_load_ps(v2)));
          v1 += 4;
          v2 += 4;
        }
      } else {
        while(v1 != ewPtr) {
          sum = _mm_add_ps(sum,_mm_mul_ps(_mm_loadu_ps(v1),_mm_loadu_ps(v2)));
          v1 += 4;
          v2 += 4;
        }
      }
    }
    
    sum = _mm_add_ps(sum,_mm_shuffle_ps(sum,sum, _MM_SHUFFLE(2,3,0,1)));
    sum = _mm_add_ps(sum,_mm_shuffle_ps(sum,sum, _MM_SHUFFLE(1,0,3,2)));
    
    float ret = 0;
    _mm_store_ss(&ret,sum);
    
    UIntT remainder = n - quadLen;
    // Add in leftovers
    for(;remainder > 0;remainder--) {
      ret += *(v1++) * *(v2++);
    }
    return ret;
  }

  
  
  static double SSEQuadProductD(const double *data, 
				const double *weights1,
				const double *weights2, 
				size_t size) 
  {
    const double* wPtr = weights1;
    const double* w2Ptr = weights2;
    const double* const ewPtr = weights1 + (size & ~0x1);
    const double* dPtr = data;
    __m128d sum = _mm_setzero_pd();
    if(Is16ByteAligned(dPtr) && Is16ByteAligned(wPtr) && Is16ByteAligned(w2Ptr))  { // Data 16-byte aligned ?
      while(wPtr != ewPtr) {
        const __m128d val = _mm_load_pd(dPtr);
        sum = _mm_add_pd(sum,
                         _mm_mul_pd(val,
                                    _mm_add_pd(_mm_mul_pd(val,
                                                          _mm_load_pd(w2Ptr)),
                                               _mm_load_pd(wPtr))));
        dPtr += 2;
        wPtr += 2;
        w2Ptr += 2;
      }
    }
    else {
      while(wPtr != ewPtr) {
        const __m128d val = _mm_loadu_pd(dPtr);
        sum = _mm_add_pd(sum,
                         _mm_mul_pd(val,
                                    _mm_add_pd(_mm_mul_pd(val,
                                                          _mm_loadu_pd(w2Ptr)),
                                               _mm_loadu_pd(wPtr))));
        dPtr += 2;
        wPtr += 2;
        w2Ptr += 2;
      }
    }
    
    if(size & 1) { // Odd length ?
      const __m128d val = _mm_load_sd(dPtr++);
      sum = _mm_add_pd(sum,
                       _mm_mul_sd(val,
                                  _mm_add_sd(_mm_mul_sd(val,
                                                        _mm_load_sd(w2Ptr++)),
                                             _mm_load_sd(wPtr++))));
    }
    
    double dsum;
    sum = _mm_add_sd(sum,_mm_shuffle_pd(sum,sum,_MM_SHUFFLE2(0,1)));
    _mm_store_sd(&dsum, sum);
    return dsum;
  }
  
  static float SSEQuadProductF(const float *data, 
			       const float *weights1,
			       const float *weights2, 
			       size_t size
			       ) 
  {
    const float* wPtr = weights1;
    const float* w2Ptr = weights2;
    const float* const ewPtr = weights1 + (size & ~0x3);
    const float* dPtr = data;
    __m128 sum = _mm_setzero_ps();
    
    if(Is16ByteAligned(wPtr)  && Is16ByteAligned(w2Ptr)) {
      if(Is16ByteAligned(dPtr))  { // Data 16-byte aligned ?
        while(wPtr != ewPtr) {
          const __m128 val = _mm_load_ps(dPtr);
          sum = _mm_add_ps(sum,
                           _mm_mul_ps(val,
                                      _mm_add_ps(_mm_mul_ps(val,
                                                            _mm_load_ps(w2Ptr)),
                                                 _mm_load_ps(wPtr))));
          dPtr += 4;
          wPtr += 4;
          w2Ptr += 4;
        }
      } else {
        while(wPtr != ewPtr) {
          const __m128 val = _mm_loadu_ps(dPtr);
          sum = _mm_add_ps(sum,
                           _mm_mul_ps(val,
                                      _mm_add_ps(_mm_mul_ps(val,
                                                            _mm_load_ps(w2Ptr)),
                                                 _mm_load_ps(wPtr))));
          dPtr += 4;
          wPtr += 4;
          w2Ptr += 4;
        }        
      }
    } else {
      while(wPtr != ewPtr) {
        const __m128 val = _mm_loadu_ps(dPtr);
        sum = _mm_add_ps(sum,
                         _mm_mul_ps(val,
                                    _mm_add_ps(_mm_mul_ps(val,
                                                          _mm_loadu_ps(w2Ptr)),
                                               _mm_loadu_ps(wPtr))));
        dPtr += 4;
        wPtr += 4;
        w2Ptr += 4;
      }
    }
    
    if(size & 3) {// Odd length ?
      int c = size & 3;
      for(int i = 0; i< c;i++) {
        const __m128 val = _mm_load_ss(dPtr++);
        sum = _mm_add_ps(sum,
                         _mm_mul_ss(val,
                                    _mm_add_ss(_mm_mul_ss(val,
                                               _mm_load_ss(w2Ptr++)),
                                    _mm_load_ss(wPtr++))));
      }
    }

    sum = _mm_add_ps(sum,_mm_shuffle_ps(sum,sum, _MM_SHUFFLE(2,3,0,1)));
    sum = _mm_add_ps(sum,_mm_shuffle_ps(sum,sum, _MM_SHUFFLE(1,0,3,2)));
    
    float ret = 0;
    _mm_store_ss(&ret,sum);
    return ret;
  }
  
  
  static void SSEReal2ByteD(unsigned char * byteData, const double *realData, size_t size) {
    __m128d min = _mm_set1_pd (0);
    __m128d max = _mm_set1_pd (255);
    __m128d voff = _mm_set1_pd (0.5);
    unsigned char *ber = &(byteData[size&~1]);
    for(;byteData < ber;) {
      __m128i ivs = _mm_cvttpd_epi32(_mm_max_pd (_mm_min_pd (_mm_add_pd (_mm_loadu_pd (realData),voff),max),min));
      *(byteData++) = (ByteT) _mm_cvtsi128_si32 (ivs);
      *(byteData++) = (ByteT) _mm_cvtsi128_si32 ( _mm_shuffle_epi32(ivs,_MM_SHUFFLE (1,1,1,1)));
      realData += 2;
    }
    if(size & 1) {
      __m128i ivs = _mm_cvttpd_epi32(_mm_max_pd (_mm_min_pd (_mm_add_pd (_mm_load_sd (realData),voff),max),min));
      *byteData = (ByteT) _mm_cvtsi128_si32 (ivs);
    }
  }
  
  static void SSEMatrixMulVectorD(const double *matrix, 
                                  const double *vec, // Must be 'col' entries
                                  UIntT rows,
                                  UIntT cols,         
                                  IntT stride,        // Stride of matrix, number of elements in a row
                                  double *result       // Must have 'rows' entries
                                  ) 
  {
    if(rows == 1) {
      *result = SSEDotProductD(matrix,vec,cols);
      return ;
    }
    const double *rowStart = matrix;
    double *resultAt = result;
    if(cols < 12) {
      for(unsigned int i = 0;i < rows;i++,rowStart += stride) {
        double accum = rowStart[0]*vec[0];
        for(unsigned int j = 1;j < cols;j++)
          accum += rowStart[j]*vec[j];
        *(resultAt++) = accum;
      }
      return ;
    }
    if((Is16ByteAligned(matrix) && Is16ByteAligned(vec) && ((stride % 2) == 0))) {
      if((cols & 1) == 0) {
        for(unsigned int i = 0;i < rows;i++,rowStart += stride) {
          const double *rowAt = rowStart;
          const double *vecAt = vec;
          __m128d accum =  _mm_mul_pd(_mm_load_pd(rowAt),_mm_load_pd(vecAt));
          for(unsigned int j = 2;j < cols;j += 2) {
            rowAt += 2;
            vecAt += 2;
            accum = _mm_add_pd(accum,_mm_mul_pd(_mm_load_pd(rowAt),_mm_load_pd(vecAt)));
          }
          // Add two accumulators together
          accum = _mm_add_sd(accum,_mm_shuffle_pd(accum,accum,_MM_SHUFFLE2(0,1)));
          _mm_store_sd(resultAt++,accum);
        }
      } else {
        UIntT fastCols = (cols & ~0x1);
        for(unsigned int i = 0;i < rows;i++,rowStart += stride) {
          const double *rowAt = rowStart;
          const double *vecAt = vec;
          __m128d accum =  _mm_mul_pd(_mm_load_pd(rowAt),_mm_load_pd(vecAt));
          for(unsigned int j = 2;j < fastCols;j += 2) {
            rowAt += 2;
            vecAt += 2;
            accum = _mm_add_pd(accum,_mm_mul_pd(_mm_load_pd(rowAt),_mm_load_pd(vecAt)));
          }
          rowAt += 2;
          vecAt += 2;
          accum = _mm_add_pd(accum,_mm_mul_sd(_mm_load_sd(rowAt),_mm_load_sd(vecAt)));
          // Add two accumulators together
          accum = _mm_add_sd(accum,_mm_shuffle_pd(accum,accum,_MM_SHUFFLE2(0,1)));
          _mm_store_sd(resultAt++,accum);
        }        
      }
    } else {
      if((cols & 1) == 0) {
        for(unsigned int i = 0;i < rows;i++,rowStart += stride) {
          const double *rowAt = rowStart;
          const double *vecAt = vec;
          __m128d accum = _mm_mul_pd(_mm_loadu_pd(rowAt),_mm_loadu_pd(vecAt));
          for(unsigned int j = 2;j < cols;j += 2) {
            rowAt += 2;
            vecAt += 2;
            accum = _mm_add_pd(accum,_mm_mul_pd(_mm_loadu_pd(rowAt),_mm_loadu_pd(vecAt)));
          }
          // Add two accumulators together
          accum = _mm_add_sd(accum,_mm_shuffle_pd(accum,accum,_MM_SHUFFLE2(0,1)));
          _mm_store_sd(resultAt++,accum);
        }
      } else {
        UIntT fastCols = (cols & ~0x1);
        for(unsigned int i = 0;i < rows;i++,rowStart += stride) {
          const double *rowAt = rowStart;
          const double *vecAt = vec;
          __m128d accum = _mm_mul_pd(_mm_loadu_pd(rowAt),_mm_loadu_pd(vecAt));
          for(unsigned int j = 2;j < fastCols;j += 2) {
            rowAt += 2;
            vecAt += 2;
            accum = _mm_add_pd(accum,_mm_mul_pd(_mm_loadu_pd(rowAt),_mm_loadu_pd(vecAt)));
          }
          rowAt += 2;
          vecAt += 2;
          accum = _mm_add_pd(accum,_mm_mul_sd(_mm_load_sd(rowAt),_mm_load_sd(vecAt)));
          // Add two accumulators together
          accum = _mm_add_sd(accum,_mm_shuffle_pd(accum,accum,_MM_SHUFFLE2(0,1)));
          _mm_store_sd(resultAt++,accum);
          
        }        
      }
    }
    
  }

  static void SSEMatrixMulVectorF(const float *matrixA,
                                  const float *vecA, // Must be 'col' entries
                                  UIntT rows,
                                  UIntT cols,
                                  IntT stride,        // Stride of matrix, number of elements in a row
                                  float *result       // Must have 'rows' entries
                                  )
  {
    const float *__restrict__ matrix = matrixA;
    const float *__restrict__ vec = vecA;
    if(rows == 1) {
      *result = SSEDotProductF(matrix,vec,cols);
      return ;
    }
    //std::cerr << "cols=" << cols << " Rows=" << rows << " Stride=" << stride <<"\n";
    if(cols < 8) { // || !(Is16ByteAligned(matrix) && Is16ByteAligned(vec) && ((stride % 4) == 0))
      const float * __restrict__  rowStart = matrix;
      for(unsigned int i = 0;i < rows;i++,rowStart += stride) {
        float accum = rowStart[0]*vec[0];
        for(unsigned int j = 1;j < cols;j++)
          accum += rowStart[j]*vec[j];
        result[i] = accum;
      }
      return ;
    }
    float * __restrict__ resultAt = result;
    const float *__restrict__ rowStart = matrix;

    if(Is16ByteAligned(matrix) && Is16ByteAligned(vec) && ((stride % 4) == 0)) {
      // Everything is aligned
      UIntT fastCols = (cols & ~0x3);
      //std::cerr << " A fastCols=" << fastCols << " Rem=" << (cols % 4)  <<"\n";
      if((cols % 4) == 0) {
        for(unsigned int i = 0;i < rows;i++,rowStart += stride) {
          const float * __restrict__ rowAt = rowStart;
          const float * __restrict__ vecAt = vec;
          __m128 accum =  _mm_mul_ps(_mm_load_ps(rowAt),_mm_load_ps(vecAt));
          for(unsigned int j = 4;j < cols;j += 4) {
            rowAt += 4;
            vecAt += 4;
            accum = _mm_add_ps(accum,_mm_mul_ps(_mm_load_ps(rowAt),_mm_load_ps(vecAt)));
          }
          accum = _mm_add_ps(accum,_mm_shuffle_ps(accum,accum, _MM_SHUFFLE(2,3,0,1)));
          accum = _mm_add_ps(accum,_mm_shuffle_ps(accum,accum, _MM_SHUFFLE(1,0,3,2)));
          _mm_store_ss(resultAt++,accum);
        }
      } else {
        for(unsigned int i = 0;i < rows;i++,rowStart += stride) {
          const float * __restrict__ rowAt = rowStart;
          const float * __restrict__ vecAt = vec;
          __m128 accum =  _mm_mul_ps(_mm_load_ps(rowAt),_mm_load_ps(vecAt));
          unsigned int j;
          for(j = 4;j < fastCols;j += 4) {
            rowAt += 4;
            vecAt += 4;
            accum = _mm_add_ps(accum,_mm_mul_ps(_mm_load_ps(rowAt),_mm_load_ps(vecAt)));
          }
          // Finish off row
          rowAt += 4;
          vecAt += 4;
          for(;j < cols;j++) {
            accum = _mm_add_ps(accum,_mm_mul_ss(_mm_load_ss(rowAt++),_mm_load_ss(vecAt++)));
          }
          accum = _mm_add_ps(accum,_mm_shuffle_ps(accum,accum, _MM_SHUFFLE(2,3,0,1)));
          accum = _mm_add_ps(accum,_mm_shuffle_ps(accum,accum, _MM_SHUFFLE(1,0,3,2)));
          _mm_store_ss(resultAt++,accum);
        }
      }

    } else {
      //
      if((cols % 4) == 0) {
        //std::cerr << " B fastCols=" << fastCols << " cols=" << cols << " Rem=" << (cols % 4)  <<"\n";
        // Unaligned with no extra columns
        for(unsigned int i = 0;i < rows;i++,rowStart += stride) {
          const float * __restrict__  rowAt = rowStart;
          const float * __restrict__ vecAt = vec;
          __m128 accum =  _mm_mul_ps(_mm_loadu_ps(rowAt),_mm_loadu_ps(vecAt));
          unsigned int j;
          for(j = 4;j < cols;j += 4) {
            rowAt += 4;
            vecAt += 4;
            accum = _mm_add_ps(accum,_mm_mul_ps(_mm_loadu_ps(rowAt),_mm_loadu_ps(vecAt)));
          }
          accum = _mm_add_ps(accum,_mm_shuffle_ps(accum,accum, _MM_SHUFFLE(2,3,0,1)));
          accum = _mm_add_ps(accum,_mm_shuffle_ps(accum,accum, _MM_SHUFFLE(1,0,3,2)));
          _mm_store_ss(resultAt++, accum);
        }
      } else {
        //std::cerr << " C fall back.\n";
        // Unaligned with extra columns
        UIntT fastCols = (cols & ~0x3);
        for(unsigned int i = 0;i < rows;i++,rowStart += stride) {
          const float * __restrict__  rowAt = rowStart;
          const float * __restrict__  vecAt = vec;
          __m128 accum =  _mm_mul_ps(_mm_loadu_ps(rowAt),_mm_loadu_ps(vecAt));
          unsigned int j;
          for(j = 4;j < fastCols;j += 4) {
            rowAt += 4;
            vecAt += 4;
            accum = _mm_add_ps(accum,_mm_mul_ps(_mm_loadu_ps(rowAt),_mm_loadu_ps(vecAt)));
          }
          // Finish off row
          rowAt += 4;
          vecAt += 4;
          for(;j < cols;j++) {
            accum = _mm_add_ps(accum,_mm_mul_ss(_mm_load_ss(rowAt++),_mm_load_ss(vecAt++)));
          }
          accum = _mm_add_ps(accum,_mm_shuffle_ps(accum,accum, _MM_SHUFFLE(2,3,0,1)));
          accum = _mm_add_ps(accum,_mm_shuffle_ps(accum,accum, _MM_SHUFFLE(1,0,3,2)));
          _mm_store_ss(resultAt++, accum);
        }
      }
    }
  }

  
  static void SSEMatrixMulVectorAddF(const float *matrixA,
                                  const float *vecA, // Must be 'col' entries
                                  UIntT rows,
                                  UIntT cols,         
                                  IntT stride,        // Stride of matrix, number of elements in a row
                                  float *result       // Must have 'rows' entries
                                  ) 
  {
    const float *__restrict__ matrix = matrixA;
    const float *__restrict__ vec = vecA;
    if(rows == 1) {
      *result += SSEDotProductF(matrix,vec,cols);
      return ;
    }
    //std::cerr << "cols=" << cols << " Rows=" << rows << " Stride=" << stride <<"\n";
    if(cols < 8) { // || !(Is16ByteAligned(matrix) && Is16ByteAligned(vec) && ((stride % 4) == 0))
      const float * __restrict__  rowStart = matrix;
      for(unsigned int i = 0;i < rows;i++,rowStart += stride) {
        float accum = rowStart[0]*vec[0];
        for(unsigned int j = 1;j < cols;j++)
          accum += rowStart[j]*vec[j];
        result[i] += accum;
      }
      return ;      
    }
    float * __restrict__ resultAt = result;
    const float *__restrict__ rowStart = matrix;
    
    if(Is16ByteAligned(matrix) && Is16ByteAligned(vec) && ((stride % 4) == 0)) {
      // Everything is aligned
      UIntT fastCols = (cols & ~0x3);
      //std::cerr << " A fastCols=" << fastCols << " Rem=" << (cols % 4)  <<"\n";
      if((cols % 4) == 0) {
        for(unsigned int i = 0;i < rows;i++,rowStart += stride) {
          const float * __restrict__ rowAt = rowStart;
          const float * __restrict__ vecAt = vec;
          __m128 accum =  _mm_mul_ps(_mm_load_ps(rowAt),_mm_load_ps(vecAt));
          for(unsigned int j = 4;j < cols;j += 4) {
            rowAt += 4;
            vecAt += 4;
            accum = _mm_add_ps(accum,_mm_mul_ps(_mm_load_ps(rowAt),_mm_load_ps(vecAt)));
          }
          accum = _mm_add_ps(accum,_mm_shuffle_ps(accum,accum, _MM_SHUFFLE(2,3,0,1)));
          accum = _mm_add_ps(accum,_mm_shuffle_ps(accum,accum, _MM_SHUFFLE(1,0,3,2)));
          accum = _mm_add_ps(accum,_mm_load1_ps(resultAt));
          _mm_store_ss(resultAt++,accum);
        }
      } else {
        for(unsigned int i = 0;i < rows;i++,rowStart += stride) {
          const float * __restrict__ rowAt = rowStart;
          const float * __restrict__ vecAt = vec;
          __m128 accum =  _mm_mul_ps(_mm_load_ps(rowAt),_mm_load_ps(vecAt));
          unsigned int j;
          for(j = 4;j < fastCols;j += 4) {
            rowAt += 4;
            vecAt += 4;
            accum = _mm_add_ps(accum,_mm_mul_ps(_mm_load_ps(rowAt),_mm_load_ps(vecAt)));
          }
          // Finish off row
          rowAt += 4;
          vecAt += 4;
          for(;j < cols;j++) {
            accum = _mm_add_ps(accum,_mm_mul_ss(_mm_load_ss(rowAt++),_mm_load_ss(vecAt++)));            
          }
          accum = _mm_add_ps(accum,_mm_shuffle_ps(accum,accum, _MM_SHUFFLE(2,3,0,1)));
          accum = _mm_add_ps(accum,_mm_shuffle_ps(accum,accum, _MM_SHUFFLE(1,0,3,2)));
          accum = _mm_add_ps(accum,_mm_load1_ps(resultAt));
          _mm_store_ss(resultAt++,accum);
        }     
      }
        
    } else {
      // 
      if((cols % 4) == 0) {
        //std::cerr << " B fastCols=" << fastCols << " cols=" << cols << " Rem=" << (cols % 4)  <<"\n";
        // Unaligned with no extra columns
        for(unsigned int i = 0;i < rows;i++,rowStart += stride) {
          const float * __restrict__  rowAt = rowStart;
          const float * __restrict__ vecAt = vec;
          __m128 accum =  _mm_mul_ps(_mm_loadu_ps(rowAt),_mm_loadu_ps(vecAt));
          unsigned int j;
          for(j = 4;j < cols;j += 4) {
            rowAt += 4;
            vecAt += 4;
            accum = _mm_add_ps(accum,_mm_mul_ps(_mm_loadu_ps(rowAt),_mm_loadu_ps(vecAt)));
          }
          accum = _mm_add_ps(accum,_mm_shuffle_ps(accum,accum, _MM_SHUFFLE(2,3,0,1)));
          accum = _mm_add_ps(accum,_mm_shuffle_ps(accum,accum, _MM_SHUFFLE(1,0,3,2)));
          accum = _mm_add_ps(accum,_mm_load1_ps(resultAt));
          _mm_store_ss(resultAt++, accum);
        }
      } else {
        //std::cerr << " C fall back.\n";
        // Unaligned with extra columns
        UIntT fastCols = (cols & ~0x3);
        for(unsigned int i = 0;i < rows;i++,rowStart += stride) {
          const float * __restrict__  rowAt = rowStart;
          const float * __restrict__  vecAt = vec;
          __m128 accum =  _mm_mul_ps(_mm_loadu_ps(rowAt),_mm_loadu_ps(vecAt));
          unsigned int j;
          for(j = 4;j < fastCols;j += 4) {
            rowAt += 4;
            vecAt += 4;
            accum = _mm_add_ps(accum,_mm_mul_ps(_mm_loadu_ps(rowAt),_mm_loadu_ps(vecAt)));
          }
          // Finish off row
          rowAt += 4;
          vecAt += 4;
          for(;j < cols;j++) {
            accum = _mm_add_ps(accum,_mm_mul_ss(_mm_load_ss(rowAt++),_mm_load_ss(vecAt++)));            
          }
          accum = _mm_add_ps(accum,_mm_shuffle_ps(accum,accum, _MM_SHUFFLE(2,3,0,1)));
          accum = _mm_add_ps(accum,_mm_shuffle_ps(accum,accum, _MM_SHUFFLE(1,0,3,2)));
          accum = _mm_add_ps(accum,_mm_load1_ps(resultAt));
          _mm_store_ss(resultAt++, accum);
        }
      }
    }
  }

  void SSEConvolveKernelF(const float *vi, // Scanned image, probably not aligned.
                          const float *vk, // Kernel, expected to be aligned.
                          size_t rows,
                          size_t cols,
                          int byteStride,
                          float *result
                         )
  {
    //std::cerr << "Rows=" << rows << " Cols=" << cols << " vk=" << std::hex << (void*) vk << " vi=" << (void*) vi << " Stride=" << byteStride << std::dec <<"\n";
    __m128 sum = _mm_setzero_ps ();
    const size_t cols4 = cols >> 2;

    if(Is16ByteAligned(vk) && ((cols & 0x3) == 0)) {
      // Kernel is byte aligned.
      for(size_t i = rows; i > 0; i--) {
        const float *vir = vi; // Image row.
        if(Is16ByteAligned(vir)) {
          for(size_t j = cols4; j > 0; j--) {
            sum = _mm_add_ps(sum,_mm_mul_ps(_mm_load_ps(vk),_mm_load_ps(vir)));
            vk += 4;
            vir += 4;
          }
        } else {
          for(size_t j = cols4; j > 0; j--) {
            sum = _mm_add_ps(sum,_mm_mul_ps(_mm_load_ps(vk),_mm_loadu_ps(vir)));
            vk += 4;
            vir += 4;
          }
        }

        // Add stride bytes.
        vi = reinterpret_cast<const float *>(reinterpret_cast<const char *>(vi) + byteStride);
      }

    } else {
      // Kernel is not byte aligned.
      float remainder = 0;
      for(size_t i = rows; i > 0; i--) {
        const float *vir = vi; // Image row.
        if(Is16ByteAligned(vir)) {
          for(size_t j = cols4; j > 0; j--) {
            sum = _mm_add_ps(sum,_mm_mul_ps(_mm_loadu_ps(vk),_mm_load_ps(vir)));
            vk += 4;
            vir += 4;
          }
        } else {
          for(size_t j = cols4; j > 0; j--) {
            sum = _mm_add_ps(sum,_mm_mul_ps(_mm_loadu_ps(vk),_mm_loadu_ps(vir)));
            vk += 4;
            vir += 4;
          }
        }

        //finish the row
        for(int j = cols & 0x3; j > 0; j--) {
          remainder += *vk * *vir;
          vk++;
          vir++;
        }

        // Add stride bytes.
        vi = reinterpret_cast<const float *>(reinterpret_cast<const char *>(vi) + byteStride);
      }
      sum = _mm_add_ps(sum,_mm_load_ss(&remainder));
    }

    sum = _mm_add_ps(sum,_mm_shuffle_ps(sum,sum, _MM_SHUFFLE(2,3,0,1)));
    sum = _mm_add_ps(sum,_mm_shuffle_ps(sum,sum, _MM_SHUFFLE(1,0,3,2)));

    _mm_store_ss(result,sum);
  }

  void SSEConvolveKernelQuadF(const float *vi, // Scanned image, probably not aligned.
                              const float *vk1, // Kernel, expected to be aligned.
                              const float *vk2, // Kernel, expected to be aligned.
                              size_t rows,
                              size_t cols,
                              int byteStride,
                              float *result
                         )
  {
    //std::cerr << "Rows=" << rows << " Cols=" << cols << " vk=" << std::hex << (void*) vk << " vi=" << (void*) vi << " Stride=" << byteStride << std::dec <<"\n";
    __m128 sum = _mm_setzero_ps ();
    const size_t cols4 = cols >> 2;

    if(Is16ByteAligned(vk1) && Is16ByteAligned(vk2) && ((cols & 0x3) == 0)) {
      // Kernels are byte aligned.
      for(size_t i = rows; i > 0; i--) {
        const float *vir = vi; // Image row.
        if(Is16ByteAligned(vir)) {
          for(size_t j = cols4; j > 0; j--) {
            const __m128 val = _mm_load_ps(vir);
            sum = _mm_add_ps(sum,
                             _mm_mul_ps(val,
                                        _mm_add_ps(_mm_mul_ps(val,
                                                              _mm_load_ps(vk2)),
                                                   _mm_load_ps(vk1))));
            vk1 += 4;
            vk2 += 4;
            vir += 4;
          }
        } else {
          for(size_t j = cols4; j > 0; j--) {
            const __m128 val = _mm_loadu_ps(vir);
            sum = _mm_add_ps(sum,
                             _mm_mul_ps(val,
                                        _mm_add_ps(_mm_mul_ps(val,
                                                              _mm_load_ps(vk2)),
                                                   _mm_load_ps(vk1))));
            vk1 += 4;
            vk2 += 4;
            vir += 4;
          }
        }

        // Add stride bytes.
        vi = reinterpret_cast<const float *>(reinterpret_cast<const char *>(vi) + byteStride);
      }
    } else {
      // Kernel is not byte aligned.
      float remainder = 0;
      for(size_t i = rows; i > 0; i--) {
        const float *vir = vi; // Image row.
        if(Is16ByteAligned(vir)) {
          for(size_t j = cols4; j > 0; j--) {
            const __m128 val = _mm_load_ps(vir);
            sum = _mm_add_ps(sum,
                             _mm_mul_ps(val,
                                        _mm_add_ps(_mm_mul_ps(val,
                                                              _mm_loadu_ps(vk2)),
                                                   _mm_loadu_ps(vk1))));
            vk1 += 4;
            vk2 += 4;
            vir += 4;
          }
        } else {
          for(size_t j = cols4; j > 0; j--) {
            const __m128 val = _mm_loadu_ps(vir);
            sum = _mm_add_ps(sum,
                             _mm_mul_ps(val,
                                        _mm_add_ps(_mm_mul_ps(val,
                                                              _mm_loadu_ps(vk2)),
                                                   _mm_loadu_ps(vk1))));
            vk1 += 4;
            vk2 += 4;
            vir += 4;
          }
        }

        //finish the row
        for(int j = cols & 0x3; j > 0; j--) {
          remainder += *vir * (*vk1 + *vir * *vk2);
          vk1++;
          vk2++;
          vir++;
        }

        // Add stride bytes.
        vi = reinterpret_cast<const float *>(reinterpret_cast<const char *>(vi) + byteStride);
      }
      sum = _mm_add_ps(sum,_mm_load_ss(&remainder));
    }

    sum = _mm_add_ps(sum,_mm_shuffle_ps(sum,sum, _MM_SHUFFLE(2,3,0,1)));
    sum = _mm_add_ps(sum,_mm_shuffle_ps(sum,sum, _MM_SHUFFLE(1,0,3,2)));

    _mm_store_ss(result,sum);
  }

  // The following code is based on fast_sse_tanh by lubomir i. ivanov, neolit123 [at] gmail

  static const float g_TANH_RANGE = 5.f;
  static const float g_TANH_CLAMP = 1.f;
  static const float g_TANH_K0 = 21.f;
  static const float g_TANH_K1 = 210.f;
  static const float g_TANH_K2 = 1260.f;
  static const float g_TANH_K3 = 4725.f;
  static const float g_TANH_K4 = 10395.f;

#define v4sf          __m128

  /* sse intrinsic version */
  static inline v4sf fast_tanh_sse(v4sf x)
  {
    v4sf    y, s, d;
    #ifdef _TANH_FAST_DIV
      v4sf  i_d;
    #endif

    /* check each value in the vector */
    #ifdef _TANH_CLAMP_INDIVIDUAL
      short i;
      union
      {
        v4sf    v;
        float   f[4];
      } u;

      i = 0;
      u.v = x;
      while (i < 4)
      {
        if (u.f[i] < -_TANH_RANGE)
          u.f[i] = -_TANH_CLAMP;
        if (u.f[i] > _TANH_RANGE)
          u.f[i] = _TANH_CLAMP;
        i++;
      }
      x = u.v;
    /* clamp entire vector */
    #else
      if      (_mm_movemask_ps(_mm_cmplt_ps(x, _mm_set1_ps(-g_TANH_RANGE))))
        return _mm_set1_ps(-g_TANH_CLAMP);
      else if (_mm_movemask_ps(_mm_cmpgt_ps(x, _mm_set1_ps( g_TANH_RANGE))))
        return _mm_set1_ps( g_TANH_CLAMP);
    #endif

    s = _mm_mul_ps(x, x);

    /* denominator */
    d = _mm_add_ps(s, _mm_set1_ps(g_TANH_K1));
    d = _mm_mul_ps(d, s);
    d = _mm_add_ps(d, _mm_set1_ps(g_TANH_K3));
    d = _mm_mul_ps(d, s);
    d = _mm_add_ps(d, _mm_set1_ps(g_TANH_K4));

    /* numerator */
    y = _mm_mul_ps(s, _mm_set1_ps(g_TANH_K0));
    y = _mm_add_ps(y, _mm_set1_ps(g_TANH_K2));
    y = _mm_mul_ps(y, s);
    y = _mm_add_ps(y, _mm_set1_ps(g_TANH_K4));
    y = _mm_mul_ps(y, x);

    #ifdef _TANH_FAST_DIV
      /* reciprocal of the denominator with one newton iteration */
      i_d = _mm_rcp_ps(d);
      i_d = _mm_sub_ps( _mm_add_ps(i_d, i_d),
                        _mm_mul_ps(d, _mm_mul_ps(i_d, i_d)));
      return _mm_mul_ps(y, i_d);
    #else
      return _mm_div_ps(y, d);
    #endif
  }

  static void SSEFastTanhF(float *v1,const float *v2,size_t n)
  {
    if(n < 4) {
      // Not worth using this code for small vectors.
      for(unsigned int i=n; i>0; --i)
        *v1++ = fast_tanh(*v2++);
      return;
    }
    UIntT quadLen = (n & ~0x3);
    const float* const ewPtr = v1 + quadLen;

    if(Is16ByteAligned(v1)) {  // v1 16-byte aligned ?
      if(Is16ByteAligned(v2)) {// v2 16-byte aligned ?
        while(v1 != ewPtr) {
          _mm_store_ps(v1,fast_tanh_sse(_mm_load_ps(v2)));
          v1 += 4;
          v2 += 4;
        }
      } else {
        while(v1 != ewPtr) {
          _mm_store_ps(v1,fast_tanh_sse(_mm_loadu_ps(v2)));
          v1 += 4;
          v2 += 4;
        }
      }
    } else {
      if(Is16ByteAligned(v2)) {// v2 16-byte aligned ?
        while(v1 != ewPtr) {
          _mm_storeu_ps(v1,fast_tanh_sse(_mm_load_ps(v2)));
          v1 += 4;
          v2 += 4;
        }
      } else {
        while(v1 != ewPtr) {
          _mm_storeu_ps(v1,fast_tanh_sse(_mm_loadu_ps(v2)));
          v1 += 4;
          v2 += 4;
        }
      }
    }

    UIntT remainder = n - quadLen;
    // Add in leftovers
    for(;remainder > 0;remainder--) {
      *(v1++) = fast_tanh(*(v2++));
    }
  }


  int VectorSSEInit() {
    if (SSE2() && 1) {
      g_DotProductD = &SSEDotProductD;
      g_DotProductF = &SSEDotProductF;
      g_QuadProductD = &SSEQuadProductD;
      g_QuadProductF = &SSEQuadProductF;
      g_MatrixMulVectorF = &SSEMatrixMulVectorF;
      g_MatrixMulVectorAddF = &SSEMatrixMulVectorAddF;
      g_MatrixMulVectorD = &SSEMatrixMulVectorD;
      g_Real2ByteD = &SSEReal2ByteD;
      g_ConvolveKernelF = &SSEConvolveKernelF;
      g_ConvolveKernelQuadF = &SSEConvolveKernelQuadF;
      g_fastTanhF = &SSEFastTanhF;
      //cerr<<"SSE:yes\n";
    } else {
      //cerr<<"SSE:no\n";
    }
    return 0;
  }
  
  static int a = VectorSSEInit();
  
}
