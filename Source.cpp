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

int main(int argc, char* argv[]) {
	//tentative de push 
	//commentaire
	// Mathias test
	//dev

	const int TAILLE = 12;
	Bambou Jardin[TAILLE];

	InitTab(Jardin, TAILLE);

	cout << Jardin[0].num << " ";
	cout << Jardin[0].taille << " ";
	cout << Jardin[0].croissance << endl;

	cout << Jardin[11].num << " ";
	cout << Jardin[11].taille << " ";
	cout << Jardin[11].croissance << endl;

	return 0;
}