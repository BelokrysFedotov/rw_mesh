/*
 * rw_mesh_vtk.h
 * Фунции работы с форматом vtk
 *
 *  Created on: 20.04.2015
 *      Author: moric
 */
#ifndef RW_MESH_VTK_H_
#define RW_MESH_VTK_H_

#include "rw_mesh_header.h"

#define EXTENTION_VTK "*\.vtk"

#define RW_MESH_VTK_TYPE_NONE				0
#define RW_MESH_VTK_TYPE_STRUCTURED_POINTS	1	// "STRUCTURED_POINTS"
#define RW_MESH_VTK_TYPE_STRUCTURED_GRID	2	// "STRUCTURED_GRID"
#define RW_MESH_VTK_TYPE_RECTILINEAR_GRID	3	// "RECTILINEAR_GRID"
#define RW_MESH_VTK_TYPE_POLYGONAL_DATA		4	// "POLYDATA"
#define RW_MESH_VTK_TYPE_UNSTRUCTURED_GRID	5	// "UNSTRUCTURED_GRID"
#define RW_MESH_VTK_TYPE_FIELD				6	// "FIELD"

#define RW_MESH_VTK_DATA_OBJECT_NONE		0
#define RW_MESH_VTK_DATA_OBJECT_POINTS		1
#define RW_MESH_VTK_DATA_OBJECT_CELLS		2

#define RW_MESH_VTK_CELL_TYPE_NONE			 0
#define RW_MESH_VTK_CELL_TYPE_TRIANGLE		 5
#define RW_MESH_VTK_CELL_TYPE_POLYGON		 7
#define RW_MESH_VTK_CELL_TYPE_QUAD			 9
#define RW_MESH_VTK_CELL_TYPE_TETRA			10
#define RW_MESH_VTK_CELL_TYPE_VOXEL 		11
#define RW_MESH_VTK_CELL_TYPE_HEXAHEDRON	12
#define RW_MESH_VTK_CELL_TYPE_WEDGE			13
#define RW_MESH_VTK_CELL_TYPE_PYRAMID		14

#define RW_MESH_VTK_TYPE_BIT			 1	//bit
#define RW_MESH_VTK_TYPE_UNSIGNED_CHAR	 2	//unsigned_char
#define RW_MESH_VTK_TYPE_CHAR			 3	//char
#define RW_MESH_VTK_TYPE_UNSIGNED_SHORT	 4	//unsigned_short
#define RW_MESH_VTK_TYPE_SHORT			 5	//short
#define RW_MESH_VTK_TYPE_UNSIGNED_INT	 6	//unsigned_int
#define RW_MESH_VTK_TYPE_INT			 7	//int
#define RW_MESH_VTK_TYPE_UNSIGNED_LONG	 8	//unsigned_long
#define RW_MESH_VTK_TYPE_LONG			 9	//long
#define RW_MESH_VTK_TYPE_FLOAT			10	//float
#define RW_MESH_VTK_TYPE_DOUBLE			11	//double

#define RW_MESH_VTK_DATASET_TYPE_NONE					0
#define RW_MESH_VTK_DATASET_TYPE_SCALARS				1 // "SCALARS"
#define RW_MESH_VTK_DATASET_TYPE_COLOR_SCALARS			2 // "COLOR_SCALARS"
#define RW_MESH_VTK_DATASET_TYPE_LOOKUP_TABLE			3 // "LOOKUP_TABLE"
#define RW_MESH_VTK_DATASET_TYPE_VECTORS				4 // "VECTORS"
#define RW_MESH_VTK_DATASET_TYPE_NORMALS				5 // "NORMALS"
#define RW_MESH_VTK_DATASET_TYPE_TEXTURE_COORDINATES	6 // "TEXTURE_COORDINATES"
#define RW_MESH_VTK_DATASET_TYPE_TENSORS				7 // "TENSORS"
#define RW_MESH_VTK_DATASET_TYPE_FIELD					8 // "FIELD"

#define RW_MESH_VTK_SKIP_UNKNOWN	1
#define RW_MESH_VTK_USE_POINTS_FLOAT	2

struct RW_MESH_VTK_DATA_SCALARS_STRUCT{
	int dataType;
	int numComp;	// default 1
	char*LookupTable; // default "default"
	void*values;
};

struct RW_MESH_VTK_DATA_COLOR_SCALARS_STRUCT{
	int nValues;
	float*values;
};

struct RW_MESH_VTK_DATA_LOOKUP_TABLE_STRUCT{
	int size;
	float*values;//*rgba[4];
};

struct RW_MESH_VTK_DATA_VECTORS_STRUCT{
	int dataType;
	void*values;//vector[3];
};

struct RW_MESH_VTK_DATA_NORMALS_STRUCT{
	int dataType;
	void*values;//normals[3];
};

struct RW_MESH_VTK_DATA_TEXTURE_COORDINATES_STRUCT{
	int dataType;
	int dim;
	void*values;//normals[3];
};

struct RW_MESH_VTK_DATA_TENSORS_STRUCT{
	int dataType;
	void*values;//normals[3];
};

struct RW_MESH_VTK_DATA_FIELD_ARRAY_STRUCT{
	char*name;
	int numComponents;
	int numTuples;
	int dataType;
	void*values;
};

struct RW_MESH_VTK_DATA_FIELD_STRUCT{
	int numArrays;
	struct RW_MESH_VTK_DATA_FIELD_ARRAY_STRUCT*Arrays;//normals[3];
};


struct RW_MESH_VTK_DATASET_STRUCT{
	int DataType;
	char*DataName;
	int Counts;
	void*Data;
};


//TODO Structured Points
struct RW_MESH_VTK_STRUCTURED_POINTS_STRUCT{
	int a;
};

struct RW_MESH_VTK_STRUCTURED_GRID_STRUCT{
	int Nx,Ny,Nz;
	REAL*Points;
};

//TODO Rectilinear Grid
struct RW_MESH_VTK_RECTILINER_GRID_STRUCT{
	int a;
};

//TODO Polygonal Data
struct RW_MESH_VTK_POLYGONAL_DATA_STRUCT{
	int a;
};

struct RW_MESH_VTK_UNSTRUCTURED_GRID_STRUCT{
	int CountOfPoints;
	REAL*Points;

	int CountOfCells;
	int*Cells;
	int*CellOffset;
	int*CellSizes;
	int*CellTypes;

};

struct RW_MESH_VTK_STRUCT{
	int type;
	void*Grid;

	int CountOfPointsData;
	struct RW_MESH_VTK_DATASET_STRUCT*PointsData;

	int CountOfCellsData;
	struct RW_MESH_VTK_DATASET_STRUCT*CellsData;

	int CountOfData;
	struct RW_MESH_VTK_DATASET_STRUCT*Data;

};

int rw_mesh_vtk_struct_init(struct RW_MESH_VTK_STRUCT*VTK);
int rw_mesh_vtk_struct_clean(struct RW_MESH_VTK_STRUCT*VTK);
int rw_mesh_vtk_struct_free(struct RW_MESH_VTK_STRUCT*VTK);

int rw_mesh_vtk_structured_points_struct_init(struct RW_MESH_VTK_STRUCTURED_POINTS_STRUCT*VTK);
int rw_mesh_vtk_structured_points_struct_clean(struct RW_MESH_VTK_STRUCTURED_POINTS_STRUCT*VTK);
int rw_mesh_vtk_structured_points_struct_free(struct RW_MESH_VTK_STRUCTURED_POINTS_STRUCT*VTK);

int rw_mesh_vtk_structured_grid_struct_init(struct RW_MESH_VTK_STRUCTURED_GRID_STRUCT*VTK);
int rw_mesh_vtk_structured_grid_struct_clean(struct RW_MESH_VTK_STRUCTURED_GRID_STRUCT*VTK);
int rw_mesh_vtk_structured_grid_struct_free(struct RW_MESH_VTK_STRUCTURED_GRID_STRUCT*VTK);

int rw_mesh_vtk_rectiliner_grid_struct_init(struct RW_MESH_VTK_RECTILINER_GRID_STRUCT*VTK);
int rw_mesh_vtk_rectiliner_grid_struct_clean(struct RW_MESH_VTK_RECTILINER_GRID_STRUCT*VTK);
int rw_mesh_vtk_rectiliner_grid_struct_free(struct RW_MESH_VTK_RECTILINER_GRID_STRUCT*VTK);

int rw_mesh_vtk_polygonal_data_struct_init(struct RW_MESH_VTK_POLYGONAL_DATA_STRUCT*VTK);
int rw_mesh_vtk_polygonal_data_struct_clean(struct RW_MESH_VTK_POLYGONAL_DATA_STRUCT*VTK);
int rw_mesh_vtk_polygonal_data_struct_free(struct RW_MESH_VTK_POLYGONAL_DATA_STRUCT*VTK);

int rw_mesh_vtk_unstructured_grid_struct_init(struct RW_MESH_VTK_UNSTRUCTURED_GRID_STRUCT*VTK);
int rw_mesh_vtk_unstructured_grid_struct_clean(struct RW_MESH_VTK_UNSTRUCTURED_GRID_STRUCT*VTK);
int rw_mesh_vtk_unstructured_grid_struct_free(struct RW_MESH_VTK_UNSTRUCTURED_GRID_STRUCT*VTK);

int rw_mesh_vtk_dataset_struct_init(struct RW_MESH_VTK_DATASET_STRUCT*DATASET);
int rw_mesh_vtk_dataset_struct_clean(struct RW_MESH_VTK_DATASET_STRUCT*DATASET);
int rw_mesh_vtk_dataset_struct_free(struct RW_MESH_VTK_DATASET_STRUCT*DATASET);

void rw_mesh_vtk_data_field_add_array(struct RW_MESH_VTK_DATA_FIELD_STRUCT*Field,char*name,int Count,int numComp,int type,void*Values);

int read_format_vtk_struct(struct RW_MESH_VTK_STRUCT*VTK,char filename[256],int flags);

int read_format_vtk_unstructured_simplified(
		int*CountOfPoints, REAL3**Points,
		int*CountOfPointMasks, int**PointMasks,
		int*CountOfPointFunctions, REAL**PointFunctions,

		int*CountOfCells,int**Cells,int**CellSizes,int**CellTypes,int**CellOffset,
		int*CountOfCellMasks, int**CellMasks,
		int*CountOfCellFunctions, REAL**CellFunctions,

		char*filename,int flags);

int write_format_vtk_struct(struct RW_MESH_VTK_STRUCT*VTK,char filename[256],int flags);

struct RW_MESH_VTK_STRUCT* rw_mesh_vtk_create_structured_simplified(int Nx,int Ny,int Nz, REAL3*Points);
struct RW_MESH_VTK_STRUCT* rw_mesh_vtk_create_unstructured_simplified(int CountOfPoints, REAL3*Points,int CountOfCells,int*Cells,int*CellSizes,int*CellTypes,int*CellOffset);

int rw_mesh_vtk_add_scalars(struct RW_MESH_VTK_STRUCT*VTK,int data_object,int Count,int data_type,void*values,char*name);

int rw_mesh_vtk_add_mask(struct RW_MESH_VTK_STRUCT*VTK,int data_object,int Count,int*Mask,char*name);
int rw_mesh_vtk_add_mask_points(struct RW_MESH_VTK_STRUCT*VTK,int Count,int*Mask,char*name);
int rw_mesh_vtk_add_mask_cells(struct RW_MESH_VTK_STRUCT*VTK,int Count,int*Mask,char*name);

int rw_mesh_vtk_add_function(struct RW_MESH_VTK_STRUCT*VTK,int data_object,int Count,REAL*Function,char*name);
int rw_mesh_vtk_add_function_points(struct RW_MESH_VTK_STRUCT*VTK,int Count,REAL*Function,char*name);
int rw_mesh_vtk_add_function_cells(struct RW_MESH_VTK_STRUCT*VTK,int Count,REAL*Function,char*name);

int write_format_vtk_structured_simplified(
	int Nx,int Ny,int Nz, REAL3*Points,
	int CountOfPointMasks, int*PointMasks,
	int CountOfPointFunctions, REAL*PointFunctions,

	int CountOfCellMasks, int*CellMasks,
	int CountOfCellFunctions, REAL*CellFunctions,

	char*filename,int flags);


int write_format_vtk_unstructured_simplified(
	int CountOfPoints, REAL3*Points,
	int CountOfPointMasks, int*PointMasks,
	int CountOfPointFunctions, REAL*PointFunctions,

	int CountOfCells,int*Cells,int*CellSizes,int*CellTypes,int*CellOffset,
	int CountOfCellMasks, int*CellMasks,
	int CountOfCellFunctions, REAL*CellFunctions,

	char*filename,int flags);

/*int write_format_vtk(int nv, REAL* v, int* mskv, int dim,
					 int ncells, int* cells, int* cell_sizes,
					 int* cell_types, int *cell_mask, char filename[256]);
int read_format_vtk(int *out_nv, REAL3** out_v, int **out_mskv,
					int *out_ncells, int** out_cells, int** out_cell_sizes,
					int **out_cell_types, int **out_cell_mask, char filename[256]);
int write_format_vtk_points_function(int nv, REAL* v, int *mskv,REAL*fv, int dim,
					 int ncells, int* cells, int* cell_sizes,
					 int* cell_types, int *cell_mask, char filename[256]);*/
/*int write_format_vtk(int nv, REAL* v, int *mskv, int dim,
					 int ncells, int* cells, int* cell_sizes,
					 int* cell_types, int *cell_mask, char filename[256]);*/

#endif /*RW_MESH_VTK_H_*/
