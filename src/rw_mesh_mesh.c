/*
 * rw_mesh_mesh.c
 * Фунции работы с форматом mesh
 *
 *  Created on: 26.06.2013
 *      Author: moric
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "rw_mesh_mesh.h"


int read_format_mesh(int *out_nv, REAL3 **out_v, int** out_emskv,
					 int *out_ncells_T, INT3 **out_cells_T, int** out_emskc_T,
					 int *out_ncells_Q, INT4 **out_cells_Q, int** out_emskc_Q,
					 int *out_ntetr, INT4 **out_tetr, int **out_emsk_tetr,
					 int *out_nhex, INT8 **out_hex, int **out_emsk_hex,
					 char filename[256]){
	char word[256], word1[256];
	char *currword, *currword1, *currword2, *currword3;
	int i, k, sbaglio;
	FILE *LUN;
	int nv, ncells_T, ncells_Q, ntetr, nhex;
	REAL3 *v;
	int *mskv, *mskt, *mskq, *msktetr, *mskhex;
	INT3 *cells_T;
	INT4 *cells_Q, *tetr;
	INT8 *hex;

	LUN = fopen (filename, "r");
	if (LUN == NULL)
	{
		 log_write ( "error opening/reading input .mesh file %s \n", filename);
		 return -1;
	}

	strcpy (word, " ");

	sbaglio = mesh_read_line (LUN, word);
	if (sbaglio == -1)
	{
		log_write ( "input .mesh file %s is empty \n", filename);
		 return -1;
	}

	nv = 0;
	ncells_T = 0;
	ncells_Q = 0;
	ntetr = 0;
	nhex = 0;

	v = NULL;
	cells_T = NULL;
	cells_Q = NULL;
	tetr = NULL;
	hex = NULL;

	mskv = NULL;
	mskt = NULL;
	mskq = NULL;
	msktetr = NULL;
	mskhex = NULL;

    while (1)
    {

      sscanf (word, " %s", word1);
      currword = strstr (word1, "Vertices");
      if (currword != NULL)
        break;
      currword1 = strstr (word1, "End");
      if (currword1 != NULL)
      {
    	 log_write ( "no vertices in input .mesh file %s \n", filename);
		 return -1;
	  }

       sbaglio = mesh_read_line (LUN, word);
    }

    if (currword == NULL)
	{
    	log_write ( "error opening/reading input .mesh file %s \n", filename);
		 return -1;
	}

    sbaglio = mesh_read_line (LUN, word);
    sscanf (word, " %d", &(nv));
    //printf ("nv %d \n", nv);

	v = (REAL3*) malloc(nv * sizeof(REAL3));
	mskv = (int*) malloc(nv * sizeof(int));

	//rallocmesh(mesh);
	for (k = 0; k < nv; k++)
	{
		 sbaglio = mesh_read_line (LUN, word);
		 sscanf (word, " %lf %lf %lf %d", &(v[k][0]),
										  &(v[k][1]),
										  &(v[k][2]), &mskv[k]);
	}

	if (out_nv != NULL)
		*out_nv = nv;

	if (out_v != NULL)
		*out_v = v;
	else
		free(v);

	if (out_emskv != NULL)
		*out_emskv = mskv;
	else
		free(mskv);

    sbaglio = mesh_read_line (LUN, word);
    //printf (" %s \n", word);

    while (1)
    {
      sscanf (word, " %s", word1);
      currword = strstr (word1, "End");
      if (currword != NULL) break;

      currword = strstr (word1, "Triangles");
      currword1 = strstr (word1, "Quadrilaterals");
	  currword2 = strstr (word1, "Tetrahedra");
      currword3 = strstr (word1, "Hexahedra");

	  // found triangles
      if (currword != NULL)
      {
        sbaglio = mesh_read_line (LUN, word);
        sscanf (word, " %d", &ncells_T);

		cells_T = (INT3*) calloc(ncells_T, sizeof(INT3));
		mskt = (int*) calloc(ncells_T, sizeof(int));

		for (k = 0; k < ncells_T; k++)
		{
		  sbaglio = mesh_read_line (LUN, word);
		  sscanf (word, " %d %d %d %d ", &(cells_T[k][0]),
				  &(cells_T[k][1]),&(cells_T[k][2]),
				  &(mskt[k]));
		  for (i = 0; i < 3; i++)
			cells_T[k][i] = cells_T[k][i] - 1;
		}

		if (out_ncells_T != NULL)
			*out_ncells_T = ncells_T;

		if (out_cells_T != NULL)
			*out_cells_T = cells_T;
		else
			free(cells_T);

		if (out_emskc_T != NULL)
			*out_emskc_T = mskt;
		else
			free(mskt);
      }

	  // found quadrilaterals
	  if (currword1 != NULL)
      {
        sbaglio = mesh_read_line (LUN, word);
        sscanf (word, " %d", &ncells_Q);

		cells_Q = (INT4*) malloc(ncells_Q * sizeof(INT4));
		mskq = (int*) malloc(ncells_Q * sizeof(int));

		for (k = 0; k < ncells_Q; k++)
		{
		  sbaglio = mesh_read_line (LUN, word);
		  sscanf (word, " %d %d %d %d %d", &(cells_Q[k][0]),
			  &(cells_Q[k][1]),&(cells_Q[k][2]), &(cells_Q[k][3]),
				  &(mskq[k]));
		  for (i = 0; i < 4; i++)
			cells_Q[k][i] = cells_Q[k][i] - 1;
		}
		if (out_ncells_Q != NULL)
			*out_ncells_Q = ncells_Q;

		if (out_cells_Q != NULL)
			*out_cells_Q = cells_Q;
		else
			free(cells_Q);

		if (out_emskc_Q != NULL)
			*out_emskc_Q = mskq;
		else
			free(mskq);

      }

	  // found tetrahedra
      if (currword2 != NULL)
      {
        sbaglio = mesh_read_line (LUN, word);
        sscanf (word, " %d", &ntetr);

		tetr = (INT4*) malloc(ntetr * sizeof(INT4));
		msktetr = (int*) malloc(ntetr * sizeof(int));

		for (k = 0; k < ntetr; k++)
		{
		  sbaglio = mesh_read_line (LUN, word);
		  sscanf (word, " %d %d %d %d %d", &(tetr[k][0]),
				  &(tetr[k][1]),&(tetr[k][2]), &(tetr[k][3]),
				  &(msktetr[k]));
		  for (i = 0; i < 4; i++)
			tetr[k][i] = tetr[k][i] - 1;
		}

		if (out_ntetr != NULL)
			*out_ntetr = ntetr;

		if (out_tetr != NULL)
			*out_tetr = tetr;
		else
			free(tetr);

		if (out_emsk_tetr != NULL)
			*out_emsk_tetr = msktetr;
		else
			free(msktetr);

      }

	  // found quadrilaterals
	  if (currword3 != NULL)
      {
        sbaglio = mesh_read_line (LUN, word);
        sscanf (word, " %d", &nhex);

		hex = (INT8*) malloc(nhex * sizeof(INT8));
		mskhex = (int*) malloc(nhex * sizeof(int));

		for (k = 0; k < nhex; k++)
		{
		  sbaglio = mesh_read_line (LUN, word);
		  sscanf (word, " %d %d %d %d %d %d %d %d  %d", &(hex[k][0]),
			  &(hex[k][1]), &(hex[k][2]), &(hex[k][3]),
			  &(hex[k][4]), &(hex[k][5]), &(hex[k][6]),
			  &(hex[k][7]), &(mskhex[k]));
		  for (i = 0; i < 8; i++)
			hex[k][i] = hex[k][i] - 1;
		}

		if (out_nhex != NULL)
			*out_nhex = nhex;

		if (out_hex != NULL)
			*out_hex = hex;
		else
			free(hex);

		if (out_emsk_hex != NULL)
			*out_emsk_hex = mskhex;
		else
			free(mskhex);
      }

      sbaglio = mesh_read_line (LUN, word);
    }

	//printf ("nv %d, ntri %d nq %d ntetr %d nhex %d \n", nv, ncells_T, ncells_Q, ntetr, nhex);

	fclose(LUN);
    return (0);
}



int write_format_mesh (int nv, REAL* v, int* emskv, int dim,
					   int ncells_T, INT3* cells_T, int* emskc_T,
					   int ncells_Q, INT4* cells_Q, int* emskc_Q,
					   int ntetr, INT4* tetr, int* emsk_tetr,
					   int nhex, INT8* hex, int* emsk_hex,
					   char filename[256])
{
  char word[256];
  int i, k;
  FILE *LUN;
  int *mskv, *mskc_T, *mskc_Q, *msk_tetr, *msk_hex;

  /* the numbering in index arrays is started from 1 here! */
  if (strlen(filename) == 0) goto L_777;

  LUN = fopen (filename, "w");
  if (LUN == NULL)
    goto L_100;

  if (emskv == NULL)
	  mskv = (int*)calloc(nv, sizeof(int));
  else
	  mskv = emskv;

  if (ncells_T != 0){
	  if (emskc_T == NULL)
		  mskc_T = (int*)calloc(ncells_T, sizeof(int));
	  else
		  mskc_T = emskc_T;
  }

  if (ncells_Q != 0){
	  if (emskc_Q == NULL)
		  mskc_Q = (int*)calloc(ncells_Q, sizeof(int));
	  else
		  mskc_Q = emskc_Q;
  }

  if (ntetr != 0){
	  if (emsk_tetr == NULL)
		  msk_tetr = (int*)calloc(ntetr, sizeof(int));
	  else
		  msk_tetr = emsk_tetr;
  }

  if (nhex != 0){
	  if (emsk_hex == NULL)
		  msk_hex = (int*)calloc(nhex, sizeof(int));
	  else
		  msk_hex = emsk_hex;
  }

  sprintf (word, "%s", "MeshVersionFormatted 1");
  fprintf (LUN, " %s\n", word);
  sprintf (word, "%s", "Dimension");
  fprintf (LUN, " %s\n", word);
  fprintf (LUN, " %d\n", 3);
  sprintf (word, "%s", "Vertices");
  fprintf (LUN, " %s\n", word);
  fprintf (LUN, " %d\n", nv);


  if (dim == 2)
	  for (k = 0; k < nv; k++)
		fprintf (LUN, " %le %le %le %d \n", v[2 * k + 0],
										 v[2 * k + 1],
										 0.0e0,
										 mskv[k]);

  if (dim == 3)
	  for (k = 0; k < nv; k++)
		fprintf (LUN, " %le %le %le %d \n", v[3 * k + 0],
										 v[3 * k + 1],
										 v[3 * k + 2],
										 mskv[k]);

  /* compute number of triangles and quadrilaterals */

  if (ncells_T > 0)
  {

    sprintf (word, "%s", "Triangles");
    fprintf (LUN, " %s\n", word);
    fprintf (LUN, " %d\n", ncells_T);
    for (i = 0; i < ncells_T; i++)
      fprintf (LUN, " %d %d %d %d \n",
           cells_T[i][0] + 1,
           cells_T[i][1] + 1,
           cells_T[i][2] + 1,
           mskc_T[i]);
  }
  if (ncells_Q > 0)
  {

    sprintf (word, "%s", "Quadrilaterals");
    fprintf (LUN, " %s\n", word);
    fprintf (LUN, " %d\n", ncells_Q);
    for (i = 0; i < ncells_Q; i++)
      fprintf (LUN, " %d %d %d %d %d \n",
           cells_Q[i][0] + 1,
           cells_Q[i][1] + 1,
           cells_Q[i][2] + 1,
           cells_Q[i][3] + 1,
           mskc_Q[i]);
  }
  if (ntetr > 0)
  {

    sprintf (word, "%s", "Tetrahedra");
    fprintf (LUN, " %s\n", word);
    fprintf (LUN, " %d\n", ntetr);
    for (i = 0; i < ntetr; i++)
      fprintf (LUN, " %d %d %d %d %d \n",
           tetr[i][0] + 1,
           tetr[i][1] + 1,
           tetr[i][2] + 1,
           tetr[i][3] + 1,
           msk_tetr[i]);
  }
  if (nhex > 0)
  {
    sprintf (word, "%s", "Hexahedra");
    fprintf (LUN, " %s\n", word);
    fprintf (LUN, " %d\n", nhex);
    for (i = 0; i < nhex; i++)
      fprintf (LUN, " %d %d %d %d %d %d %d %d %d\n",
           hex[i][0] + 1,
           hex[i][1] + 1,
           hex[i][2] + 1,
           hex[i][3] + 1,
		   hex[i][4] + 1,
           hex[i][5] + 1,
		   hex[i][6] + 1,
           hex[i][7] + 1,
           msk_hex[i]);
  }
  sprintf (word, "%s", "End");
  fprintf (LUN, " %s\n", word);

  if (nhex != 0)
	  if (emsk_hex == NULL)
		  free(msk_hex);

  if (ntetr != 0)
	  if (emsk_tetr == NULL)
		  free(msk_tetr);

  if (ncells_Q != 0)
	  if (emskc_Q == NULL)
		  free(mskc_Q);

  if (ncells_T != 0)
	  if (emskc_T == NULL)
		  free(mskc_T);

   if (emskv == NULL)
	  free(mskv);
  fclose (LUN);

L_777:
  ;
  return (0);

L_100:
  ;
  fprintf (stdout, "error opening/reading output .mesh file %s \n",
           filename);

  return (-1);
}

