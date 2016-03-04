/*
 * rw_mesh_header.h
 * Общий заголовочный файл для всех форматов
 *
 *  Created on: 26.06.2013
 *      Author: Belokrys-Fedotov A.I.
 */

#ifndef RW_MESH_HEADER_H_
#define RW_MESH_HEADER_H_

#include <stdio.h>
#include <locale.h>

#ifndef NULL
#define NULL 0
#endif

#ifndef REAL
#define REAL double
#endif

typedef REAL REAL2[2];
typedef REAL REAL3[3];
typedef REAL REAL4[4];


typedef int INT;
typedef INT INT2[2];
typedef INT INT3[3];
typedef INT INT4[4];
typedef INT INT5[5];
typedef INT INT6[6];
typedef INT INT7[7];
typedef INT INT8[8];

#define log_write

#define __save_locale char*oldlocale = setlocale(LC_NUMERIC, "C")
#define __load_locale setlocale(LC_NUMERIC, oldlocale)

#ifndef ffree
#define ffree(pointer) if(pointer){free(pointer);pointer=NULL;}
#endif

void rw_mesh_set_filename(char*filename);
void rw_mesh_set_error(int lineNumber,char*error);
void rw_mesh_get_error(int*lineNumber,char*error,char*filename);
void rw_mesh_print_error();

#define RW_MESH_CELL_TYPE_NONE			 0
#define RW_MESH_CELL_TYPE_EDGE			 3
#define RW_MESH_CELL_TYPE_TRIANGLE		 5
#define RW_MESH_CELL_TYPE_POLYGON		 7
#define RW_MESH_CELL_TYPE_QUAD			 9
#define RW_MESH_CELL_TYPE_TETRA			10
#define RW_MESH_CELL_TYPE_VOXEL 		11
#define RW_MESH_CELL_TYPE_HEXAHEDRON	12
#define RW_MESH_CELL_TYPE_WEDGE			13
#define RW_MESH_CELL_TYPE_PYRAMID		14

/**
 * Размер ячеёки типа CellType
 * Возвращает -1 для типа RW_MESH_CELL_TYPE_POLYGON, и -2 для неизвестного типа
 */
int rw_mesh_cell_type_size(int CellType);

#endif /* RW_MESH_HEADER_H_ */
