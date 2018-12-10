#ifndef EFFECTS_H_INCLUDED
#define EFFECTS_H_INCLUDED
#include "image.h"

typedef struct{
    int xa;
    int ya;
    int xb;
    int yb;
    int colour;
    int intensity;
    float alpha;
    char antialiasing;
} line;

void Filtrer_Noir_Blanc(image * img);
void Damier(image * img);

int embed_image(image* overlay, image* background, uint x, uint y, float alpha);
int set_pixel_rgba(image* img, uint x, uint y, int colour, int intensity, float alpha);
int draw_line_vertical(image* img, line* l);
int draw_line_horizontal(image* img, line* l);
int draw_line_diagonal(image* img, line* l);
int draw_line_Bresenham(image *img, line* l);
int draw_line_Wu(image* img, line* l);
int draw_line_generic(image* img, line* l);

#endif // EFFECTS_H_INCLUDED
