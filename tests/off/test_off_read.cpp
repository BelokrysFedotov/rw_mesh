/*
 * test_off_read.cpp
 *
 *  Created on: 26.06.2013
 *      Author: Belokrys-Fedotov A.I.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gtest/gtest.h>
extern "C" {
	#include "../../src/rw_mesh.h"
}

class OffClass{

public:
	int CountOfPoints;
	REAL*Points;
	int PointsDimention;
	int EnablePointsFunction;
	REAL*PointsFunction;
	int EnablePointsNormal;
	REAL*PointsNormal;
	int EnablePointsMaterial;
	int*PointsMaterial;
	int EnablePointsColor;
	REAL4*PointsColor;
	int EnablePointsTextureCoords;
	REAL2*PointsTextureCoords;

	int CountOfFaces;
	int*Faces;
	int*FacesSizes;
	int EnableFacesMaterial;
	int*FacesMaterial;
	int EnableFacesColor;
	REAL4*FacesColor;

	OffClass(){this->init();}

	~OffClass(){this->clean();}

	void init(){
		this->CountOfPoints = 0;
		this->Points = NULL;
		this->PointsDimention = 0;
		this->EnablePointsFunction = 0;
		this->PointsFunction = NULL;
		this->EnablePointsNormal = 0;
		this->PointsNormal = NULL;
		this->EnablePointsMaterial = 0;
		this->PointsMaterial = NULL;
		this->EnablePointsColor = 0;
		this->PointsColor = NULL;
		this->EnablePointsColor = 0;
		this->PointsTextureCoords = NULL;

		this->CountOfFaces = 0;
		this->Faces = NULL;
		this->FacesSizes = NULL;
		this->EnableFacesColor = 0;
		this->FacesColor = NULL;
		this->EnableFacesMaterial = 0;
		this->FacesMaterial = NULL;
	}

	void clean(){
		ffree(this->Points);
		ffree(this->PointsFunction);
		ffree(this->PointsNormal);
		ffree(this->PointsMaterial);
		ffree(this->PointsColor);
		ffree(this->PointsTextureCoords);

		ffree(this->Faces);
		ffree(this->FacesSizes);
		ffree(this->FacesColor);
		ffree(this->FacesMaterial);

		this->init();

	}

	int read_off(char*filename){
		return read_format_off(
				&(this->CountOfPoints),
				&(this->Points),
				&(this->PointsDimention),
				&(this->EnablePointsFunction),
				&(this->PointsFunction),
				&(this->EnablePointsNormal),
				&(this->PointsNormal),
				&(this->EnablePointsMaterial),
				&(this->PointsMaterial),
				&(this->EnablePointsColor),
				&(this->PointsColor),
				&(this->EnablePointsTextureCoords),
				&(this->PointsTextureCoords),
				&(this->CountOfFaces),
				&(this->Faces),
				&(this->FacesSizes),
				&(this->EnableFacesMaterial),
				&(this->FacesMaterial),
				&(this->EnableFacesColor),
				&(this->FacesColor),
				filename
				);
	}

	int write_off(char*filename){
		return write_format_off(
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
				);
	}

};

#ifndef WORK_DIRECTORY
#define WORK_DIRECTORY "./"
#endif

#define FILES_DIRECTORY WORK_DIRECTORY "files/off/"

//#define __check_point(p,k,v) 	EXPECT_DOUBLE_EQ(D.Points[p*D.PointsDimention+k],v);
#define __check_point(p,k,v) 	EXPECT_NEAR(v,D.Points[p*D.PointsDimention+k],1.e-6);

#define __check_point3(p,x,y,z) 	__check_point(p,0,x); \
	__check_point(p,1,y); \
	__check_point(p,2,z);


#define __check_face3(f,a,b,c) 	ASSERT_EQ(3,D.FacesSizes[f+1]-D.FacesSizes[f]); \
	EXPECT_EQ(a,D.Faces[D.FacesSizes[f]+0]); \
	EXPECT_EQ(b,D.Faces[D.FacesSizes[f]+1]); \
	EXPECT_EQ(c,D.Faces[D.FacesSizes[f]+2]);

#define __check_face4(f,a,b,c,d) 	ASSERT_EQ(4,D.FacesSizes[f+1]-D.FacesSizes[f]); \
	EXPECT_EQ(a,D.Faces[D.FacesSizes[f]+0]); \
	EXPECT_EQ(b,D.Faces[D.FacesSizes[f]+1]); \
	EXPECT_EQ(c,D.Faces[D.FacesSizes[f]+2]); \
	EXPECT_EQ(d,D.Faces[D.FacesSizes[f]+3]);

TEST(Test, cube3_off){
	char filename[256] = FILES_DIRECTORY "cube3.off";
	
	OffClass D;

	ASSERT_EQ(0,D.read_off(filename));

	ASSERT_EQ(0,D.EnablePointsFunction);
	ASSERT_EQ(0,D.EnablePointsNormal);
	ASSERT_EQ(0,D.EnablePointsColor);
	ASSERT_EQ(0,D.EnablePointsMaterial);
	ASSERT_EQ(0,D.EnablePointsTextureCoords);

	ASSERT_EQ(0,D.EnableFacesColor);
	ASSERT_EQ(0,D.EnableFacesMaterial);

	ASSERT_EQ(8,D.CountOfPoints);
	ASSERT_EQ(12,D.CountOfFaces);

	ASSERT_EQ(3,D.PointsDimention);

	__check_point(0,0,-1.0);
	__check_point(1,1,-1.0);
	__check_point(2,2,-1.0);
	__check_point(7,0, 1.0);
	__check_point(7,2,-1.0);

	__check_face3( 0,0,1,2);
	__check_face3( 2,4,0,3);
	__check_face3(11,6,4,5);

}

TEST(Test, octa_off){
	char filename[256] = FILES_DIRECTORY "octa.off";

	OffClass D;
	ASSERT_EQ(0,D.read_off(filename));

	ASSERT_EQ(0,D.EnablePointsFunction);
	ASSERT_EQ(0,D.EnablePointsNormal);
	ASSERT_EQ(0,D.EnablePointsColor);
	ASSERT_EQ(0,D.EnablePointsMaterial);
	ASSERT_EQ(0,D.EnablePointsTextureCoords);

	ASSERT_EQ(0,D.EnableFacesColor);
	ASSERT_EQ(0,D.EnableFacesMaterial);

	ASSERT_EQ(6,D.CountOfPoints);
	ASSERT_EQ(7,D.CountOfFaces);

	ASSERT_EQ(3,D.PointsDimention);

	__check_point3(0,-0.408248290464,0.707106781187,0.57735026919);
	__check_point3(1,0.816496580928,0.0,0.57735026919);
	__check_point3(2,0.408248290464,0.707106781187,-0.57735026919);
	__check_point3(3,-0.816496580928,0.0,-0.57735026919);
	__check_point3(4,0.408248290464,-0.707106781187,-0.57735026919);
	__check_point3(5,-0.408248290464,-0.707106781187,0.57735026919);

	__check_face3( 0,0,1,2);
	__check_face3( 2,3,2,4);
	__check_face3( 6,5,4,1);

}

TEST(Test, octa_coff){
	char filename[256] = FILES_DIRECTORY "octa_coff.off";

	OffClass D;

	ASSERT_EQ(0,D.read_off(filename));

	ASSERT_EQ(0,D.EnablePointsFunction);
	ASSERT_EQ(0,D.EnablePointsNormal);
	ASSERT_EQ(0,D.EnablePointsColor);
	ASSERT_EQ(1,D.EnablePointsMaterial);
	ASSERT_EQ(0,D.EnablePointsTextureCoords);

	ASSERT_EQ(0,D.EnableFacesColor);
	ASSERT_EQ(0,D.EnableFacesMaterial);

	ASSERT_EQ(6,D.CountOfPoints);
	ASSERT_EQ(7,D.CountOfFaces);

	ASSERT_EQ(3,D.PointsDimention);

	__check_point3(0,-0.408248290464,0.707106781187,0.57735026919);
	__check_point3(1,0.816496580928,0.0,0.57735026919);
	__check_point3(2,0.408248290464,0.707106781187,-0.57735026919);
	__check_point3(3,-0.816496580928,0.0,-0.57735026919);
	__check_point3(4,0.408248290464,-0.707106781187,-0.57735026919);
	__check_point3(5,-0.408248290464,-0.707106781187,0.57735026919);

	__check_face3( 0,0,1,2);
	__check_face3( 2,3,2,4);
	__check_face3( 6,5,4,1);

	ASSERT_EQ(10,D.PointsMaterial[0]);
	ASSERT_EQ(19,D.PointsMaterial[1]);
	ASSERT_EQ(29,D.PointsMaterial[2]);
	ASSERT_EQ(1,D.PointsMaterial[3]);
	ASSERT_EQ(57,D.PointsMaterial[4]);
	ASSERT_EQ(55,D.PointsMaterial[5]);

}

TEST(Test, cube4_off){
	char filename[256] = FILES_DIRECTORY "cube4.off";

	OffClass D;

	ASSERT_EQ(0,D.read_off(filename));

	ASSERT_EQ(0,D.EnablePointsFunction);
	ASSERT_EQ(0,D.EnablePointsNormal);
	ASSERT_EQ(0,D.EnablePointsColor);
	ASSERT_EQ(0,D.EnablePointsMaterial);
	ASSERT_EQ(0,D.EnablePointsTextureCoords);

	ASSERT_EQ(0,D.EnableFacesColor);
	ASSERT_EQ(0,D.EnableFacesMaterial);

	ASSERT_EQ(8,D.CountOfPoints);
	ASSERT_EQ(6,D.CountOfFaces);

	ASSERT_EQ(3,D.PointsDimention);

	__check_point3(0,-1.0,-1.0,1.0);
	__check_point3(2, 1.0,-1.0,-1.0);
	__check_point3(7, 1.0, 1.0,-1.0);

	__check_face4( 0,0,1,2,3);
	__check_face4( 1,4,0,3,5);
	__check_face4( 2,4,6,1,0);
	__check_face4( 3,1,6,7,2);
	__check_face4( 4,3,2,7,5);
	__check_face4( 5,5,7,6,4);

}

TEST(Test, cube4_4off){
	char filename[256] = FILES_DIRECTORY "cube4_4off.off";

	OffClass D;

	ASSERT_EQ(0,D.read_off(filename));

	ASSERT_EQ(1,D.EnablePointsFunction);
	ASSERT_EQ(0,D.EnablePointsNormal);
	ASSERT_EQ(0,D.EnablePointsColor);
	ASSERT_EQ(0,D.EnablePointsMaterial);
	ASSERT_EQ(0,D.EnablePointsTextureCoords);

	ASSERT_EQ(0,D.EnableFacesColor);
	ASSERT_EQ(0,D.EnableFacesMaterial);

	ASSERT_EQ(8,D.CountOfPoints);
	ASSERT_EQ(6,D.CountOfFaces);

	ASSERT_EQ(3,D.PointsDimention);

	__check_point3(0,-1.0,-1.0,1.0);
	__check_point3(2, 1.0,-1.0,-1.0);
	__check_point3(7, 1.0, 1.0,-1.0);

	__check_face4( 0,0,1,2,3);
	__check_face4( 1,4,0,3,5);
	__check_face4( 2,4,6,1,0);
	__check_face4( 3,1,6,7,2);
	__check_face4( 4,3,2,7,5);
	__check_face4( 5,5,7,6,4);

	ASSERT_EQ(10,		D.PointsFunction[0]);
	ASSERT_EQ(21.25,	D.PointsFunction[1]);
	ASSERT_EQ(10,		D.PointsFunction[2]);
	ASSERT_EQ(46.25,	D.PointsFunction[3]);
	ASSERT_EQ(60.75,	D.PointsFunction[4]);
	ASSERT_EQ(100,		D.PointsFunction[5]);
	ASSERT_EQ(52.25,	D.PointsFunction[6]);
	ASSERT_EQ(75,		D.PointsFunction[7]);

}

TEST(Test, cube4_coff1){
	char filename[256] = FILES_DIRECTORY "cube4_coff.off";

	OffClass D;

	ASSERT_EQ(0,D.read_off(filename));

	ASSERT_EQ(0,D.EnablePointsFunction);
	ASSERT_EQ(0,D.EnablePointsNormal);
	ASSERT_EQ(0,D.EnablePointsColor);
	ASSERT_EQ(1,D.EnablePointsMaterial);
	ASSERT_EQ(0,D.EnablePointsTextureCoords);

	ASSERT_EQ(0,D.EnableFacesColor);
	ASSERT_EQ(0,D.EnableFacesMaterial);

	ASSERT_EQ(8,D.CountOfPoints);
	ASSERT_EQ(6,D.CountOfFaces);

	ASSERT_EQ(3,D.PointsDimention);

	__check_point3(0,-1.0,-1.0,1.0);
	__check_point3(2, 1.0,-1.0,-1.0);
	__check_point3(7, 1.0, 1.0,-1.0);

	__check_face4( 0,0,1,2,3);
	__check_face4( 1,4,0,3,5);
	__check_face4( 2,4,6,1,0);
	__check_face4( 3,1,6,7,2);
	__check_face4( 4,3,2,7,5);
	__check_face4( 5,5,7,6,4);

	ASSERT_EQ( 1, D.PointsMaterial[0]);
	ASSERT_EQ( 2, D.PointsMaterial[1]);
	ASSERT_EQ( 3, D.PointsMaterial[2]);
	ASSERT_EQ( 4, D.PointsMaterial[3]);
	ASSERT_EQ( 5, D.PointsMaterial[4]);
	ASSERT_EQ( 6, D.PointsMaterial[5]);
	ASSERT_EQ( 7, D.PointsMaterial[6]);
	ASSERT_EQ(-7, D.PointsMaterial[7]);

}

TEST(Test, cube4_coff2){
	char filename[256] = FILES_DIRECTORY "cube4_coff2.off";

	OffClass D;

	ASSERT_EQ(0,D.read_off(filename));

	ASSERT_EQ(0,D.EnablePointsFunction);
	ASSERT_EQ(0,D.EnablePointsNormal);
	ASSERT_EQ(1,D.EnablePointsColor);
	ASSERT_EQ(0,D.EnablePointsMaterial);
	ASSERT_EQ(0,D.EnablePointsTextureCoords);

	ASSERT_EQ(0,D.EnableFacesColor);
	ASSERT_EQ(0,D.EnableFacesMaterial);

	ASSERT_EQ(8,D.CountOfPoints);
	ASSERT_EQ(6,D.CountOfFaces);

	ASSERT_EQ(3,D.PointsDimention);

	__check_point3(0,-1.0,-1.0,1.0);
	__check_point3(2, 1.0,-1.0,-1.0);
	__check_point3(7, 1.0, 1.0,-1.0);

	__check_face4( 0,0,1,2,3);
	__check_face4( 1,4,0,3,5);
	__check_face4( 2,4,6,1,0);
	__check_face4( 3,1,6,7,2);
	__check_face4( 4,3,2,7,5);
	__check_face4( 5,5,7,6,4);

	ASSERT_EQ(0.1, D.PointsColor[0][0]);
	ASSERT_EQ(0.1, D.PointsColor[0][1]);
	ASSERT_EQ(0.1, D.PointsColor[0][2]);
	ASSERT_EQ(1.0, D.PointsColor[0][3]);
	ASSERT_EQ(0.2, D.PointsColor[1][0]);
	ASSERT_EQ(0.3, D.PointsColor[1][1]);
	ASSERT_EQ(0.4, D.PointsColor[1][2]);
	ASSERT_EQ(0.8, D.PointsColor[2][2]);
	ASSERT_EQ(1.0, D.PointsColor[6][2]);
	ASSERT_EQ(0.0, D.PointsColor[7][1]);
	ASSERT_EQ(0.0, D.PointsColor[7][2]);

}

TEST(Test, cube4_coff3){
	char filename[256] = FILES_DIRECTORY "cube4_coff3.off";

	OffClass D;

	ASSERT_EQ(0,D.read_off(filename));

	ASSERT_EQ(0,D.EnablePointsFunction);
	ASSERT_EQ(0,D.EnablePointsNormal);
	ASSERT_EQ(1,D.EnablePointsColor);
	ASSERT_EQ(0,D.EnablePointsMaterial);
	ASSERT_EQ(0,D.EnablePointsTextureCoords);

	ASSERT_EQ(0,D.EnableFacesColor);
	ASSERT_EQ(0,D.EnableFacesMaterial);

	ASSERT_EQ(8,D.CountOfPoints);
	ASSERT_EQ(6,D.CountOfFaces);

	ASSERT_EQ(3,D.PointsDimention);

	__check_point3(0,-1.0,-1.0,1.0);
	__check_point3(2, 1.0,-1.0,-1.0);
	__check_point3(7, 1.0, 1.0,-1.0);

	__check_face4( 0,0,1,2,3);
	__check_face4( 1,4,0,3,5);
	__check_face4( 2,4,6,1,0);
	__check_face4( 3,1,6,7,2);
	__check_face4( 4,3,2,7,5);
	__check_face4( 5,5,7,6,4);

	ASSERT_EQ(0.1, D.PointsColor[0][0]);
	ASSERT_EQ(0.3, D.PointsColor[1][1]);
	ASSERT_EQ(0.4, D.PointsColor[1][2]);
	ASSERT_EQ(0.8, D.PointsColor[2][2]);
	ASSERT_EQ(1.0, D.PointsColor[6][2]);
	ASSERT_EQ(0.0, D.PointsColor[7][1]);
	ASSERT_EQ(0.0, D.PointsColor[7][2]);
	ASSERT_EQ(0.5, D.PointsColor[0][3]);
	ASSERT_EQ(0.6, D.PointsColor[1][3]);
	ASSERT_EQ(0.0, D.PointsColor[7][3]);

}


TEST(Test, cube4_noff){
	char filename[256] = FILES_DIRECTORY "cube4_noff.off";

	OffClass D;

	ASSERT_EQ(0,D.read_off(filename));

	ASSERT_EQ(0,D.EnablePointsFunction);
	ASSERT_EQ(0,D.EnablePointsNormal);
	ASSERT_EQ(0,D.EnablePointsColor);
	ASSERT_EQ(0,D.EnablePointsMaterial);
	ASSERT_EQ(0,D.EnablePointsTextureCoords);

	ASSERT_EQ(0,D.EnableFacesColor);
	ASSERT_EQ(0,D.EnableFacesMaterial);

	ASSERT_EQ(8,D.CountOfPoints);
	ASSERT_EQ(6,D.CountOfFaces);

	ASSERT_EQ(4,D.PointsDimention);

	__check_point3(0,-1.0,-1.0,1.0);
	__check_point3(2, 1.0,-1.0,-1.0);
	__check_point3(7, 1.0, 1.0,-1.0);

	EXPECT_NEAR(10.0,D.Points[0*D.PointsDimention+3],1.e-6);
	EXPECT_NEAR(10.0,D.Points[2*D.PointsDimention+3],1.e-6);
	EXPECT_NEAR(60.75,D.Points[4*D.PointsDimention+3],1.e-6);
	EXPECT_NEAR(75.0,D.Points[7*D.PointsDimention+3],1.e-6);

	__check_face4( 0,0,1,2,3);
	__check_face4( 1,4,0,3,5);
	__check_face4( 2,4,6,1,0);
	__check_face4( 3,1,6,7,2);
	__check_face4( 4,3,2,7,5);
	__check_face4( 5,5,7,6,4);

}

TEST(Test, cube4_Noff){
	char filename[256] = FILES_DIRECTORY "cube4_nooff.off";

	OffClass D;

	ASSERT_EQ(0,D.read_off(filename));

	ASSERT_EQ(0,D.EnablePointsFunction);
	ASSERT_EQ(1,D.EnablePointsNormal);
	ASSERT_EQ(0,D.EnablePointsColor);
	ASSERT_EQ(0,D.EnablePointsMaterial);
	ASSERT_EQ(0,D.EnablePointsTextureCoords);

	ASSERT_EQ(0,D.EnableFacesColor);
	ASSERT_EQ(0,D.EnableFacesMaterial);

	ASSERT_EQ(8,D.CountOfPoints);
	ASSERT_EQ(6,D.CountOfFaces);

	ASSERT_EQ(3,D.PointsDimention);

	__check_point3(0,-1.0,-1.0,1.0);
	__check_point3(2, 1.0,-1.0,-1.0);
	__check_point3(7, 1.0, 1.0,-1.0);

	EXPECT_NEAR(1.0,	D.PointsNormal[0*D.PointsDimention+0],1.e-6);
	EXPECT_NEAR(0.1,	D.PointsNormal[0*D.PointsDimention+1],1.e-6);
	EXPECT_NEAR(0.0,	D.PointsNormal[0*D.PointsDimention+2],1.e-6);
	EXPECT_NEAR(21.25,	D.PointsNormal[1*D.PointsDimention+0],1.e-6);
	EXPECT_NEAR(55.0,	D.PointsNormal[1*D.PointsDimention+1],1.e-6);
	EXPECT_NEAR(89.0,	D.PointsNormal[1*D.PointsDimention+2],1.e-6);
	EXPECT_NEAR(75.0,	D.PointsNormal[7*D.PointsDimention+0],1.e-6);
	EXPECT_NEAR(54.0,	D.PointsNormal[7*D.PointsDimention+1],1.e-6);
	EXPECT_NEAR(123.1,	D.PointsNormal[7*D.PointsDimention+2],1.e-6);

	__check_face4( 0,0,1,2,3);
	__check_face4( 1,4,0,3,5);
	__check_face4( 2,4,6,1,0);
	__check_face4( 3,1,6,7,2);
	__check_face4( 4,3,2,7,5);
	__check_face4( 5,5,7,6,4);

}

TEST(Test, cube4_stoff){
	char filename[256] = FILES_DIRECTORY "cube4_stoff.off";

	OffClass D;

	ASSERT_EQ(0,D.read_off(filename));

	ASSERT_EQ(0,D.EnablePointsFunction);
	ASSERT_EQ(0,D.EnablePointsNormal);
	ASSERT_EQ(0,D.EnablePointsColor);
	ASSERT_EQ(0,D.EnablePointsMaterial);
	ASSERT_EQ(1,D.EnablePointsTextureCoords);

	ASSERT_EQ(0,D.EnableFacesColor);
	ASSERT_EQ(0,D.EnableFacesMaterial);

	ASSERT_EQ(8,D.CountOfPoints);
	ASSERT_EQ(6,D.CountOfFaces);

	ASSERT_EQ(3,D.PointsDimention);

	__check_point3(0,-1.0,-1.0,1.0);
	__check_point3(2, 1.0,-1.0,-1.0);
	__check_point3(7, 1.0, 1.0,-1.0);

	EXPECT_NEAR(1.0,	D.PointsTextureCoords[0][0],1.e-6);
	EXPECT_NEAR(0.1,	D.PointsTextureCoords[0][1],1.e-6);
	EXPECT_NEAR(21.25,	D.PointsTextureCoords[1][0],1.e-6);
	EXPECT_NEAR(55.0,	D.PointsTextureCoords[1][1],1.e-6);
	EXPECT_NEAR(75.0,	D.PointsTextureCoords[7][0],1.e-6);
	EXPECT_NEAR(54.0,	D.PointsTextureCoords[7][1],1.e-6);

	__check_face4( 0,0,1,2,3);
	__check_face4( 1,4,0,3,5);
	__check_face4( 2,4,6,1,0);
	__check_face4( 3,1,6,7,2);
	__check_face4( 4,3,2,7,5);
	__check_face4( 5,5,7,6,4);

}

TEST(Test, cube4_fm){
	char filename[256] = FILES_DIRECTORY "cube4_fm.off";

	OffClass D;

	ASSERT_EQ(0,D.read_off(filename));

	ASSERT_EQ(0,D.EnablePointsFunction);
	ASSERT_EQ(0,D.EnablePointsNormal);
	ASSERT_EQ(0,D.EnablePointsColor);
	ASSERT_EQ(0,D.EnablePointsMaterial);
	ASSERT_EQ(0,D.EnablePointsTextureCoords);

	ASSERT_EQ(0,D.EnableFacesColor);
	ASSERT_EQ(1,D.EnableFacesMaterial);

	ASSERT_EQ(8,D.CountOfPoints);
	ASSERT_EQ(6,D.CountOfFaces);

	ASSERT_EQ(3,D.PointsDimention);

	__check_point3(0,-1.0,-1.0,1.0);
	__check_point3(2, 1.0,-1.0,-1.0);
	__check_point3(7, 1.0, 1.0,-1.0);

	__check_face4( 0,0,1,2,3);
	__check_face4( 1,4,0,3,5);
	__check_face4( 2,4,6,1,0);
	__check_face4( 3,1,6,7,2);
	__check_face4( 4,3,2,7,5);
	__check_face4( 5,5,7,6,4);

	ASSERT_EQ(1,	D.FacesMaterial[0]);
	ASSERT_EQ(2,	D.FacesMaterial[1]);
	ASSERT_EQ(3,	D.FacesMaterial[2]);
	ASSERT_EQ(4,	D.FacesMaterial[3]);
	ASSERT_EQ(0,	D.FacesMaterial[4]);
	ASSERT_EQ(-1,	D.FacesMaterial[5]);


}

TEST(Test, cube4_all){
	char filename[256] = FILES_DIRECTORY "cube4_alloff.off";

	OffClass D;

	ASSERT_EQ(0,D.read_off(filename));

	ASSERT_EQ(1,D.EnablePointsFunction);
	ASSERT_EQ(1,D.EnablePointsNormal);
	ASSERT_EQ(1,D.EnablePointsColor);
	ASSERT_EQ(1,D.EnablePointsMaterial);
	ASSERT_EQ(1,D.EnablePointsTextureCoords);

	ASSERT_EQ(1,D.EnableFacesColor);
	ASSERT_EQ(1,D.EnableFacesMaterial);

	ASSERT_EQ(9,D.CountOfPoints);
	ASSERT_EQ(5,D.CountOfFaces);

	ASSERT_EQ(2,D.PointsDimention);

	__check_point(0,0,0.0);
	__check_point(0,1,0.0);
	__check_point(1,0,0.0);
	__check_point(1,1,1.0);
	__check_point(2,0,1.0);
	__check_point(2,1,2.0);
	__check_point(3,0,1.0);
	__check_point(3,1,3.0);
	__check_point(4,0,2.0);
	__check_point(4,1,4.0);
	__check_point(5,0,4.0);
	__check_point(5,1,4.1);
	__check_point(6,0,0.1);
	__check_point(6,1,2.9);
	__check_point(7,0,-.9);
	__check_point(7,1,0.3);
	__check_point(8,0,0.0);
	__check_point(8,1,0.2);

	ASSERT_EQ(1,D.FacesSizes[1]-D.FacesSizes[0]);
	EXPECT_EQ(0,D.Faces[D.FacesSizes[0]+0]);

	ASSERT_EQ(2,D.FacesSizes[2]-D.FacesSizes[1]);
	EXPECT_EQ(1,D.Faces[D.FacesSizes[1]+0]);
	EXPECT_EQ(2,D.Faces[D.FacesSizes[1]+1]);

	ASSERT_EQ(3,D.FacesSizes[3]-D.FacesSizes[2]);
	EXPECT_EQ(2,D.Faces[D.FacesSizes[2]+0]);
	EXPECT_EQ(3,D.Faces[D.FacesSizes[2]+1]);
	EXPECT_EQ(4,D.Faces[D.FacesSizes[2]+2]);

	ASSERT_EQ(4,D.FacesSizes[4]-D.FacesSizes[3]);
	EXPECT_EQ(3,D.Faces[D.FacesSizes[3]+0]);
	EXPECT_EQ(4,D.Faces[D.FacesSizes[3]+1]);
	EXPECT_EQ(5,D.Faces[D.FacesSizes[3]+2]);
	EXPECT_EQ(6,D.Faces[D.FacesSizes[3]+3]);

	ASSERT_EQ(5,D.FacesSizes[5]-D.FacesSizes[4]);
	EXPECT_EQ(3,D.Faces[D.FacesSizes[4]+0]);
	EXPECT_EQ(4,D.Faces[D.FacesSizes[4]+1]);
	EXPECT_EQ(7,D.Faces[D.FacesSizes[4]+2]);
	EXPECT_EQ(8,D.Faces[D.FacesSizes[4]+3]);
	EXPECT_EQ(0,D.Faces[D.FacesSizes[4]+4]);

//	ASSERT_EQ(1,	D.FacesMaterial[0]);
//	EXPECT_NEAR(1.0,	D.PointsTextureCoords[0][0],1.e-6);

	EXPECT_NEAR(0.1,	D.PointsFunction[0],1.e-6);
	EXPECT_NEAR(0.2,	D.PointsFunction[1],1.e-6);
	EXPECT_NEAR(0.1,	D.PointsFunction[2],1.e-6);
	EXPECT_NEAR(0.3,	D.PointsFunction[3],1.e-6);
	EXPECT_NEAR(0.4,	D.PointsFunction[4],1.e-6);
	EXPECT_NEAR(1.0,	D.PointsFunction[5],1.e-6);
	EXPECT_NEAR(-.1,	D.PointsFunction[6],1.e-6);
	EXPECT_NEAR(0.0,	D.PointsFunction[7],1.e-6);
	EXPECT_NEAR(.999,	D.PointsFunction[8],1.e-6);

	EXPECT_NEAR(1.0,	D.PointsNormal[0],1.e-6);
	EXPECT_NEAR(1.0,	D.PointsNormal[1],1.e-6);
	EXPECT_NEAR(1.0,	D.PointsNormal[2],1.e-6);
	EXPECT_NEAR(-1.0,	D.PointsNormal[3],1.e-6);
	EXPECT_NEAR(-1.0,	D.PointsNormal[4],1.e-6);
	EXPECT_NEAR(1.0,	D.PointsNormal[5],1.e-6);
	EXPECT_NEAR(-1.0,	D.PointsNormal[6],1.e-6);
	EXPECT_NEAR(0.0,	D.PointsNormal[7],1.e-6);
	EXPECT_NEAR(0.0,	D.PointsNormal[8],1.e-6);
	EXPECT_NEAR(1.0,	D.PointsNormal[9],1.e-6);
	EXPECT_NEAR(0.5,	D.PointsNormal[10],1.e-6);
	EXPECT_NEAR(0.4,	D.PointsNormal[11],1.e-6);
	EXPECT_NEAR(0.1,	D.PointsNormal[12],1.e-6);
	EXPECT_NEAR(0.9,	D.PointsNormal[13],1.e-6);
	EXPECT_NEAR(1.0,	D.PointsNormal[14],1.e-6);
	EXPECT_NEAR(1.0,	D.PointsNormal[15],1.e-6);
	EXPECT_NEAR(0.0,	D.PointsNormal[16],1.e-6);
	EXPECT_NEAR(0.0,	D.PointsNormal[17],1.e-6);

	ASSERT_EQ(0,	D.PointsMaterial[0]);
	ASSERT_EQ(2,	D.PointsMaterial[1]);
	ASSERT_EQ(3,	D.PointsMaterial[2]);
	ASSERT_EQ(4,	D.PointsMaterial[3]);
	ASSERT_EQ(0,	D.PointsMaterial[4]);
	ASSERT_EQ(0,	D.PointsMaterial[5]);
	ASSERT_EQ(0,	D.PointsMaterial[6]);
	ASSERT_EQ(0,	D.PointsMaterial[7]);
	ASSERT_EQ(0,	D.PointsMaterial[8]);

#define __check_ts(A,i,c1,c2) 	EXPECT_NEAR(c1,A[i][0],1.e-6); \
 	EXPECT_NEAR(c2,A[i][1],1.e-6);
#define __check_color4(A,i,c1,c2,c3,c4) 	EXPECT_NEAR(c1,A[i][0],1.e-6); \
	EXPECT_NEAR(c2,A[i][1],1.e-6); \
	EXPECT_NEAR(c3,A[i][2],1.e-6); \
	EXPECT_NEAR(c4,A[i][3],1.e-6);

	__check_color4(D.PointsColor,0,1,1,0,1);
	__check_color4(D.PointsColor,1,0,0,0,0); //default color
	__check_color4(D.PointsColor,2,0,0,0,0); //default color
	__check_color4(D.PointsColor,3,0,0,0,0); //default color
	__check_color4(D.PointsColor,4,1,1,1,.5);
	__check_color4(D.PointsColor,5,1,0,0,.6);
	__check_color4(D.PointsColor,6,0,1,0,.7);
	__check_color4(D.PointsColor,7,0,0,1,.8);
	__check_color4(D.PointsColor,8,.6,.6,.6,1);

	__check_ts(D.PointsTextureCoords,0,.0,.0);
	__check_ts(D.PointsTextureCoords,1,.0,.1);
	__check_ts(D.PointsTextureCoords,2,.1,.2);
	__check_ts(D.PointsTextureCoords,3,.1,.3);
	__check_ts(D.PointsTextureCoords,4,.2,.4);
	__check_ts(D.PointsTextureCoords,5,.4,.41);
	__check_ts(D.PointsTextureCoords,6,1,1);
	__check_ts(D.PointsTextureCoords,7,.9,.9);
	__check_ts(D.PointsTextureCoords,8,.1,.9);

	ASSERT_EQ(1,	D.FacesMaterial[0]);
	ASSERT_EQ(2,	D.FacesMaterial[1]);
	ASSERT_EQ(3,	D.FacesMaterial[2]);
	ASSERT_EQ(0,	D.FacesMaterial[3]);
	ASSERT_EQ(0,	D.FacesMaterial[4]);

	__check_color4(D.FacesColor,0,0,0,0,0); //default color
	__check_color4(D.FacesColor,1,0,0,0,0); //default color
	__check_color4(D.FacesColor,2,0,0,0,0); //default color
	__check_color4(D.FacesColor,3,.1,.2,.3,1); //default color
	__check_color4(D.FacesColor,4,1,1,1,.6); //default color

}

//TCoff
//all off

int main (int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
