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
		rn = read_format_vtk_unstructured_in_struct(this->VTK,filename);
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

	ASSERT_EQ(0,D.read_vtk(filename));
//
//	ASSERT_EQ(322494,D.VTK->CountOfPoints);
//	ASSERT_EQ(1878692,D.VTK->CountOfCells);
//
//	ASSERT_EQ(0,D.VTK->CountOfPointsData);
//	ASSERT_EQ(0,D.VTK->CountOfCellsData);
//	ASSERT_EQ(0,D.VTK->CountOfData);

//	__check_point(0,0,-1.0);
//	__check_point(1,1,-1.0);
//	__check_point(2,2,-1.0);
//	__check_point(7,1, 1.0);
//	__check_point(7,2, 1.0);

//	__check_cell( 0, 0, 0);
//	__check_cell( 0, 3, 3);
//	__check_cell( 0, 6, 6);
//	__check_cell( 0, 7, 7);

//	__check_cell_size( 0, 8);
//	__check_cell_type( 0, RW_MESH_VTK_CELL_TYPE_HEXAHEDRON);

}

int main (int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
