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
	int index_solution,index_field,index_bc;
	int icelldim,iphysdim;
	cgsize_t isize[3][3];

	int ilo, ihi, jlo, jhi, klo, khi;
	cgsize_t ipnts[6];
	double exponents[5];

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

//	cg_goto(index_file,index_base,"end");
//	cg_dataclass_write(Dimensional);
//	cg_units_write(Kilogram,Meter,Second,Kelvin,Degree);
//	cg_units_write(MassUnitsNull,LengthUnitsNull,TimeUnitsNull,TemperatureUnitsNull,Radian);
//
//	exponents[0]=0;
//	exponents[1]=1;
//	exponents[2]=0;
//	exponents[3]=0;
//	exponents[4]=0;

//	for(i=1;i<=3;i++){
//		cg_goto(index_file,index_base,"Zone_t",1,"GridCoordinates_t",1,"DataArray_t",i,"end");
//		cg_exponents_write(RealDouble,exponents);
//	}

	if(CountOfPointMasks || CountOfPointFunctions){
		cg_sol_write(index_file,index_base,index_zone,"Vertex Solution", Vertex, &index_solution);

		if(CountOfPointMasks)for(n=0;n<CountOfPointMasks;n++){
			sprintf(field_name,"Mask_%d",n+1);
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

	ilo=1;	ihi=isize[0][0];
	jlo=1;	jhi=isize[0][1];
	klo=1;	khi=isize[0][2];

	ipnts[0] = ilo; ipnts[1] = jlo; ipnts[2] = klo; ipnts[3] = ilo; ipnts[4] = jhi; ipnts[5] = khi;
	cg_boco_write(index_file,index_base,index_zone,"Ilo", BCDataTypeNull,PointRange,2,ipnts,&index_bc);

	ipnts[0] = ihi; ipnts[1] = jlo; ipnts[2] = klo; ipnts[3] = ihi; ipnts[4] = jhi; ipnts[5] = khi;
	cg_boco_write(index_file,index_base,index_zone,"Ihi", BCDataTypeNull,PointRange,2,ipnts,&index_bc);

	ipnts[0] = ilo; ipnts[1] = jlo; ipnts[2] = klo; ipnts[3] = ihi; ipnts[4] = jlo; ipnts[5] = khi;
	cg_boco_write(index_file,index_base,index_zone,"Jlo", BCDataTypeNull,PointRange,2,ipnts,&index_bc);

	ipnts[0] = ilo; ipnts[1] = jhi; ipnts[2] = klo; ipnts[3] = ihi; ipnts[4] = jhi; ipnts[5] = khi;
	cg_boco_write(index_file,index_base,index_zone,"Jhi", BCDataTypeNull,PointRange,2,ipnts,&index_bc);

	ipnts[0] = ilo; ipnts[1] = jlo; ipnts[2] = klo; ipnts[3] = ihi; ipnts[4] = jhi; ipnts[5] = klo;
	cg_boco_write(index_file,index_base,index_zone,"Klo", BCDataTypeNull,PointRange,2,ipnts,&index_bc);

	ipnts[0] = ilo; ipnts[1] = jlo; ipnts[2] = khi; ipnts[3] = ihi; ipnts[4] = jhi; ipnts[5] = khi;
	cg_boco_write(index_file,index_base,index_zone,"Khi", BCDataTypeNull,PointRange,2,ipnts,&index_bc);

	/* close CGNS file */
	cg_close(index_file);
	return 0;
}
