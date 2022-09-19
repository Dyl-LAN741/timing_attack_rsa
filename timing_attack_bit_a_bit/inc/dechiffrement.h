#ifndef __DECHIFFREMENT__
#define __DECHIFFREMENT__
#include <gmp.h>

void dechiffrement_RSA(const mpz_t c, const mpz_t  d, const mpz_t n, mpz_t m);
void dechiffrement_RSA_montgomery(const mpz_t c, const mpz_t d, const mpz_t n, mpz_t m, const mpz_t v, const unsigned int N_SIZE);
void verification_signature(mpz_t s, const mpz_t e, const mpz_t n, const mpz_t hm);


void Montgomery_Exponentiation_decrypt(mpz_t decrypt, const mpz_t a, const mpz_t v, const mpz_t d , const mpz_t n, const unsigned int N_SIZE);

#endif /*__DECHIFFREMENT__*/
