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

#endif // EFFECTS_H_INCLUDED
