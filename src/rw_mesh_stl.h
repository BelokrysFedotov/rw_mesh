/*
 * rw_mesh_stl.h
 * Фунции работы с форматом stl
 *
 *  Created on: 07.03.2016
 *      Author: Belokrys-Fedotov A.I.
 */

#ifndef RW_MESH_STL_H_
#define RW_MESH_STL_H_

#include "rw_mesh_header.h"

#define EXTENTION_STL "*\.stl"

int read_format_stl(
			int*CountOfFaces,
			REAL3**FaceVertexes,
			REAL3**FaceNormals,
			char*filename);

int read_format_stl_simplified(
			int*CountOfPoints,
			REAL3**Points,
			int*CountOfFaces,
			INT3**Faces,
			REAL3**FaceNormals,
			char*filename);

int write_format_stl(
		int CountOfFaces,
		REAL3*FaceVertexes,
		REAL3*FaceNormals,
		char*filename,
		int binary);


int write_format_stl_simpifiled(
		int CountOfPoints,
		REAL3*Points,
		int CountOfFaces,
		INT3*Faces,
		REAL3*FaceNormals,
		char*filename,
		int binary);

#endif /* RW_MESH_STL_H_ */
