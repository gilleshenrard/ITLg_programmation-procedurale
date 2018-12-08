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
} line;

void Filtrer_Noir_Blanc(image * img);
void Damier(image * img);

image* embed_image(image* overlay, image* background, uint x, uint y, float alpha);
int set_pixel_rgba(image* img, uint x, uint y, int colour, int intensity, float alpha);
image* draw_line_Bresenham(image *original, line* l);
image* draw_line_Wu(image* original, line* l);

#endif // EFFECTS_H_INCLUDED
