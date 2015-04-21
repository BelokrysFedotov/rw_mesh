/*
 * rw_mesh_header.h
 * Общий заголовочный файл для всех форматов
 *
 *  Created on: 26.06.2013
 *      Author: moric
 */

#ifndef RW_MESH_HEADER_H_
#define RW_MESH_HEADER_H_

#include <stdio.h>
#include <locale.h>

#ifndef NULL
#define NULL 0
#endif

typedef double REAL;

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

#endif /* RW_MESH_HEADER_H_ */
