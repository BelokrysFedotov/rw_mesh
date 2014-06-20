/*
 * format_mesh.c
 *
 *  Created on: 22.02.2012
 *      Author: moric
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "header.h"
#include "format_mesh.h"

int read_line (FILE * STREAM, char *line)
{
  int i;

  if( fgets (line, 255, STREAM) == NULL)
  {
    printf("end of file reached while reading input file \n");
    return -1;
  }
  i = strlen(line);
  return i;
}

int read_format_mesh(int *out_nv, REAL3 **out_v,
					 int *out_ntri, int3 **out_tri, int **out_trimask,
					 int *out_nquad, int4 **out_quad, int **out_quadmask,
					 char filename[256])
{
  char word[256], word1[256], *currword, *currword1;
  int i, k, sbaglio, maskv, nc;
  int nv, NumTri, NumQuad;
  int *TriMask, *QuadMask;
  REAL3 *v;
  int3 *TriFaces;
  int4 *QuadFaces;
  FILE *LUN;

  TriMask=0;
  QuadMask=0;
  TriFaces=0;
  QuadFaces=0;

    LUN = fopen (filename, "r");
    if (LUN == NULL)
      goto L_100;

    nv = 0;
    NumTri = 0;
	NumQuad = 0;

    strcpy (word, " ");

    sbaglio = read_line (LUN, word);
    if (sbaglio == -1)
      goto L_100;


    while (1)
    {

      sscanf (word, " %s", word1);
      currword = strstr (word1, "Vertices");
      if (currword != NULL)
        break;
      currword1 = strstr (word1, "End");
      if (currword1 != NULL)
        goto L_100;

       sbaglio = read_line (LUN, word);
    }

    if (currword == NULL)
      goto L_100;

    sbaglio = read_line (LUN, word);
    sscanf (word, " %d", &( nv));
    printf ("nv %d \n",  nv);

	 v=(REAL3*)calloc( nv,sizeof(REAL3));
    //rallocmesh(mesh);
    for (k = 0; k <  nv; k++)
    {
      sbaglio = read_line (LUN, word);
      sscanf (word, " %le %le %le %d", &( v[k][0]),
                                       &( v[k][1]),
                                       &( v[k][2]), &maskv);
    }
    sbaglio = read_line (LUN, word);
    printf (" %s \n", word);

    while (1)
    {
      sscanf (word, " %s", word1);
      currword = strstr (word1, "End");
      if (currword != NULL) break;

      currword = strstr (word1, "Triangles");
      currword1 = strstr (word1, "Quadrilaterals");

      /*      printf ("%s %s \n", currword, currword1); */

	  // found triangles
      if (currword != NULL)
      {
        sbaglio = read_line (LUN, word);
        sscanf (word, " %d", &nc);
		 NumTri = nc;

		printf ("ncells %d  \n",  NumTri);
		 TriFaces = (int3*) calloc(nc,sizeof(int3));
		 TriMask = (int*) calloc(nc, sizeof(int));

        for (k = 0; k < nc; k++)
        {
          sbaglio = read_line (LUN, word);
          sscanf (word, " %d %d %d %d ", &( TriFaces[k][0]),
                  &( TriFaces[k][1]),&( TriFaces[k][2]),
                  &( TriMask[k]));
          for (i = 0; i < 3; i++)
            ( TriFaces)[k][i] = ( TriFaces)[k][i] - 1;
        }
      }

	  // found quadrilaterals
	  if (currword1 != NULL)
      {
        sbaglio = read_line (LUN, word);
        sscanf (word, " %d", &nc);
		 NumQuad = nc;

		printf ("nquad %d \n",  NumQuad);
		 QuadFaces = (int4*) calloc(nc, sizeof(int4));
		 QuadMask = (int*) calloc(nc, sizeof(int));

        for (k = 0; k < nc; k++)
        {
          sbaglio = read_line (LUN, word);
          sscanf (word, " %d %d %d %d %d", &( QuadFaces[k][0]),
			  &( QuadFaces[k][1]),&( QuadFaces[k][2]), &( QuadFaces[k][3]),
                  &( QuadMask[k]));
          for (i = 0; i < 4; i++)
            ( QuadFaces)[k][i] = ( QuadFaces)[k][i] - 1;
        }
      }
      sbaglio = read_line (LUN, word);
    }

	printf ("nv %d, nc %d nq %d \n", nv,  NumTri,  NumQuad);
	if (out_nv != NULL)
		*out_nv = nv;

	if (out_v != NULL)
		*out_v = v;

	if (out_ntri != NULL)
		*out_ntri = NumTri;

	if (out_tri != NULL)
		*out_tri = TriFaces;
	else
		if(TriFaces)free(TriFaces);

	if (out_trimask != NULL)
		*out_trimask = TriMask;
	else
		if(TriMask)free(TriMask);

	if (out_nquad != NULL)
		*out_nquad = NumQuad;

	if (out_quad != NULL)
		*out_quad = QuadFaces;
	else
		if(QuadFaces)free(QuadFaces);

	if (out_quadmask != NULL)
		*out_quadmask = QuadMask;
	else
		if(QuadMask)free(QuadMask);

	fclose(LUN);
    return (0);

L_100:
  ;
  fprintf (stdout, "error opening/reading input .mesh file %s \n",
           filename);
  /*if (cfg->INANDOUT != NULL)
    fprintf (cfg->INANDOUT, "error opening/reading input .mesh file %s \n",
             filename);*/
  return (-1);

}
