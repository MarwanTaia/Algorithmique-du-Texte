/**
 * FICHIER:
 * --------
 * demo.c
 * 
 * DESCRIPTION:
 * ------------
 * Programme de démo des algorithmes de recherche exacte.
 * 
 * Le programme génère un texte aléatoire, puis recherche une liste de mots
 * aléatoire dans ce texte. Cela pour un texte de longueur donnée, une liste de
 * longueur donnée, des mots de longueur donnée, et un alphabet de taille donnée
 * (par défaut, qlmkdfjlqsdjfqsjdfk).
 * 
 * Le texte est généreé en exécutant le programme singleStringText_generator 
 * avec la longueur du texte, la taille de l'alphabet, et le nom du fichier de
 * sortie en paramètres.
 * La liste de mots est générée en exécutant le programme wordList_generator
 * avec la longueur de la liste, la longueur des mots, la taille de l'alphabet,
 * et le nom du fichier de sortie en paramètres.
 * Le programme stocke le texte et la liste de mots respectivement dans les
 * fichiers demo_text.txt et demo_wordList.txt.
 * 
 * Le programme affiche le temps d'exécution de chaque algorithme de recherche
 * exacte.
 * 
 * AUTHEUR:
 * --------
 * M. TAIA
 */

////////////////////////////////////////////////////////////////////////////////
// INCLUDES
////////////////////////////////////////////////////////////////////////////////
#include "config.h"
#include "exactSearchAlgos.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

////////////////////////////////////////////////////////////////////////////////
// DEFINES
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Fonctions
////////////////////////////////////////////////////////////////////////////////
/**
 * Fonction qui génère un texte aléatoire de longueur donnée sur un alphabet de
 * taille donnée, en exécutant le programme singleStringText_generator. Stocke
 * le texte généré dans le fichier demo_text.txt.
 * Renvoie 0 en cas de succès, -1 sinon.
 * 
 * @param textLength longueur du texte à générer.
 * @param alphabetSize taille de l'alphabet sur lequel générer le texte.
 * @return 0 en cas de succès, -1 sinon.
 */
int generateText(int textLength, int alphabetSize) {
    // Génération du texte.
    char command[100];
    sprintf(command, "./singleStringText_generator %d %d demo_text.txt", textLength, alphabetSize);
    int result = system(command);
    // Test d'erreur. Si la génération a échoué, on renvoie -1.
    if (result == -1) {
        return -1;
    }
    // Succès, on renvoie 0.
    return 0;
}

/**
 * Fonction qui génère une liste de mots aléatoire de longueur donnée sur un
 * alphabet de taille donnée, en exécutant le programme wordList_generator.
 * Stocke la liste de mots générée dans le fichier demo_wordList.txt.
 * Renvoie 0 en cas de succès, -1 sinon.
 * 
 * @param wordListLength longueur de la liste de mots à générer.
 * @param wordLength longueur des mots à générer.
 * @param alphabetSize taille de l'alphabet sur lequel générer les mots.
 * @return 0 en cas de succès, -1 sinon.
 */
int generateWordList(int wordListLength, int wordLength, int alphabetSize) {
    // Génération de la liste de mots.
    char command[100];
    sprintf(command, "./wordList_generator %d %d %d demo_wordList.txt", wordListLength, wordLength, alphabetSize);
    int result = system(command);
    // Test d'erreur. Si la génération a échoué, on renvoie -1.
    if (result == -1) {
        return -1;
    }
    // Succès, on renvoie 0.
    return 0;
}

/**
 * Fonction qui lit le texte dans le fichier demo_text.txt, et le stocke dans
 * la chaîne de caractères text.
 * Renvoie 0 en cas de succès, -1 sinon.
 * 
 * @param text chaîne de caractères dans laquelle stocker le texte.
 * @return 0 en cas de succès, -1 sinon.
 */
int readText(char* text) {
    // Ouverture du fichier.
    FILE* file = fopen("demo_text.txt", "r");
    // Test d'erreur. Si l'ouverture a échoué, on renvoie -1.
    if (file == NULL) {
        return -1;
    }
    // Lecture du texte avec fgets et test d'erreur.
    if (fgets(text, MAX_TEXT_LENGTH, file) == NULL) {
        return -1;
    }
    // Fermeture du fichier.
    fclose(file);
    // Succès, on renvoie 0.
    return 0;
}

/**
 * Fonction qui cherche toutes les occurrences d'un mot dans un autre en
 * utilisant l'algorithme pris en paramètre.
 * L'algorithme pris en paramètre prend en paramètre le texte, le mot à chercher
 * et rend l'indice de la première occurrence du mot dans le texte.
 * Il faut donc appeler l'algorithme tant qu'il renvoie un indice différent de
 * -1.
 * 
 * @param algo algorithme de recherche exacte à utiliser.
 * @param text texte dans lequel chercher le mot.
 * @param word mot à chercher dans le texte.
 * @param textLength longueur du texte.
 * @param wordLength longueur du mot.
 * @return 0 en cas de succès, -1 sinon.
 */
int runAlgo(int (*algo)(char*, char*, int, int), char* text, char* word, int textLength, int wordLength) {
    // Initialisation de l'indice de la première occurrence.
    int index = 0;
    // Tant qu'on trouve une occurrence, on décale l'indice de la première
    // occurrence
    for (int i = 0; i < textLength - wordLength + 1; i++) {
        // Appel de l'algorithme.
        index = algo(text + i, word, textLength - i, wordLength);
        // Test d'erreur. Si l'algorithme a échoué, on renvoie -1.
        if (index == -2) {
            return -1;
        }
        // Si l'algorithme a trouvé une occurrence, on décale l'indice de la
        // première occurrence.
        if (index != -1) {
            i += index;
        }
        // Si aucune occurrence n'a été trouvée, on sort de la boucle.
        else {
            break;
        }
    }
    // Succès, on renvoie 0.
    return 0;
}

/**
 * Fonction qui calcule la moyenne des valeurs contenues dans le tableau
 * values, et renvoie cette moyenne.
 * 
 * @param values tableau de valeurs.
 * @param length longueur du tableau.
 * @return moyenne des valeurs contenues dans le tableau.
 */
double computeAverage(double* values, int length) {
    // Initialisation de la somme.
    double sum = 0;
    // On additionne les valeurs du tableau.
    for (int i = 0; i < length; i++) {
        sum += values[i];
    }
    // On renvoie la moyenne.
    return sum / length;
}

/**
 * Fonction qui mesure le temps moyen d'exécution de l'algorithme pris en 
 * paramètre. Pour cela, on exécute lit la liste de mots dans le fichier
 * file, et on mesure le temps moyen d'exécution de l'algorithme sur chaque
 * mot de la liste.
 * Renvoie le temps moyen d'exécution de l'algorithme (en ms) en cas de succès,
 * -1 sinon.
 * 
 * @param algo algorithme de recherche exacte à utiliser.
 * @param file pointeur de fichier vers le fichier contenant la liste de mots.
 * @param text texte dans lequel chercher les mots de la liste.
 * @param textLength longueur du texte.
 * @param wordListLength longueur de la liste de mots.
 * @param wordLength longueur des mots de la liste.
 * @return temps moyen d'exécution de l'algorithme (en ms) en cas de succès,
 * -1 sinon.
 */
double measureTime(int (*algo)(char*, char*, int, int), FILE* file, char* text, int textLength, int wordListLength, int wordLength) {
    // Initialisations
    int err = 0;
    // Tableau de temps d'exécution
    double times[wordListLength];

    // On lit chaque mot de la liste
    for (int i = 0; i < wordListLength; i++) {
        // On lit le mot
        char word[wordLength + 1];
        if (fgets(word, wordLength + 1, file) == NULL) {
            return -1;
        }
        // On mesure le temps d'exécution de l'algorithme
        clock_t start = clock();
        err = runAlgo(algo, text, word, textLength, wordLength);
        clock_t end = clock();
        // Test d'erreur
        if (err == -1) {
            return -1;
        }
        // On stocke le temps d'exécution dans le tableau
        times[i] = (double) (end - start) / CLOCKS_PER_SEC * 1000;
    }
    // On renvoie la moyenne des temps d'exécution
    return computeAverage(times, wordListLength);
}

/**
 * Fonction qui lit la liste de mots dans le fichier demo_wordList.txt, et
 * mesure le temps moyen d'exécution de chaque algorithme de recherche exacte.
 * Pour chaque algorithme, on affiche le temps moyen d'exécution sur la recherche
 * de chaque mot de la liste dans le texte text renseigné en paramètre.
 * Renvoie 0 en cas de succès, -1 sinon.
 * 
 * @param text texte dans lequel rechercher les mots de la liste.
 * @param textLength longueur du texte.
 * @param wordListLength longueur de la liste de mots.
 * @param wordLength longueur des mots de la liste.
 * @return 0 en cas de succès, -1 sinon.
 */
int testAlgos(char* text, int textLength, int wordListLength, int wordLength) {
    // Ouverture du fichier.
    FILE* file = fopen("demo_wordList.txt", "r");
    // Test d'erreur. Si l'ouverture a échoué, on renvoie -1.
    if (file == NULL) {
        return -1;
    }
    // Initialisation de variables.
    double err = 0;

    // Mesure du temps d'exécution de l'algorithme naïf, sans boucle rapide, sans sentinelle.
    printf("Algo naïf, sans boucle rapide, sans sentinelle : ");
    err = measureTime(&naif, file, text, textLength, wordListLength, wordLength);
    // Test d'erreur. Si la mesure a échoué, on renvoie -1.
    if (err == -1) {
        return -1;
    }
    // Affichage du temps d'exécution.
    printf("%f (ms)\n", err);
    // On revient au début du fichier.
    rewind(file);

    // Mesure du temps d'exécution de l'algorithme naïf, avec boucle rapide, sans sentinelle.
    printf("Algo naïf, avec boucle rapide, sans sentinelle : ");
    err = measureTime(&naifBoucleRapide, file, text, textLength, wordListLength, wordLength);
    // Test d'erreur. Si la mesure a échoué, on renvoie -1.
    if (err == -1) {
        return -1;
    }
    // Affichage du temps d'exécution.
    printf("%f (ms)\n", err);
    // On revient au début du fichier.
    rewind(file);

    // Mesure du temps d'exécution de l'algorithme naïf, avec boucle rapide, avec sentinelle.
    printf("Algo naïf, avec boucle rapide, avec sentinelle : ");
    err = measureTime(&naifSentinelle, file, text, textLength, wordListLength, wordLength);
    // Test d'erreur. Si la mesure a échoué, on renvoie -1.
    if (err == -1) {
        return -1;
    }
    // Affichage du temps d'exécution.
    printf("%f (ms)\n", err);
    // On revient au début du fichier.
    rewind(file);

    // Mesure du temps d'exécution de l'algorithme naïf avec strncmp, sans boucle rapide, sans sentinelle.
    printf("Algo naïf avec strncmp, sans boucle rapide, sans sentinelle : ");
    err = measureTime(&naifStrncmp, file, text, textLength, wordListLength, wordLength);
    // Test d'erreur. Si la mesure a échoué, on renvoie -1.
    if (err == -1) {
        return -1;
    }
    // Affichage du temps d'exécution.
    printf("%f (ms)\n", err);
    // On revient au début du fichier.
    rewind(file);

    // Mesure du temps d'exécution de l'algorithme naïf avec strncmp, avec boucle rapide, sans sentinelle.
    printf("Algo naïf avec strncmp, avec boucle rapide, sans sentinelle : ");
    err = measureTime(&naifRapideStrncmp, file, text, textLength, wordListLength, wordLength);
    // Test d'erreur. Si la mesure a échoué, on renvoie -1.
    if (err == -1) {
        return -1;
    }
    // Affichage du temps d'exécution.
    printf("%f (ms)\n", err);
    // On revient au début du fichier.
    rewind(file);

    // Mesure du temps d'exécution de l'algorithme naïf avec strncmp, avec boucle rapide, avec sentinelle.
    printf("Algo naïf avec strncmp, avec boucle rapide, avec sentinelle : ");
    err = measureTime(&naifSentinelleStrncmp, file, text, textLength, wordListLength, wordLength);
    // Test d'erreur. Si la mesure a échoué, on renvoie -1.
    if (err == -1) {
        return -1;
    }
    // Affichage du temps d'exécution.
    printf("%f (ms)\n", err);
    // On revient au début du fichier.
    rewind(file);

    // Succès, on renvoie 0.
    return 0;
}
    


/**
 * Fonction principale.
 * 
 * @param argc nombre d'arguments.
 * @param argv arguments.
 * @return 0 en cas de succès, -1 sinon.
 */
int main(int argc, char** argv) {
    // Test du nombre d'arguments.
    if (argc != 5) {
        printf("Usage : %s textLength alphabetSize wordLength wordListLength \n", argv[0]);
        return -1;
    }

    // Récupération des arguments.
    int textLength = atoi(argv[1]);
    int alphabetSize = atoi(argv[2]);
    int wordLength = atoi(argv[3]);
    int wordListLength = atoi(argv[4]);

    // Test des arguments
    if (textLength <= 0 || textLength > MAX_TEXT_LENGTH) {
        fprintf(stderr, "La longueur du texte doit être comprise entre 1 et %d.\n", MAX_TEXT_LENGTH);
        return -1;
    }
    if (alphabetSize <= 0 || alphabetSize > MAX_ALPHABET_SIZE) {
        fprintf(stderr, "La taille de l'alphabet doit être comprise entre 1 et %d.\n", MAX_ALPHABET_SIZE);
        return -1;
    }
    if (wordLength <= 0 || wordLength > MAX_WORD_LENGTH) {
        fprintf(stderr, "La longueur des mots doit être comprise entre 1 et %d.\n", MAX_WORD_LENGTH);
        return -1;
    }
    if (wordListLength <= 0 || wordListLength > MAX_WORD_LIST_LENGTH) {
        fprintf(stderr, "La longueur de la liste de mots doit être comprise entre 1 et %d.\n", MAX_WORD_LIST_LENGTH);
        return -1;
    }

    // Génération du texte et de la liste de mots.
    if (generateText(textLength, alphabetSize) == -1) {
        fprintf(stderr, "Erreur lors de la génération du texte.\n");
        return -1;
    }
    if (generateWordList(wordListLength, wordLength, alphabetSize) == -1) {
        fprintf(stderr, "Erreur lors de la génération de la liste de mots.\n");
        return -1;
    }
    // Lecture du texte.
    char text[MAX_TEXT_LENGTH];
    if (readText(text) == -1) {
        fprintf(stderr, "Erreur lors de la lecture du texte.\n");
        return -1;
    }
    // Test des algorithmes.
    if (testAlgos(text, textLength, wordListLength, wordLength) == -1) {
        fprintf(stderr, "Erreur lors du test des algorithmes.\n");
        return -1;
    }
    // Succès, on renvoie 0.
    return 0;
}