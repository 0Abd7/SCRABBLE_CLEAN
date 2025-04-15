#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <stack>
#include <iostream>
#include "board.hpp"
#include "joueur.hpp"
#include "spot.hpp"
#include "direction.hpp"
#include "gaddagNoeud.hpp"
#include <unordered_set>
#include <set>

class Gaddag {
public:
    GaddagNode *racine;
    Gaddag();
    std::unordered_set<std::string> dictionnaire_complet;
    
    void ajouter_mot(const std::string& mot);
    bool cherche_mot_sans_plus(const std::string &mot) const;
    void dictionnaire();
    void genereCombinaisons(const std::string &lettres, std::string actuelle, std::set<std::string> &combinations);
    std::vector<std::string> trouveMotValideChevalet(const Gaddag &gaddag, const std::string &lettresChevalet);
    bool checkMotsCroises(const Board &board, int ligne, int col, bool horizontal, const std::string &mot);
    std::string getMotHorizontalComplet(const Board &board, int ligne, int col) const;
    std::string getMotVerticalComplet(const Board &board, int ligne, int col) const;
};