#ifndef _PGRID_HPP_
#define _PGRID_HPP_

#pragma once

/* d3db.h
   Author - Eric Bylaska

*/

#pragma once

#include "Balance.hpp"
#include "Lattice.hpp"
#include "Parallel.hpp"
#include "d3db.hpp"
#include <cmath>

namespace pwdft {

class PGrid : public d3db {

  Balance *mybalance;
  int balanced;
  double *Garray, *Gpack[2];
  double Gmax, Gmin;
  int *masker[2], *packarray[2];
  int nwave[2], nwave_entire[2], nwave_all[2], nida[2], nidb[2], nidb2[2];

  /* pfft data */
  bool *zero_row2[2], *zero_row3[2], *zero_slab23[2];

  /* pfft_queue data */
  int aqmax, aqsize, alast_index;
  int *aqindx, *aqstatus;
  double *atmp;

  int bqmax, bqsize, blast_index;
  int *bqindx, *bqstatus;
  double *btmp;

  /* zplane data */
  double *zplane_tmp1, *zplane_tmp2;

public:
  /* lattice pointer */
  Lattice *lattice;

  /* r_grid data */
  bool has_r_grid = false;
  double *r_grid;

  /* constructor */
  PGrid(Parallel *, Lattice *, int, int, int, int, int, int);
  PGrid(Parallel *, Lattice *, Control2 &);

  /* destructor */
  ~PGrid() {
     delete[] Garray;
     delete[] Gpack[0];
     // delete [] Gpack[1];
     delete[] masker[0];
     // delete [] masker[1];
     delete[] packarray[0];
     // delete [] packarray[1];
     if (balanced)
       delete mybalance;
     delete[] zero_row3[0];
     delete[] zero_row3[1];
     delete[] zero_row2[0];
     delete[] zero_row2[1];
     delete[] zero_slab23[0];
     delete[] zero_slab23[1];
     delete[] zplane_tmp1;
     delete[] zplane_tmp2;
     if (has_r_grid)
       delete[] r_grid;
     delete[] atmp;
     delete[] aqindx;
     delete[] aqstatus;
     delete[] btmp;
     delete[] bqindx;
     delete[] bqstatus;
    
    std::cout << "delete pgrid, aqmax=" << aqmax << std::endl;
     // deallocate async buffer data
     for (auto q = 0; q<bqmax; ++q)
       parall->aend(3+q);

    std::cout << "delete Pgrid end" << std::endl;
    
  }

  double *Gxyz(const int i) { return &Garray[i * nfft3d]; }
  double *Gpackxyz(const int nb, const int i) {
    return &(Gpack[nb][i * (nida[nb] + nidb[nb])]);
  }
  double Gmax_ray() { return Gmax; }
  double Gmin_ray() { return Gmin; }
  double dGmin_ray() { return 0.01 * Gmin; }
  int n_ray() {
    int nray0 = (int)ceil(100 * (Gmax / Gmin) + 1.0);
    if (nray0 < 10)
      nray0 = 10;
    return nray0;
  }
  double *generate_G_ray() {
    int nray0 = (int)ceil(100 * (Gmax / Gmin) + 1.0);
    if (nray0 < 10)
      nray0 = 10;
    double *g_ray = new (std::nothrow) double[nray0]();
    double dGmin = 0.01 * Gmin;
    for (auto i = 0; i < nray0; ++i)
      g_ray[i] = dGmin * i;
    return g_ray;
  }

  int nzero(const int nb) { return nida[nb]; }
  int npack(const int nb) { return (nida[nb] + nidb[nb]); }
  int npack_all(const int nb) { return nwave_all[nb]; }
  int isbalanced() { return balanced; }

  double *c_pack_allocate(const int nb) {
    double *ptr;
    ptr = new (std::nothrow) double[2 * npack(nb)]();
    return ptr;
  }
  void c_pack_deallocate(double *ptr) { delete[] ptr; }

  double *t_pack_allocate(const int nb) {
    double *ptr;
    ptr = new (std::nothrow) double[npack(nb)]();
    return ptr;
  }
  void t_pack_deallocate(double *ptr) { delete[] ptr; }

  void c_unpack(const int, double *);
  void c_pack(const int, double *);
  void cc_pack_copy(const int, const double *, double *);
  double cc_pack_dot(const int, double *, double *);
  double cc_pack_idot(const int, double *, double *);
  void cc_pack_indot(const int, const int, double *, double *, double *);
  double tt_pack_dot(const int, double *, double *);
  double tt_pack_idot(const int, double *, double *);

  void cc_pack_inprjdot(const int, int, int, double *, double *, double *);

  void t_unpack(const int, double *);
  void t_pack(const int, double *);
  void tt_pack_copy(const int, double *, double *);
  void t_pack_nzero(const int, const int, double *);

  void tc_pack_copy(const int, double *, double *);

  void tcc_pack_Mul(const int, const double *, const double *, double *);
  void tcc_pack_aMul(const int, const double, const double *, const double *, double *);
  void tc_pack_Mul(const int, const double *, double *);
  void tcc_pack_iMul(const int, const double *, const double *, double *);
  void tc_pack_iMul(const int, const double *, double *);

  void tcc_pack_MulSum2(const int, const double *, const double *, double *);
  void cc_pack_Sum2(const int, const double *, double *);
  void cccc_pack_Sum(const int, const double *, const double *, const double *,
                     double *);
  void tcc_pack_aMulAdd(const int, const double, const double *, const double *,
                        double *);

  void c_pack_addzero(const int, const double, double *);
  void c_pack_noimagzero(const int, double *);

  void c_pack_zero(const int, double *);
  void c_pack_SMul(const int, const double, double *);
  void cc_pack_SMul(const int, const double, const double *, double *);
  void cc_pack_daxpy(const int, const double, const double *, double *);
  void cct_pack_iconjgMul(const int, const double *, const double *, double *);
  void cct_pack_iconjgMulb(const int, const double *, const double *, double *);

  void i_pack(const int, int *);
  void ii_pack_copy(const int, int *, int *);

  void cr_pfft3b_queuein(const int, double *);
  void cr_pfft3b_queueout(const int, double *);
  int cr_pfft3b_queuefilled();
  void cr_pfft3b(const int, double *);
  void pfftb_step(const int, const int, double *, double *, double *, const int);
  void pfftb_step7(const int, const int, double *, double *, double *, const int,const int);

  void c_unpack_start(const int, double *, double *, const int, const int);
  void c_unpack_mid(const int, double *, double *, const int, const int);
  void c_unpack_end(const int, double *, double *, const int);
  void pfftbz(const int, double *, double *, int);
  void pfftby(const int, double *, double *, int);
  void pfftbx(const int, double *, double *, int);

  void rc_pfft3f_queuein(const int, double *);
  void rc_pfft3f_queueout(const int, double *);
  int rc_pfft3f_queuefilled();
  void rc_pfft3f(const int, double *);
  void pfftf_step(const int, const int, double *, double *, double *, int);
  void c_pack_start(const int, double *, double *, const int, const int);
  void c_pack_end(const int, double *, const int);
  void pfftfx(const int, double *, double *, double *, int);
  void pfftfy(const int, double *, double *, int);
  void pfftfz(const int, double *, double *, int);
  void pfftf_final(const int, double *, double *, int);

  void pfftbz_start(const int, double *, double *, int, int);
  void pfftbz_compute(const int, double *, double *, int, int);
  void pfftbz_end(const int, double *, double *, int, int);

  void tcr_pack_iMul_unpack_fft(const int, const double *, const double *,
                                double *);

  void regenerate_r_grid();
  void initialize_r_grid() {
    if (!has_r_grid) {
      has_r_grid = true;
      r_grid = r_nalloc(3);
      this->regenerate_r_grid();
    }
  }
  void generate_r_sym_grid(double *);
  void generate_r_sym_mask(double *);

  void c_Laplacian(const int, double *);
  void cc_Laplacian(const int, const double *, double *);
  void rr_Laplacian(const int, const double *, double *);
  void rr_Helmholtz(const int, const double *, const double *, double *);
  void rrr_solve_Helmholtz(const int, const double *, const double *, double *);

  void rrrr_FD_gradient(const double *, double *, double *, double *);
  void rrrr_FD_laplacian(const double *, double *, double *, double *);

  void t_pack_gaussian(const int nb, const double rcut, double *gauss) {
    double w = rcut * rcut / 4.0;
    int npack0 = nida[nb] + nidb[nb];
    double *gx = Gpackxyz(nb, 0);
    double *gy = Gpackxyz(nb, 1);
    double *gz = Gpackxyz(nb, 2);
    for (auto k = 0; k < npack0; ++k) {
      auto gg = gx[k] * gx[k] + gy[k] * gy[k] + gz[k] * gz[k];
      gauss[k] = std::exp(-w * gg);
    }
  }
};

} // namespace pwdft

#endif
