/*
 * rw_mesh_plot3d.c
 * Фунции работы с форматом plot3d
 *
 *  Created on: 02.08.2013
 *      Author: moric
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "rw_mesh_plot3d.h"


static int _read_array_REAL(FILE*fd,int count,REAL*value,int*current_line,char*first_line){
	int i;
	char line[256];
	char word[256];

	i=0;
	if(first_line && first_line[0]){
		strcpy(line,first_line);
	}else{
		line[0]=0;
	}
	word[0]=0;
	while(i<count){
		if(line[0]==0){
			read_line_trim(fd,line);(*current_line)++;
		}
		string_cut_word(line,word);

		if(sscanf(word,"%lf",value+i)!=1){
			printf("%d/%d\n",i,count);
			rw_mesh_set_error(*current_line,"Error in reading values");
			return 1;
		}
		i++;
	}

	return 0;
}

int read_format_pxdfmt(
			int*_CountOfBlocks,
			int*_dimension,
			struct plot3d_struct**_Blocks,
			char*filename){
	__save_locale;
	FILE*fd;
	int i,j,k;
	int current_line;
	char*line;
	char*word;
	int dimension;

	int CountOfBlocks;
	struct plot3d_struct*Blocks;

	dimension = (_dimension)?(*_dimension):0;

	__load_locale;

	rw_mesh_set_filename(filename);
	fd=fopen(filename,"r");if(fd==NULL){
		rw_mesh_set_error(0,"Can't find file");
		return 1;
	}
	current_line = 0;

	line = (char*)calloc(256,sizeof(char));
	word = (char*)calloc(256,sizeof(char));

	//read count of blocks
	read_line_trim(fd,line);current_line++;
	if(sscanf(line,"%d",&CountOfBlocks)!=1){
		rw_mesh_set_error(current_line,"Can't read count of blocks");
		return 1;
	}

	if(CountOfBlocks==0){
		ffree(line);
		ffree(word);
		fclose(fd);
		if(_CountOfBlocks)
			*_CountOfBlocks = CountOfBlocks;
		return 0;
	}else if(CountOfBlocks<0){
		ffree(line);
		ffree(word);
		fclose(fd);
		rw_mesh_set_error(current_line,"Count of blocks less 0");
		return 1;
	}

	if(_CountOfBlocks)
		*_CountOfBlocks = CountOfBlocks;

	Blocks = (struct plot3d_struct*)calloc(CountOfBlocks,sizeof(struct plot3d_struct*));

	//read (NI NJ [NK]){CountOfBlocks}
	read_line_trim(fd,line);current_line++;

	if(dimension==0){ //auto detection of dimension
		i = string_count_of_words(line);
		if(i == 2*CountOfBlocks){
			dimension = 2;
		}else if(i == 3*CountOfBlocks){
			dimension = 3;
		}
	}
	if(dimension!=2 && dimension!=3){
		rw_mesh_set_error(current_line,"Error on reading sizes of grid, expected 2*CountOfBlocks or 3*CountOfBlocks");
		return 1;
	}

	for(i=0;i<CountOfBlocks;i++){
		Blocks[i].dimension = dimension;
		Blocks[i].ni = 1;
		Blocks[i].nj = 1;
		Blocks[i].nk = 1;
		Blocks[i].points = NULL;
	}

	for(i=0;i<CountOfBlocks;i++){

		for(j=0;j<dimension;j++){
			if(!string_cut_word(line,word)){
				if(dimension==2){
					rw_mesh_set_error(current_line,"Error on reading sizes of grid");
				}else if(dimension==3){
					rw_mesh_set_error(current_line,"Error on reading sizes of grid");
				}
				ffree(Blocks);
				ffree(line);
				ffree(word);
				fclose(fd);
				return 1;
			}
			if(sscanf(word,"%d",&k)!=1){
				rw_mesh_set_error(current_line,"Error on reading sizes of grid");
				ffree(Blocks);
				ffree(line);
				ffree(word);
				fclose(fd);
				return 1;
			}
			if(j==0){
				Blocks[i].ni = k;
			}else if(j==1){
				Blocks[i].nj = k;
			}else if(j==2){
				Blocks[i].nk = k;
			}
		}

		printf("%d %d %d\n",Blocks[i].ni,Blocks[i].nj,Blocks[i].nk);
	}

	for(i=0;i<CountOfBlocks;i++){
		Blocks[i].points = (REAL*)calloc(dimension*Blocks[i].ni*Blocks[i].nj*Blocks[i].nk,sizeof(REAL));
		if(_read_array_REAL(fd,dimension*Blocks[i].ni*Blocks[i].nj*Blocks[i].nk,(REAL*)Blocks[i].points,&current_line,0)){
			for(i=0;i<CountOfBlocks;i++)
				ffree(Blocks[i].points);
			ffree(Blocks);
			ffree(line);
			ffree(word);
			fclose(fd);
			return 1;
		}
	}

	if(_Blocks){
		*_Blocks = Blocks;
	}else{
		ffree(Blocks);
	}
	ffree(line);
	ffree(word);
	fclose(fd);
	return 0;

}

int read_format_p2dfmt(
			int*_CountOfBlocks,
			int**_NI, int**_NJ,
			REAL2***_Points,
			char*filename){
	int i;
	int dimention;
	int CountOfBlocks;
	int*NI,*NJ;
	REAL**Points;
	struct plot3d_struct*Blocks;
	dimention = 2;
	i = read_format_pxdfmt(&CountOfBlocks,&dimention,&Blocks,filename);

	if(i){
		return i;
	}

	NI = (int*)calloc(CountOfBlocks,sizeof(int));
	NJ = (int*)calloc(CountOfBlocks,sizeof(int));
	Points = (REAL**)calloc(2*CountOfBlocks,sizeof(REAL*));

	for(i=0;i<CountOfBlocks;i++){
		NI[i] = Blocks[i].ni;
		NJ[i] = Blocks[i].nj;
		Points[i] = Blocks[i].points;
	}

	ffree(Blocks);

	if(_CountOfBlocks) *_CountOfBlocks = CountOfBlocks;
	if(_NI){*_NI = NI;}else{ffree(NI);}
	if(_NJ){*_NJ = NJ;}else{ffree(NJ);}
	if(_Points){
		*_Points = (REAL2**)Points;
	}else{
		for(i=0;i<CountOfBlocks;i++)
			ffree(Points[i]);
		ffree(Points);
	}
	return 0;
}

int read_format_p3dfmt(
			int*_CountOfBlocks,
			int**_NI, int**_NJ, int**_NK,
			REAL3***_Points,
			char*filename){
	int i;
	int dimention;
	int CountOfBlocks;
	int*NI,*NJ,*NK;
	REAL**Points;
	struct plot3d_struct*Blocks;
	dimention = 0; // auto detection
	i = read_format_pxdfmt(&CountOfBlocks,&dimention,&Blocks,filename);

	if(i){
		return i;
	}

	NI = (int*)calloc(CountOfBlocks,sizeof(int));
	NJ = (int*)calloc(CountOfBlocks,sizeof(int));
	NK = (int*)calloc(CountOfBlocks,sizeof(int));
	Points = (REAL**)calloc(3*CountOfBlocks,sizeof(REAL*));

	for(i=0;i<CountOfBlocks;i++){
		NI[i] = Blocks[i].ni;
		NJ[i] = Blocks[i].nj;
		NK[i] = Blocks[i].nk;
		Points[i] = Blocks[i].points;
	}

	ffree(Blocks);

	if(_CountOfBlocks) *_CountOfBlocks = CountOfBlocks;
	if(_NI){*_NI = NI;}else{ffree(NI);}
	if(_NJ){*_NJ = NJ;}else{ffree(NJ);}
	if(_NK){*_NK = NK;}else{ffree(NK);}
	if(_Points){
		*_Points = (REAL3**)Points;
	}else{
		for(i=0;i<CountOfBlocks;i++)
			ffree(Points[i]);
		ffree(Points);
	}
	return 0;
}
