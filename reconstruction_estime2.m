function val=reconstruction_estime2(COEFFICIENTS,lambdamax,anglemax,x)

S=[size(COEFFICIENTS) ones(1,3-length(size(COEFFICIENTS)))];
s=(S+1)/2;
%f=@(x) 0;
val=0;
for a=1:S(1)
    for b=1:S(2)
        for c=1:S(3)

val=val+COEFFICIENTS(a,b,c)*...
     base_function2(lambdamax,a-s(1),b-s(2),anglemax,c-s(3),x);


           
        end
    end
end
%f([1,1,1]) 