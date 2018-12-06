#ifndef EFFECTS_H_INCLUDED
#define EFFECTS_H_INCLUDED
#include "image.h"

typedef struct{
    uint xa;
    uint ya;
    uint xb;
    uint yb;
    int colour;
    int intensity;
    float alpha;
} line;

void Filtrer_Noir_Blanc(image * img);
void Damier(image * img);

image* embed_image(image* overlay, image* background, uint x, uint y, float alpha);
image* draw_line_Bresenham(image *original, line* l);
int set_pixel_rgba(image* img, uint x, uint y, int colour, int intensity, float alpha);

#endif // EFFECTS_H_INCLUDED
