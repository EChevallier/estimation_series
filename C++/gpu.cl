#define PI 3.14159265359


__kernel void compute_Coefs (__global const float* points,
                     __global const float* func,
                     __global float* coefs,
                     __global const float* lambdamax,
                     __global const float* anglemax,
              const int pointNumber,
              const int coefNumber)
{
    const int j = get_global_id(0);

    if(j<coefNumber)
    {
        float indice1 = 2.*PI*func[j*6+0]/lambdamax[0];
        float indice2 = 2.*PI*func[j*6+1]/lambdamax[1];
        float indice3 = 2.*PI*func[j*6+2]/lambdamax[2];
        float indice4 = 2.*PI*func[j*6+3]/anglemax[0];
        float indice5 = 2.*PI*func[j*6+4]/anglemax[1];
        float indice6 = 2.*PI*func[j*6+5]/anglemax[2];

coefs[2*j]=0;
coefs[2*j+1]=0;
        for(int i=0;i<pointNumber;i++)
        {
            float a = indice1*points[i*6+0]+indice2*points[i*6+1]+
                      indice3*points[i*6+2]+indice4*points[i*6+3]+
                      indice5*points[i*6+4]+indice6*points[i*6+5];

                
            coefs[2*j] += cos(a);
			coefs[2*j+1] += sin(a);
  //coefs[j] += i;
        }
    }
}