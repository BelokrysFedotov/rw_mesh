/*
 * rw_mesh.h
 *
 *  Created on: 26.06.2013
 *      Author: moric
 */

#ifndef RW_MESH_H_
#define RW_MESH_H_

#include "rw_mesh_header.h"

#include "rw_mesh_string.h"

#include "rw_mesh_off.h"
#include "rw_mesh_mesh.h"


/*wr_vtk.c*/
int write_format_vtk(int nv, REAL* v, int* mskv, int dim,
					 int ncells, int* cells, int* cell_sizes,
					 int* cell_types, int *cell_mask, char filename[256]);
int read_format_vtk(int *out_nv, REAL3** out_v, int **out_mskv,
					int *out_ncells, int** out_cells, int** out_cell_sizes,
					int **out_cell_types, int **out_cell_mask, char filename[256]);
/*wr_vtk.c*/

#endif /* RW_MESH_H_ */
