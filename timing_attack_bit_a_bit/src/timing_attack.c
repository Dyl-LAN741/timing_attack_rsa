#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <gmp.h>
#include "temps.h"
#include "timing_attack.h"
#include "creation_des_cles.h"

//Initialisation des variables globales par défaut
ENSEMBLE* A = NULL;
ENSEMBLE* B = NULL;
TAB* T = NULL;
unsigned int target_bit = 0;
unsigned int position_bit = 0;
unsigned int TIMING_ATTACK_CONFIRMED = 0;
unsigned int DECRYPT = 0;
unsigned int arret = 1;
unsigned int TEMPS_REDUC = 0;

//Initialise les variables globales pour le timing attack
void initialiser_variables_globales_timing_attack()
{
    A = initialiser_ensemble();
    B = initialiser_ensemble();
    T = initialiser_tableau();
}

ELEMENT* initialiser_element(const double temps)
{
	ELEMENT *elem = malloc(sizeof(ELEMENT));

	if(!elem)
	{
		fprintf(stderr, "Erreur lors de l'allocation de l'element.\n");
		exit(10);
	}

	elem->temps = temps;
	elem->suiv = NULL;
	
	return elem;
}

void afficher_element(const ELEMENT* elem, const char* nom)
{
	if(!elem)
	{
		printf("\t\tL'element %s est vide.\n\n", nom);
		return;
	}

	printf("\t\tL'element %s est %.9f\n\n", nom, elem->temps);
}

LISTE* initialiser_liste()
{
	LISTE* liste = malloc(sizeof(LISTE));
	
	if(!liste)
	{
		fprintf(stderr, "Erreur lors de l'allocation de la liste.\n");
		exit(11);
	}

	liste->elem = NULL;
	liste->fin = liste->elem;
	liste->taille = 0;
	liste->temps_moyen = 0.0;
	liste->temps_total = 0.0;
	liste->difference_temps_moyen = 0.0;

	return liste;
}

//vérifie si une liste existe (≠ NULL)
static bool verification_liste_non_null(const LISTE* liste)
{
	if(!liste)
		return 0;
	else
		return 1;
}

void afficher_liste_complete(const LISTE* liste, const char* nom)
{	
	if(verification_liste_non_null(liste))
	{
		printf("\t\t%s", nom);
		
		if(!liste->elem)
		{
			printf(" (taille 0) (temps moyen 0) (temps total 0) :\n\n");
			printf("\t\t   vide\n\n");
			return;
		}
		
		unsigned long int i = 1;
		printf(" (taille %lu) (temps moyen %f) (temps total %f) :\n\n", liste->taille, liste->temps_moyen, liste->temps_total);
		printf("\t\t   [%lu]: %f\n", i, liste->elem->temps);
		ELEMENT* e = liste->elem->suiv;
		while(e)
		{
			i++;
			printf("\t\t   [%lu]: %f\n", i, e->temps);
			e = e->suiv;
		}
		printf("\n");
		return;
	}
	fprintf(stderr,"\t\tLa liste %s n'existe pas.\n\n", nom);
}

void afficher_liste_simple(const LISTE* liste, const char* nom)
{	
	if(verification_liste_non_null(liste))
	{
		printf("%s", nom);
		
		if(!liste->elem)
		{
			printf("\n\ttaille : 0\n\ttemps moyen : 0\n\ttemps total : 0\n\n");
			return;
		}
		
		printf("\n\ttaille : %lu\n\ttemps moyen %.9f\n\ttemps total : %.6f\n\n", liste->taille, liste->temps_moyen, liste->temps_total);
		return;
	}
	fprintf(stderr,"\t\tLa liste %s n'existe pas.\n\n", nom);
}

void ajouter_element_liste(ELEMENT* elem, LISTE** liste)
{
	if(verification_liste_non_null(*liste))
	{
		if((*liste)->taille == 0)
		{
			(*liste)->elem = elem;
			(*liste)->fin = (*liste)->elem;
		} else {
			(*liste)->fin->suiv = elem;
			(*liste)->fin = elem;
		}
		(*liste)->taille++;
		(*liste)->temps_total += elem->temps; 
	}
}

//retourne le premier élément de la liste
ELEMENT* retourner_element_liste(LISTE** liste)	
{
	if(verification_liste_non_null(*liste))
	{
		if((*liste)->elem == NULL)
			return NULL;
		
		ELEMENT* premier = initialiser_element(0.0);
		premier = (*liste)->elem;

		ELEMENT* elem = initialiser_element(premier->temps);
		(*liste)->elem = (*liste)->elem->suiv;
		
		premier->suiv = NULL;
		(*liste)->taille--;
		free(premier);

		return elem;
	}
	return NULL;
}

void calculer_temps_moyen_liste(LISTE** liste)
{
	if(verification_liste_non_null(*liste))
	{
		(*liste)->temps_moyen = (*liste)->temps_total / (*liste)->taille;
	}
}

void supprimer_liste(LISTE** liste, const char* nom)
{
	if(verification_liste_non_null(*liste))
	{
		if(!(*liste)->elem)
		{
			*liste = NULL;
			free((*liste)->elem);
			free(*liste);
			printf("\t\tLa liste %s a ete supprimee.\n\n", nom);
			return;
		}

		ELEMENT* e = (*liste)->elem->suiv;
		while(e)
		{
			(*liste)->elem->suiv = NULL;
			(*liste)->elem = NULL;
			free((*liste)->elem);
			(*liste)->taille--;
			//printf("\t\tL'element %lu a ete supprime.\n", (*liste)->taille + 1);
			(*liste)->elem = e;
			e = e->suiv;
		}
		(*liste)->elem = NULL;
		free((*liste)->elem);
		(*liste)->taille--;
		//printf("\t\tL'element %lu a ete supprime.\n", (*liste)->taille + 1);
		*liste = NULL;
	}
	free(*liste);
	printf("\t\tLa liste %s a ete supprimee.\n\n", nom);
}

ENSEMBLE* initialiser_ensemble()
{
	unsigned int i = 0;

	ENSEMBLE* ens = malloc(sizeof(ENSEMBLE));	
	
	if(!ens)
	{
		fprintf(stderr, "Erreur lors de l'allocation de l'ensemble.\n");
		exit(12);
	}

	ens->bit = malloc(sizeof(LISTE) * d_size - 1);
	
	if(!ens->bit)
	{
		fprintf(stderr, "Erreur lors de l'allocation de l'ensemble.\n");
		exit(13);
	}

	for(i = 0; i < d_size - 1; i++)
	{
		ens->bit[i] = initialiser_liste();
	}

	return ens;
}

//vérifie si l'ensemble existe (≠ NULL)
static bool verification_ensemble_non_null(ENSEMBLE* ens)
{
	if(!ens)
	{
		return 0;
	}

	return 1;
}

//affiche l'ensemble et tous ses éléments
void afficher_ensemble_complet(ENSEMBLE* ens, const char* nom)
{
	unsigned int i = 0;
	if(verification_ensemble_non_null(ens))
	{
		for(i = 0; i < d_size - 1; i++)
		{
			if(verification_liste_non_null(ens->bit[i]))
			{
				char str_bit[255];
				char nom_liste[9] = "bit "; 
				char* nom_ensemble = malloc(sizeof(char) * strlen(nom) + sizeof(char) * 2 + sizeof(char) * (int) (log10(i+1) + 1) + 1);

				nom_ensemble[0] = '\0';
				sprintf(str_bit, "%d", i);
				
				strncat(nom_ensemble, nom, strlen(nom));
				strncat(nom_ensemble, "[", 1);
				strncat(nom_ensemble, str_bit, strlen(str_bit));
				strncat(nom_ensemble, "]", 1);
				printf("%s\n", nom_ensemble);
				
				sprintf(str_bit, "%d", i+1);
				strncat(nom_liste, str_bit, strlen(str_bit));
				afficher_liste_complete(ens->bit[i], nom_liste);
			}
		}
	}
}

//affiche les variables de l'ensemble
void afficher_ensemble_simple(ENSEMBLE* ens, const char* nom)
{
	unsigned int i = 0;
	if(verification_ensemble_non_null(ens))
	{
		for(i = 0; i < d_size - 1; i++)
		{
			if(verification_liste_non_null(ens->bit[i]))
			{
				char str_bit[255];
				char nom_liste[9] = "bit "; 
				char* nom_ensemble = malloc(sizeof(char) * strlen(nom) + sizeof(char) * 5 + sizeof(char) * 9 + sizeof(char) * (int) (log10(i+1) + 1) + 1);

				nom_ensemble[0] = '\0';
				sprintf(str_bit, "%d", i);
				
				strncat(nom_ensemble, nom, strlen(nom));
				strncat(nom_ensemble, "[", 1);
				strncat(nom_ensemble, str_bit, strlen(str_bit));
				strncat(nom_ensemble, "]", 1);
				strncat(nom_ensemble, " : ", 3);

				sprintf(str_bit, "%d", i+1);
				strncat(nom_liste, str_bit, strlen(str_bit));

				strncat(nom_ensemble, nom_liste, strlen(nom_liste));
				printf("%s\n", nom_ensemble);

				afficher_liste_simple(ens->bit[i], "");
			}
		}
	}
}

void ajouter_element(ELEMENT* elem, ENSEMBLE** ens, const unsigned int i)
{
	if(verification_ensemble_non_null(*ens))
	{
		ajouter_element_liste(elem, &((*ens)->bit[i]));
	}
}

// retourne le premier élément de la liste à l'indice du tableau i
ELEMENT* retourner_element(ENSEMBLE** ens, const unsigned int i)
{
	if(verification_ensemble_non_null(*ens))
	{
		ELEMENT* e = initialiser_element(0.0);
		e = retourner_element_liste(&(*ens)->bit[i]);
		return e;
	}

	fprintf(stderr,"Erreur : L'ensemble n'existe pas.\n");

	return NULL;
}

void calculer_temps_moyen(ENSEMBLE** ens, unsigned int i)
{
	if(verification_ensemble_non_null(*ens))
	{

		calculer_temps_moyen_liste(&(*ens)->bit[i]);
	}
}

static inline void valeur_absolue_double(double* valeur)
{
	if(*valeur < 0)
		*valeur *= -1; 
}

void calculer_difference_temps_moyen(ENSEMBLE** a, ENSEMBLE** b, unsigned int i)
{
	double difference, delta;
	if(verification_ensemble_non_null(*a) && verification_ensemble_non_null(*b))
	{
		if((*a)->bit[i]->taille != 0 && (*b)->bit[i]->taille != 0)	//si on a mesuré un temps dans A et B
		{
			//Tb - Ta
			difference = (*b)->bit[i]->temps_moyen - (*a)->bit[i]->temps_moyen;
			//|Tb - Ta|
			valeur_absolue_double(&difference);

			(*a)->bit[i]->difference_temps_moyen = difference;
			(*b)->bit[i]->difference_temps_moyen = difference;

			//β - α
			delta = (*b)->bit[i]->temps_total - (*a)->bit[i]->temps_total;
			//|β - α|
			valeur_absolue_double(&delta);

			if(difference <= delta)	//si |Tb - Ta| converge vers |β - α|
			{
				T->difference[i] = difference;
				T->bit_value[i] = 1;	//bit i = 1
				arret = 0;	//vrai
			}
		}
	}
}

void supprimer_ensemble(ENSEMBLE** ens, const char* nom)
{
	if(verification_ensemble_non_null(*ens))
	{
		unsigned int i = 0;
		
		for(i = 0; i > d_size - 1; i++)
		{
			supprimer_liste(&(*ens)->bit[i], nom);
		}
		*ens = NULL;
	}
	free(*ens);
	//printf("\t\tL'ensemble %s a ete supprimee.\n\n", nom);
}

TAB* initialiser_tableau()
{
	unsigned int i;
	TAB* tab = malloc(sizeof(TAB));	
	
	if(!tab)
	{
		fprintf(stderr, "Erreur lors de l'allocation de la map.\n");
		exit(14);
	}

	tab->difference = malloc(sizeof(double) * n_size);

	if(!tab->difference)
	{
		fprintf(stderr, "Erreur lors de l'allocation de la map.\n");
		exit(15);
	}

	tab->bit_value = malloc(sizeof(unsigned int) * n_size);

	if(!tab->bit_value)
	{
		fprintf(stderr, "Erreur lors de l'allocation de la map.\n");
		exit(16);
	}

	for(i = 0; i < d_size - 1; i++)
	{
		tab->difference[i] = 0.0;
		tab->bit_value[i] = 0.0;
	}
	tab->difference[i] = -1;	//-1 car la différence ne sera jamais calculé
	tab->bit_value[i] = 1;		//bit connu dk-1 = 1
	
	return tab;
}

void supprimer_tableau(TAB** tab)
{
	if(*tab && (*tab)->bit_value && (*tab)->bit_value)
	{	
		free((*tab)->difference);
		free((*tab)->bit_value);
		*tab = NULL;
	}
	free(*tab);
	//printf("\t\tLe tableau a ete supprimee.\n\n");
}

void afficher_tableau_T()
{
	unsigned int i;
	printf("T :\n");
	for(i = 0; i < d_size; i++)
	{
		printf("\tbit %d\n\tdifference : %.9f\n\tvaleur du bit : %u\n\n", i+1, T->difference[i], T->bit_value[i]);
	}
}

void reconstituer_d(mpz_t resultat)
{
	unsigned int i;
	char* str_d = malloc(sizeof(char) * d_size + 1);
	mpz_t d_secret;
	mpz_init(d_secret);
	
	str_d[0] = '\0';
	strncat(str_d, "1", 1);		//dk-1 = 1
	for(i = d_size - 2; i > 0; i--)	//pour dk - i avec 1 < i < k-1
	{
		if(T->bit_value[i] == 0)
		{
			strncat(str_d, "0", 1);

		} else if(T->bit_value[i] == 1)
		{
			strncat(str_d, "1", 1);
		}
	}
	strncat(str_d, "1", 1);		//d0 = 1
	mpz_set_str(d_secret, str_d, 2);

	mpz_set(resultat, d_secret);	//sauve garde la clé secrète

	free(str_d);
	mpz_clear(d_secret);
}
