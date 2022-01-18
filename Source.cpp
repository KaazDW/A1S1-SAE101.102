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

	if (panda1.batterie != 0 && panda2.batterie != 0)
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
		if (panda1.batterie != 0 && panda2.batterie != 0)
			deplacement(panda1, panda2, indice_croissance1, indice_croissance2);
		batterie_et_decoupe(tab, panda1, panda2, indice_croissance1, indice_croissance2);
	}
}


void InitStats(Statistique tab[], int taille, int cpt_jour, Bambou tab_jardin[], int taille_jardin, int& indice_premier_plus_grand, int& indice_deuxieme_plus_grand) {

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


/*

	 __  __  ___   ____   __           __  __
	/ / / / / __| | __ \ | |          / / / /
   / / / / | (___ | |  | | |         / / / /
  / / / /   \___ \| |  | | |        / / / /
 / / / /    ____) | |__| | |____   / / / /
/_/ /_/    |_____/|_____/|______| /_/ /_/


*/

const int PositionX_leaveButton = 700;
const int PositionY_leaveButton = 500;
const int LargeurFenetre = 1640;
const int HauteurFenetre = 880;
SDL_Rect returnmenu_button;
SDL_Rect rectborduregauche;
SDL_Rect rectborduredroite;
SDL_Rect RectChoixSaveGauche;
SDL_Rect RectChoixSaveMillieu;
SDL_Rect RectChoixSaveDroite;

bool ActivChoixDroite = false;
bool ActivChoixMillieu = false;
bool ActivChoixGauche = false;
bool ActivStartMenu = false;


void affiche_terre_bambou(SDL_Renderer* rendu) {
	SDL_SetRenderDrawColor(rendu, 255, 215, 0, 255);
	SDL_RenderDrawLine(rendu, 70, 720, 850, 720);
	SDL_RenderPresent(rendu);
}


void affiche(SDL_Renderer* rendu, Statistique tab[], int jour) {
	ActivStartMenu = false;
	SDL_RenderClear(rendu);
	SDL_Rect rectarriereplan; //fond noir
	rectarriereplan.w = LargeurFenetre;
	rectarriereplan.h = HauteurFenetre;
	rectarriereplan.y = 0;
	rectarriereplan.x = 0;
	SDL_SetRenderDrawColor(rendu, 0, 0, 0, 0);
	SDL_RenderFillRect(rendu, &rectarriereplan);

	SDL_Rect rectfond; //fond gauche affichage des bambou
	rectfond.w = LargeurFenetre - 750;
	rectfond.h = HauteurFenetre - 50;
	rectfond.y = 25;
	rectfond.x = 25;
	SDL_SetRenderDrawColor(rendu, 0, 30, 40, 255);
	SDL_RenderFillRect(rendu, &rectfond);

	SDL_Rect rect; //fond millieu affichage des graphiques
	rect.w = LargeurFenetre - 1245;
	rect.h = HauteurFenetre - 50;
	rect.y = 25;
	rect.x = 930;
	SDL_SetRenderDrawColor(rendu, 0, 30, 40, 255);
	SDL_RenderFillRect(rendu, &rect);

	rect.w = 280; //fond droite affichage des info stat button
	rect.h = HauteurFenetre - 50;
	rect.y = 25;
	rect.x = LargeurFenetre - 300;
	SDL_SetRenderDrawColor(rendu, 0, 30, 40, 255);
	SDL_RenderFillRect(rendu, &rect);

	returnmenu_button.w = 250; //Button retour au menu de choix
	returnmenu_button.h = 50;
	returnmenu_button.y = HauteurFenetre - 90;
	returnmenu_button.x = LargeurFenetre - 285;
	SDL_SetRenderDrawColor(rendu, 0, 15, 15, 255);
	SDL_RenderFillRect(rendu, &returnmenu_button);

	/*Button de choix save*/
	int TailleCarreButton = 70;
	RectChoixSaveGauche.w = TailleCarreButton;//Gauche
	RectChoixSaveGauche.h = TailleCarreButton;
	RectChoixSaveGauche.y = HauteurFenetre - 180;
	RectChoixSaveGauche.x = LargeurFenetre - 285;
	SDL_SetRenderDrawColor(rendu, 50, 0, 15, 255);
	SDL_RenderFillRect(rendu, &RectChoixSaveGauche);
	RectChoixSaveMillieu.w = TailleCarreButton;//Millieu
	RectChoixSaveMillieu.h = TailleCarreButton;
	RectChoixSaveMillieu.y = HauteurFenetre - 180;
	RectChoixSaveMillieu.x = LargeurFenetre - 285 + 90;
	SDL_SetRenderDrawColor(rendu, 50, 0, 15, 255);
	SDL_RenderFillRect(rendu, &RectChoixSaveMillieu);
	RectChoixSaveDroite.w = TailleCarreButton;//Droite
	RectChoixSaveDroite.h = TailleCarreButton;
	RectChoixSaveDroite.y = HauteurFenetre - 180;
	RectChoixSaveDroite.x = LargeurFenetre - 285 + 90 * 2;
	SDL_SetRenderDrawColor(rendu, 50, 0, 15, 255);
	SDL_RenderFillRect(rendu, &RectChoixSaveDroite);


	//Texte Chiffre Ligne Bambou

	TTF_Init();
	TTF_Font* font = TTF_OpenFont("C:\\Windows\\Fonts\\Calibri.ttf", 21);
	SDL_Color blanc = { 255, 255, 255 , 80};

	SDL_Rect pos_chiffre;

		pos_chiffre.x = 93;
		pos_chiffre.y = 730;
		SDL_Texture* texture1 = loadText(rendu, "0", blanc, font);
		SDL_QueryTexture(texture1, NULL, NULL, &pos_chiffre.w, &pos_chiffre.h);
		SDL_RenderCopy(rendu, texture1, NULL, &pos_chiffre);
		SDL_RenderPresent(rendu);
		SDL_DestroyTexture(texture1);

		pos_chiffre.x = 155;
		pos_chiffre.y = 730;
		SDL_Texture* texture3 = loadText(rendu, "1", blanc, font);
		SDL_QueryTexture(texture3, NULL, NULL, &pos_chiffre.w, &pos_chiffre.h);
		SDL_RenderCopy(rendu, texture3, NULL, &pos_chiffre);
		SDL_RenderPresent(rendu);
		SDL_DestroyTexture(texture3);

		pos_chiffre.x = 217;
		pos_chiffre.y = 730;
		SDL_Texture* texture2 = loadText(rendu, "2", blanc, font);
		SDL_QueryTexture(texture2, NULL, NULL, &pos_chiffre.w, &pos_chiffre.h);
		SDL_RenderCopy(rendu, texture2, NULL, &pos_chiffre);
		SDL_RenderPresent(rendu);
		SDL_DestroyTexture(texture2);

		pos_chiffre.x = 279;
		pos_chiffre.y = 730;
		SDL_Texture* texture4 = loadText(rendu, "3", blanc, font);
		SDL_QueryTexture(texture4, NULL, NULL, &pos_chiffre.w, &pos_chiffre.h);
		SDL_RenderCopy(rendu, texture4, NULL, &pos_chiffre);
		SDL_RenderPresent(rendu);
		SDL_DestroyTexture(texture4);

		pos_chiffre.x = 341;
		pos_chiffre.y = 730;
		SDL_Texture* texture0 = loadText(rendu, "4", blanc, font);
		SDL_QueryTexture(texture0, NULL, NULL, &pos_chiffre.w, &pos_chiffre.h);
		SDL_RenderCopy(rendu, texture0, NULL, &pos_chiffre);
		SDL_RenderPresent(rendu);
		SDL_DestroyTexture(texture0);

		pos_chiffre.x = 403;
		pos_chiffre.y = 730;
		SDL_Texture* texture5 = loadText(rendu, "5", blanc, font);
		SDL_QueryTexture(texture5, NULL, NULL, &pos_chiffre.w, &pos_chiffre.h);
		SDL_RenderCopy(rendu, texture5, NULL, &pos_chiffre);
		SDL_RenderPresent(rendu);
		SDL_DestroyTexture(texture5);

		pos_chiffre.x = 465;
		pos_chiffre.y = 730;
		SDL_Texture* texture6 = loadText(rendu, "6", blanc, font);
		SDL_QueryTexture(texture6, NULL, NULL, &pos_chiffre.w, &pos_chiffre.h);
		SDL_RenderCopy(rendu, texture6, NULL, &pos_chiffre);
		SDL_RenderPresent(rendu);
		SDL_DestroyTexture(texture6);

		pos_chiffre.x = 527;
		pos_chiffre.y = 730;
		SDL_Texture* texture7 = loadText(rendu, "7", blanc, font);
		SDL_QueryTexture(texture7, NULL, NULL, &pos_chiffre.w, &pos_chiffre.h);
		SDL_RenderCopy(rendu, texture7, NULL, &pos_chiffre);
		SDL_RenderPresent(rendu);
		SDL_DestroyTexture(texture7);

		pos_chiffre.x = 589;
		pos_chiffre.y = 730;
		SDL_Texture* texture8 = loadText(rendu, "8", blanc, font);
		SDL_QueryTexture(texture8, NULL, NULL, &pos_chiffre.w, &pos_chiffre.h);
		SDL_RenderCopy(rendu, texture8, NULL, &pos_chiffre);
		SDL_RenderPresent(rendu);
		SDL_DestroyTexture(texture8);

		pos_chiffre.x = 651;
		pos_chiffre.y = 730;
		SDL_Texture* texture9 = loadText(rendu, "9", blanc, font);
		SDL_QueryTexture(texture9, NULL, NULL, &pos_chiffre.w, &pos_chiffre.h);
		SDL_RenderCopy(rendu, texture9, NULL, &pos_chiffre);
		SDL_RenderPresent(rendu);
		SDL_DestroyTexture(texture9);

		pos_chiffre.x = 706;
		pos_chiffre.y = 730;
		SDL_Texture* texture10 = loadText(rendu, "10", blanc, font);
		SDL_QueryTexture(texture10, NULL, NULL, &pos_chiffre.w, &pos_chiffre.h);
		SDL_RenderCopy(rendu, texture10, NULL, &pos_chiffre);
		SDL_RenderPresent(rendu);
		SDL_DestroyTexture(texture10);

		pos_chiffre.x = 768;
		pos_chiffre.y = 730;
		SDL_Texture* texture11 = loadText(rendu, "11", blanc, font);
		SDL_QueryTexture(texture11, NULL, NULL, &pos_chiffre.w, &pos_chiffre.h);
		SDL_RenderCopy(rendu, texture11, NULL, &pos_chiffre);
		SDL_RenderPresent(rendu);
		SDL_DestroyTexture(texture11);


	//Texte stats droite
		//Jour :

		TTF_Init();
		TTF_Font* fontjour = TTF_OpenFont("C:\\Windows\\Fonts\\Calibrii.TTF", 35);
		SDL_Color rougejour = { 255, 100, 100 , 255 };

		SDL_Rect pos_jour;

		pos_jour.x = 1363;
		pos_jour.y = 60;
		SDL_Texture* texturejour = loadText(rendu, "Jour : ", rougejour, fontjour);
		SDL_QueryTexture(texturejour, NULL, NULL, &pos_jour.w, &pos_jour.h);
		SDL_RenderCopy(rendu, texturejour, NULL, &pos_jour);
		SDL_RenderPresent(rendu);
		SDL_DestroyTexture(texturejour);

	//Taille min
		TTF_Init();
		TTF_Font* fonttextestat = TTF_OpenFont("C:\\Windows\\Fonts\\Calibri.TTF", 25);
		SDL_Color couleurtext = { 100, 100, 100 , 255 };

		SDL_Rect pos_text;
		pos_text.x = 1363;
		pos_text.y = 120;
		SDL_Texture* texturetext = loadText(rendu, "Taille Min : ", couleurtext, fonttextestat);
		SDL_QueryTexture(texturetext, NULL, NULL, &pos_text.w, &pos_text.h);
		SDL_RenderCopy(rendu, texturetext, NULL, &pos_text);
		SDL_RenderPresent(rendu);
		SDL_DestroyTexture(texturetext);

	//Taille max 1
		SDL_Rect pos_textmin1;
		pos_textmin1.x = 1363;
		pos_textmin1.y = 150;
		SDL_Texture* texturetextmin1 = loadText(rendu, "Taille Max 1 :", couleurtext, fonttextestat);
		SDL_QueryTexture(texturetextmin1, NULL, NULL, &pos_textmin1.w, &pos_textmin1.h);
		SDL_RenderCopy(rendu, texturetextmin1, NULL, &pos_textmin1);
		SDL_RenderPresent(rendu);
		SDL_DestroyTexture(texturetextmin1);

	//Taille max 2
		SDL_Rect pos_textmin2;
		pos_textmin2.x = 1363;
		pos_textmin2.y = 180;
		SDL_Texture* texturetextmin2 = loadText(rendu, "Taille Max 2 :" , couleurtext, fonttextestat);
		SDL_QueryTexture(texturetextmin2, NULL, NULL, &pos_textmin2.w, &pos_textmin2.h);
		SDL_RenderCopy(rendu, texturetextmin2, NULL, &pos_textmin2);
		SDL_RenderPresent(rendu);
		SDL_DestroyTexture(texturetextmin2);

	//Taille moy
		SDL_Rect pos_textmoy;
		pos_textmoy.x = 1363;
		pos_textmoy.y = 210;
		SDL_Texture* texturetextmoy = loadText(rendu, "Taille Moy : ", couleurtext, fonttextestat);
		SDL_QueryTexture(texturetextmoy, NULL, NULL, &pos_textmoy.w, &pos_textmoy.h);
		SDL_RenderCopy(rendu, texturetextmoy, NULL, &pos_textmoy);
		SDL_RenderPresent(rendu);
		SDL_DestroyTexture(texturetextmoy);

	//Button 1
		SDL_Rect pos_textbutton1;
		pos_textbutton1.x = 1385;
		pos_textbutton1.y = 725;
		SDL_Texture* texturebutton1 = loadText(rendu, "1", couleurtext, fonttextestat);
		SDL_QueryTexture(texturebutton1, NULL, NULL, &pos_textbutton1.w, &pos_textbutton1.h);
		SDL_RenderCopy(rendu, texturebutton1, NULL, &pos_textbutton1);
		SDL_RenderPresent(rendu);
		SDL_DestroyTexture(texturebutton1);

		SDL_Rect pos_textbutton2;
		pos_textbutton2.x = 1473;
		pos_textbutton2.y = 725;
		SDL_Texture* texturebutton2 = loadText(rendu, "2", couleurtext, fonttextestat);
		SDL_QueryTexture(texturebutton2, NULL, NULL, &pos_textbutton2.w, &pos_textbutton2.h);
		SDL_RenderCopy(rendu, texturebutton2, NULL, &pos_textbutton2);
		SDL_RenderPresent(rendu);
		SDL_DestroyTexture(texturebutton2);

		SDL_Rect pos_textbutton3;
		pos_textbutton3.x = 1563;
		pos_textbutton3.y = 725;
		SDL_Texture* texturebutton3 = loadText(rendu, "3", couleurtext, fonttextestat);
		SDL_QueryTexture(texturebutton3, NULL, NULL, &pos_textbutton3.w, &pos_textbutton3.h);
		SDL_RenderCopy(rendu, texturebutton3, NULL, &pos_textbutton3);
		SDL_RenderPresent(rendu);
		SDL_DestroyTexture(texturebutton3);

		SDL_Rect pos_textmenu;
		pos_textmenu.x = 1450;
		pos_textmenu.y = 805;
		SDL_Texture* texturebuttonmenu = loadText(rendu, "MENU", couleurtext, fonttextestat);
		SDL_QueryTexture(texturebuttonmenu, NULL, NULL, &pos_textmenu.w, &pos_textmenu.h);
		SDL_RenderCopy(rendu, texturebuttonmenu, NULL, &pos_textmenu);
		SDL_RenderPresent(rendu);
		SDL_DestroyTexture(texturebuttonmenu);

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

	TTF_Init();
	TTF_Font* font = TTF_OpenFont("C:\\Windows\\Fonts\\SHOWG.TTF", 60);
	SDL_Color blanc = { 90, 90, 90 , 250 };
	SDL_Color devant = { 255, 255, 255 , 255 };
	SDL_Color textdansbox = { 255, 255, 255, 255 };
	SDL_Color groupe = { 255, 255, 255 , 180 };


	SDL_Rect pos_titre1;
	pos_titre1.x = 460;
	pos_titre1.y = 190;
	SDL_Texture* texturetitre = loadText(rendu, "Panda'Robot Simulator", blanc, font);
	SDL_QueryTexture(texturetitre, NULL, NULL, &pos_titre1.w, &pos_titre1.h);
	SDL_RenderCopy(rendu, texturetitre, NULL, &pos_titre1);
	SDL_RenderPresent(rendu);
	SDL_DestroyTexture(texturetitre);


	SDL_Rect pos_titre2;
	pos_titre2.x = 460;
	pos_titre2.y = 186;
	SDL_Texture* texturetitre2 = loadText(rendu, "Panda'Robot Simulator", devant, font);
	SDL_QueryTexture(texturetitre2, NULL, NULL, &pos_titre2.w, &pos_titre2.h);
	SDL_RenderCopy(rendu, texturetitre2, NULL, &pos_titre2);
	SDL_RenderPresent(rendu);
	SDL_DestroyTexture(texturetitre2);

	TTF_Init();
	TTF_Font* fontgroupe = TTF_OpenFont("C:\\Windows\\Fonts\\GILSANUB.TTF", 25);

	SDL_Rect pos_titregroupe;
	pos_titregroupe.x = 683;
	pos_titregroupe.y = 276;
	SDL_Texture* texturegroupe = loadText(rendu, "Groupe 1 - 8 (S1G1)", groupe, fontgroupe);
	SDL_QueryTexture(texturegroupe, NULL, NULL, &pos_titregroupe.w, &pos_titregroupe.h);
	SDL_RenderCopy(rendu, texturegroupe, NULL, &pos_titregroupe);
	SDL_RenderPresent(rendu);
	SDL_DestroyTexture(texturegroupe);

	TTF_Init();
	TTF_Font* fontmode = TTF_OpenFont("C:\\Windows\\Fonts\\GILSANUB.TTF", 23);

	SDL_Rect pos_reducetitre;
	pos_reducetitre.x = 475;
	pos_reducetitre.y = 510;
	SDL_Texture* texturereduce = loadText(rendu, "ReduceMax", textdansbox, fontmode);
	SDL_QueryTexture(texturereduce, NULL, NULL, &pos_reducetitre.w, &pos_reducetitre.h);
	SDL_RenderCopy(rendu, texturereduce, NULL, &pos_reducetitre);
	SDL_RenderPresent(rendu);
	SDL_DestroyTexture(texturereduce);

	SDL_Rect pos_fastesttitre;
	pos_fastesttitre.x = 1025;
	pos_fastesttitre.y = 510;
	SDL_Texture* texturefast = loadText(rendu, "ReduceFast", textdansbox, fontmode);
	SDL_QueryTexture(texturefast, NULL, NULL, &pos_fastesttitre.w, &pos_fastesttitre.h);
	SDL_RenderCopy(rendu, texturefast, NULL, &pos_fastesttitre);
	SDL_RenderPresent(rendu);
	SDL_DestroyTexture(texturefast);

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

void affichage_panda1(SDL_Renderer* rendu, Robot& panda1, int taille) {
	SDL_Surface* image1 = IMG_Load("PandaGris.png");

	if (!image1)
	{
		cout << "Erreur de chargement de l'image : %s", SDL_GetError();
		return;
	}

	SDL_Texture* monImage1 = SDL_CreateTextureFromSurface(rendu, image1);
	SDL_FreeSurface(image1);

	SDL_Rect posImg1;

	posImg1.y = 760;

	for (int i = 0; i < taille; i++) {
		if (panda1.position[i] == true) {
			posImg1.x = 62 * i + 63;
			SDL_QueryTexture(monImage1, NULL, NULL, &posImg1.w, &posImg1.h);
			SDL_RenderCopy(rendu, monImage1, NULL, &posImg1);
		}
	}

	SDL_RenderPresent(rendu);
}

void affichage_panda2(SDL_Renderer* rendu, Robot& panda1, int taille) {
	SDL_Surface* image1 = IMG_Load("PandaBleu.png");

	if (!image1)
	{
		cout << "Erreur de chargement de l'image : PandaBleu", SDL_GetError();
		return;
	}

	SDL_Texture* monImage1 = SDL_CreateTextureFromSurface(rendu, image1);
	SDL_FreeSurface(image1);

	SDL_Rect posImg1;

	posImg1.y = 760;

	for (int i = 0; i < taille; i++) {
		if (panda1.position[i] == true) {
			posImg1.x = 62 * i + 63;
			SDL_QueryTexture(monImage1, NULL, NULL, &posImg1.w, &posImg1.h);
			SDL_RenderCopy(rendu, monImage1, NULL, &posImg1);
		}
	}

	SDL_RenderPresent(rendu);
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
			rectangle_superieur.y = 715 - 5 * j;
			SDL_SetRenderDrawColor(rendu, 69, 224, 11, 255); // couleur verte
			SDL_RenderFillRect(rendu, &rectangle_superieur);
			rectangle_inferieur.y = rectangle_superieur.y + 1;
			SDL_SetRenderDrawColor(rendu, 151, 160, 9, 255); // couleur verte plus fonce
			SDL_RenderFillRect(rendu, &rectangle_inferieur);
		}
	}
	SDL_RenderPresent(rendu);
}


void Sauvegarder_Progression(Bambou jardin[], Statistique RecupStats[], Robot& panda1, Robot& panda2, int TAILLE, int TAILLE_STATS, int cpt_jour) {

	Sauvegarde_Stats_Graphique(RecupStats, TAILLE_STATS);
	Sauvegarde_Jardin_Jour_Robot(jardin, panda1, panda2, TAILLE, cpt_jour);
	cout << "Sauvegarde effectuee !" << endl;
}

void NePasSauvegarder() {
	cout << "Aucune sauvegarde effectuee !" << endl;
}

void NePasSauvegarder_EtRemove() {
	remove("jardin.txt");
	remove("stats.txt");
	cout << "Aucune sauvegarde et suppression des fichiers effectuee ! " << endl;
}



int main(int argc, char* argv[]) {

	// Déclaration tableau et constante
	const int TAILLE = 12;
	const int TAILLE_STATS = 1000;
	Bambou jardin[TAILLE];
	Statistique RecupStats[TAILLE_STATS];
	int cpt_jour = 0;

	// Initialisation des indices qui nous seront utiles après appels de fonctions.
	int indice_premier_plus_grand = 0, indice_deuxieme_plus_grand = 0;

	Robot panda1, panda2;

	ifstream entree("jardin.txt", ios::in);
	ifstream entree2("stats.txt", ios::in);
	if (!entree) {
		if (!entree2) {

			entree.close();
			entree2.close();

			InitTab(jardin, TAILLE);
			InitRobot(panda1);
			InitRobot(panda2);
		}
		else {
			cout << "Erreur, il manque un fichier de sauvegarde." << endl;
		}
	}
	else {

		entree.close();
		entree2.close();

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

				/*
				char choix;
				cout << "Entrez 's' pour sauvegarder la progression, 'n' pour fermer sans sauvegarder la progression, 'r' pour fermer sans sauvegarder la progression et supprimer les fichiers de sauvegarde" << endl;
				cin >> choix;

				if (choix == 's') {
					Sauvegarder_Progression(jardin, RecupStats, panda1, panda2, TAILLE, TAILLE_STATS, cpt_jour);
				}
				if (choix == 'n') {
					NePasSauvegarder();
				}
				if (choix == 'r') {
					NePasSauvegarder_EtRemove();
				}
				*/

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
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_r) { //touche r

				InitStats(RecupStats, TAILLE_STATS, cpt_jour, jardin, TAILLE, indice_premier_plus_grand, indice_deuxieme_plus_grand);
				afficheStats(RecupStats, cpt_jour, indice_premier_plus_grand, indice_deuxieme_plus_grand);

				ReduceMax(jardin, TAILLE, panda1, panda2);
				SDL_RenderClear(rendu);
				affiche(rendu, RecupStats, cpt_jour);

				affichage_panda1(rendu, panda1, TAILLE);
				affichage_panda2(rendu, panda2, TAILLE);
				bambous_tracer_pour_reducemax(rendu, jardin, TAILLE);
				croissance(jardin, TAILLE);


				afficheTab(jardin, TAILLE);
				afficheTab(jardin, TAILLE);
				cout << "Batterie panda1 : " << panda1.batterie << endl;
				cout << "Batterie panda2 : " << panda2.batterie << endl;


				cpt_jour++;

			}
			break;
		case SDL_MOUSEBUTTONUP://appui souris
			if (event.button.button == SDL_BUTTON_LEFT) {
				if (ActivStartMenu == true &&
					event.button.x > rectborduredroite.x &&
					event.button.x<rectborduredroite.x + rectborduredroite.w &&
					event.button.y>rectborduredroite.y &&
					event.button.y < rectborduredroite.y + rectborduredroite.h) {

					ActivChoixGauche = false;
					ActivStartMenu = false;
					ActivChoixDroite = true;

					affiche(rendu, RecupStats, cpt_jour);
				}
				SDL_RenderPresent(rendu);//on rafraichit
				if (ActivStartMenu == true &&
					event.button.x > rectborduregauche.x &&
					event.button.x<rectborduregauche.x + rectborduregauche.w &&
					event.button.y>rectborduregauche.y &&
					event.button.y < rectborduregauche.y + rectborduregauche.h) {
					ActivChoixGauche = true;
					ActivStartMenu = false;
					ActivChoixDroite = true;
					affiche(rendu, RecupStats, cpt_jour);
					bambous_tracer_pour_reducemax(rendu, jardin, TAILLE);
				}
				SDL_RenderPresent(rendu);//on rafraichit
				if (event.button.x > returnmenu_button.x &&
					event.button.x<returnmenu_button.x + returnmenu_button.w &&
					event.button.y>returnmenu_button.y &&
					event.button.y < returnmenu_button.y + returnmenu_button.h) {
					start_choice(rendu);
				}
				SDL_RenderPresent(rendu);//on rafraichit
				if (ActivChoixGauche == true &&
					event.button.x > RectChoixSaveGauche.x &&
					event.button.x<RectChoixSaveGauche.x + RectChoixSaveGauche.w &&
					event.button.y>RectChoixSaveGauche.y &&
					event.button.y < RectChoixSaveGauche.y + RectChoixSaveGauche.h) {
					//fonction pour button Gauche
					Sauvegarder_Progression(jardin, RecupStats, panda1, panda2, TAILLE, TAILLE_STATS, cpt_jour);
				}
				SDL_RenderPresent(rendu);//on rafraichit
				if (ActivChoixGauche == true &&
					event.button.x > RectChoixSaveMillieu.x &&
					event.button.x<RectChoixSaveMillieu.x + RectChoixSaveMillieu.w &&
					event.button.y>RectChoixSaveMillieu.y &&
					event.button.y < RectChoixSaveMillieu.y + RectChoixSaveMillieu.h) {
					//fonction pour button Millieu
					NePasSauvegarder();
					ifstream entree("jardin.txt", ios::in);
					ifstream entree2("stats.txt", ios::in);
					if (!entree) {
						if (!entree2) {

							entree.close();
							entree2.close();

							InitTab(jardin, TAILLE);
							InitRobot(panda1);
							InitRobot(panda2);
						}
						else {
							cout << "Erreur, il manque un fichier de sauvegarde." << endl;
						}
					}
					else {

						entree.close();
						entree2.close();

						Recharge_Sauvegarde_Stats_Graphique(RecupStats, TAILLE_STATS);
						Recharge_Sauvegarde_Jardin_Jour_Robot(jardin, panda1, panda2, TAILLE, cpt_jour);
					}
					start_choice(rendu);


				}
				SDL_RenderPresent(rendu);//on rafraichit
				if (ActivChoixGauche == true &&
					event.button.x > RectChoixSaveDroite.x &&
					event.button.x<RectChoixSaveDroite.x + RectChoixSaveDroite.w &&
					event.button.y>RectChoixSaveDroite.y &&
					event.button.y < RectChoixSaveDroite.y + RectChoixSaveDroite.h) {
					//fonction pour button Droite
					NePasSauvegarder_EtRemove();
					InitTab(jardin, TAILLE);
					InitRobot(panda1);
					InitRobot(panda2);
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