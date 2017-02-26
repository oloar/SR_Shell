Sorin Gaëtan

Gr1


# Systèmes et Réseaux

## Projet Shell

Le but du projet est de realiser un interprète pour un langage de commande simplifie. L’objectif est d’une part de comprendre la structure d’un shell et d’autre part d’apprendre à utiliser quelques appels systemes importants, typiquement ceux qui concernent la gestion des processus, les tubes et la redéfinition des fichiers standards d’entrée et de sortie.


### Travail réalisé:
- Interprétation des commande simple (Création de processus `fork()`, `waitpid()`, execution des commandes grace à **exec()**).
- Redirection d'entrée et/ou de sortie (Manipulation des descripteurs de fichiers, utilisation de la commande **dup2()**).
- Implémentation des séquences de commandes (Gestion des pipes avec la commande `pipe()`).
- Exécution des taches en arrière-plan (Reconnaissance du caractère `&`, modification des appels de **waitpid()**).

### Travail à réalisé:
- Changement d'état du processus de premier plan (Capture des signaux `SIGINT` et `SIGTSTP`, et transmission des signaux au(x) processus concerné(s)).
- Gestion des zombies (Implémenté une gestion de `SIGCHLD` et modification des appels de `waitpid()`).
- Implémentation des commandes `fg` et `bg`, qui transitionnent un jobs de l'arrière-plan vers le premier plan pour `fg` et inversement pour `bg`, et de le commande `stop` qui interrompt l'execution d'un processus.

### Tests réalisés:

Le shell de référence choisi est **Bash 4.4.12**

- Exécution des commandes: Différentes commandes ont été éxécuté et leurs sorties comparées à celle de leur exécution dans Bash.
- Redirections: Les redirections ont été testées de et vers des fichiers en variant les permissions accordées.
- Pipes: En l'état, le shell créé support jusqu'à 10 pipes (nombre arbitrairement choisi, modifié la valeur NBPIPESMAX si nécéssaire). Les tests ont été réalisé dans et au delà de cette limite en imbriquant plusieurs commandes variées.
