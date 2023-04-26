#ifndef _PNEB_HPP_
#define _PNEB_HPP_

#pragma once

/* Pneb.h
   Author - Eric Bylaska
*/

#pragma once

#include "Control2.hpp"
#include "Lattice.hpp"
#include "PGrid.hpp"
#include "Parallel.hpp"
#include "d1db.hpp"
#include "gdevice.hpp"
#include "nwpw_timing.hpp"
#include "util.hpp"

namespace pwdft {

class Pneb : public PGrid, public d1db {

  // int ispin,ne[2],neq[2];
  int parallelized;
  double *s22, *s21, *s12, *s11, *sa1, *sa0, *st1;

  int *ma[2], *ma1[2], *ma2[2], *mc[2], *na[2], *nc[2];
  int mcq[2], ncq[2];
  int ncqmax;
  int g_rnd_algorihm = 1;

public:
  /* constructors */
  Pneb(Parallel *, Lattice *, Control2 &, int, int *);

  /* destructor */
  ~Pneb() {
    delete[] s22;
    if (parallelized)
      for (int ms = 0; ms < ispin; ++ms) {
        delete[] ma[ms];
        delete[] ma1[ms];
        delete[] ma2[ms];
        delete[] mc[ms];
        delete[] na[ms];
        delete[] nc[ms];
      }
  }

  void g_generate_random(double *);
  void g_generate1_random(double *);
  void g_generate2_random(double *);
  void g_read(const int, double *);
  void g_write(const int, double *);

  double *g_allocate(const int nb) {
    double *ptr;
    ptr = new (std::nothrow) double[2 * (neq[0] + neq[1]) * PGrid::npack(nb)]();
    return ptr;
  }
  void g_deallocate(double *ptr) { delete[] ptr; }

  double *g_nallocate(const int nb, const int nblock) {
    double *ptr;
    ptr = new (std::nothrow) double[nblock * 2 * (neq[0] + neq[1]) *
                                    PGrid::npack(nb)]();
    return ptr;
  }

  double *h_allocate() {
    double *ptr;
    ptr = new (std::nothrow) double[(neq[0] + neq[1]) * n2ft3d]();
    return ptr;
  }
  void h_deallocate(double *ptr) { delete[] ptr; }

  int m_size(const int mb) {
    int nsize;
    if (mb == -1)
      nsize = ne[0] * ne[0] + ne[1] * ne[1];
    else
      nsize = ne[mb] * ne[mb];
    return nsize;
  }
  double *m_allocate(const int mb, const int nblock) {
    double *ptr;
    int nsize;
    if (mb == -1)
      nsize = ne[0] * ne[0] + ne[1] * ne[1];
    else
      nsize = ne[mb] * ne[mb];

    ptr = new (std::nothrow) double[nblock * nsize]();
    return ptr;
  }
  void m_deallocate(double *ptr) { delete[] ptr; }

  double *w_allocate(const int mb, const int nblock) {
    double *ptr;
    int nsize;
    if (mb == -1)
      nsize = 2 * (ne[0] * ne[0] + ne[1] * ne[1]);
    else
      nsize = 2 * ne[mb] * ne[mb];

    ptr = new (std::nothrow) double[nblock * nsize]();
    return ptr;
  }
  void w_deallocate(double *ptr) { delete[] ptr; }

  double *m4_allocate(const int mb, const int nblock) {
    double *ptr;
    int nsize;
    if (mb == -1)
      nsize = 4 * (ne[0] * ne[0] + ne[1] * ne[1]);
    else
      nsize = 4 * ne[mb] * ne[mb];

    ptr = new (std::nothrow) double[nblock * nsize]();
    return ptr;
  }
  void m4_deallocate(double *ptr) { delete[] ptr; }

  double gg_traceall(double *, double *);
  void gg_copy(double *, double *);
  void g_zero(double *);
  void hr_aSumSqr(const double, double *, double *);
  void hhr_aSumMul(const double, const double *, const double *, double *);

  void ggm_sym_Multiply(double *, double *, double *);
  void ggm_Multiply(double *, double *, double *);
  void ffm_sym_Multiply(const int, double *, double *, double *);
  void ffm_Multiply(const int, double *, double *, double *);
  void ggm_SVD(double *, double *, double *, double *);

  void ffm4_sym_Multiply(const int, double *, double *, double *, double *,
                         double *, double *);
  void ffm3_sym_Multiply(const int, double *, double *, double *, double *,
                         double *);
  void m_scale_s22_s21_s12_s11(const int, const double, double *s22,
                               double *s21, double *s12, double *s11);
  void m_scale_s22_s21_s11(const int, const double, double *s22, double *s21,
                           double *s11);
  void fmf_Multiply(const int, double *, double *, double, double *, double);

  void fm_QR(const int, double *, double *);

  void mmm4_AR_to_T4(const int, const double *, const double *, double *);
  void m4_FactorSkew(const int, double *, double *, double *, double *S);
  void m4_RotationSkew(const int, const double, double *, double *, double *,
                       double *, double *, double *);
  void m4_R4_to_MN(const int, const double *, double *, double *);

  void mm_SCtimesVtrans(const int, const double, double *, double *, double *,
                        double *, double *, double *);
  void mm_SCtimesVtrans2(const int, const double, double *, double *, double *,
                         double *, double *, double *);
  void mm_SCtimesVtrans3(const int, const double, double *, double *, double *,
                         double *, double *, double *);

  void m_scal(const double, double *);
  double m_trace(double *);
  void m_diagonalize(double *, double *);
  void mmm_Multiply(const int, double *, double *, double, double *, double);
  void mmm_Multiply2(const int, double *, double *, double, double *, double);
  void mm_transpose(const int, double *, double *);
  void mm_Kiril_Btransform(const int, double *, double *);

  void gh_fftb(double *, double *);
  void ggm_lambda(double, double *, double *, double *);
  // void ggm_lambda2(double, double *, double *, double *);
  void ggm_lambda_sic(double, double *, double *, double *);
  void g_ortho(double *);

  void gg_SMul(double, double *, double *);
  // void g_SMul1(double, double *);
  void g_Scale(double, double *);
  void gg_Sum2(double *, double *);
  void gg_Minus2(double *, double *);
  void ggg_Minus(double *, double *, double *);

  void gg_daxpy(double, double *, double *);
};
} // namespace pwdft

#endif
