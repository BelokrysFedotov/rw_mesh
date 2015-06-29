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
	int i,k;
	int rn;
	char filename[256];

	if(argc<2){
		printf("Dont set filename\n");
		return 1;
	}

	strcpy(filename,argv[1]);

	printf("Filename = '%s'\n",filename);

	if(0){
		struct RW_MESH_VTK_STRUCT*VTK;
		VTK = (struct RW_MESH_VTK_STRUCT*)calloc(1,sizeof(struct RW_MESH_VTK_STRUCT));
		rw_mesh_vtk_struct_init(VTK);

		rn = read_format_vtk_struct(VTK,filename,0);

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

		if(rn){
			rw_mesh_print_error();
			return 1;
		}

		int*MM;
		MM = (int*)calloc(CountOfPoints,sizeof(int));

		for(i=0;i<CountOfPoints;i++){
			MM[i] = (int)(PointFunctions[i*4+3]);
		}

		int nT,nQ,nTetr,nHex;
		int*T,*Q,*Tetr,*Hex;

		nT=nQ=nTetr=nHex=0;
		for(i=0;i<CountOfCells;i++)
			switch(CellTypes[i]){
			case RW_MESH_VTK_CELL_TYPE_TRIANGLE:nT++;break;
			case RW_MESH_VTK_CELL_TYPE_QUAD:nQ++;break;
			case RW_MESH_VTK_CELL_TYPE_TETRA:nTetr++;break;
			case RW_MESH_VTK_CELL_TYPE_HEXAHEDRON:nHex++;break;
			}

		T = nT?(int*)calloc(nT,3*sizeof(int)):NULL;
		Q = nQ?(int*)calloc(nQ,4*sizeof(int)):NULL;
		Tetr = nTetr?(int*)calloc(nTetr,4*sizeof(int)):NULL;
		Hex = nHex?(int*)calloc(nHex,8*sizeof(int)):NULL;

		nT=nQ=nTetr=nHex=0;
		for(i=0;i<CountOfCells;i++)
					switch(CellTypes[i]){
					case RW_MESH_VTK_CELL_TYPE_TRIANGLE:
						for(k=0;k<3;k++)
							T[nT*3+k] = Cells[CellOffset[i]+k];
						nT++;
						break;
					case RW_MESH_VTK_CELL_TYPE_QUAD:
						for(k=0;k<4;k++)
							Q[nQ*4+k] = Cells[CellOffset[i]+k];
						nQ++;
						break;
					case RW_MESH_VTK_CELL_TYPE_TETRA:
						for(k=0;k<4;k++)
							Tetr[nTetr*4+k] = Cells[CellOffset[i]+k];
						nTetr++;
						break;
					case RW_MESH_VTK_CELL_TYPE_HEXAHEDRON:
						for(k=0;k<8;k++)
							Hex[nHex*8+k] = Cells[CellOffset[i]+k];
						nHex++;
						break;
					}

		write_format_mesh(CountOfPoints,(REAL*)Points,MM,3,
				nT,(INT3*)T,0,nQ,(INT4*)Q,0,nTetr,(INT4*)Tetr,0,nHex,(INT8*)Hex,0,"tmp.mesh");
//		write_format_vtk(CountOfPoints,(REAL*)Points,MM,3,
//				CountOfCells,Cells,CellOffset,CellTypes,0,"tmp.vtk");




	}
	if(rn){
		rw_mesh_print_error();
	}
	printf("END");
	return rn;
}
