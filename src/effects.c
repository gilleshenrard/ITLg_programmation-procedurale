#include "../lib/effects.h"

/****************************************************************************************
* Filtrer l'image en noir et blanc
*   img     : structure image
****************************************************************************************/
void Filtrer_Noir_Blanc(image * img)
{
	int i, j, cm;

	printf("Filtrage noir et blanc en cours......");

	for(i=0;i<img->header.hauteur;i++)
	{
		for (j=0;j<img->header.largeur;j++)
		{
			cm = ( img->pic[i][j][0] + img->pic[i][j][1] + img->pic[i][j][2] ) / 3;

			img->pic[i][j][0] = cm;		// blue
			img->pic[i][j][1] = cm;		// green
			img->pic[i][j][2] = cm;		// red
		}
	}

	printf(" termine.\n\n");
}

/****************************************************************************************
* Genere une image en damier noir et blanc
*   img     : structure image
****************************************************************************************/
void Damier(image * img)
{
	int i, j, k, col=0;

	for(i=0;i<img->header.hauteur;i++)
	{
	    col = i%2?0:255;

		for (j=0;j<img->header.largeur;j++)
		{
		    for (k=0; k<3; k++) img->pic[i][j][k] = col;
		    col = 255 - col;
		}
	}

    return;
}

/****************************************************************************************/
/*  I : image to embed to                                                               */
/*      image to embed in                                                               */
/*      x coordinate of the overlay                                                     */
/*      y coordinate of the overlay                                                     */
/*      transparency factor                                                             */
/*  P : Embeds an image in another one (with transparency)                              */
/*  O : resulting image of the embedding                                                */
/*      NULL if error                                                                   */
/****************************************************************************************/
image* embed_image(image* overlay, image* background, uint x, uint y, float alpha){
    image* buffer = NULL;

    buffer = Creer_Image("", background->header.hauteur, background->header.largeur, NOIR, NIVEAU_8);

    //for each pixel in the frame
    for(uint i=0 ; i<background->header.hauteur ; i++){
        for(uint j=0 ; j<background->header.largeur ; j++){
            //if the pixel is to be replaced by the overlay corresponding pixel
            if(is_in_frame(i-y, j-x, overlay) && !is_dummy_pixel(i-y, j-x, overlay)){
                for(int k=0 ; k<3 ; k++)
                    buffer->pic[i][j][k] = (overlay->pic[i-y][j-x][k] * alpha) + (background->pic[i][j][k] * (1.0-alpha));
            }
            //otherwise
            else{
                for(int k=0 ; k<3 ; k++)
                    buffer->pic[i][j][k] = background->pic[i][j][k];
            }
        }
    }
    return buffer;
}

/****************************************************************************************/
/*  I : image in which colour a pixel                                                   */
/*      x coordinate of the pixel                                                       */
/*      y coordinate of the pixel                                                       */
/*      colour to assign                                                                */
/*      transparency factor                                                             */
/*  P : sets a pixel at a particular colour in a picture without creating a copy        */
/*  O :  1 if OK                                                                        */
/*       0 if ignored                                                                   */
/*      -1 if error                                                                     */
/****************************************************************************************/
/*  WARNING : function much likely to be used in a loop, therefore uses the original    */
/*              picture instead of creating a copy                                      */
/****************************************************************************************/
int set_pixel_rgba(image* img, uint x, uint y, int colour, int intensity, float alpha){
    uchar* col = NULL;

    if(!is_in_frame(x, y, img))
        return 0;

    col = Get_Color(colour, intensity);

    for(int i=0 ; i<3 ; i++)
        img->pic[y][x][i] = (col[i]*alpha) + (img->pic[y][x][i] * (1.0-alpha));

    return 1;
}

/****************************************************************************************/
/*  I : Image in which to draw a line                                                   */
/*      metadata of the line to draw                                                    */
/*  P : Draws a vertical line in the image                                              */
/*  O :  0 if OK                                                                        */
/*      -1 if error                                                                     */
/****************************************************************************************/
int draw_line_vertical(image* img, line* l){
    int dx = l->xb - l->xa;
    int dy = l->yb - l->ya;
    int y = l->ya;

    if(!dx){
        if (dy < 0)
            dy = -dy;

        while(dy){
            y = (y < l->yb ? y+1 : y-1);
            set_pixel_rgba(img, l->xa, y, l->colour, l->intensity, l->alpha);
            dy--;
        }
    }
    return 0;
}

/****************************************************************************************/
/*  I : Image in which to draw a line                                                   */
/*      metadata of the line to draw                                                    */
/*  P : Draws a horizontal line in the image                                            */
/*  O :  0 if OK                                                                        */
/*      -1 if error                                                                     */
/****************************************************************************************/
int draw_line_horizontal(image* img, line* l){
    int dx = l->xb - l->xa;
    int dy = l->yb - l->ya;
    int x = l->xa;

    if(!dy){
        if(dx < 0)
            dx = -dx;

        while(dx){
            x = (x < l->xb ? x+1 : x-1);
            set_pixel_rgba(img, x, l->ya, l->colour, l->intensity, l->alpha);
            dx--;
        }
    }
    return 0;
}

/****************************************************************************************/
/*  I : Image in which to draw a line                                                   */
/*      metadata of the line to draw                                                    */
/*  P : Draws a diagonal line in the image                                              */
/*  O :  0 if OK                                                                        */
/*      -1 if error                                                                     */
/****************************************************************************************/
int draw_line_diagonal(image* img, line* l){
    int dx = l->xb - l->xa;
    int dy = l->yb - l->ya;
    int x = l->xa;
    int y = l->ya;

    if(abs(dx) == abs(dy)){
        if(dx < 0)
            dx = -dx;

        while(dx){
             x = (x < l->xb ? x+1 : x-1);
             y = (y < l->yb ? y+1 : y-1);
             set_pixel_rgba(img, x, y, l->colour, l->intensity, l->alpha);
             dx--;
        }
    }
    return 0;
}

/****************************************************************************************/
/*  I : Image in which to draw a line                                                   */
/*      metadata of the line to draw                                                    */
/*  P : Draws a line in the image using Bresenham's algorithm (no antialiasing)         */
/*  O :  0 if OK                                                                        */
/*      -1 if error                                                                     */
/****************************************************************************************/
int draw_line_Bresenham(image* img, line* l){
    int x0=l->xa, x1=l->xb, y0=l->ya, y1=l->yb, tmp, x;
    int dx=l->xb - l->xa, dy=l->yb - l->ya;
    int steep, slope=1, d;

    // swap the co-ordinates if slope > 1
    steep = abs(dy) < abs(dx);
    if(steep){
        tmp = x0; x0 = y0; y0 = tmp;
        tmp = x1; x1 = y1; y1 = tmp;
    }

    // swap the co-ordinates if we draw backwards
    if (y0 > y1){
        tmp = x0; x0 = x1; x1 = tmp;
        tmp = y1; y1 = y0; y0 = tmp;
    }

    dx = x1 - x0;
    dy = y1 - y0;

    if(dx < 0){
        slope = -1;
        dx = -dx;
    }

    d = 2*dx - dy;
    x = x0;

    for(int y=y0 ; y<y1 ; y++){
        if(steep)
            set_pixel_rgba(img, y, x, l->colour, l->intensity, l->alpha);
        else
            set_pixel_rgba(img, x, y, l->colour, l->intensity, l->alpha);

        if(d > 0){
            x += slope;
            d -= 2*dy;
        }
        d += 2*dx;
    }

    return 0;
}

/****************************************************************************************/
/*  I : Image in which to draw a line                                                   */
/*      metadata of the line to draw                                                    */
/*  P : Draws a line in the image using Xiaolin Wu's algorithm (with antialiasing)      */
/*  O :  0 if OK                                                                        */
/*      -1 if error                                                                     */
/****************************************************************************************/
int draw_line_Wu(image* img, line* l){
    int x0=l->xa, x1=l->xb, y0=l->ya, y1=l->yb;
    int steep, tmp;
    float dx = (float)x1 - (float)x0;
    float dy = (float)y1 - (float)y0;
    float gradient, intersectY, fracPartY;

    // swap the co-ordinates if slope > 1
    steep = abs(dx)<abs(dy);
    if (steep){
        tmp = x0; x0 = y0; y0 = tmp;
        tmp = x1; x1 = y1; y1 = tmp;
    }

    // swap the co-ordinates if we draw backwards
    if (x0 > x1){
        tmp = x0; x0 = x1; x1 = tmp;
        tmp = y1; y1 = y0; y0 = tmp;
    }

    //compute the slope
    dx = (float)x1 - (float)x0;
    dy = (float)y1 - (float)y0;
    gradient = dy/dx;
    if (dx == 0.0)
        gradient = 1;
    intersectY=y0;

    // main loop
    for (int x = x0 ; x <= x1 ; x++)
    {
        fracPartY = (intersectY>0 ? intersectY-(int)intersectY : intersectY-((int)intersectY+1));

        if (steep)
        {
            // pixel coverage is determined by fractional part of y co-ordinate
            set_pixel_rgba(img, (int)intersectY, x, l->colour, l->intensity, (1.0-fracPartY)*l->alpha);
            set_pixel_rgba(img, (int)intersectY+1, x, l->colour, l->intensity, fracPartY*l->alpha);

        }
        else
        {
            // pixel coverage is determined by fractional part of y co-ordinate
            set_pixel_rgba(img, x, (int)intersectY, l->colour, l->intensity, (1-fracPartY)*l->alpha);
            set_pixel_rgba(img, x, (int)intersectY+1, l->colour, l->intensity, fracPartY*l->alpha);
        }
        intersectY += gradient;
    }

    return 0;
}

/****************************************************************************************/
/*  I : Image in which to draw a line                                                   */
/*      metadata of the line to draw                                                    */
/*  P : Sorts which algorithm use to draw a line                                        */
/*  O :  0 if OK                                                                        */
/*      -1 if error                                                                     */
/****************************************************************************************/
int draw_line_generic(image* img, line* l){
    int dx = l->xb - l->xa;
    int dy = l->yb - l->ya;

    //draw the very first pixel (in case the length of the line is 1)
    set_pixel_rgba(img, l->xa, l->ya, l->colour, l->intensity, l->alpha);

    //diagonal line
    if(abs(dx) == abs(dy))
        return draw_line_diagonal(img, l);

    //horizontal line
    if(!dy)
        return draw_line_horizontal(img, l);

    //vertical line
    if(!dx)
        return draw_line_vertical(img, l);

    if(l->antialiasing)
        return draw_line_Wu(img, l);
    else
        return draw_line_Bresenham(img, l);
}
