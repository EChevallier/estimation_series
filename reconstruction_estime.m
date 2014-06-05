function val=reconstruction_estime(COEFFICIENTS,lambdamax,anglemax,x)

S=[size(COEFFICIENTS) ones(1,6-length(size(COEFFICIENTS)))];
s=(S+1)/2;
%f=@(x) 0;
val=0;
for a=1:S(1)
    for b=1:S(2)
        for c=1:S(3)
            for d=1:S(4)
                for e=1:S(5)
                    for f=1:S(6)
val=val+COEFFICIENTS(a,b,c,d,e,f)*...
     base_function(lambdamax,a-s(1),b-s(2),c-s(3),anglemax,d-s(4),e-s(5),f-s(6),x);


                    end
                end
            end            
        end
    end
end
%f([1,1,1]) 