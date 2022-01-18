#ifndef	_PSP1D_PAWPPV1_H_
#define _PSP1D_PAWPPV1_H_

#include        "Parallel.hpp"
#include        "PGrid.hpp"

namespace pwdft {
using namespace pwdft;


class Psp1d_pawppv1 {

public:
   int version,psp_type,n1dgrid,nmax,lmax,nbasis,nprj,icut,locp;
   double rlocal,amass,zv,zion,r1,rmax,sigma,log_amesh,amesh;

   double rc[10],core_kin_energy;
   char atom[2];
   char comment[80];


   int *nae;
   int *nps;
   int *lps;

   double *rgrid;
   double *eig;
   double *phi_ae;
   double *dphi_ae;
   double *phi_ps;
   double *dphi_ps;
   double *core_ae;
   double *core_ps;
   double *core_ae_prime;
   double *core_ps_prime;
   double *v_ps;
   double *prj_ps;
   double *prj_ps0;
  

   /* comp_charge_matrix(nbasis,nbasis,0:2*lmax) */
   double *comp_charge_matrix;

   /* comp_pot_matrix(nbasis,nbasis,0:2*lmax) */
   double *comp_pot_matrix;

   /* hartree_matrix(nbasis,nbasis,nbasis,nbasis,0:2*lmax) */
   double *hartree_matrix;

   /* 1-electron psp operators - Normalization constants */
   double *Gijl;

   int    *n_prj, *l_prj, *m_prj, *b_prj;


   /* Constructors */
   Psp1d_pawppv1(Parallel *, const char *);

   /* destructor */
   ~Psp1d_pawppv1() {
      delete [] rgrid;
      delete [] nae;
      delete [] nps;
      delete [] lps;

      delete [] rgrid;
      delete [] eig;
      delete [] phi_ae;
      delete [] dphi_ae;
      delete [] phi_ps;
      delete [] dphi_ps;
      delete [] core_ae;
      delete [] core_ps;
      delete [] core_ae_prime;
      delete [] core_ps_prime;
      delete [] v_ps;
      delete [] prj_ps;
      delete [] prj_ps0;

      delete [] comp_charge_matrix;
      delete [] comp_pot_matrix;
      delete [] hartree_matrix;
      delete [] Gijl;
      if (nprj>0)
      {
         delete [] n_prj;
         delete [] l_prj;
         delete [] m_prj;
         delete [] b_prj;
      }
    }

   /* G integration routines */
   void vpp_generate_ray(Parallel *, int, double *, double *, double *, double *);
   void vpp_generate_spline(PGrid *, int, double *, double *, double *, double *, double *, double *, double *);


};

}

#endif
