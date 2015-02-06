#ifndef DEF_PROF
#define DEF_PROF

#include <iostream>
#include <string>

struct Cours
{
	std::string sigle;
	Cours *suivant;
};
Cours *courant_cours;

struct Etudiant
{
	std::string nom;
	Etudiant *apres;
};
Etudiant *courant_etu;

struct Professeur
{
	std::string nom;
	std::string prenom;
	Cours *listecours;
	Etudiant *listetudiant;
	Professeur *suivant;
};
Professeur *courant_prof;

class DossierProfesseur
{
  private:
	Professeur *tete; // début de la liste chaînée

  public:
	DossierProfesseur (std::string FP); // Construit de la structure de la liste chaînée en mémoire à partir du fichier FP
	~DossierProfesseur ( );											 // Détruit de la liste chaînée existant en mémoire.
	void supprimer (std::string nom, std::string prenom );		 // supprime de la liste chaînée un professeur donné
	std::string afficherleprofplusetudiant ( ) const;				 // affiche le nom du professeur ayant le plus d’étudiants
	std::string affichercoursplusdemande ( ) const;				 // affiche le cours le plus demandé de la liste
	int affichernbreprofpouruncours (std::string coursdonne) const; // affiche le nombre de professeurs pour le
																	 // cours coursdonne recopie la liste chaînée
	void recopier (std::string FP); // dans le fichier FP
};

#endif
