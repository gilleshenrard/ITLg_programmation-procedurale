#ifndef FILM_H_INCLUDED
#define FILM_H_INCLUDED
#include "image.h"

typedef struct Film
{
    uint nb_scene;      // nombre de scènes dans le film
    char nm_film[28];   // nom du film
    int scene[32];      // taille en nombre d'images des 32 scènes
} film;

int save_movie(film* movie);
int register_scene(film* movie, int nb_frames);

#endif // FILM_H_INCLUDED
