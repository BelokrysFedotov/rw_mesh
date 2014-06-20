/*
 * format_off.c
 *
 * Format OFF
 *
 *  Created on: 15.12.2011
 *      Author: moric
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "rw_mesh_off.h"


//TODO add filter for comments
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

	faces = NULL;
	faces_allocated = 0;
	facesSizes = NULL;
	facesColor = NULL;
	facesMaterial = NULL;

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
		return 2;
	}

	read_line_trim(fd,line); current_line++;

	pointsDimention = 3;
	if(string_is_integer(line)){
		rn = sscanf(line,"%d",&pointsDimention);
		if(rn != 1){
			rw_mesh_set_error(linepos,"Can't read Ndim");
			return 3;
		}
		if(pointsDimention<1){
			rw_mesh_set_error(linepos,"Ndim must be more than 0");
			return 3;
		}
		//read next line
		read_line_trim(fd,line); current_line++;
	}else if(enablePointsn){
		// Dont set Ndim
		// Возможно просто пропускать этот факт и считать размерность==3
		rw_mesh_set_error(linepos,"Didn't set Ndim");
		return 3;
	}

	// NVertices  NFaces  NEdges
	// already readed line
	//read_line_trim(fd,line);

	rn = sscanf(line,"%d %d %d",&countOfPoints,&countOfFaces,&countOfEdges);
	if(rn!=3){
		rw_mesh_set_error(linepos,"Can't read count of points, faces and edges.");
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
			pointsMaterial = (int*)calloc(pointsDimention*countOfPoints,sizeof(int));
			pointsColor = (REAL4*)calloc(pointsDimention*countOfPoints,sizeof(REAL4));
			enablePointsColor_color = 0;
			enablePointsColor_material = 0;
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
					(enablePointsColor && ( (c==b+1) || (c==b+3) || (c==b+4) ) )
					){
				_error_free_mem
				if(enablePointsColor){
					sprintf(line_error,"Cant read data of points. It's must be %d,%d or %d values, but found %d.",b+1,b+3,b+4,c);
				}else{
					sprintf(line_error,"Cant read data of points. It's must be %d values, but finded %d.",b,c);
				}
				rw_mesh_set_error(linepos,line_error);
				return 4;
			}

			for(k=0;k<pointsDimention;k++){
				string_cut_word(line,word);
				rn = sscanf(word,"%lf",points+p*pointsDimention+k);
				if(rn!=1){
					sprintf(line_error,"Can't read point's #%d coords %d: `%s`",p,k,word);
					rw_mesh_set_error(linepos,line_error);
					_error_free_mem
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
				return 12;
			}

			if(i<1){
				_error_free_mem
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
							return 17;
						}
					}
					break;
			}

		}

		if(facesSizes[countOfFaces+1]!=faces_allocated){
			faces_allocated = facesSizes[countOfFaces+1];
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
		}else if(enablePoints4){
			ffree(pointsFunction);
		}

		if(EnablePointsNormal) *EnablePointsNormal = enablePointsNormal;
		if(PointsNormal){
			*PointsNormal = pointsNormal;
		}else if(enablePointsNormal){
			ffree(pointsNormal);
		}

		if(EnablePointsMaterial) *EnablePointsMaterial = enablePointsColor_material;
		if(PointsMaterial){
			*PointsMaterial = pointsMaterial;
		}else if(enablePointsColor){
			ffree(pointsMaterial);
		}


		if(EnablePointsColor) *EnablePointsColor = enablePointsColor_color;
		if(PointsColor){
			*PointsColor = pointsColor;
		}else if(enablePointsColor){
			ffree(pointsColor);
		}


		if(EnablePointsTextureCoords) *EnablePointsTextureCoords = enablePointsTextureCoords;
		if(PointsTextureCoords){
			*PointsTextureCoords = pointsTextureCoords;
		}else if(enablePointsTextureCoords){
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

	return 0;
}




/*

int read_format_off3(int*CountOfPoints,REAL3**Points, int*CountOfFaces, INT3**Faces,char*filename){
	__save_locale;

	FILE*fd;
	int i,j,k,a;
	REAL x,y,z;
	char format[128];

	fd=fopen(filename,"r");if(fd==NULL)return 1;

	if(fscanf(fd,"%s\n",format)<1)return 1;
	if(strcmp(format,"OFF"))return 1;

	if(fscanf(fd,"%d %d %d\n",CountOfPoints,CountOfFaces,&i)<3)return 1;
	if(!CountOfPoints) return 1;
	if(!CountOfFaces) return 1;

	*Points=(REAL3*)malloc(*CountOfPoints*sizeof(REAL3));
	if(!Points) return 1;

	for(i=0;i<*CountOfPoints;i++){
		k=fscanf(fd,"%lf %lf %lf\n",&x,&y,&z);
		(*Points)[i][0]=x;
		(*Points)[i][1]=y;
		(*Points)[i][2]=z;
	}

	*Faces=(INT3*)malloc(*CountOfFaces*sizeof(INT3));
	if(!Faces)return 1;

	for(i=0;i<*CountOfFaces;i++){
		fscanf(fd,"%d",&j);
		if(j!=3)return 1;

		for(k=0;k<j;k++){
			fscanf(fd,"%d",&a);
			(*Faces)[i][k]=a;
		}
	}
	fclose(fd);
	__load_locale;
	return 0;
}

int write_format_off3(int CountOfPoints,REAL3*Points, int CountOfFaces, INT3*Faces,char*filename){
	__save_locale;

	FILE*fd;
	int i;

	fd=fopen(filename,"w");if(fd==NULL)return 1;

	fprintf(fd,"OFF\n");
	fprintf(fd,"%d %d 0\n",CountOfPoints,CountOfFaces);

	for(i=0;i<CountOfPoints;i++){
		fprintf(fd,"%lf %lf %lf\n",Points[i][0],Points[i][1],Points[i][2]);
	}

	for(i=0;i<CountOfFaces;i++){
		fprintf(fd,"3 %d %d %d\n",Faces[i][0],Faces[i][1],Faces[i][2]);
	}

	fclose(fd);
	__load_locale;
	return 0;
}

int write_format_coff3(int CountOfPoints,REAL3*Points,REAL*PointsFunctoion, int CountOfFaces, INT3*Faces,char*filename){
	__save_locale;

	FILE*fd;
	int i;

	fd=fopen(filename,"w");if(fd==NULL)return 1;

	fprintf(fd,"COFF\n");
	fprintf(fd,"%d %d 0\n",CountOfPoints,CountOfFaces);

	for(i=0;i<CountOfPoints;i++){
		fprintf(fd,"%lf %lf %lf %lf\n",Points[i][0],Points[i][1],Points[i][2],PointsFunctoion[i]);
	}

	for(i=0;i<CountOfFaces;i++){
		fprintf(fd,"3 %d %d %d\n",Faces[i][0],Faces[i][1],Faces[i][2]);
	}

	fclose(fd);
	__load_locale;
	return 0;
}*/
