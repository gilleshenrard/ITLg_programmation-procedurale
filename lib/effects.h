#ifndef EFFECTS_H_INCLUDED
#define EFFECTS_H_INCLUDED
#include "image.h"
#include <math.h>

#define NO_FLIP
#define VERTICAL 1
#define HORIZONTAL 2

typedef struct{
    int xa;
    int ya;
    int xb;
    int yb;
    uchar* colour;
    float alpha;
    char antialiasing;
} line;

typedef struct{
    char    name[32];       //ship's model (Romulan, Klingon, ...)
    int     center[2];      //coordinates of the center of the ship
    char    nb_weapons;     //number of weapons on the ship
    int     weapons[3][2];  //coordinates of the weapons (3 max)
    couleur w_colour;       //colour of the weapons (lasers)
    image   *img;           //image of the ship
} ship_t;

void Filtrer_Noir_Blanc(image * img);
void Damier(image * img);

int embed_image(image* overlay, image* background, uint x, uint y, float alpha);
int set_pixel_rgba(image* img, uint x, uint y, uchar* colour, float alpha);
int draw_line_vertical(image* img, line* l);
int draw_line_horizontal(image* img, line* l);
int draw_line_diagonal(image* img, line* l);
int draw_line_Bresenham(image *img, line* l);
int draw_line_Wu(image* img, line* l);
int draw_line_generic(image* img, line* l);
image* rotate_image(image* img, int angle, int offsetX, int offsetY);
int flip_image(image* img, int axis);
image* zoom_image(image* img, float factor);

//ship specific
int compute_weapons_coordinates(ship_t*, char, int, int, int, float);

#endif // EFFECTS_H_INCLUDED
