#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include "config_sdl.h"
using namespace std;

const int TAILLE_TAB = 6;
int bambou_taille[TAILLE_TAB] = { 2, 4, 1, 8, 12, 0 };


void TailleMax(int &premier_plus_grand_ind, int &deuxieme_plus_grand_ind) {

	int max = bambou_taille[0];
	int imax = 0;

	for (int i = 0; i < TAILLE_TAB; i++) {
		if (max < bambou_taille[i]) {
			max = bambou_taille[i];
			imax = i;
		}
	}

	premier_plus_grand_ind = imax;
	
	if (imax != 0) {
		int tmp = bambou_taille[imax];
		bambou_taille[imax] = bambou_taille[0];
		bambou_taille[0] = tmp;
	}
	
	max = bambou_taille[1];
	
	for (int i = 1; i < TAILLE_TAB; i++) {
		if (max < bambou_taille[i]) {
			max = bambou_taille[i];
			imax = i;
		}
	}

	deuxieme_plus_grand_ind = imax;
}


int main(int argc, char* argv[]) {
	
	int indice_premier_plus_grand = 0, indice_deuxieme_plus_grand = 0;
	TailleMax(indice_premier_plus_grand, indice_deuxieme_plus_grand);

	cout << indice_premier_plus_grand << " --- " << indice_deuxieme_plus_grand;

	return 0;
}