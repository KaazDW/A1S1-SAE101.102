#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include "config_sdl.h"
using namespace std;



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
			imax = i;
		}
	}

	deuxieme_plus_grand_ind = imax;

	Bambou tmp = tab[premier_plus_grand_ind];
	tab[premier_plus_grand_ind] = tab[0];
	tab[0] = tmp;
}


int main(int argc, char* argv[]) {
	
	// Déclaration tableau et constante
	const int TAILLE = 12;
	Bambou jardin[TAILLE];

	// Initialisation du tableau jardin
	InitTab(jardin, TAILLE);

	for (int i = 0; i < TAILLE; i++) {
		cout << jardin[i].taille << " ";
	}

	// Appel fo,ction TailleMax qui renvoie les indices du premier t esnuite du deuxieme plus grand arbre
	int indice_premier_plus_grand = 0, indice_deuxieme_plus_grand = 0;
	TailleMax(jardin, TAILLE, indice_premier_plus_grand, indice_deuxieme_plus_grand);

	cout << " --- " << indice_premier_plus_grand << " --- " << indice_deuxieme_plus_grand;

	return 0;
}