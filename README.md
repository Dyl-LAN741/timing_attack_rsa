*Projet réalisé par Linda Bedjaoui, Allaye Diallo, Sofiane Hamad, Dylan Haral et Shuyang Zhou*

*Référence: IN15221-TER*

*Formation: Master SeCReTS*
___
# Présentation
Projet universitaire de l'*UVSQ* portant sur le sujet ***« Démonstrateur d’attaque temporelle sur le RSA »***.

Ce projet comporte 2 versions :
- une version pratique `/src` qui effectue l'attaque sur l'ensemble des bits de l'exposant privée d.
- une version de démonstration réaliste `/timing_attack_bit_a_bit` qui effectue l'attaque bit à bit de l'exposant privée d.

## Prérequis
- bibliothèque de développement d'[OpenSSL](https://openssl-library.org/)
- bibliothèque de [GNU MP](https://gmplib.org/)

## Aides
Compilation et exécution :
```shell
$ make
```

Nettoyage :
```shell
$ make clean
```

À l'exécution du programme les options suivantes sont proposées :
### Le mode RSA :
>	* Tapez 1 pour RSA Square and Multiply
>	* Tapez 2 pour RSA Montgomery

### La taille du module RSA :
>	* Tapez 1 pour 1024 bits
>	* Tapez 2 pour 2048 bits
>	* Tapez 3 pour 3072 bits
>	* Tapez 4 pour 4096 bits

### Le temps de la réduction de Montgomery pour le timing attack :
>	* Tapez le temps de la réduction (en nanoseconde)

### Le nombre maximum de messages aléatoitres génerés :
>	* Tapez le nombre maximum de messages souhaités

### Le padding PKCS#1 v1.5 :
>	* Tapez 1 avec padding
>	* Tapez 2 sans padding

### Résultat :
Affiche :
- les details des choix pris en compte à l'éxecution
- les messages chiffrés et déchiffrés
- le résultat de l'attaque si le mode RSA Montgomery est utilisé
- aucune attaque n'est éffectuée si le mode RSA Square and Multiply est utilisé
