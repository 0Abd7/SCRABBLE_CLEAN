#include "gaddag.hpp"
#include "board.hpp"
#include "joueur.hpp"
#include "spot.hpp"
#include "direction.hpp"
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <stack>
#include "board.hpp"

Gaddag::Gaddag()
{
    racine = new GaddagNode();
}

void Gaddag::ajouter_mot(const std::string &mot)
{
    for (size_t i = 1; i <= mot.size(); ++i)
    {
        std::string prefix = mot.substr(0, i);
        std::reverse(prefix.begin(), prefix.end());

        std::string suffix = mot.substr(i);

        std::string forme = prefix + "+" + suffix;

        // std::cout << "Ajout dans le GADDAG : " << forme << std::endl;
        GaddagNode *current = racine;
        for (char c : forme)
        {
            if (current->children.find(c) == current->children.end())
            {
                current->children[c] = new GaddagNode();
            }
            current = current->children[c];
        }
        current->terminal = true;
        //Chaque caractère devient un nœud dans l'arbre, avec le dernier nœud marqué comme terminal.
    }
}


void Gaddag::dictionnaire() {
    std::ifstream fichier("../data/dico.txt"); // Ouvre le fichier
    std::string ligne;

    if (fichier.is_open()) {
        std::cout << "Chargement du dictionnaire...\n";
        while (getline(fichier, ligne)) { 
            ajouter_mot(ligne);  // Insère chaque mot dans le Gaddag
            dictionnaire_complet.insert(ligne);
        }
        std::cout << "Chargement termine. " << dictionnaire_complet.size() << " mots charges.\n";
        fichier.close(); // Fermer le fichier après lecture
    } else {
        std::cout << "Erreur : Impossible de lire le fichier." << std::endl;
    }
}



bool Gaddag::cherche_mot_sans_plus(const std::string &mot) const
{
    // Générer toutes les formes Gaddag possibles pour le mot
    for (size_t i = 1; i <= mot.size(); ++i)
    {
        std::string prefix = mot.substr(0, i);
        std::reverse(prefix.begin(), prefix.end());
        std::string suffix = mot.substr(i);
        std::string forme = prefix + "+" + suffix;

        // Rechercher la forme transformée dans l'arbre
        GaddagNode *node = racine;
        bool found = true;
        for (char c : forme)
        {
            if (node->children.find(c) == node->children.end())
            {
                found = false;
                break;
            }
            node = node->children[c];
        }
        if (found && node->terminal)
        {
            return true; // Le mot existe sous cette forme
        }
    }
    return false; // Aucune forme transformée n'a été trouvée
} 

//recursive genere toutes les combinaison possibles 
void Gaddag::genereCombinaisons(const std::string &lettres, std::string actuelle, std::set<std::string> &combinations)
{
    if (!actuelle.empty())
    {
        combinations.insert(actuelle); // Ajouter la combinaison courante
    }
    for (size_t i = 0; i < lettres.size(); ++i)
    {
        genereCombinaisons(lettres.substr(0, i) + lettres.substr(i + 1), actuelle + lettres[i], combinations);
    }
}

std::vector<std::string> Gaddag::trouveMotValideChevalet(const Gaddag &gaddag, const std::string &lettresChevalet)
{
    std::set<std::string> combinaisons;
    genereCombinaisons(lettresChevalet, "", combinaisons);

    std::vector<std::string> motsValides;
    for (const std::string &mot : combinaisons)
    {
        if (gaddag.cherche_mot_sans_plus(mot))
        {
            motsValides.push_back(mot);
        }
    }
    //std::cout << "Nb mots valides trouves : " << motsValides.size() << std::endl;
    return motsValides;
}

//cree une copie du tableau pour tester toute formes si cest valides return vrai si passe ttes les verifs
bool Gaddag::checkMotsCroises(const Board &board, int ligne, int col, bool horizontal, const std::string &mot) {
    Board tempBoard = board;

    // Placer temporairement le mot sur la copie du plateau
    for (size_t i = 0; i < mot.size(); ++i) {
        int l = ligne + (horizontal ? 0 : i);
        int c = col + (horizontal ? i : 0);

        // Si la case est déjà occupée, vérifier que c'est la même lettre
        if (tempBoard(l, c).letter != 0 && tempBoard(l, c).letter != mot[i]) {
            return false;
        }

        tempBoard(l, c).letter = mot[i];
    }

    // Vérifier le mot principal
    std::string motPrincipal;
    if (horizontal) {
        // Construire le mot horizontal complet
        motPrincipal = getMotHorizontalComplet(tempBoard, ligne, col);
    }
    else {
        // Construire le mot vertical complet
        motPrincipal = getMotVerticalComplet(tempBoard, ligne, col);
    }

    // Vérifier que le mot principal est valide
    if (motPrincipal.length() > 1 && !cherche_mot_sans_plus(motPrincipal)) {
        return false;
    }

    // Vérifier tous les mots croisés
    for (size_t i = 0; i < mot.size(); ++i) {
        int l = ligne + (horizontal ? 0 : i);
        int c = col + (horizontal ? i : 0);

        // Vérifier seulement aux positions où nous avons ajouté une lettre
        // et où la position était vide sur le plateau original
        if (board(l, c).letter == 0) {
            std::string motCroise;

            if (horizontal) {
                // Vérifier le mot vertical croisé
                motCroise = getMotVerticalComplet(tempBoard, l, c);
            }
            else {
                // Vérifier le mot horizontal croisé
                motCroise = getMotHorizontalComplet(tempBoard, l, c);
            }

            // Si un mot croisé est formé, vérifier qu'il est valide
            if (motCroise.length() > 1 && !cherche_mot_sans_plus(motCroise)) {
                return false;
            }
        }
    }

    return true;
}

std::string Gaddag::getMotHorizontalComplet(const Board &board, int ligne, int col) const {
    std::string mot;
    // Aller à gauche jusqu'au début du mot
    int debutCol = col;
    while (debutCol > 0 && board(ligne, debutCol - 1).letter != 0) {
        debutCol--;
    }
    // Construire le mot complet vers la droite
    int colActuel = debutCol;
    while (colActuel < 15 && board(ligne, colActuel).letter != 0) {
        mot += board(ligne, colActuel).letter;
        colActuel++;
    }
    return mot;
}

std::string Gaddag::getMotVerticalComplet(const Board &board, int ligne, int col) const {
    std::string mot;
    // Remonter jusqu'au début du mot
    int debutLigne = ligne;
    while (debutLigne > 0 && board(debutLigne - 1, col).letter != 0) {
        debutLigne--;
    }
    // Construire le mot complet vers le bas
    int ligneActuelle = debutLigne;
    while (ligneActuelle < 15 && board(ligneActuelle, col).letter != 0) {
        mot += board(ligneActuelle, col).letter;
        ligneActuelle++;
    }
    return mot;
}














//===================================================================================== Pas utilisé ancienne version






/*
int coord_to_index(int x, int y) {
    return y * 15 + x;
}

std::pair<int, int> index_to_coord(int index) {
    return { index % 15, index / 15 };
}

// Renvoie les indices des cases vides adjacentes à des lettres posées
std::vector<int> cases_depart_possibles(const Board& board) {
    std::set<int> resultat;

    for (int l = 0; l < 15; ++l) {
        for (int c = 0; c < 15; ++c) {
            const Spot& spot = board(l, c);
            if (spot.letter != 0) { // case occupée
                std::vector<std::pair<int, int>> voisins;
                voisins.push_back(std::make_pair(l+1, c));
                voisins.push_back(std::make_pair(l-1, c));
                voisins.push_back(std::make_pair(l, c+1));
                voisins.push_back(std::make_pair(l, c-1));

                for (std::vector<std::pair<int, int>>::const_iterator it = voisins.begin(); it != voisins.end(); ++it) {
                    int vl = it->first;
                    int vc = it->second;

                    if (vl >= 0 && vl < 15 && vc >= 0 && vc < 15) {
                        const Spot& voisin = board(vl, vc);
                        if (voisin.letter == 0) { // case vide
                            int index = vl * 15 + vc;
                            resultat.insert(index);
                        }
                    }
                }
            }
        }
    }
    for (int idx : resultat) {
        int l = idx / 15;
        int c = idx % 15;
        std::cout << "Case départ possible : (" << l << "," << c << ") = index " << idx << std::endl;
    }
    
    return std::vector<int>(resultat.begin(), resultat.end());
}



std::stack<Etat> initialiser_etats(
    const Board& board,
    const Joueur& joueur,
    GaddagNode* racine_gaddag
) {
    std::stack<Etat> pile;
    std::vector<int> depart_possibles = cases_depart_possibles(board);
    const std::vector<char>& lettres = joueur.get_chevalet();

    for (int index : depart_possibles) {
        int x = index % 15;
        int y = index / 15;
    
        // Vérifie les cases autour pour déterminer la direction valide
        bool horizontal = false;
        bool vertical = false;
    
        if (x > 0 && board(y, x - 1).letter != 0) horizontal = true;
        if (x < 14 && board(y, x + 1).letter != 0) horizontal = true;
    
        if (y > 0 && board(y - 1, x).letter != 0) vertical = true;
        if (y < 14 && board(y + 1, x).letter != 0) vertical = true;
    
        if (horizontal) {
            Etat e(index, lettres, racine_gaddag, HORIZONTAL, {}, false);
            pile.push(e);
            std::cout << "Position HORIZONTAL: (" << x << "," << y << ")" << std::endl;
        }
    
        if (vertical) {
            Etat e(index, lettres, racine_gaddag, VERTICAL, {}, false);
            pile.push(e);
            std::cout << "Position VERTICAL: (" << x << "," << y << ")" << std::endl;
        }
    }
    

    return pile;
}

std::vector<Coup> explorer_etats(Board& board, std::stack<Etat>& pile) {
    std::vector<Coup> coups_valides;

    while (!pile.empty()) {
        Etat etat = pile.top();
        pile.pop();

        int x = etat.position % 15;
        int y = etat.position / 15;
        

        if (x < 0 || x >= 15 || y < 0 || y >= 15)
            continue;

        Spot& spot = board(y, x);

        // --- CASE OCCUPÉE ---
        if (spot.letter != 0) {
            char lettre_plateau = spot.letter;
            if (etat.noeud->children.count(lettre_plateau)) {
                Etat suivant = etat;
                suivant.noeud = etat.noeud->children[lettre_plateau];
                suivant.mot_construit += lettre_plateau;
                suivant.position += (etat.direction == HORIZONTAL) ? 1 : 15;

                //std::cout << "Lettre plateau '" << lettre_plateau << "' lue -> noeud ";
                //std::cout << (suivant.noeud->terminal ? "terminal" : "non-terminal");
                //std::cout << ", mot = " << suivant.mot_construit << std::endl;
                pile.push(suivant);
            }
            // sinon lettre incompatible → état abandonné
            continue;
        }

        // --- CASE VIDE ---

        // Vérifie si on est en noeud terminal
        if (etat.noeud->terminal && !etat.lettres_posees.empty()) {
            std::cout << "noeud terminal atteint pour mot : " << etat.mot_construit << std::endl;
            int next = etat.position;
            int nx = next % 15;
            int ny = next / 15;
            bool case_suivante_valide = (
                nx < 0 || nx >= 15 || ny < 0 || ny >= 15 || board(ny, nx).letter == 0
            );
        
            if (case_suivante_valide) {
                std::cout << "Mot trouve : " << etat.mot_construit << std::endl;
                Coup coup;
                coup.positions = etat.lettres_posees;
                coup.mot = etat.mot_construit;
                coups_valides.push_back(coup);
            }
        }

        // Traverser le + si on ne l’a pas encore fait
        if (!etat.plus_utilise && etat.noeud->children.count('+')) {
            Etat suivant = etat;
            suivant.noeud = etat.noeud->children['+'];
            suivant.plus_utilise = true;

            //std::cout << "Traversee du pivot '+' -> nouveau noeud "<< (suivant.noeud->terminal ? "terminal" : "non-terminal")<< ", mot = " << suivant.mot_construit << std::endl;

            // Ne change pas la position ici, mais la direction changera après
            suivant.direction = (etat.direction == HORIZONTAL) ? HORIZONTAL : VERTICAL;
            //suivant.position += (suivant.direction == HORIZONTAL) ? 1 : 15;
            pile.push(suivant);
        }

        // 🅰 Essayer de poser une lettre du chevalet
        for (size_t i = 0; i < etat.lettres_restantes.size(); ++i) {
            char l = etat.lettres_restantes[i];
            if (etat.noeud->children.count(l)) {
                Etat suivant = etat;
                suivant.noeud = etat.noeud->children[l];
                suivant.lettres_posees.push_back(etat.position);
                suivant.mot_construit += l;
                suivant.lettres_restantes.erase(suivant.lettres_restantes.begin() + i);
                suivant.position += (etat.direction == HORIZONTAL) ? 1 : 15;

                //std::cout << "Lettre '" << l << "' posee -> noeud ";
                //std::cout << (suivant.noeud->terminal ? "terminal" : "non-terminal");
                //std::cout << ", mot = " << suivant.mot_construit << std::endl;

                // Pose temporaire sur le plateau
                board.spots[etat.position].letter = l;
                pile.push(suivant);
                board.spots[etat.position].letter = 0; // nettoyage
            }
        }
    }

    return coups_valides;
}

bool Gaddag::mot_dans_dico(const std::string& mot) const {
    return dictionnaire_complet.count(mot) > 0;
}

int calculer_score(const Coup& c, const Board& board, const Sac& sac);


void afficher_coup(const Coup& c, const Board& board, const Sac& sac) {
    std::cout << "Mot : " << c.mot << "\n";
    std::cout << "Positions : ";
    for (int pos : c.positions) {
        int x = pos % 15;
        int y = pos / 15;
        std::cout << "(" << x << "," << y << ") ";
    }
    std::cout << "\n";

    std::cout << "Score : " << calculer_score(c, board, sac) << "\n";
}















int calculer_score(const Coup& coup, const Board& board, const Sac& sac) {
    int total = 0;

    for (size_t i = 0; i < coup.positions.size(); ++i) {
        int index = coup.positions[i];
        int x = index % 15;
        int y = index / 15;

        char lettre = board(y, x).letter;

        // Si lettre non encore posée sur le plateau, on prend celle du mot
        if (lettre == 0) {
            lettre = coup.mot[i];
        }

        total += sac.Score(lettre);
    }

    return total;
}

GaddagNode::GaddagNode() : terminal(false) {}

GaddagNode::~GaddagNode() {
    for (auto& pair : children) {
        delete pair.second;
    }
}

Gaddag::Gaddag() {
    racine = new GaddagNode();
}

Gaddag::~Gaddag() {
    delete racine;
}

void Gaddag::inserer_forme(const std::string& forme) {
    GaddagNode* current = racine;
    for (char c : forme) {
        //std::cout<<c<<std::endl;
        if (current->children.find(c) == current->children.end()) {
            current->children[c] = new GaddagNode();
        }
        current = current->children[c];
    }
    current->terminal = true;
}

void Gaddag::ajouter_mot(const std::string& mot) {
    int n = mot.size();
    for (int i = 1; i <= n; ++i) {
        std::string prefix = mot.substr(0, i);       // ce qui sera avant le +
        std::string suffix = mot.substr(i);          // ce qui sera après le +

        std::reverse(prefix.begin(), prefix.end());  // <- c'est ici qu'on inverse
        std::string forme = prefix + "+" + suffix;
        //std::cout<<forme<<std::endl;
        inserer_forme(forme);
    }
}


bool Gaddag::contient(const std::string& mot) const {
    GaddagNode* current = racine;
    for (char c : mot) {
        if (current->children.find(c) == current->children.end()) {
            return false;
        }
        current = current->children.at(c);
    }
    return current->terminal;
}

bool Gaddag::cherche_mot_sans_plus(const std::string &word) const
{
    // Générer toutes les formes Gaddag possibles pour le mot
    for (size_t i = 1; i <= word.size(); ++i)
    {
        std::string prefix_part = word.substr(0, i);
        std::reverse(prefix_part.begin(), prefix_part.end());
        std::string suffix_part = word.substr(i);
        std::string transformed = prefix_part + "+" + suffix_part;

        // Rechercher la forme transformée dans l'arbre
        GaddagNode *node = root;
        bool found = true;
        for (char c : transformed)
        {
            if (node->children.find(c) == node->children.end())
            {
                found = false;
                break;
            }
            node = node->children[c];
        }
        if (found && node->isTerminal)
        {
            return true; // Le mot existe sous cette forme
        }
    }
    return false; // Aucune forme transformée n'a été trouvée
}

GaddagNode* Gaddag::get_racine() const {
    return racine;
}

void Gaddag::dictionnaire() {
    std::ifstream fichier("dico.txt"); // Ouvre le fichier
    std::string ligne;

    if (fichier.is_open()) {
        while (getline(fichier, ligne)) { 
            ajouter_mot(ligne);  // Insère chaque mot dans le Gaddag
            dictionnaire_complet.insert(ligne);
        }
        fichier.close(); // Fermer le fichier après lecture
    } else {
        std::cout << "Erreur : Impossible de lire le fichier." << std::endl;
    }
}

*/