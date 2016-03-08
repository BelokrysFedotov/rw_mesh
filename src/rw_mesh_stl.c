/*
 * format_off.c
 *
 * Format OFF
 *
 *  Created on: 15.12.2011
 *      Author: Belokrys-Fedotov A.I.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "rw_mesh_off.h"



int read_format_stl(
			int*CountOfFaces,
			REAL3**FaceVertexes,
			REAL3**FaceNormals,
			char*filename){

	__save_locale;

	FILE*fd;
	int i,j,k;

	char line[256];
	char word[256];
	int current_line;

	int faces_allocated;
	int faces_count;
	REAL3*faces_vertex;
	REAL3*faces_normals;

	uint32_t uint32;
	uint16_t uint16;
	float 	 real32;


	faces_allocated = 0;
	faces_count = 0;
	faces_vertex = NULL;
	faces_normals = NULL;

	rw_mesh_set_filename(filename);
	fd=fopen(filename,"r");if(fd==NULL){
		rw_mesh_set_error(0,"Can't find file");
		return 1;
	}
	current_line = 0;

	//read 'solid '
	if(fread(line,1,6,fd)<6){
		fclose(fd);
		rw_mesh_set_error(1,"Can't read header");
		return 1;
	}

	if(strncmp(line,"solid ",6)==0){
		//it is ASCII file

		//read name, skip
		current_line++;
		if(read_line_trim(fd,line)<0){
			fclose(fd);
			rw_mesh_set_error(current_line,"Can't read header");
			return 1;
		}




		while(1){
			current_line++;
			if(read_line_trim(fd,line)<0){
				ffree(faces_vertex);
				ffree(faces_normals);
				fclose(fd);
				rw_mesh_set_error(current_line,"There is no end of file (endsolid)");
				return 1;
			}

			if(!string_cut_word(line,word)){
				ffree(faces_vertex);
				ffree(faces_normals);
				fclose(fd);
				rw_mesh_set_error(current_line,"empty string");
				return 1;
			}

			if(strcmp(word,"endsolid")==0){
				break;
			}

			//facet normal ni nj nk
			if(strcmp(word,"facet")!=0){
				ffree(faces_vertex);
				ffree(faces_normals);
				fclose(fd);
				rw_mesh_set_error(current_line,"unknown keyword");
				return 1;
			}

			if(faces_allocated){
				if(faces_count >= faces_allocated){
					faces_allocated += 16;
					faces_vertex = (REAL3*)realloc(faces_vertex,faces_allocated*3*sizeof(REAL3));
					faces_normals = (REAL3*)realloc(faces_normals,faces_allocated*sizeof(REAL3));
				}
			}else{
				faces_allocated = 16;
				faces_vertex = (REAL3*)calloc(faces_allocated,3*sizeof(REAL3));
				faces_normals = (REAL3*)calloc(faces_allocated,sizeof(REAL3));
			}

			if(!string_cut_word(line,word) || strcmp(word,"normal")!=0){
				ffree(faces_vertex);
				ffree(faces_normals);
				fclose(fd);
				rw_mesh_set_error(current_line,"it's must be 'facet normal ni nj nk'");
				return 1;
			}

			if(sscanf(line,"%lf %lf %lf",faces_normals[faces_count]+0,faces_normals[faces_count]+1,faces_normals[faces_count]+2)!=3){
				ffree(faces_vertex);
				ffree(faces_normals);
				fclose(fd);
				rw_mesh_set_error(current_line,"it's must be 'facet normal ni nj nk'");
				return 1;
			}

			current_line++;
			if(read_line_trim(fd,line)<0){
				ffree(faces_vertex);
				ffree(faces_normals);
				fclose(fd);
				rw_mesh_set_error(current_line,"Unexpecting end of file");
				return 1;
			}

			if(strcmp(line,"outer loop")!=0){
				ffree(faces_vertex);
				ffree(faces_normals);
				fclose(fd);
				rw_mesh_set_error(current_line,"it's must be 'outer loop'");
				return 1;
			}

			for(k=0;k<3;k++){
				current_line++;
				if(read_line_trim(fd,line)<0){
					ffree(faces_vertex);
					ffree(faces_normals);
					fclose(fd);
					rw_mesh_set_error(current_line,"Unexpecting end of file");
					return 1;
				}

				if(!string_cut_word(line,word) || strcmp(word,"vertex")!=0){
					ffree(faces_vertex);
					ffree(faces_normals);
					fclose(fd);
					rw_mesh_set_error(current_line,"it's must be 'vertex vx vy vz'");
					return 1;
				}

				if(sscanf(line,"%lf %lf %lf",faces_vertex[faces_count*3+k]+0,faces_normals[faces_count*3+k]+1,faces_normals[faces_count*3+k]+2)!=3){
					ffree(faces_vertex);
					ffree(faces_normals);
					fclose(fd);
					rw_mesh_set_error(current_line,"it's must be 'vertex vx vy vz'");
					return 1;
				}

			}

			current_line++;
			if(read_line_trim(fd,line)<0){
				ffree(faces_vertex);
				ffree(faces_normals);
				fclose(fd);
				rw_mesh_set_error(current_line,"Unexpecting end of file");
				return 1;
			}

			if(strcmp(line,"endloop")!=0){
				ffree(faces_vertex);
				ffree(faces_normals);
				fclose(fd);
				rw_mesh_set_error(current_line,"it's must be 'endloop'");
				return 1;
			}

			current_line++;
			if(read_line_trim(fd,line)<0){
				ffree(faces_vertex);
				ffree(faces_normals);
				fclose(fd);
				rw_mesh_set_error(current_line,"Unexpecting end of file");
				return 1;
			}

			if(strcmp(line,"endfacet")!=0){
				ffree(faces_vertex);
				ffree(faces_normals);
				fclose(fd);
				rw_mesh_set_error(current_line,"it's must be 'endfacet'");
				return 1;
			}

		}

		if(CountOfFaces)
			*CountOfFaces = faces_count;
		if(FaceVertexes){
			*FaceVertexes = faces_vertex;
		}else{
			ffree(faces_vertex);
		}
		if(FaceNormals){
			*FaceNormals = faces_normals;
		}else{
			ffree(faces_normals);
		}

		fclose(fd);
		return 0;
	}else{
		//it is BINARY file

		//read name
		if(fread(line+6,1,80-6,fd)<80-6){
			fclose(fd);
			rw_mesh_set_error(1,"Can't read header");
			return 1;
		}

		if(fread(&uint32,sizeof(uint32),1,fd)<1){
			fclose(fd);
			rw_mesh_set_error(1,"Can't read count of faces");
			return 1;
		}

		if(uint32==0){
			if(CountOfFaces)
				*CountOfFaces = 0;
			if(FaceVertexes)
				*FaceVertexes = NULL;
			if(FaceNormals)
				*FaceNormals = NULL;

			fclose(fd);
			return 0;
		}

		faces_count = uint32;
		faces_allocated = faces_count;
		faces_normals = (REAL3*)calloc(faces_count,sizeof(REAL3));
		faces_vertex = (REAL3*)calloc(faces_count,sizeof(REAL3));

		for(i=0;i<faces_count;i++){
			for(k=0;k<3;k++){
				if(fread(&real32,sizeof(real32),1,fd)<1){
					fclose(fd);
					rw_mesh_set_error(1,"Can't read data");
					return 1;
				}
				faces_normals[i][k] = real32;
			}
			for(j=0;j<3;j++){
				for(k=0;k<3;k++){
					if(fread(&real32,sizeof(real32),1,fd)<1){
						fclose(fd);
						rw_mesh_set_error(1,"Can't read data");
						return 1;
					}
					faces_vertex[i*3+j][k] = real32;
				}
			}
			if(fread(&uint16,sizeof(uint16),1,fd)<1){
				fclose(fd);
				rw_mesh_set_error(1,"Can't read data");
				return 1;
			}
		}

		if(CountOfFaces)
			*CountOfFaces = faces_count;
		if(FaceVertexes){
			*FaceVertexes = faces_vertex;
		}else{
			ffree(faces_vertex);
		}
		if(FaceNormals){
			*FaceNormals = faces_normals;
		}else{
			ffree(faces_normals);
		}


		fclose(fd);
		return 0;
	}



	return 1;
}

int read_format_stl_simplified(
			int*CountOfPoints,
			REAL3**Points,
			int*CountOfFaces,
			INT3**Faces,
			REAL3**FaceNormals,
			char*filename){

	int countOfFaces,i,k;
	REAL3* faceVertexes;
	REAL3* faceNormals;

	if(read_format_stl(&countOfFaces,&faceVertexes,&faceNormals,filename)){
		return 1;
	}

	if(countOfFaces==0){
		if(CountOfPoints) *CountOfPoints = 0;
		if(Points) *Points = NULL;
		if(CountOfFaces) *CountOfFaces = 0;
		if(Faces) *Faces = NULL;
		if(FaceNormals) *FaceNormals  = NULL;
		return 0;
	}

	if(CountOfPoints)
		*CountOfPoints = countOfFaces*9;
	if(Points){
		*Points = (REAL3*)faceVertexes;
	}else{
		ffree(faceVertexes);
	}
	if(CountOfFaces)
		*CountOfFaces = countOfFaces;
	if(Faces){
		*Faces = (INT3*)calloc(countOfFaces,sizeof(INT3));
		for(i=0;i<countOfFaces;i++)
			for(k=0;k<3;k++)
				(*Faces)[i][k] = i*3+k;
	}
	if(FaceNormals){
		*FaceNormals = faceVertexes;
	}else{
		ffree(faceNormals);
	}

	return 1;
}

int write_format_stl(
		int CountOfFaces,
		REAL3*FaceVertexes,
		REAL3*FaceNormals,
		char*filename,
		int binary){

	__save_locale;

	FILE*fd;
	char line[256];
	int i,j,k;

	uint32_t uint32;
	uint16_t uint16;
	float 	 real32;

	rw_mesh_set_filename(filename);
	fd=fopen(filename,"w");if(fd==NULL){
		rw_mesh_set_error(0,"Can't find file");
		return 1;
	}

	if(binary){
		strcpy(line,"noname");
		i = strlen(line);
		memset(line+i,0,80-i);
		fwrite(line,80,1,fd);

		uint32 = CountOfFaces;
		fwrite(&uint32,sizeof(uint32),1,fd);

		for(i=0;i<CountOfFaces;i++){
			for(k=0;k<3;k++){
				real32 = FaceNormals[i][k];
				fwrite(&real32,sizeof(real32),1,fd);
			}

			for(j=0;j<3;j++){
				for(k=0;k<3;k++){
					real32 = FaceVertexes[i][k];
					fwrite(&real32,sizeof(real32),1,fd);
				}
			}

			uint16 = 0;
			fwrite(&uint16,sizeof(uint16),1,fd);
		}

	}else{
		fprintf(fd,"solid %s\n","noname");
		for(i=0;i<CountOfFaces;i++){
			fprintf(fd,"facet normal %e %e %e\n",FaceNormals[i][0],FaceNormals[i][1],FaceNormals[i][2]);
			fprintf(fd,"outer loop\n");
			for(j=0;j<3;j++){
				fprintf(fd,"vertex %e %e %e\n",FaceVertexes[i*3+j][0],FaceVertexes[i*3+j][1],FaceVertexes[i*3+j][2]);
			}
			fprintf(fd,"endloop\n");
			fprintf(fd,"endfacet\n");
		}

		fprintf(fd,"endsolid %s\n","noname");
	}

	fclose(fd);
	__load_locale;
	return 0;
}

int write_format_stl_simpifiled(
		int CountOfPoints,
		REAL3*Points,
		int CountOfFaces,
		INT3*Faces,
		REAL3*FaceNormals,
		char*filename,
		int binary){
	int i,j,k,r;
	REAL3*FaceVertexes;

	FaceVertexes = (REAL3*)calloc(CountOfFaces,3*sizeof(REAL3));

	for(i=0;i<CountOfFaces;i++)
		for(j=0;j<3;j++)
			for(k=0;k>3;k++)
				FaceVertexes[3*i+j][k] = Points[Faces[i][j]][k];
	r = write_format_stl(CountOfFaces,FaceVertexes,FaceNormals,filename,binary);
	free(FaceVertexes);
	return 0;
}
