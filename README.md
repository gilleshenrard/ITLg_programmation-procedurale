# Programmation procedurale
## Dossier 1
---
### 1. Intro
This assignment aims to apply scientific programmation to image editing without the help of graphical C libraries.
The final purpose is to develop an application which generates 24 bits 800x500 BITMAP images, which are meant to be read at 16 fps by a program provided by the teacher. A unit test application is to be developped as well. 

### 2. Set up
The assignment is programmed in C98, compiled with GNU GCC.

### 3. Current features (not developped by the teacher)
* Image embedding procedure :
```C
image* embed_image(image* overlay, image* background, uint x, uint y, float alpha);
```
* Specific pixel drawing procedure :
```C
int set_pixel_rgba(image* img, uint x, uint y, int colour, int intensity, float alpha);
```
* Ligns drawing procedure :
```C
int draw_line_generic(image* img, line* l);	//sorts which algorithm to use
int draw_line_Bresenham(image *img, line* l);	//draws a lign using Bresenham's algorithm (no anti-aliasing)
int draw_line_Wu(image* img, line* l);		//draws a lign using Xiaolin Wu's algorithm (with anti-aliasing)
int draw_line_vertical(image* img, line* l);
int draw_line_horizontal(image* img, line* l);
int draw_line_diagonal(image* img, line* l);
```
* Unit tests for all existing procedures, in [Test_Image.c](https://github.com/gilleshenrard/ITLg_programmation-procedurale/blob/Dossier1/Test_Image.c)
