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

    if(dx == dy){
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
    signed int primA, primB, secA, secB;
    signed int deltaPrim, deltaSec;
    signed int primI, d, primInc;
    char Yprioritised = 0;

    //sorting which coordinate (x or y) is the primary and which is the secondary
    //      depending on the octant of the line
    if(abs(l->yb - l->ya) < abs(l->xb - l->xa)){
        if(l->xa > l->xb){
            //plotLineLow(xb, yb, xa, ya)
            primA = l->yb;
            primB = l->ya;
            secA = l->xb;
            secB = l->xa;
        }
        else{
            //plotLineLow(xa, ya, xb, yb)
            primA = l->ya;
            primB = l->yb;
            secA = l->xa;
            secB = l->xb;
        }
        Yprioritised = 1;
    }
    else{
        if(l->ya > l->yb){
            //plotLineHigh(xb, yb, xa, ya)
            //bottom right + bottom right
            primA = l->xb;
            primB = l->xa;
            secA = l->yb;
            secB = l->ya;
        }
        else{
            //plotLineHigh(xa, ya, xb, yb)
            //top right + top left
            primA = l->xa;
            primB = l->xb;
            secA = l->ya;
            secB = l->yb;
        }
    }

    //defining deltas (equivalent of dx and dy, prioritised)
    deltaPrim = primB - primA;
    deltaSec = secB - secA;

    //defining slope increment
    primI = 1;
    if(deltaPrim < 0){
        primI = -1;
        deltaPrim = -deltaPrim;
    }

    //preparing resolution factor and increment variables
    d = 2*deltaPrim - deltaSec;
    primInc = primA;


    for(uint secInc=secA ; secInc<secB ; secInc++){
        //draw pixel depending on which coordinate has been set as primary
        if(Yprioritised)
            set_pixel_rgba(img, secInc, primInc, l->colour, l->intensity, l->alpha);
        else
            set_pixel_rgba(img, primInc, secInc, l->colour, l->intensity, l->alpha);

        if(d > 0){
            primInc += primI;
            d = d - (2*deltaSec);
        }
        d = d + (2*deltaPrim);
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
    steep = abs(dx)>abs(dy);
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
    for (uint x = x0 ; x <= x1 ; x++)
    {
        fracPartY = (intersectY>0 ? intersectY-(int)intersectY : intersectY-((int)intersectY+1));

        if (steep)
        {
            // pixel coverage is determined by fractional part of y co-ordinate
            set_pixel_rgba(img, (int)intersectY, x, l->colour, l->intensity, (1.0-fracPartY)*l->alpha);
            set_pixel_rgba(img, (int)intersectY, x+1, l->colour, l->intensity, fracPartY*l->alpha);

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
    if(dx == dy)
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
