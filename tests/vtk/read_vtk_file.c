/*
 * readline.c
 *
 *  Created on: 26.06.2013
 *      Author: moric
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../../src/rw_mesh.h"

int main (int argc, char *argv[]) {
	FILE *fd;
	int i;
	int rn;
	char filename[256];

	if(argc<2){
		printf("Dont set filename\n");
		return 1;
	}

	strcpy(filename,argv[1]);

	printf("Filename = '%s'\n",filename);

	if(0){
		struct RW_MESH_VTK_UNSTRUCTURED_GRID_STRUCT*VTK;
		VTK = (struct RW_MESH_VTK_UNSTRUCTURED_GRID_STRUCT*)calloc(1,sizeof(struct RW_MESH_VTK_UNSTRUCTURED_GRID_STRUCT));
		rw_mesh_vtk_struct_init(VTK);

		rn = read_format_vtk_unstructured_in_struct(VTK,filename,0);
	}else{
		int CountOfPoints;
		REAL3*Points;

		int CountOfPointMasks;
		int*PointMasks;
		int CountOfPointFunctions;
		REAL*PointFunctions;

		int CountOfCells;
		int*Cells;
		int*CellSizes;
		int*CellTypes;
		int*CellOffset;

		int CountOfCellMasks;
		int *CellMasks;
		int CountOfCellFunctions;
		REAL*CellFunctions;

		rn = read_format_vtk_unstructured_simplified(
				&CountOfPoints,&Points,
				&CountOfPointMasks,&PointMasks,
				&CountOfPointFunctions,&PointFunctions,
				&CountOfCells,&Cells,
				&CellSizes,&CellTypes,&CellOffset,
				&CountOfCellMasks,&CellMasks,
				&CountOfCellFunctions,&CellFunctions,
				filename,0);
	}
	if(rn){
		rw_mesh_print_error();
	}
	printf("END");
	return rn;
}
