function evaluation=base_function(lambdamax,a,b,c,anglemax,d,e,f,x)
%,c
%,anglemax,d,e,f

%% partie fourier

v1=exp(2*1i*pi*a*x(:,1)/(lambdamax(1))).*exp(2*1i*pi*b*x(:,2)/(lambdamax(2))).*...
    exp(2*1i*pi*c*x(:,3)/(lambdamax(3)));




%% partie harmonique

v2=1;%spherical_harmonic(d,e,);
v2=exp(2*1i*pi*d*x(:,4)/(anglemax(1))).*exp(2*1i*pi*e*x(:,5)/(anglemax(2))).*...
   exp(2*1i*pi*f*x(:,6)/(2*anglemax(3))); %le 3eme angle va de -pi a pi

%d_angle=sin(x(:,4)).^2.*sin(x(:,5));
%v2=v2./sqrt(d_angle);
%% produit

evaluation=v1.*v2;