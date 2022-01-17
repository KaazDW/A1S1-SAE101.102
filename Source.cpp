#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include "config_sdl.h"
using namespace std;


struct Statistique {
	int Jour;
	int TailleMax1;
	int TailleMax2;
	int TailleMoy;
	int TailleMin;
};


struct Bambou
{
	int num;
	int taille;
	int croissance;
};


struct Robot {
	bool position[12] = { 0 };
	int batterie;
};


void InitTab(Bambou tab[], int taille) {

	for (int i = 0; i < taille; i++) {
		tab[i].num = i;
		tab[i].taille = rand() % 15;
		tab[i].croissance = 1 + rand() % 20;
	}
}


void InitRobot(Robot& panda) {
	panda.batterie = 7;
	panda.position[11] = 1;
}


// Cette fonction me sert juste à tester les autres
void afficheTab(Bambou tab[], int taille) {
	for (int i = 0; i < taille; i++) {
		cout << tab[i].taille << " ";
	}

	cout << endl;

	for (int i = 0; i < taille; i++) {
		cout << tab[i].croissance << " ";
	}
	cout << endl;
}


void TailleMax(Bambou tab[], int taille, int &premier_plus_grand_ind, int &deuxieme_plus_grand_ind) {

	int max = tab[0].taille;
	int imax = 0;

	for (int i = 0; i < taille; i++) {
		if (max < tab[i].taille) {
			max = tab[i].taille;
			imax = i;
		}
	}

	premier_plus_grand_ind = imax;
	
	
	Bambou tmp = tab[imax];
	tab[imax] = tab[0];
	tab[0] = tmp;
	
	max = tab[1].taille;
	
	for (int i = 1; i < taille; i++) {
		if (max < tab[i].taille) {
			max = tab[i].taille;
			imax = i;
		}
	}
	
	if (premier_plus_grand_ind == imax)
		deuxieme_plus_grand_ind = 0;
	else
		deuxieme_plus_grand_ind = imax;

	tmp = tab[premier_plus_grand_ind];
	tab[premier_plus_grand_ind] = tab[0];
	tab[0] = tmp;
}


float TailleMoyenne(Bambou tab[], int Taille) {
	float somme = 0.0, cpt = 0.0;
	for (int i = 0; i < Taille; i++) {
		somme = somme + tab[i].taille;
		cpt++;
	}
	float moyenne = somme / cpt;
	return moyenne;
}


void croissance(Bambou tab[], int taille) {
	for (int i = 0; i < taille; i++) {
		tab[i].taille += tab[i].croissance;
	}
}


void deplacement(Robot &panda1, Robot &panda2, int indice_premier, int indice_deuxieme) {
	for (int i = 0; i < 12; i++) {
		panda1.position[i] = false;
		panda2.position[i] = false;
	}
	panda1.position[indice_premier] = true;
	panda2.position[indice_deuxieme] = true;
}


void batterie_et_decoupe(Bambou tab[], Robot &panda1, Robot &panda2, int indice_premier, int indice_deuxieme) {
	if (panda1.batterie > 0) {
		tab[indice_premier].taille = 0;
		panda1.batterie -= 1;
	}
	else {
		panda1.batterie = 7;
	}

	if (panda2.batterie > 0) {
		tab[indice_deuxieme].taille = 0;
		panda2.batterie -= 1;
	}
	else {
		panda2.batterie = 7;
	}
}


void ReduceMax(Bambou tab[], int taille, Robot &panda1, Robot &panda2) {
	int indice_premier = 0, indice_deuxieme = 0;
	TailleMax(tab, taille, indice_premier, indice_deuxieme);

	deplacement(panda1, panda2, indice_premier, indice_deuxieme);
	
	batterie_et_decoupe(tab, panda1, panda2, indice_premier, indice_deuxieme);
}


void ReduceFast(Bambou tab[], int taille, Robot &panda1, Robot &panda2) {

	int somme_croissance_bambou = 0;

	for (int i = 0; i < taille; i++) {
		somme_croissance_bambou += tab[i].croissance;
	}

	float x = 1 + sqrt(5);
	float taille_minimale = x * somme_croissance_bambou;

	int indice_croissance1, indice_croissance2;
	int max1, max2;

	max1 = tab[0].croissance;
	indice_croissance1 = 0;

	for (int i = 0; i < taille; i++) {
		if (tab[i].taille > taille_minimale && max1 <= tab[i].croissance) {
			max1 = tab[i].croissance;
			indice_croissance1 = i;
		}
	}
	if (indice_croissance1 != 0) {
		Bambou tmp = tab[indice_croissance1];
		tab[indice_croissance1] = tab[0];
		tab[0] = tmp;
	}

	max2 = tab[1].croissance;
	indice_croissance2 = 1;

	for (int i = 1; i < taille; i++) {
		if (tab[i].taille > taille_minimale && max2 <= tab[i].croissance) {
			max2 = tab[i].croissance;
			indice_croissance2 = i;
		}
	}

	if (indice_croissance1 != 0) {
		Bambou tmp = tab[indice_croissance1];
		tab[indice_croissance1] = tab[0];
		tab[0] = tmp;
	}
	
	if (tab[indice_croissance1].taille > taille_minimale && tab[indice_croissance2].taille > taille_minimale) {
		deplacement(panda1, panda2, indice_croissance1, indice_croissance2);
		batterie_et_decoupe(tab, panda1, panda2, indice_croissance1, indice_croissance2);
	}
}


int main(int argc, char* argv[]) {
	
	// Déclaration tableau et constante
	const int TAILLE = 12;
	Bambou jardin[TAILLE];

	// Initialisation du tableau jardin
	InitTab(jardin, TAILLE);

	// Initialisation des indices qui nous seront utiles après appels de fonctions.
	int indice_premier_plus_grand = 0, indice_deuxieme_plus_grand = 0;

	Robot panda1, panda2;
	InitRobot(panda1);
	InitRobot(panda2);


	bool simulation = true;
	char continuer = ' ';

	afficheTab(jardin, TAILLE);

	while (simulation) {
		cout << "Entrez 'r' pour relancer un jour, 'q' pour quitter." << endl;
		cin >> continuer;

		if (continuer == 'q') {
			cout << "Fin." << endl;
			simulation = false;
		}

		else if (continuer == 'r') {
			afficheTab(jardin, TAILLE);	
			croissance(jardin, TAILLE);
			afficheTab(jardin, TAILLE);
			ReduceFast(jardin, TAILLE, panda1, panda2);

			cout << "Batterie panda1 : " << panda1.batterie << endl;
			cout << "Batterie panda2 : " << panda2.batterie << endl;
		}
		cout << endl;
	}


	return 0;
}