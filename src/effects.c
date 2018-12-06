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
        img->pic[x][y][i] = (col[i]*alpha) + (img->pic[x][y][i] * (1.0-alpha));

    return 1;
}

/****************************************************************************************/
/*  I : original image in the copy of which draw a line                                 */
/*      metadata of the line to draw                                                    */
/*  P : Creates a copy of the image and draws a line in it (using Bresenham's algo)     */
/*  O : resulting image of the embedding                                                */
/*      NULL if error                                                                   */
/****************************************************************************************/
image* draw_line_Bresenham(image* original, line* l){
    image *buffer = NULL;
    uint deltaX = abs(l->xa - l->xb), deltaY = abs(l->ya - l->yb);
    uint p = 2 * deltaY - deltaX;
    uint x, y, xEnd;

    buffer = copy_image(original);

    if (l->xa > l->xb){
        x = l->xb;
        y = l->yb;
        xEnd = l->xa;
    }else{
        x = l->xa;
        y = l->ya;
        xEnd = l->xb;
    }

    set_pixel_rgba(buffer, x, y, l->colour, l->intensity, l->alpha);

    while(x < xEnd){
        x++;
        if(p < 0)
            p += 2 * deltaY;
        else{
            y++;
            p += 2 * (deltaY - deltaX);
        }
        set_pixel_rgba(buffer, x, y, l->colour, l->intensity, l->alpha);
    }

    return buffer;
}
