// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlLapackWraps
//! file="Ravl/Contrib/IntelMKL/testIntelFFT2d.cc"

#include "Ravl/Lapack/lapack.hh"
#include "Ravl/SVD.hh"
#include "Ravl/StrStream.hh"
#include "Ravl/SVD.hh"
#include "Ravl/Eigen.hh"
#include "Ravl/OS/Date.hh"

using namespace RavlN;

int testLapackMatrixInverse();
int testLapackEigen();
int testLapackSVD();
int testTimings();

int main()
{
  int ln;
#if 1
  if ((ln = testLapackMatrixInverse()) != 0) {
    cerr << "Error line " << ln << "\n";
    return 1;
  }
#endif
#if 1
  if ((ln = testLapackEigen()) != 0) {
    cerr << "Error line " << ln << "\n";
    return 1;
  }
#endif
#if 1
  if ((ln = testLapackSVD()) != 0) {
    cerr << "Error line " << ln << "\n";
    return 1;
  }
#endif
#if 0
  if ((ln = testTimings()) != 0) {
    cerr << "Error line " << ln << "\n";
    return 1;
  }
#endif

  cout << "Test passed. \n";
  return 0;
}

bool testInverse(MatrixC & m)
{
  // Compute inverse using Ravl
  // Make sure we use RAVL standard inverse function
  RealT det;
  MatrixC ravlInverse(m.Copy());
  InverseIP_GaussJordan(ravlInverse, det);

  // Compute inverse using Intel
  if (!InverseIP_Lapack(m)) {
    return false;
  }

  // Compare results and they should be roughly equal
  RealT err = (ravlInverse.AsVector() - m.AsVector()).SumOfSqr();
  if (err > 0.0000001) {
    return false;
  }

  return true;
}

int testLapackMatrixInverse()
{
  cerr << "Testing Lapack Matrix Inverse...";

  RavlN::MatrixC test;
  // Simple test
  StrIStreamC("3 3 "
      "1.0 0.0 0.0"
      "0.0 2.0 0.0"
      "0.0 0.0 1.0") >> test;

  if (!testInverse(test)) {
    return __LINE__;
  }

  // Slightly bigger test
  StrIStreamC("5 5 "
      "1.0 0.0 0.0 1.0 1.4"
      "0.0 2.0 0.0 3.0 7.0"
      "9.0 0.0 1.0 9.0 7.0"
      "1.0 2.0 4.0 1.0 1.0"
      "4.0 1.2 4.3 2.3 1.3") >> test;

  if (!testInverse(test)) {
    return __LINE__;
  }

  // Even bigger test
  for (UIntT i = 1; i < 100; i++) {
    test = RandomMatrix(10, 10);
    if (!testInverse(test)) {
      return __LINE__;
    }
  }
  cerr << "passed!\n";
  return 0;
}


bool testEigen(MatrixC & m)
{
  MatrixC orig(m.Copy());

  // Compute using standard Ravl way
  EigenValueC<RealT> ev(m);

  // Check we get back to original matrix
  MatrixC eval;
  ev.getD(eval);
  MatrixC evec = ev.EigenVectors();
  MatrixC m2 = evec * eval * evec.T();
  RealT diff = (m2 - orig).AsVector().Sum();
  if (diff > 0.0000001) {
    return false;
  }

  // Compute using Lapack
  VectorMatrixC vm;
  if (!EigenVectors_Lapack(m, vm)) {
    return false;
  }
  MatrixC evm(vm.Matrix().Rows(), vm.Matrix().Cols());
  evm.Fill(0.0);
  evm.SetDiagonal(vm.Vector());
  MatrixC m3 = vm.Matrix() * evm * vm.Matrix().T();
  RealT diff2 = (m3 - orig).AsVector().SumOfSqr();
  if (diff2 > 0.0000001) {
    return false;
  }

  // Check eigen values are roughly equal
  RealT err1 = (vm.Vector() - eval.Diagonal()).SumOfSqr();
  if (err1 > 0.0000001) {
    return false;
  }
  return true;
}

int testLapackEigen()
{

  cerr << "Testing Lapack EigenVectors...";
  RavlN::MatrixC test;

  // Simple test
  StrIStreamC("3 3 "
      "1.0 0.0 0.0 "
      "0.0 2.0 0.0 "
      "0.0 0.0 1.0") >> test;

  if (!testEigen(test)) {
    return __LINE__;
  }

  // Even bigger test
  for (UIntT i = 1; i < 10; i++) {
    test = RandomPositiveDefiniteMatrix(10 * i);
    if (!testEigen(test)) {
      return __LINE__;
    }
  }
  cerr << "passed!\n";
  return 0;
}



bool testSVD(MatrixC & m)
{


  // Keep an original copy of the matrix
  MatrixC orig = m.Copy();

  // Want to compare it with the original Ravl one
  SVDC<RealT> svd(m);
  MatrixC vRavl = svd.GetV();
  MatrixC uRavl = svd.GetU();
  VectorC sRavl = svd.SingularValues();

  // Check we get back to original matrix
  MatrixC md(uRavl.Cols(), sRavl.Size());
  md.Fill(0);
  md.SetDiagonal(sRavl);
  MatrixC m2 = uRavl * md * vRavl.T();
  if((m2 - orig).AsVector().Sum() > 0.0000001) {
    return false;
  }

  //cerr << uRavl << endl;
  //cerr << sRavl << endl;
  //cerr << vRavl << endl;

  // Next compute the one using Lapack routine
  VectorC s;
  MatrixC u;
  MatrixC v;
  if (!SVD_IP_Lapack(m, u, s, v)) {
    return false;
  }

  //cerr << u << endl;
  //cerr << s << endl;
  //cerr << v << endl;

  // The first check we can do is to make sure the
  // singular values add up-to nearly the same.
  RealT sErr = (sRavl - s).SumOfSqr();
  if (sErr > 0.000001) {
    return false;
  }

  // The final check we can do is to make sure we can reconstruct
  // our original matrix
  MatrixC sv(u.Cols(), s.Size());
  sv.Fill(0.0);
  sv.SetDiagonal(s);
  MatrixC reconstruct = u * sv * v.T();
  RealT diff = (reconstruct - orig).AsVector().SumOfSqr();
  if (diff > 0.0000001) {
    return false;
  }

  // All looking good
  return true;
}

int testLapackSVD()
{
  cerr << "Testing Lapack SVD...";
  RavlN::MatrixC test;

  // Simple test
  StrIStreamC("6 5 "
      "8.79  9.93  9.83 5.45  3.16 "
      "6.11  6.91  5.04 -0.27  7.98 "
      "-9.15 -7.93  4.86 4.85  3.01 "
      " 9.57  1.64  8.83 0.74  5.80 "
      "-3.49  4.02  9.80 10.00  4.27 "
      "9.84  0.15 -8.99 -6.02 -5.31") >> test;

  if (!testSVD(test)) {
    return __LINE__;
  }


  // Even bigger test
  for (IntT i = -5; i < 5; i++) {
    test = RandomMatrix(100+i, 100);
    if (!testSVD(test)) {
      return __LINE__;
    }
  }
  cerr << "passed!\n";
  return 0;
}

int testTimings()
{
  cerr << "Test timings of Lapack routine routines..." << endl;
  MatrixC test;
  DateC now;
  DateC finish;
  OStreamC os("tests.txt");
  os << "dim, inv_intel, inv_ravl, svd_intel, svd_ravl, eigen_intel, eigen_ravl" << endl;
  for (UIntT i = 1; i < 50; i+=5) {
    RavlN::MatrixC orig = RandomMatrix(50*i, 50*i);
    os << orig.Rows();
    cerr << "Tests on " << orig.Rows() << "x" << orig.Cols() << " dimension matrix." << endl;
#if 1
    cerr << "Timing Inverse" << endl;
    cerr << " - Lapack...";
    test = orig.Copy();
    now = DateC::NowUTC();
    InverseIP_Lapack(test);
    finish = DateC::NowUTC();
    cerr << (finish - now).Double() << endl;
    os << "," << (finish - now).Double();

    cerr << " - Ravl...";
    test = orig.Copy();
    RealT det;
    now = DateC::NowUTC();
    InverseIP_GaussJordan(test, det);
    finish = DateC::NowUTC();
    os << "," << (finish - now).Double();
    cerr << (finish - now).Double() << endl;
#endif

#if 1
    cerr << "Timing SVD" << endl;
    cerr << " - Lapack...";
    test = orig.Copy();
    now = DateC::NowUTC();
    MatrixC u, v;
    VectorC s;
    SVD_IP_Lapack(test, u, s, v);
    finish = DateC::NowUTC();
    os << "," << (finish - now).Double();
    cerr << (finish - now).Double() << endl;

    cerr << " - Ravl...";
    test = orig.Copy();
    now = DateC::NowUTC();
    SVDC<RealT> svd(test);
    finish = DateC::NowUTC();
    os << "," << (finish - now).Double();
    cerr << (finish - now).Double() << endl;
#endif

#if 1
    cerr << "Timing Eigen" << endl;
    orig = RandomPositiveDefiniteMatrix(orig.Rows());
    cerr << " - Lapack...";
    test = orig.Copy();
    now = DateC::NowUTC();
    VectorMatrixC vm;
    EigenVectors_Lapack(test, vm);
    finish = DateC::NowUTC();
    os << "," << (finish - now).Double();
    cerr << (finish - now).Double() << endl;

    cerr << " - Ravl...";
    test = orig.Copy();
    now = DateC::NowUTC();
    EigenValueC<RealT> eigen(test);
    finish = DateC::NowUTC();
    os << "," << (finish - now).Double();
    cerr << (finish - now).Double() << endl;
#endif
    os << "\n";
  }
  os.Close();
  return 0;
}


