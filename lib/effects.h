#ifndef EFFECTS_H_INCLUDED
#define EFFECTS_H_INCLUDED
#include "image.h"
#include <math.h>

#define VERTICAL 0
#define HORIZONTAL 1

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
    int     weapons[2][3];  //coordinates of the weapons (3 max)
    couleur w_colour;       //colour of the weapons (lasers)
    image   *img;           //image of the ship
    char    flipped;        //flag to determine if the ship has been flipped
    int     coordinates[2]: //x and y coordinates of the image (translation)
    int     angle;          //angle with which the ship has been rotated
    float   zoom;           //zoom factor
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
int compute_weapons_coordinates(ship_t* ship);

#endif // EFFECTS_H_INCLUDED
