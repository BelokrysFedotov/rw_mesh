/*
 * write_vtk_structed_file.c
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
	int i,j,k;
	int rn;
	char filename[256];

	if(argc<2){
		printf("Dont set filename\n");
		return 1;
	}

	strcpy(filename,argv[1]);

	printf("Filename = '%s'\n",filename);

	int Nx,Ny,Nz;
	REAL3*Points;
	REAL*Function;
	int*Mask;

	Nx = 5;
	Ny = 5;
	Nz = 5;

	Points = (REAL3*)calloc(Nx*Ny*Nz,sizeof(REAL3));
	Function = (REAL*)calloc(Nx*Ny*Nz,sizeof(REAL));
	Mask = (int*)calloc(Nx*Ny*Nz,sizeof(int));
	for(i=0;i<Nx;i++)
		for(j=0;j<Ny;j++)
			for(k=0;k<Nz;k++){
				Points[i*Ny*Nz+j*Nz+k][0] = 1.e0*i/(Nx-1);
				Points[i*Ny*Nz+j*Nz+k][1] = 1.e0*j/(Ny-1);
				Points[i*Ny*Nz+j*Nz+k][2] = 1.e0*k/(Nz-1);
				Function[i*Ny*Nz+j*Nz+k] = ((1.e0*i/(Nx-1))*(1.e0*i/(Nx-1)) + (1.e0*j/(Ny-1))*(1.e0*j/(Ny-1))) + 1.e0*k/(Nz-1);
				Mask[i*Ny*Nz+j*Nz+k] = (i>j)?i:j;
			}

	rn = write_format_cgns_structured_simplified(Nx,Ny,Nz,Points,1,Mask,1,Function,0,NULL,0,NULL,filename,0);

	ffree(Points);
	ffree(Function);
	ffree(Mask);

	if(rn){
		rw_mesh_print_error();
	}
	printf("END\n");
	return rn;
}
