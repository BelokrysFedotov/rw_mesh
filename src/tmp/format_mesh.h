/*
 * format_mesh.h
 *
 *  Created on: 22.02.2012
 *      Author: moric
 */

#ifndef FORMAT_MESH_H_
#define FORMAT_MESH_H_

int read_format_mesh(int *out_nv, REAL3 **out_v,
					 int *out_ntri, int3 **out_tri, int **out_trimask,
					 int *out_nquad, int4 **out_quad, int **out_quadmask,
					 char filename[256]);

#endif /* FORMAT_MESH_H_ */
