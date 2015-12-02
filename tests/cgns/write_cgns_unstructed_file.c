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

	int BC_Counts;
	int*BC_TriFacesCount;
	int**BC_TriFaces;
	int*BC_QuadFacesCount;
	int**BC_QuadFaces;
	int*BC_Names;



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

	BC_Counts = 3;
	BC_TriFacesCount = (int*)calloc(BC_Counts,sizeof(int));
	BC_TriFaces = (int**)calloc(BC_Counts,sizeof(int*));
	BC_QuadFacesCount = 0;
	BC_QuadFaces = 0;
	BC_Names = (int*)calloc(BC_Counts,sizeof(int));

	BC_TriFacesCount[0] = 1;
	BC_TriFaces[0] = (int*)calloc(BC_TriFacesCount[0]*3,sizeof(int));
	BC_TriFaces[0][0] = 0;
	BC_TriFaces[0][1] = 2;
	BC_TriFaces[0][2] = 1;
	BC_Names[0] = 10;

	BC_TriFacesCount[1] = 2;
	BC_TriFaces[1] = (int*)calloc(BC_TriFacesCount[1]*3,sizeof(int));
	BC_TriFaces[1][0] = 0;
	BC_TriFaces[1][1] = 3;
	BC_TriFaces[1][2] = 2;
	BC_TriFaces[1][3] = 0;
	BC_TriFaces[1][4] = 1;
	BC_TriFaces[1][5] = 3;
	BC_Names[1] = 12;

	BC_TriFacesCount[2] = 1;
	BC_TriFaces[2] = (int*)calloc(BC_TriFacesCount[2]*3,sizeof(int));
	BC_TriFaces[2][0] = 1;
	BC_TriFaces[2][1] = 2;
	BC_TriFaces[2][2] = 3;
	BC_Names[2] = 9;


	rn = write_format_cgns_unstructured_simplified_with_bc(CountOfPoints,Points,CountOfPointMasks,PointMasks,
			CountOfPointFunctions,PointFunctions,CountOfCells,Cells,CellSizes,CellTypes,CellOffset,
			CountOfCellMasks,CellMasks,CountOfCellFunctions,CellFunctions,
			BC_Counts,BC_TriFacesCount,BC_TriFaces,BC_QuadFacesCount,BC_QuadFaces,BC_Names,
			filename,0);

	ffree(Points);
	ffree(Cells);
	ffree(CellSizes);
	ffree(CellTypes);
	ffree(CellOffset);

	ffree(PointMasks);
	ffree(PointFunctions);
	ffree(CellMasks);
	ffree(CellFunctions);

	ffree(BC_TriFacesCount);
	for(i=0;i<BC_Counts;i++)
		ffree(BC_TriFaces[i]);
	ffree(BC_TriFaces);
	ffree(BC_Names);



	if(rn){
		rw_mesh_print_error();
	}
	printf("END\n");
	return rn;
}
