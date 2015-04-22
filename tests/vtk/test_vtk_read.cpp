/*
 * test_off_read.cpp
 *
 *  Created on: 26.06.2013
 *      Author: moric
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gtest/gtest.h>
extern "C" {
	#include "../../src/rw_mesh.h"
}

class VtkClass{

public:

	struct RW_MESH_VTK_UNSTRUCTURED_GRID_STRUCT*VTK;

	VtkClass(){this->VTK=NULL;this->init();}

	~VtkClass(){this->clean();}

	void init(){

		this->VTK = (struct RW_MESH_VTK_UNSTRUCTURED_GRID_STRUCT*)calloc(1,sizeof(struct RW_MESH_VTK_UNSTRUCTURED_GRID_STRUCT));
		rw_mesh_vtk_struct_init(this->VTK);

	}

	void clean(){
		rw_mesh_vtk_struct_init(this->VTK);
	}

	int read_vtk(char*filename){
		int rn;
		rn = read_format_vtk_unstructured_in_struct(this->VTK,filename,0);
		if(rn){
			rw_mesh_print_error();
		}
		return rn;
	}

	int write_vtk(char*filename){
/*		return write_format_off(
				this->CountOfPoints,
				this->Points,
				this->PointsDimention,
				this->PointsFunction,
				this->PointsNormal,
				this->PointsMaterial,
				this->PointsColor,
				this->PointsTextureCoords,

				this->CountOfFaces,
				this->Faces,
				this->FacesSizes,

				this->FacesMaterial,
				this->FacesColor,
				filename
				);*/
	}

};

#ifndef WORK_DIRECTORY
#define WORK_DIRECTORY "./"
#endif

#define FILES_DIRECTORY WORK_DIRECTORY "files/vtk/"

#define __check_point(p,k,v) 	EXPECT_NEAR(v,D.VTK->Points[p*3+k],1.e-6);
#define __check_cell(c,k,v) 	EXPECT_EQ(v,D.VTK->Cells[D.VTK->CellOffset[c]+k])
#define __check_cell_size(c,v) 	EXPECT_EQ(v,D.VTK->CellSizes[c])
#define __check_cell_type(c,v) 	EXPECT_EQ(v,D.VTK->CellTypes[c])


TEST(Test, cube_vtk){
	char filename[256] = FILES_DIRECTORY "cube.vtk";
	
	VtkClass D;

	ASSERT_EQ(0,D.read_vtk(filename));

	ASSERT_EQ(8,D.VTK->CountOfPoints);
	ASSERT_EQ(1,D.VTK->CountOfCells);

	ASSERT_EQ(0,D.VTK->CountOfPointsData);
	ASSERT_EQ(0,D.VTK->CountOfCellsData);
	ASSERT_EQ(0,D.VTK->CountOfData);

	__check_point(0,0,-1.0);
	__check_point(1,1,-1.0);
	__check_point(2,2,-1.0);
	__check_point(7,1, 1.0);
	__check_point(7,2, 1.0);

	__check_cell( 0, 0, 0);
	__check_cell( 0, 3, 3);
	__check_cell( 0, 6, 6);
	__check_cell( 0, 7, 7);

	__check_cell_size( 0, 8);
	__check_cell_type( 0, RW_MESH_VTK_CELL_TYPE_HEXAHEDRON);

}

TEST(Test, visit_rotor_small_ascii_vtk){
	char filename[256] = FILES_DIRECTORY "visit_rotor_small_ascii.vtk";

	VtkClass D;
	RW_MESH_VTK_DATA_FIELD_STRUCT*F;

	ASSERT_EQ(0,D.read_vtk(filename));

	ASSERT_EQ(322494,D.VTK->CountOfPoints);
	ASSERT_EQ(1878692,D.VTK->CountOfCells);

	ASSERT_EQ(1,D.VTK->CountOfPointsData);
	ASSERT_EQ(0,D.VTK->CountOfCellsData);
	ASSERT_EQ(1,D.VTK->CountOfData);


	ASSERT_EQ(RW_MESH_VTK_DATASET_TYPE_FIELD,D.VTK->PointsData[0].DataType);
	ASSERT_EQ(1,D.VTK->PointsData[0].Counts);
	ASSERT_STRCASEEQ("FieldData",D.VTK->PointsData[0].DataName);
	ASSERT_NE((void*)0,D.VTK->PointsData[0].Data);

	F = (RW_MESH_VTK_DATA_FIELD_STRUCT*)D.VTK->PointsData[0].Data;


	ASSERT_EQ(4,F->numArrays);

	ASSERT_EQ(RW_MESH_VTK_TYPE_FLOAT,F->Arrays[0].dataType);
	ASSERT_STRCASEEQ("X",F->Arrays[0].name);
	ASSERT_EQ(1,F->Arrays[0].numComponents);
	ASSERT_EQ(322494,F->Arrays[0].numTuples);

	EXPECT_NEAR(116.065,((float*)F->Arrays[0].values)[0],1.e-5);
	EXPECT_NEAR(129.861,((float*)F->Arrays[0].values)[10],1.e-5);

	ASSERT_EQ(RW_MESH_VTK_TYPE_FLOAT,F->Arrays[3].dataType);
	ASSERT_STRCASEEQ("fluid_id",F->Arrays[3].name);
	ASSERT_EQ(1,F->Arrays[3].numComponents);
	ASSERT_EQ(322494,F->Arrays[3].numTuples);

	EXPECT_NEAR(4,((float*)F->Arrays[3].values)[2],1.e-5);
	EXPECT_NEAR(9,((float*)F->Arrays[3].values)[72],1.e-5);

}

TEST(Test, visit_rotor_small_ascii_vtk_s){
	char filename[256] = FILES_DIRECTORY "visit_rotor_small_ascii.vtk";

	int CountOfPoints;
	REAL3*Points;

	int CountOfPointMasks;
	int*PointMasks;
	int CountOfPointFunctions;
	REAL*PointFunctions;

	int CountOfCells;
	int*Cells;
	int*CellSizes;
	int*CellTypes;
	int*CellOffset;

	int CountOfCellMasks;
	int *CellMasks;
	int CountOfCellFunctions;
	REAL*CellFunctions;

	ASSERT_EQ(0,read_format_vtk_unstructured_simplified(
			&CountOfPoints,&Points,
			&CountOfPointMasks,&PointMasks,
			&CountOfPointFunctions,&PointFunctions,
			&CountOfCells,&Cells,
			&CellSizes,&CellTypes,&CellOffset,
			&CountOfCellMasks,&CellMasks,
			&CountOfCellFunctions,&CellFunctions,
			filename,0));


	ASSERT_EQ(322494,CountOfPoints);
	ASSERT_EQ(1878692,CountOfCells);

	ASSERT_EQ(0,CountOfPointMasks);
	ASSERT_EQ(4,CountOfPointFunctions);
	ASSERT_EQ(0,CountOfCellMasks);
	ASSERT_EQ(0,CountOfCellFunctions);

}

TEST(Test, tsagi_c_vtk){
	char filename[256] = FILES_DIRECTORY "tsagi_c.vtk";

	VtkClass D;

	ASSERT_EQ(0,D.read_vtk(filename));

}

int main (int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
