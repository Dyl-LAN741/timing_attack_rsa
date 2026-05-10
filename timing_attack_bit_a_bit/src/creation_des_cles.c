#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include "miller_rabin.h"
#include "creation_des_cles.h"
#include "logs.h"

//nombre de bits qui seront utilisés pour p et q
#define PRIME_NUMBER_SIZE prime_size

//Initialisation variables globales
unsigned int prime_size = 0;
unsigned int n_size = 0;
unsigned int d_size = 0;

void generer_npq(mpz_t n, mpz_t p, mpz_t q)
{
    int difference = -1;
    unsigned int p_size, q_size;
    mpz_t verif_p, verif_q, rop, msk, rop2;
    
    mpz_init(verif_p);
    mpz_init(verif_q);
    mpz_init(n);
    mpz_init(p);
    mpz_init(q);
    mpz_init(rop);
    mpz_init(msk);
    mpz_init(rop2);
    mpz_set_ui(msk, 1);
    mpz_set_ui(rop2, 2);

    //génère un nombre premier alétoire de k bits avec le test de primalité de Miller-Rabin
    generer_un_nombre_premier(p, PRIME_NUMBER_SIZE); 
    generer_un_nombre_premier(q, PRIME_NUMBER_SIZE);

    p_size = (unsigned int) mpz_sizeinbase(p, 2);
    q_size = (unsigned int) mpz_sizeinbase(q, 2);
    fprintf(logs, "taille de p : %u\ttaille de q: %u\n", p_size, q_size);

    mpz_mod_ui(verif_p, p, 65537);     //p mod e
    mpz_mod_ui(verif_q, q, 65537);     //q mod e

    //si p ≠ q
    if(mpz_cmp(p, q))
    {
        //2^51
        mpz_mul_2exp(rop, msk, 51);
        //2^0.5 = racine carré de 2
        mpz_sqrt(rop2, rop2);
        //2^51 * 2^0.5 = 2^51.5
        mpz_mul_2exp(rop, rop, mpz_get_ui(rop2));
        //2^51.5 * 2^𝑘/4
        mpz_mul_2exp(rop, rop, ((PRIME_NUMBER_SIZE * 2) / 4 ));
        //p - q
        mpz_sub (rop2, p, q);
        //|p - q|
        mpz_abs(rop2, rop2);
        //p et q sont bon si |p - q| ≥ 2^51.5 * 2^𝑘/4
        difference = mpz_cmp(rop2, rop);    //pour éviter la factorisation de n par la méthode de Fermat
    }

    //si (p ≠ q) ET (p mod e ≠ 1) ET (q mod e ≠ 1) ET (|p - q| ≥ 2^51.5 * 2^𝑘/4)
    if(difference >= 0 && mpz_cmp(p, q) && mpz_cmp_ui(verif_p, 1) && mpz_cmp_ui(verif_q, 1))
    {
        mpz_mul(n, p, q);   //n = p * q
        n_size = (unsigned int) mpz_sizeinbase(n, 2);   //taille de n
        
        mpz_clear(verif_p);
        mpz_clear(verif_q);
        mpz_clear(rop);
        mpz_clear(msk);
        mpz_clear(rop2);

        return;
    } else {
        generer_npq(n, p, q);

        mpz_clear(verif_p);
        mpz_clear(verif_q);
        mpz_clear(rop);
        mpz_clear(msk);
        mpz_clear(rop2);
        
        return;
    }
}

/* Pour un e quelconque, choisir :
* - 1 < e < phi(n)
* - PGCD(e, φ(n)) = 1 (e premier avec phi(n))
*/ 
void generer_exposant_public(mpz_t e)
{
    mpz_set_ui(e, 65537);   //valeur standard pour RSA
}

void phi(const mpz_t p, const mpz_t q, mpz_t phi)
{
    mpz_t tmp_p, tmp_q;
    mpz_init(tmp_p);
    mpz_init(tmp_q);

    //p - 1
    mpz_sub_ui(tmp_p, p, 1);
    //q - 1
    mpz_sub_ui(tmp_q, q, 1);
    //phi(n) = (p - 1) * (q - 1)
    mpz_mul(phi, tmp_p, tmp_q);

    mpz_clear(tmp_p);
    mpz_clear(tmp_q);
}


void generer_exposant_privee(const mpz_t e, const mpz_t phi_n, mpz_t d)
{
    mpz_t pgcd_r;
    mpz_init(pgcd_r);

    //si l'inverse modulaire n'existe pas on arrête le programme
    if(!(mpz_invert(d, e, phi_n)))
    {
        fprintf(logs, "Erreur: l'inverse modulaire de l'exposant public n'existe pas.\n");
        mpz_clear(pgcd_r);
        
        exit(5);
    } else {    
        mpz_gcd(pgcd_r, e, d);  
        
        //si le PGCD(e,d) ≠ 1 on arrête le programme
        if(mpz_cmp_ui(pgcd_r, 1))
        {
            fprintf(logs, "Erreur: l'inverse modulaire de l'exposant public n'est pas premier avec l'exposant privé d.\n");
            mpz_clear(pgcd_r);
            
            exit(6);
        }

        d_size = (unsigned int) mpz_sizeinbase(d, 2);   //taille de d
        //printf("d_size : %u", d_size);
    }
}

void affichage_binaire_mpz(const mpz_t d)
{
    unsigned int i;
    mpz_t msk, rop, tmp_d;
    mpz_init(msk);
    mpz_init(rop);
    mpz_init(tmp_d);
    mpz_set_ui(msk, 1);
    mpz_set(tmp_d, d);

    fprintf(logs, "\n");
    for(i = d_size; i > 0; i--)
    {
        mpz_tdiv_q_2exp(rop, tmp_d, i - 1); //recupération du bit de poids faible de d
        mpz_and(rop, rop, msk);             //récupération de la valeur du bit de d
        
        if(!(mpz_cmp_ui(rop, 1)))
            fprintf(logs, "1");
        else
            fprintf(logs, "0");
    }
    fprintf(logs, "\n");
}
