/**
 * FICHIER:
 * --------
 * ac-hachage.c
 * 
 * DESCRIPTION:
 * ------------
 * Programme implémentant l'algorithme d'Aho-Corasick pour compter le nombre
 * d'occurrences d'un ensemble de mots dans un texte.
 * L'algorithme est implémenté avec une table de hachage.
 * Les mots et le texte sont lus depuis des fichiers dont les noms sont passés
 * en arguments.
 * 
 * AUTEUR:
 * -------
 * M. TAIA
 */


////////////////////////////////////////////////////////////////////////////////
// INCLUDES
////////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <string.h>


////////////////////////////////////////////////////////////////////////////////
// DEFINES
////////////////////////////////////////////////////////////////////////////////

// Taille de l'alphabet égal à UCHAR_MAX
#define ALPHABET_SIZE UCHAR_MAX

// Taux de remplissage
#define FILL_RATE 0.75

#define QUEUE_SIZE 1000000


////////////////////////////////////////////////////////////////////////////////
// STRUCTURES
////////////////////////////////////////////////////////////////////////////////

// Liste de table de hachage
struct _list {
    int startNode; // État de départ de la transition
    int targetNode; // Cible de la transition
    unsigned char letter; // Etiquette de la transition
    struct _list *next; // Maillon suivant
};

typedef struct _list *List;

// Trie à table de hachage
struct _trie {
    int maxNode; // Nombre maximal de noeuds du trie
    int nextNode; // Indice du prochain noeud disponible
    List *transition; // listes d'adjacence
    char *finite; // États terminaux
};

typedef struct _trie *Trie;


////////////////////////////////////////////////////////////////////////////////
// TRIE A TABLE DE HACHAGE
////////////////////////////////////////////////////////////////////////////////

int length(unsigned char *word) {
    int i = 0;
    while (word[i] != '\0') {
        i++;
    }
    return i;
}

// Fonction de hashage avec nombres premiers
int hash(unsigned char letter, unsigned char *word, int maxNode) {
    int i = 0;
    int hash = letter;
    while (word[i] != '\0') {
        hash = (hash * 31 + word[i]) % maxNode;
        i++;
    }
    return hash;
}

/**
 * DESCRIPTION:
 *  Création d'un trie à table de hachage avec un taux de remplissage de FILL_RATE
 * ARGUMENTS :
 *  int maxNode : nombre maximal de noeuds du trie.
 * RETURN : 
 *  Un trie à table de hachage en cas de succès, NULL sinon.
 */
Trie createTrie(int maxNode) {
    // Création du trie
    // Allouer la mémoire pour le trie
    Trie trie = malloc(sizeof(struct _trie));
    // Test d'erreur
    if (trie == NULL) {
        perror("Erreur d'allocation mémoire");
        return NULL;
    }

    // Initialisation du trie
    // Nombre maximal de noeuds du trie
    trie->maxNode = maxNode;
    // Indice du prochain noeud disponible
    trie->nextNode = 0;
    // Table de hachage
    trie->transition = malloc((size_t) (maxNode * FILL_RATE * sizeof(List)));
    // Test d'erreur
    if (trie->transition == NULL) {
        perror("Erreur d'allocation mémoire");
        return NULL;
    }
    // Initialiser la liste de table de hachage avec NULL
    memset(trie->transition, 0, (size_t) (maxNode * FILL_RATE * sizeof(List)));
    // États terminaux
    trie->finite = malloc((size_t) (maxNode * FILL_RATE * sizeof(char)));
    // Test d'erreur
    if (trie->finite == NULL) {
        perror("Erreur d'allocation mémoire");
        return NULL;
    }
    // Initialiser les états terminaux avec 0
    memset(trie->finite, 0, (size_t) (maxNode * FILL_RATE * sizeof(char)));

    // Initialiser le champ supplement avec NULL
    trie->supplement = malloc((size_t) (maxNode * FILL_RATE * sizeof(int)));
    // Test d'erreur
    if (trie->supplement == NULL) {
        perror("Erreur d'allocation mémoire");
        return NULL;
    }
    // Initialiser les suppléants avec -1
    memset(trie->supplement, -1, (size_t) (maxNode * FILL_RATE * sizeof(int)));


    return trie;

}

/**
 * DESCRIPTION:
 *  Ajout d'un mot dans un trie.
 * ARGUMENTS:
 *  Trie trie : un trie.
 *  unsigned char *w : un mot.
 * RETURN:
 *  0 en cas de succès, -2 si le trie est plein, -1 sinon.
 */
int insertInTrie(Trie trie, unsigned char *word) {

    int i = 0;
    int currentNode = hash(word[i], word, (int)(trie->maxNode * FILL_RATE));
    int nextNode = 0;
    List currentList = NULL;
    List newList = NULL;
    // Parcours du mot
    while (word[i] != '\0') {
        // On récupère la liste d'adjacence du noeud courant
        currentList = trie->transition[currentNode];
        // On parcourt la liste d'adjacence
        while (currentList != NULL) {
            // Si la lettre de la transition est la même que la lettre du mot:
            //  pas de création de noeud et on passe à la lettre suivante.
            if (currentList->letter == word[i]) {
                currentNode = currentList->targetNode;
                break;
            }
            // Sinon, on passe à la transition suivante
            currentList = currentList->next;
        }
        // Si on a parcouru toute la liste d'adjacence sans trouver de
        //  transition avec la lettre du mot, on crée un nouveau noeud.
        if (currentList == NULL) {
            // Création d'un nouveau noeud
            nextNode = trie->nextNode;
            // On incrémente l'indice du prochain noeud disponible
            trie->nextNode++;
            // Création d'une nouvelle transition
            newList = malloc(sizeof(struct _list));
            // Test d'erreur
            if (newList == NULL) {
                perror("Erreur d'allocation mémoire");
                return -1;
            }
            // Initialisation de la nouvelle transition
            newList->startNode = currentNode;
            newList->targetNode = nextNode;
            newList->letter = word[i];
            newList->next = trie->transition[currentNode];
            trie->transition[currentNode] = newList;
            currentNode = nextNode;
        }
        i++;
    }
    // On marque le noeud courant comme étant terminal
    trie->finite[currentNode] = 1;

    return 0;
}

/**
 * DESCRIPTION:
 *  Recherche d'un mot dans un trie.
 * ARGUMENTS:
 *  Trie trie : un trie.
 *  unsigned char *w : un mot.
 * RETURN:
 *  1 si le mot est présent dans le trie, 0 sinon.
 */
int searchWord(Trie trie, unsigned char *word) {
    int i = 0;
    int currentNode = hash(word[i], word, (int)(trie->maxNode * FILL_RATE));
    List currentList = NULL;
    // Parcours du mot
    while (word[i] != '\0') {
        // On récupère la liste d'adjacence du noeud courant
        currentList = trie->transition[currentNode];
        // On parcourt la liste d'adjacence
        while (currentList != NULL) {
            // Si la lettre de la transition est la même que la lettre du mot:
            //  pas de création de noeud et on passe à la lettre suivante.
            if (currentList->letter == word[i]) {
                currentNode = currentList->targetNode;
                break;
            }
            // Sinon, on passe à la transition suivante
            currentList = currentList->next;
        }
        // Si on a parcouru toute la liste d'adjacence sans trouver de
        //  transition avec la lettre du mot, on retourne 0.
        if (currentList == NULL) {
            return 0;
        }
        i++;
    }
    // Si le noeud courant est terminal, on retourne 1.
    return trie->finite[currentNode];
}

/**
 * DESCRIPTION:
 *  Libération de la mémoire allouée pour un trie.
 * ARGUMENTS:
 *  Trie trie : un trie.
 * RETURN:
 *  void.
 */
void freeTrie(Trie trie) {
    List currentList = NULL;
    List nextList = NULL;
    for (int i = 0; i < trie->maxNode*FILL_RATE; i++) {
        currentList = trie->transition[i];
        while (currentList != NULL) {
            nextList = currentList->next;
            free(currentList);
            currentList = nextList;
        }
        trie->transition[i] = NULL;
    }

    free(trie->transition);
    free(trie->finite);
    free(trie);
}

/**
 * DESCRIPTION:
 *  Affichage d'un trie.
 * ARGUMENTS:
 *  Trie trie : un trie.
 * RETURN:
 *  void.
 */
void printTrie(Trie trie) {
    List currentList = NULL;
    for (int i = 0; i < trie->nextNode; i++) {
        currentList = trie->transition[i];
        while (currentList != NULL) {
            printf("%d -> %d : %c, %d\n", currentList->startNode, \
                currentList->targetNode, currentList->letter, \
                trie->finite[currentList->targetNode]);
            currentList = currentList->next;
        }
    }
}

/**
 * DESCRIPTION:
 *  Extraction de préfixe.
 * ARGUMENTS:
 *  unsigned char *word: un mot.
 *  int n : la longueur du préfixe.
 * RETURN:
 *  Le préfixe du mot. 
 *  NULL en cas d'erreur.
 */
unsigned char *prefix(unsigned char *word, int n) {
    // Si la longueur du mot est inférieure à n, on retourne le mot.
    if (length(word) < n) {
        return word;
    }

    // Allocation mémoire pour le préfixe
    unsigned char *prefix = malloc((long unsigned int) (n + 1) * sizeof(unsigned char));
    // Test d'erreur
    if (prefix == NULL) {
        perror("Erreur d'allocation mémoire");
        return NULL;
    }
    // Copie du préfixe
    for (int i = 0; i < n; i++) {
        prefix[i] = word[i];
    }
    // Ajout du caractère de fin de chaîne
    prefix[n] = '\0';

    return prefix;
}

/**
 * DESCRIPTION:
 *  Extraction de suffixe d'un mot.
 * ARGUMENTS:
 *  unsigned char *word : un mot.
 *  int n : la longueur du suffixe.
 * RETURN:
 *  Le suffixe du mot.
 *  NULL en cas d'erreur.
 */
unsigned char *suffix(unsigned char *word, int n) {
    // Si la longueur du mot est inférieure à n, on retourne le mot.
    if (length(word) < n) {
        return word;
    }

    // Allocation mémoire pour le suffixe
    unsigned char *suffix = malloc((long unsigned int) (n + 1) * sizeof(unsigned char));
    // Test d'erreur
    if (suffix == NULL) {
        perror("Erreur d'allocation mémoire");
        return NULL;
    }
    // Copie du suffixe
    int i = 0;
    while (word[i] != '\0') {
        i++;
    }
    for (int j = 0; j < n; j++) {
        suffix[j] = word[i - n + j];
    }
    suffix[n] = '\0';
    return suffix;
}

////////////////////////////////////////////////////////////////////////////////
// ALGORITHME
////////////////////////////////////////////////////////////////////////////////

