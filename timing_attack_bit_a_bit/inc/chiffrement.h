#ifndef __CHIFFREMENT__
#define __CHIFFREMENT__
#include <gmp.h>
#include <stdbool.h>

bool padding;

void chiffrement_RSA(mpz_t m, const mpz_t  e, const mpz_t n, mpz_t c);
void chiffrement_RSA_montgomery(mpz_t m, const mpz_t e, const mpz_t n, mpz_t c, const mpz_t v, const unsigned int N_SIZE);
void padding_signature(mpz_t m);
void hash(mpz_t hm);	//SHA256
void signature(mpz_t m, const mpz_t d, const mpz_t n, mpz_t s);
void generer_R_montgomery(mpz_t r);
void bezout(const mpz_t r, const mpz_t n, mpz_t u, mpz_t v, mpz_t pgcd);
void verification(const mpz_t u, const mpz_t z, const mpz_t n, mpz_t verif);
void Montgomery_product(const mpz_t v, const mpz_t a_bar, const mpz_t b_bar,  const mpz_t n, mpz_t res, const unsigned int N_SIZE);
void Montgomery_Exponentiation_crypt(mpz_t crypt, const mpz_t a, const mpz_t v, const mpz_t e, const mpz_t n, const unsigned int N_SIZE);

#endif /*__CHIFFREMENT__*/
