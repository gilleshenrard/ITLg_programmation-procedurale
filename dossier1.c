#include "lib/image.h"
#include "lib/effects.h"
#include "lib/film.h"

#define FILM_NAME   "Dossier1"

void create_directory(char dir_name[]);
void scene01(void);

int main(int argc, char *argv[]){

    scene01();

    return EXIT_SUCCESS;
}

void create_directory(char dir_name[]){
    char command[128] = {0};

    sprintf(command, "if not exist ..\\Films\\%s\\%s_%s mkdir ..\\Films\\%s\\%s_%s", FILM_NAME, FILM_NAME, dir_name, FILM_NAME, FILM_NAME, dir_name);
    system(command);
}

void scene01(void){
    image *background=NULL, *tmp=NULL;
    image *img_txt=NULL;
    char txt[2][16]={"Hello", "World!"};
    char filename[32] = {0};

    printf("\n------------------- scene 1 : Intro ---------------------------\n");
    background = Lire_Image("Star", NULL, "Field");
    create_directory("01");

    for(int time=0 ; time<200 ; time++){
        tmp = copy_image(background);
        for(int i=0 ; i<2 ; i++){
            img_txt = get_text(txt[i], JAUNE, BLUE_SCREEN);
            embed_image(img_txt, background, 300, -20, 1.0);
            Free_Image(img_txt);
        }
        strncpy(tmp->nom_base, "FILM_NAME", FIC_NM);
        sprintf(filename, "%4d", time);
        Ecrire_Image(tmp,"01", "embed_horizontal");
        Free_Image(tmp);
    }

    Free_Image(background);
}
