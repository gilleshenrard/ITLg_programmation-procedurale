#include "lib/image.h"
#include "lib/effects.h"
#include "lib/film.h"

#define FILM_NAME   "Starship_Soldiers"

film movie;

void create_directory(char dir_name[]);
void scene01(void);
void scene02(void);

int main(int argc, char *argv[]){

    memset(&movie, 0, sizeof(film));
    strcpy(movie.nm_film, FILM_NAME);

    scene01();
    scene02();

    save_movie(&movie);

    return EXIT_SUCCESS;
}

/****************************************************************************************/
/*  I : Scene directory name                                                            */
/*  P : Creates a subdirectory for a scene in the movie directory                       */
/*  O : /                                                                               */
/****************************************************************************************/
void create_directory(char dir_name[]){
    char command[128] = {0};

    sprintf(command, "if not exist ..\\Films\\%s\\%s_%s mkdir ..\\Films\\%s\\%s_%s", FILM_NAME, FILM_NAME, dir_name, FILM_NAME, FILM_NAME, dir_name);
    system(command);
}

/****************************************************************************************/
/*  I : /                                                                               */
/*  P : Generates all the frames for the scene 01 : Intro and movie title               */
/*  O : /                                                                               */
/****************************************************************************************/
void scene01(void){
    image *background=NULL, *title1=NULL, *title2=NULL, *tmp=NULL;
    image *img_txt[4] = {NULL};
    float f_time = 0.0;
    int i=0, title_height=0, dx=0, frames=260;
    char filename[32] = {0};
    char txt[6][38]={   "              Starship               ",
                        "              Soldiers               ",
                        " Alors que des millenaires d'etudes, ",
                        "observations et preuves scientifiques",
                        "    avaient permi d'assurer que      ",
                        "   les astres etaient spheriques...  "};

    printf("\n------------------- scene 1 : Intro and movie title ---------------------------\n");
    background = Lire_Image("Star", NULL, "Field");
    create_directory("01");

    //Prepare the title images
    tmp = get_text(txt[0], JAUNE, BLUE_SCREEN);
    title1 = zoom_image(tmp, 4.0);
    title_height = title1->header.hauteur;
    dx = background->header.largeur/2 - title1->header.largeur/2;
    Free_Image(tmp);
    tmp = get_text(txt[1], JAUNE, BLUE_SCREEN);
    title2 = zoom_image(tmp, 4.0);
    Free_Image(tmp);

    for(i=0 ; i<4 ; i++){
        img_txt[i] = get_text(txt[i+2], JAUNE, BLUE_SCREEN);
    }

    //Fade the title in (alpha 0 to 1) during 50 frames, centered
    for(int time0=1 ; time0<51 ; time0++){
        f_time = (float)time0;
        tmp = copy_image(background);
        embed_image(title1, tmp, dx, tmp->header.hauteur/2, f_time/50.0);
        embed_image(title2, tmp, dx, tmp->header.hauteur/2 - title_height, f_time/50.0);
        strncpy(tmp->nom_base, FILM_NAME, FIC_NM);
        sprintf(filename, "%04d", time0);
        Ecrire_Image(tmp,"01", filename);
        Free_Image(tmp);
    }

    //scroll title and text 3 pixels every tick
    for(int time1=1 ; time1<(frames-49) ; time1++){
        tmp = copy_image(background);

        //embed the title
        dx = background->header.largeur/2 - title1->header.largeur/2;
        embed_image(title1, tmp, dx, tmp->header.hauteur/2+(time1*3), 1.0);
        embed_image(title2, tmp, dx, tmp->header.hauteur/2 - title_height + (time1*3), 1.0);

        //embed the text
        for(i=0 ; i<4 ; i++){
            dx = background->header.largeur/2 - img_txt[i]->header.largeur/2;
            embed_image(img_txt[i], tmp, dx, -(img_txt[i]->header.hauteur*(i+1)) + (time1*3), 1.0);
        }
        //write the final frame
        strncpy(tmp->nom_base, FILM_NAME, FIC_NM);
        sprintf(filename, "%04d", time1+50);
        Ecrire_Image(tmp,"01", filename);
        Free_Image(tmp);
    }

    register_scene(&movie, frames);
    Free_Image(background);
    for(i=0 ; i<4 ; i++){
        Free_Image(img_txt[i]);
    }
}

/****************************************************************************************/
/*  I : /                                                                               */
/*  P : Generates all the frames for the scene 01 : Main ship appearance                */
/*  O : /                                                                               */
/****************************************************************************************/
void scene02(void){
    image *frame=NULL, *star_field=NULL, *planet=NULL, *ship=NULL;
    char filename[32] = {0};
    int frames = 300;
    double Xt = 0.0, Alphat = 0.0;

    printf("\n------------------- scene 2 : Main ship appearance ---------------------------\n");
    star_field = Lire_Image("Star", NULL, "Field_lg");
    planet = Lire_Image("Star", NULL, "Pla_Coruscant");
    ship = Lire_Image("Ship", NULL, "Enterprise");
    create_directory("02");

    //Show the scenery with a parallaxed scroll (100 frames)
    for(int time0=1 ; time0 <= 100 ; time0++){
        frame = Creer_Image(FILM_NAME, 500, 800, NOIR, NIVEAU_8);
        embed_image(star_field, frame, -(time0*2), 0, 1.0);
        embed_image(planet, frame, 700 - (time0), 200, 1.0);
        sprintf(filename, "%04d", time0);
        Ecrire_Image(frame,"02", filename);
        Free_Image(frame);
    }

    //Include the main ship (speed expotientially decreasing, alpha expotientially increasing)  (100 frames)
    for(int time1=1 ; time1 <= (frames-200) ; time1++){
        frame = Creer_Image(FILM_NAME, 500, 800, NOIR, NIVEAU_8);
        embed_image(star_field, frame, -((time1+100)*2), 0, 1.0);
        embed_image(planet, frame, 700 - (time1+100), 200, 1.0);
        // during 100 frames, x goes from -500 to 100 and speed goes from 30 to 0
        Xt = (log((double)time1)*50)-100.0;
        Alphat = (log((double)time1)/7)+0.4;
        embed_image(ship, frame, (int)Xt, 50, (float)Alphat);
        sprintf(filename, "%04d", time1+100);
        Ecrire_Image(frame,"02", filename);
        Free_Image(frame);
    }

    //slowly decrease the parallax effect  (100 frames)
    for(int time2=1 ; time2 <= (frames-200) ; time2++){
        frame = Creer_Image(FILM_NAME, 500, 800, NOIR, NIVEAU_8);
        embed_image(star_field, frame, -(time2+400), 0, 1.0);
        embed_image(planet, frame, 500-(time2*0.5), 200, 1.0);
        embed_image(ship, frame, 131, 50, 1.0);
        sprintf(filename, "%04d", time2+200);
        Ecrire_Image(frame,"02", filename);
        Free_Image(frame);
    }

    register_scene(&movie, frames);
    Free_Image(star_field);
    Free_Image(planet);
    Free_Image(ship);
}
