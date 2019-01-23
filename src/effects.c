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
/*  O :  0 if OK                                                                        */
/*      -1 if error                                                                     */
/****************************************************************************************/
int embed_image(image* overlay, image* background, uint x, uint y, float alpha){

    overlay->center[0] = x + (overlay->header.largeur/2);
    overlay->center[1] = y + (overlay->header.hauteur/2);

    //for each pixel in the frame
    for(uint i=0 ; i<background->header.hauteur ; i++){
        for(uint j=0 ; j<background->header.largeur ; j++){
            //if the pixel is to be replaced by the overlay corresponding pixel
            if(is_in_frame(i-y, j-x, overlay) && !is_dummy_pixel(i-y, j-x, overlay)){
                set_pixel_rgba(background, j, i, overlay->pic[i-y][j-x], alpha);
            }
        }
    }
    return 0;
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
int set_pixel_rgba(image* img, uint x, uint y, uchar* colour, float alpha){
    if(!is_in_frame(x, y, img))
        return 0;

    for(int i=0 ; i<3 ; i++)
        img->pic[y][x][i] = (colour[i]*alpha) + (img->pic[y][x][i] * (1.0-alpha));

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
            set_pixel_rgba(img, l->xa, y, l->colour, l->alpha);
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
            set_pixel_rgba(img, x, l->ya, l->colour, l->alpha);
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
             set_pixel_rgba(img, x, y, l->colour, l->alpha);
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
            set_pixel_rgba(img, y, x, l->colour, l->alpha);
        else
            set_pixel_rgba(img, x, y, l->colour, l->alpha);

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
            set_pixel_rgba(img, (int)intersectY, x, l->colour, (1.0-fracPartY)*l->alpha);
            set_pixel_rgba(img, (int)intersectY+1, x, l->colour, fracPartY*l->alpha);

        }
        else
        {
            // pixel coverage is determined by fractional part of y co-ordinate
            set_pixel_rgba(img, x, (int)intersectY, l->colour, (1-fracPartY)*l->alpha);
            set_pixel_rgba(img, x, (int)intersectY+1, l->colour, fracPartY*l->alpha);
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
    set_pixel_rgba(img, l->xa, l->ya, l->colour, l->alpha);

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

/****************************************************************************************/
/*  I : Image to rotate                                                                 */
/*      Angle with which rotate                                                         */
/*  P : Creates a rotated (and resized) copy of an image if angle is a multiple of 90°  */
/*  O : image generated                                                                 */
/*      NULL if error                                                                   */
/****************************************************************************************/
image* rotate_image_90(image* img, int angle, int offsetX, int offsetY){
    image* buffer=NULL;
    uint new_width, new_height;

    if(angle%90 == 0){
        //prepare a new image (adapt dimensions for the rotation)
        new_width = (angle/90 == 2 ? img->header.largeur : img->header.hauteur);
        new_height = (angle/90 == 2 ? img->header.hauteur : img->header.largeur);
        buffer = Creer_Image(img->nom_base, new_height, new_width, BLANC, NIVEAU_8);

        for(int y=0 ; y < img->header.hauteur ; y++){
            for(int x=0 ; x < img->header.largeur ; x++){
                switch(angle){
                    case 90:
                    case -270:
                        set_pixel_rgba(buffer, img->header.largeur -y -1, x, img->pic[y][x], 1.0);
                        break;

                    case 270:
                    case -90:
                        set_pixel_rgba(buffer, y, img->header.hauteur -x -1, img->pic[y][x], 1.0);
                        break;

                    case 180:
                    case -180:
                        set_pixel_rgba(buffer, img->header.largeur -x -1, img->header.hauteur -y -1, img->pic[y][x], 1.0);
                        break;
                }
            }
        }
    }

    return buffer;
}

/****************************************************************************************/
/*  I : Image to rotate                                                                 */
/*      Angle with which rotate                                                         */
/*  P : Creates a rotated (and resized) copy of an image                                */
/*  O :  0 if OK                                                                        */
/*      -1 if error                                                                     */
/****************************************************************************************/
image* rotate_image(image* img, int angle, int offsetX, int offsetY){
    image* buffer=NULL;
    double sinVal = sin((double)(angle*M_PI)/180);
    double cosVal = cos((double)(angle*M_PI)/180);

    if(angle%90 == 0)
        return rotate_image_90(img, angle, offsetX, offsetY);

    //prepare a new image
    buffer = Creer_Image(img->nom_base, img->header.hauteur, img->header.largeur, BLANC, NIVEAU_8);

    for(int y=0 ; y < img->header.hauteur ; y++){
        for(int x=0 ; x < img->header.largeur ; x++){
            //compute the new location for every pixel and assign it to the new image
            //| cos    -sin |   | x - center_x |   | center_x |
            //| sin     cos | * | y - center_y | + | center_y |
            double translate_x = (double)x - buffer->center[0];
            double translate_y = (double)y - buffer->center[1];
            int srcX = (int)((cosVal*translate_x) - (sinVal*translate_y)) + buffer->center[0];
            int srcY = (int)((sinVal*translate_x) + (cosVal*translate_y)) + buffer->center[1];
            if(is_in_frame(srcX, srcY, img))
                set_pixel_rgba(buffer, x, y, img->pic[srcY][srcX], 1.0);
        }
    }


    return buffer;
}

/****************************************************************************************/
/*  I : Image to flip                                                                   */
/*      Direction according to which flip the image (VERTICAL or HORIZONTAL)            */
/*  P : Flips the image                                                                 */
/*  O :  0 if OK                                                                        */
/*      -1 if error                                                                     */
/****************************************************************************************/
int flip_image(image* img, int axis){
    uchar *tmp = NULL;

    tmp = (uchar*)malloc(3*sizeof(uchar));
    if(!tmp)
        return -1;

    switch(axis){
        case VERTICAL:
            for(int y=0 ; y<img->header.hauteur ; y++){
                for(int x=0 ; x<img->header.largeur/2 ; x++){
                    assign_pixel(tmp, img->pic[y][x]);
                    assign_pixel(img->pic[y][x], img->pic[y][img->header.largeur - x - 1]);
                    assign_pixel(img->pic[y][img->header.largeur - x - 1], tmp);
                }
            }
            break;

        case HORIZONTAL:
            for(int y=0 ; y<img->header.hauteur/2 ; y++){
                for(int x=0 ; x<img->header.largeur ; x++){
                    assign_pixel(tmp, img->pic[y][x]);
                    assign_pixel(img->pic[y][x], img->pic[img->header.hauteur - y - 1][x]);
                    assign_pixel(img->pic[img->header.hauteur - y - 1][x], tmp);
                }
            }
            break;

        default:
            free(tmp);
            return -1;
    }

    free(tmp);
    return 0;
}

/****************************************************************************************/
/*  I : Image to zoom                                                                   */
/*      Zoom factor                                                                     */
/*  P : Generates an increased or decreased image                                       */
/*  O : image generated                                                                 */
/*      NULL if error                                                                   */
/****************************************************************************************/
image* zoom_image(image* img, float factor){
    image* buffer = NULL;

    if(factor <= 0.0)
        return NULL;

    if(factor == 1){
        //image simply copied
        return copy_image(img);
    }

    buffer = Creer_Image(img->nom_base, img->header.hauteur*factor, img->header.largeur*factor, BLANC, NIVEAU_8);

    for(int y=0 ; y<buffer->header.hauteur ; y++){
        for(int x=0 ; x<buffer->header.largeur ; x++){
            int srcX = (int)(float)(x/factor);
            int srcY = (int)(float)(y/factor);

            set_pixel_rgba(buffer, x, y, img->pic[srcY][srcX], 1.0);
        }
    }

    return buffer;
}

/****************************************************************************************/
/*  I : Ship for which compute the weapons coordinates                                  */
/*  P : Computes the coordinates of the weapon depending if the image has been flipped, */
/*          translated, zoomed, ...                                                     */
/*  O : -1 if error                                                                     */
/*       0 otherwise                                                                    */
/****************************************************************************************/
int compute_weapons_coordinates(ship_t* ship, char flipped, int translation_x, int translation_y, int angle, float zoom){
    int x0=0, y0=0;

    if(!ship)
        return -1;

    //compute the image x0 and y0
    x0 = ship->img->center[0] - (ship->img->header.largeur/2);
    y0 = ship->img->center[1] - (ship->img->header.hauteur/2);

    for(int i=0 ; i<ship->nb_weapons ; i++){
        //compute ship translation
        ship->weapons[i][0] += translation_x;
        ship->weapons[i][1] += translation_y;

        //compute flipping
        switch(flipped){
            case VERTICAL:
                ship->weapons[i][0] = x0 + ship->img->header.largeur - (ship->weapons[i][0]-x0) - 1;
                break;
            case HORIZONTAL:
                ship->weapons[i][1] = y0 + ship->img->header.hauteur - (ship->weapons[i][1]-y0) - 1;
                break;
            default:
                break;
        }

        //compute zoom
        if(zoom > 0){
            ship->weapons[i][0] = x0 + (int)(float)((ship->weapons[i][0] - x0) * zoom);
            ship->weapons[i][1] = y0 + (int)(float)((ship->weapons[i][1] - y0) * zoom);
        }

        if(angle % 360 != 0){
            double sinVal = sin((double)(angle*M_PI)/180);
            double cosVal = cos((double)(angle*M_PI)/180);
            double translate_x = (double)ship->weapons[i][0] - (ship->img->center[0] + x0);
            double translate_y = (double)ship->weapons[i][1] - (ship->img->center[1] + y0);
            //| cos    -sin |   | x - center_x - x0 |   | center_x |   | x0 |
            //| sin     cos | * | y - center_y - y0 | + | center_y | + | y0 |
            ship->weapons[i][0] = (int)((cosVal*translate_x) - (sinVal*translate_y)) + (ship->img->center[0] + x0);
            ship->weapons[i][1] = (int)((sinVal*translate_x) + (cosVal*translate_y)) + (ship->img->center[1] + y0);
        }
    }

    return 0;
}
