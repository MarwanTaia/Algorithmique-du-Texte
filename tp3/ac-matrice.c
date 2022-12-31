// Inclusions //////////////////////////////////////////////////////////////////
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <string.h>


// Macros //////////////////////////////////////////////////////////////////////
// Taille de l'alphabet égal à UCHAR_MAX
#define ALPHABET_SIZE UCHAR_MAX
// Taux de remplissage
#define FILL_RATE 0.75


////////////////////////////////////////////////////////////////////////////////
// STRUCTURES
////////////////////////////////////////////////////////////////////////////////

// Structure d'un noeud du trie à table de transitions
struct _trie {
    int maxNode; // Nombre maximal de noeuds du trie
    int nextNode; // Indice du prochain noeud disponible
    int **transitions; // Table de transitions
    char *finite; // États terminaux
};

typedef struct _trie *Trie;


////////////////////////////////////////////////////////////////////////////////
// TRIE À TABLE DE TRANSITIONS
////////////////////////////////////////////////////////////////////////////////

Trie createTrie(int maxNode) {
    // Création d'un trie à table de transitions

    // Allocation de la structure
    Trie trie = malloc(sizeof(struct _trie));
    // Test d'erreur
    if (trie == NULL) {
        perror("Erreur d'allocation mémoire");
        return NULL;
    }

    // Initialisation des attributs
    // Nombre maximal de noeuds du trie
    trie->maxNode = maxNode;
    // Indice du prochain noeud disponible
    trie->nextNode = 0;
    // Table de transitions
    trie->transitions = malloc((long unsigned int) maxNode * sizeof(int *));
    // Test d'erreur
    if (trie->transitions == NULL) {
        perror("Erreur d'allocation mémoire");
        return NULL;
    }
    // États terminaux
    trie->finite = malloc((long unsigned int) maxNode * sizeof(char));
    // Test d'erreur
    if (trie->finite == NULL) {
        perror("Erreur d'allocation mémoire");
        return NULL;
    }

    // Initialisation de la table de transitions
    for (int i = 0; i < maxNode; i++) {
        trie->transitions[i] = malloc(ALPHABET_SIZE * sizeof(int));
        // Test d'erreur
        if (trie->transitions[i] == NULL) {
            perror("Erreur d'allocation mémoire");
            return NULL;
        }
        // Initialisation de la ligne de la table de transitions à l'aide de memset
        memset(trie->transitions[i], -1, ALPHABET_SIZE * sizeof(int));
        trie->finite[i] = 0;
    }

    return trie;
}

/**
 * DESCRIPTION:
 *  Ajout d'un mot dans un trie à table de transitions.
 * ARGUMENTS:
 *  Trie trie : un trie à table de transitions.
 *  unsigned char *word : un mot.
 * RETURN:
 *  0 si le mot a été ajouté, -2 si le trie est plein, -1 en cas d'erreur.
 */
int insertInTrie(Trie trie, unsigned char *word) {

    // Indice du noeud courant
    int currentNode = 0;

    // Parcours des caractères du mot jusqu'à la fin du préfixe
    int i;
    for (i = 0; word[i] != 0; i++) {
        // Si la transition n'existe pas, on la crée
        if (trie->transitions[currentNode][word[i]] == -1) {
            trie->transitions[currentNode][word[i]] = trie->nextNode;
            trie->nextNode++;
        }
        // On passe au noeud suivant
        currentNode = trie->transitions[currentNode][word[i]];
    }

    // Insertion du suffixe
    for (; word[i] != 0; i++) {
        trie->transitions[currentNode][word[i]] = trie->nextNode;
        trie->nextNode++;
        currentNode = trie->transitions[currentNode][word[i]];
    }

    // On marque le noeud comme terminal
    trie->finite[currentNode] = 1;

    return 0;

}

/**
 * DESCRIPTION:
 *  Recherche d'un mot dans un trie à table de transitions.
 * ARGUMENTS:
 *  Trie trie : un trie à table de transitions.
 *  unsigned char *w : un mot.
 * RETURN:
 *  1 si le mot est présent, 0 sinon.
 */
int searchWord(Trie trie, unsigned char *w) {
    // Recherche d'un mot dans un trie à table de transitions

    // Indice du noeud courant
    int currentNode = 0;

    // Parcours des caractères du mot
    for (int i = 0; w[i] != 0; i++) {
        // Si la transition n'existe pas, le mot n'est pas présent
        if (trie->transitions[currentNode][w[i]] == -1) {
            return 0;
        }
        // On passe au noeud suivant
        currentNode = trie->transitions[currentNode][w[i]];
    }

    // On retourne si le mot est présent ou non
    return trie->finite[currentNode];
}

/**
 * DESCRIPTION:
 *  Libération de la mémoire d'un trie à table de transitions.
 * ARGUMENTS:
 *  Trie trie : un trie à table de transitions.
 * RETURN:
 *  0 si le trie a été libéré.
 */
int freeTrie(Trie trie) {
    // Libération de la mémoire d'un trie à table de transitions
    
    // Libération de la mémoire des transitions
    for (int i = 0; i < trie->maxNode; i++) {
        free(trie->transitions[i]);
    }
    free(trie->transitions);

    // Libération de la mémoire des états terminaux
    free(trie->finite);

    // Libération de la mémoire du trie
    free(trie);

    // Retour succès
    return 0;
}

/**
 * DESCRIPTION:
 *  Affichage d'un trie à table de transitions.
 * ARGUMENTS:
 *  Trie trie : un trie à table de transitions.
 * RETURN:
 *  Aucun
 */
void printTrie(Trie trie) {
    // Affichage d'un trie à table de transitions sous forme d'un automate
    printf("Graph G { rankdir=LR\n");
    printf("\tnode [shape = doublecircle]: ");
    for (int i = 0; i < trie->maxNode; i++) {
        if (trie->finite[i] == 1) {
            printf(" %d ", i);
        }
    }
    printf(";\n");
    printf("\tnode [shape = circle]:\n");
    int found = 0;
    for (int i = 0; i < trie->maxNode; i++) {
        for (int j = 0; j < ALPHABET_SIZE; j++) {
            if (trie->transitions[i][j] != -1) {
                found = 1;
                printf("\t\t%d -> %d [label = \"%c\"];", i, trie->transitions[i][j], j);
            }
        }
        if (found == 1) {
            printf("\n");
        }
        found = 0;
    }

    printf("}\n");
}


////////////////////////////////////////////////////////////////////////////////
// AHO-CORASICK
////////////////////////////////////////////////////////////////////////////////


