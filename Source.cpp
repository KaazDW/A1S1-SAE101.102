#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include "config_sdl.h"
using namespace std;

int main(int argc, char* argv[]) {
	//tentative de push 
	//commentaire
	// Aylin test

	return 0;
}


struct Taille{
	int Jour;
	int TailleMax1;
	int TailleMax2;
	int TailleMoy;
	int TailleMin;
}

struct RobotPanda {
	int nRobot;
	int Batterie;
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


void TailleMoy(Bambou tab[], int taille) {
	tab[i].taille;

}

