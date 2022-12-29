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
    int failNode; // Noeud de suppléant
    unsigned char letter; // Etiquette de la transition
    struct _list *next; // Maillon suivant
};


typedef struct _list *List;

// Trie à table de hachage
struct _trie {
    int maxNode; // Nombre maximal de noeuds du trie
    int nextNode; // Indice du prochain noeud disponible
    List *transition; // listes d'adjacence
    int *failNode; // Noeuds de suppléant
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

    // Table de hachage des noeuds de suppléant
    trie->failNode = malloc((size_t) (maxNode * FILL_RATE * sizeof(int)));
    // Test d'erreur
    if (trie->failNode == NULL) {
        perror("Erreur d'allocation mémoire");
        return NULL;
    }
    // Initialiser la table de hachage des noeuds de suppléant avec -1
    memset(trie->failNode, -1, (size_t) (maxNode * FILL_RATE * sizeof(int)));
    
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

// Structure de file
struct _queue {
    int *elements; // Éléments de la file
    int head; // Indice du premier élément de la file
    int tail; // Indice du dernier élément de la file
    int size; // Taille de la file
    int capacity; // Capacité de la file
};

typedef struct _queue *Queue;

/**
 * DESCRIPTION:
 *  Enqueue un élément dans une file
 * ARGUMENTS:
 *  Queue queue : une file
 *  int element : un élément à ajouter à la file
 * RETURN:
 *  0 en cas de succès, -1 sinon
 */
int enqueue(Queue queue, int element) {
    // Si la file est pleine
    if (queue->size == queue->capacity) {
        // Erreur
        return -1;
    }
    // Ajouter l'élément à la fin de la file
    queue->elements[queue->tail] = element;
    // Mettre à jour la taille de la file
    queue->size++;
    // Mettre à jour la fin de la file
    queue->tail = (queue->tail + 1) % queue->capacity;
    // Succès
    return 0;
}

/**
 * DESCRIPTION:
 *  Dequeue un élément d'une file
 * ARGUMENTS:
 *  Queue queue : une file
 * RETURN:
 *  L'élément dequeue de la file en cas de succès, -1 sinon
 */
int dequeue(Queue queue) {
    // Si la file est vide
    if (queue->size == 0) {
        // Erreur
        return -1;
    }
    // Récupérer l'élément à la tête de la file
    int element = queue->elements[queue->head];
    // Mettre à jour la tête de la file
    queue->head = (queue->head + 1) % queue->capacity;
    // Mettre à jour la taille de la file
    queue->size--;
    // Retourner l'élément dequeue
    return element;
}

/**
 * DESCRIPTION:
 *  Initialise une file avec une capacité donnée
 * ARGUMENTS:
 *  int capacity : capacité de la file
 * RETURN:
 *  Un pointeur vers la file initialisée en cas de succès, NULL sinon
 */
Queue makeQueue(int capacity) {
    // Allouer la mémoire pour la file
    Queue queue = malloc(sizeof(struct _queue));
    // Test d'erreur
    if (queue == NULL) {
        perror("Erreur d'allocation mémoire");
        return NULL;
    }
    // Allouer la mémoire pour les éléments de la file
    queue->elements = malloc((long unsigned int) capacity * sizeof(int));
    // Test d'erreur
    if (queue->elements == NULL) {
        perror("Erreur d'allocation mémoire");
        return NULL;
    }
    // Initialiser la tête de la file
    queue->head = 0;
    // Initialiser la queue de la file
    queue->tail = 0;
    // Initialiser la taille de la file
    queue->size = 0;
    // Enregistrer la capacité de la file
    queue->capacity = capacity;
    // Retourner la file
    return queue;
}

/**
 * DESCRIPTION:
 *  Enfile une file vide pour le parcours en largeur d'un trie
 * ARGUMENTS:
 *  Trie trie : un trie
 *  Queue queue : une file
 * RETURN:
 *  0 en cas de succès, -1 sinon
 */
int enqueueEmptyQueue(Trie trie, Queue queue) {
    // Pour chaque noeud du trie
    for (int i = 0; i < trie->nextNode; i++) {
        // Si le noeud est le noeud racine
        if (trie->failNode[i] == -1) {
            // Enqueue le noeud dans la file
            int success = enqueue(queue, i);
            // Test d'erreur
            if (success == -1) {
                perror("Erreur d'ajout à la file");
                return -1;
            }
        }
    }
    // Succès
    return 0;
}



void calcSupp(Trie trie, Queue queue) {
    // Initialiser les liens de suppléant de la racine du trie à 0
    trie->failNode[0] = 0;
    printf("\n\nhere\n\n");
    // Tant que la queue n'est pas vide
    while (queue->size != 0) {
        // Récupérer le noeud courant depuis la queue
        int currentNode = dequeue(queue);
        // Récupérer la liste d'adjacence du noeud courant
        List currentList = trie->transition[currentNode];
        // Parcourir la liste d'adjacence
        while (currentList != NULL) {
            // Récupérer le noeud cible de la transition courante
            int targetNode = currentList->targetNode;
            // Enqueue le noeud cible dans la queue
            enqueue(queue, targetNode);
            // Récupérer le noeud de suppléant du noeud courant
            int failNode = trie->failNode[currentNode];
            // Tant que le noeud de suppléant du noeud courant n'a pas de transition
            // correspondant à l'étiquette de la transition courante
            while (trie->transition[failNode] == NULL || trie->transition[failNode]->letter != currentList->letter) {
                // Passer au noeud de suppléant du noeud de suppléant courant
                failNode = trie->failNode[failNode];
            }
            // Récupérer le noeud cible de la transition correspondant à l'étiquette de la transition courante
            // depuis le noeud de suppléant du noeud courant
            int failTarget = trie->transition[failNode]->targetNode;
            // Mettre à jour le lien de suppléant du noeud cible de la transition courante
            trie->failNode[targetNode] = failTarget;
            // Passer à la transition suivante
            currentList = currentList->next;
        }
    }
}


/**
 * DESCRIPTION:
 *  Compte les occurences de mots dans un trie dans un texte donné
 * ARGUMENTS:
 *  Trie trie : un trie
 *  unsigned char *text : un texte
 * RETURN:
 *  Le nombre total d'occurences de mots du trie dans le texte
 */
int countWordsInText(Trie trie, unsigned char *text) {
    // Initialiser le compteur d'occurences
    int count = 0;
    // Initialiser le noeud courant
    int currentNode = 0;
    // Pour chaque lettre du texte
    for (int i = 0; text[i] != '\0'; i++) {
        // Récupérer la liste d'adjacence du noeud courant
        List currentList = trie->transition[currentNode];
        // Initialiser le prochain noeud à -1 (pas de transition)
        int nextNode = -1;
        // Parcourir la liste d'adjacence
        while (currentList != NULL) {
            // Si la lettre de la transition correspond à la lettre du texte
            if (currentList->letter == text[i]) {
                // Mettre à jour le prochain noeud
                nextNode = currentList->targetNode;
                // Si le noeud cible est terminal
                if (trie->finite[nextNode]) {
                    // Incrémenter le compteur d'occurences
                    count++;
                }
                break;
            }
            // Passer au maillon suivant
            currentList = currentList->next;
        }
        // Si aucune transition n'a été trouvée
        if (nextNode == -1) {
            // Utiliser le lien de suppléant
            nextNode = trie->failNode[currentNode];
        }
        // Mettre à jour le noeud courant
        currentNode = nextNode;
    }
    // Retourner le compteur d'occurences
    return count;
}


int main(int argc, char **argv) {
    // Vérifier le nombre d'arguments
    if (argc != 3) {
        fprintf(stderr, "Usage: %s WORD_FILE TEXT_FILE\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Ouvrir le fichier de mots
    FILE *wordFile = fopen(argv[1], "r");
    // Test d'erreur
    if (wordFile == NULL) {
        perror("Erreur d'ouverture du fichier de mots");
        return EXIT_FAILURE;
    }

    // Lire le nombre de mots dans le fichier
    int numWords;
    int success = fscanf(wordFile, "%d", &numWords);
    if (success == -1) {
        perror("Erreur de lecture du nombre de mots");
        return EXIT_FAILURE;
    }

    // Créer le trie
    Trie trie = createTrie(10000);
    // Test d'erreur
    if (trie == NULL) {
        return EXIT_FAILURE;
    }

    // Lire les mots du fichier et les ajouter au trie
    unsigned char word[1000];
    for (int i = 0; i < numWords; i++) {
        int success = fscanf(wordFile, "%s", word);
        if (success == -1) {
            perror("Erreur de lecture d'un mot");
            return EXIT_FAILURE;
        }
        success = insertInTrie(trie, word);
        // Test d'erreur
        if (success == -1) {
            return EXIT_FAILURE;
        }
    }
    printTrie(trie);

    // Fermer le fichier de mots
    fclose(wordFile);

    // Créer la file pour le parcours en largeur
    Queue queue = makeQueue(numWords);
    // Test d'erreur
    if (queue == NULL) {
        return EXIT_FAILURE;
    }

    // Calculer les liens de suppléant
    calcSupp(trie, queue);
    printf("\n\n\n\n");
    printTrie(trie);

    // Ouvrir le fichier de texte
    FILE *textFile = fopen(argv[2], "r");
    // Test d'erreur
    if (textFile == NULL) {
        perror("Erreur d'ouverture du fichier de texte");
        return EXIT_FAILURE;
    }

    // Lire le texte du fichier
    unsigned char text[1000];
    success = fscanf(textFile, "%s", text);
    if (success == EOF) {
        fprintf(stderr, "Erreur de lecture du texte\n");
        return EXIT_FAILURE;
    }

    // Fermer le fichier de texte
    fclose(textFile);

    // Compter le nombre d'occurences de mots du trie dans le texte
    int count = countWordsInText(trie, text);

    // Afficher le résultat
    printf("Nombre d'occurences de mots du trie dans le texte: %d\n", count);

    // Libérer la mémoire
    freeTrie(trie);

    return EXIT_SUCCESS;
}