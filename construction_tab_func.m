function t=construction_tab_func(bandes,pas)

t=[];

if mod((2*bandes(1))/pas(1),1) ~= 0 
    disp('attention le pas ne divise pas la bande')
end 

if length(bandes)>1
    length(bandes)
    n_restant=prod((2*bandes(2:end))./pas(2:end)+1);
    for i=-bandes(1):pas(1):bandes(1)
        
        t=[t;[i*ones(n_restant,1) , construction_tab_func(bandes(2:end),pas(2:end))]];
    end
else
    %size((-bandes(1):pas(1):bandes(1)).')
    t=(-bandes(1):pas(1):bandes(1)).';
end