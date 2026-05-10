#ifndef __MILLER_RABIN__
#define __MILLER_RABIN__
#include <gmp.h>
#include <stdbool.h>

void generer_un_nombre_premier(mpz_t p, int b);
bool miller_rabin(const mpz_t n);

#endif /*__MILLER_RABIN__*/
