/**
 * @file trieHT.c
 * @author M. TAIA
 * @version 0.2
 *   Ce fichier contient les structures et primitives permettant la création
 * et la manipulation de tries à table de hachage et table de transitions.
 * 
 */

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
// Si 1, exécuter la version avec table de transitions. Sinon, exécuter la
//  version avec table de hachage.
#define HASH 1

// Fonctions Outils ////////////////////////////////////////////////////////////
/**
 * DESCRIPTION:
 *  Chercher la longueur d'un mot.
 * ARGUMENTS:
 *  unsigned char *w : un mot.
 * RETURN:
 *  La longueur du mot.
 */
int length(unsigned char *word) {
    int i = 0;
    while (word[i] != '\0') {
        i++;
    }
    return i;
}

#if HASH == 0

// Structure d'un noeud du trie à table de transitions
struct _trie {
    int maxNode; // Nombre maximal de noeuds du trie
    int nextNode; // Indice du prochain noeud disponible
    int **transitions; // Table de transitions
    char *finite; // États terminaux
};

typedef struct _trie *Trie;

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
        for (int j = 0; j < ALPHABET_SIZE; j++) {
            trie->transitions[i][j] = -1;
        }
        trie->finite[i] = 0;
    }

    // Retour du trie
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
int addWord(Trie trie, unsigned char *word) {

    // Indice du noeud courant
    int currentNode = 0;

    // Parcours des caractères du mot
    for (int i = 0; word[i] != 0; i++) {
        // Si la transition n'existe pas, on la crée
        if (trie->transitions[currentNode][word[i]] == -1) {
            trie->transitions[currentNode][word[i]] = trie->nextNode;
            trie->nextNode++;
        }
        // On passe au noeud suivant
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

#else

////////////////////////////////////////////////////////////////////////////////
// Structures //////////////////////////////////////////////////////////////////

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
// Fonctions ///////////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////////////////////////
// Primitives //////////////////////////////////////////////////////////////////

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
    // États terminaux
    trie->finite = malloc((size_t) (maxNode * FILL_RATE * sizeof(char)));
    // Test d'erreur
    if (trie->finite == NULL) {
        perror("Erreur d'allocation mémoire");
        return NULL;
    }
    // Initialisation des états terminaux
    for (int i = 0; i < FILL_RATE * maxNode; i++) {
        trie->transition[i] = NULL;
        trie->finite[i] = 0;
    }

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

#endif

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
 *  Ajout des préfixes d'un mot dans un trie.
 * ARGUMENTS:
 *  Trie trie : un trie.
 *  unsigned char *word : un mot.
 * RETURN:
 *  0 en cas de succès, -1 en cas d'erreur.
 */
int addPrefixes(Trie trie, unsigned char *word) {
    int n = length(word);
    int err = 0;
    for (int i = 0; i < n + 1; i++) {
        err = insertInTrie(trie, prefix(word, i));
        if (err == -1) {
            perror("Erreur d'allocation mémoire");
            return -1;
        } else if (err == -2) {
            perror("Erreur de redimensionnement du trie");
            return -1;
        }
    }

    return 0;
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

/**
 * DESCRIPTION:
 *  Ajout des suffixes d'un mot dans un trie.
 * ARGUMENTS:
 *  Trie trie : un trie.
 *  unsigned char *word : un mot.
 * RETURN:
 *  0 en cas de succès, -1 en cas d'erreur.
 */
int addSuffixes(Trie trie, unsigned char *word) {
    int n = length(word);
    int err = 0;
    for (int i = 0; i < n + 1; i++) {
        err = insertInTrie(trie, suffix(word, i));
        // Tests d'erreur
        if (err == -1) {
            perror("Erreur d'allocation mémoire");
            return -1;
        } else if (err == -2) {
            perror("Erreur de redimensionnement du trie");
            return -1;
        }
    }

    return 0;
}

/**
 * DESCRIPTION:
 *  Fonction qui ajoute tous les facteurs d'un mot à un trie. Ceci en insérant
 *   successivement tous les suffixes du mot du plus long au plus court.
 * ARGUMENTS:
 *  Trie trie : un trie.
 *  unsigned char *word : un mot.
 * RETURN:
 *  0 en cas de succès, -1 en cas d'erreur.
 */
int addFactors(Trie trie, unsigned char *word) {
    int err = 0;
    int n = length(word);
    // pour chaque préfixe du mot, on ajoute tous les suffixes
    for (int i = n; i > 0; i--) {
        unsigned char *prefixWord = prefix(word, i);
        err = addSuffixes(trie, prefixWord);
        // Tests d'erreur
        if (err == -1) {
            perror("Erreur d'allocation mémoire");
            return -1;
        } else if (err == -2) {
            perror("Erreur de redimensionnement du trie");
            return -1;
        }
        free(prefixWord);
    }

    return 0;
}

int main(void) {
    // test du trie
    Trie trie = createTrie(100);
    unsigned char word1[] = "tests";
    unsigned char word2[] = "test2";
    unsigned char word3[] = "test3";
    unsigned char word4[] = "test4";

    insertInTrie(trie, word1);
    insertInTrie(trie, word2);
    insertInTrie(trie, word3);

    printf("Recherche de %s (1) : %d\n", word1, searchWord(trie, word1));
    printf("Recherche de %s (1) : %d\n", word2, searchWord(trie, word2));
    printf("Recherche de %s (1) : %d\n", word3, searchWord(trie, word3));
    printf("Recherche de %s (0) : %d\n", word4, searchWord(trie, word4));
    printTrie(trie);
    printf("___________________________\n\n");

    freeTrie(trie);

    // test du trie de préfixes
    trie = createTrie(100);
    addPrefixes(trie, word4);
    printTrie(trie);

    unsigned char word5[] = "test";
    unsigned char word6[] = "tes";
    unsigned char word7[] = "te";
    unsigned char word8[] = "t";

    printf("Recherche de %s : %d\n", word5, searchWord(trie, word5));
    printf("Recherche de %s : %d\n", word6, searchWord(trie, word6));
    printf("Recherche de %s : %d\n", word7, searchWord(trie, word7));
    printf("Recherche de %s : %d\n", word8, searchWord(trie, word8));

    freeTrie(trie);

    // test du trie de suffixes
    trie = createTrie(100);
    addSuffixes(trie, word5);
    printTrie(trie);

    unsigned char word15[] = "est";
    unsigned char word16[] = "st";
    unsigned char word17[] = "t";

    printf("Recherche de %s : %d\n", word5, searchWord(trie, word5));
    printf("Recherche de %s : %d\n", word15, searchWord(trie, word15));
    printf("Recherche de %s : %d\n", word16, searchWord(trie, word16));
    printf("Recherche de %s : %d\n", word17, searchWord(trie, word17));
    

    freeTrie(trie);

    // test du trie de facteurs
    trie = createTrie(100);
    unsigned char word9[] = "ABC";
    addFactors(trie, word9);
    printTrie(trie);

    unsigned char word10[] = "BC";
    unsigned char word11[] = "C";
    unsigned char word12[] = "AB";
    unsigned char word13[] = "B";
    unsigned char word14[] = "A";

    printf("Recherche de %s : %d\n", word9, searchWord(trie, word9));
    printf("Recherche de %s : %d\n", word10, searchWord(trie, word10));
    printf("Recherche de %s : %d\n", word11, searchWord(trie, word11));
    printf("Recherche de %s : %d\n", word12, searchWord(trie, word12));
    printf("Recherche de %s : %d\n", word13, searchWord(trie, word13));
    printf("Recherche de %s : %d\n", word14, searchWord(trie, word14));
}