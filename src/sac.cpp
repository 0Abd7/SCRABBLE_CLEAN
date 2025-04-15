#include <iostream>
#include <vector>
#include <random>
#include "sac.hpp"
using namespace std;

// Constructeur du sac de lettres
Sac::Sac() {
    // Initialise le générateur de nombres aléatoires avec une graine système
    std::random_device hwseed;
    rng.seed(hwseed());

    // Remplissage du sac en fonction du nombre de chaque lettre (stocké dans le tableau `nombre`)
    for (unsigned i = 0; i < nombre.size(); i++) {
        for (int j = 0; j < nombre[i]; j++) {
            lettres.push_back('A' + i); // Ajoute la lettre correspondante ('A' + i) autant de fois que nécessaire
        }
    }
}

// Tire une lettre aléatoire du sac en garantissant une distribution uniforme
char Sac::alea() {
    std::uniform_int_distribution<int> uniform(0, lettres.size() - 1); // Distribution uniforme entre 0 et lettres.size()-1
    unsigned int i = uniform(rng); // Choisit un indice aléatoire
    char stock = lettres[i];       // Lettre tirée
    char store = lettres.back();   // Dernière lettre du vecteur

    lettres[i] = store;            // Remplace la lettre tirée par la dernière pour éviter les trous
    lettres.pop_back();           // Supprime la dernière lettre (qui est maintenant en double)
    return stock;                 // Retourne la lettre tirée
}

// Calcule le score d'une lettre donnée, en tenant compte de la casse
int Sac::Score(char lettre) const {
    if (lettre >= 'A' && lettre <= 'Z') {
        return points[lettre - 'A']; // Lettre majuscule
    } else if (lettre >= 'a' && lettre <= 'z') {
        return points[lettre - 'a']; // Lettre minuscule
    } else {
        return 0; // Caractère non alphabétique
    }
}

// Valeurs des lettres selon les règles du Scrabble (anglophone)
const std::map<char, int> Sac::lettrePoints = {
    {'A', 1}, {'B', 3}, {'C', 3}, {'D', 2}, {'E', 1},
    {'F', 4}, {'G', 2}, {'H', 4}, {'I', 1}, {'J', 8},
    {'K', 10}, {'L', 1}, {'M', 2}, {'N', 1}, {'O', 1},
    {'P', 3}, {'Q', 8}, {'R', 1}, {'S', 1}, {'T', 1},
    {'U', 1}, {'V', 4}, {'W', 10}, {'X', 10}, {'Y', 10}, {'Z', 10}
};
