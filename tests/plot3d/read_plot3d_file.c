/*
 * read_plot3d_file.c
 *
 *  Created on: 08.08.2015
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
	char filename[256];
	int CountOfBlocks;
	int dimention;
	struct plot3d_struct*Grids;

	if(argc<2){
		printf("Dont set filename\n");
		return 1;
	}

	strcpy(filename,argv[1]);

	printf("Filename = '%s'\n",filename);

	if(read_format_pxdfmt(&CountOfBlocks,&dimention,&Grids,filename)){
		rw_mesh_print_error();
		return 1;
	}

	printf("END\n");
	return 0;
}
