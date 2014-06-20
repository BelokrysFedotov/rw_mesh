#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "wr.h"

int str_divide( char* input, const char* separator, char* before, char* after);
int str_trim(char* s);
int char_is_space(char c);
int vtk_read_line (FILE * STREAM, char *line);

int write_format_vtk(int nv, REAL* v, int *mskv, int dim, 
					 int ncells, int* cells, int* cell_sizes, 
					 int* cell_types, int *cell_mask, char filename[256])
{
	char word[256];
	int i, k;
	FILE *LUN; 

	/* the numbering in index arrays is started from 1 here! */
	if (strlen(filename) == 0)
	{
		log_write("empty filename \n");
		return (-1);
	};

	LUN = fopen (filename, "w");
	if (LUN == NULL)
	{
		log_write("can't open vtk-file \n");
		return (-1);
	};

	sprintf (word, "%s", "# vtk DataFile Version 3.0");  
	fprintf (LUN, "%s\n", word);
	sprintf (word, "%s", "vtk output"); 
	fprintf (LUN, "%s\n", word);
	sprintf (word, "%s", "ASCII");      
	fprintf (LUN, "%s\n", word);
	sprintf (word, "%s", "DATASET UNSTRUCTURED_GRID");      
	fprintf (LUN, "%s\n", word);

	sprintf (word, "%s %d double", "POINTS", nv); 

	fprintf (LUN, "%s\n", word);
	if (dim == 2)
		for (i = 0; i < nv; i++)
			fprintf (LUN, "%le %le %le \n", v[2 * i + 0], 
			v[2 * i + 1], 0.0e0);

	if (dim == 3)
		for (i = 0; i < nv; i++)
			fprintf (LUN, "%le %le %le\n", v[3 * i + 0], 
			v[3 * i + 1], v[3 * i + 2]);

    
	if(ncells > 0)
	{
		sprintf (word, "%s %d %d", "CELLS", ncells, cell_sizes[ncells] + ncells);  
		fprintf (LUN, "%s\n", word);	
		  
		for (i = 0; i < ncells; i++)
		{
			fprintf (LUN, "%d ", cell_sizes[i + 1] - cell_sizes[i]);
			for (k = cell_sizes[i]; k < cell_sizes[i + 1]; k++)
				fprintf (LUN, "%d ", cells[k]); 
			fprintf (LUN, "\n");
		}
	}
		
	if (cell_types != NULL)
	{
		sprintf (word, "%s %d", "CELL_TYPES", ncells);   
		fprintf (LUN, "%s\n", word);		
		for (i = 0; i < ncells; i++)
			fprintf (LUN, "%d\n", cell_types[i]); 
	}

	if (mskv != NULL)
	{
		sprintf (word, "%s", "FIELD FieldData 1");  
		fprintf (LUN, "%s\n", word);	
		sprintf (word, "%s %d %s", "optimization-points 1 ", nv, " vtkIdType");
		fprintf (LUN, "%s\n", word);		

		for (i = 0; i < nv; i++)
			fprintf (LUN, "%d\n", mskv[i]); 
	}

	if (cell_mask != NULL)
	{
		sprintf (word, "%s %d", "CELL_DATA", ncells); 
		fprintf (LUN, "%s\n", word);
		sprintf (word, "%s", "SCALARS scalars int");
		fprintf (LUN, "%s\n", word);
		sprintf (word, "%s", "LOOKUP_TABLE default");
		fprintf (LUN, "%s\n", word);		

		for (i = 0; i < ncells; i++)
			fprintf (LUN, "%d\n", cell_mask[i]); 
	}
		
	fclose (LUN);

	return (0);
}

int read_format_vtk(int *out_nv, REAL3** out_v, int **out_mskv,
					int *out_ncells, int** out_cells, int** out_cell_sizes, 
					int **out_cell_types, int **out_cell_mask, char filename[256])
{
    char word[256], word1[256], word2[256], word3[256];
    int i, j, elem, sbaglio;
    FILE *LUN;
    int nv, ncells, *mskv, *cells, *cell_sizes, *cell_types, *cell_mask;
    REAL *v;

	nv = 0;
	ncells = 0;

	v = NULL;
	mskv = NULL;
	cells = NULL;
	cell_sizes = NULL;
	cell_types = NULL;
	cell_mask = NULL;

	if (strlen(filename) == 0)
	{
		log_write( "Error reading file: empty filename \n");
		return (1);
	};

	LUN = fopen (filename, "r");
	if (LUN == NULL)
	{
		log_write( "Error reading file: can't open vtk-file \n");
		return (-1);
	}
	      
	strcpy (word, " ");
	      
	sbaglio = vtk_read_line (LUN, word);
	if (sbaglio == -1)
	{		 
		log_write( "Error reading file: vtk-file is empty \n");
		return (-1);
	}
    
	while (sbaglio != -1)
	{
      if (strstr(word, "POINTS") != NULL)
	  {
    	 // log_write( "download points \n");

		  	str_divide(word, " ", word1, word3);
			str_divide(word3, " ", word1, word2);
			sscanf(word1, "%d", &nv);

			v = (REAL*) malloc(3 * nv * sizeof(REAL));
			for (i = 0; i < 3 * nv; i++)
				fscanf(LUN, "%lf", &(v[i]));

			//log_write(stderr, "download ended \n");
	  }  
	  else if (strstr (word, "CELLS") != NULL)
	  {
		  //log_write( "download cells \n");
		  	str_divide(word, " ", word1, word3);
			sscanf(word3, "%d %d", &ncells, &elem);

		    cells = (int*) malloc ((elem - ncells) * sizeof(int));
			cell_sizes = (int*) malloc((ncells + 1) * sizeof(int));

			cell_sizes[0] = 0;
			for (i = 0; i < ncells; i++)
			{
				fscanf(LUN, "%d", &(cell_sizes[i + 1]));
				cell_sizes[i + 1] = cell_sizes[i] + cell_sizes[i + 1];
				for (j = cell_sizes[i]; j < cell_sizes[i + 1]; j++)
					fscanf(LUN, "%d", &(cells[j]));
			}
			//log_write(stderr, "download ended \n");
	  }
	  else if (strstr (word, "CELL_TYPES") != NULL)
	  {
		  //log_write( "download cell types \n");
			str_divide(word, " ", word1, word3);
			sscanf(word3, "%d", &ncells);

		  	cell_types = (int*) malloc(ncells * sizeof(int));
			for (i = 0; i < ncells; i++)
				fscanf(LUN, "%d", &(cell_types[i]));
		
			//log_write(stderr, "download ended \n");
	  }
	  else if (strstr (word, "CELL_DATA") != NULL)
	  {
		  //log_write( "download cell data \n");
			str_divide(word, " ", word1, word3);
			sscanf(word3, "%d", &ncells);
			vtk_read_line (LUN, word);
			vtk_read_line (LUN, word);

		  	cell_mask = (int*) malloc(ncells * sizeof(int));
			for (i = 0; i < ncells; i++)
				fscanf(LUN, "%d", &(cell_mask[i]));

			//log_write( "download ended \n");
	  }
	  else if (strstr (word, "FIELD")!= NULL)
	  {
		  //log_write( "download field \n");
		    vtk_read_line (LUN, word);
			str_divide(word, " ", word1, word3);
			sscanf(word3, "%d %d", &elem, &nv);

		  	mskv = (int*) malloc(nv * sizeof(int));
			for (i = 0; i < nv; i++)
				fscanf(LUN, "%d", &mskv[i]);

			//log_write( "download ended \n");
	  }
	  else if (strstr (word, "POLYGONS") != NULL)
	  {
		  //log_write( "download cells \n");
		  	str_divide(word, " ", word1, word3);
			sscanf(word3, "%d %d", &ncells, &elem);

		    cells = (int*) malloc ((elem - ncells) * sizeof(int));
			cell_sizes = (int*) malloc((ncells + 1) * sizeof(int));
			cell_types = (int*) malloc(ncells * sizeof(int));

			cell_sizes[0] = 0;
			for (i = 0; i < ncells; i++)
			{
				fscanf(LUN, "%d", &(cell_sizes[i + 1]));
				if (cell_sizes[i + 1] == 3)
					cell_types[i] = 5;
				else if (cell_sizes[i + 1] == 4)
					cell_types[i] = 9;
				else 
					cell_types[i] = 7;

				cell_sizes[i + 1] = cell_sizes[i] + cell_sizes[i + 1];
				for (j = cell_sizes[i]; j < cell_sizes[i + 1]; j++)
					fscanf(LUN, "%d", &(cells[j]));
			}
			//log_write( "download ended \n");
	  }
      sbaglio = vtk_read_line (LUN, word);
  }

  if (out_nv != NULL) *out_nv = nv;

  if (out_v != NULL) *out_v = (REAL3*) v;
  else if (v != NULL) free(v);

  if (out_mskv != NULL) *out_mskv = mskv;
  else if (mskv != NULL) free(mskv);

  if (out_ncells != NULL) *out_ncells = ncells;

  if (out_cells != NULL) *out_cells = cells;
  else if (cells != NULL) free(cells);

  if (out_cell_sizes != NULL) *out_cell_sizes = cell_sizes;
  else if (cell_sizes != NULL) free(cell_sizes);

  if (out_cell_types != NULL) *out_cell_types = cell_types;
  else if (cell_types != NULL) free(cell_types);

  if (out_cell_mask != NULL) *out_cell_mask = cell_mask;
  else if (cell_mask != NULL) free(cell_mask);

  return (0);
}
int str_divide( char* input, const char* separator, char* before, char* after)
{
	char* s;
	int len, lens, pos;

	len = strlen(input);
	lens = strlen(separator);
	s = strstr(input, separator);

	if (!s) return (1);

	pos = s-input;
	memcpy( before, input, pos);
	before[pos] = '\0';
	str_trim(before);
	memcpy( after, s + lens, len - lens - pos);
	after[len - lens - pos] = '\0';
	str_trim(after);

	return (0);
}

int str_trim(char* s)
{
	int i, len, lf, rt;
	len = strlen(s);

	lf = 0; 
	while (char_is_space(s[lf]))
		lf++;

	rt = len - 1;
	while (rt > 0 && char_is_space(s[rt]))
		rt--;

	if (rt == 0 && char_is_space(s[rt]))
	{
		s[0] = '\0';
		return (0);
	}

	if (lf > 0)
	{
		for (i = lf; i <= rt; i++)
			s[i - lf] = s[i];
	}

	s[rt - lf + 1] = '\0';
	return (lf + len - rt - 1);
}

int char_is_space(char c)
{
	if (c == ' ') return (1);
	if (c == '\t') return (1);
	if (c == '\r') return (1);
	if (c == '\n') return (1);
	return (0);
}
int vtk_read_line (FILE * STREAM, char *line)
{
  int i;
  
  if (fgets (line, 256, STREAM) == NULL)
      return (-1);

  i = strlen(line);
  return i;
}

