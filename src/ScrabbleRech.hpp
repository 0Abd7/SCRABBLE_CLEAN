#pragma once

#include <map>
#include <vector>
#include <random>
#include <iostream>
#include <unordered_map>
#include <set>
#include <algorithm>
#include "board.hpp"
#include "sac.hpp"
#include "gaddag.hpp"
#include "joueur.hpp"

// Structure pour représenter un placement de mot potentiel
struct CoupPossible {
    std::string mot;
    int ligne;
    int col;
    bool horizontal;
    int score;
};

class ScrabbleRech {
public:
    ScrabbleRech(Board& board, Gaddag& gaddag);
    bool estVide() const;

    void placerMot(const std::string& mot, int ligne, int col, bool horizontal);
    bool peutPlacerMot(const std::string& mot, int ligne, int col, bool horizontal) const;
    bool estLie(int ligne, int col, bool horizontal, const std::string& mot) const;
    int motScore(const std::string& mot, int ligne, int col, bool horizontal) const;
    std::vector<CoupPossible> touvePlacementPossible(const std::string& lettresChevalet);
    bool essaiePlaceMot(Joueur& joueur, int& ScoreTotal);

private:
    Board& board;
    Gaddag& gaddag;
};