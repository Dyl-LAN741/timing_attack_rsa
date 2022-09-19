*Projet réalisé par Linda Bedjaoui, Allaye Diallo, Sofiane Hamad, Dylan Haral et Shuyang Zhou*

*Référence: IN15221-TER*

*Formation: Master SeCReTS*
___
# Présentation
Projet universitaire de l'*UVSQ* portant sur le sujet de ***« Démonstrateur d’attaque temporelle sur le RSA »***.


Ce projet comporte 2 versions :
- La première qui est dans la racine du projet consiste à effectuer l'attaque sur tous les bits de l'exposant secret en même temps.
- La seconde qui se trouve dans le dossier `./timing_attack_bit_a_bit` effectue l'attaque sur l'exposant secret bit à bit.

Les deux versions fonctionnes, la seconde est beaucoup plus longue en temps d'execution, puisqu'elle fait l'attaque sur chaque bit.

## Aides
Pour compiler le projet :
```shell
$ make
```

Pour nettoyer le projet :
```shell
$ make clean
```

Lors de l'execution du programme vous pouvez choisir :
### Le mode RSA
>	* Tapez 1 pour RSA avec Square and Multiply 
>	* Tapez 2 pour RSA avec Montgomery 

### La taille du module RSA :
>	* Tapez 1 pour 1024 bits  
>	* Tapez 2 pour 2048 bits 
>	* Tapez 3 pour 3072 bits 
>	* Tapez 4 pour 4096 bits 

### Le temps de la réduction de Montgomery pour le timing attack :
>	* Tapez le temps de la réduction (en nanoseconde)

### Le nombre maximum de messages aléatoitres que vous voulez génerer :
>	* Tapez le nombre maximum de messages souhaités

### Avec padding PKCS#1 v1.5
>	* Tapez 1 avec padding
>	* Tapez 2 sans padding

### Résultat:
Vous aurez par la suite les details de vos choix et les messages chiffrés et déchiffrés avec le mode RSA que vous avez choisi.
Si vous choisissez le mode 2 RSA avec Montgomery, le timing attack sera effectué avec le d secret qui sera reconstitué.
Si vous choississez le mode 1 RSA le timing attack n'est pas effectué.