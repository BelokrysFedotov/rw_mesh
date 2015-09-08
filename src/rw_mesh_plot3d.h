/*
 * rw_mesh_plot3d.h
 * Фунции работы с форматом plot3d
 *
 *  Created on: 02.08.2013
 *      Author: moric
 */

#ifndef RW_MESH_PLOT3D_H_
#define RW_MESH_PLOT3D_H_

#include "rw_mesh_header.h"

#define EXTENTION_PLOT3D_2D "*\.p2dfmt"
#define EXTENTION_PLOT3D_3D "*\.p3dfmt"
#define EXTENTION_PLOT3D "*\.p[23]dfmt"

struct plot3d_struct{
	int dimension;
	int ni,nj,nk;
	REAL*points;
};

int read_format_pxdfmt(
			int*CountOfBlocks,
			int*Dimension,
			struct plot3d_struct**Blocks,
			char*filename);

int read_format_p2dfmt(
			int*CountOfBlocks,
			int**NI, int**NJ,
			REAL2***Points,
			char*filename);

int read_format_p3dfmt(
			int*CountOfBlock,
			int**NI, int**NJ, int**NK,
			REAL3***Points,
			char*filename);


#endif /* RW_MESH_PLOT3D_H_ */
