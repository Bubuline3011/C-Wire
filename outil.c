#include "projet.h"

// Fonction pour renvoyer le maximum entre deux valeurs
int max(int a, int b) {
    if (a > b) {
        return a;
    } else {
        return b;
    }
}

// Fonction pour renvoyer le minimum entre deux valeurs
int min(int a, int b) {
    if (a < b) {
        return a;
    } else {
        return b;
    }
}

// Fonction pour renvoyer le minimum parmi trois valeurs
int min3(int a, int b, int c) {
    int min_ab = min(a, b);  // Trouver le minimum entre `a` et `b`
    return min(min_ab, c);   // Comparer ce minimum avec `c`
}

// Fonction pour renvoyer le maximum parmi trois valeurs
int max3(int a, int b, int c) {
    int max_ab = max(a, b);  // Trouver le maximum entre `a` et `b`
    return max(max_ab, c);   // Comparer ce maximum avec `c`
}

