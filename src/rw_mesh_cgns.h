/*
 * rw_mesh_cgns.h
 * Фунции работы с форматом cgns
 *
 *  Created on: 26.11.2015
 *      Author: Belokrys-Fedotov A.I.
 */
#ifndef RW_MESH_CGNS_H_

#define RW_MESH_CGNS_H_
#include "rw_mesh_header.h"

#define EXTENTION_CGNS "*\.cgns"

int write_format_cgns_structured_simplified(
	int Nx,int Ny,int Nz, REAL3*Points,
	int CountOfPointMasks, int*PointMasks,
	int CountOfPointFunctions, REAL*PointFunctions,
	int CountOfCellMasks, int*CellMasks,
	int CountOfCellFunctions, REAL*CellFunctions,
	char*filename,int flags);

int write_format_cgns_unstructured_simplified(
	int CountOfPoints, REAL3*Points,
	int CountOfPointMasks, int*PointMasks,
	int CountOfPointFunctions, REAL*PointFunctions,
	int CountOfCells,int*Cells,int*CellSizes,int*CellTypes,int*CellOffset,
	int CountOfCellMasks, int*CellMasks,
	int CountOfCellFunctions, REAL*CellFunctions,
	char*filename,int flags);


#endif /*RW_MESH_CGNS_H_*/
