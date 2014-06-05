function COEFFICIENTS=coeff_base2(subs,vals,lambdamax,anglemax,dens)

s=[ 7 7 7];

COEFFICIENT=zeros(s*2+1);

for a=-s(1):s(1)
    a
    for b=-s(2):s(2)
        b;
        for c=-s(3):s(3)
            c;
           
                        
            COEFFICIENTS(a+s(1)+1,b+s(2)+1,c+s(3)+1)=...
            	estimation_prod_scal2(lambdamax,a,b,anglemax,c,subs,vals,dens);
                        

        end
    end
end

                