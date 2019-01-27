#ifndef FILM_H_INCLUDED
#define FILM_H_INCLUDED

typedef struct Film
{
    uint nb_scene;      // nombre de sc�nes dans le film
    char nm_film[28];   // nom du film
    int scene[32];      // taille en nombre d'images des 32 sc�nes
} film;

#endif // FILM_H_INCLUDED
