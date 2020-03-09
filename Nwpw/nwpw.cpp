// A simple program that computes the square root of a number

#include <cmath>
//#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <stdio.h>
#include <string>

#include "NwpwConfig.h"
#include "mpi.h"

#include "util_date.hpp"

using namespace std;

//extern int cpsd(int argc, char *argv[]);
extern int cpsd(MPI_Comm comm_world0);

int main(int argc, char* argv[])
{
  int ierr=0;
  int taskid,np;
  int MASTER=0;
  string line,nwinput,nwfilename;
  string rtdbstr;

  // Initialize MPI
  ierr = MPI_Init(&argc,&argv);
  ierr += MPI_Comm_size(MPI_COMM_WORLD,&np);
  ierr += MPI_Comm_rank(MPI_COMM_WORLD,&taskid);


  if (taskid==MASTER)
  {
     std::cout << argv[0] << " (NWChemEx) - Version " << Nwpw_VERSION_MAJOR << "."
               << Nwpw_VERSION_MINOR << std::endl;
     if (argc>1) 
     {
        nwfilename = argv[1];
        nwinput = "";
        ifstream nwfile(argv[1]);
        if (nwfile.good())
        {
           while (getline(nwfile,line))
              nwinput += line + "\n";
        }
        nwfile.close();
     }
     else
     {
        nwfilename = "stdin";
        while (getline(std::cin,line))
              nwinput += line + "\n";
     }
     std::cout << std::endl;
     std::cout << "============================== echo of input deck ==============================\n";
     std::cout << nwinput;
     std::cout << "================================================================================\n\n";
     std::cout << "              NorthwestEx Computational Chemistry Package 1.0.0\n";
     std::cout << "           --------------------------------------------------------\n\n";
     std::cout << "                  Pacific Northwest National Laboratory\n";
     std::cout << "                           Richland, WA 99354\n\n";
     std::cout << "                         Copyright (c) 2020\n";
     std::cout << "                  Pacific Northwest National Laboratory\n";
     std::cout << "                       Battelle Memorial Institute\n\n";

     std::cout << "        NWChemEx is an open-source computational chemistry package\n";
     std::cout << "                   distributed under the terms of the\n";
     std::cout << "                 Educational Community License (ECL) 2.0\n";
     std::cout << "        A copy of the license is included with this distribution\n";
     std::cout << "                         in the LICENSE.TXT file\n\n";
     std::cout << "                             ACKNOWLEDGMENT\n";
     std::cout << "                             --------------\n\n";
     std::cout << "       This software and its documentation were developed at the\n";
     std::cout << "       Pacific Northwest National Laboratory, a multiprogram\n";
     std::cout << "       national laboratory, operated for the U.S. Department of Energy\n";
     std::cout << "       by Battelle under Contract Number DE-AC05-76RL01830. Support\n";
     std::cout << "       for this work was provided by the Department of Energy \n";
     std::cout << "       Office of Advanced Scientific Computing and the Office of Basic\n";
     std::cout << "       Energy Sciences.\n\n";
     std::cout << "       Job information\n";
     std::cout << "       ---------------\n";
     std::cout << "       program          = pwdft (NWChemEx)\n";
     std::cout << "       build configured = " << Nwpw_COMPILE_TIMESTAMP << std::endl;
     std::cout << "       version          = " << Nwpw_VERSION_MAJOR << "." << Nwpw_VERSION_MINOR << std::endl << std::endl;
     std::cout << "       date             = " << util_date() << std::endl;
     std::cout << "       nproc            = " << np << std::endl;
     std::cout << "       input            = " << nwfilename << std::endl;

/*

    compiled        = Tue_Jan_21_22:33:23_2020
    source          = /Users/bylaska/nwchem-releases/nwchem
    nwchem branch   = 7.0.0
    nwchem revision = N/A
    ga revision     = 5.7.0
    use scalapack   = F
    input           = eric0.nw
    prefix          = eric.
    data base       = ./eric.db
    status          = startup
    nproc           =        1
    time left       =     -1s
*/

     std::cout << std::endl << std::endl;

  }

  //int ijk = cpsd(argc,argv);
  ierr += cpsd(MPI_COMM_WORLD);


  // Finalize MPI
  ierr += MPI_Finalize();


  return ierr;
}
