/**
 * FICHIER:
 * --------
 * singleStringText_generator.c
 * 
 * DESCRIPTION:
 * ------------
 * Programme de générateur pseudo-aléatoire de texte, permettant de stocker dans
 * des fichiers des textes d'une longueur donnée sur un alphabet de taille
 * donnée.
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
 * Fonction qui génère un texte aléatoire de longueur donnée sur un alphabet
 * de taille donnée.
 * Renvoie le texte généré, en cas de succès, NULL sinon.
 * 
 * @param textLength longueur du texte à générer.
 * @param alphabetSize taille de l'alphabet sur lequel générer le texte.
 * @return le texte généré.
 */
char* generateText(int textLength, int alphabetSize) {
    // Allocation de la mémoire pour le texte.
    char* text = (char*) malloc((long unsigned int) textLength * sizeof(char));
    // Test d'erreur. Si l'allocation a échoué, on renvoie NULL.
    if (text == NULL) {
        return NULL;
    }

    // Génération du texte.
    for (int i = 0; i < textLength; i++) {
        text[i] = (char) (rand() % alphabetSize + '!');
    }
    // Succès, on renvoie le texte généré.
    return text;
}

/**
 * Fonction qui écrit un texte dans un fichier.
 * 
 * @param text texte à écrire.
 * @param textLength longueur du texte à écrire.
 * @param fileName nom du fichier dans lequel écrire le texte.
 */
void writeTextToFile(char* text, int textLength, char* fileName) {
    // Ouverture du fichier en écriture.
    FILE* file = fopen(fileName, "w");
    // Test d'erreur. Si l'ouverture a échoué, on affiche un message d'erreur et
    // on désalloue la mémoire du texte et on quitte.
    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier %s en écriture. Abandon.\n", fileName);
        free(text);
        exit(1);
    }
    // Ecriture du texte dans le fichier.
    fwrite(text, sizeof(char), (long unsigned int) textLength, file);
    // Fermeture du fichier.
    fclose(file);
}

/**
 * Fonction principale.
 * 
 * @param argc nombre d'arguments.
 * @param argv arguments.
 * @return code de retour.
 */
int main(int argc, char** argv) {
    // Test du nombre d'arguments. Si le nombre d'arguments est incorrect, on
    // affiche un message d'erreur et on quitte.
    if (argc != 4) {
        fprintf(stderr, "Usage: %s : Générateur de texte aléatoire de longueur \
                argv[1] sur un alphabet de taille argv[2] dans le fichier \
                argv[3].\n", argv[0]);
        exit(1);
    }

    // Récupération des arguments.
    int textLength = atoi(argv[1]);
    int alphabetSize = atoi(argv[2]);
    char* fileName = argv[3];
    // Tests d'erreur. Si les arguments sont incorrects, on affiche un message
    // d'erreur et on quitte.
    if (textLength > MAX_TEXT_LENGTH) {
        fprintf(stderr, "La longueur du texte ne doit pas dépasser %d. Abandon.\
                \n", MAX_TEXT_LENGTH);
        exit(1);
    }
    if (alphabetSize > MAX_ALPHABET_SIZE) {
        fprintf(stderr, "La taille de l'alphabet ne doit pas dépasser %d. \
                Abandon.\n", MAX_ALPHABET_SIZE);
        exit(1);
    }
    // Génération du texte.
    char* text = generateText(textLength, alphabetSize);
    // Test d'erreur. Si la génération a échoué, on affiche un message d'erreur
    // et on quitte.
    if (text == NULL) {
        fprintf(stderr, "Erreur lors de la génération du texte. Abandon.\n");
        exit(1);
    }
    // Ecriture du texte dans le fichier.
    writeTextToFile(text, textLength, fileName);
    // Désallocation de la mémoire du texte.
    free(text);
    // Succès, on quitte.
    return 0;
}