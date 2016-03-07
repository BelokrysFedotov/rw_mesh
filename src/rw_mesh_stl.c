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
		REAL3 FaceNormals,
		char*filename,
		int binary){
	return 1;
}

int write_format_stl_simpifiled(
		int CountOfPoints,
		REAL3 Points,
		int CountOfFaces,
		INT3*Faces,
		REAL3 FaceNormals,
		char*filename,
		int binary){
	return 1;
}

//TODO add filter for comments
int read_format_off1(
			int*CountOfPoints, REAL**Points,int* PointsDimention,
			int*EnablePointsFunction,REAL**PointsFunction,
			int*EnablePointsNormal,REAL**PointsNormal,
			int*EnablePointsMaterial,int**PointsMaterial,
			int*EnablePointsColor,REAL4**PointsColor,
			int*EnablePointsTextureCoords,REAL2**PointsTextureCoords,
			int*CountOfFaces, int**Faces,int**FacesSizes,
			int*EnableFacesMaterial, int**FacesMaterial,
			int*EnableFacesColor,REAL4**FacesColor,
			char*filename){
	__save_locale;

	FILE*fd;
	int i,j,k;
	int a,b,c;
	int p,f;
	int rn;

	char line[256];
	char word[256];
	int current_line;
	int linepos;
	char line_error[256];

	int enablePointsTextureCoords;
	int enablePointsColor;
	int enablePointsColor_color;
	int enablePointsColor_material;
	int enablePointsNormal;
	int enablePoints4;
	int enablePointsn;
	int pointsDimention;

	int countOfPoints;
	REAL*points;
	REAL*pointsFunction;
	REAL*pointsNormal;
	int*pointsMaterial;
	REAL4*pointsColor;
	REAL2*pointsTextureCoords;

	int countOfFaces;
	int*faces;
	int faces_allocated;
	int*facesSizes;

	int enableFacesColor_color;
	int enableFacesColor_material;
	int*facesMaterial;
	REAL4*facesColor;

	int countOfEdges;

	points = NULL;
	pointsFunction = NULL;
	pointsNormal = NULL;
	pointsColor = NULL;
	pointsMaterial = NULL;
	pointsTextureCoords = NULL;

	faces = NULL;
	faces_allocated = 0;
	facesSizes = NULL;
	facesColor = NULL;
	facesMaterial = NULL;

	enablePointsColor_color = 0;
	enablePointsColor_material = 0;

#define _error_free_mem ffree(points);ffree(pointsFunction);ffree(pointsNormal);ffree(pointsColor);ffree(pointsMaterial);ffree(pointsTextureCoords);ffree(facesSizes);ffree(facesColor);ffree(facesMaterial);

	rw_mesh_set_filename(filename);
	fd=fopen(filename,"r");if(fd==NULL){
		rw_mesh_set_error(0,"Can't find file");
		return 1;
	}
	current_line = 0;

	//read [ST][C][N][4][n]OFF
	read_line_trim(fd,line); current_line++;
	linepos=0;
	//Check enablePointsTextureCoords
	enablePointsTextureCoords = 0;
	if(line[linepos]=='S'&&line[linepos+1]=='T'){
		enablePointsTextureCoords = 1;
		linepos+=2;
	}
	//Check enablePointsColor
	enablePointsColor = 0;
	if(line[linepos]=='C'){
		enablePointsColor = 1;
		linepos++;
	}
	//Check enablePointsNormal
	enablePointsNormal = 0;
	if(line[linepos]=='N'){
		enablePointsNormal = 1;
		linepos++;
	}
	//Check enablePoints4 / Function
	enablePoints4 = 0;
	if(line[linepos]=='4'){
		enablePoints4 = 1;
		linepos++;
	}
	//Check enablePointsn
	enablePointsn = 0;
	if(line[linepos]=='n'){
		enablePointsn = 1;
		linepos++;
	}

	if(line[linepos]=='O'&&line[linepos+1]=='F'&&line[linepos+2]=='F'){

	}else{
		//it isnt OFF header
		rw_mesh_set_error(linepos,"It isn't OFF header");
		fclose(fd);
		return 2;
	}

	read_line_trim(fd,line); current_line++;

	pointsDimention = 3;
	if(string_is_integer(line)){
		rn = sscanf(line,"%d",&pointsDimention);
		if(rn != 1){
			rw_mesh_set_error(linepos,"Can't read Ndim");
			fclose(fd);
			return 3;
		}
		if(pointsDimention<1){
			rw_mesh_set_error(linepos,"Ndim must be more than 0");
			fclose(fd);
			return 3;
		}
		//read next line
		read_line_trim(fd,line); current_line++;
	}else if(enablePointsn){
		// Dont set Ndim
		// Возможно просто пропускать этот факт и считать размерность==3
		rw_mesh_set_error(linepos,"Didn't set Ndim");
		fclose(fd);
		return 3;
	}

	// NVertices  NFaces  NEdges
	// already readed line
	//read_line_trim(fd,line);

	rn = sscanf(line,"%d %d %d",&countOfPoints,&countOfFaces,&countOfEdges);
	if(rn!=3){
		rw_mesh_set_error(linepos,"Can't read count of points, faces and edges.");
		fclose(fd);
		return 3;
	}

	if(countOfPoints>0){
		//reading points

		b = 0; //counts of non colored values
		points = (REAL*)calloc(pointsDimention*countOfPoints,sizeof(REAL));
		b+=pointsDimention;
		if(enablePoints4){
			pointsFunction = (REAL*)calloc(countOfPoints,sizeof(REAL));
			b++;
		}
		if(enablePointsNormal){
			pointsNormal = (REAL*)calloc(pointsDimention*countOfPoints,sizeof(REAL));
			b+=pointsDimention;
		}
		if(enablePointsColor){
			pointsMaterial = (int*)calloc(countOfPoints,sizeof(int));
			pointsColor = (REAL4*)calloc(countOfPoints,sizeof(REAL4));
		}
		if(enablePointsTextureCoords){
			pointsTextureCoords = (REAL2*)calloc(countOfPoints,sizeof(REAL2));
			b+=2;
		}

		for(p = 0; p < countOfPoints; p++){
			//read new point
			read_line_trim(fd,line); current_line++;

			c = string_count_of_words(line);

			if(
					(!enablePointsColor && b!=c) ||
					(enablePointsColor && !( (c==b+1) || (c==b+3) || (c==b+4) ) )
					){
				_error_free_mem
				if(enablePointsColor){
					sprintf(line_error,"Cant read data of points. It's must be %d,%d or %d values, but found %d.",b+1,b+3,b+4,c);
				}else{
					sprintf(line_error,"Cant read data of points. It's must be %d values, but finded %d.",b,c);
				}
				rw_mesh_set_error(linepos,line_error);
				fclose(fd);
				return 4;
			}

			for(k=0;k<pointsDimention;k++){
				string_cut_word(line,word);
				rn = sscanf(word,"%lf",points+p*pointsDimention+k);
				if(rn!=1){
					sprintf(line_error,"Can't read point's #%d coords %d: `%s`",p,k,word);
					rw_mesh_set_error(linepos,line_error);
					_error_free_mem
					fclose(fd);
					return 5;
				}
			}

			if(enablePoints4){
				string_cut_word(line,word);
				rn = sscanf(word,"%lf",pointsFunction+p);
				if(rn!=1){
					sprintf(line_error,"Can't read point's #%d function: `%s`",p,word);
					rw_mesh_set_error(linepos,line_error);
					_error_free_mem
					fclose(fd);
					return 6;
				}
			}

			if(enablePointsNormal){
				for(k=0;k<pointsDimention;k++){
					string_cut_word(line,word);
					rn = sscanf(word,"%lf",pointsNormal+p*pointsDimention+k);
					if(rn!=1){
						sprintf(line_error,"Can't read point's #%d normal's coords %d: `%s`",p,k,word);
						rw_mesh_set_error(linepos,line_error);
						_error_free_mem
						fclose(fd);
						return 7;
					}
				}
			}

			if(enablePointsColor){
				switch(c-b){
					case 1:
						string_cut_word(line,word);
						rn = sscanf(word,"%d",pointsMaterial+p);
						enablePointsColor_material = 1;
						if(rn!=1){
							sprintf(line_error,"Can't read point's #%d material: `%s`",p,word);
							rw_mesh_set_error(linepos,line_error);
							_error_free_mem
							fclose(fd);
							return 8;
						}
						break;
					case 3:
						for(k=0;k<3;k++){
							string_cut_word(line,word);
							rn = sscanf(word,"%lf",pointsColor[p]+k);
							if(rn!=1){
								sprintf(line_error,"Can't read point's #%d color's component %d: `%s`",p,k,word);
								rw_mesh_set_error(linepos,line_error);
								_error_free_mem
								fclose(fd);
								return 9;
							}
						}
						pointsColor[p][3] = 1.e0;// set default alpha
						enablePointsColor_color = 1;
						break;
					case 4:
						for(k=0;k<4;k++){
							string_cut_word(line,word);
							rn =sscanf(word,"%lf",pointsColor[p]+k);
							if(rn!=1){
								sprintf(line_error,"Can't read point's #%d color's component %d: `%s`",p,k,word);
								rw_mesh_set_error(linepos,line_error);
								_error_free_mem
								fclose(fd);
								return 10;
							}
						}
						enablePointsColor_color = 1;
						break;
				};

			}

			if(enablePointsTextureCoords){
				for(k=0;k<2;k++){
					string_cut_word(line,word);
					rn = sscanf(word,"%lf",pointsTextureCoords[p]+k);
					if(rn!=1){
						sprintf(line_error,"Can't read point's #%d texture coords %d: `%s`",p,k,word);
						rw_mesh_set_error(linepos,line_error);
						_error_free_mem
						fclose(fd);
						return 11;
					}
				}
			}

		};

	}

	if(countOfFaces>0){
		// reading Faces

		faces_allocated = 3*countOfFaces;
		faces = (int*)calloc(faces_allocated,sizeof(int));
		facesSizes = (int*)calloc(countOfFaces+1,sizeof(int));

		facesMaterial = (int*)calloc(countOfFaces,sizeof(int));
		facesColor = (REAL4*)calloc(countOfFaces,sizeof(REAL4));
		enableFacesColor_color = 0;
		enableFacesColor_material = 0;

		facesSizes[0] = 0;
		for(f = 0 ; f < countOfFaces; f++){
			read_line_trim(fd,line); current_line++;

			string_cut_word(line,word);

			rn = sscanf(word,"%d",&i); // size of face
			if(rn!=1){
				sprintf(line_error,"Can't read face's #%d size: `%s`",f,word);
				rw_mesh_set_error(linepos,line_error);
				_error_free_mem
				fclose(fd);
				return 12;
			}

			if(i<1){
				_error_free_mem
				fclose(fd);
				return 13;
			}

			facesSizes[f+1] = facesSizes[f]+i;

			if(facesSizes[f+1]>faces_allocated){
				faces_allocated = faces_allocated+countOfFaces;
				faces = (int*)realloc(faces,faces_allocated*sizeof(int));
			}

			for(k=0;k<i;k++){
				string_cut_word(line,word);
				rn = sscanf(word,"%d",faces+facesSizes[f]+k);
				if(rn!=1){
					sprintf(line_error,"Can't read face's #%d component %d: `%s`",f,k,word);
					rw_mesh_set_error(linepos,line_error);
					_error_free_mem
					fclose(fd);
					return 14;
				}
			}

			c = string_count_of_words(line);

			switch(c){
				case 1:
					enableFacesColor_material = 1;
					string_cut_word(line,word);
					rn = sscanf(word,"%d",facesMaterial+f);
					if(rn!=1){
						sprintf(line_error,"Can't read face's #%d material: `%s`",f,word);
						rw_mesh_set_error(linepos,line_error);
						_error_free_mem
						fclose(fd);
						return 15;
					}
					break;
				case 3:
					enableFacesColor_color = 1;
					for(k=0;k<3;k++){
						string_cut_word(line,word);
						rn = sscanf(word,"%lf",facesColor[f]+k);
						if(rn!=1){
							sprintf(line_error,"Can't read face's #%d color component %d: `%s`",f,k,word);
							rw_mesh_set_error(linepos,line_error);
							_error_free_mem
							fclose(fd);
							return 16;
						}
					}
					facesColor[f][3] = 1.e0;
					break;
				case 4:
					enableFacesColor_color = 1;
					for(k=0;k<4;k++){
						string_cut_word(line,word);
						rn = sscanf(word,"%lf",facesColor[f]+k);
						if(rn!=1){
							sprintf(line_error,"Can't read face's #%d color component %d: `%s`",f,k,word);
							rw_mesh_set_error(linepos,line_error);
							_error_free_mem
							fclose(fd);
							return 17;
						}
					}
					break;
			}

		}

		if(facesSizes[countOfFaces]!=faces_allocated){
			faces_allocated = facesSizes[countOfFaces];
			faces = (int*)realloc(faces,faces_allocated*sizeof(int));
		}

	}

	if(countOfPoints>0){
		if(CountOfPoints)*CountOfPoints = countOfPoints;
		if(Points){
			*Points = points;
		}else{
			ffree(points);
		}

		if(PointsDimention) *PointsDimention = pointsDimention;

		if(EnablePointsFunction) *EnablePointsFunction = enablePoints4;
		if(PointsFunction){
			*PointsFunction = pointsFunction;
		}else{
			ffree(pointsFunction);
		}

		if(EnablePointsNormal) *EnablePointsNormal = enablePointsNormal;
		if(PointsNormal){
			*PointsNormal = pointsNormal;
		}else{
			ffree(pointsNormal);
		}

		if(EnablePointsMaterial) *EnablePointsMaterial = enablePointsColor_material;
		if(PointsMaterial){
			*PointsMaterial = pointsMaterial;
		}else{
			ffree(pointsMaterial);
		}


		if(EnablePointsColor) *EnablePointsColor = enablePointsColor_color;
		if(PointsColor){
			*PointsColor = pointsColor;
		}else{
			ffree(pointsColor);
		}


		if(EnablePointsTextureCoords) *EnablePointsTextureCoords = enablePointsTextureCoords;
		if(PointsTextureCoords){
			*PointsTextureCoords = pointsTextureCoords;
		}else{
			ffree(pointsTextureCoords);
		}
	}

	if(countOfFaces>0){
		if(CountOfFaces) *CountOfFaces = countOfFaces;
		if(Faces){
			*Faces = faces;
		}else{
			ffree(faces);
		}
		if(FacesSizes){
			*FacesSizes = facesSizes;
		}else{
			ffree(facesSizes);
		}

		if(EnableFacesMaterial) *EnableFacesMaterial = enableFacesColor_material;
		if(FacesMaterial){
			*FacesMaterial = facesMaterial;
		}else{
			ffree(facesMaterial);
		}

		if(EnableFacesColor) *EnableFacesColor = enableFacesColor_color;
		if(FacesColor){
			*FacesColor = facesColor;
		}else{
			free(facesColor);
		}
	}

	__load_locale;

	fclose(fd);

#undef _error_free_mem

	return 0;
}

/**
 * Write data to file in format OFF
 * ! If set PointsMaterial, PointsColor is ignored
 * ! If set FacesMaterial, FacesColor is ignored
 */
int write_format_off1(
		int CountOfPoints, REAL*Points,int PointsDimention,
		REAL*PointsFunction,REAL*PointsNormal,int*PointsMaterial,REAL4*PointsColor,REAL2*PointsTextureCoords,
		int CountOfFaces, int*Faces,int*FacesSizes,
		int*FacesMaterial,REAL4*FacesColor,
		char*filename){

	__save_locale;

	FILE*fd;
	int i,j,k;
	int a,b,c;
	int p,f;

	char line[256];

	rw_mesh_set_filename(filename);
	fd=fopen(filename,"w");if(fd==NULL){
		rw_mesh_set_error(0,"Can't find file");
		return 1;
	}

	strcpy(line,"");
	// [ST][C][N][4][n]OFF
	if(PointsTextureCoords)
		strcat(line,"ST");
	if(PointsMaterial || PointsColor)
		strcat(line,"C");
	if(PointsNormal)
		strcat(line,"N");
	if(PointsFunction)
		strcat(line,"4");
	if(PointsDimention!=3)
		strcat(line,"n");

	fprintf(fd,"%s\n",line);

	if(PointsDimention!=3)
		fprintf(fd,"%d\n",PointsDimention);

	fprintf(fd,"%d %d %d\n",CountOfPoints,CountOfFaces,0);

	if(CountOfPoints)for(p=0;p<CountOfPoints;p++){
		for(k=0;k<PointsDimention;k++)
			fprintf(fd,"%lf ",Points[p*PointsDimention+k]);
		if(PointsFunction)
			fprintf(fd,"%lf ",PointsFunction[p]);
		if(PointsNormal)
			for(k=0;k<PointsDimention;k++)
				fprintf(fd,"%lf ",PointsNormal[p*PointsDimention+k]);
		if(PointsMaterial){
			fprintf(fd,"%d ",PointsMaterial[p]);
		}else if(PointsColor){
			for(k=0;k<4;k++)
				fprintf(fd,"%lf ",PointsColor[p][k]);
		}

		if(PointsTextureCoords)
			for(k=0;k<2;k++)
				fprintf(fd,"%lf ",PointsTextureCoords[p][k]);
		fprintf(fd,"\n");
	}

	if(CountOfFaces)for(f=0;f<CountOfFaces;f++){
		fprintf(fd,"%d",FacesSizes[f+1]-FacesSizes[f]);
		for(k=0;k<FacesSizes[f+1]-FacesSizes[f];k++)
			fprintf(fd," %d",Faces[FacesSizes[f]+k]);
		if(FacesMaterial){
			fprintf(fd,"%d ",FacesMaterial[f]);
		}else if(FacesColor){
			for(k=0;k<4;k++)
				fprintf(fd,"%lf ",FacesColor[p][k]);
		}
	}

	fclose(fd);
	__load_locale;

	return 0;
}
