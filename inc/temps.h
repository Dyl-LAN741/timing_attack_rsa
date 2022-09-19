#ifndef _TEMPS_H_
#define _TEMPS_H_
#define _POSIX_C_SOURCE 199309L
#include <sys/time.h>
#include <time.h>
#define _GNU_SOURCE	//fix warning implicit declaration

struct timespec;	//fix warning scope

void temps_reel(struct timespec *tr);
void temps_cpu(clock_t *tc);
void afficher_temps_reel(double *ttr);
void afficher_temps_cpu(double *ttc);
void debut_chrono(clock_t *tdc, struct timespec *tdr);
void fin_chrono(double *ttc, clock_t tdc, clock_t tfc, double *ttr, struct timespec tdr, struct timespec tfr);
void debut_chrono_timing_attack(struct timespec *tda);
void fin_chrono_timing_attack(double *tta, struct timespec tda, struct timespec tfa);

#endif /* _TEMPS_H_ */
