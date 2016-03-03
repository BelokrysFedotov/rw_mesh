/*
 * readline.c
 *
 *  Created on: 26.06.2013
 *      Author: Belokrys-Fedotov A.I.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../../src/rw_mesh.h"

int main (int argc, char *argv[]) {
	FILE *fd;
	int i,k;
	int rn;
	char FileIn[256];
	char FileOut[256];

	if(argc<3){
		printf("Dont set filename\n");
		return 1;
	}

	strcpy(FileIn,argv[1]);
	strcpy(FileOut,argv[2]);

	printf("FileIn  = '%s'\n",FileIn);
	printf("Fileout = '%s'\n",FileOut);

	struct RW_MESH_VTK_STRUCT*VTK;
	VTK = (struct RW_MESH_VTK_STRUCT*)calloc(1,sizeof(struct RW_MESH_VTK_STRUCT));
	rw_mesh_vtk_struct_init(VTK);

	rn = read_format_vtk_struct(VTK,FileIn,0);

	if(rn){
		rw_mesh_print_error();
		return 1;
	}

	rn = write_format_vtk_struct(VTK,FileOut,RW_MESH_VTK_BINARY);

	if(rn){
		rw_mesh_print_error();
	}

	rw_mesh_vtk_struct_free(VTK);
	free(VTK);

	printf("END\n");
	return rn;
}
