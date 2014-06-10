#include <iostream>
#include <Windows.h>
#include <CL/cl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <fstream>
//#include <math.h>

#define PI 3.14159265359

float* multiplicationComplexe(float* z1, float* z2){
	float* res = new float[2];
	res[0] = z1[0] * z2[0] - z1[1] * z2[1];
	res[1] = z1[0] * z2[1] + z1[1] * z2[0];

	return res;
}

float densiteFisher(float* pointCourant){
	float d;
	d = 1 / (pow(pointCourant[1]*pointCourant[2]*pointCourant[3], 2));
	return d;
}

int nombrePoints(float* lambdamax, float* anglemax, float* pas){
	int nb_points = (floor(lambdamax[0] / pas[0]) + 1) * (floor(lambdamax[1] / pas[1]) + 1) * (floor(lambdamax[2] / pas[2]) + 1) * (floor(anglemax[0] / pas[3]) + 1) * (floor(anglemax[1] / pas[4]) + 1) * (floor(anglemax[2] / pas[5]) + 1);
	return nb_points;
}

float* generationGrille(float* lambdamax, float* anglemax, float* pas){

	//float temp = lambdamax[1] / pas[1] + lambdamax[2] / pas[2] + lambdamax[3] / pas[3] + anglemax[1] / pas[4] + anglemax[2] / pas[5] + anglemax[3] / pas[6];

	int nb_points = nombrePoints(lambdamax, anglemax, pas);

	float* grille = new float[nb_points * 6];

	int c = 0;
	float l1 = 0;
	float l2 = 0;
	float l3 = 0;
	float a1 = 0;
	float a2 = 0;
	float a3 = 0;

	for (l1 = 0; l1 <= lambdamax[0]; l1 = l1 + pas[0]){
	
		for (l2 = 0; l2 <= lambdamax[1];l2 = l2 + pas[1]){
		
			for (l3 = 0; l3 <= lambdamax[2]; l3 = l3 + pas[2]){
				
				for (a1 = 0; a1 <= anglemax[0]; a1 = a1 + pas[3]){
				
					for (a2 = 0; a2 <= anglemax[1]; a2= a2 + pas[4]){
						
						for (a3 = 0; a3 <= anglemax[2]; a3 = a3 + pas[5]){
							
							grille[6*c] = l1;
							grille[6*c + 1] = l2;
							grille[6*c + 2] = l3;
							grille[6*c + 3] = a1;
							grille[6*c + 4] = a2;
							//std::cout << " " << grille[c + 4];
							grille[6*c + 5] = a3;
							//std::cout << " " << grille[c+5];
							c = c + 1;
							std::cout << " " << c;
						}
					}
				}
			}
		}
		
	}
	std::cout << "coherence " << c - nb_points;
	return grille;
}

float integraleFisher(float* fonction, float* points,int nb_points,float dEucl)
{
	// la fonction doit etre reel
	float I = 0;
	float dmu;
	float* pointCourant = new float[6];
	int i = 0;
	for (i = 0; i < nb_points; i++){
		pointCourant[0] = points[6 * i];
		pointCourant[1] = points[6 * i + 1];
		pointCourant[2] = points[6 * i + 2];
		pointCourant[3] = points[6 * i + 3];
		pointCourant[4] = points[6 * i + 4];
		pointCourant[5] = points[6 * i + 5];
		dmu = densiteFisher(pointCourant)*dEucl;
		I = I + fonction[i]*dmu;

	}
	return I;
}



float* prodScal(float* fonction1, float* fonction2, float* points, int nb_points, float* pas){
	float* p = new float[2];
	p[0] = 0;
	p[1] = 1;
	float dEucl = pas[1] * pas[2] * pas[3] * pas[4] * pas[5] * pas[6];
	int i;
	float* z1=new float[2];
	float* z2 = new float[2];
	float* res = new float[2];
	for (i = 0; i < nb_points; i++){
		z1[0] = fonction1[2 * i];
		z1[1] = fonction1[2 * i + 1] ;
		z2[0] = fonction2[2 * i];
		z2[1] = -fonction1[2 * i + 1];
		res=multiplicationComplexe(z1, z2);
		p[0] = p[0] + res[0] * dEucl;
		p[1] = p[1]  + res[1] * dEucl;
	}

	return p;
}

float* pordScalFisher(float* fonction1, float* fonction2, float* points, int nb_points,float* pas){
	float* p = new float[2];
	float dEucl = pas[1] * pas[2] * pas[3] * pas[4] * pas[5] * pas[6];
	float* pointCourant = new float[6];
	float dmu;
	float* z1 = new float[2];
	float* z2 = new float[2];
	float* res = new float[2];
	int  i;
	for (i = 0; i < nb_points; i++){
		pointCourant[0] = points[6 * i];
		pointCourant[1] = points[6 * i + 1];
		pointCourant[2] = points[6 * i + 2];
		pointCourant[3] = points[6 * i + 3];
		pointCourant[4] = points[6 * i + 4];
		pointCourant[5] = points[6 * i + 5];
		dmu = densiteFisher(pointCourant)*dEucl;
		z1[0] = fonction1[2 * i];
		z1[1] = fonction1[2 * i + 1];
		z2[0] = fonction2[2 * i];
		z2[1] = -fonction1[2 * i + 1];
		res = multiplicationComplexe(z1, z2);
		p[0] = p[0] + res[0] * dmu;
		p[1] = p[1] + res[1] * dmu;
	}

	return p;
}




float* generationIndicesRect(float* bandes){

	int nb_fon = 2 * bandes[0] + 1 + 2 * bandes[1] + 1 + 2 * bandes[2] + 1 + 2 * bandes[3] + 1 + 2 * bandes[4] + 1 + 2 * bandes[5] + 1;
	float* indiceFonction = new float[nb_fon * 6];
	int c = 0;
	float l1 = 0;
	float l2 = 0;
	float l3 = 0;
	float a1 = 0;
	float a2 = 0;
	float a3 = 0;
	for (l1 = -bandes[0]; l1 < bandes[0]+1; l1 ++){
		for (l2 = -bandes[1]; l1 < bandes[1] + 1; l2++){
			for (l3 = -bandes[2]; l1 < bandes[2] + 1; l3++){
				for (a1 = -bandes[3]; l1 < bandes[3] + 1; a1++){
					for (a2 = -bandes[4]; l1 < bandes[4] + 1; a2++){
						for (a3 = -bandes[5]; l1 < bandes[5] + 1; a3++){
							indiceFonction[c] = l1;
							indiceFonction[c + 1] = l2;
							indiceFonction[c + 2] = l3;
							indiceFonction[c + 3] = a1;
							indiceFonction[c + 4] = a2;
							indiceFonction[c + 5] = a3;
							c = c + 1;
						}
					}
				}
			}
		}
	}
	return indiceFonction;
}

float* generationIndicesT(float T,int* nb_fonc){

	int nb_fon_max = pow(T, 6);
	float* indiceFonction = new float[nb_fon_max * 6];
	int c = 0;
	float l1 = 0;
	float l2 = 0;
	float l3 = 0;
	float a1 = 0;
	float a2 = 0;
	float a3 = 0;
	for (l1 = -T; l1 < T + 1; l1++){
		for (l2 = -T; l1 < T + 1; l2++){
			for (l3 = -T; l1 < T + 1; l3++){
				for (a1 = -T; l1 < T + 1; a1++){
					for (a2 = -T; l1 < T + 1; a2++){
						for (a3 = -T; l1 < T + 1; a3++){
							if (l1*l1 + l2*l2 + l3*l3 + a1*a1 + a2*a2 + a3*a3 < T*T)
							indiceFonction[c] = l1;
							indiceFonction[c + 1] = l2;
							indiceFonction[c + 2] = l3;
							indiceFonction[c + 3] = a1;
							indiceFonction[c + 4] = a2;
							indiceFonction[c + 5] = a3;
							c = c + 1;
						}
					}
				}
			}
		}
	}
	nb_fonc[1] = c;
	return indiceFonction;
}

float* transformeeExp(float* fonction, float* grille, int nb_points,float* pas, float* indiceFonction,int nb_fonc,float* lambdamax,float* anglemax){

	float* coefs = new float[nb_fonc * 2];
	float* fonctionCourante = new float[nb_points * 2];
	int i;
	for (i = 0; i < nb_fonc; i++){
		float indice1 = 2.*PI*indiceFonction[i * 6 + 0] / lambdamax[0];
		float indice2 = 2.*PI*indiceFonction[i * 6 + 1] / lambdamax[1];
		float indice3 = 2.*PI*indiceFonction[i * 6 + 2] / lambdamax[2];
		float indice4 = 2.*PI*indiceFonction[i * 6 + 3] / anglemax[0];
		float indice5 = 2.*PI*indiceFonction[i * 6 + 4] / anglemax[1];
		float indice6 = 2.*PI*indiceFonction[i * 6 + 5] / anglemax[2];

		int j;
		for (j = 0; j < nb_points; j++){

			float a = indice1*grille[j * 6 + 0] + indice2*grille[j * 6 + 1] +
				indice3*grille[j * 6 + 2] + indice4*grille[j * 6 + 3] +
				indice5*grille[j * 6 + 4] + indice6*grille[j * 6 + 5];
			fonctionCourante[2 * j] = cos(a);
			fonctionCourante[2 * j+1] = sin(a);
		}

		float* p = prodScal(fonction, fonctionCourante, grille, nb_points, pas);
		coefs[2 * i] = p[0];
		coefs[2 * i + 1] = p[1];

	}

	return coefs;
}

float* transformeeExpInv(float* coefs, int nb_coefs, float* indiceFonction, float* pointCourant, float* lambdamax, float* anglemax){

	float val[2];
	val[0] = 0;
	val[1] = 0;
	float* fonctionCourante = new float[2];
	int i;
	for (i = 0; i < nb_coefs; i++){
		float indice1 = 2.*PI*indiceFonction[i * 6 + 0] / lambdamax[0];
		float indice2 = 2.*PI*indiceFonction[i * 6 + 1] / lambdamax[1];
		float indice3 = 2.*PI*indiceFonction[i * 6 + 2] / lambdamax[2];
		float indice4 = 2.*PI*indiceFonction[i * 6 + 3] / anglemax[0];
		float indice5 = 2.*PI*indiceFonction[i * 6 + 4] / anglemax[1];
		float indice6 = 2.*PI*indiceFonction[i * 6 + 5] / anglemax[2];


		float a = indice1*pointCourant[0] + indice2*pointCourant[1] +
			indice3*pointCourant[2] + indice4*pointCourant[3] +
			indice5*pointCourant[4] + indice6*pointCourant[5];
		fonctionCourante[0] = cos(a);
		fonctionCourante[1] = sin(a);
		
		val[1] = val[1] + coefs[2 * i] * cos(a);
		val[2] = val[2] + coefs[2 * i+1] * sin(a);
	}

	return val;
}