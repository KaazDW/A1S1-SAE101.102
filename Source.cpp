#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include "config_sdl.h"
#include<fstream>
using namespace std;


struct Statistique {
	int Jour;
	int ind_TailleMax1;
	int ind_TailleMax2;
	int val_TailleMax1;
	int val_TailleMax2;
	float TailleMoy;
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


// Cette fonction me sert juste a tester les autres
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


int TailleMin(Bambou tab[], int taille) {

	int imin;
	imin = 0;

	for (int i = 0; i < taille; i++) {
		if (tab[imin].taille > tab[i].taille) {
			imin = i;
		}
	}
	return imin;
}


void TailleMax(Bambou tab[], int taille, int& premier_plus_grand_ind, int& deuxieme_plus_grand_ind) {

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


void deplacement(Robot& panda1, Robot& panda2, int indice_premier, int indice_deuxieme) {
	for (int i = 0; i < 12; i++) {
		panda1.position[i] = false;
		panda2.position[i] = false;
	}
	panda1.position[indice_premier] = true;
	panda2.position[indice_deuxieme] = true;
}


void batterie_et_decoupe(Bambou tab[], Robot& panda1, Robot& panda2, int indice_premier, int indice_deuxieme) {
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


int somme_croissance(Bambou tab[], int taille) {
	int somme_croissance_bambou = 0;

	for (int i = 0; i < taille; i++) {
		somme_croissance_bambou += tab[i].croissance;
	}

	return somme_croissance_bambou;
}


void ReduceMax(Bambou tab[], int taille, Robot& panda1, Robot& panda2) {
	int indice_premier = 0, indice_deuxieme = 0;
	TailleMax(tab, taille, indice_premier, indice_deuxieme);

	deplacement(panda1, panda2, indice_premier, indice_deuxieme);

	batterie_et_decoupe(tab, panda1, panda2, indice_premier, indice_deuxieme);
}


void ReduceFast(Bambou tab[], int taille, Robot& panda1, Robot& panda2) {

	int somme_croissance_bambou = somme_croissance(tab, taille);

	double x = 1 + sqrt(5);
	double taille_minimale = x * somme_croissance_bambou;

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

	Bambou tmp = tab[indice_croissance1];
	tab[indice_croissance1] = tab[0];
	tab[0] = tmp;

	max2 = tab[1].croissance;
	indice_croissance2 = 1;

	int i;

	for (i = 1; i < taille; i++) {
		if (tab[i].taille > taille_minimale && max2 <= tab[i].croissance) {
			max2 = tab[i].croissance;
			indice_croissance2 = i;
		}
	}

	if (indice_croissance1 == indice_croissance2)
		indice_croissance2 = 0;
	

	tmp = tab[indice_croissance1];
	tab[indice_croissance1] = tab[0];
	tab[0] = tmp;


	if (tab[indice_croissance1].taille > taille_minimale && tab[indice_croissance2].taille > taille_minimale) {
		deplacement(panda1, panda2, indice_croissance1, indice_croissance2);
		batterie_et_decoupe(tab, panda1, panda2, indice_croissance1, indice_croissance2);
	}
}


void InitStats(Statistique tab[], int taille, int cpt_jour, Bambou tab_jardin[], int taille_jardin, int &indice_premier_plus_grand, int &indice_deuxieme_plus_grand) {

	tab[cpt_jour].Jour = cpt_jour;

	TailleMax(tab_jardin, taille_jardin, indice_premier_plus_grand, indice_deuxieme_plus_grand);

	tab[cpt_jour].val_TailleMax1 = tab_jardin[indice_premier_plus_grand].taille;
	tab[cpt_jour].val_TailleMax2 = tab_jardin[indice_deuxieme_plus_grand].taille;
	tab[cpt_jour].ind_TailleMax1 = indice_premier_plus_grand;
	tab[cpt_jour].ind_TailleMax2 = indice_deuxieme_plus_grand;

	float moyenne = TailleMoyenne(tab_jardin, taille_jardin);

	tab[cpt_jour].TailleMoy = moyenne;

	int minimum = TailleMin(tab_jardin, taille_jardin);

	tab[cpt_jour].TailleMin = minimum;
}


void afficheStats(Statistique tab[], int jour, int indice_premier_plus_grand, int indice_deuxieme_plus_grand) {

	cout << "Jour numero : " << tab[jour].Jour << endl;
	cout << "-----------------" << endl;
	cout << "Indice du premier plus grand : " << tab[jour].ind_TailleMax1 << endl;
	cout << "Valeur du premier plus grand : " << tab[jour].val_TailleMax1 << endl;
	cout << "-----------------" << endl;
	cout << "Indice du deuxieme plus grand : " << tab[jour].ind_TailleMax2 << endl;
	cout << "Valeur du deuxieme plus grand : " << tab[jour].val_TailleMax2 << endl;
	cout << "-----------------" << endl;
}


void Sauvegarde_Jardin_Jour_Robot(Bambou tab[], Robot& panda1, Robot& panda2, int taille, int cpt_jour) {
	ofstream sortie("jardin.txt", ios::trunc);

	for (int i = 0; i < taille; i++) {
		sortie << tab[i].num << ',';
		sortie << tab[i].taille << ',';
		sortie << tab[i].croissance << endl;
	}

	for (int j = 0; j < taille; j++) {
		if (panda1.position[j] == true) {
			sortie << panda1.batterie << ',';
			sortie << j << endl;
		}
		if (panda2.position[j] == true) {
			sortie << panda2.batterie << ',';
			sortie << j << endl;
		}
	}

	sortie << cpt_jour << endl;

	sortie.close();
}

void Recharge_Sauvegarde_Jardin_Jour_Robot(Bambou tab[], Robot& panda1, Robot& panda2, int taille_tab, int& cpt_jour) {

	ifstream entree("jardin.txt", ios::in);//declaration du flot 
	if (!entree) //si le flot vaut false
		cout << "Probleme d'ouverture " << endl;
	else {
		//lecture du fichier ici (cf. ci-dessous)

		int i = 0;
		for (; i < taille_tab; i++) {
			char c_num[100], c_taille[100], c_croissance[100];
			entree.getline(c_num, 100, ',');
			entree.getline(c_taille, 100, ',');
			entree.getline(c_croissance, 100);

			int Num = atoi(c_num);
			int Taille = atoi(c_taille);
			int Croissance = atoi(c_croissance);

			tab[i].num = Num;
			tab[i].taille = Taille;
			tab[i].croissance = Croissance;
		}

		char batterie[100], position[100];
		entree.getline(batterie, 100, ',');
		entree.getline(position, 100);

		int bat = atoi(batterie);
		int pos = atoi(position);

		panda1.batterie = bat;
		panda1.position[pos] = 1;

		entree.getline(batterie, 100, ',');
		entree.getline(position, 100);

		bat = atoi(batterie);
		pos = atoi(position);

		panda2.batterie = bat;
		panda2.position[pos] = 1;

		char jour[100];
		entree.getline(jour, 100);
		cpt_jour = atoi(jour);

		entree.close();
	}

}

void Sauvegarde_Stats_Graphique(Statistique tab[], int taille) {
	ofstream sortie("stats.txt", ios::trunc);

	for (int i = 0; i < taille; i++) {
		if (tab[i].Jour >= 0) {
			sortie << tab[i].Jour << endl;
			sortie << tab[i].TailleMin << endl;
			sortie << tab[i].ind_TailleMax1 << ',' << tab[i].val_TailleMax1 << endl;
			sortie << tab[i].ind_TailleMax2 << ',' << tab[i].val_TailleMax2 << endl;
			sortie << tab[i].TailleMoy << endl;
		}
		else {
			return;
		}
	}
}

void Recharge_Sauvegarde_Stats_Graphique(Statistique tab[], int taille) {
	ifstream entree("stats.txt", ios::in);//declaration du flot 
	if (!entree) //si le flot vaut false
		cout << "Probleme d'ouverture " << endl;
	else {
		//lecture du fichier ici (cf. ci-dessous)

		while (!entree.eof()) {

			char jour[100], min[100], ind_max1[100], val_max1[100], ind_max2[100], val_max2[100], moy[100];

			entree.getline(jour, 100);
			entree.getline(min, 100);
			entree.getline(ind_max1, 100, ',');
			entree.getline(val_max1, 100);
			entree.getline(ind_max2, 100, ',');
			entree.getline(val_max2, 100);
			entree.getline(moy, 100);

			int _jour = atoi(jour);
			int _min = atoi(min);
			int _ind_max1 = atoi(ind_max1);
			int _val_max1 = atoi(val_max1);
			int _ind_max2 = atoi(ind_max2);
			int _val_max2 = atoi(val_max2);
			float _moy = atof(moy);

			tab[_jour].Jour = _jour;
			tab[_jour].TailleMin = _min;
			tab[_jour].ind_TailleMax1 = _ind_max1;
			tab[_jour].val_TailleMax1 = _val_max1;
			tab[_jour].ind_TailleMax2 = _ind_max2;
			tab[_jour].val_TailleMax2 = _val_max2;
			tab[_jour].TailleMoy = _moy;
		}

		entree.close();
	}
}



// Fonction en rapport a SDL et constantes

const int PositionX_leaveButton = 700;
const int PositionY_leaveButton = 500;
const int LargeurFenetre = 1640;
const int HauteurFenetre = 880;
SDL_Rect returnmenu_button;
SDL_Rect rectborduregauche;
SDL_Rect rectborduredroite;

bool ActivStartMenu = false;


void affiche_terre_bambou(SDL_Renderer* rendu) {
	SDL_SetRenderDrawColor(rendu, 255, 215, 0, 255);	
	SDL_RenderDrawLine(rendu, 70, 720, 850, 720);
	SDL_RenderPresent(rendu);
}


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
	rectfond.w = LargeurFenetre - 750;
	rectfond.h = HauteurFenetre - 50;
	rectfond.y = 25;
	rectfond.x = 25;
	SDL_SetRenderDrawColor(rendu, 0, 30, 40, 255);
	SDL_RenderFillRect(rendu, &rectfond);

	SDL_Rect rect;
	rect.w = LargeurFenetre - 1245;
	rect.h = HauteurFenetre - 50;
	rect.y = 25;
	rect.x = 930;
	SDL_SetRenderDrawColor(rendu, 0, 30, 40, 255);
	SDL_RenderFillRect(rendu, &rect);

	rect.w = 280;
	rect.h = HauteurFenetre - 50;
	rect.y = 25;
	rect.x = LargeurFenetre - 300;
	SDL_SetRenderDrawColor(rendu, 0, 30, 40, 255);
	SDL_RenderFillRect(rendu, &rect);

	returnmenu_button.w = 250;
	returnmenu_button.h = 50;
	returnmenu_button.y = HauteurFenetre - 90;
	returnmenu_button.x = LargeurFenetre - 285;
	SDL_SetRenderDrawColor(rendu, 0, 15, 15, 255);
	SDL_RenderFillRect(rendu, &returnmenu_button);


	affiche_terre_bambou(rendu);

	/*
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
	TTF_Quit();*/
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


void bambous_tracer_pour_reducemax(SDL_Renderer* rendu, Bambou jardin[], int taille_jardin) {
	
	SDL_Rect rectangle_inferieur;
	SDL_Rect rectangle_superieur;

	rectangle_inferieur.w = 8;
	rectangle_inferieur.h = 4;

	rectangle_superieur.w = 12;
	rectangle_superieur.h = 1;
	
	for (int i = 0; i < taille_jardin; i++) {
		rectangle_inferieur.x = 62 * i + 95;
		rectangle_superieur.x = rectangle_inferieur.x - 2;
		
		for (int j = 0; j < jardin[i].taille; j++) {		
			rectangle_superieur.y = 715 - 5  * j;
			SDL_SetRenderDrawColor(rendu, 69, 224, 11, 255); // couleur verte
			SDL_RenderFillRect(rendu, &rectangle_superieur);
			rectangle_inferieur.y = rectangle_superieur.y + 1;
			SDL_SetRenderDrawColor(rendu, 151, 160, 9, 255); // couleur verte plus fonce
			SDL_RenderFillRect(rendu, &rectangle_inferieur);
		}
	}
	SDL_RenderPresent(rendu);
}


int main(int argc, char* argv[]) {

	// Declaration tableau et constante
	const int TAILLE = 12;
	const int TAILLE_STATS = 101;
	Bambou jardin[TAILLE];
	Statistique RecupStats[TAILLE_STATS];
	int cpt_jour = 0;

	// Initialisation du tableau jardin
	InitTab(jardin, TAILLE);

	// Initialisation des indices qui nous seront utiles apres appels de fonctions.
	int indice_premier_plus_grand = 0, indice_deuxieme_plus_grand = 0;

	Robot panda1, panda2;

	ifstream entree("jardin.txt", ios::in);
	ifstream entree2("stats.txt", ios::in);
	if (!entree) {
		if (!entree2) {
			InitTab(jardin, TAILLE);
			InitRobot(panda1);
			InitRobot(panda2);
		}
		else {
			cout << "Erreur, il manque un fichier de sauvegarde." << endl;
		}
	}
	else {
		Recharge_Sauvegarde_Stats_Graphique(RecupStats, TAILLE_STATS);
		Recharge_Sauvegarde_Jardin_Jour_Robot(jardin, panda1, panda2, TAILLE, cpt_jour);
	}

	
	bool simulation = true;
	char choix_suite, mode;

	cout << "Choix mode : 'f' pour Fast, 'm' pour Max." << endl;
	cin >> mode;

	if (mode == 'm') {

		while (simulation) {

			cout << "Entrez 'r' pour relancer un jour, 'q' pour quitter." << endl;
			cin >> choix_suite;

			if (choix_suite == 'q') {
				cout << "Fin." << endl;
				simulation = false;

				Sauvegarde_Stats_Graphique(RecupStats, TAILLE_STATS);
				Sauvegarde_Jardin_Jour_Robot(jardin, panda1, panda2, TAILLE, cpt_jour);
			}

			else if (choix_suite == 'r') {

				InitStats(RecupStats, TAILLE_STATS, cpt_jour, jardin, TAILLE, indice_premier_plus_grand, indice_deuxieme_plus_grand);
				afficheStats(RecupStats, cpt_jour, indice_premier_plus_grand, indice_deuxieme_plus_grand);

				croissance(jardin, TAILLE);
				afficheTab(jardin, TAILLE);
				ReduceMax(jardin, TAILLE, panda1, panda2);
				afficheTab(jardin, TAILLE);
				cout << "Batterie panda1 : " << panda1.batterie << endl;
				cout << "Batterie panda2 : " << panda2.batterie << endl;

				cpt_jour++;
			}
			cout << endl;
		}
	}
	
	/*
	else if (mode == 'f') {

		while (simulation) {

			cout << "Entrez 'r' pour relancer un jour, 'q' pour quitter." << endl;
			cin >> choix_suite;

			if (choix_suite == 'q') {
				cout << "Fin." << endl;
				simulation = false;
			}

			else if (choix_suite == 'r') {

				InitStats(RecupStats, TAILLE_STATS, cpt_jour, jardin, TAILLE, indice_premier_plus_grand, indice_deuxieme_plus_grand);
				afficheStats(RecupStats, cpt_jour, indice_premier_plus_grand, indice_deuxieme_plus_grand);

				croissance(jardin, TAILLE);
				afficheTab(jardin, TAILLE);
				ReduceFast(jardin, TAILLE, panda1, panda2);
				afficheTab(jardin, TAILLE);
				cout << "Batterie panda1 : " << panda1.batterie << endl;
				cout << "Batterie panda2 : " << panda2.batterie << endl;

				cpt_jour++;

			}
			cout << endl;
		}
	}
	*/

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
						bambous_tracer_pour_reducemax(rendu, jardin, TAILLE);
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
		}
	}

	return 0;
}