#include <stdio.h>
#include <gmp.h>
#include "square_multiply.h"

#define KEY_SIZE (size_t) mpz_sizeinbase(exp, 2) //nombre de bits de e (exposant publique) ou de d (exposant privée)

/* Exponentiation rapide (pour chiffrer et déchiffrer)
* x = m (message) / c (chiffré)
* exp = e (clé publique) / d (clé privée)
* n = p * q
* y = c (chiffré) / m (message)
*/
void square_and_multiply(const mpz_t x, const mpz_t exp, const mpz_t n, mpz_t y)
{
    size_t i;
    mpz_t msk, rshiftr, andr;
    
    mpz_init(msk);      //masque de ...0001 en binaire
    mpz_init(rshiftr);  //résultat d'un décallage à droite
    mpz_init(andr);     //résultat d'un AND
    mpz_set_ui(msk, 1);
    mpz_set_ui(y, 1);

    for (i = KEY_SIZE; i > 0; i--)
    {
        //square
        mpz_mul(y, y, y); mpz_mod(y, y, n);
        
        mpz_tdiv_q_2exp(rshiftr, exp, i - 1);   //recupération bit de poids faible de exp
        mpz_and(andr, rshiftr, msk);            //récupération type du bit de exp

        //vérification bit de l'exposant exp
        if(!(mpz_cmp_ui(andr, 1)))
        {   
            //multiply
            mpz_mul(y, y, x); mpz_mod(y, y, n);
        }
    }

    mpz_clear(msk);
    mpz_clear(rshiftr);
    mpz_clear(andr);
}
