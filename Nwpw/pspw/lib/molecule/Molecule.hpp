#ifndef	_MOLECULE_HPP_
#define _MOLECULE_HPP_


#include        <iostream>
#include 	<iomanip> 
using namespace std;

#include	"Control2.hpp"
#include	"Pneb.hpp"
#include	"Ion.hpp"
#include	"Ewald.hpp"
#include        "Strfac.hpp"
#include	"Pseudopotential.hpp"
#include	"Electron.hpp"
#include	"psi.hpp"

#define	ionstream(A,B,C)	(A) << scientific << setw(19) << setprecision(10) << (B) << setw(0) <<  " (" << setw(15) << setprecision(5)  << (C) << setw(0) << " /ion)" << "\n"
#define	elcstream(A,B,C)	(A) << scientific << setw(19) << setprecision(10) << (B) << setw(0) <<  " (" << setw(15) << setprecision(5)  << (C) << setw(0) << " /electron)" << "\n"

#define	eig1stream(A,B)		scientific << setw(18) << setprecision(7) << (A) << setw(0) <<  " (" << fixed << setw(8) << setprecision(3)  << (B) << setw(0) << "eV)\n"
#define	eig2stream(A,B,C,D)	scientific << setw(18) << setprecision(7) << (A) << setw(0) <<  " (" << fixed << setw(8) << setprecision(3)  << (B) << setw(0) << "eV) " << scientific << setw(18) << setprecision(7) << (C) << setw(0) <<  " (" << fixed << setw(8) << setprecision(3)  << (D) << setw(0) << " eV)\n" 

class	Molecule {

   double omega,scal2,scal1,dv;
   double E[20],en[2];

   int ispin,ne[2],neall,n2ft3d,shift1,shift2;
   int nfft[3];
   int version=3;
   

public:
   Pneb   *mygrid;
   Ion    *myion;
   Strfac *mystrfac;
   Ewald  *myewald;
   Electron_Operators *myelectron;

   double *psi1, *rho1, *rho1_all, *dng1;
   double *psi2, *rho2, *rho2_all, *dng2;
   double *hml, *eig;

   /* Constructors */
   Molecule(char *,  Pneb *, Ion *, Strfac *, Ewald *, Electron_Operators *);

   /* destructor */
   ~Molecule() {
         delete [] psi1;
         delete [] rho1;
         delete [] rho1_all;
         delete [] dng1;

         delete [] psi2;
         delete [] rho2;
         delete [] rho2_all;
         delete [] dng2;

         delete [] hml;
         delete [] eig;
    }


   /* write psi molecule */
   void writepsi(char *output_filename) {
      psi_write(mygrid, &version, nfft, mygrid->lattice->unita_ptr(),
                &ispin, ne, psi1, output_filename);
    }


   /* molecule energy */
   double energy() {
      myelectron->run(psi1,rho1,dng1,rho1_all);
      E[0] = (myelectron->energy(psi1,rho1,dng1,rho1_all) +  myewald->energy());
      return E[0];
   }


   /* molecule energy and eigenvalues */
   double energy_eigenvalues() {
      myelectron->run(psi1,rho1,dng1,rho1_all);
      E[0] = (myelectron->energy(psi1,rho1,dng1,rho1_all) +  myewald->energy());

      /* generate eigenvalues */
      myelectron->gen_hml(psi1,hml);
      mygrid->m_diagonalize(hml,eig);

      return E[0];
   }


   /* molecule energy and eigenvalues and other energies and en */
   double gen_all_energies() {
      myelectron->run(psi1,rho1,dng1,rho1_all);
      myelectron->gen_energies_en(psi1,rho1,dng1,rho1_all,E,en);
      E[4] = myewald->energy();
      E[0] += E[4];

      /* generate eigenvalues */
      myelectron->gen_hml(psi1,hml);
      mygrid->m_diagonalize(hml,eig);

      return E[0];
   }

   /* various molecule energies */
   double eorbit()   { return myelectron->eorbit(psi1); }
   double ehartree() { return myelectron->ehartree(dng1); }
   double exc()      { return myelectron->exc(rho1_all); }
   double pxc()      { return myelectron->pxc(rho1); }
   double eke()      { return myelectron->eke(psi1); }
   double vl_ave()   { return myelectron->vl_ave(dng1); }
   double vnl_ave()  { return myelectron->vnl_ave(psi1); }
   double eion()     { return myewald->energy(); }

   /* molecule - generate current hamiltonian */
   void gen_hml() { myelectron->gen_hml(psi1,hml); }

   /* molecule - diagonalize the current hamiltonian */
   void diagonalize() { mygrid->m_diagonalize(hml,eig); }
  

   friend ostream& operator<<(ostream& os, const Molecule& mymolecule) {
      /* using old style c++ formatting */
      ios init(NULL);
      init.copyfmt(os);
      string eoln = "\n";
      os << "     ==============  energy results (Molecule object)  ==============" << eoln;
      os << eoln << eoln;

      os << fixed << " number of electrons: spin up= " << setw(11) << setprecision(5) << mymolecule.en[0] 
                                         << "  down= " << setw(11) << setprecision(5) << mymolecule.en[mymolecule.ispin] 
                                         << " (real space)";
      os << eoln;
      os << eoln;
      os << ionstream(" total     energy    : ",mymolecule.E[0],mymolecule.E[0]/mymolecule.myion->nion);
      os << elcstream(" total orbital energy: ",mymolecule.E[1],mymolecule.E[1]/mymolecule.neall);
      os << elcstream(" hartree energy      : ",mymolecule.E[2],mymolecule.E[2]/mymolecule.neall);
      os << elcstream(" exc-corr energy     : ",mymolecule.E[3],mymolecule.E[3]/mymolecule.neall);
      os << ionstream(" ion-ion energy      : ",mymolecule.E[4],mymolecule.E[4]/mymolecule.myion->nion);
      os << eoln;
      os << elcstream(" kinetic (planewave) : ",mymolecule.E[5],mymolecule.E[5]/mymolecule.neall);
      os << elcstream(" V_local (planewave) : ",mymolecule.E[6],mymolecule.E[6]/mymolecule.neall);
      os << elcstream(" V_nl    (planewave) : ",mymolecule.E[7],mymolecule.E[7]/mymolecule.neall);
      os << elcstream(" V_Coul  (planewave) : ",mymolecule.E[8],mymolecule.E[8]/mymolecule.neall);
      os << elcstream(" V_xc    (planewave) : ",mymolecule.E[9],mymolecule.E[9]/mymolecule.neall);
      os << " Viral Coefficient   : "
         << setw(19) << setprecision(10) << (mymolecule.E[9] + mymolecule.E[8] + mymolecule.E[7] + mymolecule.E[6])/mymolecule.E[5];
      os << eoln;
      os << eoln;
      os << " orbital energy:" << eoln;
      int nn = mymolecule.ne[0] - mymolecule.ne[1];
      double ev = 27.2116;
      for (int i=0; i<nn; ++i)
         os << eig1stream(mymolecule.eig[i],mymolecule.eig[i]*ev);
      for (int i=0; i<mymolecule.ne[1]; ++i)
         os << eig2stream(mymolecule.eig[i+nn], 
                          mymolecule.eig[i+nn]*ev,
                          mymolecule.eig[i+(mymolecule.ispin-1)*mymolecule.ne[0]], 
                          mymolecule.eig[i+(mymolecule.ispin-1)*mymolecule.ne[0]]*ev);
      os << eoln;

      os.copyfmt(init);

      return os;
   }
   

};

#endif
