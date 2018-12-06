#ifndef EFFECTS_H_INCLUDED
#define EFFECTS_H_INCLUDED
#include "image.h"

void Filtrer_Noir_Blanc(image * img);
void Damier(image * img);

image* embed_image(image* overlay, image* background, uint x, uint y, float alpha);
image* draw_line_Bresenham(uint xa, uint ya, uint xb, uint yb, uchar* colour, image *original);
int set_pixel(image* img, uint x, uint y, uchar* colour);

#endif // EFFECTS_H_INCLUDED
