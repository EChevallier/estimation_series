function COEFFICIENTS=coeff_base(subs,vals,lambdamax,anglemax)

s=[1000 0 0 0 0 0 ];

%COEFFICIENT=zeros(s*2+1);
COEFFICIENTS=zeros(s(1)*2+1,s(2)*2+1,s(3)*2+1,s(4)*2+1,s(5)*2+1,s(6)*2+1);

for a=-s(1):s(1)
    a
    for b=-s(2):s(2)
        %b;
        for c=-s(3):s(3)
            %c;
            for d=-s(4):s(4)
                %d;
                for e=-s(5):s(5)
                    for f=-s(6):s(6)
                        
                        COEFFICIENTS(a+s(1)+1,b+s(2)+1,c+s(3)+1,...
                            d+s(4)+1,e+s(5)+1,f+s(6)+1)=...
                            estimation_prod_scal(lambdamax,a,b,c,anglemax,d,e,f,subs,vals);
                        
                    end
                end
            end
        end
    end
end

                