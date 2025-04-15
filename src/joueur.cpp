#include "joueur.hpp"
#include <iostream>
#include <algorithm>

// Constructeur : initialise un joueur avec 7 lettres tirées aléatoirement depuis le sac
Joueur::Joueur(Sac &sac)
{
    for (int i = 0; i < 7; ++i) { // Un chevalet contient 7 lettres au Scrabble
        chevalet.push_back(sac.alea()); // On pioche une lettre du sac à chaque itération
    }
}

// Tire des lettres aléatoires du sac pour compléter le chevalet jusqu'à 7 lettres
void Joueur::tirer(Sac& sac) {
    while (chevalet.size() < 7 && !sac.lettres.empty()) {
        chevalet.push_back(sac.alea());
    }
}

// Remplit manuellement le chevalet avec un vecteur de lettres donné (utile pour les tests)
void Joueur::remplir_chevalet_manuellement(const std::vector<char>& lettres) {
    chevalet = lettres;
}

// Affiche les lettres actuelles du chevalet du joueur
void Joueur::afficher_chevalet() const {
    std::cout << "Chevalet : ";
    for (char c : chevalet) {
        std::cout << c << ' ';
    }
    std::cout << std::endl;
}

// Vérifie si une lettre est présente dans le chevalet
bool Joueur::a_lettre(char c) const {
    return std::count(chevalet.begin(), chevalet.end(), c) > 0;
}

// Vérifie si le joueur peut former un mot avec les lettres de son chevalet
bool Joueur::peut_former(const std::string& mot) const {
    std::vector<char> temp = chevalet; // Copie du chevalet pour ne pas modifier l'original
    for (char c : mot) {
        auto it = std::find(temp.begin(), temp.end(), c);
        if (it == temp.end()) // Lettre absente
            return false;
        temp.erase(it); // Retire la lettre utilisée
    }
    return true;
}

// Retire les lettres d'un mot du chevalet (après qu'il a été joué)
void Joueur::retirer_lettres(const std::string& mot) {
    for (char c : mot) {
        auto it = std::find(chevalet.begin(), chevalet.end(), c);
        if (it != chevalet.end()) {
            chevalet.erase(it);
        }
    }
}

// Retourne le contenu du chevalet sous forme de chaîne (string)
std::string Joueur::getChevalet() const {
    std::string lettres;
    for (char lettre : chevalet) {
        lettres += lettre;
    }
    return lettres;
}

// Supprime les lettres utilisées pour un mot du chevalet
void Joueur::supprLettreDuChevalet(const std::string& mot) {
    std::string lettresRestantes = mot;
    for (auto it = chevalet.begin(); it != chevalet.end(); ) {
        auto pos = lettresRestantes.find(*it); // Cherche la lettre dans le mot
        if (pos != std::string::npos) {
            lettresRestantes.erase(pos, 1);     // Retire la lettre utilisée du mot
            it = chevalet.erase(it);            // Retire la lettre du chevalet
        } else {
            ++it; // Passe à la lettre suivante
        }
    }
}
