#pragma OPENCL EXTENSION cl_khr_fp64 : enable


__kernel void generate_projectors(const int ii, const int ng0, const int nprjall, 
                                  const --global int    *indxi,
                                  const --global int    *indxj,
                                  const --global int    *indxk,
                                  const --global double *phfacx,
                                  const --global double *phfacy,
                                  const --global double *phfacz,
                                  const __global int    *sdfunction, 
                                  const --global double *vnl,
                                  __global double *prj) {
   int shftx = 2*ii*nx;
   int shfty = 2*ii*ny;
   int shftz = 2*ii*ny;
   int ng    = 2*ng0;
   double ai,aj,ak,c,d;
   double bi,bj,bk;

   int i = get_global_id(0); //ng
   //int l = get_global_id(1); //nprj
   
   double ai = phfacx[shftx+2*indxi[i]]; double bi = phfacx[shftx+2*indxi[i]+1];
   double aj = phfacy[shfty+2*indxj[i]]; double bj = phfacy[shfty+2*indxj[i]+1];
   double ak = phfaxz[shftz+2*indxk[i]]; double bk = phfaxz[shftz+2*indxk[i]+1];
   double c  = aj*ak - bj*bk;
   double d  = aj*bk + ak*bj;
   double rexi = (ai*c - bi*d);
   double iexi = (ai*d + bi*c);

   for (int l=0; l<nprj; ++l)
   {
      if (sdfunction[l])
      {
         prj[2*i   + (l+nprjall)*ng] = rexi * vnl[i+l*ng0];
         prj[2*i+1 + (l+nprjall)*ng] = iexi * vnl[i+l*ng0];
      } else { 
         prj[2*i   + (l+nprjall)*ng] = -iexi * vnl[i+l*ng0];
         prj[2*i+1 + (l+nprjall)*ng] =  rexi * vnl[i+l*ng0];
      }
   }
}

