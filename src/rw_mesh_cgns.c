/*
 * rw_mesh_cgns.c
 * Фунции работы с форматом cgns
 *
 *  Created on: 26.11.2015
 *      Author: Belokrys-Fedotov A.I.
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "rw_mesh_string.h"
#include "rw_mesh_cgns.h"

#include "cgnslib.h"


int write_format_cgns_structured_simplified(
	int Nx,int Ny,int Nz, REAL3*Points,
	int CountOfPointMasks, int*PointMasks,
	int CountOfPointFunctions, REAL*PointFunctions,
	int CountOfCellMasks, int*CellMasks,
	int CountOfCellFunctions, REAL*CellFunctions,
	char*filename,int flags){

	int i,n,Np,Nc;
	double*x;
	int*m;
	int index_file,index_base,index_zone,index_coord;
	int index_solution,index_field;
	int icelldim,iphysdim;
	cgsize_t isize[3][3];

	char error[256];
	char field_name[256];

	if (cg_open(filename,CG_MODE_WRITE,&index_file)){
		strcpy(error, cg_get_error());
		rw_mesh_set_error(0,error);
		return 1;
	}

	icelldim=3;
	iphysdim=3;

	cg_base_write(index_file,"Base",icelldim,iphysdim,&index_base);

	Np = Nx*Ny*Nz;
	Nc = (Nx-1)*(Ny-1)*(Nz-1);

	/* vertex size */
	isize[0][0]=Nx;
	isize[0][1]=Ny;
	isize[0][2]=Nz;
	/* cell size */
	isize[1][0]=isize[0][0]-1;
	isize[1][1]=isize[0][1]-1;
	isize[1][2]=isize[0][2]-1;
	/* boundary vertex size (always zero for structured grids) */
	isize[2][0]=0;
	isize[2][1]=0;
	isize[2][2]=0;
	/* create zone */
	cg_zone_write(index_file,index_base,"Zone 1",*isize,Structured,&index_zone);
	/* write grid coordinates (user must use SIDS-standard names here) */
	x = (double*)calloc(Np,sizeof(double));

	for(i=0;i<Np;i++) x[i] = Points[i][0];
	cg_coord_write(index_file,index_base,index_zone,RealDouble,"CoordinateX", x,&index_coord);

	for(i=0;i<Np;i++) x[i] = Points[i][1];
	cg_coord_write(index_file,index_base,index_zone,RealDouble,"CoordinateY", x,&index_coord);

	for(i=0;i<Np;i++) x[i] = Points[i][2];
	cg_coord_write(index_file,index_base,index_zone,RealDouble,"CoordinateZ", x,&index_coord);

	free(x);

	if(CountOfPointMasks || CountOfPointFunctions){
		cg_sol_write(index_file,index_base,index_zone,"Vertex Solution", Vertex, &index_solution);

		if(CountOfPointMasks)for(n=0;n<CountOfPointMasks;n++){
			sprintf(field_name,"Mask %d",n+1);
			m = (int*)calloc(Np,sizeof(int));
			for(i=0;i<Np;i++) m[i] = PointMasks[i*CountOfPointMasks+n];
			cg_field_write(index_file, index_base, index_zone, index_solution, Integer, field_name, m, &index_field);
			free(m);
		}

		if(CountOfPointFunctions)for(n=0;n<CountOfPointFunctions;n++){
			sprintf(field_name,"Function %d",n+1);
			x = (double*)calloc(Np,sizeof(double));
			for(i=0;i<Np;i++) x[i] = PointFunctions[i*CountOfPointFunctions+n];
			cg_field_write(index_file, index_base, index_zone, index_solution, RealDouble, field_name, m, &index_field);
			free(x);
		}
	}

	if(CountOfCellMasks || CountOfCellFunctions){
		cg_sol_write(index_file,index_base,index_zone,"Cell Solution", CellCenter, &index_solution);

		if(CountOfCellMasks)for(n=0;n<CountOfCellMasks;n++){
			sprintf(field_name,"Mask %d",n+1);
			m = (int*)calloc(Nc,sizeof(int));
			for(i=0;i<Nc;i++) m[i] = CellMasks[i*CountOfCellMasks+n];
			cg_field_write(index_file, index_base, index_zone, index_solution, Integer, field_name, m, &index_field);
			free(m);
		}

		if(CountOfCellFunctions)for(n=0;n<CountOfCellFunctions;n++){
			sprintf(field_name,"Function %d",n+1);
			x = (double*)calloc(Nc,sizeof(double));
			for(i=0;i<Nc;i++) x[i] = CellFunctions[i*CountOfCellFunctions+n];
			cg_field_write(index_file, index_base, index_zone, index_solution, RealDouble, field_name, m, &index_field);
			free(x);
		}
	}

	/* close CGNS file */
	cg_close(index_file);
	return 0;
}
