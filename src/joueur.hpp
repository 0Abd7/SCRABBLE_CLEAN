#ifndef JOUEUR_HPP
#define JOUEUR_HPP

#include <vector>
#include <string>
#include "sac.hpp"

class Joueur {
private:
    std::vector<char> chevalet;

public:
    Joueur(Sac& sac);

    void tirer(Sac& sac);
    void remplir_chevalet_manuellement(const std::vector<char>& lettres);
    void afficher_chevalet() const;

    bool a_lettre(char c) const;
    bool peut_former(const std::string& mot) const;

    void retirer_lettres(const std::string& mot);
    const std::vector<char>& get_chevalet() const;
    std::string getChevalet() const;
    void supprLettreDuChevalet(const std::string& mot);
};

#endif
