#pragma once

#include "gdevices.hpp"

// using namespace pwdft;
namespace pwdft {


class gdevice2 {

Gdevices *mygdevice2;

public:

   /* constructor */
   gdevice2();

   void TN4_dgemm(int, int, double, double *, double *, double, double *, double *, double *, double *);
   void TN3_dgemm(int, int, double, double *, double *, double, double *, double *, double *);
   void TN1_dgemm(int, int, double, double *, double *, double, double *);
   void TN_dgemm(int, int, int, double, double *, double *, double, double *);
   void T_free();
  
   void NN_dgemm(int, int, double, double *, double *, double, double *);
   void NT_dgemm(int, int, int, double, double *, double *, double, double *);

   void NN_dgemm1(int, int, int, double, double *, int, double *, int, double, double *,int);
   void TN_dgemm2(int, int, int, double, double *, int, double *, int, double, double *,int);
   void TN_dgemm2c(int, int, int, int, double *, double *, double *);
   void NT_dgemm3(int, int, int, double, double *, int, double *, int, double, double *,int);
  
   void MM6_dgemm(int, double *, double *, double *, double *, double *, double *);
  
   void NN_eigensolver(int, int *, double *, double *);
  
   void psi_alloc(int, int, int);
   void psi_dealloc();
  
   void psi_copy_host2gpu(int, int, double *);
   void hpsi_copy_host2gpu(int, int, double *);
  
   void psi_copy_gpu2host(int, int, double *);
   void hpsi_copy_gpu2host(int, int, double *);
  
   int  batch_fft_init(int, int, int, int, int, int);
   void batch_fft_end(int);

   void batch_fft_pipeline_mem_init(const int,const int);

   void batch_rfftx_tmpx(const int, bool, int, int, int, double *, double *);
   void batch_cfftx_tmpx(const int, bool, int, int, int, double *, double *);
   void batch_cffty_tmpy(const int, bool, int, int, int, double *, double *);
   void batch_cfftz_tmpz(const int, bool, int, int, int, double *, double *);

   void batch_rfftx_stages_tmpx(const int,const int, bool, int, int, int, double *, double *,int);
   void batch_cfftx_stages_tmpx(const int,const int, bool, int, int, int, double *, double *,int);
   void batch_cffty_stages_tmpy(const int,const int, bool, int, int, int, double *, double *,int);
   void batch_cfftz_stages_tmpz(const int,const int, bool, int, int, int, double *, double *,int);
  
   void batch_cffty_tmpy_zero(const int, bool, int, int, int, double *, double *, bool *);
   void batch_cfftz_tmpz_zero(const int, bool, int, int, int, double *, double *, bool *);

   void batch_cffty_stages_tmpy_zero(const int, const int, bool, int, int, int, double *, double *, bool *,int);
   void batch_cfftz_stages_tmpz_zero(const int, const int, bool, int, int, int, double *, double *, bool *,int);

   bool has_gpu() { return mygdevice2->hasgpu; }
};

} // namespace pwdft
