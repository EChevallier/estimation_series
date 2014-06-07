#define PI 3.14


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
        float coef1 = 2.*PI*func[j*6+0]/lambdamax[0];
        float coef2 = 2.*PI*func[j*6+1]/lambdamax[1];
        float coef3 = 2.*PI*func[j*6+2]/lambdamax[2];
        float coef4 = 2.*PI*func[j*6+3]/anglemax[0];
        float coef5 = 2.*PI*func[j*6+4]/anglemax[1];
        float coef6 = 2.*PI*func[j*6+5]/anglemax[2];

coefs[j]=0;
        for(int i=0;i<pointNumber;i++)
        {
            float a = coef1*points[i*6+0]+coef2*points[i*6+1]+
                      coef3*points[i*6+2]+coef4*points[i*6+3]+
                      coef5*points[i*6+4]+coef6*points[i*6+5];

                
            coefs[j] += cos(a);
  //coefs[j] += i;
        }
    }
}