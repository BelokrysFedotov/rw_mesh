/*
 * rw_mesh_string.c
 * Общие функции работы со строками и файлами
 *
 *  Created on: 26.06.2013
 *      Author: moric
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "rw_mesh_string.h"

/**
 * Прочитать из потока STREAM строку line.
 * Возращает кол-во прочитанных символов (длина строки).
 * Символы \r и \n отрезаются
 */
int read_line (FILE * STREAM, char *line){
  int i;

  if (fgets (line, 256, STREAM) == NULL)
      return (-1);

  i = strlen(line);

  while(i>0 && (line[i-1]=='\r' || line[i-1]=='\n')){
	  i--;line[i]='\0';
  }

  return i;
}

/**
 * Найти символ в наборе символов и вывести его индекс
 * или вернуть -1, если не найдено
 */
int in_charset(const char c,const char*charset){
	int i;
	i=0;
	while(i<255 && charset[i]!='\0'){
		if(c==charset[i])return i;
		i++;
	}
	return -1;
}

/**
 * Удалить символы из указанного набора вначале строки
 */
int string_ltrim_charset(char*line,const char*charset){
	int i,j;

	i=0;
	while(in_charset(line[i],charset)>=0) i++;

	j=0;
	while(line[i+j]!='\0'){
		line[j] = line[i+j];
		j++;
	}
	line[j]='\0';

	return j;
}

/**
 * Удалить символы из указанного набора вконце строки
 */
int string_rtrim_charset(char*line,const char*charset){
	int i,j,l;

	l = strlen(line);
	if(l==0)return 0;

	i=l-1;
	while(i>=0 && in_charset(line[i],charset)>=0) i--;
	i++;
	line[i]='\0';
	return i;
}


/**
 * Удалить символы из указанного набора c концов строки
 */
int string_trim_charset(char*line,const char*charset){
	string_rtrim_charset(line,charset);
	return string_ltrim_charset(line,charset);
}

/**
 * Удаляет пробельные символы c концов строки
 */
int string_trim(char*line){
	return string_trim_charset(line," \t\r\n\x0B");
}

/**
 * Прочитать из потока строку и удалить пробелные символы на концах
 */
int read_line_trim (FILE * STREAM, char *line){
	if(!read_line(STREAM,line))return 0;
	return string_trim(line);
}

int string_is_integer(const char*string){
	int i;
	i=0;
	if(string[i]=='\0')return 0;
	if(in_charset(string[i],CHARSET_PLUSMINUS)>=0)i++;
	if(string[i]=='\0')return 0;
	while(string[i]!='\0' && in_charset(string[i],CHARSET_NUMERIC)>=0)i++;
	if(string[i]=='\0')return 1;
	return 0;
}

int string_count_of_words(const char*string){
	int count;
	int i,s;

	count = 0;

	i=0;
	if(string[i]=='\0')return 0;
	while(string[i]!='\0' && in_charset(string[i],CHARSET_SPACE)>=0)i++;
	if(string[i]=='\0')return 0;

	s = 1;
	count = 1;

	while(string[i]!='\0'){
		if(in_charset(string[i],CHARSET_SPACE)>=0){
			if(s){
				s = 0;
			}
		}else{
			if(!s){
				s = 1;
				count++;
			}
		}
		i++;
	}
	return count;
}

int string_cut_word(char*string,char*word){
	int count;
	int i,j,k;

	count = 0;

	i=0;

	word[0]='\0';
	if(string[i]=='\0')return 0;
	while(string[i]!='\0' && in_charset(string[i],CHARSET_SPACE)>=0)i++;
	if(string[i]=='\0'){
		string[0]='\0';
		return 0;
	}

	j = i;
	k = 0;
	while(string[i]!='\0' && in_charset(string[i],CHARSET_SPACE)<0){
		word[k] = string[i];
		i++;
		k++;
	}
	word[k] = '\0';

	while(string[i]!='\0' && in_charset(string[i],CHARSET_SPACE)>=0)i++;

	k=0;
	while(string[i]!='\0'){
		string[k] = string[i];
		k++;
		i++;
	}
	string[k] = '\0';

	return 1;
}
