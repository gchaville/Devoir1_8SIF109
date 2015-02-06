#include <iostream>
#include <fstream>
#include "DossierProfesseur.h"
#include <string>



void afficher (void);

DossierProfesseur::DossierProfesseur (std::string FP) /* Constructeur */
{
	std::ifstream entree;/* Ouverture en mode fichier */
	tete = courant_prof = new Professeur;
	entree.open (FP.c_str()); /* Ouverture du fichier */
	if (entree.fail ()) /* Si le fichier n'ouvre pas */
	{
		std::cout << "Erreur d'ouverture";
		entree.close(); /* Fermeture du fichier */
	}
	else // CREE TETE LISTE
	{
		while  (!entree.eof()) /* Boucle tant que le fichier n'est pas terminé */
		{
			getline (entree, courant_prof->nom);
			getline (entree, courant_prof->prenom);
			courant_prof->listecours = new Cours;  // cree liste de cours
			courant_cours = courant_prof->listecours;
			getline (entree, courant_cours->sigle);
			while (courant_cours->sigle != "&") /* arret pour le chaine de cours */
			{
				courant_cours->suivant = new Cours; // ajoute nouveau cours
				courant_cours = courant_cours->suivant;
				getline (entree, courant_cours->sigle);
			}
			courant_cours->suivant = NULL; /* Fin de la liste de cour */
			courant_prof->listetudiant = new Etudiant; // cree liste etudiant
			courant_etu = courant_prof->listetudiant;
			getline (entree, courant_etu->nom);
			while (courant_etu->nom != "&") /* meme arret que le courant cours */
			{
				courant_etu->apres = new Etudiant; // ajoute nouveau etudiant
				courant_etu = courant_etu->apres;
				getline (entree, courant_etu->nom);
			}
			courant_etu->apres = NULL;/* arret de la liste d'etudiant */
			if (!entree.eof()) /* Nous pouvons faire un do while au lieu */
			{
				courant_prof->suivant = new Professeur; // cree nouveau professeur
				courant_prof = courant_prof->suivant;
			}
		}
		courant_prof->suivant = NULL;
	}
	entree.close(); /* Fermeture du fichier */
	courant_prof = tete; // re-pointe courant prof sur la tete
}

DossierProfesseur::~DossierProfesseur ( ) /*Destructeur */
{
	courant_prof = tete;
	while (courant_prof != NULL)/* boucle qui termine la chaine */
	{
		courant_etu = courant_prof->listetudiant;/*Pointe sur la liste etudiant */
		while (courant_etu != NULL) /*parcour la liste etudaint */
		{
			courant_prof->listetudiant= courant_etu->apres;
			delete courant_etu; /*Détruit un élément de la chaine */
			courant_etu = courant_prof->listetudiant;
		}
		courant_cours = courant_prof->listecours;
		while (courant_cours != NULL) /*rentre dans la chaine de cours */
		{
			courant_prof->listecours= courant_cours->suivant;
			delete courant_cours;/* destruction d'un element de la chaine */
			courant_cours = courant_prof->listecours;
		}
		tete = courant_prof->suivant;/*pointe sur la tete de l'élément prof */
		delete courant_prof; /* détruit l'element prof */
		courant_prof = tete; /* redonne la position de la tete */
	}
}

void DossierProfesseur::supprimer (std::string nom, std::string prenom )
/* recois un nom et un prenom appartire du fichier texte que l'utilisateur doit composer */
{
	Professeur *precedent;/* Pointeur qui permet de trouver le nom et prenom */
	precedent = NULL;
	courant_prof = tete;

	while (courant_prof != NULL && courant_prof->nom != nom && courant_prof->prenom != prenom)
	/* Condition pour trouver un nom et prenom ou arrete à la fin du fichier */
	{
		precedent = courant_prof; /* Précédent va prendre la position du professeur courant et elle peut-sortire pas la suite */
		courant_prof = courant_prof->suivant;
	}
	if (courant_prof == NULL)
	{
		std::cout << std::endl << "Le professeur recherche n'existe pas." << std::endl;
	}
	else
	{
		if (precedent != NULL) /* si precedent a pris la position de courant prof*/
			precedent->suivant = courant_prof->suivant;
		else // si precedent est égale à null
			tete = tete->suivant;
		delete courant_prof;
		recopier ("FP.txt");//appel la fonction recopier qui copier les modification
	}
	courant_prof = tete;
}

std::string DossierProfesseur::afficherleprofplusetudiant ( ) const
{
	int nb_etudiants, tampon;
	Professeur *profrecherche;
	std::string nomrecherche;

	profrecherche = courant_prof = tete;
	courant_etu = courant_prof->listetudiant;

	for (nb_etudiants = 0; courant_etu->apres != NULL; nb_etudiants++)
		{
			courant_etu = courant_etu->apres;
		}
	courant_prof = courant_prof->suivant;
	courant_etu = courant_prof->listetudiant;
	while (courant_prof != NULL)
	{
		courant_etu = courant_prof->listetudiant;
		for (tampon = 0; courant_etu->apres != NULL; tampon++)
		{
			courant_etu = courant_etu->apres;
		}
		if (tampon > nb_etudiants)
		{
			nb_etudiants = tampon;
			profrecherche = courant_prof;
		}
		else if (tampon == nb_etudiants && courant_prof->prenom != profrecherche->prenom)
		{
			nomrecherche = courant_prof->nom + " " + courant_prof->prenom + " et ";
		}
		courant_prof = courant_prof->suivant;
	}
	nomrecherche += profrecherche->nom + " " + profrecherche->prenom;
	courant_prof = tete;
	return nomrecherche;
}

int DossierProfesseur::affichernbreprofpouruncours (std::string coursdonne) const
{
	int nbprofpourcours = 0;
	courant_prof = tete;

	while (courant_prof != NULL)
	{
		courant_cours = courant_prof->listecours;
		while (courant_cours != NULL)
		{
			if (coursdonne  == courant_cours->sigle)
				nbprofpourcours += 1;
			courant_cours = courant_cours->suivant;
		}
		courant_prof = courant_prof->suivant;
	}
	if (nbprofpourcours == 0)
		return 0;
	else
		return nbprofpourcours;

}

std::string DossierProfesseur::affichercoursplusdemande ( ) const
{
	int nbre = 0, tampon=0;
	std::string plusdemande;
	Cours *coursrecherche = NULL;

	courant_prof = tete;
	courant_cours = courant_prof->listecours;
	coursrecherche = courant_prof->listecours;
	nbre = affichernbreprofpouruncours (courant_cours->sigle);

	while (courant_prof != NULL)
	{
		courant_cours = courant_prof->listecours;
		while (courant_cours != NULL)
		{
			tampon = affichernbreprofpouruncours (courant_cours->sigle);
			if (nbre < tampon)
			{
				nbre = tampon;
				coursrecherche = courant_cours;
			}
			courant_cours = courant_cours->suivant;
		}
		courant_prof = courant_prof->suivant;
	}
	plusdemande += coursrecherche->sigle;
	if (nbre == 0)
		return "0";
	else
		return plusdemande;
}



void DossierProfesseur::recopier (std::string FP)
{
	std::ofstream ficEcriture(FP,std::ios::out); /* je n'es peut-être pas utilisé le bon mode */
    courant_prof = tete;
    while (courant_prof != NULL) // Ouverture en monde écriture
    {
        ficEcriture << courant_prof->nom << std::endl; // mets la valeur de courant_prof->Nom
		ficEcriture << courant_prof->prenom << std::endl; // Mets la valeur de courant_prof ->prenom
        courant_cours=courant_prof->listecours;
        while (courant_cours->sigle != "&")
        {
            ficEcriture << courant_cours->sigle << std::endl;
            courant_cours=courant_cours->suivant;
        }
        ficEcriture << "&" << std::endl;
        courant_etu=courant_prof->listetudiant;
        while (courant_etu->nom != "&")
        {
            ficEcriture << courant_etu->nom << std::endl;
            courant_etu=courant_etu->apres;
        }
        ficEcriture << "&";
		if (courant_prof->suivant != NULL)
			ficEcriture << std::endl;
        courant_prof = courant_prof->suivant;
	}
    ficEcriture.close();
	courant_prof = tete;
}

void main ()
{
	std::string cours;
    std::string desnom, desprenom;
	std::ifstream ft_entree;
	char choix;
	int coursplusdemande;
	DossierProfesseur first ("FP.txt");

	afficher ();
	ft_entree.open ("test.txt");
	if (ft_entree.fail ())
	{
		std::cout << "Erreur d'ouverture";
	}
	else // CREE TETE LISTE
	{
		while (!ft_entree.eof())
		{
			ft_entree >> choix;
			switch (choix)
			{
			case '-':ft_entree >> desnom >> desprenom;
					 first.supprimer(desnom, desprenom);
					 break;
			case '#':std::cout << std::endl << "Le/s prof/s qui a le plus d'etudiant est/sont " << first.afficherleprofplusetudiant( );
					 break;
			case '*':std::cout << std::endl << "Le cours le plus demande est " << first.affichercoursplusdemande ( );
					 break;
			case '%':ft_entree >> cours;
					 coursplusdemande = first.affichernbreprofpouruncours (cours);
					 if (coursplusdemande == 0)
						 std::cout << std::endl << "Ce cours n'existe pas";
					 else
						std::cout << std::endl << "Le nombre de profs pour le cours " << cours << " est de " << coursplusdemande;
					 break;
			case '$':first.recopier  ("FP.txt");
					 break;
			default: std::cout << std::endl << "L'operation demandee n'est pas executable !";
					 break;
			}
		}
	}
	system ("pause");
}

void afficher (void)
{
	while (courant_prof!=NULL)
	{
		std::cout << std::endl << "-------------------------------------------------------------------" << std::endl;
		std::cout << courant_prof->nom << "  " << courant_prof->prenom << std::endl;
		courant_cours = courant_prof->listecours;
		while (courant_cours->suivant != NULL)
		{
			std::cout << courant_cours->sigle << std::endl;
			courant_cours = courant_cours->suivant;
		}
		courant_etu = courant_prof->listetudiant;
		while (courant_etu->apres != NULL)
		{
			std::cout << courant_etu->nom << std::endl;
			courant_etu = courant_etu->apres;
		}
		courant_prof = courant_prof->suivant;
	}
}
