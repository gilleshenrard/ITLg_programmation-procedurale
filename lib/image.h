#ifndef IMAGE_H_INCLUDED
#define IMAGE_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/****************************************************************************************
* Constantes symboliques
****************************************************************************************/
#define HEADER_SIZE 54
#define BIT_PIXEL 24

#define FIC_NM 80

#define PHOTO_DIR ".\\"

enum couleur
{
	NOIR,
	BLANC,
	BLEU,
	VERT,
	ROUGE,
	JAUNE,		// Vert + Rouge
	CYAN,		// Bleu + Vert
	MAGENTA,	// Bleu + Rouge
	DEF_COL     // Defaut
};

enum intensite
{
	NIVEAU_1,   // Faible
	NIVEAU_2,
	NIVEAU_3,
	NIVEAU_4,   // Moyenne
	NIVEAU_5,
	NIVEAU_6,
	NIVEAU_7,
	NIVEAU_8,   // Forte
	DEF_NIV     // Defaut
};

/****************************************************************************************
* Definition de la structure de donnees image
****************************************************************************************/
typedef unsigned int uint;
typedef unsigned short int usint;
typedef unsigned char uchar;

typedef struct Header_Bitmap    // 52 char, SANS BM au debut sinon pas multiple de 4
{
	uint taille;        // taille du fichier
	char applic_id[4];  // identification de l'application creant le fichier
	uint offset;        // offset dans le fichier BMP du 1er pixel = longueur totale header
	uint lg_head;       // longueur totale header - 14
	uint largeur;       // largeur de l'image
	uint hauteur;       // hauteur de l'image
	usint planes;       // nb of planes - 1
	usint bitpix;       // nb bits par pixels - 24
	char fill[22];
} head_bmp;

typedef struct Image
{
    head_bmp header;        // contenu du header
    uchar ***pic;           // matrice image
    char nom_base[FIC_NM];  // nom de base de l'image sans extension
	uint nb_pix;            // nombre de pixels sur l'image
} image;

/****************************************************************************************
* Prototypes Systeme : Gestion des image sur disque et allocation dynamique memoire
****************************************************************************************/
image * Lire_Image(char * nom_bas, char * nom_tag);
image * Creer_Image(char * nom_bas, uint hauteur, uint largeur, int col, int niv);
void Ecrire_Image(image * img, char * nom_tag);
void Nom_Image(char * nom, char * nom_base, char * nom_tag);

uchar *** Malloc_Pic(uint hauteur, uint largeur);
void Free_Pic(uchar *** t, uint hauteur, uint largeur);
void Free_Image(image * img);

void Afficher_Header(image * img);
uchar * Get_Color(int col, int niv);

void Initialiser_Image(image * img, uchar * color);

int is_in_frame(uint x, uint y, image* frame);
int is_white_pixel(uint x, uint y, image* frame);
image* embed_image(image* overlay, image* background, uint x, uint y, float alpha);

#endif // IMAGE_H_INCLUDED
