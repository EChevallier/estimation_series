function evaluation=base_function2(lambdamax,a,b,anglemax,c,x,dens)
%,c
%,anglemax,d,e,f

%% partie fourier

%v1=exp(2*1i*pi*a*x(:,1)/(lambdamax(1))).*exp(2*1i*pi*b*x(:,2)/(lambdamax(2))).*
%    exp(2*1i*pi*c*x(:,3)/(lambdamax(3)));

v1=exp(2*1i*pi*a*x(:,1)/(1.5*lambdamax(1))).*exp(2*1i*pi*b*x(:,2)/(1.5*lambdamax(2)));
    

% d_eucl=abs(x(:,1)-x(:,2));
% d_fish=1./sqrt( x(:,1).^3.*x(:,2).^3 );
% d_wass=sqrt(x(:,1).*x(:,2).*(x(:,1)+x(:,2)));
% 
% 
% d_val=d_eucl.*d_wass;
%v1=v1./sqrt(d_val);
%disp('coucou')

%% partie harmonique

%v2=1;%spherical_harmonic(d,e,);
%v2=exp(2*1i*pi*d*x(:,4)/(anglemax(1))).*exp(2*1i*pi*e*x(:,5)/(anglemax(2))).*...
%   exp(2*1i*pi*f*x(:,6)/(2*anglemax(3))); %le 3eme angle va de -pi a pi


v2=exp(2*1i*pi*c*x(:,3)/(1.5*anglemax(1)));

%d_angle=sin(x(:,4)).^2.*sin(x(:,5));
%v2=v2./sqrt(d_angle);

%% produit

evaluation=v1.*v2./sqrt(dens);

%% cas param sym
% xx=x;
% xx(:,2)=x(:,2)*sqrt(2);
% for i=1:size(x,1)
%     y(i,:)=param_SOD2(xx(i,:));
% end
% 
% d_fish=1./sqrt( y(:,1).^3.*y(:,2).^3 );
% d_wass=sqrt(y(:,1).*y(:,2).*(y(:,1)+y(:,2)));
% 
% %d_val=abs(x(:,1)-x(:,2));%./sqrt( x(:,1).^3.*x(:,2).^3 );
% d=d_wass;
% 
% 
% 
% evaluation=v1.*v2./sqrt(d);

