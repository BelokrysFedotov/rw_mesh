/*
 * format_off.c
 *
 *  Created on: 15.12.2011
 *      Author: moric
 */

#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>
#include "header.h"
#include "format_off.h"

#define __save_locale char*oldlocale = setlocale(LC_NUMERIC, "C")
#define __load_locale setlocale(LC_NUMERIC, oldlocale)


int read_off3(char*filename,int*CountOfPoints, int*CountOfFaces,REAL3**Points, INT3**Faces)
{
	__save_locale;

	FILE*fd;
	int i,j,k,a;
	REAL x,y,z;
	char format[128];

	fd=fopen(filename,"r");if(fd==NULL)return 1;

	if(fscanf(fd,"%s\n",format)<1)return 1;
	if(strcmp(format,"OFF"))return 1;

	if(fscanf(fd,"%d %d %d\n",CountOfPoints,CountOfFaces,&i)<3)return 1;
	if(!CountOfPoints) return 1;
	if(!CountOfFaces) return 1;

	*Points=(REAL3*)malloc(*CountOfPoints*sizeof(REAL3));
	if(!Points) return 1;

	for(i=0;i<*CountOfPoints;i++){
		k=fscanf(fd,"%lf %lf %lf\n",&x,&y,&z);
		(*Points)[i][0]=x;
		(*Points)[i][1]=y;
		(*Points)[i][2]=z;
	}

	*Faces=(INT3*)malloc(*CountOfFaces*sizeof(INT3));
	if(!Faces)return 1;

	for(i=0;i<*CountOfFaces;i++){
		fscanf(fd,"%d",&j);
		if(j!=3)return 1;

		for(k=0;k<j;k++){
			fscanf(fd,"%d",&a);
			(*Faces)[i][k]=a;
		}
	}
	fclose(fd);
	__load_locale;
	return 0;
}


int write_off(char*filename,int CountOfPoints, int CountOfFaces,REAL3*Points, INT3*Faces){
	__save_locale;

	FILE*fd;
	int i;

	fd=fopen(filename,"w");if(fd==NULL)return 1;

	fprintf(fd,"OFF\n");
	fprintf(fd,"%d %d 0\n",CountOfPoints,CountOfFaces);

	for(i=0;i<CountOfPoints;i++){
		fprintf(fd,"%lf %lf %lf\n",Points[i][0],Points[i][1],Points[i][2]);
	}

	for(i=0;i<CountOfFaces;i++){
		fprintf(fd,"3 %d %d %d\n",Faces[i][0],Faces[i][1],Faces[i][2]);
	}

	fclose(fd);
	__load_locale;
	return 0;
}
