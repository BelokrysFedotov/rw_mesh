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

	int CountOfFaces;
	REAL3* FaceVertexes;
	REAL3* FaceNormals;


	if(argc<2){
		printf("Dont set filename\n");
		return 1;
	}

	strcpy(filename,argv[1]);

	printf("Filename = '%s'\n",filename);

	rn = read_format_stl(&CountOfFaces,&FaceVertexes,&FaceNormals,filename);

	if(rn){
		rw_mesh_print_error();
		return 1;
	}

	printf("END");
	return 0;
}
