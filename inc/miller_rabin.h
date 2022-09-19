#ifndef __MILLER_RABIN__
#define __MILLER_RABIN__
#include <gmp.h>
#include <stdbool.h>

void generer_un_nombre_premier(mpz_t p, int b);
bool miller_rabin(const mpz_t n);
void random_bit(char** random_number, double bit_value, double limit_value);

#endif /*__MILLER_RABIN__*/
