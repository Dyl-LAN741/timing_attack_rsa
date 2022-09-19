#define _GNU_SOURCE //fix timespec initialisation

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <gmp.h>
#include "temps.h"
#include <time.h>
#include <sys/time.h>
#include "creation_des_cles.h"
#include "chiffrement.h"
#include "dechiffrement.h"
#include "rsa.h"
#include "timing_attack.h"

static void choix_mode_rsa(char* choix)
{
    int ctr = 1;
    printf("\n\tChoisir le mode de RSA :\n\t- normal (1)\n\t- montgomery (2)\n\t  ");
    *choix = getchar();
    while(getchar() != '\n')
        ctr++;

    if((*choix != '1' && *choix != '2') || ctr != 1)
    {
        fprintf(stderr, "Erreur : saisie incorrecte\n");
        printf("Usage : entrez la touche 1 ou 2\n");
        choix_mode_rsa(choix);
    }
}

static void choix_taille_module_rsa(char* choix)
{
    int ctr = 1;
    printf("\n\tChoisir la taille du module RSA :\n\t- petite : 1024 (1)\n\t- moyenne : 2048 (2)\n\t- grande : 3072 (3)\n\t- très grande : 4096 (4)\n\t  ");
    *choix = getchar();
    while(getchar() != '\n')
        ctr++;

    if((*choix != '1' && *choix != '2' && *choix != '3' && *choix != '4') || ctr != 1)
    {
        fprintf(stderr, "Erreur : saisie incorrecte\n");
        printf("Usage : entrez la touche 1, 2, 3 ou 4\n");
        choix_taille_module_rsa(choix);
    }
}

static void choix_loop_iteration(char*  choix, char** iter)
{
    unsigned int taille = 1, str_length;
    char* tmp = NULL;
    printf("\n\tChoisir le nombre de message pour le timing attack :\n\t  ");
    *choix = getchar();
    if(*choix != '0' && *choix != '1' && *choix != '2' && *choix != '3' && *choix != '4' && *choix != '5' && *choix != '6' && *choix != '7' && *choix != '8' && *choix != '9')
    {
        while(getchar() != '\n');
        choix_loop_iteration(choix, iter);
        return;
    }
    *iter = malloc(sizeof(char) + 1);
    strncpy(*iter, choix, 1);
    while(1)
    {
        *choix = getchar();
        if(*choix == '\n')
            return;
        if(*choix != '0' && *choix != '1' && *choix != '2' && *choix != '3' && *choix != '4' && *choix != '5' && *choix != '6' && *choix != '7' && *choix != '8' && *choix != '9')
        {
            fprintf(stderr, "Erreur : saisie incorrecte\n");
            printf("Usage : entrez un nombre\n");
            if(tmp)
                free(tmp);
            free(*iter);
            choix_loop_iteration(choix, iter);
            return;
        }
        taille++;
        if(tmp)
            free(tmp);
        tmp = malloc(sizeof(char) * strlen(*iter) + 1);
        str_length = sizeof(*iter);
        strncpy(tmp, *iter, str_length);
        free(*iter);
        *iter = malloc(sizeof(char) * taille + 1);
        str_length = sizeof(tmp);
        strncpy(*iter, tmp, str_length);
        strncat(*iter, choix, 1);
    }
}

static void choix_temps_reduction(char*  choix, char** temps_reduc)
{
    unsigned int taille = 1, str_length;
    char* tmp = NULL;
    printf("\n\tChoisir le temps de la reduction de montgomery pour timing attack (en nanosecondes) :\n\t  ");
    *choix = getchar();
    if(*choix != '0' && *choix != '1' && *choix != '2' && *choix != '3' && *choix != '4' && *choix != '5' && *choix != '6' && *choix != '7' && *choix != '8' && *choix != '9')
    {
        while(getchar() != '\n');
        choix_temps_reduction(choix, temps_reduc);
        return;
    }
    *temps_reduc = malloc(sizeof(char) + 1);
    strncpy(*temps_reduc, choix, 1);
    while(1)
    {
        *choix = getchar();
        if(*choix == '\n')
            return;
        if(*choix != '0' && *choix != '1' && *choix != '2' && *choix != '3' && *choix != '4' && *choix != '5' && *choix != '6' && *choix != '7' && *choix != '8' && *choix != '9')
        {
            fprintf(stderr, "Erreur : saisie incorrecte\n");
            printf("Usage : entrez un nombre\n");
            if(tmp)
                free(tmp);
            free(*temps_reduc);
            choix_temps_reduction(choix, temps_reduc);
            return;
        }
        taille++;
        if(tmp)
            free(tmp);
        tmp = malloc(sizeof(char) * strlen(*temps_reduc) + 1);
        str_length = sizeof(*temps_reduc);
        strncpy(tmp, *temps_reduc, str_length);
        free(*temps_reduc);
        *temps_reduc = malloc(sizeof(char) * taille + 1);
        str_length = sizeof(tmp);
        strncpy(*temps_reduc, tmp, str_length);
        strncat(*temps_reduc, choix, 1);
    }
}

static void choix_padding(char* choix)
{
    int ctr = 1;
    printf("\n\tAvec padding PKCS#1 v1.5 ?\n\t- oui (1)\n\t- non (2)\n\t  ");
    *choix = getchar();
    while(getchar() != '\n')
        ctr++;

    if((*choix != '1' && *choix != '2') || ctr != 1)
    {
        fprintf(stderr, "Erreur : saisie incorrecte\n");
        printf("Usage : entrez la touche 1 ou 2\n");
        choix_padding(choix);
    }
}

void run()
{
    unsigned long int nombre_iteration;
    char m = '\0', t = '\0', p = '\0', c = '\0';
    char* iteration = NULL;
    char* temps_reduction = NULL;
    srand(getpid() + time(NULL));           //utiliser pour p, q, PKCS#1, (et le message clair)
    printf("\n\t\t--Timing Attack--\n");
    choix_mode_rsa(&m);
    choix_taille_module_rsa(&t);
    if(m == '2')
    {
        choix_temps_reduction(&c, &temps_reduction);
        choix_loop_iteration(&c, &iteration);
    }
    else {
        iteration = malloc(sizeof(char) * 1 + 1);
        iteration = "1";
        temps_reduction = malloc(sizeof(char) * 1 + 1);
        temps_reduction = "0";
    }
    choix_padding(&p);

    if(t == '1')
    {
        printf("\n\t\tRSA-1024");
        prime_size = PRIME_LOW_SIZE;
    }

    if(t == '2')
    {
        printf("\n\t\tRSA-2048");
        prime_size = PRIME_MEDIUM_SIZE;
    }

    if(t == '3')
    {
        printf("\n\t\tRSA-3072");
        prime_size = PRIME_LONG_SIZE;
    }

    if(t == '4')
    {
        printf("\n\t\tRSA-4096");
        prime_size = PRIME_VERY_LONG_SIZE;
    }

    if(m == '1')
        printf("-CLASSIQUE");
    else
        printf("-MONTGOMERY");

    if(p == '1') {
        padding = true;
        printf("-AVEC_PADDING\n\n");
    }
    else {
        padding = false;
        printf("-SANS_PADDING\n\n");
    }
    
    if(m == '2')
        nombre_iteration = atoi(iteration);
    else
        nombre_iteration = 1;
    
    if(m == '2')
        TEMPS_REDUC = atoi(temps_reduction);
    else
        TEMPS_REDUC = 0;

    run_rsa(m, nombre_iteration);

    if(m == '2')
    {
        free(iteration);
        free(temps_reduction);
    } 
}

void run_rsa(const char mode, unsigned long int numero_iteration)
{
    //--------------------CHRONO--------------------------
    clock_t t_cpu_deb = 0, t_cpu_fin = 0;
    struct timespec t_reel_deb = {0,0}, t_reel_fin = {0,0};
    double temps_reel_total = 0.0, temps_cpu_total = 0.0;
    debut_chrono(&t_cpu_deb,&t_reel_deb);
    //----------------------------------------------------
    
    //unsigned int i;
    unsigned int taille;
    unsigned long int iteration = numero_iteration, iteration_total = 1;
    mpz_t  m, e, p , q, n, c, phi_n, d, pkcs_sgn, s, hm, r, u, v, pgcd_bezout, d_secret, tmp_m;
    mpz_init(m);        //clair
    mpz_init(e);        //exposant publique e : 1 < e < phi(n)
    mpz_init(p);        //nombre premier, premier facteur de n
    mpz_init(q);        //nombre premier, deuxième facteur de n
    mpz_init(n);        //produit des deux nombres premiers
    mpz_init(c);        //chiffré
    mpz_init(phi_n);    //phi(n) : (p - 1) * (q - 1)
    mpz_init(d);        //exposant privé : e^-1 mod phi(n)
    mpz_init(pkcs_sgn); //PKCS#1 signature : µ(H(m))
    mpz_init(s);        //signature
                        //  - sans padding : H(m)^(d) mod n
                        //  - avec padding : µ(H(m))^(d) mod n
    mpz_init(hm);       //hash du message pour la signature : H(m)
    mpz_init(r);
    mpz_init(u);
    mpz_init(v);
    mpz_init(pgcd_bezout);
    mpz_init(d_secret);
    mpz_init(tmp_m);
    
    printf("chiffrement n°%lu\n", iteration - numero_iteration + 1);
    //message clair aléatoire
    mpz_set_ui(m, (unsigned long int) rand());
    gmp_printf("message d'origine : %Z0X\n", m);
    mpz_set(s, m);
    mpz_set(hm, m);
    hash(hm);
    
    //gmp_printf("\nhash : %Z0X\n", hm);
    mpz_set(pkcs_sgn, hm);

    //génération de n, p, q
    generer_npq(n, p, q);
    //gmp_printf("\np : %Zd, \nq : %Zd, \nn : %Zd\n", p, q, n);
    
    //génération de phi(n)
    phi(p, q, phi_n);
    //gmp_printf("\nphi(n) : %Zd\n", phi_n);
    
    //génération de e
    generer_exposant_public(e);
    gmp_printf("exposant publique e : %Zd\n\n", e);

    //génération de d
    generer_exposant_privee(e, phi_n, d);
    //gmp_printf("\nexposant privee d : %Zd\n", d);

    if(mode == '2')
    {
        //initialise A, B, T
        initialiser_variables_globales_timing_attack();
    }

    for(target_bit = d_size - 1; target_bit > 0; target_bit--)  //bit cible
    {
        while(arret) //on s'arrete si le bit = 1
        {
            if(numero_iteration == 0)
                break;

            if(iteration_total != 1)
            {
                printf("chiffrement n°%lu - bit cible n°%u\n", iteration_total, target_bit);
                mpz_set_ui(m, (unsigned long int) rand());  //génération d'un nouveau message aléatoire
                gmp_printf("message d'origine : %Z0X\n", m);
                mpz_set(s, m);
                mpz_set(hm, m);
                hash(hm);
                //gmp_printf("\nhash : %Z0X\n", hm);
                mpz_set(pkcs_sgn, hm);
            }

            if(mode == '2')
            {
                //générer R
                generer_R_montgomery(r);
                //bezout
                bezout(r, n, u, v, pgcd_bezout);
                //gmp_printf("R(%Zd) * v(%Zd) + N(%Zd) * u(%Zd)\n\n", r, u, n, v);
            }

            //chiffrement de m
            if(mode == '1')
                chiffrement_RSA(m, e, n, c);
            else
                chiffrement_RSA_montgomery(m, e, n, c, v, n_size);
            gmp_printf("chiffré : %Z0X\n", c);

            //signature de m
            signature(pkcs_sgn, d, n, s);
            //gmp_printf("\nsignature : %Z0X\n", s);

            //vérification de s
            verification_signature(s, e, n, hm);

            DECRYPT = 1;

            //dechifrement de m
            if(mode == '1')
                dechiffrement_RSA(c, d, n, m);
            else
                dechiffrement_RSA_montgomery(c, d, n, m, v, n_size);
            gmp_printf("message déchiffré : %Z0X\n", m);
            
            DECRYPT = 0;

            numero_iteration--;
            iteration_total++;
            
            if(mode == '1')
                break;
            if(mode == '2')
                printf("********************************************************************************************************************************************************************************************************\n");
        }

        if(mode == '1')
            break;

        numero_iteration = iteration;
        arret = 1;
    }

    if(mode ==  '2')
    {
        //afficher_ensemble_simple(A, "A");
        //afficher_ensemble_simple(B, "B");
        //printf("\n");
        //afficher_tableau_T();
        
        //Affichage de d
        //printf("taille de n : %u\n", n_size);
        //printf("taille de d : %u\n", d_size);
        //affichage_binaire_mpz(d);
        
        //Affichage de d secret
        reconstituer_d(d_secret);   //reconstitution à l'aide du tableau T
        taille = (unsigned int) mpz_sizeinbase(d_secret, 2);    //taille de d secret
        printf("taille de d secret : %u\n", taille);
        //affichage_binaire_mpz(d_secret);

        //Comparaison entre d et d secret
        //gmp_printf("\nd : %Z0X\n", d);
        gmp_printf("\nd secret : %Z0X\n", d_secret);
        printf("\n");

        //Vérification de d secret
        printf("vérification de d secret ...\n\n");
        gmp_printf("message clair attendu : %Z0X\n", m);
        gmp_printf("chiffré test : %Z0X\n", c);
        mpz_set(tmp_m, m);
        dechiffrement_RSA_montgomery(c, d_secret, n, m, v, n_size);
        gmp_printf("message déchiffré : %Z0X\n", m);
        if(!(mpz_cmp(tmp_m, m)))
        {
            printf("d secret est valide.\n\n");
            printf("L'attaque temporelle a réussi.\n");
        } else {
            fprintf(stderr,"d secret est invalide.\n\n");
            printf("L'attaque temporelle a échoué.\n");
        }
        
        printf("\ttemps de la reduction : %.9f\n", TEMPS_REDUC * 1e-9);
        printf("\tmessage(s) par bit : %lu\n", iteration);
        printf("\tnombre de messages total : %lu\n", iteration_total);

        //désallocation des ensembles A et B
        supprimer_ensemble(&A, "A");
        supprimer_ensemble(&B, "B");
        //désallocation du tableau T
        supprimer_tableau(&T);
    }

    mpz_clear(m);
    mpz_clear(e);
    mpz_clear(p);
    mpz_clear(q);
    mpz_clear(n);
    mpz_clear(c);
    mpz_clear(phi_n);
    mpz_clear(d);
    mpz_clear(pkcs_sgn);
    mpz_clear(s);
    mpz_clear(hm);
    mpz_clear(r);
    mpz_clear(u);
    mpz_clear(v);
    mpz_clear(pgcd_bezout);
    mpz_clear(d_secret);
    mpz_clear(tmp_m);

    //-----------------------------------FIN_CHRONO-----------------------------------------
    fin_chrono(&temps_cpu_total,t_cpu_deb,t_cpu_fin,&temps_reel_total,t_reel_deb,t_reel_fin);
    printf("\t");
    afficher_temps_cpu(&temps_cpu_total);
    printf("\t");
    afficher_temps_reel(&temps_reel_total);
    //--------------------------------------------------------------------------------------
    printf("\n");
}
