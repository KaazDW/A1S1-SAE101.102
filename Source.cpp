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
	int position;
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
	panda.position = 11;
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


void croissance(Bambou tab[], int taille) {
	for (int i = 0; i < taille; i++) {
		tab[i].taille += tab[i].croissance;
	}
}


void deplacement(Robot &panda1, Robot &panda2, int indice_premier, int indice_deuxieme) {
	panda1.position = indice_premier;
	panda2.position = indice_deuxieme;
}


void ReduceMax(Bambou tab[], int taille, Robot &panda1, Robot &panda2) {
	int indice_premier = 0, indice_deuxieme = 0;
	TailleMax(tab, taille, indice_premier, indice_deuxieme);

	deplacement(panda1, panda2, indice_premier, indice_deuxieme);
	
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

/*
void ReduceFastest(Bambou tab[], int taille) {

	int somme_croissance_bambou = 0;

	for (int i = 0; i < taille; i++) {
		somme_croissance_bambou += tab[i].croissance;
	}

	float x = 1 + sqrt(5);
	float taille_minimale = x * somme_croissance_bambou;

	bool taille_sup[12];

	for (int i = 0; i < taille; i++) {
		if (tab[i].taille >= taille_minimale)
			taille_sup[i] = true;
		else
			taille_sup[i] = false;
	}

	int indice_premier_plus_vite = 0, indice_deuxieme_plus_vite = 0;
	int max_premier, max_deuxieme;
	int tmp_indice, tmp_max;

	

}
*/

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

	// Test croissance
	/*
	afficheTab(jardin, TAILLE);
	croissance(jardin, TAILLE);
	afficheTab(jardin, TAILLE);
	*/

	// Test ReduceMax
	Robot panda1, panda2;
	InitRobot(panda1);
	InitRobot(panda2);

	ReduceMax(jardin, TAILLE, panda1, panda2);
	cout << "position panda1 : " << panda1.position << endl;
	cout << "position panda2 : " << panda2.position << endl;
	cout << "Battrie panda1 : " << panda1.batterie << endl;
	cout << "Battrie panda2 : " << panda2.batterie << endl;



	return 0;
}