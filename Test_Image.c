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
#include "lib/image.h"
#include "lib/effects.h"

void Tst_System_01(void);
void Tst_System_02(void);
void Tst_System_03(void);
void Tst_System_04(void);
void Tst_Embed_complete(void);
void Tst_Embed_cropped(void);
void Tst_Embed_alpha(void);

/****************************************************************************************
* Test des differentes fonctions de manipulation des images
****************************************************************************************/
int main(void)
{
    //Tst_System_01();

    //Tst_System_02();

    //Tst_System_03();

    //Tst_System_04();

    //Tst_Embed_complete();

    //Tst_Embed_cropped();

    Tst_Embed_alpha();

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
* Test_Embed_complete : Embedding an image in another one (full image in the frame)
*
* Purpose : testing that a simple embedding case occurs properly (at the right coord.)
****************************************************************************************/
void Tst_Embed_complete(void)
{
    image *  tst=NULL, *ship=NULL, *space_back=NULL;

    printf("\n--- Test Embed Complete -----------------------------------------------------\n\n");
    ship = Lire_Image("Test", "_Enterprise");
    space_back = Lire_Image("Test", "_Field");

    tst = embed_image(ship, space_back, 0, 0, 1.0);
    strncpy(tst->nom_base, "Test", FIC_NM);
    Ecrire_Image(tst,"_embed0");
    Free_Image(tst);

    tst = embed_image(ship, space_back, 100, 0, 1.0);
    strncpy(tst->nom_base, "Test", FIC_NM);
    Ecrire_Image(tst,"_embed_horizontal");
    Free_Image(tst);

    tst = embed_image(ship, space_back, 0, 100, 1.0);
    strncpy(tst->nom_base, "Test", FIC_NM);
    Ecrire_Image(tst,"_embed_vertical");
    Free_Image(tst);

    return;
}

/****************************************************************************************
* Test_Embed_cropped : Embedding an image in another one (partially cropped)
*
* Purpose : testing that an embedding made out of the frame crops the image
****************************************************************************************/
void Tst_Embed_cropped(void)
{
    image *  tst=NULL, *ship=NULL, *space_back=NULL;

    printf("\n--- Test Embed Cropped -----------------------------------------------------\n\n");
    ship = Lire_Image("Test", "_Enterprise");
    space_back = Lire_Image("Test", "_Field");

    tst = embed_image(ship, space_back, -200, 0, 1.0);
    strncpy(tst->nom_base, "Test", FIC_NM);
    Ecrire_Image(tst,"_x_neg");
    Free_Image(tst);

    tst = embed_image(ship, space_back, 600, 0, 1.0);
    strncpy(tst->nom_base, "Test", FIC_NM);
    Ecrire_Image(tst,"_x_out");
    Free_Image(tst);

    tst = embed_image(ship, space_back, 0, -200, 1.0);
    strncpy(tst->nom_base, "Test", FIC_NM);
    Ecrire_Image(tst,"_y_neg");
    Free_Image(tst);

    tst = embed_image(ship, space_back, 0, 250, 1.0);
    strncpy(tst->nom_base, "Test", FIC_NM);
    Ecrire_Image(tst,"_y_out");
    Free_Image(tst);

    tst = embed_image(ship, space_back, 10000, 10000, 1.0);
    strncpy(tst->nom_base, "Test", FIC_NM);
    Ecrire_Image(tst,"_fully_out");
    Free_Image(tst);

    return;
}

/****************************************************************************************
* Test_Embed_alpha : Embedding an image in another one with various alpha values
*
* Purpose : testing that alpha value is taken into account when embedding
****************************************************************************************/
void Tst_Embed_alpha(void)
{
    image *  tst=NULL, *ship=NULL, *space_back=NULL;

    printf("\n--- Test Embed Alpha -----------------------------------------------------\n\n");
    ship = Lire_Image("Test", "_Enterprise");
    space_back = Lire_Image("Test", "_Field");

    tst = embed_image(ship, space_back, 10, 10, 1.0);
    strncpy(tst->nom_base, "Test", FIC_NM);
    Ecrire_Image(tst,"_alpha_full");
    Free_Image(tst);

    tst = embed_image(ship, space_back, 10, 10, 0.7);
    strncpy(tst->nom_base, "Test", FIC_NM);
    Ecrire_Image(tst,"_alpha_07");
    Free_Image(tst);

    tst = embed_image(ship, space_back, 10, 10, 0.4);
    strncpy(tst->nom_base, "Test", FIC_NM);
    Ecrire_Image(tst,"_alpha_04");
    Free_Image(tst);

    tst = embed_image(ship, space_back, 10, 10, 0);
    strncpy(tst->nom_base, "Test", FIC_NM);
    Ecrire_Image(tst,"_alpha_transparent");
    Free_Image(tst);

    return;
}
