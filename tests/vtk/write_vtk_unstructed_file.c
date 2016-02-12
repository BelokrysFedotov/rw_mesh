/*
 * write_vtk_structed_file.c
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
	int i,j,k;
	int rn;
	char filename[256];

	if(argc<2){
		printf("Dont set filename\n");
		return 1;
	}

	strcpy(filename,argv[1]);

	printf("Filename = '%s'\n",filename);

	struct RW_MESH_VTK_STRUCT*VTK;



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
	int*CellMasks;
	int CountOfCellFunctions;
	REAL*CellFunctions;

	CountOfPoints = 4;
	Points = (REAL3*)calloc(CountOfPoints,sizeof(REAL3));

	Points[0][0] = 0.e0;	Points[0][1] = 0.e0;	Points[0][2] = 0.e0;
	Points[1][0] = 1.e0;	Points[1][1] = 0.e0;	Points[1][2] = 0.e0;
	Points[2][0] = 0.e0;	Points[2][1] = 1.e0;	Points[2][2] = 0.e0;
	Points[3][0] = 0.e0;	Points[3][1] = 0.e0;	Points[3][2] = 1.e0;

	CountOfPointMasks = 1;
	PointMasks = (int*)calloc(CountOfPoints,sizeof(int));
	for(i=0;i<CountOfPoints;i++)
		PointMasks[i] = i+1;
	CountOfPointFunctions = 1;
	PointFunctions = (double*)calloc(CountOfPoints,sizeof(double));
	for(i=0;i<CountOfPoints;i++)
		PointFunctions[i] = 1.e0*i/CountOfPoints;

	CountOfCells = 1;
	Cells = (int*)calloc(4,sizeof(int));
	CellSizes = (int*)calloc(CountOfCells,sizeof(int));
	CellTypes = (int*)calloc(CountOfCells,sizeof(int));
	CellOffset = (int*)calloc(CountOfCells+1,sizeof(int));

	Cells[0] = 0; Cells[1] = 1; Cells[2] = 2; Cells[3] = 3;
	CellSizes[0] = 4;
	CellTypes[0] = RW_MESH_VTK_CELL_TYPE_TETRA;
	CellOffset[0] = 0;
	CellOffset[1] = 4;

	CountOfCellMasks = 1;
	CellMasks = (int*)calloc(CellOffset[CountOfCells],sizeof(int));
	for(i=0;i<CellOffset[CountOfCells];i++)
		CellMasks[i] = i+1;
	CountOfCellFunctions = 1;
	CellFunctions = (double*)calloc(CellOffset[CountOfCells],sizeof(double));
	for(i=0;i<CellOffset[CountOfCells];i++)
		CellFunctions[i] = 1.e0*i/CellOffset[CountOfCells];


	rn = write_format_vtk_unstructured_simplified(CountOfPoints,Points,CountOfPointMasks,PointMasks,
			CountOfPointFunctions,PointFunctions,
			CountOfCells,Cells,CellSizes,CellTypes,CellOffset,CountOfCellMasks,CellMasks,CountOfCellFunctions,CellFunctions,
			filename,0);
//	rn = write_format_vtk_unstructured_simplified(CountOfPoints,Points,CountOfPointMasks,PointMasks,
//			CountOfPointFunctions,PointFunctions,
//			CountOfCells,Cells,CellSizes,CellTypes,CellOffset,CountOfCellMasks,CellMasks,CountOfCellFunctions,CellFunctions,
//			filename,RW_MESH_VTK_BINARY);

	ffree(Points);
	ffree(Cells);
	ffree(CellSizes);
	ffree(CellTypes);
	ffree(CellOffset);

	ffree(PointMasks);
	ffree(PointFunctions);
	ffree(CellMasks);
	ffree(CellFunctions);

	if(rn){
		rw_mesh_print_error();
	}
	printf("END");
	return rn;
}
