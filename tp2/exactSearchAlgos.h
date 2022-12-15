/**
 * FICHIER:
 * --------
 * exactSearchAlgos.h
 * 
 * DESCRIPTION:
 * ------------
 * Ce fichier contient les algorithmes de recherche exacte d'un mot dans un texte.
 * 
 * ALGORITHMES:
 * ------------
 * 1. Naif, avec boucle interne, sans boucle rapide, sans sentinelle.
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
// Fonctions
////////////////////////////////////////////////////////////////////////////////
/**
 * Fonction qui recherche un mot dans un texte, en utilisant l'algorithme naif,
 * avec boucle interne, sans boucle rapide, sans sentinelle.
 * Renvoie l'indice du premier caractère du mot dans le texte, si le mot est
 * trouvé, -1 sinon.
 * 
 * @param text texte dans lequel rechercher le mot.
 * @param word mot à rechercher dans le texte.
 * @param textLength longueur du texte.
 * @param wordLength longueur du mot.
 * @return l'indice du premier caractère du mot dans le texte, si le mot est
 *        trouvé, -1 sinon, ou -2 en cas d'erreur.
*/
int naif(char* text, char* word, int textLength, int wordLength) {
    // Test d'erreur. Si le texte ou le mot est NULL, on renvoie -2.
    if (text == NULL || word == NULL) {
        return -2;
    }

    // Test d'erreur. Si le mot est plus long que le texte, on renvoie -1.
    if (wordLength > textLength) {
        return -1;
    }

    // Recherche du mot dans le texte.
    for (int i = 0; i < textLength - wordLength + 1; i++) {
        // Boucle interne.
        for (int j = 0; j < wordLength; j++) {
            // Si le caractère du texte est différent du caractère du mot, on
            // sort de la boucle interne.
            if (text[i + j] != word[j]) {
                break;
            }
            // Si on est arrivé à la fin du mot, on renvoie l'indice du premier
            // caractère du mot dans le texte.
            if (j == wordLength - 1) {
                return i;
            }
        }
    }

    // Le mot n'a pas été trouvé, on renvoie -1.
    return -1;
}

/**
 * Fonction qui recherche un mot dans un texte, en utilisant l'algorithme naif,
 * avec boucle interne, avec boucle rapide, sans sentinelle.
 * Renvoie l'indice du premier caractère du mot dans le texte, si le mot est
 * trouvé, -1 sinon.
 * 
 * @param text texte dans lequel rechercher le mot.
 * @param word mot à rechercher dans le texte.
 * @param textLength longueur du texte.
 * @param wordLength longueur du mot.
 * @return l'indice du premier caractère du mot dans le texte, si le mot est
 *       trouvé, -1 sinon, ou -2 en cas d'erreur.
 */
int naifBoucleRapide(char* text, char* word, int textLength, int wordLength) {
    // Test d'erreur. Si le texte ou le mot est NULL, on renvoie -2.
    if (text == NULL || word == NULL) {
        return -2;
    }

    // Test d'erreur. Si le mot est plus long que le texte, on renvoie -1.
    if (wordLength > textLength) {
        return -1;
    }

    // Recherche du mot dans le texte (boucle rapide).
    // On stocke le dernier caractère du mot dans une variable.
    char lastChar = word[wordLength - 1];
    for (int i = 0; i < textLength - wordLength + 1; i++) {
        // Si le dernier caractère du mot est différent du dernier caractère du
        // texte, on passe au caractère suivant du texte.
        if (lastChar != text[i + wordLength - 1]) {
            continue;
        }

        // Boucle interne.
        for (int j = 0; j < wordLength; j++) {
            // Si le caractère du texte est différent du caractère du mot, on
            // sort de la boucle interne.
            if (text[i + j] != word[j]) {
                break;
            }
            // Si on est arrivé à la fin du mot, on renvoie l'indice du premier
            // caractère du mot dans le texte.
            if (j == wordLength - 1) {
                return i;
            }
        }
    }

    // Le mot n'a pas été trouvé, on renvoie -1.
    return -1;
}

/**
 * Fonction qui recherche un mot dans un texte, en utilisant l'algorithme naif,
 * avec boucle interne, avec boucle rapide, avec sentinelle.
 * Renvoie l'indice du premier caractère du mot dans le texte, si le mot est
 * trouvé, -1 sinon.
 * 
 * @param text texte dans lequel rechercher le mot.
 * @param word mot à rechercher dans le texte.
 * @param textLength longueur du texte.
 * @param wordLength longueur du mot.
 * @return l'indice du premier caractère du mot dans le texte, si le mot est
 *       trouvé, -1 sinon, ou -2 en cas d'erreur.
 */
int naifSentinelle(char* text, char* word, int textLength, int wordLength) {
    // Test d'erreur. Si le texte ou le mot est NULL, on renvoie -2.
    if (text == NULL || word == NULL) {
        return -2;
    }

    // Test d'erreur. Si le mot est plus long que le texte, on renvoie -1.
    if (wordLength > textLength) {
        return -1;
    }

    // Ajout de la sentinelle.
    text[textLength] = word[wordLength - 1];

    // Recherche du mot dans le texte (boucle rapide).
    // On stocke le dernier caractère du mot dans une variable.
    char lastChar = word[wordLength - 1];
    for (int i = 0; i < textLength - wordLength + 1; i++) {
        // Si le dernier caractère du mot est différent du dernier caractère du
        // texte, on passe au caractère suivant du texte.
        if (lastChar != text[i + wordLength - 1]) {
            continue;
        }

        // Boucle interne.
        for (int j = 0; j < wordLength; j++) {
            // Si le caractère du texte est différent du caractère du mot, on
            // sort de la boucle interne.
            if (text[i + j] != word[j]) {
                break;
            }
            // Si on est arrivé à la fin du mot, on renvoie l'indice du premier
            // caractère du mot dans le texte.
            if (j == wordLength - 1) {
                return i;
            }
        }
    }

    // Le mot n'a pas été trouvé, on renvoie -1.
    return -1;
}

////////////////////////////////////////////////////////////////////////////////
/**
 * Fonction qui recherche un mot dans un texte, en utilisant l'algorithme naif,
 * avec boucle interne, sans boucle rapide, sans sentinelle.
 * 
 * Cet algorithme fonctionne en utilisant strncmp().
 * 
 * Renvoie l'indice du premier caractère du mot dans le texte, si le mot est
 * trouvé, -1 sinon.
 * 
 * @param text texte dans lequel rechercher le mot.
 * @param word mot à rechercher dans le texte.
 * @param textLength longueur du texte.
 * @param wordLength longueur du mot.
 * @return l'indice du premier caractère du mot dans le texte, si le mot est
 *        trouvé, -1 sinon, ou -2 en cas d'erreur.
*/
int naifStrncmp(char* text, char* word, int textLength, int wordLength) {
    // Test d'erreur. Si le texte ou le mot est NULL, on renvoie -2.
    if (text == NULL || word == NULL) {
        return -2;
    }

    // Test d'erreur. Si le mot est plus long que le texte, on renvoie -1.
    if (wordLength > textLength) {
        return -1;
    }

    // Recherche du mot dans le texte.
    for (int i = 0; i < textLength - wordLength + 1; i++) {
        // Si le mot n'est pas trouvé à l'indice i, on saute le nombre de
        // caractères correspondant à la longueur du mot.
        if (strncmp(text + i, word, (size_t) wordLength) != 0) {
            i += 1;
        } else {
            // Le mot a été trouvé, on renvoie l'indice du premier caractère du
            // mot dans le texte.
            return i;
        }
    }

    // Le mot n'a pas été trouvé, on renvoie -1.
    return -1;
}

/**
 * Fonction qui recherche un mot dans un texte, en utilisant l'algorithme naif,
 * avec boucle interne, avec boucle rapide, sans sentinelle.
 * 
 * Cet algorithme fonctionne en utilisant strncmp().
 * 
 * Renvoie l'indice du premier caractère du mot dans le texte, si le mot est
 * trouvé, -1 sinon.
 * 
 * @param text texte dans lequel rechercher le mot.
 * @param word mot à rechercher dans le texte.
 * @param textLength longueur du texte.
 * @param wordLength longueur du mot.
 * @return l'indice du premier caractère du mot dans le texte, si le mot est
 *        trouvé, -1 sinon, ou -2 en cas d'erreur.
*/
int naifRapideStrncmp(char* text, char* word, int textLength, int wordLength) {
    // Test d'erreur. Si le texte ou le mot est NULL, on renvoie -2.
    if (text == NULL || word == NULL) {
        return -2;
    }

    // Test d'erreur. Si le mot est plus long que le texte, on renvoie -1.
    if (wordLength > textLength) {
        return -1;
    }

    // Recherche du mot dans le texte (boucle rapide).
    // On stocke le dernier caractère du mot dans une variable.
    char lastChar = word[wordLength - 1];
    // On utilise strncmp pour les comparaisons
    for (int i = 0; i < textLength - wordLength + 1; i++) {
        // Si le dernier caractère du mot est différent du dernier caractère du
        // texte, on passe au caractère suivant du texte.
        if (lastChar != text[i + wordLength - 1]) {
            continue;
        }

        // Boucle interne (comparaisons avec strncmp)
        if (strncmp(text + i, word, (size_t) wordLength) != 0) {
            i += 1;
        } else {
            // Le mot a été trouvé, on renvoie l'indice du premier caractère du
            // mot dans le texte.
            return i;
        }
    }

    // Le mot n'a pas été trouvé, on renvoie -1.
    return -1;
}

/**
 * Fonction qui recherche un mot dans un texte, en utilisant l'algorithme naif,
 * avec boucle interne, avec boucle rapide, avec sentinelle.
 * 
 * Cet algorithme fonctionne en utilisant strncmp().
 * 
 * Renvoie l'indice du premier caractère du mot dans le texte, si le mot est
 * trouvé, -1 sinon.
 * 
 * @param text texte dans lequel rechercher le mot.
 * @param word mot à rechercher dans le texte.
 * @param textLength longueur du texte.
 * @param wordLength longueur du mot.
 * @return l'indice du premier caractère du mot dans le texte, si le mot est
 *        trouvé, -1 sinon, ou -2 en cas d'erreur.
*/
int naifSentinelleStrncmp(char* text, char* word, int textLength, int wordLength) {
    // Test d'erreur. Si le texte ou le mot est NULL, on renvoie -2.
    if (text == NULL || word == NULL) {
        return -2;
    }

    // Test d'erreur. Si le mot est plus long que le texte, on renvoie -1.
    if (wordLength > textLength) {
        return -1;
    }

    // Ajout d'une sentinelle à la fin du texte.
    text[textLength] = word[0];

    // Recherche du mot dans le texte (boucle rapide).
    // On stocke le dernier caractère du mot dans une variable.
    char lastChar = word[wordLength - 1];
    // On utilise strncmp pour les comparaisons
    for (int i = 0; i < textLength - wordLength + 1; i++) {
        // Si le dernier caractère du mot est différent du dernier caractère du
        // texte, on passe au caractère suivant du texte.
        if (lastChar != text[i + wordLength - 1]) {
            continue;
        }

        // Boucle interne (comparaisons avec strncmp)
        if (strncmp(text + i, word, (size_t) wordLength) != 0) {
            i += 1;
        } else {
            // Le mot a été trouvé, on renvoie l'indice du premier caractère du
            // mot dans le texte.
            return i;
        }
    }

    // Le mot n'a pas été trouvé, on renvoie -1.
    return -1;
}

////////////////////////////////////////////////////////////////////////////////

/**
 * Fonction qui calcule la table de préfixes de Bon-Pré.
 * 
 * @param word mot pour lequel calculer la table de préfixes.
 * @param wordLength longueur du mot.
 * @param bonPref table de préfixes de Bon-Pré.
 * @return 0 en cas de succès, -1 en cas d'erreur.
 */
int bonPrefTable(char* word, int wordLength, int* bonPref) {
    // Test d'erreur. Si le mot est NULL ou si la table de préfixes est NULL,
    // on renvoie -1.
    if (word == NULL || bonPref == NULL) {
        return -1;
    }

    // Initialisation de la table de préfixes.
    bonPref[0] = -1;
    bonPref[1] = 0;

    // Calcul de la table de préfixes.
    for (int i = 2; i <= wordLength; i++) {
        int j = bonPref[i - 1];
        while (j >= 0 && word[j] != word[i - 1]) {
            j = bonPref[j];
        }
        bonPref[i] = j + 1;
    }

    return 0;
}

/**
 * Fonction qui recherche un mot dans un texte, en utilisant l'algorithme de
 * Morris-Pratt.
 * Renvoie l'indice du premier caractère du mot dans le texte, si le mot est
 * trouvé, -1 sinon.
 * 
 * @param text texte dans lequel rechercher le mot.
 * @param word mot à rechercher dans le texte.
 * @param textLength longueur du texte.
 * @param wordLength longueur du mot.
 * @param bonPref table de préfixes de Bon-Pré.
 * @return l'indice du premier caractère du mot dans le texte, si le mot est
 *       trouvé, -1 sinon, ou -2 en cas d'erreur.
 */
int morrisPratt(char* text, char* word, int textLength, int wordLength, int* bonPref) {
    // Test d'erreur. Si le texte ou le mot est NULL, on renvoie -2.
    if (text == NULL || word == NULL) {
        return -2;
    }

    // Test d'erreur. Si le mot est plus long que le texte, on renvoie -1.
    if (wordLength > textLength) {
        return -1;
    }

    // Recherche du mot dans le texte.
    int i = 0;
    int j = 0;
    while (i < textLength) {
        if (text[i] == word[j]) {
            i += 1;
            j += 1;
            if (j == wordLength) {
                // Le mot a été trouvé, on renvoie l'indice du premier
                // caractère du mot dans le texte.
                return i - wordLength;
            }
        } else {
            j = bonPref[j];
            if (j < 0) {
                i += 1;
                j += 1;
            }
        }
    }

    // Le mot n'a pas été trouvé, on renvoie -1.
    return -1;
}

////////////////////////////////////////////////////////////////////////////////

/**
 * Fonction qui calcule la table de préfixes de Meil-Pré.
 * 
 * @param word mot pour lequel calculer la table de préfixes.
 * @param wordLength longueur du mot.
 * @param meilPref table de préfixes de Meil-Pré.
 * @return 0 en cas de succès, -1 en cas d'erreur.
 */
int meilPrefTable(char* word, int wordLength, int* meilPref) {
    // Test d'erreur. Si le mot est NULL ou si la table de préfixes est NULL,
    // on renvoie -1.
    if (word == NULL || meilPref == NULL) {
        return -1;
    }

    // Initialisation de la table de préfixes.
    meilPref[0] = -1;
    meilPref[1] = 0;

    // Calcul de la table de préfixes.
    int j = 0;
    for (int i = 2; i <= wordLength; i++) {
        while (j >= 0 && word[j] != word[i - 1]) {
            j = meilPref[j];
        }
        j += 1;
        meilPref[i] = j;
    }

    return 0;
}


/**
 * Fonction qui recherche un mot dans un texte, en utilisant l'algorithme de
 * Knuth-Morris-Pratt.
 * Renvoie l'indice du premier caractère du mot dans le texte, si le mot est
 * trouvé, -1 sinon.
 * 
 * @param text texte dans lequel rechercher le mot.
 * @param word mot à rechercher dans le texte.
 * @param textLength longueur du texte.
 * @param wordLength longueur du mot.
 * @param meilPref table de préfixes de Meil-Pré.
 * @return l'indice du premier caractère du mot dans le texte, si le mot est
 *       trouvé, -1 sinon, ou -2 en cas d'erreur.
 */
int knuthMorrisPratt(char* text, char* word, int textLength, int wordLength, int* meilPref) {
    // Test d'erreur. Si le texte ou le mot est NULL, on renvoie -2.
    if (text == NULL || word == NULL) {
        return -2;
    }

    // Test d'erreur. Si le mot est plus long que le texte, on renvoie -1.
    if (wordLength > textLength) {
        return -1;
    }

    // Recherche du mot dans le texte.
    int i = 0;
    int j = 0;
    while (i < textLength) {
        if (text[i] == word[j]) {
            i += 1;
            j += 1;
            if (j == wordLength) {
                // Le mot a été trouvé, on renvoie l'indice du premier
                // caractère du mot dans le texte.
                return i - wordLength;
            }
        } else {
            j = meilPref[j];
            if (j < 0) {
                i += 1;
                j += 1;
            }
        }
    }

    // Le mot n'a pas été trouvé, on renvoie -1.
    return -1;
}
