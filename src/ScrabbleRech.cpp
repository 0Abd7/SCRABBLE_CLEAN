#include "ScrabbleRech.hpp"
#include "gaddag.hpp"

// Constructeur de la classe ScrabbleRech : initialise avec un plateau et une structure GADDAG
ScrabbleRech::ScrabbleRech(Board& board, Gaddag& gaddag) : board(board), gaddag(gaddag) {}

// Vérifie si le plateau est vide (aucune lettre placée)
bool ScrabbleRech::estVide() const {
    for (int i = 0; i < 225; ++i) {
        if (board.spots[i].letter != 0) return false;
    }
    return true;
}

// Place un mot sur le plateau à la position (ligne, col) dans la direction horizontale ou verticale
void ScrabbleRech::placerMot(const std::string& mot, int ligne, int col, bool horizontal) {
    for (size_t i = 0; i < mot.size(); ++i) {
        int l = ligne + (horizontal ? 0 : i);
        int c = col + (horizontal ? i : 0);

        // Ne place la lettre que si la case est vide
        if (board(l, c).letter == 0) {
            board(l, c).letter = mot[i];
        }
    }
}

// Vérifie si un mot peut être placé à une position donnée, sans sortir du plateau ni entrer en conflit
bool ScrabbleRech::peutPlacerMot(const std::string& mot, int ligne, int col, bool horizontal) const {
    // Vérifie les limites du plateau
    if (ligne < 0 || col < 0) return false;
    if (horizontal && col + mot.length() > 15) return false;
    if (!horizontal && ligne + mot.length() > 15) return false;

    // Vérifie s'il y a des conflits avec des lettres déjà posées
    for (size_t i = 0; i < mot.size(); ++i) {
        int l = ligne + (horizontal ? 0 : i);
        int c = col + (horizontal ? i : 0);

        Spot spot = board(l, c);
        if (spot.letter != 0 && spot.letter != mot[i]) {
            return false;
        }
    }
    return true;
}

// Vérifie si un mot est connecté correctement à d'autres lettres sur le plateau (ou couvre le centre si premier mot)
bool ScrabbleRech::estLie(int ligne, int col, bool horizontal, const std::string& mot) const {
    if (estVide()) {
        // Si le plateau est vide, le premier mot doit couvrir la case centrale (7,7)
        for (size_t i = 0; i < mot.size(); ++i) {
            int l = ligne + (horizontal ? 0 : i);
            int c = col + (horizontal ? i : 0);
            if (l == 7 && c == 7) return true;
        }
        return false;
    }

    bool aConnection = false;
    bool motContinue = true;

    // Vérifie la connexion à d'autres lettres
    for (size_t i = 0; i < mot.size(); ++i) {
        int l = ligne + (horizontal ? 0 : i);
        int c = col + (horizontal ? i : 0);

        if (board(l, c).letter != 0) {
            aConnection = true;
        } else {
            bool aLettreAdjacente = false;
            if (l > 0 && board(l-1, c).letter != 0) aLettreAdjacente = true;
            if (l < 14 && board(l+1, c).letter != 0) aLettreAdjacente = true;
            if (c > 0 && board(l, c-1).letter != 0) aLettreAdjacente = true;
            if (c < 14 && board(l, c+1).letter != 0) aLettreAdjacente = true;
            if (aLettreAdjacente) aConnection = true;
        }
    }

    // Vérifie que le mot forme une séquence continue
    int debut = -1, fin = -1;
    for (size_t i = 0; i < mot.size(); ++i) {
        int l = ligne + (horizontal ? 0 : i);
        int c = col + (horizontal ? i : 0);
        if (board(l, c).letter == 0) { debut = i; break; }
    }
    for (int i = mot.size() - 1; i >= 0; --i) {
        int l = ligne + (horizontal ? 0 : i);
        int c = col + (horizontal ? i : 0);
        if (board(l, c).letter == 0) { fin = i; break; }
    }

    if (debut != -1 && fin != -1) {
        for (int i = debut; i <= fin; ++i) {
            int l = ligne + (horizontal ? 0 : i);
            int c = col + (horizontal ? i : 0);

            if (board(l, c).letter == 0) {
                bool nvlLettrePossedeAdjacente = false;
                if (horizontal) {
                    if (c > 0 && board(l, c-1).letter != 0) nvlLettrePossedeAdjacente = true;
                    if (c < 14 && board(l, c+1).letter != 0) nvlLettrePossedeAdjacente = true;
                } else {
                    if (l > 0 && board(l-1, c).letter != 0) nvlLettrePossedeAdjacente = true;
                    if (l < 14 && board(l+1, c).letter != 0) nvlLettrePossedeAdjacente = true;
                }

                if (!nvlLettrePossedeAdjacente) {
                    bool aAdjacentPerpendiculaire = false;
                    if (horizontal) {
                        if (l > 0 && board(l-1, c).letter != 0) aAdjacentPerpendiculaire = true;
                        if (l < 14 && board(l+1, c).letter != 0) aAdjacentPerpendiculaire = true;
                    } else {
                        if (c > 0 && board(l, c-1).letter != 0) aAdjacentPerpendiculaire = true;
                        if (c < 14 && board(l, c+1).letter != 0) aAdjacentPerpendiculaire = true;
                    }

                    if (!aAdjacentPerpendiculaire && i > debut) {
                        int precL = ligne + (horizontal ? 0 : i-1);
                        int precC = col + (horizontal ? i-1 : 0);
                        if (board(precL, precC).letter == 0) {
                            motContinue = false;
                            break;
                        }
                    }
                }
            }
        }
    }

    return aConnection && motContinue;
}

// Calcule le score total d’un mot à placer, en appliquant les bonus des cases
int ScrabbleRech::motScore(const std::string &mot, int ligne, int col, bool horizontal) const {
    int score = 0;
    int motMultiplier = 1;

    for (size_t i = 0; i < mot.size(); ++i) {
        int ligneActuelle = ligne + (horizontal ? 0 : i);
        int colActuelle = col + (horizontal ? i : 0);

        Spot spot = board(ligneActuelle, colActuelle);
        int lettreScore = Sac::lettrePoints.at(mot[i]);

        lettreScore = spot.bonus.apply_letter(lettreScore); // Bonus lettre
        motMultiplier *= spot.bonus.apply_word(1);          // Bonus mot

        score += lettreScore;
    }

    return score * motMultiplier;
}

// Cherche tous les placements possibles avec les lettres du chevalet sur le plateau
std::vector<CoupPossible> ScrabbleRech::touvePlacementPossible(const std::string& lettresChevalet) {
    std::cout << "Recherche de placement avec chevalet : " << lettresChevalet << std::endl;
    std::vector<CoupPossible> placements;

    for (int ligne = 0; ligne < 15; ++ligne) {
        for (int col = 0; col < 15; ++col) {
            if (board(ligne, col).letter == 0) {
                bool lettreAdjacente = false;

                if ((ligne > 0 && board(ligne-1, col).letter != 0) ||
                    (ligne < 14 && board(ligne+1, col).letter != 0) ||
                    (col > 0 && board(ligne, col-1).letter != 0) ||
                    (col < 14 && board(ligne, col+1).letter != 0)) {
                    lettreAdjacente = true;
                }

                if (estVide() && ligne == 7 && col == 7) lettreAdjacente = true;

                if (lettreAdjacente) {
                    for (bool horizontal : {true, false}) {
                        auto motsValides = gaddag.trouveMotValideChevalet(gaddag, lettresChevalet);
                        for (const auto& mot : motsValides) {
                            for (size_t lettreIndex = 0; lettreIndex < mot.size(); ++lettreIndex) {
                                int startLigne = ligne - (horizontal ? 0 : lettreIndex);
                                int startCol = col - (horizontal ? lettreIndex : 0);

                                if (startLigne >= 0 && startCol >= 0 &&
                                   (horizontal ? startCol + mot.size() <= 15 : startLigne + mot.size() <= 15)) {
                                    if (peutPlacerMot(mot, startLigne, startCol, horizontal) &&
                                        estLie(startLigne, startCol, horizontal, mot) &&
                                        gaddag.checkMotsCroises(board, startLigne, startCol, horizontal, mot)) {
                                        int score = motScore(mot, startLigne, startCol, horizontal);
                                        placements.push_back({mot, startLigne, startCol, horizontal, score});
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return placements;
}

// Tente de placer le meilleur mot possible pour un joueur et met à jour son score
bool ScrabbleRech::essaiePlaceMot(Joueur& joueur, int& totalScore) {
    std::string lettresChevalet = joueur.getChevalet();
    auto placements = touvePlacementPossible(lettresChevalet);

    if (placements.empty()) {
        std::cout << "Aucun placement possible trouvé avec : " << lettresChevalet << std::endl;
        return false;
    }

    auto meilleurPlacement = *std::max_element(placements.begin(), placements.end(),
        [](const CoupPossible& a, const CoupPossible& b) {
            return a.score < b.score;
        });

    placerMot(meilleurPlacement.mot, meilleurPlacement.ligne, meilleurPlacement.col, meilleurPlacement.horizontal);
    joueur.supprLettreDuChevalet(meilleurPlacement.mot);
    totalScore += meilleurPlacement.score;

    std::cout << "Mot placé : " << meilleurPlacement.mot
              << " en position (" << meilleurPlacement.ligne << "," << meilleurPlacement.col << ")"
              << " direction " << (meilleurPlacement.horizontal ? "horizontale" : "verticale")
              << " - Score : " << meilleurPlacement.score << std::endl;

    return true;
}
