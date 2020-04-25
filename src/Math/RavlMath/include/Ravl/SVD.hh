#ifndef RAVL_SVD_HEADER
#define RAVL_SVD_HEADER 1
//! license=own
//! docentry="Ravl.API.Math.Linear Algebra.Matrix Decomposition"
//! rcsid="$Id$"
//! lib=RavlMath
//! file="Ravl/Math/LinearAlgebra/General/SVD.hh"

#include "Ravl/TMatrix.hh"
#include "Ravl/TVector.hh"
#include "Ravl/Math.hh"

namespace RavlN
{
  //! userlevel=Normal
  //: Singular Value Decomposition.
  //<P>
  //    For an m-by-n matrix A with m >= n, the singular value decomposition is
  //    an m-by-n orthogonal matrix U, an n-by-n diagonal matrix S, and
  //    an n-by-n orthogonal matrix V so that A = U*S*V'.
  //<P>
  //    The singular values, sigma[k] = S[k][k], are ordered so that
  //    sigma[0] >= sigma[1] >= ... >= sigma[n-1].
  //<P>
  //    The singular value decompostion always exists, so the constructor will
  //    never fail.  The matrix condition number and the effective numerical
  //    rank can be computed from this decomposition.
  //
  //<p>
  //    (Adapted from <a href =
  //    "http://math.nist.gov/javanumerics/jama">JAMA</a>, a Java Matrix
  //    Library, developed jointly by the Mathworks and NIST.)
  
  template <class NumT>
  class SVDC
  {
  public:
    SVDC (const TMatrixC<NumT> &arg,bool want_u = true,bool want_v = true) 
      :  wantu(want_u),
	 wantv(want_v)
    {
      bool shortnfat = (arg.Size1()<arg.Size2());
      TMatrixC<NumT> mat(arg.Copy());
      if (shortnfat) mat = mat.T();
      m = mat.Size1();
      n = mat.Size2();
      int nu = Min(m,n);
      
      int nss = Min(m+1,n);
      s = TVectorC<NumT>(nss);
      U = TMatrixC<NumT>(m, nu, NumT(0));
      V = TMatrixC<NumT>(n,n);
      
      DoSVD(mat);
      if (shortnfat)
        Swap(U, V);
    }
    //: Setup SVD of 'arg'
    //!param: arg - Matrix to decompose
    //!param: want_u - Flag to indicate if we need to compute U matrix.
    //!param: want_v - Flag to indicate if we need to compute V matrix.
    
    SVDC (const TMatrixC<NumT> &arg,TVectorC<NumT> &ns,TMatrixC<NumT> &nU,TMatrixC<NumT> &nV,bool want_u = true,bool want_v = true) 
      :  wantu(want_u),
	 wantv(want_v)
    {
      bool shortnfat = (arg.Size1()<arg.Size2());
      TMatrixC<NumT> mat(arg.Copy());
      if (shortnfat) mat = mat.T();
      m = mat.Size1();
      n = mat.Size2();
      
      int nss = Min(m+1,n);
      RavlAssert(nss >= (int) ns.Size());
      s = TVectorC<NumT>(SArray1dC<NumT>(ns,nss));
      
      RavlAssert(((int) nU.Size1() == m && (int) nU.Size2() == Min(m,n)) || (!wantu));
      U = nU;
      U.Fill(0);
      
      RavlAssert(((int) nV.Size1() == n && (int) nV.Size2() == n) || (!wantv));
      V = nV;
      
      DoSVD(mat);
      if (shortnfat) 
        Swap(U, V);
    }
    //: Setup SVD of 'arg'
    //!param: arg - Matrix to decompose
    //!param: ns - Vector to store singular values.
    //!param: nU - Matrix to store U result in. (If want_u is false, can be empty).
    //!param: nV - Matrix to store V result in. (If want_v is false, can be empty).
    //!param: want_u - Flag to indicate if we need to compute U matrix.
    //!param: want_v - Flag to indicate if we need to compute V matrix.
    
  protected:
    void DoSVD(TMatrixC<NumT> &A) {
      TVectorC<NumT> e(n);
      TVectorC<NumT> work(m);
      int nu = Min(m,n);
      int i=0, j=0, k=0;
      
#if 0
      if(m < n)
	cerr << "SVDC, This SVD code it not reliable where m < n.\n";
      // But this is now fixed in the constructors [Bill Xmas]
#endif
      
      // Reduce A to bidiagonal form, storing the diagonal elements
      // in s and the super-diagonal elements in e.

      int nct = Min(m-1,n);
      int nrt = Max(0,Min(n-2,m));
      for (k = 0; k < Max(nct,nrt); k++) {
	if (k < nct) {

	  // Compute the transformation for the k-th column and
	  // place the k-th diagonal in s[k].
	  // Compute 2-norm of k-th column without under/overflow.
	  s[k] = 0;
	  for (i = k; i < m; i++) {
	    s[k] = Hypot(s[k],A[i][k]);
	  }
	  if (s[k] != 0.0) {
	    if (A[k][k] < 0.0) {
	      s[k] = -s[k];
	    }
	    for (i = k; i < m; i++) {
	      A[i][k] /= s[k];
	    }
	    A[k][k] += 1.0;
	  }
	  s[k] = -s[k];
	}
	for (j = k+1; j < n; j++) {
	  if ((k < nct) && (s[k] != 0.0))  {

            // Apply the transformation.

	    NumT t = 0;
	    for (i = k; i < m; i++) {
	      t += A[i][k]*A[i][j];
	    }
	    t = -t/A[k][k];
	    for (i = k; i < m; i++) {
	      A[i][j] += t*A[i][k];
	    }
	  }

	  // Place the k-th row of A into e for the
	  // subsequent calculation of the row transformation.

	  e[j] = A[k][j];
	}
	if (wantu && (k < nct)) {

	  // Place the transformation in U for subsequent back
	  // multiplication.

	  for (i = k; i < m; i++) {
	    U[i][k] = A[i][k];
	  }
	}
	if (k < nrt) {

	  // Compute the k-th row transformation and place the
	  // k-th super-diagonal in e[k].
	  // Compute 2-norm without under/overflow.
	  e[k] = 0;
	  for (i = k+1; i < n; i++) {
	    e[k] = Hypot(e[k],e[i]);
	  }
	  if (e[k] != 0.0) {
	    if (e[k+1] < 0.0) {
	      e[k] = -e[k];
	    }
	    for (i = k+1; i < n; i++) {
	      e[i] /= e[k];
	    }
	    e[k+1] += 1.0;
	  }
	  e[k] = -e[k];
	  if ((k+1 < m) && (e[k] != 0.0)) {

            // Apply the transformation.

	    for (i = k+1; i < m; i++) {
	      work[i] = 0.0;
	    }
	    for (j = k+1; j < n; j++) {
	      for (i = k+1; i < m; i++) {
		work[i] += e[j]*A[i][j];
	      }
	    }
	    for (j = k+1; j < n; j++) {
	      NumT t = -e[j]/e[k+1];
	      for (i = k+1; i < m; i++) {
		A[i][j] += t*work[i];
	      }
	    }
	  }
	  if (wantv) {

            // Place the transformation in V for subsequent
            // back multiplication.

	    for (i = k+1; i < n; i++) {
	      V[i][k] = e[i];
	    }
	  }
	}
      }

      // Set up the final bidiagonal matrix or order p.

      int p = Min(n,m+1);
      if (nct < n) {
	s[nct] = A[nct][nct];
      }
      if (m < p) {
	s[p-1] = 0.0;
      }
      if (nrt+1 < p) {
	e[nrt] = A[nrt][p-1];
      }
      e[p-1] = 0.0;

      // If required, generate U.

      if (wantu) {
	for (j = nct; j < nu; j++) {
	  for (i = 0; i < m; i++) {
	    U[i][j] = 0.0;
	  }
	  U[j][j] = 1.0;
	}
	for (k = nct-1; k >= 0; k--) {
	  if (s[k] != 0.0) {
	    for (j = k+1; j < nu; j++) {
	      NumT t = 0;
	      for (i = k; i < m; i++) {
		t += U[i][k]*U[i][j];
	      }
	      t = -t/U[k][k];
	      for (i = k; i < m; i++) {
		U[i][j] += t*U[i][k];
	      }
	    }
	    for (i = k; i < m; i++ ) {
	      U[i][k] = -U[i][k];
	    }
	    U[k][k] = 1.0 + U[k][k];
	    for (i = 0; i < k-1; i++) {
	      U[i][k] = 0.0;
	    }
	  } else {
	    for (i = 0; i < m; i++) {
	      U[i][k] = 0.0;
	    }
	    U[k][k] = 1.0;
	  }
	}
      }

      // If required, generate V.

      if (wantv) {
	for (k = n-1; k >= 0; k--) {
	  if ((k < nrt) && (e[k] != 0.0)) {
	    for (j = k+1; j < nu; j++) {
	      NumT t = 0;
	      for (i = k+1; i < n; i++) {
		t += V[i][k]*V[i][j];
	      }
	      t = -t/V[k+1][k];
	      for (i = k+1; i < n; i++) {
		V[i][j] += t*V[i][k];
	      }
	    }
	  }
	  for (i = 0; i < n; i++) {
	    V[i][k] = 0.0;
	  }
	  V[k][k] = 1.0;
	}
      }

      // Main iteration loop for the singular values.

      int pp = p-1;
      int iter = 0;

      NumT eps = Pow(2.0,-52.0);
      NumT tiny = Pow(2.0,-966.0);
      // Vars to allow algorithm to cope with rounding in floating point representations.
      // 2^-52 comes from IEEE Double possessing 52 mantissa bits, other than the original
      // (updated) JAMA source, I do not recognise the 2^-966 magic number other than it
      // is a remarkably small number and addition of "tiny" to the algorithm does indeed
      // correct a problem with certain input (see test program).
      //
      // N.B. Although this is templated code, the algorithm wasn't specifically written
      // with non-IEEE doubles in mind. "Your milage may vary" with other numeric types.
      // For example, if using non-IEEE floating point (or quad precision) the magic
      // values for eps and tiny may need modifying.
      //
      while (p > 0) {
	int k=0;
	int kase=0;

	// Here is where a test for too many iterations would go.

	// This section of the program inspects for
	// negligible elements in the s and e arrays.  On
	// completion the variables kase and k are set as follows.
	
	// kase = 1     if s(p) and e[k-1] are negligible and k<p
	// kase = 2     if s(k) is negligible and k<p
	// kase = 3     if e[k-1] is negligible, k<p, and
	//              s(k), ..., s(p) are not negligible (qr step).
	// kase = 4     if e(p-1) is negligible (convergence).

	for (k = p-2; k >= -1; k--) {
	  if (k == -1) {
	    break;
	  }
	  if (Abs(e[k]) <= tiny + eps*(Abs(s[k]) + Abs(s[k+1]))) {
	    e[k] = 0.0;
	    break;
	  }
	}
	if (k == p-2) {
	  kase = 4;
	} else {
	  int ks;
	  for (ks = p-1; ks >= k; ks--) {
	    if (ks == k) {
	      break;
	    }
	    NumT t = (ks != p ? Abs(e[ks]) : 0.) + 
	      (ks != k+1 ? Abs(e[ks-1]) : 0.);
	    if (Abs(s[ks]) <= tiny + eps*t)  {
	      s[ks] = 0.0;
	      break;
	    }
	  }
	  if (ks == k) {
	    kase = 3;
	  } else if (ks == p-1) {
	    kase = 1;
	  } else {
	    kase = 2;
	    k = ks;
	  }
	}
	k++;

	// Perform the task indicated by kase.

	switch (kase) {

	  // Deflate negligible s(p).

	case 1: {
	  NumT f = e[p-2];
	  e[p-2] = 0.0;
	  for (j = p-2; j >= k; j--) {
	    NumT t = Hypot(s[j],f);
	    NumT cs = s[j]/t;
	    NumT sn = f/t;
	    s[j] = t;
	    if (j != k) {
	      f = -sn*e[j-1];
	      e[j-1] = cs*e[j-1];
	    }
	    if (wantv) {
	      for (i = 0; i < n; i++) {
		t = cs*V[i][j] + sn*V[i][p-1];
		V[i][p-1] = -sn*V[i][j] + cs*V[i][p-1];
		V[i][j] = t;
	      }
	    }
	  }
	}
	  break;

	  // Split at negligible s(k).

	case 2: {
	  NumT f = e[k-1];
	  e[k-1] = 0.0;
	  for (j = k; j < p; j++) {
	    NumT t = Hypot(s[j],f);
	    NumT cs = s[j]/t;
	    NumT sn = f/t;
	    s[j] = t;
	    f = -sn*e[j];
	    e[j] = cs*e[j];
	    if (wantu) {
	      for (i = 0; i < m; i++) {
		t = cs*U[i][j] + sn*U[i][k-1];
		U[i][k-1] = -sn*U[i][j] + cs*U[i][k-1];
		U[i][j] = t;
	      }
	    }
	  }
	}
	  break;

	  // Perform one qr step.

	case 3: {

	  // Calculate the shift.
   
	  NumT scale = Max(Max(Max(Max(
					Abs(s[p-1]),Abs(s[p-2])),Abs(e[p-2])), 
				Abs(s[k])),Abs(e[k]));
	  NumT sp = s[p-1]/scale;
	  NumT spm1 = s[p-2]/scale;
	  NumT epm1 = e[p-2]/scale;
	  NumT sk = s[k]/scale;
	  NumT ek = e[k]/scale;
	  NumT b = ((spm1 + sp)*(spm1 - sp) + epm1*epm1)/2.0;
	  NumT c = (sp*epm1)*(sp*epm1);
	  NumT shift = 0.0;
	  if ((b != 0.0) || (c != 0.0)) {
	    shift = sqrt(b*b + c);
	    if (b < 0.0) {
	      shift = -shift;
	    }
	    shift = c/(b + shift);
	  }
	  NumT f = (sk + sp)*(sk - sp) + shift;
	  NumT g = sk*ek;
   
	  // Chase zeros.
   
	  for (j = k; j < p-1; j++) {
	    NumT t = Hypot(f,g);
	    NumT cs = f/t;
	    NumT sn = g/t;
	    if (j != k) {
	      e[j-1] = t;
	    }
	    f = cs*s[j] + sn*e[j];
	    e[j] = cs*e[j] - sn*s[j];
	    g = sn*s[j+1];
	    s[j+1] = cs*s[j+1];
	    if (wantv) {
	      for (i = 0; i < n; i++) {
		t = cs*V[i][j] + sn*V[i][j+1];
		V[i][j+1] = -sn*V[i][j] + cs*V[i][j+1];
		V[i][j] = t;
	      }
	    }
	    t = Hypot(f,g);
	    cs = f/t;
	    sn = g/t;
	    s[j] = t;
	    f = cs*e[j] + sn*s[j+1];
	    s[j+1] = -sn*e[j] + cs*s[j+1];
	    g = sn*e[j+1];
	    e[j+1] = cs*e[j+1];
	    if (wantu && (j < m-1)) {
	      for (i = 0; i < m; i++) {
		t = cs*U[i][j] + sn*U[i][j+1];
		U[i][j+1] = -sn*U[i][j] + cs*U[i][j+1];
		U[i][j] = t;
	      }
	    }
	  }
	  e[p-2] = f;
	  iter = iter + 1;
	}
	  break;

	  // Convergence.

	case 4: {

	  // Make the singular values positive.
   
	  if (s[k] <= 0.0) {
	    s[k] = (s[k] < 0.0 ? -s[k] : 0.0);
	    if (wantv) {
	      for (i = 0; i <= pp; i++) {
		V[i][k] = -V[i][k];
	      }
	    }
	  }
   
	  // Order the singular values.
   
	  while (k < pp) {
	    if (s[k] >= s[k+1]) {
	      break;
	    }
	    NumT t = s[k];
	    s[k] = s[k+1];
	    s[k+1] = t;
	    if (wantv && (k < n-1)) {
	      for (i = 0; i < n; i++) {
		t = V[i][k+1]; V[i][k+1] = V[i][k]; V[i][k] = t;
	      }
	    }
	    if (wantu && (k < m-1)) {
	      for (i = 0; i < m; i++) {
		t = U[i][k+1]; U[i][k+1] = U[i][k]; U[i][k] = t;
	      }
	    }
	    k++;
	  }
	  iter = 0;
	  p--;
	}
	  break;
	}
      }
    }
  public:
    
    TMatrixC<NumT> GetU () 
    { return U; }
    //: Generate matrix U.
    
    TMatrixC<NumT> &GetV()
    { return V; }
    //: Return the right singular vectors 
    
    TVectorC<NumT> &SingularValues()
    { return s; }
    //: Return the one-dimensional array of singular values
    
    TMatrixC<NumT> GetS() {
      TMatrixC<NumT> A(n,n,0.0);
      for (int i = 0; i < n; i++)
	A[i][i] = s[i];
      return A;
    }
    //: Generate the diagonal matrix of singular values
    // return S
    
    RealT Norm2 () const
    { return s[0]; }
    //: Two norm  (Max(S))
    
    RealT ConditionNumber() const
    { return s[0]/s[Min(m,n)-1]; }
    //: Two norm of condition number (Max(S)/Min(S)) 
    
    IntT Rank () const {
      RealT eps = Pow(2.0,-52.0);
      RealT tol = Max(m,n)*s[0]*eps;
      int r = 0; 
      for (unsigned int i = 0; i < s.Size(); i++) {
	if (s[i] > tol)
	  r++;
      }
      return r;
    }
    //: Effective numerical matrix rank
    // Returns the number of nonnegligible singular values.

  protected:
    TMatrixC<NumT> U, V;
    TVectorC<NumT> s;
    int m, n;
    bool wantu;
    bool wantv;
  };

}
#endif
