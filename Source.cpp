#include <iostream>
#include <fstream>
#include <SDL.h>
#include <SDL_ttf.h>
#include "config_sdl.h"
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


void ReduceMax(Bambou tab[], int taille, Robot& panda1, Robot& panda2) {
	int indice_premier = 0, indice_deuxieme = 0;
	TailleMax(tab, taille, indice_premier, indice_deuxieme);

	deplacement(panda1, panda2, indice_premier, indice_deuxieme);

	batterie_et_decoupe(tab, panda1, panda2, indice_premier, indice_deuxieme);
}


void ReduceFast(Bambou tab[], int taille, Robot& panda1, Robot& panda2) {

	int somme_croissance_bambou = 0;

	for (int i = 0; i < taille; i++) {
		somme_croissance_bambou += tab[i].croissance;
	}

	float x = 1 + sqrt(5);
	float taille_minimale = x * somme_croissance_bambou;

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
	if (indice_croissance1 != 0) {
		Bambou tmp = tab[indice_croissance1];
		tab[indice_croissance1] = tab[0];
		tab[0] = tmp;
	}

	max2 = tab[1].croissance;
	indice_croissance2 = 1;

	for (int i = 1; i < taille; i++) {
		if (tab[i].taille > taille_minimale && max2 <= tab[i].croissance) {
			max2 = tab[i].croissance;
			indice_croissance2 = i;
		}
	}

	if (indice_croissance1 != 0) {
		Bambou tmp = tab[indice_croissance1];
		tab[indice_croissance1] = tab[0];
		tab[0] = tmp;
	}

	if (tab[indice_croissance1].taille > taille_minimale && tab[indice_croissance2].taille > taille_minimale) {
		deplacement(panda1, panda2, indice_croissance1, indice_croissance2);
		batterie_et_decoupe(tab, panda1, panda2, indice_croissance1, indice_croissance2);
	}
}

void Sauvegarde_Jardin_Jour_Robot(Bambou tab[], Robot& panda1, Robot& panda2 ,int taille, int cpt_jour) {
	ofstream sortie("jardin.txt", ios::trunc);

	for (int i = 0; i < taille; i++){
		sortie << tab[i].num <<',';
		sortie << tab[i].taille <<',';
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

int main(int argc, char* argv[]) {

	// Déclaration tableau et constante
	const int TAILLE = 12;
	const int TAILLE_STATS = 101;
	Bambou jardin[TAILLE];
	Statistique RecupStats[TAILLE_STATS];
	int cpt_jour = 0;
	int cpt_for_stats = 0;

	// Initialisation des indices qui nous seront utiles après appels de fonctions.
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
	char continuer = ' ', mode;

	cout << "Choix mode : 'f' pour Fast, 'm' pour Max." << endl;
	cin >> mode;

	if (mode == 'm') {
		while (simulation) {
		cout << "Entrez 'r' pour relancer un jour, 'q' pour quitter." << endl;
		cin >> continuer;

		if (continuer == 'q') {
			cout << "Fin." << endl;
			simulation = false;

			Sauvegarde_Stats_Graphique(RecupStats, TAILLE_STATS);
			Sauvegarde_Jardin_Jour_Robot(jardin, panda1, panda2, TAILLE, cpt_jour);
		}

		else if (continuer == 'r') {

			InitStats(RecupStats, TAILLE_STATS, cpt_jour, jardin, TAILLE, indice_premier_plus_grand, indice_deuxieme_plus_grand);
			afficheStats(RecupStats, cpt_jour, indice_premier_plus_grand, indice_deuxieme_plus_grand);

			ReduceMax(jardin, TAILLE, panda1, panda2);
			afficheTab(jardin, TAILLE);
			croissance(jardin, TAILLE);
			afficheTab(jardin, TAILLE);
			cout << "Batterie panda1 : " << panda1.batterie << endl;
			cout << "Batterie panda2 : " << panda2.batterie << endl;

			cpt_jour++;
		}
		cout << endl;
		}
	}

	else if (mode == 'f') {
		while (simulation) {
			cout << "Entrez 'r' pour relancer un jour, 'q' pour quitter." << endl;
			cin >> continuer;

			if (continuer == 'q') {
				cout << "Fin." << endl;
				simulation = false;

				Sauvegarde_Stats_Graphique(RecupStats, TAILLE_STATS);
				Sauvegarde_Jardin_Jour_Robot(jardin, panda1, panda2, TAILLE, cpt_jour);
			}

			else if (continuer == 'r') {

				InitStats(RecupStats, TAILLE_STATS, cpt_jour, jardin, TAILLE, indice_premier_plus_grand, indice_deuxieme_plus_grand);
				afficheStats(RecupStats, cpt_jour, indice_premier_plus_grand, indice_deuxieme_plus_grand);

				ReduceFast(jardin, TAILLE, panda1, panda2);
				afficheTab(jardin, TAILLE);
				croissance(jardin, TAILLE);
				afficheTab(jardin, TAILLE);
				cout << "Batterie panda1 : " << panda1.batterie << endl;
				cout << "Batterie panda2 : " << panda2.batterie << endl;

				cpt_jour++;
			}
			cout << endl;
		}
	}


	return 0;
}