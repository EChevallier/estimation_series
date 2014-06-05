function c=estimation_prod_scal2(lambdamax,a,b,anglemax,c,subs,vals,dens)
%subs_=-subs; %pour le produit scalaire on prend le conjugué
c=sum(conj(base_function2(lambdamax,a,b,anglemax,c,subs,dens)).*vals)/size(subs,1);