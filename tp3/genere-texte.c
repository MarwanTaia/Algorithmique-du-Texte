/**
 * FICHIER:
 * --------
 * genere_texte.c
 * 
 * DESCRIPTION:
 * ------------
 * Programme de générateur pseudo-aléatoire de texte, permettant de stocker dans
 * des fichiers des textes d'une longueur donnée sur un alphabet de taille
 * donnée. Le texte est généré est écrit sur la sortie standard.
 * 
 * AUTEUR:
 * -------
 * M. TAIA
 */


////////////////////////////////////////////////////////////////////////////////
// INCLUDES
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


////////////////////////////////////////////////////////////////////////////////
// DEFINES
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// FONCTIONS
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


////////////////////////////////////////////////////////////////////////////////
// MAIN
////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv) {
    // Test du nombre d'arguments. Si le nombre d'arguments est incorrect, on
    // affiche un message d'erreur et on quitte.
    if (argc != 3) {
        fprintf(stderr, "Usage: %s textLength alphabetSize\n", argv[0]);
        exit(1);
    }

    // Récupération de la longueur du texte et de la taille de l'alphabet.
    int textLength = atoi(argv[1]);
    int alphabetSize = atoi(argv[2]);

    // Test d'erreur. Si la longueur du texte ou la taille de l'alphabet est
    // négative, trop petite, trop grande, ou nulle, on affiche un message
    // d'erreur et on quitte.
    if (textLength <= 0 || alphabetSize <= 0 || alphabetSize > 94) { // TODO: set 94 as macro
        fprintf(stderr, "Erreur: la longueur du texte et la taille de\
                l'alphabet doivent être positives et la taille de l'alphabet\
                doit être inférieure ou égale à 94.\n");
        exit(1);
    }

    // Génération du texte.
    char* text = generateText(textLength, alphabetSize);
    // Test d'erreur. Si la génération a échoué, on affiche un message d'erreur et on quitte.
    if (text == NULL) {
        fprintf(stderr, "Erreur lors de la génération du texte. Abandon.\n");
        exit(1);
    }

    // Écriture du texte sur la sortie standard.
    printf("%s\n", text);

    // Libération de la mémoire allouée pour le texte.
    free(text);

    return 0;
}
