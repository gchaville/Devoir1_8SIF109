#include "DossierProfesseur.h"
#include <iostream>
#include <string>
#include <fstream>
 
void afficher (void);
 
DossierProfesseur::DossierProfesseur (std::string FP) /* Constructeur */
 {
    std::ifstream entree;/* Ouverture en mode fichier */
 	tete = courant_prof = new Professeur;
	entree.open (FP.c_str()); /* Ouverture du fichier */
	if (entree.fail ()) /* Si le fichier n'ouvre pas */
 	{
 		std::cout << "Erreur d'ouverture";
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
		    if (!entree.eof()) 
 			{
 				courant_prof->suivant = new Professeur; // cree nouveau professeur si le fichier n'est pas fini
 				courant_prof = courant_prof->suivant;
 		    }
 		    courant_prof->suivant = NULL;
 	    }
	}
 	entree.close(); /* Fermeture du fichier */
    courant_prof = tete; // re-pointe courant prof sur la tete
 }

DossierProfesseur::~DossierProfesseur ( ) /*Destructeur */
 {
 	
	while (tete != NULL)/* boucle qui termine la chaine */
 	{
		courant_prof = tete;
		/*Pointe sur la liste etudiant */
		while (courant_prof->listetudiant != NULL) /*parcour la liste etudaint */
 		{
			courant_etu = courant_prof->listetudiant;
 			courant_prof->listetudiant= courant_etu->apres;
			delete courant_etu; /*Détruit un élément de la chaine */
 		}
 		
		while (courant_prof->listecours != NULL) /*rentre dans la chaine de cours */
 		{
			courant_cours = courant_prof->listecours;
 			courant_prof->listecours= courant_cours->suivant;
			delete courant_cours;/* destruction d'un element de la chaine */
 		}
		tete = tete->suivant;/*pointe sur la tete de l'élément prof */
		delete courant_prof; /* détruit l'element prof */	
 	}
    courant_prof = tete; /* redonne la position de la tete */
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
		precedent = courant_prof; /* Précédent va prendre la position du professeur courant et elle peut sortir par la suite */
 		courant_prof = courant_prof->suivant;
 	}
 	if (courant_prof == NULL) // Si le prof recherché n'a pas été retrouvé
 	{
 		std::cout << "Le professeur recherche n'existe pas." << "("+ nom + " " + prenom+ ")" << std::endl;
 	}
	else if (courant_prof->nom != nom || courant_prof->prenom != prenom)
	/* Soit le nom ou soit le prenom est comme dans le fichier */
	{
 		std::cout << "Le professeur recherche n'existe pas." << "("+ nom + " " + prenom+ ")" << std::endl;
 	}
	else
	{
		std::cout << "Le professeur trouve et supprime." << std::endl;
		if (precedent != NULL) /* si precedent a pris la position de courant prof*/
 			precedent->suivant = courant_prof->suivant;
		else // si precedent est égale à null, courant_prof pointe sur la tete
 			tete = tete->suivant;
 		delete courant_prof;
		recopier ("FP.txt");//appel la fonction recopier qui copie les modifications
 	}
 	courant_prof = tete;
 }
 
 std::string DossierProfesseur::afficherleprofplusetudiant ( ) const
 {
	int nb_etudiants, tampon; // Tampon comparaison entre les professeurs 
 	Professeur *profrecherche; //Tampon pour mémoriser le professeur ayant le plus d'etudiants
 	std::string nomrecherche;
 
    profrecherche = courant_prof = tete; //initialisation a la tete
 	courant_etu = courant_prof->listetudiant;
 
 	for (nb_etudiants = 0; courant_etu->apres != NULL; nb_etudiants++)
		{ /* Compte le nombre de d'etudiants de la tete */
 			courant_etu = courant_etu->apres;
 		}
    courant_prof = courant_prof->suivant;
    courant_etu = courant_prof->listetudiant;
    while (courant_prof != NULL) // Jusqu'a la fin de la liste
 	{	
        courant_etu = courant_prof->listetudiant;
        for (tampon = 0; courant_etu->apres != NULL; tampon++)
        {
            courant_etu = courant_etu->apres;
        }
		if (tampon > nb_etudiants) /* Si tampon est plus grand */
 		{
			nb_etudiants = tampon; /* On donne le nombre a nb_etudiant */
			nomrecherche = ""; // Correction bugg du a la 2nd condition
			profrecherche = courant_prof; /* On pointe sur le profrecherche */
 		}
 		else if (tampon == nb_etudiants && courant_prof->prenom != profrecherche->prenom)
		{ /* Condition pour 2 professeur avec le même nombre d'etudiants */
 			nomrecherche = courant_prof->nom + " " + courant_prof->prenom + " et ";
 		}
 		courant_prof = courant_prof->suivant;
 	}
	nomrecherche += profrecherche->nom + " " + profrecherche->prenom; /* Donne le nom et prenom du profrecherche*/
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
			if (coursdonne  == courant_cours->sigle) /* Si le nom de la variable recus*/
				nbprofpourcours += 1; /* Incrémentation dans nôtre compteur*/
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
	/*Le nombre est égal au cours le plus demander par les professeurs */
 	while (courant_prof != NULL) /* Boucle qui traverse notre liste chaine */
	{
 		courant_cours = courant_prof->listecours;
 		while (courant_cours != NULL) /* Liste qui traverse les cours */
		{
 			tampon = affichernbreprofpouruncours (courant_cours->sigle);
			if (nbre < tampon) /* Vérifie si nbre est inférieur a tampon */
 			{
				nbre = tampon; /* nbre recois la valeur de tampon */
 				coursrecherche = courant_cours; 
 			}
 			courant_cours = courant_cours->suivant;
        }
        courant_prof = courant_prof->suivant;
    }
    plusdemande += coursrecherche->sigle;
    return plusdemande;
}
 void DossierProfesseur::recopier (std::string FP)
 {
	std::ofstream ficEcriture(FP,std::ios::out); // Ouverture en monde écriture
    courant_prof = tete;
    while (courant_prof != NULL) // Parcours la liste chainee
    {
        ficEcriture << courant_prof->nom << std::endl; // mets la valeur de courant_prof->Nom
 		ficEcriture << courant_prof->prenom << std::endl; // Mets la valeur de courant_prof ->prenom
        courant_cours=courant_prof->listecours;
        while (courant_cours->sigle != "&") // Parcours la liste des cours
         {
             ficEcriture << courant_cours->sigle << std::endl;
             courant_cours=courant_cours->suivant;
         }
        ficEcriture << "&" << std::endl; // Marque la fin de la liste de cours
        courant_etu=courant_prof->listetudiant;
        while (courant_etu->nom != "&") // Parcours la liste des etudiants
         {
             ficEcriture << courant_etu->nom << std::endl;
             courant_etu=courant_etu->apres;
         }
        ficEcriture << "&"; // Marque la fin de la liste des etudiants
		if (courant_prof->suivant != NULL) /* Vérification si la chaine continu */
 			ficEcriture << std::endl;
         courant_prof = courant_prof->suivant;
 	}
    ficEcriture.close();
    courant_prof = tete;
}

void main ()
 {
 	std::string cours; // Cours a rechercher
    std::string desNom, desPrenom; /* Nom et prenom du prof a supprimer */
	std::ifstream ft_entree; /* Fichier qui contient les instruction */
	char choix; 
 	int coursplusdemande;

    DossierProfesseur first ("FP.txt"); /* Lecture du Constructeur avec le fichier "FP.TXT" */

	afficher (); /* Permet de voir le fichier lu avec les informations*/
	ft_entree.open ("FT.txt"); /* Ouverture des instructions */
 	if (ft_entree.fail ())
 	{
 		std::cout << "Erreur d'ouverture";
 	}
 	else 
 	{
		while (!ft_entree.eof()) /* tant que le fichier d'instruction n'a pas termine */
 		{
 			ft_entree >> choix;
			switch (choix)
 			{
			case '-':ft_entree >> desNom >> desPrenom;
					 first.supprimer(desNom, desPrenom); /* Appel d'une fonction */
 					 break;
			case '#':std::cout << "Le/s prof/s qui a le plus d'etudiant est/sont " << first.afficherleprofplusetudiant( ) << std::endl; /* Appel d'une fonction */
 					 break;
			case '*':std::cout << "Le cours le plus demande est " << first.affichercoursplusdemande ( ) << std::endl; /* Appel d'une fonction */
 					 break;
 			case '%':ft_entree >> cours;
					 coursplusdemande = first.affichernbreprofpouruncours (cours); /* Appel d'une fonction */
 					 if (coursplusdemande == 0)
 						 std::cout << "Ce cours n'existe pas." << "(" + cours + ")" << std::endl;
 					 else
 						std::cout << "Le nombre de profs pour le cours " << cours << " est de " << coursplusdemande << std::endl;
 					 break;
			case '$':first.recopier  ("FP.txt"); /* Appel d'une fonction */
 					 break;
 			default: std::cout << "L'operation demandee n'est pas executable !" << std::endl;
 					 break;
			}
		}
	}
	ft_entree.close ();
	system ("pause");
}

void afficher (void)
 {
	int nbreprof=0;
 	while (courant_prof!=NULL)
	{
		nbreprof++;
		std::cout << std::endl << "PROFESSEUR #" << nbreprof;
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
	std::cout << std::endl;
}
