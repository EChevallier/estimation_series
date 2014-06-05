function c=estimation_prod_scal(lambdamax,a,b,c,anglemax,d,e,f,subs,vals)
%subs_=-subs; %pour le produit scalaire on prend le conjugué
c=sum(conj(base_function(lambdamax,a,b,c,anglemax,d,e,f,subs)).*vals)/size(subs,1);