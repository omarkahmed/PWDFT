#ifndef _UTIL_H_
#define _UTIL_H_

#include        "Parallel.hpp"

extern void c_aindexcopy(const int, const int *, double *, double *);
extern void c_bindexcopy(const int, const int *, double *, double *);
extern void c_bindexcopy_conjg(const int, const int *, double *, double *);

extern void t_aindexcopy(const int, const int *, double *, double *);
extern void t_bindexcopy(const int, const int *, double *, double *);

extern void i_aindexcopy(const int, const int *, int *, int *);

extern void eigsrt(double *, double *, int);
extern double util_random(const int);

extern void util_getfilling(int, int *, int *, double *);

extern bool util_filefind(Parallel *, char *);

#endif
