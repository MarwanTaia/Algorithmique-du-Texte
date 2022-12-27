#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/**
 * Fonction qui génère un mot aléatoire de longueur aléatoire comprise entre
 * une longueur minimale et maximale sur un alphabet de taille donnée.
 * Renvoie le mot généré, en cas de succès, NULL sinon.
 * 
 * @param minWordLength longueur minimale du mot à générer.
 * @param maxWordLength longueur maximale du mot à générer.
 * @param alphabetSize taille de l'alphabet sur lequel générer le mot.
 * @return le mot généré.
 */
char* generateWord(int minWordLength, int maxWordLength, int alphabetSize) {
    // Initialisation de la fonction de génération de nombres aléatoires.
    srand(time(NULL));

    // Allocation de la mémoire pour le mot.
    int wordLength = rand() % (maxWordLength - minWordLength + 1) + minWordLength;
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

int main(int argc, char** argv) {
    // Test du nombre d'arguments. Si le nombre d'arguments est incorrect, on
    // affiche un message d'erreur et on quitte.
    if (argc != 4) {
        fprintf(stderr, "Usage: %s wordCount minWordLength maxWordLength alphabetSize\n", argv[0]);
        exit(1);
    }

    // Initialisation du générateur de nombres aléatoires.
    srand(time(NULL));

    // Récupération du nombre de mots à générer, de la longueur minimale et
    // maximale des mots et de la taille de l'alphabet.
    int wordCount = atoi(argv[1]);
    int minWordLength = atoi(argv[2]);
    int maxWordLength = atoi(argv[3]);
    int alphabetSize = atoi(argv[4]);

    // Vérification de la validité des paramètres.
    if (wordCount <= 0 || minWordLength < 1 || maxWordLength < minWordLength || alphabetSize < 2) {
        fprintf(stderr, "Paramètres non valides.\n");
        exit(1);
    }

    // Génération de la liste de mots.
    for (int i = 0; i < wordCount; i++) {
        // Génération d'un mot aléatoire de longueur aléatoire.
        int wordLength = rand() % (maxWordLength - minWordLength + 1) + minWordLength;
        char* word = generateWord(wordLength, alphabetSize);
        // Test d'erreur. Si la génération d'un mot a échoué, on quitte.
        if (word == NULL) {
            fprintf(stderr, "Erreur lors de la génération d'un mot.\n");
            exit(1);
        }

        // Affichage du mot sur la sortie standard.
        printf("%s\n", word);

        // Désallocation de la mémoire du mot.
        free(word);
    }

    // Succès, on quitte.
    return 0;
}