/*
 * format_off.h
 *
 *  Created on: 15.12.2011
 *      Author: moric
 */

#ifndef FORMAT_OFF_H_
#define FORMAT_OFF_H_

/**
 * Чтение файла формата Off содержащего только треугольники
 */
int read_off3(char*filename,int*CountOfPoints, int*CountOfFaces,REAL3**Points, INT3**Faces);
int write_off(char*filename,int CountOfPoints, int CountOfFaces,REAL3*Points, INT3*Faces);

#endif /* FORMAT_OFF_H_ */
