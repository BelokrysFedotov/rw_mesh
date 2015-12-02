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
#include "rw_mesh_vtk.h"

#include "cgnslib.h"

#define CELL_COUNT_OF_FACE_TETRA 4
#define CELL_COUNT_OF_FACE_VOXEL 6
#define CELL_COUNT_OF_FACE_HEXAHEDRON 6
#define CELL_COUNT_OF_FACE_WEDGE 5
#define CELL_COUNT_OF_FACE_PYRAMID 5

int _cell_faces_tetra[CELL_COUNT_OF_FACE_TETRA][4] = { { 1, 0, 2, -1 }, { 0, 1, 3, -1 }, { 1, 2, 3, -1 }, { 2, 0, 3, -1 } };
int _cell_faces_voxel[CELL_COUNT_OF_FACE_VOXEL][4] = { { 0, 1, 5, 4 }, { 1, 3, 7, 5 }, { 3, 2, 6, 7 }, { 2, 0, 4, 6 }, { 1, 0, 2, 3 }, { 4, 5, 7, 6 } };
int _cell_faces_hexahedron[CELL_COUNT_OF_FACE_HEXAHEDRON][4] = { { 0, 1, 5, 4 }, { 1, 2, 6, 5 }, { 2, 3, 7, 6 }, { 3, 0, 4, 7 }, { 0, 3, 2, 1 }, { 4, 5, 6, 7 } };
int _cell_faces_wedge[CELL_COUNT_OF_FACE_WEDGE][4] = { { 0, 2, 5, 3 }, { 2, 1, 4, 5 }, { 1, 0, 3, 4 }, { 0, 1, 2, -1 }, { 3, 5, 4, -1 } };
int _cell_faces_pyramid[CELL_COUNT_OF_FACE_PYRAMID][4] = { { 0, 3, 2, 1 }, { 0, 1, 4, -1 }, { 1, 2, 4, -1 }, { 2, 3, 4, -1 }, { 3, 0, 4, -1 } };

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

	if (cg_open(filename,CG_MODE_WRITE,&index_file)!=CG_OK){
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
	cg_zone_write(index_file,index_base,"Zone",*isize,Structured,&index_zone);
	/* write grid coordinates (user must use SIDS-standard names here) */
	x = (double*)calloc(Np,sizeof(double));

	for(i=0;i<Np;i++) x[i] = Points[i][0];
	cg_coord_write(index_file,index_base,index_zone,RealDouble,"CoordinateX", x,&index_coord);

	for(i=0;i<Np;i++) x[i] = Points[i][1];
	cg_coord_write(index_file,index_base,index_zone,RealDouble,"CoordinateY", x,&index_coord);

	for(i=0;i<Np;i++) x[i] = Points[i][2];
	cg_coord_write(index_file,index_base,index_zone,RealDouble,"CoordinateZ", x,&index_coord);

	free(x);

	// добавляем информацию про размерности используемые в файле
//	cg_goto(index_file,index_base,"end");
//	cg_dataclass_write(Dimensional);
//	cg_units_write(Kilogram,Meter,Second,Kelvin,Degree);
//	cg_units_write(MassUnitsNull,LengthUnitsNull,TimeUnitsNull,TemperatureUnitsNull,Radian);

	// задаём размерность координат
//	exponents[0]=0;
//	exponents[1]=1;
//	exponents[2]=0;
//	exponents[3]=0;
//	exponents[4]=0;
//	for(i=1;i<=3;i++){
//		cg_goto(index_file,index_base,"Zone_t",1,"GridCoordinates_t",1,"DataArray_t",i,"end");
//		cg_exponents_write(RealDouble,exponents);
//	}

	// маски и функции на вершинах
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

	// маски и функции в ячейках
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
			cg_field_write(index_file, index_base, index_zone, index_solution, RealDouble, field_name, x, &index_field);
			free(x);
		}
	}

	// задание граничных условий
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


void _copy_cell_vtk2cgns(int in_cell_type,int in_cell_size,int*in_cell,cgsize_t*out_cell){
	int i;
	switch(in_cell_type){
		case RW_MESH_VTK_CELL_TYPE_TRIANGLE:
			for(i=0;i<3;i++) out_cell[i] = in_cell[i]+1; break;
		case RW_MESH_VTK_CELL_TYPE_QUAD:
			for(i=0;i<4;i++) out_cell[i] = in_cell[i]+1; break;
		case RW_MESH_VTK_CELL_TYPE_TETRA:
			for(i=0;i<4;i++) out_cell[i] = in_cell[i]+1; break;
		case RW_MESH_VTK_CELL_TYPE_VOXEL:
			out_cell[0] = in_cell[0]+1;
			out_cell[1] = in_cell[1]+1;
			out_cell[2] = in_cell[3]+1;
			out_cell[3] = in_cell[2]+1;
			out_cell[4] = in_cell[4]+1;
			out_cell[5] = in_cell[5]+1;
			out_cell[6] = in_cell[7]+1;
			out_cell[7] = in_cell[6]+1;
			break;
		case RW_MESH_VTK_CELL_TYPE_HEXAHEDRON:
			for(i=0;i<8;i++) out_cell[i] = in_cell[i]+1; break;
		case RW_MESH_VTK_CELL_TYPE_WEDGE:
			out_cell[0] = in_cell[0]+1;
			out_cell[1] = in_cell[2]+1;
			out_cell[2] = in_cell[1]+1;
			out_cell[3] = in_cell[3]+1;
			out_cell[4] = in_cell[5]+1;
			out_cell[5] = in_cell[4]+1;
			break;
		case RW_MESH_VTK_CELL_TYPE_PYRAMID:
			for(i=0;i<5;i++) out_cell[i] = in_cell[i]+1; break;
		default:
			for(i=0;i<in_cell_size;i++)
				out_cell[i] = in_cell[i]+1;
	}


}
CGNS_ENUMT(ElementType_t) _copy_celltupe_vtk2cgns(int in_cell_type,int in_cell_size){
	switch(in_cell_type){
		case RW_MESH_VTK_CELL_TYPE_NONE:		return ElementTypeNull; break;
		case RW_MESH_VTK_CELL_TYPE_TRIANGLE:	return TRI_3; break;
		case RW_MESH_VTK_CELL_TYPE_POLYGON:		return ElementTypeNull; break;
		case RW_MESH_VTK_CELL_TYPE_QUAD:		return QUAD_4; break;
		case RW_MESH_VTK_CELL_TYPE_TETRA:		return TETRA_4; break;
		case RW_MESH_VTK_CELL_TYPE_VOXEL:		return HEXA_8; break;
		case RW_MESH_VTK_CELL_TYPE_HEXAHEDRON:	return HEXA_8; break;
		case RW_MESH_VTK_CELL_TYPE_WEDGE:		return PENTA_6; break;
		case RW_MESH_VTK_CELL_TYPE_PYRAMID:		return PYRA_5; break;
		default: return ElementTypeNull;
	}
}


int write_format_cgns_unstructured_simplified_with_bc(
	int CountOfPoints, REAL3*Points,
	int CountOfPointMasks, int*PointMasks,
	int CountOfPointFunctions, REAL*PointFunctions,
	int CountOfCells,int*Cells,int*CellSizes,int*CellTypes,int*CellOffset,
	int CountOfCellMasks, int*CellMasks,
	int CountOfCellFunctions, REAL*CellFunctions,
	int BC_Counts,int*BC_TriFacesCount,int**BC_TriFaces,int*BC_QuadFacesCount,int**BC_QuadFaces,int*BC_Names,
	char*filename,int flags){

	int i,n,b;
	double*x;
	int*m;

	int icelldim, iphysdim;
	int index_file, index_base, index_zone, index_coord, index_section;
	int index_solution, index_field,index_bc;
	int nelem_start, nelem_end, BC_elem_start, BC_elem_end, section;
	CGNS_ENUMT(ElementType_t) element_type;
	cgsize_t isize[3];
	cgsize_t*ielem;
	cgsize_t*ipnts;
	char error[256];
	char section_name[256];
	char field_name[256];

	if (cg_open(filename,CG_MODE_WRITE,&index_file) != CG_OK){
		strcpy(error, cg_get_error());
		rw_mesh_set_error(0,error);
		return 1;
	}

	icelldim = 3;
	iphysdim = 3;
	cg_base_write(index_file, "Base", icelldim, iphysdim, &index_base);

	// vertex size
	isize[0] = CountOfPoints;
	// cell size
	isize[1] = CountOfCells;
	// boundary vertex size (zero if elements not sorted)
	isize[2] = 0;
	// create zone
	cg_zone_write(index_file,index_base,"Zone",isize, Unstructured,&index_zone);
	// write grid coordinates (user must use SIDS-standard names here)

	x = (double*)calloc(CountOfPoints,sizeof(double));

	for(i=0;i<CountOfPoints;i++) x[i] = Points[i][0];
	cg_coord_write(index_file,index_base,index_zone,RealDouble,"CoordinateX", x,&index_coord);

	for(i=0;i<CountOfPoints;i++) x[i] = Points[i][1];
	cg_coord_write(index_file,index_base,index_zone,RealDouble,"CoordinateY", x,&index_coord);

	for(i=0;i<CountOfPoints;i++) x[i] = Points[i][2];
	cg_coord_write(index_file,index_base,index_zone,RealDouble,"CoordinateZ", x,&index_coord);

	free(x);

	nelem_start = 0;
	if(CountOfCells){
		section = 0;

		while(nelem_start<CountOfCells){
			section++;
			nelem_end = nelem_start + 1;
			while(nelem_end < CountOfCells && CellTypes[nelem_start] == CellTypes[nelem_end] && CellSizes[nelem_start] == CellSizes[nelem_end])
				nelem_end++;

			ielem = (cgsize_t*)calloc((nelem_end-nelem_start)*CellSizes[nelem_start],sizeof(cgsize_t));
			for(n=0;n<nelem_end-nelem_start;n++){
				_copy_cell_vtk2cgns(CellTypes[nelem_start],CellSizes[nelem_start],Cells+CellOffset[nelem_start+n],ielem+n*CellSizes[nelem_start]);
			}

			sprintf(section_name,"Elem %d",section);
			element_type = _copy_celltupe_vtk2cgns(CellTypes[nelem_start],CellSizes[nelem_start]);

			cg_section_write(index_file,index_base,index_zone, section_name,element_type,nelem_start+1,nelem_end,0,ielem, &index_section);

			free(ielem);

			nelem_start = nelem_end;



		}
	}



	// маски и функции на вершинах
	if(CountOfPointMasks || CountOfPointFunctions){
		cg_sol_write(index_file,index_base,index_zone,"Vertex Solution", Vertex, &index_solution);

		if(CountOfPointMasks)for(n=0;n<CountOfPointMasks;n++){
			sprintf(field_name,"Mask %d",n+1);
			m = (int*)calloc(CountOfPoints,sizeof(int));
			for(i=0;i<CountOfPoints;i++) m[i] = PointMasks[i*CountOfPointMasks+n];
			cg_field_write(index_file, index_base, index_zone, index_solution, Integer, field_name, m, &index_field);
			free(m);
		}

		if(CountOfPointFunctions)for(n=0;n<CountOfPointFunctions;n++){
			sprintf(field_name,"Function %d",n+1);
			x = (double*)calloc(CountOfPoints,sizeof(double));
			for(i=0;i<CountOfPoints;i++) x[i] = PointFunctions[i*CountOfPointFunctions+n];
			cg_field_write(index_file, index_base, index_zone, index_solution, RealDouble, field_name, x, &index_field);
			free(x);
		}
	}

	// маски и функции в ячейках
	if(CountOfCellMasks || CountOfCellFunctions){
		cg_sol_write(index_file,index_base,index_zone,"Cell Solution", CellCenter, &index_solution);

		if(CountOfCellMasks)for(n=0;n<CountOfCellMasks;n++){
			sprintf(field_name,"Mask %d",n+1);
			m = (int*)calloc(CountOfCells,sizeof(int));
			for(i=0;i<CountOfCells;i++) m[i] = CellMasks[i*CountOfCellMasks+n];
			cg_field_write(index_file, index_base, index_zone, index_solution, Integer, field_name, m, &index_field);
			free(m);
		}

		if(CountOfCellFunctions)for(n=0;n<CountOfCellFunctions;n++){
			sprintf(field_name,"Function %d",n+1);
			x = (double*)calloc(CountOfCells,sizeof(double));
			for(i=0;i<CountOfCells;i++) x[i] = CellFunctions[i*CountOfCellFunctions+n];
			cg_field_write(index_file, index_base, index_zone, index_solution, RealDouble, field_name, x, &index_field);
			free(x);
		}
	}

	if(BC_Counts)for(b=0;b<BC_Counts;b++){
		BC_elem_start = nelem_start;

		if((BC_TriFacesCount&&BC_TriFacesCount[b]) || (BC_QuadFacesCount&&BC_QuadFacesCount[b])){
			if(BC_TriFacesCount&&BC_TriFacesCount[b]){
				nelem_end = nelem_start+BC_TriFacesCount[b];
				ielem = (cgsize_t*)calloc(BC_TriFacesCount[b]*3,sizeof(cgsize_t));
				for(i=0;i<3*BC_TriFacesCount[b];i++){
					ielem[i] = BC_TriFaces[b][i]+1;
				}
				sprintf(section_name,"Boundary %d tris",BC_Names[b]);
				cg_section_write(index_file,index_base,index_zone,section_name,TRI_3,nelem_start+1,nelem_end,0,ielem,&index_section);
				free(ielem);
				nelem_start = nelem_end;
			}
			if(BC_QuadFacesCount&&BC_QuadFacesCount[b]){
				nelem_end = nelem_start+BC_QuadFacesCount[b];
				ielem = (cgsize_t*)calloc(BC_QuadFacesCount[b]*4,sizeof(cgsize_t));
				for(i=0;i<4*BC_QuadFacesCount[b];i++){
					ielem[i] = BC_QuadFaces[b][i]+1;
				}
				sprintf(section_name,"Boundary %d quads",BC_Names[b]);
				cg_section_write(index_file,index_base,index_zone,section_name,QUAD_4,nelem_start+1,nelem_end,0,ielem,&index_section);
				free(ielem);
				nelem_start = nelem_end;
			}
		}

		BC_elem_end = nelem_end;

		ipnts = (cgsize_t*)calloc(BC_elem_end-BC_elem_start,sizeof(cgsize_t));

		for(n=0;n<BC_elem_end-BC_elem_start;n++)
			ipnts[n] = BC_elem_start+n+1;

		sprintf(section_name,"Boundary %d",BC_Names[b]);
		cg_boco_write(index_file,index_base,index_zone,section_name,BCTypeNull,PointList,BC_elem_end-BC_elem_start,ipnts,&index_bc);
		cg_goto(index_file,index_base,"Zone_t",1,"ZoneBC_t",1,"BC_t",index_bc,"end");
		cg_gridlocation_write(FaceCenter);
		free(ipnts);

	}


	/* close CGNS file */
	cg_close(index_file);
	return 0;
}

int write_format_cgns_unstructured_simplified(
	int CountOfPoints, REAL3*Points,
	int CountOfPointMasks, int*PointMasks,
	int CountOfPointFunctions, REAL*PointFunctions,
	int CountOfCells,int*Cells,int*CellSizes,int*CellTypes,int*CellOffset,
	int CountOfCellMasks, int*CellMasks,
	int CountOfCellFunctions, REAL*CellFunctions,
	char*filename,int flags){
	return write_format_cgns_unstructured_simplified_with_bc(CountOfPoints,Points,CountOfPointMasks,PointMasks,
			CountOfPointFunctions,PointFunctions,CountOfCells,Cells,CellSizes,CellTypes,CellOffset,
			CountOfCellMasks,CellMasks,CountOfCellFunctions,CellFunctions,0,0,0,0,0,0,filename,flags);
}
