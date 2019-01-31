#include "../lib/film.h"

/****************************************************************************************/
/*  I : Movie structure to save to disk                                                 */
/*  P : Writes a metadata file with the information provided in the structure           */
/*  O : -1 if error                                                                     */
/*       0 otherwise                                                                    */
/****************************************************************************************/
int save_movie(film* movie){
    char filename[64] = {0};
    FILE* file=NULL;

    sprintf(filename, "..\\Films\\%s\\%s.dat", movie->nm_film, movie->nm_film);
    file = fopen(filename, "wb");
    if(file == NULL)
        return 1;

    fwrite(movie, sizeof(film), 1, file);
    printf("\n%s saved\n", filename);

    fclose(file);
    return 0;
}

/****************************************************************************************/
/*  I : Movie structure to populate                                                     */
/*      Number of frames in the scene to regiser                                        */
/*  P : Increments the scenes counter and registers the number of frames                */
/*          in the right slot                                                           */
/*  O : -1 if error                                                                     */
/*       0 otherwise                                                                    */
/****************************************************************************************/
int register_scene(film* movie, int nb_frames){
    movie->nb_scene += 1;
    movie->scene[movie->nb_scene] = nb_frames;

    return 0;
}
