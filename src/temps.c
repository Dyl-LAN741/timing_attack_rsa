#include "temps.h"
#include <stdio.h> 
#include <sys/time.h>
#include <time.h>

/*
*	Fonction qui permet de calculer le temps réel.
*/
void temps_reel(struct timespec *temps_reel_t) {
	clock_gettime(CLOCK_MONOTONIC, temps_reel_t);
}

/*
*	Fonction qui permet de calculer le temps CPU.
*/
void temps_cpu(clock_t *temps_cpu_t) {
	*temps_cpu_t = clock();
}

/*
*	Fonction qui permet afficher le temps réel écoulé depuis le début de l'exécution.
*/
void afficher_temps_reel(double *temps_reel) {
	printf("temps réel = %.2f secondes\n",*temps_reel);
}

/*
*	Fonction qui permet afficher le temps CPU écoulé depuis le début de l'exécution.
*/
void afficher_temps_cpu(double *temps_cpu) {
	printf("temps CPU = %.12f secondes\n",*temps_cpu);
}

/*
*	Fonction qui permet lancer le début du chrono.
*/
void debut_chrono(clock_t *tc, struct timespec *tr) {
	temps_cpu(tc);
	temps_reel(tr);
}

/*
*	Fonction qui permet lancer la fin du chrono.
*/
void fin_chrono(double *ttc, clock_t tdc, clock_t tfc, double *ttr, struct timespec tdr, struct timespec tfr) {
	temps_cpu(&tfc);
	temps_reel(&tfr);
	*ttc = (tfc - tdc) * 1e-6;
	*ttr= (tfr.tv_sec - tdr.tv_sec);
}

/*
*	Fonction qui permet lancer le début du chrono pour le timing attack.
*/
void debut_chrono_timing_attack(struct timespec *tda) {
	temps_reel(tda);
}

/*
*	Fonction qui permet lancer la fin du chrono timing attack.
*/
void fin_chrono_timing_attack(double *tta, struct timespec tda, struct timespec tfa) {
	temps_reel(&tfa);
	*tta= (tfa.tv_sec - tda.tv_sec);
}


