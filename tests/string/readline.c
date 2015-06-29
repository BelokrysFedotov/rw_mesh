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
	int numberline;
	char line[256];
	char checkline[256];

	if(argc<3){
		printf("Dont set filename and checkline\n");
		return 1;
	}

	strcpy(filename,argv[1]);
	strcpy(checkline,argv[2]);


	printf("Filename = '%s'\n",filename);
	printf("Checkline = '%s'\n",checkline);

	numberline = 0;
	if(argc>3)
		numberline = atoi(argv[3]);
	printf("numberline = %d\n",numberline);

	fd = fopen (filename, "r");
	if(!fd){
		printf("Dont opened file\n");
		return 1;
	}

	for(i=0;i<numberline;i++){
		rw_mesh_read_line(fd,line);
		if(feof(fd)){
			fclose(fd);
			printf("File endedon line %d\n",i);
			return 1;
		}
	}

	rw_mesh_read_line(fd,line);
	fclose(fd);


	printf("Readedline = '%s'\n",line);
	printf("Checkline  = '%s'\n",checkline);

	if(strcmp(line,checkline)==0){
		return 0;
	}


	return 1;
}
