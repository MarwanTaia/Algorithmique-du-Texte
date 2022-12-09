/**
 * FICHIER:
 * --------
 * wordList_generator.c
 * 
 * DESCRIPTION:
 * ------------
 * Programme de générateur pseudo-aléatoire de liste de mots, permettant de
 * stocker dans des fichiers des listes de longueur donnée, contenant des mots
 * de longueur donnée, sur un alphabet de taille donnée.
 * 
 * AUTEUR:
 * -------
 * M. TAIA
 */

////////////////////////////////////////////////////////////////////////////////
// INCLUDES
////////////////////////////////////////////////////////////////////////////////
#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

////////////////////////////////////////////////////////////////////////////////
// DEFINES
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Fonctions
////////////////////////////////////////////////////////////////////////////////
/**
 * Fonction qui génère un mot aléatoire de longueur donnée sur un alphabet de
 * taille donnée.
 * Renvoie le mot généré, en cas de succès, NULL sinon.
 * 
 * @param wordLength longueur du mot à générer.
 * @param alphabetSize taille de l'alphabet sur lequel générer le mot.
 * @return le mot généré.
 */
char* generateWord(int wordLength, int alphabetSize) {
    // Allocation de la mémoire pour le mot.
    char* word = (char*) malloc((long unsigned int) wordLength * sizeof(char));
    // Test d'erreur. Si l'allocation a échoué, on renvoie NULL.
    if (word == NULL) {
        return NULL;
    }

    // Génération du mot.
    for (int i = 0; i < wordLength; i++) {
        word[i] = (char) (rand() % alphabetSize + 'A');
    }
    // Insertion du caractère de fin de chaîne.
    word[wordLength] = '\0';
    // Succès, on renvoie le mot généré.
    return word;
}

/**
 * Fonction qui génère une liste de mots aléatoire de longueur donnée, contenant
 * des mots de longueur donnée, sur un alphabet de taille donnée.
 * Renvoie la liste de mots générée, en cas de succès, NULL sinon.
 * 
 * @param wordListLength longueur de la liste de mots à générer.
 * @param wordLength longueur des mots à générer.
 * @param alphabetSize taille de l'alphabet sur lequel générer les mots.
 * @return la liste de mots générée.
 */
char** generateWordList(int wordListLength, int wordLength, int alphabetSize) {
    // Allocation de la mémoire pour la liste de mots.
    char** wordList = (char**) malloc((long unsigned int) wordListLength * sizeof(char*));
    // Test d'erreur. Si l'allocation a échoué, on renvoie NULL.
    if (wordList == NULL) {
        return NULL;
    }

    // Génération de la liste de mots.
    for (int i = 0; i < wordListLength; i++) {
        wordList[i] = generateWord(wordLength, alphabetSize);
        // Test d'erreur. Si la génération d'un mot a échoué, on renvoie NULL.
        if (wordList[i] == NULL) {
            return NULL;
        }
    }
    // Succès, on renvoie la liste de mots générée.
    return wordList;
}

/**
 * Fonction qui écrit une liste de mots dans un fichier.
 * Renvoie 1 en cas de succès, 0 sinon.
 * 
 * @param wordList liste de mots à écrire dans le fichier.
 * @param wordListLength longueur de la liste de mots.
 * @param wordLength longueur des mots de la liste.
 * @param alphabetSize taille de l'alphabet sur lequel générer les mots.
 * @param fileName nom du fichier dans lequel écrire la liste de mots.
 * @return 0 en cas de succès, -1 sinon.
 */
int writeWordList(char** wordList, int wordListLength, char* fileName) {
    // Ouverture du fichier en écriture.
    FILE* file = fopen(fileName, "w");
    // Test d'erreur. Si l'ouverture a échoué, on renvoie 0.
    if (file == NULL) {
        return -1;
    }

    // Ecriture de la liste de mots dans le fichier avec test d'erreur.
    for (int i = 0; i < wordListLength; i++) {
        if (fprintf(file, "%s\n", wordList[i]) < 0) {
            fprintf(stderr, "Erreur lors de l'écriture du mot %d dans le fichier %s.\n", i, fileName);
            return -1;
        }
    }

    // Fermeture du fichier.
    fclose(file);
    // Succès, on renvoie 0.
    return 0;
}

/**
 * Fonction qui libère la mémoire allouée pour une liste de mots.
 * 
 * @param wordList liste de mots dont on veut libérer la mémoire.
 * @param wordListLength longueur de la liste de mots.
 */
void freeWordList(char** wordList, int wordListLength) {
    // Libération de la mémoire pour chaque mot de la liste.
    for (int i = 0; i < wordListLength; i++) {
        free(wordList[i]);
    }
    // Libération de la mémoire pour la liste de mots.
    free(wordList);
}

////////////////////////////////////////////////////////////////////////////////
// MAIN
////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv) {
    // Test du nombre d'arguments.
    if (argc != 5) {
        fprintf(stderr, "Usage: %s : Générateur de liste de taille argv[1] de \
                mots de longueur argv[2] sur un alphabet de taille argv[3] \
                dans le fichier argv[4].\n", argv[0]);
        return -1;
    }

    // Récupération des arguments.
    int wordListLength = atoi(argv[1]);
    int wordLength = atoi(argv[2]);
    int alphabetSize = atoi(argv[3]);
    char* fileName = argv[4];

    // Test des arguments.
    if (wordListLength <= 0 || wordListLength > MAX_WORD_LIST_LENGTH) {
        fprintf(stderr, "La longueur de la liste de mots doit être comprise entre 1 et %d.\n", MAX_WORD_LIST_LENGTH);
        return -1;
    }
    if (wordLength <= 0 || wordLength > MAX_WORD_LENGTH) {
        fprintf(stderr, "La longueur des mots doit être comprise entre 1 et %d.\n", MAX_WORD_LENGTH);
        return -1;
    }
    if (alphabetSize <= 0 || alphabetSize > MAX_ALPHABET_SIZE) {
        fprintf(stderr, "La taille de l'alphabet doit être comprise entre 1 et %d.\n", MAX_ALPHABET_SIZE);
        return -1;
    }

    // Génération de la liste de mots.
    char** wordList = generateWordList(wordListLength, wordLength, alphabetSize);
    // Test d'erreur. Si la génération a échoué, on renvoie -1.
    if (wordList == NULL) {
        fprintf(stderr, "Erreur lors de la génération de la liste de mots.\n");
        return -1;
    }

    // Ecriture de la liste de mots dans le fichier.
    int result = writeWordList(wordList, wordListLength, fileName);
    // Test d'erreur. Si l'écriture a échoué, on renvoie -1.
    if (result != 0) {
        fprintf(stderr, "Erreur lors de l'écriture de la liste de mots dans le fichier %s.\n", fileName);
        return -1;
    }

    // Libération de la mémoire allouée pour la liste de mots.
    freeWordList(wordList, wordListLength);

    // Succès, on renvoie 0.
    return 0;
}