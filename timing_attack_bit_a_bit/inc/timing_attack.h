#ifndef _TIMING_ATTACK_H_
#define _TIMING_ATTACK_H_
#include <gmp.h>

typedef struct element {
	double temps;
	struct element* suiv;
} ELEMENT; 				//représente un élément de l'ensemble A ou B.

typedef struct liste {
	ELEMENT* elem;
	ELEMENT* fin;
	unsigned long int taille;
	double temps_moyen;
	double temps_total;
	double difference_temps_moyen;
} LISTE; 				//représente la liste des mesures (pour un bit) sous la forme d'une simple liste chaînée.

typedef struct ensemble {
	LISTE** bit;
} ENSEMBLE;				//représente l'ensemble A et B (pour tous les bits) sous la forme d'un tableau de listes chaînées.
						//	- tableau de taille n
						//	- liste de taille >= nombre d'itération sur le bit i
						//	- l'indice du tableau représente le bit
						//	- la liste représente les temps mesurés

typedef struct tab {
	double* difference;
	unsigned int* bit_value;
} TAB;					//représente une map des différences des temps moyens entre A et B et la valeur du bit correspondante.

//Fonctions pour les éléments
ELEMENT* initialiser_element(const double temps);
void afficher_element(const ELEMENT* elem, const char* nom);

//Fonctions pour les listes
LISTE* initialiser_lsite();
void ajouter_element_liste(ELEMENT* elem, LISTE** ens);
ELEMENT* retourner_element_liste(LISTE** ens);
void afficher_liste_complete(const LISTE* ens, const char* nom);
void afficher_liste_simple(const LISTE* ens, const char* nom);
void supprimer_liste(LISTE** ens, const char* nom);
void calculer_temps_moyen_liste(LISTE** liste);

//Fonctions pour les ensembles
ENSEMBLE* initialiser_ensemble();
void ajouter_element(ELEMENT *elem, ENSEMBLE** eg, const unsigned int bit);
ELEMENT* retourner_element(ENSEMBLE** eg, const unsigned int bit);
void supprimer_ensemble(ENSEMBLE** eg, const char* nom);
void afficher_ensemble_complet(ENSEMBLE* eg, const char* nom);
void afficher_ensemble_simple(ENSEMBLE* eg, const char* nom);
void calculer_temps_moyen(ENSEMBLE** ens, unsigned int i);
void calculer_difference_temps_moyen(ENSEMBLE** a, ENSEMBLE** b, unsigned int i);	//calcul la différence des temps moyens entre a et b

//Fonctions pour le tableau
TAB* initialiser_tableau();
void supprimer_tableau(TAB** tab);
void afficher_tableau_T();

//Variables globales
extern ENSEMBLE* A;
extern ENSEMBLE* B;
extern TAB* T;
extern unsigned int position_bit;				//la position du bit courant
extern unsigned int target_bit;					//le bit mesuré lors du timing attack
extern unsigned int TIMING_ATTACK_CONFIRMED;	//0 = désactive le timing attack, 1 = active le timing attack
extern unsigned int DECRYPT;					//0 = déchiffrement non effectué, 1 = déchiffrement effectué 
extern unsigned int arret;						//1 = faux, 0 vrai
extern unsigned int TEMPS_REDUC;				//temps de la reduction de mongomery en nanosecondes

//Fonctions pour les variables globales
void initialiser_variables_globales_timing_attack();

//Fonctions générales
void reconstituer_d(mpz_t resultat);			//reconstitue la clé secrète d à l'aide du tableau T

#endif /* _TIMING_ATTACK_H_ */
