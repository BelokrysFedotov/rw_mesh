/*created 20.06.2012*/
#ifndef WR_H_
#define WR_H_

#include "header.h"

typedef int int8[8];

/*wr_mesh.c*/
int write_format_mesh (int nv, REAL* v, int* emskv, int dim,
					   int ncells_T, int3* cells_T, int* emskc_T, 
					   int ncells_Q, int4* cells_Q, int* emskc_Q, 
					   int ntetr, int4* tetr, int* emsk_tetr,
					   int nhex, int8* hex, int* emsk_hex,
					   char filename[256]);
int read_format_mesh(int *out_nv, REAL3 **out_v, int** out_emskv, 
					 int *out_ncells_T, int3 **out_cells_T, int** out_emskc_T, 
					 int *out_ncells_Q, int4 **out_cells_Q, int** out_emskc_Q, 
					 int *out_ntetr, int4 **out_tetr, int **out_emsk_tetr,
					 int *out_nhex, int8 **out_hex, int **out_emsk_hex,
					 char filename[256]);
/*wr_mesh.c*/

/*wr_vtk.h*/
int write_format_vtk(int nv, REAL* v, int* mskv, int dim, 
					 int ncells, int* cells, int* cell_sizes, 
					 int* cell_types, int *cell_mask, char filename[256]);
int read_format_vtk(int *out_nv, REAL3** out_v, int **out_mskv,
					int *out_ncells, int** out_cells, int** out_cell_sizes, 
					int **out_cell_types, int **out_cell_mask, char filename[256]);
/*wr_vtk.h*/
#endif /*WR_H_*/
