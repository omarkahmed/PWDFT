#ifndef _INNERLOOP_H_
#define _INNERLOOP_H_



#include	"Control2.hpp"
#include        "Pneb.hpp"
#include        "PGrid.hpp"
#include        "Ion.hpp"
#include        "Ewald.hpp"
#include	"Kinetic.hpp"
#include	"Coulomb12.hpp"
#include        "exchange_correlation.hpp"
#include	"Strfac.hpp"
#include        "Pseudopotential.hpp"



namespace pwdft {

extern void inner_loop(Control2&, Pneb *, Ion *, 
                       Kinetic_Operator *, Coulomb12_Operator *, XC_Operator *,
                       Pseudopotential *, Strfac *, Ewald *,
                       double *, double *, double *, double *,
                       double *, double *, double *,
                       double *, double *, double *, double *);
}
#endif
