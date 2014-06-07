%main series ortho
clear all
close all

%% generation du tableau de points

points=[1 2 3 4 5 6;7 8 9 10 11 12];

fid=fopen('points.txt','w');
fprintf(fid,'%d %d %d %d %d %d \n',points.');
fclose(fid)

edit points.txt


%% generation du tableau d'indice des fonctions

bandes=[10 10 10 10 10 10];
pas=[2 1 1 1 1 1 ];

dimension=length(bandes);

%t=construction_tab_func(bandes,pas);

fid=fopen('liste_fonctions.txt','w');
fprintf(fid,'%d %d\n',[bandes;pas]);
fclose(fid)

edit liste_fonctions.txt

