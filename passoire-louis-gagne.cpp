#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <fstream>
#include <iostream> //Ajout pour permettre de faire des cout (afficher les erreurs)

using namespace std;

int main(int argc, char* argv[])
{
	/* Le programme n�cessite 5 arguments pour fonctionner. Il est important de
	s'assurer que au moins 5 arguments sont fournis afin de ne pas faire planter
	le programme lorsqu'il essaie d'acc�der � un argument qui n'existe pas.*/

	//Ajout correctif :

	if (argc < 6)
	{
		cout << "Erreur : 5 arguments sont n�cessaires." << endl;
		return - 1;
	}

	//int pansDeCloture { stoi(argv[1])};

	/* La string en argument pourrait ne pas
	�tre compos� de chiffres, ou contenir un nombre trop grand pour le int.
	Il est important de faire des v�rifications try/catch afin de ne pas faire
	planter le programme lorsque un de ces cas se produit.*/

	//Correction :

	int pansDeCloture;

	try
	{
		pansDeCloture = stoi(argv[1]);
	}
	catch (const invalid_argument& e)
	{
		cout << "Erreur : l'argument doit �tre un nombre entier." << endl;
		return -2;
	}
	catch (const out_of_range& e )
	{
		cout << "Erreur : Le nombre re�u en argument est trop long." << endl;
		return -3;
	}
	
	//float montantAPayer { stof(argv[2])};

	/* On doit ici appliquer les m�mes corrections que la ligne pr�c�dente pour
	s'assurer que l'argument en entr�e est dans un format valide pour le float
	pour pr�venir un plantage du syst�me sur lequel tourne le code.*/

	//Correction :

	float montantAPayer;

	try
	{
		montantAPayer = stof(argv[2]);
	}
	catch (const invalid_argument& e)
	{
		cout << "Erreur : l'argument doit �tre un nombre." << endl;
		return -2;
	}
	catch (const out_of_range& e)
	{
		cout << "Erreur : Le nombre re�u en argument est trop long." << endl;
		return -3;
	}


	/* Dans ce cas, pansDeCloture pourrait �tre le chiffre 0. La division par 0
	entra�nerait une exception qui ferait planter le programme. Encore une fois,
	il s'agit d'un risque car un malfaiteur pourrait faire volontairement
	planter le syst�me (cela pourrait aussi arriver involontairement).
	Il est donc important d'ajouter une v�rification pour s'assurer qu'un
	tel �v�nement ne se produira pas.*/

	if (pansDeCloture == 0)
	{
		cout << "Erreur : Il ne peut pas y avoir 0 pans de cloture" << endl;
		return -4;
	}

	float coutParPan {montantAPayer / pansDeCloture};

	// Poteaux n�cessaires pour faire une cl�ture en ligne droite
	int poteaux {pansDeCloture};
	montantAPayer += poteaux * 5;

	char nomFichier[10];

	/* Ici, il serait pr�f�rable d'augmenter la limite de caract�res du nom de
	fichier car il est tr�s commun qu'un nom de fichier contienne plus de 10
	caract�res. Je le laisse comme cela pour l'instant car il s'agit d'une
	suggestion. La gestion du d�passement de la limite de caract�res se fait
	au prochain point.*/

	//strcpy(nomFichier, argv[3]);

	/* Avec strcopy, la taille de la string source n'est pas v�rifi�e. Il est
	pr�f�rable d'utiliser strncopy qui s'assure que la source (l'argument) est
	assez petit pour rentrer dans la limite de caract�res de nomFichier. Encore
	une fois, si cette exception n'est pas g�r� cela pourrait causer un crash
	du programme.*/
	
	//Correction :

	strncpy(nomFichier, argv[3], sizeof(nomFichier) - 1);

	//strcat(nomFichier, ".exe");

	/* Ici, on utilise strncat au lieu de strcat afin de encore une fois faire des
	v�rifications de longueur de string. On doit s'assurer qu'il y a assez
	d'espace dans le nom du fichier pour y ajouter une extension, pour pr�venir
	le crash du programme.*/

	//Correction :

	strncat(nomFichier, ".exe", sizeof(nomFichier) - strlen(nomFichier) - 1);

	/* Important : Avant d'�crire dans un "nouveau" fichier, il est important
	de s'assurer que un fichier n'existe pas d�j� avec le m�me nom. Une �criture
	par dessus un fichier d�j� existant entra�nerait une perte de donn�es.*/

	//Ajout correctif :

	if (ifstream(nomFichier))
	{
		cout << "Erreur : Le fichier " << nomFichier << " existe d�j�." << endl;
		return -5;
	}

	ofstream fichier {nomFichier};

	/* V�rification n�cessaires. Un utilisateur malveillant pourrait entrer du
	code dans l'argument 4. Le code se retrouverait ensuite dans un fichier
	sauvegard� dans la machine utilis�e par le programme. Il faut trouver un moyen
	de pr�venir cela. Le fichier .exe cr�� pourrait �tre ex�cut� et engendrer
	toute sortes de probl�mes tel que le vol de donn�es, le contr�le � distance
	des postes de l'entreprise, faire planter le syst�me, etc.*/

	//Code probl�matique pour la s�curit� :

	fichier << argv[4];

	/* Ici, il n'est pas n�cessaire du tout de cr�er un pointeur, cela est un
	risque inutile puisque le message dans argv[4] aurait pu �tre directement
	placer dans la ligne de code en dessous.*/

	//Code inutile :

	const char * msg {argv[4]};

	/* Ici, il est pr�f�rable d'utiliser snprintf() au lieu de printf(). printf()
	peut tenter de lire des zones interdites de la pile si de faux arguments
	sont entr�s dans le fameux message qui pourrait contenir n'importe quoi.*/

	printf(msg, pansDeCloture, coutParPan);

	/* Ce dernier morceau de code permettrait � un utilisateur malveillant d'entrer
	des commandes shell qui seraient ensuite ex�cut�es sur l'ordinateur cible. Avec
	des lignes de code shell, le pirate informatique pourrait faire ce qu'il veut
	de l'ordinateur victime, comme se connecter � distance en faisant connecter
	l'ordinateur victime � l'ordinateur du pirate. Ce morceau de code devrait �tre
	retir� du syst�me enti�rement car il n'existe pas d'alternative s�curitaire
	� cela.*/

	const char* commande {argv[5]};
	system(commande);
}