#include<fstream>
#include<SDL.h>     //ne pas oublier
#include<SDL_ttf.h> //ne pas oublier
#include<iostream>
#include "config_sdl.h"
using namespace std;
const int LARGEUR = 12;  //largeur fenetre
const int HAUTEUR = 16;  //hauteur fenetre
const int CARRE = 30;
const int TAILLEX = CARRE * LARGEUR + 120;
const int TAILLEY = CARRE * HAUTEUR;


void charger(const char* nom_fichier, SDL_Color tabcouleur[HAUTEUR][LARGEUR], SDL_Renderer* rendu) {
    ifstream lecture(nom_fichier, ios::in);
    if (!lecture) {
        cout << "Erreur lecture fichier" << endl;
    }
    else {
        char lettre[2];
        for (int i = 0; i < HAUTEUR; i++) {
            for (int j = 0; j < LARGEUR; j++) {
                if (j < LARGEUR - 1)
                    lecture.getline(lettre, 2, ' ');
                else
                    lecture.getline(lettre, 2);

                if (lettre[0] == 'B') {
                    tabcouleur[i][j].r = 255;
                    tabcouleur[i][j].g = 255;
                    tabcouleur[i][j].b = 255;
                }
                else if (lettre[0] == 'N') {
                    tabcouleur[i][j].r = 0;
                    tabcouleur[i][j].g = 0;
                    tabcouleur[i][j].b = 0;

                }
                else if (lettre[0] == 'R') {
                    tabcouleur[i][j].r = 255;
                    tabcouleur[i][j].g = 0;
                    tabcouleur[i][j].b = 0;

                }
                else if (lettre[0] == 'V') {
                    tabcouleur[i][j].r = 106;
                    tabcouleur[i][j].g = 164;
                    tabcouleur[i][j].b = 30;
                }
                else if (lettre[0] == 'J') {
                    tabcouleur[i][j].r = 255;
                    tabcouleur[i][j].g = 255;
                    tabcouleur[i][j].b = 0;
                }
                else if (lettre[0] == 'L') {
                    tabcouleur[i][j].r = 0;
                    tabcouleur[i][j].g = 0;
                    tabcouleur[i][j].b = 255;
                }
            }
        }
    }
    lecture.close();
}

void afficheTabCouleur(SDL_Renderer* rendu, SDL_Color tab[HAUTEUR][LARGEUR]) {
    SDL_Rect rect;
    rect.w = rect.h = CARRE;
    for (int i = 0; i < HAUTEUR; i++) {
        rect.y = i * CARRE;
        for (int j = 0; j < LARGEUR; j++) {
            // Choix pinceau
            SDL_SetRenderDrawColor(rendu, tab[i][j].r, tab[i][j].g, tab[i][j].b, 255);
            // Rectangle
            rect.x = j * CARRE;
            SDL_RenderFillRect(rendu, &rect);
        }
    }
    SDL_RenderPresent(rendu);
}

void negatif(SDL_Color tabcouleur[HAUTEUR][LARGEUR], SDL_Renderer* rendu) {
    for (int i = 0; i < HAUTEUR; i++) {
        for (int j = 0; j < LARGEUR; j++) {
            tabcouleur[i][j].r = 255 - tabcouleur[i][j].r;
            tabcouleur[i][j].g = 255 - tabcouleur[i][j].g;
            tabcouleur[i][j].b = 255 - tabcouleur[i][j].b;
        }
    }
    afficheTabCouleur(rendu, tabcouleur);
}

void charger_palette(SDL_Renderer* rendu) {
    SDL_Rect rectangle_noir;
    rectangle_noir.x = TAILLEX - 100;
    rectangle_noir.y = 30;
    rectangle_noir.w = 20;
    rectangle_noir.h = 20;
    SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);

    SDL_Rect rectangle_blanc;
    rectangle_blanc.x = TAILLEX - 70;
    rectangle_blanc.y = 30;
    rectangle_blanc.w = 20;
    rectangle_blanc.h = 20;
    SDL_SetRenderDrawColor(rendu, 255, 255, 255, 255);

    SDL_Rect rectangle_rouge;
    rectangle_rouge.x = TAILLEX - 70;
    rectangle_rouge.y = 30;
    rectangle_rouge.w = 20;
    rectangle_rouge.h = 20;
    SDL_SetRenderDrawColor(rendu, 255, 255, 255, 255);
}

int main(int argn, char* argv[]) {

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        cout << "Echec à l’ouverture";
        return 1;
    }

    SDL_Color tabcouleur[HAUTEUR][LARGEUR];

    // GENERATION

    SDL_Window* fenetre = SDL_CreateWindow("Mon app",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        TAILLEX,
        TAILLEY,
        SDL_WINDOW_SHOWN
    );

    if (fenetre == NULL) {
        cout << "Erreur ouverture fenetre";
    }

    SDL_Renderer* dessin = SDL_CreateRenderer(
        fenetre,
        -1,
        SDL_RENDERER_ACCELERATED
    );

    charger("Mystere.txt", tabcouleur, dessin);


    bool continuer = true;
    SDL_Event event;

    while (continuer) {
        SDL_RenderPresent(dessin);
        SDL_WaitEvent(&event);
        switch (event.type)
        {
        case SDL_QUIT:
            continuer = false;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_n) {
                negatif(tabcouleur, dessin);
            }
            break;
        }
    }


    // DESTRUCTION

    SDL_DestroyRenderer(dessin);

    SDL_DestroyWindow(fenetre);

    SDL_Quit();

    return 0;
}