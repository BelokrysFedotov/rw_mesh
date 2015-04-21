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
	char filename[256];
	struct RW_MESH_VTK_UNSTRUCTURED_GRID_STRUCT*VTK;

	if(argc<2){
		printf("Dont set filename\n");
		return 1;
	}

	strcpy(filename,argv[1]);

	printf("Filename = '%s'\n",filename);


	VTK = (struct RW_MESH_VTK_UNSTRUCTURED_GRID_STRUCT*)calloc(1,sizeof(struct RW_MESH_VTK_UNSTRUCTURED_GRID_STRUCT));
	rw_mesh_vtk_struct_init(VTK);

	int rn;
	rn = read_format_vtk_unstructured_in_struct(VTK,filename);
	if(rn){
		rw_mesh_print_error();
	}
	printf("END");
	return rn;
}
