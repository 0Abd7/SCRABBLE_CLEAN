#include "board.hpp"
#include "sac.hpp"
#include "gaddag.hpp"
#include "joueur.hpp"
#include "ScrabbleRech.hpp"
#include <iostream>
#include <sstream>
#include <map>
#include <vector>
#include <thread>
#include <fstream>


/*
int calculer_score(const Coup& c, const Board& board, const Sac& sac);


const Coup* trouver_meilleur_coup(const std::vector<Coup>& coups, const Board& board, const Sac& sac) {
  const Coup* meilleur = nullptr;
  int meilleur_score = -1;

  for (const Coup& c : coups) {
      int score = calculer_score(c, board, sac);
      if (score > meilleur_score) {
          meilleur_score = score;
          meilleur = &c;
      }
  }

  return meilleur;
}

void placer_coup(Board& board, const Coup& coup) {
for (size_t i = 0; i < coup.positions.size(); ++i) {
    int index = coup.positions[i];
    int x = index % 15;
    int y = index / 15;

    board(y, x).letter = coup.mot[i];
}
}
*/
int main() {


  /*
  Board b ;
  
  //=============== TEST DU SAC ======================= 
  Sac Sac;
  char val;

  for (unsigned i=0; i<Sac.lettres.size(); i++){
    cout << Sac.lettres[i];
  }
  std::cout<<endl;

  std::cout << "Lettre aleatoire: " << Sac.alea() << std::endl;
  std::cout << "Entrez une lettre pour avoir le nombre de points :"<< endl;
  std::cin>> val;
  cout << "Score: " << Sac.Score(val)<<endl;
  
  for (unsigned i=0; i<Sac.lettres.size(); i++){
    cout << Sac.lettres[i];
  }
  std::cout<<endl;
*/
  //=============== TEST DU GADDAG ======================= 
  
  




















/*

  Board board;
  Sac sac;
  board(7, 7).letter = 'R';
  Gaddag gaddag;
  gaddag.dictionnaire();

  GaddagNode* racine = gaddag.get_racine();
  Joueur joueur;
  //joueur.remplir_chevalet_manuellement({'F', 'R', 'T'});
  joueur.tirer(sac);
  joueur.afficher_chevalet();

  std::stack<Etat> pile = initialiser_etats(board, joueur, racine);
  std::vector<Coup> coups = explorer_etats(board, pile);
  std::vector<Coup> coups_valides;

  for (const Coup& c : coups) {
    if (gaddag.mot_dans_dico(c.mot)) {
        coups_valides.push_back(c);
    }
}

const Coup* meilleur = trouver_meilleur_coup(coups_valides, board, sac);

if (meilleur != nullptr) {
    std::cout << "\n Meilleur coup joue :\n";
    afficher_coup(*meilleur, board, sac);

    int score = calculer_score(*meilleur, board, sac);
    joueur.ajouter_score(score);
    std::cout << "Score joueur apres coup : " << joueur.get_score() << "\n";

    placer_coup(board, *meilleur);

    std::cout << "\n Plateau apres pose :\n";
    std::cout << board << std::endl;
} else {
    std::cout << "Aucun coup valide trouve.\n";
}

*/

































/*
if (racine->children.count('R') > 0) {
  GaddagNode* noeud_R = racine->children['R'];
  
  std::cout << "Fils directs du nœud R :" << std::endl;

  std::unordered_map<char, GaddagNode*>::iterator it;
  for (it = noeud_R->children.begin(); it != noeud_R->children.end(); ++it) {
      char lettre = it->first;
      std::cout << " -> " << lettre << std::endl;
  }
} else {
  std::cout << "Le nœud 'R' n’est pas un fils direct de la racine." << std::endl;
}
*/

 // GaddagNode* racine = gaddag.get_racine();

  //gaddag.dictionnaire();

  /*
  if(gaddag.contient("LA") == true){
    std::cout<<"Oui il contient"<<std::endl;
  }
  else{
    std::cout<<"Non il ne contient pas"<<std::endl;
  }
  */
  /*
  //=============== TEST DU JOUEUR ======================= 
  Joueur j;
  j.tirer(Sac);
  j.afficher_chevalet();

  std::string mot = "A";
  if (j.peut_former(mot)) {
      std::cout << "Le joueur peut former " << mot << std::endl;
      j.retirer_lettres(mot);
      j.ajouter_score(12); // Ex: score du mot
  }

  j.afficher_chevalet();
  std::cout << "Score: " << j.get_score() << std::endl;
*/
  //=============== TEST RECHERCHE =======================
  /*
  Board board;
  board(7, 7).letter = 'A';  // centre du plateau (pivot)
  
  std::cout << board << std::endl;
  
  Joueur joueur;
  joueur.remplir_chevalet_manuellement({'A', 'R', 'T'});
  joueur.afficher_chevalet();
  
  Gaddag gaddag;
  gaddag.inserer_forme("L+A"); // ✅ test direct
  gaddag.dictionnaire();     // dico.txt
 
  
  
  
  
  GaddagNode* racine = gaddag.get_racine();
  std::stack<Etat> pile = initialiser_etats(board, joueur, racine);
  std::vector<Coup> coups = explorer_etats(board, pile);
  
  std::cout << "Coups trouves : " << coups.size() << "\n";
  for (const Coup& c : coups) {
      afficher_coup(c);
  }
  */
//=========================================== TEST

/*
  std::cout << b << std::endl ;

  std::stringstream ss ;
  ss << "..............." << std::endl ;
  ss << "..............." << std::endl ;
  ss << "..............." << std::endl ;
  ss << "..............." << std::endl ;
  ss << "..............." << std::endl ;
  ss << "..............." << std::endl ;
  ss << "..............." << std::endl ;
  ss << "....JUSTIN....." << std::endl ;
  ss << ".......J......." << std::endl ;
  ss << ".......U......." << std::endl ;
  ss << ".......S......." << std::endl ;
  ss << ".......T......." << std::endl ;
  ss << ".....RIADH....." << std::endl ;
  ss << "..............." << std::endl ;
  ss << "..............." << std::endl ;

  b.load(ss) ;

  std::cout << b << std::endl ;
*/











































  
  Board b;
  Sac sac;
  Joueur joueur(sac);
  //joueur.tirer(sac);
  Gaddag gaddag;

  int scoreTotal = 0;

  std::stringstream ss;
  ss << "CAIEU......H...\n";
  ss << "..N........A...\n";
  ss << "..N....BEAGLE..\n";
  ss << ".BOF.......O...\n";
  ss << "..MARGOTERAI...\n";
  ss << ".JE....O...R...\n";
  ss << "..E.C..U.......\n";
  ss << "...PUNTS.......\n";
  ss << "....M..E.......\n";
  ss << ".V..U..L.......\n";
  ss << ".I..L..L.......\n";
  ss << ".E.....E.......\n";
  ss << ".TRENDYS.......\n";
  ss << ".S.............\n";
  ss << "...............\n";



  b.load(ss);
  std::cout << b << std::endl;

  gaddag.dictionnaire();

  ScrabbleRech sr(b, gaddag);



  while (true) {
    joueur.afficher_chevalet(); // Afficher les lettres du joueur
    
    // Essayer de placer un mot en utilisant l'IA
    bool placerMot = sr.essaiePlaceMot(joueur, scoreTotal);
    
    // Si aucun mot n'a été placé, fin du jeu
    if (!placerMot) {
        std::cout << "Aucun mot valide trouvé. Fin du jeu." << std::endl;
        break;
    }
    
    // Recharger les lettres du joueur
    joueur.tirer(sac);
    
    // Afficher le plateau et le score total
    std::cout << b << std::endl;
    std::cout << "Score total : " << scoreTotal << std::endl;
  }
  return 0 ;
}