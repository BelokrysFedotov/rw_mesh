/*
 * rw_mesh_string.h
 * Общие функции работы со строками и файлами
 *
 *  Created on: 26.06.2013
 *      Author: moric
 */

#ifndef RW_MESH_STRING_H_
#define RW_MESH_STRING_H_

#include "rw_mesh_header.h"

#define CHARSET_SPACE " \t\r\n\x0B"
#define CHARSET_NUMERIC "0123456789"
#define CHARSET_ABC "ABCDEFGHIJKLMNOPRSTUVWXYZ"
#define CHARSET_PLUSMINUS "+-"

int read_line (FILE * STREAM, char *line);


int in_charset(const char c,const char*charset);
int string_ltrim_charset(char*line,const char*charset);
int string_rtrim_charset(char*line,const char*charset);
int string_trim_charset(char*line,const char*charset);
int string_trim(char*line);
int read_line_trim (FILE * STREAM, char *line);
int string_is_integer(const char*string);
int string_count_of_words(const char*string);
int string_cut_word(char*string,char*word);

#endif /* RW_MESH_STRING_H_ */
