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

#define MKDIR(x) "if not exist ..\\Films\\" #x " mkdir ..\\Films\\" #x

void point_center(image*, uchar* colour);
void Tst_System_01(void);
void Tst_System_02(void);
void Tst_System_03(void);
void Tst_System_04(void);
void Tst_Embed_complete(void);
void Tst_Embed_cropped(void);
void Tst_Embed_alpha(void);
void Tst_draw_line(char*, char);
void Tst_draw_line_cropped(char*, char);
void Tst_Rotate(void);
void Tst_Flip(void);
void Tst_Zoom(void);
void Tst_Dir_Tree(void);
void Tst_Weapon_Computation(void);

/****************************************************************************************
* Test des differentes fonctions de manipulation des images
****************************************************************************************/
int main(void)
{
//    Tst_System_01();
//    Tst_System_02();
//    Tst_System_03();
//    Tst_System_04();
//    Tst_Embed_complete();
//    Tst_Embed_cropped();
//    Tst_Embed_alpha();
//    Tst_draw_line("line_bresenham", 0);
//    Tst_draw_line("line_wu", 1);
//    Tst_draw_line_cropped("line_bresenham_cropped", 0);
//    Tst_draw_line_cropped("line_wu_cropped", 1);
//    Tst_Rotate();
//    Tst_Flip();
//    Tst_Zoom();
//    Tst_Dir_Tree();
    Tst_Weapon_Computation();

    return 0;
}

/*****************************************************************************************/
/*  I : image for which draw the center                                                  */
/*  P : Draws a point (3 pixels wide) at the center of an image                          */
/*  O : /                                                                                */
/*****************************************************************************************/
void point_center(image* img, uchar* colour){
    int centerX = img->header.largeur/2;
    int centerY = img->header.hauteur/2;

    for(int y=centerY-1 ; y<centerY+2 ; y++){
        for(int x=centerX-1 ; x<centerX+2 ; x++){
            set_pixel_rgba(img, x, y, colour, 1.0);
        }
    }
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
    system(MKDIR(Test\\Test_System_01));

    Ecrire_Image(img, "System_01", "01");

    Free_Image(img);

    // --- Creer / Ecrire / Liberer une image seconde unie

    img = Creer_Image("Test",1200,800,ROUGE, NIVEAU_4);

    /* Traitement eventuel*/

    Ecrire_Image(img, "System_01", "02");

    Free_Image(img);

    // --- Relire / Ecrire / Liberer une des images precedement creee

    img = Lire_Image("Test", "System_01", "02");
    Afficher_Header(img);

    Ecrire_Image(img, "System_01", "copy_02");

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
    system(MKDIR(USS_Ent));

    img = Lire_Image("USS_Ent", NULL, "");

    Afficher_Header(img);

    Ecrire_Image(img, NULL, "01");

    Filtrer_Noir_Blanc(img);

    Ecrire_Image(img, NULL, "NB");

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
    system(MKDIR(Test\\Test_System_03));

    for (i=0; i<5; i++)
    {
        sprintf(txt, "Ele_%d", i);
        ori = Lire_Image("Test","System_03", txt);
        sprintf(txt, "Ele_%d_C", i);
        Ecrire_Image(ori, "System_03", txt);
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

    system(MKDIR(Test\\Test_System_04));

    for (i=4; i<=5; i++)
    {
        for (j=4; j<=8; j++)
        {
            tst = Creer_Image("Test",i,j,NOIR,NIVEAU_8);
            Damier(tst);
            sprintf(txt, "CR_%d_%d",i,j);
            Ecrire_Image(tst,"System_04", txt);
            Free_Image(tst);

            tst = Lire_Image("Test", "System_04", txt);
            sprintf(txt, "CR_%d_%d_Copy",i,j);
            Ecrire_Image(tst,"System_04", txt);
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
    ship = Lire_Image("Ship", NULL, "Enterprise");
    space_back = Lire_Image("Star", NULL, "Field");
    system(MKDIR(Test\\Test_Embed));

    tst = copy_image(space_back);
    embed_image(ship, tst, 0, 0, 1.0);
    strncpy(tst->nom_base, "Test", FIC_NM);
    Ecrire_Image(tst,"Embed", "embed0");
    Free_Image(tst);

    tst = copy_image(space_back);
    embed_image(ship, tst, 100, 0, 1.0);
    strncpy(tst->nom_base, "Test", FIC_NM);
    Ecrire_Image(tst,"Embed", "embed_horizontal");
    Free_Image(tst);

    tst = copy_image(space_back);
    embed_image(ship, tst, 0, 100, 1.0);
    strncpy(tst->nom_base, "Test", FIC_NM);
    Ecrire_Image(tst,"Embed", "embed_vertical");

    Free_Image(tst);
    Free_Image(ship);
    Free_Image(space_back);

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
    ship = Lire_Image("Ship", NULL, "Enterprise");
    space_back = Lire_Image("Star", NULL, "Field");
    system(MKDIR(Test\\Test_Embed));

    //cropped left
    tst = copy_image(space_back);
    embed_image(ship, tst, -200, 0, 1.0);

    //cropped right
    embed_image(ship, tst, 600, 0, 1.0);

    //cropped bottom
    embed_image(ship, tst, 0, -200, 1.0);

    //cropped top
    embed_image(ship, tst, 0, 250, 1.0);

    //fully out
    embed_image(ship, tst, 10000, 10000, 1.0);
    strncpy(tst->nom_base, "Test", FIC_NM);
    Ecrire_Image(tst,"Embed", "embed_cropped");

    Free_Image(tst);
    Free_Image(ship);
    Free_Image(space_back);

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
    ship = Lire_Image("Ship", NULL, "Enterprise");
    space_back = Lire_Image("Star", NULL, "Field");
    system(MKDIR(Test\\Test_Embed));

    tst = copy_image(space_back);
    embed_image(ship, tst, 10, 10, 1.0);
    strncpy(tst->nom_base, "Test", FIC_NM);
    Ecrire_Image(tst,"Embed", "alpha_full");
    Free_Image(tst);

    tst = copy_image(space_back);
    embed_image(ship, tst, 10, 10, 0.7);
    strncpy(tst->nom_base, "Test", FIC_NM);
    Ecrire_Image(tst,"Embed", "alpha_07");
    Free_Image(tst);

    tst = copy_image(space_back);
    embed_image(ship, tst, 10, 10, 0.4);
    strncpy(tst->nom_base, "Test", FIC_NM);
    Ecrire_Image(tst,"Embed", "alpha_04");
    Free_Image(tst);

    tst = copy_image(space_back);
    embed_image(ship, tst, 10, 10, 0);
    strncpy(tst->nom_base, "Test", FIC_NM);
    Ecrire_Image(tst,"Embed", "alpha_transparent");

    Free_Image(tst);
    Free_Image(ship);
    Free_Image(space_back);

    return;
}

/****************************************************************************************
* Tst_draw_line : Drawing a line on an image
*
* Purpose : testing that lines are drawn properly with Bresenham's or Wu's algo
****************************************************************************************/
void Tst_draw_line(char* filename, char antialiasing)
{
    image *tst=NULL, *space_back=NULL;
    line lign = {400, 250, 0, 0, NULL, 1.0, antialiasing};
    int quarters[4]={BLEU, JAUNE, ROUGE, VERT};
    double angles[24]={0, 15, 30, 45, 60, 75, 90, 105, 120, 135, 150, 165,
                       180, 195, 210, 225, 240, 255, 270, 285, 300, 315, 330, 345};

    printf("\n--- Test Draw Line -----------------------------------------------------\n\n");
    space_back = Lire_Image("Star", NULL, "Field");
    tst = copy_image(space_back);
    system(MKDIR(Test\\Test_Line));

    for(int i=0 ; i<24 ; i++){
        lign.xb = 400 + (int)(200 * cos((angles[i]*M_PI)/180));
        lign.yb = 250 + (int)(200 * sin((angles[i]*M_PI)/180));
        lign.colour = Get_Color(quarters[i/6], NIVEAU_8);
        draw_line_generic(tst, &lign);
    }

    strncpy(tst->nom_base, "Test", FIC_NM);
    Ecrire_Image(tst,"Line", filename);

    Free_Image(tst);
    Free_Image(space_back);

    return;
}

/****************************************************************************************
* Tst_draw_line_Bresenham_cropped : Drawing a line which gets out of the image
*
* Purpose : testing that parts out of the picture are well ignored
****************************************************************************************/
void Tst_draw_line_cropped(char* filename, char antialiasing)
{
    image *tst=NULL, *space_back=NULL;
    line l1 = {350, 200, 900, 200, NULL, 1.0, antialiasing};          //horizontal, out on the right
    line l2 = {10, 600, 900, 700, NULL, 1.0, antialiasing};          //completely off the image (>0)
    line l3 = {-100, 200, 900, 300, NULL, 1.0, antialiasing};        //crossing from left to right
    line l4 = {400, -100, 500, 600, NULL, 1.0, antialiasing};         //crossing from to to bottom
    line l5 = {-100, -100, 900, -50, NULL, 1.0, antialiasing};     //completely off the image (<0)
    line l6 = {450, 300, -100, 300, NULL, 1.0, antialiasing};         //horizontal, out on the left
    line l7 = {400, 250, 400, 600, NULL, 1.0, antialiasing};       //vertical, out on the top
    line l8 = {500, 350, 500, -100, NULL, 1.0, antialiasing};      //vertical, out on the bottom

    printf("\n--- Test Draw Line Cropped-----------------------------------------------------\n\n");
    space_back = Lire_Image("Star", NULL, "Field");
    tst = copy_image(space_back);
    system(MKDIR(Test\\Test_Line));

    l1.colour = Get_Color(BLEU, NIVEAU_8);
    draw_line_generic(tst, &l1);
    l2.colour = Get_Color(ROUGE, NIVEAU_8);
    draw_line_generic(tst, &l2);
    l3.colour = Get_Color(JAUNE, NIVEAU_8);
    draw_line_generic(tst, &l3);
    l4.colour = Get_Color(VERT, NIVEAU_8);
    draw_line_generic(tst, &l4);
    l5.colour = Get_Color(MAGENTA, NIVEAU_8);
    draw_line_generic(tst, &l5);
    l6.colour = Get_Color(BLEU, NIVEAU_8);
    draw_line_generic(tst, &l6);
    l7.colour = Get_Color(MAGENTA, NIVEAU_8);
    draw_line_generic(tst, &l7);
    l8.colour = Get_Color(MAGENTA, NIVEAU_8);
    draw_line_generic(tst, &l8);

    strncpy(tst->nom_base, "Test", FIC_NM);
    Ecrire_Image(tst, "Line", filename);

    Free_Image(tst);
    Free_Image(space_back);

    return;
}

/****************************************************************************************
* Test_Rotate : Rotating an image
*
* Purpose : testing that pictures are properly rotated
****************************************************************************************/
void Tst_Rotate(void)
{
    image *tst=NULL, *tst2=NULL, *tst3=NULL, *ship=NULL, *background=NULL;

    printf("\n--- Test Rotate -----------------------------------------------------\n\n");
    ship = Lire_Image("Ship", NULL, "Enterprise");
    background = Lire_Image("Star", NULL, "Field");
    tst = copy_image(ship);
    system(MKDIR(Test\\Test_Rotate));

    tst2 = rotate_image(tst, 30, 0, 0);
    tst3 = copy_image(background);
    point_center(tst2, Get_Color(ROUGE, NIVEAU_8));
    embed_image(tst2, tst3, 100, 100, 1.0);
    strncpy(tst3->nom_base, "Test", FIC_NM);
    Ecrire_Image(tst3,"Rotate", "rotated_30");
    Free_Image(tst2);
    Free_Image(tst3);

    tst2 = rotate_image(tst, -30, 0, 0);
    tst3 = copy_image(background);
    point_center(tst2, Get_Color(ROUGE, NIVEAU_8));
    embed_image(tst2, tst3, 100, 100, 1.0);
    strncpy(tst3->nom_base, "Test", FIC_NM);
    Ecrire_Image(tst3,"Rotate", "rotated_-30");
    Free_Image(tst2);
    Free_Image(tst3);

    tst2 = rotate_image(tst, 60, 0, 0);
    tst3 = copy_image(background);
    point_center(tst2, Get_Color(ROUGE, NIVEAU_8));
    embed_image(tst2, tst3, 100, 100, 1.0);
    strncpy(tst3->nom_base, "Test", FIC_NM);
    Ecrire_Image(tst3,"Rotate", "rotated_60");
    Free_Image(tst2);
    Free_Image(tst3);

    tst2 = rotate_image(tst, -60, 0, 0);
    tst3 = copy_image(background);
    point_center(tst2, Get_Color(ROUGE, NIVEAU_8));
    embed_image(tst2, tst3, 100, 100, 1.0);
    strncpy(tst3->nom_base, "Test", FIC_NM);
    Ecrire_Image(tst3,"Rotate", "rotated_-60");
    Free_Image(tst2);
    Free_Image(tst3);

    tst2 = rotate_image(tst, 90, 0, 0);
    tst3 = copy_image(background);
    point_center(tst2, Get_Color(ROUGE, NIVEAU_8));
    embed_image(tst2, tst3, 100, 100, 1.0);
    strncpy(tst3->nom_base, "Test", FIC_NM);
    Ecrire_Image(tst3,"Rotate", "rotated_90");
    Free_Image(tst2);
    Free_Image(tst3);

    tst2 = rotate_image(tst, -90, 0, 0);
    tst3 = copy_image(background);
    point_center(tst2, Get_Color(ROUGE, NIVEAU_8));
    embed_image(tst2, tst3, 100, 100, 1.0);
    strncpy(tst3->nom_base, "Test", FIC_NM);
    Ecrire_Image(tst3,"Rotate", "rotated_-90");
    Free_Image(tst2);
    Free_Image(tst3);

    tst2 = rotate_image(tst, 180, 0, 0);
    tst3 = copy_image(background);
    point_center(tst2, Get_Color(ROUGE, NIVEAU_8));
    embed_image(tst2, tst3, 100, 100, 1.0);
    strncpy(tst3->nom_base, "Test", FIC_NM);
    Ecrire_Image(tst3,"Rotate", "rotated_180");
    Free_Image(tst2);
    Free_Image(tst3);

    tst2 = rotate_image(tst, 270, 0, 0);
    tst3 = copy_image(background);
    point_center(tst2, Get_Color(ROUGE, NIVEAU_8));
    embed_image(tst2, tst3, 100, 100, 1.0);
    strncpy(tst3->nom_base, "Test", FIC_NM);
    Ecrire_Image(tst3,"Rotate", "rotated_270");

    Free_Image(tst);
    Free_Image(tst2);
    Free_Image(tst3);
    Free_Image(ship);
    Free_Image(background);

    return;
}

/****************************************************************************************
* Test_Flip : Flip an image
*
* Purpose : testing that pictures are properly flipped (vertically and horizontally)
****************************************************************************************/
void Tst_Flip(void){
    image *tst=NULL, *tst2=NULL, *ship=NULL, *background=NULL;

    printf("\n--- Test Flip -----------------------------------------------------\n\n");
    ship = Lire_Image("Ship", NULL, "Enterprise");
    background = Lire_Image("Star", NULL, "Field");
    tst = copy_image(ship);
    system(MKDIR(Test\\Test_Flip));

    flip_image(tst, VERTICAL);
    tst2 = copy_image(background);
    embed_image(tst, tst2, 100, 100, 1.0);
    strncpy(tst2->nom_base, "Test", FIC_NM);
    Ecrire_Image(tst2,"Flip", "flip_vertical");
    Free_Image(tst2);

    flip_image(tst, HORIZONTAL);
    tst2 = copy_image(background);
    embed_image(tst, tst2, 100, 100, 1.0);
    strncpy(tst2->nom_base, "Test", FIC_NM);
    Ecrire_Image(tst2,"Flip", "flip_horizontal");

    Free_Image(tst);
    Free_Image(tst2);
    Free_Image(ship);
    Free_Image(background);

    return;
}

/****************************************************************************************
* Test_Zoom : Zoom an image and embed it in an image
*
* Purpose : testing that pictures are properly cut while zooming
****************************************************************************************/
void Tst_Zoom(void){
    image *tst=NULL, *tst2=NULL, *ship=NULL, *background=NULL;
    int dX=0, dY=0;

    printf("\n--- Test Zoom -----------------------------------------------------\n\n");
    ship = Lire_Image("Ship", NULL, "Enterprise");
    background = Lire_Image("Star", NULL, "Field");
    system(MKDIR(Test\\Test_Zoom));

    tst = zoom_image(ship, 1.0);
    tst2 = copy_image(background);
    embed_image(tst, tst2, 100, 100, 1.0);
    strncpy(tst2->nom_base, "Test", FIC_NM);
    Ecrire_Image(tst2,"Zoom", "zoom_embed_1");
    Free_Image(tst);
    Free_Image(tst2);

    tst = zoom_image(ship, 0.3);
    tst2 = copy_image(background);
    dX = ship->header.largeur/2 - tst->header.largeur/2;
    dY = ship->header.hauteur/2 - tst->header.hauteur/2;
    embed_image(tst, tst2, 100+dX, 100+dY, 1.0);
    strncpy(tst2->nom_base, "Test", FIC_NM);
    Ecrire_Image(tst2,"Zoom", "zoom_embed_03");
    Free_Image(tst);
    Free_Image(tst2);

    tst = zoom_image(ship, 0.5);
    tst2 = copy_image(background);
    dX = ship->header.largeur/2 - tst->header.largeur/2;
    dY = ship->header.hauteur/2 - tst->header.hauteur/2;
    embed_image(tst, tst2, 100+dX, 100+dY, 1.0);
    strncpy(tst2->nom_base, "Test", FIC_NM);
    Ecrire_Image(tst2,"Zoom", "zoom_embed_05");
    Free_Image(tst);
    Free_Image(tst2);

    tst = zoom_image(ship, 0.75);
    tst2 = copy_image(background);
    dX = ship->header.largeur/2 - tst->header.largeur/2;
    dY = ship->header.hauteur/2 - tst->header.hauteur/2;
    embed_image(tst, tst2, 100+dX, 100+dY, 1.0);
    strncpy(tst2->nom_base, "Test", FIC_NM);
    Ecrire_Image(tst2,"Zoom", "zoom_embed_075");
    Free_Image(tst);
    Free_Image(tst2);

    tst = zoom_image(ship, 1.5);
    tst2 = copy_image(background);
    dX = ship->header.largeur/2 - tst->header.largeur/2;
    dY = ship->header.hauteur/2 - tst->header.hauteur/2;
    embed_image(tst, tst2, 100+dX, 100+dY, 1.0);
    strncpy(tst2->nom_base, "Test", FIC_NM);
    Ecrire_Image(tst2,"Zoom", "zoom_embed_15");
    Free_Image(tst);
    Free_Image(tst2);

    tst = zoom_image(ship, 2.0);
    tst2 = copy_image(background);
    dX = ship->header.largeur/2 - tst->header.largeur/2;
    dY = ship->header.hauteur/2 - tst->header.hauteur/2;
    embed_image(tst, tst2, 100+dX, 100+dY, 1.0);
    strncpy(tst2->nom_base, "Test", FIC_NM);
    Ecrire_Image(tst2,"Zoom", "zoom_embed_2");
    Free_Image(tst);
    Free_Image(tst2);

    Free_Image(ship);
    Free_Image(background);

    return;
}

/****************************************************************************************
* Tst_Dir_Tree : Create a directory tree which corresponds to the one expected
*                   FILMNAME\[FILMNAME_SceneNumber\]FILMNAME_FrameNumber
*
* Purpose : Making sure scene generation respects the directory structure
****************************************************************************************/
void Tst_Dir_Tree(void){
    char tmp[64] = "0";
    char tmp_frame[5] = "0";
    char tmp_scene[5] = "0";

    printf("\n--- Test Directory Tree -----------------------------------------------------\n\n");
    Nom_Image(tmp, "Test", NULL, "Test_name_no_scene");
    for(int scene=1 ; scene<6 ; scene++){
        for(int frame=1 ; frame<13 ; frame++){
            sprintf(tmp_frame, "%04d", frame);
            sprintf(tmp_scene, "%02d", scene);
            Nom_Image(tmp, "Test", tmp_scene, tmp_frame);
        }
    }

    return;
}

/****************************************************************************************
* Tst_Weapon_Computation : Checks if weapons coordinates properly change with
*                               ships manipulation
*
* Purpose : Making sure coordinates are properly computed
****************************************************************************************/
void Tst_Weapon_Computation(){
    image *ship=NULL, *background=NULL, *target=NULL, *scene=NULL, *tmp=NULL;
    ship_t tst={"Enterprise", 2, {{247,200},{295,286},{0,0}}, VERT, NULL};
    int dX=0, dY=0;

    printf("\n--- Test Weapon Computation -----------------------------------------------------\n\n");
    ship = Lire_Image("Ship", NULL, "Enterprise");
    target = Lire_Image("Ship", NULL, "Borg_Cube");
    background = Lire_Image("Star", NULL, "Field");
    system(MKDIR(Test\\Test_Weapon_Computation));

    tst.img = copy_image(ship);
    scene = copy_image(background);
    point_center(tst.img, Get_Color(ROUGE, NIVEAU_8));
    embed_image(tst.img, scene, -50, -50, 1.0);
    compute_weapons_coordinates(&tst, NO_FLIP, -50, -50, 0, 0);
    embed_image(target, scene, 400, 150, 1.0);
    shoot(&tst, target, scene);
    strncpy(scene->nom_base, "Test", FIC_NM);
    Ecrire_Image(scene,"Weapon_Computation", "1_translated");
    Free_Image(scene);

    scene = copy_image(background);
    flip_image(tst.img, HORIZONTAL);
    point_center(tst.img, Get_Color(ROUGE, NIVEAU_8));
    embed_image(tst.img, scene, -50, -50, 1.0);
    compute_weapons_coordinates(&tst, HORIZONTAL, 0, 0, 0, 0);
    embed_image(target, scene, 400, 150, 1.0);
    shoot(&tst, target, scene);
    strncpy(scene->nom_base, "Test", FIC_NM);
    Ecrire_Image(scene,"Weapon_Computation", "2_flipped");
    Free_Image(scene);

    scene = copy_image(background);
    tmp = rotate_image(tst.img, 30, 0, 0);
    Free_Image(tst.img);
    tst.img = tmp;
    point_center(tst.img, Get_Color(ROUGE, NIVEAU_8));
    embed_image(tst.img, scene, -50, -50, 1.0);
    compute_weapons_coordinates(&tst, NO_FLIP, 0, 0, 30, 0);
    embed_image(target, scene, 400, 150, 1.0);
    shoot(&tst, target, scene);
    strncpy(scene->nom_base, "Test", FIC_NM);
    Ecrire_Image(scene,"Weapon_Computation", "3_rotated30");
    Free_Image(scene);

    scene = copy_image(background);
    tmp = zoom_image(tst.img, 0.75);
    dX = tst.img->header.largeur/2 - tmp->header.largeur/2;
    dY = tst.img->header.hauteur/2 - tmp->header.hauteur/2;
    Free_Image(tst.img);
    tst.img = tmp;
    point_center(tst.img, Get_Color(ROUGE, NIVEAU_8));
    embed_image(tst.img, scene, -50+dX, -50+dY, 1.0);
    compute_weapons_coordinates(&tst, NO_FLIP, dX, dY, 0, 0.75);
    embed_image(target, scene, 400, 150, 1.0);
    shoot(&tst, target, scene);
    strncpy(scene->nom_base, "Test", FIC_NM);
    Ecrire_Image(scene,"Weapon_Computation", "4_unzoomed");

    Free_Image(ship);
    Free_Image(background);
    Free_Image(target);
    Free_Image(tst.img);
    Free_Image(scene);
    return;
}
