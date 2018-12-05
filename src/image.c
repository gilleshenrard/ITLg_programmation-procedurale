#include "../lib/image.h"

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
*   PHOTO_DIR   : state variable (definition)
*   nom         : final full file name
*   nom_bas     : Base name (type. e.g. 'Ship')
*   nom_tag     : Suffix (e.g. '_test2')
****************************************************************************************/
void Nom_Image(char * nom, char * nom_base, char * nom_tag)
{
	sprintf(nom, "%s%s\\%s_%s.bmp", PHOTO_DIR, nom_base, nom_base, nom_tag);
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

/****************************************************************************************/
/*  I : x coordinate to test                                                            */
/*      y coordinate to test                                                            */
/*      image on which the coordinates are tested                                       */
/*  P : Tests if the coordinates are inside the frame                                   */
/*  O : 1 if inside the frame                                                           */
/*      0 otherwise                                                                     */
/****************************************************************************************/
int is_in_frame(uint x, uint y, image* frame){
    return x>=0 && x<frame->header.largeur && y>=0 && y<frame->header.hauteur;
}

/****************************************************************************************/
/*  I : x coordinate to test                                                            */
/*      y coordinate to test                                                            */
/*      image on which test the pixel                                                   */
/*  P : Tests if the pixel at the coordinates is white                                  */
/*  O : 1 if the pixel is white                                                         */
/*      0 otherwise                                                                     */
/****************************************************************************************/
int is_white_pixel(uint x, uint y, image* frame){
    return frame->pic[x][y][0]==255 && frame->pic[x][y][1]==255 && frame->pic[x][y][2]==255;
}

/****************************************************************************************/
/*  I : image to embed to                                                               */
/*      image to embed in                                                               */
/*      x coordinate of the overlay                                                     */
/*      y coordinate of the overlay                                                     */
/*      transparency factor                                                             */
/*  P : Embeds an image in another one (with transparency)                              */
/*  O : resulting image of the embedding                                                */
/*      NULL if error                                                                   */
/****************************************************************************************/
image* embed_image(image* overlay, image* background, uint x, uint y, float alpha){
    image* buffer = NULL;

    buffer = Creer_Image("", background->header.hauteur, background->header.largeur, NOIR, NIVEAU_8);

    for(uint i=0 ; i<background->header.hauteur ; i++){
        for(uint j=0 ; j<background->header.largeur ; j++){
            if(is_in_frame(i-y, j-x, overlay) && !is_white_pixel(i-y, j-x, overlay)){
                buffer->pic[i][j][0] = (overlay->pic[i-y][j-x][0] * alpha) + (background->pic[i][j][0] * (1.0-alpha));
                buffer->pic[i][j][1] = (overlay->pic[i-y][j-x][1] * alpha) + (background->pic[i][j][1] * (1.0-alpha));
                buffer->pic[i][j][2] = (overlay->pic[i-y][j-x][2] * alpha) + (background->pic[i][j][2] * (1.0-alpha));
            }
            else{
                buffer->pic[i][j][0] = background->pic[i][j][0];
                buffer->pic[i][j][1] = background->pic[i][j][1];
                buffer->pic[i][j][2] = background->pic[i][j][2];
            }
        }
    }
    return buffer;
}
