/*
 * rw_mesh_off.h
 * Фунции работы с форматом off
 *
 *  Created on: 14.06.2013
 *      Author: moric
 */

#ifndef RW_MESH_OFF_H_
#define RW_MESH_OFF_H_

#include "rw_mesh_header.h"

#define EXTENTION_OFF "*\.off"
#define EXTENTION_OFF_EXT "*\.(st|c|n|4)*off"

int read_format_off(
			int*CountOfPoints, REAL**Points,int* PointsDimention,
			int*EnablePointsFunction,REAL**PointsFunction,
			int*EnablePointsNormal,REAL**PointsNormal,
			int*EnablePointsMaterial,int**PointsMaterial,
			int*EnablePointsColor,REAL4**PointsColor,
			int*EnablePointsTextureCoords,REAL2**PointsTextureCoords,
			int*CountOfFaces, int**Faces,int**FacesSizes,
			int*EnableFacesMaterial, int**FacesMaterial,
			int*EnableFacesColor,REAL4**FacesColor,
			char*filename);

int write_format_off(
		int CountOfPoints, REAL*Points,int PointsDimention,
		REAL*PointsFunction,REAL*PointsNormal,int*PointsMaterial,REAL4*PointsColor,REAL2*PointsTextureCoords,
		int CountOfFaces, int*Faces,int*FacesSizes,
		int*FacesMaterial,REAL4*FacesColor,
		char*filename);

#endif /* RW_MESH_OFF_H_ */
