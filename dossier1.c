#include "lib/image.h"
#include "lib/effects.h"
#include "lib/film.h"

#define FILM_NAME   "Starship_Soldiers"

film movie;

void create_directory(char dir_name[]);
void scene01(void);

int main(int argc, char *argv[]){

    memset(&movie, 0, sizeof(film));
    strcpy(movie.nm_film, FILM_NAME);

    scene01();

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
    image *background=NULL, *tmp=NULL;
    image *img_txt=NULL;
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

    for(int time=0 ; time<200 ; time++){
        tmp = copy_image(background);
        for(int i=0 ; i<sizeof(txt)/sizeof(txt[0]) ; i++){
            img_txt = get_text(txt[i], JAUNE, BLUE_SCREEN);
            embed_image(img_txt, tmp, 150, (-i*img_txt->header.hauteur) + (time*3) - 20, 1.0);
            Free_Image(img_txt);
        }
        strncpy(tmp->nom_base, FILM_NAME, FIC_NM);
        sprintf(filename, "%04d", time);
        Ecrire_Image(tmp,"01", filename);
        Free_Image(tmp);
    }

    movie.nb_scene += 1;
    movie.scene[0] = 200;
    Free_Image(background);
}
