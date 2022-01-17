#include<SDL.h>
#include<SDL_ttf.h>
#include<iostream>
#include "config_sdl.h"

using namespace std;

const int PositionX_leaveButton = 700;
const int PositionY_leaveButton = 500;
const int LargeurFenetre = 1640;
const int HauteurFenetre = 880;
SDL_Rect returnmenu_button;
SDL_Rect rectborduregauche;
SDL_Rect rectborduredroite;

bool ActivStartMenu = false;


void affiche(SDL_Renderer* rendu) {
	ActivStartMenu = false;
	SDL_RenderClear(rendu);
	SDL_Rect rectarriereplan;
	rectarriereplan.w = LargeurFenetre;
	rectarriereplan.h = HauteurFenetre;
	rectarriereplan.y = 0;
	rectarriereplan.x = 0;
	SDL_SetRenderDrawColor(rendu, 0, 0, 0, 0);
	SDL_RenderFillRect(rendu, &rectarriereplan);

	SDL_Rect rectfond;
	rectfond.w = LargeurFenetre - 350;
	rectfond.h = HauteurFenetre - 50 ;
	rectfond.y = 25;
	rectfond.x = 25;
	SDL_SetRenderDrawColor(rendu, 0, 30, 40, 255);
	SDL_RenderFillRect(rendu, &rectfond);

	SDL_Rect rect;
	rect.w = 280;
	rect.h = HauteurFenetre - 50;
	rect.y = 25;
	rect.x = LargeurFenetre -300;
	SDL_SetRenderDrawColor(rendu, 0, 30, 40, 255);
	SDL_RenderFillRect(rendu, &rect);

	returnmenu_button.w = 250;
	returnmenu_button.h = 50;
	returnmenu_button.y = HauteurFenetre - 90;
	returnmenu_button.x = LargeurFenetre - 285;
	SDL_SetRenderDrawColor(rendu, 0, 15, 15, 255);
	SDL_RenderFillRect(rendu, &returnmenu_button);

	TTF_Init();
	TTF_Font* font = NULL;
	font = TTF_OpenFont("ARLRDBD.ttf", 12);

	SDL_Color text_color = { 255, 0,255 };
	if (font != 0) {
		SDL_Surface* texte = TTF_RenderText_Blended(font, "SAlut SAlut bonjour coucou", text_color);
		//affichage
		SDL_FreeSurface(texte); //On oublie toujours pas
		TTF_CloseFont(font);
	}
	else { cout << "foirage à l'ouverture de times.ttf" << endl; }
	SDL_RenderPresent(rendu);
	TTF_Quit();
}


int start_choice(SDL_Renderer* rendu) { /*Menu de choix*/
	ActivStartMenu = true;
	SDL_Rect rect;
	/*Zone de fond*/
	rect.w = LargeurFenetre - 50;
	rect.h = HauteurFenetre - 50;
	rect.y = 25;
	rect.x = 25;
	SDL_SetRenderDrawColor(rendu, 0, 30, 40, 255);
	SDL_RenderFillRect(rendu, &rect);

	/*Choix 1 bordure*/
	rectborduregauche.w = 270;
	rectborduregauche.h = 170;
	rectborduregauche.y = HauteurFenetre / 2;
	rectborduregauche.x = LargeurFenetre / 3 - 120;
	SDL_SetRenderDrawColor(rendu, 255, 255, 255, 255);
	SDL_RenderFillRect(rendu, &rectborduregauche);

	/*Choix 2 bordure*/
	rectborduredroite.w = 270;
	rectborduredroite.h = 170;
	rectborduredroite.y = HauteurFenetre / 2;
	rectborduredroite.x = (LargeurFenetre / 3) * 2 - 120;
	SDL_SetRenderDrawColor(rendu, 255, 255, 255, 255);
	SDL_RenderFillRect(rendu, &rectborduredroite);

	/*Choix 1*/
	rect.w = 260;
	rect.h = 160;
	rect.y = HauteurFenetre / 2 + 5;
	rect.x = LargeurFenetre / 3 - 120 + 5;
	SDL_SetRenderDrawColor(rendu, 0, 50, 50, 255);
	SDL_RenderFillRect(rendu, &rect);

	/*Choix 2*/
	rect.y = HauteurFenetre / 2 + 5;
	rect.x = (LargeurFenetre / 3) * 2 - 120 + 5;
	SDL_SetRenderDrawColor(rendu, 50, 20, 30, 255);
	SDL_RenderFillRect(rendu, &rect);

/*
	SDL_Surface* image = IMG_Load("shutdownlogo.png");
	if (!image)
	{
		cout << "Erreur de chargement de l'image : %s", SDL_GetError();
		return -1;
	}

	SDL_Texture* monImage = SDL_CreateTextureFromSurface(rendu, image);
	SDL_FreeSurface(image);

	SDL_Rect posImg;
	posImg.x = 100;
	posImg.y = 100;

	SDL_QueryTexture(monImage, NULL, NULL, &posImg.w, &posImg.h);
	SDL_RenderCopy(rendu, monImage, NULL, &posImg);
	*/
	SDL_RenderPresent(rendu);
	return 0;
}


int main(int argn, char* argv[]) {
	//ouverture de la SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		cout << "Echec à l’ouverture";
		return 1;
	}

	//on crée la fenêtre
	SDL_Window* win = SDL_CreateWindow("PandaRobot Simulator",
		SDL_WINDOWPOS_CENTERED,     //pos. X: autre option: SDL_WINDOWPOS_UNDEFINED
		SDL_WINDOWPOS_CENTERED,     //pos. Y: autre option: SDL_WINDOWPOS_UNDEFINED 
		LargeurFenetre,
		HauteurFenetre,
		SDL_WINDOW_SHOWN //d’autres options (plein ecran, resizable, sans bordure...)
	);
	if (win == NULL)
		cout << "erreur ouverture fenetre";

	//Création d’un dessin associé à la fenêtre (1 seul renderer par fenetre)
	SDL_Renderer* rendu = SDL_CreateRenderer(
		win,  //nom de la fenêtre
		-1, //par défaut
		SDL_RENDERER_ACCELERATED); //utilisation du GPU, valeur recommandée
	start_choice(rendu);
	/*affiche(rendu);*/
	SDL_RenderPresent(rendu);


	bool continuer = true;   //booléen fin de programme
	SDL_Event event;//gestion des évènements souris/clavier, 
					//SDL_Event est de type struct
	while (continuer) {
		SDL_WaitEvent(&event);//attente d’un évènement
		switch (event.type) //test du type d’évènement
		{
		case SDL_QUIT: //clic sur la croix de fermeture
						//on peut enlever SDL_Delay
			continuer = false;
			break;
		case SDL_MOUSEBUTTONUP://appui souris
			if (event.button.button == SDL_BUTTON_LEFT) {
				if (ActivStartMenu == true &&
					event.button.x > rectborduredroite.x &&
					event.button.x<rectborduredroite.x + rectborduredroite.w &&
					event.button.y>rectborduredroite.y &&
					event.button.y < rectborduredroite.y + rectborduredroite.h) {
					affiche(rendu);
				}
				SDL_RenderPresent(rendu);//on rafraichit
				if (event.button.button == SDL_BUTTON_LEFT) {
					if (ActivStartMenu == true &&
						event.button.x > rectborduregauche.x &&
						event.button.x<rectborduregauche.x + rectborduregauche.w &&
						event.button.y>rectborduregauche.y &&
						event.button.y < rectborduregauche.y + rectborduregauche.h) {
						affiche(rendu);
					}
					SDL_RenderPresent(rendu);//on rafraichit
					if (event.button.button == SDL_BUTTON_LEFT) {//si on clique bouton gauche
						if (event.button.x > returnmenu_button.x &&
							event.button.x<returnmenu_button.x + returnmenu_button.w &&
							event.button.y>returnmenu_button.y &&
							event.button.y < returnmenu_button.y + returnmenu_button.h) {
							start_choice(rendu);
						}
						SDL_RenderPresent(rendu);//on rafraichit

						break;

					}

				}
			}
			//destruction du renderer à la fin
			SDL_DestroyRenderer(rendu);

			//destruction à la fin
			SDL_DestroyWindow(win);   //equivalent du delete

			//fermeture
			SDL_Quit();
			return 0;
		}
	}
}