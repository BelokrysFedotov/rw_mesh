/*
 * rw_mesh_mesh.h
 * Фунции работы с форматом mesh
 *
 *  Created on: 26.06.2013
 *      Author: Belokrys-Fedotov A.I.
 */

#ifndef RW_MESH_MESH_H_
#define RW_MESH_MESH_H_

#include "rw_mesh_header.h"

#define EXTENTION_MESH "*\.mesh"

/*rw_mesh_mesh.c*/
int write_format_mesh (int nv, REAL* v, int* emskv, int dim,
					   int ncells_T, INT3* cells_T, int* emskc_T,
					   int ncells_Q, INT4* cells_Q, int* emskc_Q,
					   int ntetr, INT4* tetr, int* emsk_tetr,
					   int nhex, INT8* hex, int* emsk_hex,
					   char filename[256]);
int read_format_mesh(int *out_nv, REAL3 **out_v, int** out_emskv,
					 int *out_ncells_T, INT3 **out_cells_T, int** out_emskc_T,
					 int *out_ncells_Q, INT4 **out_cells_Q, int** out_emskc_Q,
					 int *out_ntetr, INT4 **out_tetr, int **out_emsk_tetr,
					 int *out_nhex, INT8 **out_hex, int **out_emsk_hex,
					 char filename[256]);
/*rw_mesh_mesh.c*/


#endif /* RW_MESH_MESH_H_ */
