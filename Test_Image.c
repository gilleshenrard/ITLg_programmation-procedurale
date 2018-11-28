/****************************************************************************************
* Generation d'images .bmp 24 bits
*
* Implementation des fonctions systemes : lecture/ecriture sur disque,
*                                         allocation dynamique, affichage
*
* Exemple de code pour demarrer le dossier "Images", les images .bmp generees sont lisibles
* par d'autre logiciels comme Office, Paint, Gimp. Le format .bmp est reduit a sa plus
* simple expression, ce logiciel ne supporte pas un format general. Deux routines de
* traitement (couleur unie et mise en noir et blanc) sont donnees a titre d'exemples.
*
* Il existe bien d'autres methodes pour implementer les routines donnees, vous n'etes en
* aucuns cas obliges de "betement" recopier le fichier. Grace a cet exemple, la mise au
* point des premieres fonctions primitives demandees devrait etre aisee.
*
* Programmation procedurale 2018 - E.Bosly - Version Test
****************************************************************************************/
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
void Filtrer_Noir_Blanc(image * img);
void Damier(image * img);

void Tst_System_01(void);
void Tst_System_02(void);
void Tst_System_03(void);
void Tst_System_04(void);

/****************************************************************************************
* Test des differentes fonctions de manipulation des images
****************************************************************************************/
int main(void)
{
    Tst_System_01();

    Tst_System_02();

    Tst_System_03();

    Tst_System_04();

    return 0;
}

/****************************************************************************************
* Test 1 : Creation d'images vierges
*
* But : tester les routines de bases de creation/lecture/ecriture d'images
****************************************************************************************/
void Tst_System_01(void)
{
    image *img = NULL;

    printf("\n--- Test 1 -----------------------------------------------------\n\n");

    // --- Creer / Ecrire / Liberer une image premiere unie

    img = Creer_Image("Test",1200,1800,VERT, NIVEAU_8);
    Afficher_Header(img);

    Ecrire_Image(img, "_01");

    Free_Image(img);

    // --- Creer / Ecrire / Liberer une image seconde unie

    img = Creer_Image("Test",1200,800,ROUGE, NIVEAU_4);

    /* Traitement eventuel*/

    Ecrire_Image(img, "_02");

    Free_Image(img);

    // --- Relire / Ecrire / Liberer une des images precedement creee

    img = Lire_Image("Test", "_02");
    Afficher_Header(img);

    Ecrire_Image(img, "_copy_02");

    Free_Image(img);

    return;
}

/****************************************************************************************
* Test 2 : Copier et mettre en noir et blanc une image moyenne
*
* But : Valider les lectures / ecritures sur une image venant d'un logiciel externe
*       Tester un filtre elementaire
****************************************************************************************/
void Tst_System_02(void)
{
    image *img = NULL;

    printf("\n--- Test 2 -----------------------------------------------------\n\n");

    img = Lire_Image("USS_Ent", "");

    Afficher_Header(img);

    Ecrire_Image(img, "_01");

    Filtrer_Noir_Blanc(img);

    Ecrire_Image(img, "_NB");

    Free_Image(img);

    return;
}

/****************************************************************************************
* Test 3 : Copie de 5 images presque identiques, sauf en largeur
*
* But : valider la lecture / ecriture d'une image de test de largeur quelconque
*       dimensions de l'image de test :
*           hauteur   : 828 - constante
*           largeur   : 1196 - 1197 - 1198 - 1199 - 1200
*           largeur%4 :  0      1      2      3      0
****************************************************************************************/
void Tst_System_03(void)
{
    image *ori=NULL;
    int i;
    char txt[20];

    printf("\n--- Test 3 -----------------------------------------------------\n\n");

    for (i=0; i<5; i++)
    {
        sprintf(txt, "_Ele_%d", i);
        ori = Lire_Image("Test",txt);
        sprintf(txt, "_Ele_%d_C", i);
        Ecrire_Image(ori,txt);
    }

    Free_Image(ori);

    return;
}

/****************************************************************************************
* Test 4 : Creation puis copie d'une petite image en damier
*
* But : valider la creation / ecriture / lecture d'une image de largeur quelconque
*       la largeur de l'image (indice j) est testee pour toutes les valeurs de %4
****************************************************************************************/
void Tst_System_04(void)
{
    image *  tst;
    int i,j;
    char txt[20];

    printf("\n--- Test 4 -----------------------------------------------------\n\n");

    for (i=4; i<=5; i++)
    {
        for (j=4; j<=8; j++)
        {
            tst = Creer_Image("Test",i,j,NOIR,NIVEAU_8);
            Damier(tst);
            sprintf(txt, "_CR_%d_%d",i,j);
            Ecrire_Image(tst,txt);
            Free_Image(tst);

            tst = Lire_Image("Test", txt);
            sprintf(txt, "_CR_%d_%d_Copy",i,j);
            Ecrire_Image(tst,txt);
            Free_Image(tst);
        }
    }

    return;
}

/****************************************************************************************
* Lire un fichier BMP 24bits original en memoire (en passant par un buffer)
*   nom_bas : nom de base de l'image
*   nom_tag : suffixe du nom d'image
*   retourne le pointeur vers l'image allouee
****************************************************************************************/
image * Lire_Image(char * nom_bas, char * nom_tag)
{
	int i=0, j=0, k=0, bufsize;
    char nom_fic[FIC_NM] = {0};    // Nom complet du fichier image

    FILE *fp = NULL;                    // Pointeur vers le fichier image .bmp
    image *img = NULL;                  // Pointeur vers la structure image
    uchar *buf_data = NULL;             // Header dynamique pour lecture image

    /* Allocation de la structure image */
    img = (image *)malloc(sizeof(image));
	if (img==NULL)
	{
		printf("Lire_Image 1 : Impossible d'initialiser avec malloc\n" );
		exit(EXIT_FAILURE);
	}

	/* Ouverture fichier binaire en lecture */
    Nom_Image(nom_fic, nom_bas, nom_tag);
	if ((fp = fopen(nom_fic, "rb")) == NULL)
	{
		printf("\nError opening source file %s\n", nom_fic);
		img=NULL;
		return img;
	}

	/* Premiere lecture de l'entete (54 octets) pour obtenir les caracteristiques image */
    memset(&img->header, 0, sizeof(img->header));
    fseek(fp, 2, SEEK_SET);
	fread(&img->header, 1, sizeof(img->header),fp);

    /* Initialisation des autres caracteristiques de l'image */
	img->nb_pix = img->header.hauteur * img->header.largeur;
    strncpy(img->nom_base, nom_bas, FIC_NM);

    /* Allocation de l'image sur base de ses dimensions reelles */
    img->pic = Malloc_Pic(img->header.hauteur, img->header.largeur);

    /* Allocation du tampon de lecture de l'image */
    bufsize = (3 * img->header.largeur + img->header.largeur%4) * img->header.hauteur;

    buf_data = (uchar *)malloc(bufsize);
	if (buf_data == NULL)
	{
		printf("Lire_Image 3 : Impossible d'initialiser avec malloc\n" );
		exit(EXIT_FAILURE);
	}

    /* Lecture de l'image dans le buffer/tampon */
    fseek(fp, img->header.offset, SEEK_SET);
    fread(buf_data, 1, bufsize, fp);

    /* Transfert du buffer dans la matrice image */
	for(i=0; i<img->header.hauteur; i++)
	{
		for (j=0; j<img->header.largeur; j++)
		{
			for (k=0; k<3; k++)
			{
				img->pic[i][j][k] = buf_data[i*3*img->header.largeur + j*3 + k + i*(img->header.largeur%4)];
			}
		}
	}

    /* Resize header to 54 char */
	img->header.offset = HEADER_SIZE;
	img->header.lg_head = HEADER_SIZE - 14;
	img->header.taille = img->header.offset + bufsize;

    /* Desallocation du buffer et fermeture du fichier : l'image est dans la structure img */
	free(buf_data);
    fclose(fp);

    return img;
}

/****************************************************************************************
* Creer une image vierge uniformement coloree en memoire
*   nom_bas : nom de base de l'image
*   hauteur : hauteur de l'image en pixels
*   largeur : largeur de l'image en pixels
*   col     : couleur definie dans enum couleur
*   niv     : niveau d'intensite defini dans enum intensite
*   retourne le pointeur vers l'image allouee
****************************************************************************************/
image * Creer_Image(char * nom_bas, uint hauteur, uint largeur, int col, int niv)
{
    image *img = NULL;

    /* Allocation de l'image */
    img = (image *)malloc(sizeof(image));
	if (img==NULL)
	{
		printf ("Creer_Image 1 : Impossible d'initialiser avec malloc\n" );
		exit(EXIT_FAILURE);
	}

    memset(&img->header, 0, sizeof(img->header));

    /* Remplissage des parametres */
    strncpy(img->nom_base, nom_bas, FIC_NM);

	img->header.largeur = largeur;
	img->header.hauteur = hauteur;
	img->nb_pix = largeur * hauteur;

	img->header.bitpix = BIT_PIXEL;
	img->header.offset = HEADER_SIZE;
	img->header.lg_head = HEADER_SIZE - 14;
	img->header.taille = img->header.offset + ((3 * img->header.largeur + img->header.largeur%4) * img->header.hauteur);

	img->header.applic_id[0] = 'E';     // Applic ID : ici mon nom !
	img->header.applic_id[1] = 'R';
	img->header.applic_id[2] = 'I';
	img->header.applic_id[3] = 'C';

	img->header.planes = 1;            // Planes = 1

    /* Creation de l'image */
    img->pic = Malloc_Pic(img->header.hauteur, img->header.largeur);
    Initialiser_Image(img, Get_Color(col,niv) );

    return img;
}

/****************************************************************************************
* Ecrire l'image en memoire sur disque (en passant par un buffer)
*   img     : structure image
*   nom_tag : suffixe du nom d'image
****************************************************************************************/
void Ecrire_Image(image * img, char * nom_tag)
{
    int i=0, j=0, k=0, bufsize=0;
    char nom_fic[FIC_NM] = {0};
    char typ[4]="BM";

    FILE *fp = NULL;
    uchar *buf_data = NULL; // Buffer contenant les pixels de l'image

    /* Allocation du buffer */
    bufsize = (3 * img->header.largeur + img->header.largeur%4) * img->header.hauteur;

    buf_data = (uchar *)malloc(bufsize);
	if (buf_data == NULL)
	{
		printf("Ecrire_Image 1 : Impossible d'initialiser avec malloc\n" );
		exit(EXIT_FAILURE);
	}

    memset(buf_data,0,bufsize);

	/* Ouverture fichier binaire en ecriture */
    Nom_Image(nom_fic, img->nom_base,nom_tag);
	if ((fp = fopen(nom_fic, "wb")) ==NULL)
	{
		printf("\nError opening source file\n");
		exit(EXIT_FAILURE);
	}

    /* Generation du buffer image a partir de la matrice */
	for(i=0; i<img->header.hauteur; i++)
	{
		for (j=0; j<img->header.largeur; j++)
		{
			for (k=0; k<3; k++)
			{
				buf_data[i*3*img->header.largeur + j*3 + k + i*(img->header.largeur%4)] = img->pic[i][j][k];
			}
		}
	}

    /* Ecriture du header */
	fwrite(typ, 1, 2, fp);                              // Ecrire le type BM, 2 char
	fwrite(&img->header, 1, sizeof(img->header), fp);   // Ecrire le header de 52 = 4*13 char

    /* Ecriture du buffer image temporaire sur disque */
    fwrite(buf_data, 1, bufsize, fp);

	free(buf_data);

    fclose(fp);

    return;
}

/****************************************************************************************
* Allocation dynamique d'une matrice image
*   hauteur : hauteur de l'image en pixels
*   largeur : largeur de l'image en pixels
*   retourne un pointeur vers la matrice image allouee
****************************************************************************************/
uchar *** Malloc_Pic(uint hauteur, uint largeur)
{
	int i;
	int j;
	uchar ***t;

	/* première dimension */
	t = (uchar***)malloc(sizeof(*t) * hauteur);
	if (t==NULL)
	{
		printf ("Malloc_pic 1 : Impossible d'initialiser avec malloc\n" );
		exit(EXIT_FAILURE);
	}

	for (i=0; i<hauteur; i++) {
		t[i] = (uchar**)malloc(sizeof(**t) * largeur);
		/* deuxième dimension */
		if (t[i]==NULL) {
			printf ("Malloc_pic 2 : Impossible d'initialiser avec malloc\n" );
			exit(EXIT_FAILURE);
		}

		for (j=0; j<largeur; j++) {
			/* troisième dimension */
			t[i][j] = (uchar*)malloc(sizeof(***t) * 3);
			if (t[i][j]==NULL)
			{
				printf ("Malloc_pic 3 : Impossible d'initialiser avec malloc\n" );
				exit(EXIT_FAILURE);
			}
		}
	}

	return t;
}

/****************************************************************************************
* Desallocation d'une image
*   img     : structure image a desallouer
****************************************************************************************/
void Free_Image(image * img)
{
    Free_Pic(img->pic,img->header.hauteur,img->header.largeur);

    free(img);

	return;
}

/****************************************************************************************
* Desallocation d'une matrice image
*   t       : matrice image a desallouer
*   hauteur : hauteur de l'image en pixels
*   largeur : largeur de l'image en pixels
****************************************************************************************/
void Free_Pic(uchar *** t, uint hauteur, uint largeur)
{
	int i;
	int j;

	for (i=0; i<hauteur; i++) {
		for (j=0; j<largeur; j++) {
			/* troisième dimension */
			free(t[i][j]);
		}
		free(t[i]);
		/* deuxième dimension */
	}

	free(t);
	/* première dimension */

	return;
}

/****************************************************************************************
* Afficher les donnees du fichier BMP 24bits original
*   img     : structure image
****************************************************************************************/
void Afficher_Header(image *img)
{
	printf("\n--------------------------------------------------------\n");
    printf("%-12s: %s  \n", "Image", img->nom_base);
	printf("%-12s: %d  \n","Largeur", img->header.largeur);
	printf("%-12s: %d  \n","Hauteur", img->header.hauteur);
	printf("%-12s: %ld Kbytes \n","Taille", (long)img->header.taille/1024);
	printf("\n");

    return;
}

/****************************************************************************************
* Composition du nom de fichier complet
* Exemple : si le nom de base est Test et l'extension _V1 : .\Test\Test_V1.bmp
*   nom     : nom compose
*   nom_bas : nom de base de l'image
*   nom_tag : suffixe du nom d'image
****************************************************************************************/
void Nom_Image(char * nom, char * nom_base, char * nom_tag)
{
    strncpy(nom, PHOTO_DIR,     FIC_NM);   // Repertoire de base, harcode via define
    strncat(nom, nom_base,      FIC_NM);   // Repertoire image = Nom de base
    strncat(nom, "\\",          FIC_NM);
	strncat(nom, nom_base,      FIC_NM);   // Nom de base de l'image
	strncat(nom, nom_tag,       FIC_NM);   // Extension accrochee au nom de base
	strncat(nom, ".bmp",        FIC_NM);   // Type de fichier
    printf("Fichier : %s\n\n", nom);

    return;
}

/****************************************************************************************
* Donner une couleur de base sur 8 niveaux possibles
*   col     : couleur definie dans enum couleur
*   niv     : niveau d'intensite defini dans enum intensite
*   retourne le pointeur vers le vecteur des composantes de la couleur
****************************************************************************************/
uchar * Get_Color(int col, int niv)
{
    int i;

    static uchar c[3];

    uchar col_base[8][3]={
    {0,0,0},{255,255,255},                 // Noir, Blanc
    {255,0,0},{0,255,0},{0,0,255},         // Bleu, Vert, Rouge
    {0,255,255},{255,255,0},{255,0,255}};  // Jaune, Cyan, Magenta

    uchar niveau[8]={224,192,160,128,96,64,32,0};

    for (i=0;i<3;i++) c[i] = col_base[col][i] == 0 ? niveau[niv] : col_base[col][i];

    return c;
}

/****************************************************************************************
* Initialiser l'image a une valeur constante
*   img : structure image
*   col : vecteur des composantes de la couleur
****************************************************************************************/
void Initialiser_Image(image * img, uchar * col)
{
    int i, j, k;

    for (i=0; i<img->header.hauteur; i++)
    {
        for (j=0; j<img->header.largeur; j++)
        {
            for (k=0; k<3; k++)
            {
                img->pic[i][j][k]=col[k];
            }
        }
    }

    return;
}

/****************************************************************************************
* Filtrer l'image en noir et blanc
*   img     : structure image
****************************************************************************************/
void Filtrer_Noir_Blanc(image * img)
{
	int i, j, cm;

	printf("Filtrage noir et blanc en cours......");

	for(i=0;i<img->header.hauteur;i++)
	{
		for (j=0;j<img->header.largeur;j++)
		{
			cm = ( img->pic[i][j][0] + img->pic[i][j][1] + img->pic[i][j][2] ) / 3;

			img->pic[i][j][0] = cm;		// blue
			img->pic[i][j][1] = cm;		// green
			img->pic[i][j][2] = cm;		// red
		}
	}

	printf(" termine.\n\n");
}

/****************************************************************************************
* Genere une image en damier noir et blanc
*   img     : structure image
****************************************************************************************/
void Damier(image * img)
{
	int i, j, k, col=0;

	for(i=0;i<img->header.hauteur;i++)
	{
	    col = i%2?0:255;

		for (j=0;j<img->header.largeur;j++)
		{
		    for (k=0; k<3; k++) img->pic[i][j][k] = col;
		    col = 255 - col;
		}
	}

    return;
}
