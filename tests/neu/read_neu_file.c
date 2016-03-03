/*
 * read_neu_file.c
 *
 *  Created on: 03.02.2016
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
	struct neu_mesh_struct*Mesh;

	if(argc<2){
		printf("Dont set filename\n");
		return 1;
	}

	strcpy(filename,argv[1]);

	printf("Filename = '%s'\n",filename);

	Mesh = (struct neu_mesh_struct*)calloc(1,sizeof(struct neu_mesh_struct));

	rn = read_format_neu_struct(Mesh,filename);

	if(rn){
		rw_mesh_print_error();
		return 1;
	}

	printf("END");
	return 0;
}
