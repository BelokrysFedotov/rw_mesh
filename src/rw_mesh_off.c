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

int read_format_off(
			int*CountOfPoints, REAL**Points,int* PointsDimention,
			int*EnablePointsFunction,REAL**PointsFunction,
			int*EnablePointsNormals,REAL**PointsNormals,
			int*EnablePointsMaterial,int**PointsMaterial,
			int*EnablePointsColor,REAL4**PointsColor,
			int*EnablePointsTextureCoords,REAL2**PointsTextureCoords,
			int*CountOfFaces, int**Faces,int**FaceSizes,
			int*EnableFacesMaterial, int**FacesMaterials,
			int*EnableFacesColors,REAL4**FacesColors,
			char*filename){
	__save_locale;

	FILE*fd;
	int i,j,k;
	int a,b,c;
	int p,f;
	REAL x,y,z;

	char line[256];
	char word[256];
	int current_line;
	int linepos;
	char line_error[256];

	int enablePointsTextureCoords;
	int enablePointsColor;
	int enablePointsColor_color;
	int enablePointsColor_material;
	int enablePointsNormals;
	int enablePoints4;
	int enablePointsn;
	int pointsDimention;

	int countOfPoints;
	REAL*points;
	REAL*pointsFunction;
	REAL*pointsNormals;
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
	//Check enablePointsNormals
	enablePointsNormals = 0;
	if(line[linepos]=='N'){
		enablePointsNormals = 1;
		linepos++;
	}
	//Check enablePoints4
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
		rw_mesh_set_error(linepos,"It isnt OFF header");
		return 2;
	}

	read_line_trim(fd,line); current_line++;

	pointsDimention = 3;
	if(string_is_integer(line)){
		pointsDimention = atoi(line);
		if(pointsDimention<1){
			rw_mesh_set_error(linepos,"Ndim < 1");
			return 3;
		}
		//read next line
		read_line_trim(fd,line); current_line++;
	}else if(enablePointsn){
		// Dont set Ndim
		// Возможно просто пропускать этот факт и считать размерность==3
		rw_mesh_set_error(linepos,"Dont set Ndim");
		return 3;
	}

	// NVertices  NFaces  NEdges
	// already readed line
	//read_line_trim(fd,line);

	i = sscanf(line,"%d %d %d",&countOfPoints,&countOfFaces,&countOfEdges);
	if(i!=3){
		rw_mesh_set_error(linepos,"Cant read count of points, faces and edges");
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
		if(enablePointsNormals){
			pointsNormals = (REAL*)calloc(pointsDimention*countOfPoints,sizeof(REAL));
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
			read_line_trim(fd,line); current_line++;

			c = string_count_of_words(line);

			if(
					(!enablePointsColor && b!=c) ||
					(enablePointsColor && ( (c==b+1) || (c==b+3) || (c==b+4) ) )
					){
				free(points);
				if(enablePoints4)free(pointsFunction);
				if(enablePointsNormals)free(pointsNormals);
				if(enablePointsColor){free(pointsColor);free(pointsMaterial);}
				if(enablePointsTextureCoords)free(pointsTextureCoords);
				if(enablePointsColor){
					sprintf(line_error,"Cant read data of points. It's must be %d,%d or %d values, but finded %d.",b+1,b+3,b+4,c);
				}else{
					sprintf(line_error,"Cant read data of points. It's must be %d values, but finded %d.",b,c);
				}
				rw_mesh_set_error(linepos,line_error);
				return 3;
			}

			for(k=0;k<pointsDimention;k++){
				string_cut_word(line,word);
//				points[p*pointsDimention+k] = atof(word);
				sscanf(word,"%lf",points+p*pointsDimention+k);
			}

			if(enablePoints4){
				string_cut_word(line,word);
				sscanf(word,"%lf",pointsFunction+p);
			}

			if(enablePointsNormals){
				for(k=0;k<pointsDimention;k++){
					string_cut_word(line,word);
					sscanf(word,"%lf",pointsNormals+p*pointsDimention+k);
				}
			}

			if(enablePointsColor){
				switch(c-b){
					case 1:
						string_cut_word(line,word);
						sscanf(word,"%d",pointsMaterial+p);
						enablePointsColor_material = 1;
						break;
					case 3:
						for(k=0;k<3;k++){
							string_cut_word(line,word);
							sscanf(word,"%lf",pointsColor[p]+k);
						}
						pointsColor[p][3] = 1.e0;// set default alpha
						enablePointsColor_color = 1;
						break;
					case 4:
						for(k=0;k<4;k++){
							string_cut_word(line,word);
							sscanf(word,"%lf",pointsColor[p]+k);
						}
						enablePointsColor_color = 1;
						break;
				};

			}

			if(enablePointsTextureCoords){
				for(k=0;k<2;k++){
					string_cut_word(line,word);
					sscanf(word,"%lf",pointsTextureCoords[p]+k);
				}
			}

		};

		if(CountOfPoints)*CountOfPoints = countOfPoints;
		if(Points){
			*Points = points;
		}else{
			free(points);
		}

		if(PointsDimention) *PointsDimention = pointsDimention;

		if(EnablePointsFunction) *EnablePointsFunction = enablePoints4;
		if(PointsFunction){
			*PointsFunction = pointsFunction;
		}else if(enablePoints4){
			free(pointsFunction);
		}

		if(EnablePointsNormals) *EnablePointsNormals = enablePointsNormals;
		if(PointsNormals){
			*PointsNormals = pointsNormals;
		}else if(enablePointsNormals){
			free(pointsNormals);
		}

		if(EnablePointsMaterial) *EnablePointsMaterial = enablePointsColor_material;
		if(PointsMaterial){
			*PointsMaterial = pointsMaterial;
		}else if(enablePointsColor){
			free(pointsMaterial);
		}


		if(EnablePointsColor) *EnablePointsColor = enablePointsColor_color;
		if(PointsColor){
			*PointsColor = pointsColor;
		}else if(enablePointsColor){
			free(pointsColor);
		}


		if(EnablePointsTextureCoords) *EnablePointsTextureCoords = enablePointsTextureCoords;
		if(PointsTextureCoords){
			*PointsTextureCoords = pointsTextureCoords;
		}else if(enablePointsTextureCoords){
			free(pointsTextureCoords);
		}

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

			string_cut_word(line,word);//TODO check

			sscanf(word,"%d",&i); // size of face

			if(i<=0){
				//TODO check
				return 1;
			}

			facesSizes[f+1] = facesSizes[f]+i;

			if(facesSizes[f+1]>faces_allocated){
				faces_allocated = faces_allocated+countOfFaces;
				faces = (int*)realloc(faces,faces_allocated*sizeof(int));
			}

			for(k=0;k<i;k++){
				string_cut_word(line,word);//TODO check
				sscanf(word,"%d",faces+facesSizes[f]+k);
			}

			c = string_count_of_words(line);

			switch(c){
				case 1:
					enableFacesColor_material = 1;
					string_cut_word(line,word);
					sscanf(word,"%d",facesMaterial+f);
					break;
				case 3:
					enableFacesColor_color = 1;
					for(k=0;k<3;k++){
						string_cut_word(line,word);
						sscanf(word,"%lf",facesColor[f]+k);
					}
					facesColor[f][3] = 1.e0;
					break;
				case 4:
					enableFacesColor_color = 1;
					for(k=0;k<4;k++){
						string_cut_word(line,word);
						sscanf(word,"%lf",facesColor[f]+k);
					}
					break;
			}

		}

		if(facesSizes[countOfFaces+1]!=faces_allocated){
			faces_allocated = facesSizes[countOfFaces+1];
			faces = (int*)realloc(faces,faces_allocated*sizeof(int));
		}

		if(CountOfFaces) *CountOfFaces = countOfFaces;
		if(Faces){
			*Faces = faces;
		}else{
			free(faces);
		}
		if(FaceSizes){
			*FaceSizes = facesSizes;
		}else{
			free(facesSizes);
		}

		if(EnableFacesMaterial) *EnableFacesMaterial = enableFacesColor_material;
		if(FacesMaterials){
			*FacesMaterials = facesMaterial;
		}else{
			free(facesMaterial);
		}

		if(EnableFacesColors) *EnableFacesColors = enableFacesColor_color;
		if(FacesColors){
			*FacesColors = facesColor;
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
