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


void InitTab(Bambou tab[], int taille) {

	for (int i = 0; i < taille; i++) {
		tab[i].num = i;
		tab[i].taille = rand() % 15;
		tab[i].croissance = rand() % 20;
	}
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
	
	if (imax != 0) {
		Bambou tmp = tab[imax];
		tab[imax] = tab[0];
		tab[0] = tmp;
	}
	
	max = tab[1].taille;
	
	for (int i = 1; i < taille; i++) {
		if (max < tab[i].taille) {
			max = tab[i].taille;
			imax = i;
		}
	}

	deuxieme_plus_grand_ind = imax;

	Bambou tmp = tab[premier_plus_grand_ind];
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

int main(int argc, char* argv[]) {
	
	// Déclaration tableau et constante
	const int TAILLE = 12;
	Bambou jardin[TAILLE];

	// Initialisation du tableau jardin
	InitTab(jardin, TAILLE);

	// Appel fonction TailleMax qui renvoie les indices du premier et ensuire du deuxieme plus grand arbre
	// Faudra appeler cette fonction dans une future fonction qui mettra ces valeurs dans une structure statistique comme définit plus haut
	int indice_premier_plus_grand = 0, indice_deuxieme_plus_grand = 0;
	TailleMax(jardin, TAILLE, indice_premier_plus_grand, indice_deuxieme_plus_grand);

	// Moyenne des Bambous. Faudra faire pareil que pour la fonction d'avant
	float moyenne = TailleMoyenne(jardin, TAILLE);
	cout << "Moyenne de la taille des bambous: " << moyenne << endl;


	return 0;
}