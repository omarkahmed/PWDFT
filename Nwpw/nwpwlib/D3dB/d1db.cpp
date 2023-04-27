/* d1db.C
   Author - Eric Bylaska

        this class is used for defining 3d parallel maps
*/

/*
#include        <cmath>
#include        <cstdio>
#include        <cstdlib>
#include        <iostream>
#include        <stdio.h>

*/

#include "Mapping1.hpp"
#include "Parallel.hpp"
#include "d1db.hpp"

namespace pwdft {

/********************************
 *                              *
 *         Constructors         *
 *                              *
 ********************************/

d1db::d1db(Parallel *inparall, const int inmaptype, const int inispin,
           int *inne)
    : Mapping1(inmaptype, inparall->np_j(), inparall->taskid_j(), inispin,
               inne) {
  parall = inparall;
}

} // namespace pwdft
