/*
 * rw_mesh_neu.c
 *
 *  Created on: 03.02.2016
 *      Author: Belokrys-Fedotov A.I.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rw_mesh_neu.h"

#define NEU_CELL_TYPE_EDGE 1
#define NEU_CELL_TYPE_QUAD 2
#define NEU_CELL_TYPE_TRIANGLE 3
#define NEU_CELL_TYPE_BRICK 4
#define NEU_CELL_TYPE_WEDGE 5
#define NEU_CELL_TYPE_TETRA 6
#define NEU_CELL_TYPE_PYRAMID 7

#define NEU_VERSION "1.2.1"

int _cp[5] = {0,1,3,2,4};
int _cw[6] = {0,2,1,3,5,4};
//int _cw[6] = {0,1,2,3,4,5};
int _ch[8] = {0,1,3,2,4,5,7,6};

/**
 * Инициализация структуры neu_mesh_struct
 */
void neu_mesh_struct_init(struct neu_mesh_struct*Mesh){
	Mesh->CountOfPoints = 0;
	Mesh->Points = NULL;
	Mesh->Dimension = 3;

	Mesh->CountOfCells = 0;
	Mesh->Cells = NULL;
	Mesh->CellTypes = NULL;
	Mesh->CellSizes = NULL;
	Mesh->CellOffset = NULL;

	Mesh->CountOfBoundaryConditions = 0;
	Mesh->BoundaryConditions = NULL;
}

/**
 * Очистка структуры neu_mesh_struct
 */
void neu_mesh_struct_free(struct neu_mesh_struct*Mesh){
	ffree(Mesh->Points);
	ffree(Mesh->Cells);
	ffree(Mesh->CellTypes)
	ffree(Mesh->CellSizes);
	ffree(Mesh->CellOffset);
	ffree(Mesh->BoundaryConditions);
	neu_mesh_struct_init(Mesh);
}

/**
 * Добавить в структуру neu_mesh_struct данные о точках сетки
 * Mesh - ссылка на структуру сетки, предполагается, что в ней точки не заданы
 * Dimension - размерность точек, допустимы значения 2 или 3
 * CountOfPoints - число точек
 * Points - массив точек размерностью CountOfPoints*Dimension
 */
int neu_mesh_struct_set_mesh_points(struct neu_mesh_struct*Mesh, int Dimension, int CountOfPoints, REAL*Points){
	if(Dimension==2 || Dimension==3){
		Mesh->Dimension = Dimension;
	}else if(Dimension==0){
		Mesh->Dimension = 3;
	}else{
		return -1;
	}

	if(CountOfPoints<=0 || Points==NULL){
		return -2;
	}

	Mesh->CountOfPoints = CountOfPoints;
	Mesh->Points = (REAL*)calloc(Mesh->CountOfPoints*Mesh->Dimension,sizeof(REAL));
	if(Mesh->Points==NULL)
		return -3;
	memcpy(Mesh->Points,Points,Mesh->CountOfPoints*Mesh->Dimension*sizeof(REAL));
	return 0;
}

/**
 * Добавить в структуру neu_mesh_struct данные об ячейках сетки
 * Mesh - ссылка на структуру сетки, предполагается, что в ней ячейки не заданы
 * CountOfCells - кол-во ячеек
 * Cells - массив ячеек размерностью CellOffset[CountOfCells]
 * CellTypes - типы ячеек
 * CellSizes - размеры ячеек
 * CellOffset - смещения ячеек
 */
int neu_mesh_struct_set_mesh_cells(struct neu_mesh_struct*Mesh, int CountOfCells, int*Cells, int*CellTypes, int*CellSizes, int*CellOffset){
	if(CountOfCells){
		if(Cells==NULL || CellTypes==NULL || CellSizes==NULL || CellOffset==NULL){
			return -4;
		}

		Mesh->CountOfCells = CountOfCells;
		Mesh->Cells = (int*)calloc(CellOffset[CountOfCells],sizeof(int));
		Mesh->CellTypes = (int*)calloc(CountOfCells,sizeof(int));
		Mesh->CellSizes = (int*)calloc(CountOfCells,sizeof(int));
		Mesh->CellOffset = (int*)calloc(CountOfCells+1,sizeof(int));

		memcpy(Mesh->Cells,Cells,CellOffset[CountOfCells]*sizeof(int));
		memcpy(Mesh->CellTypes,Cells,CountOfCells*sizeof(int));
		memcpy(Mesh->CellSizes,Cells,CountOfCells*sizeof(int));
		memcpy(Mesh->CellOffset,Cells,(CountOfCells+1)*sizeof(int));
	}
	return 0;
}

/**
 * Добавить в структуру neu_mesh_struct данные об ячейках однородной сетки
 * Mesh - ссылка на структуру сетки, предполагается, что в ней ячейки не заданы
 * CountOfCells - кол-во ячеек
 * Cells - массив ячеек размерностью CountOfCells * sizeof{CellType}
 * CellType - тип ячеек
 */
int neu_mesh_struct_set_mesh_cells_uniform(struct neu_mesh_struct*Mesh, int CountOfCells, int*Cells, int CellType){
	int i,size;
	if(CountOfCells){
		if(Cells==NULL){
			return -1;
		}

		size = rw_mesh_cell_type_size(CellType);
		if(size<=0)return -1;

		Mesh->CountOfCells = CountOfCells;
		Mesh->Cells = (int*)calloc(CountOfCells*size,sizeof(int));
		Mesh->CellTypes = (int*)calloc(CountOfCells,sizeof(int));
		Mesh->CellSizes = (int*)calloc(CountOfCells,sizeof(int));
		Mesh->CellOffset = (int*)calloc(CountOfCells+1,sizeof(int));

		Mesh->CellOffset[0] = 0;
		for(i=0;i<CountOfCells;i++){
			Mesh->CellTypes[i] = CellType;
			Mesh->CellSizes[i] = size;
			Mesh->CellOffset[i+1] = Mesh->CellOffset[i] + size;
		}

		memcpy(Mesh->Cells,Cells,CountOfCells*size*sizeof(int));
	}
	return 0;
}

/**
 * Добавить в структуру neu_mesh_struct данные о сетке
 * Mesh - ссылка на структуру сетки, предполагается, что в ней точки не заданы
 * Dimension - размерность точек, допустимы значения 2 или 3
 * CountOfPoints - число точек
 * Points - массив точек размерностью CountOfPoints*Dimension
 * CountOfCells - кол-во ячеек
 * Cells - массив ячеек размерностью CellOffset[CountOfCells]
 * CellTypes - типы ячеек
 * CellSizes - размеры ячеек
 * CellOffset - смещения ячеек
 */
int neu_mesh_struct_set_mesh(struct neu_mesh_struct*Mesh, int Dimension, int CountOfPoints, REAL*Points,
		int CountOfCells, int*Cells, int*CellTypes, int*CellSizes, int*CellOffset){
	int r;
	r = neu_mesh_struct_set_mesh_points(Mesh,Dimension,CountOfPoints,Points);
	if(r) return r;

	r = neu_mesh_struct_set_mesh_cells(Mesh,CountOfCells,Cells,CellTypes,CellSizes,CellOffset);
	if(r) return r;

	return 0;
}

/**
 * Добавить в структуру neu_mesh_struct данные о однородной сетке
 * Mesh - ссылка на структуру сетки, предполагается, что в ней точки не заданы
 * Dimension - размерность точек, допустимы значения 2 или 3
 * CountOfPoints - число точек
 * Points - массив точек размерностью CountOfPoints*Dimension
 * CountOfCells - кол-во ячеек
 * Cells - массив ячеек размерностью CountOfCells * sizeof{CellType}
 * CellType - тип ячеек
 */
int neu_mesh_struct_set_mesh_uniform(struct neu_mesh_struct*Mesh, int Dimension, int CountOfPoints, REAL*Points,
		int CountOfCells, int*Cells, int CellType){
	int r;
	r = neu_mesh_struct_set_mesh_points(Mesh,Dimension,CountOfPoints,Points);
	if(r) return r;

	r = neu_mesh_struct_set_mesh_cells_uniform(Mesh,CountOfCells,Cells,CellType);
	if(r) return r;

	return 0;
}

/**
 * Добавить в структуру neu_mesh_struct данные о граничых условиях
 * 	граничные условия добавляются к текущим
 * Mesh - ссылка на структуру сетки
 * CountOfBoundaryConditions - число граничных условий
 * ConditionNumber - нормера граничных условий
 * CountOfCells - число ячеек в каждом граничном условии
 * Cells - ячейки граничных условий
 * Faces - грани ячеек граничных условий
 */
int neu_mesh_struct_add_boundary_conditions(struct neu_mesh_struct*Mesh, int CountOfBoundaryConditions, int*ConditionNumber,int*CountOfCells, int**Cells,int**Faces){
	int i;
	struct neu_boundary_condition_struct*B;

	if(CountOfBoundaryConditions<0)return -1;
	if(CountOfBoundaryConditions==0)return 0;

	if(Mesh->CountOfBoundaryConditions){
		Mesh->BoundaryConditions = (struct neu_boundary_condition_struct*)realloc(Mesh->BoundaryConditions,(Mesh->CountOfBoundaryConditions+CountOfBoundaryConditions)*sizeof(struct neu_boundary_condition_struct));
		B = Mesh->BoundaryConditions+Mesh->CountOfBoundaryConditions;
		Mesh->CountOfBoundaryConditions+=CountOfBoundaryConditions;
	}else{
		Mesh->BoundaryConditions = (struct neu_boundary_condition_struct*)calloc(CountOfBoundaryConditions,sizeof(struct neu_boundary_condition_struct));
		B = Mesh->BoundaryConditions;
		Mesh->CountOfBoundaryConditions =CountOfBoundaryConditions;
	}
	if(Mesh->BoundaryConditions==NULL)
		return -1;

	for(i=0;i<CountOfBoundaryConditions;i++){
		B[i].Condition = ConditionNumber[i];
		B[i].CountOfCells = CountOfCells[i];
		B[i].Cells = (int*)calloc(B[i].CountOfCells,sizeof(int));
		if(B[i].Cells==NULL)return -1;
		memcpy(B[i].Cells,Cells[i],B[i].CountOfCells*sizeof(int));
		B[i].Faces = (int*)calloc(B[i].CountOfCells,sizeof(int));
		if(B[i].Faces==NULL)return -1;
		memcpy(B[i].Faces,Faces[i],B[i].CountOfCells*sizeof(int));
	}
	return 0;
}

/**
 * Добавить в структуру neu_mesh_struct данные о граничых условиях из массива стуктур граничных условий
 * 	граничные условия добавляются к текущим
 * Mesh - ссылка на структуру сетки
 * CountOfBoundaryConditions - число граничных условий
 * BoundaryConditions - массив структур граничных условий
 */
int neu_mesh_struct_add_boundary_conditions_struct(struct neu_mesh_struct*Mesh, int CountOfBoundaryConditions, struct neu_boundary_condition_struct*BoundaryConditions){
	int i;
	struct neu_boundary_condition_struct*B;

	if(CountOfBoundaryConditions<0)return -1;
	if(CountOfBoundaryConditions==0)return 0;

	if(Mesh->CountOfBoundaryConditions){
		Mesh->BoundaryConditions = (struct neu_boundary_condition_struct*)realloc(Mesh->BoundaryConditions,(Mesh->CountOfBoundaryConditions+CountOfBoundaryConditions)*sizeof(struct neu_boundary_condition_struct));
		B = Mesh->BoundaryConditions+Mesh->CountOfBoundaryConditions;
		Mesh->CountOfBoundaryConditions+=CountOfBoundaryConditions;
	}else{
		Mesh->BoundaryConditions = (struct neu_boundary_condition_struct*)calloc(CountOfBoundaryConditions,sizeof(struct neu_boundary_condition_struct));
		B = Mesh->BoundaryConditions;
		Mesh->CountOfBoundaryConditions =CountOfBoundaryConditions;
	}
	if(Mesh->BoundaryConditions==NULL)
		return -1;

	for(i=0;i<CountOfBoundaryConditions;i++){
		B[i].Condition = BoundaryConditions[i].Condition;
		B[i].CountOfCells = BoundaryConditions[i].CountOfCells;
		B[i].Cells = (int*)calloc(B[i].CountOfCells,sizeof(int));
		if(B[i].Cells==NULL)return -1;
		memcpy(B[i].Cells,BoundaryConditions[i].Cells,B[i].CountOfCells*sizeof(int));
		B[i].Faces = (int*)calloc(B[i].CountOfCells,sizeof(int));
		if(B[i].Faces==NULL)return -1;
		memcpy(B[i].Faces,BoundaryConditions[i].Faces,B[i].CountOfCells*sizeof(int));
	}
	return 0;
}

/**
 * Записать сетку Mesh в файл filename в формате neu
 */
int write_format_neu_struct(struct neu_mesh_struct*Mesh, char*filename){
	int i,j,k;
	FILE*fd;
	char buf[256];
	char FILE_TITLE[] = "RW_MESH_LIB FILE";
	char FILE_PROGRAM[] = "RW_MESH_LIB";
	float FILE_VERSION = 1.0;
	char FILE_TIME[] = "";
	
	if(Mesh->Dimension!=2 && Mesh->Dimension!=3){
		rw_mesh_set_error(0,"Incorrect dimension, must be 2 or 3");
		return 1;
	}

	fd = fopen (filename, "w");
    fprintf(fd,"%20s%20s\n", "CONTROL INFO", NEU_VERSION);
    fprintf(fd,"%s","** GAMBIT NEUTRAL FILE\n");
    fprintf(fd,"%20s\n",FILE_TITLE);
    fprintf(fd,"PROGRAM:  %20s     VERSION:  %5.2f\n",FILE_PROGRAM,FILE_VERSION);
    fprintf(fd,"%s\n",FILE_TIME);
    fprintf(fd,"%10s%10s%10s%10s%10s%10s\n","NUMNP","NELEM","NGRPS","NBSETS","NDFCD","NDFVL");
    fprintf(fd," %9d",Mesh->CountOfPoints);
    fprintf(fd," %9d",Mesh->CountOfCells);
    fprintf(fd," %9d",1); // Number of element groups
    fprintf(fd," %9d",Mesh->CountOfBoundaryConditions); //Number of boundary condition sets
    fprintf(fd," %9d",Mesh->Dimension); // Number of coordinate directions (2 or 3)
    fprintf(fd," %9d\n",3); // Number of velocity components (2 or 3)
    fprintf(fd,"ENDOFSECTION\n");

    if(Mesh->CountOfPoints){
		fprintf(fd,"%20s\n","NODAL COORDINATES");
		for(i=0;i<Mesh->CountOfPoints;i++){
			if(Mesh->Dimension==2){
				fprintf(fd,"%10d%20.11e%20.11e\n",i+1,Mesh->Points[i*2],Mesh->Points[i*2+1]);
			}else{
				fprintf(fd,"%10d%20.11e%20.11e%20.11e\n",i+1,Mesh->Points[i*3],Mesh->Points[i*3+1],Mesh->Points[i*3+2]);
			}
		}
		fprintf(fd,"ENDOFSECTION\n");
    }

    if(Mesh->CountOfCells){
		fprintf(fd,"%20s\n","ELEMENTS/CELLS");
		for(i=0;i<Mesh->CountOfCells;i++){

			fprintf(fd,"%8d",i+1);
			switch(Mesh->CellTypes[i]){

				case RW_MESH_CELL_TYPE_TETRA:
					fprintf(fd,"%3d",NEU_CELL_TYPE_TETRA);
					fprintf(fd,"%3d ",4);
					for(k=0;k<4;k++)
						fprintf(fd,"%8d",Mesh->Cells[Mesh->CellOffset[i]+k]+1);
					break;

				case RW_MESH_CELL_TYPE_PYRAMID:
					fprintf(fd,"%3d",NEU_CELL_TYPE_PYRAMID);
					fprintf(fd,"%3d ",5);
					for(k=0;k<5;k++)
						fprintf(fd,"%8d",Mesh->Cells[Mesh->CellOffset[i]+_cp[k]]+1);
					break;

				case RW_MESH_CELL_TYPE_WEDGE:
					fprintf(fd,"%3d",NEU_CELL_TYPE_WEDGE);
					fprintf(fd,"%3d ",6);
					for(k=0;k<6;k++)
						fprintf(fd,"%8d",Mesh->Cells[Mesh->CellOffset[i]+_cw[k]]+1);
					break;

				case RW_MESH_CELL_TYPE_HEXAHEDRON:
					fprintf(fd,"%3d",NEU_CELL_TYPE_BRICK);
					fprintf(fd,"%3d ",8);
					for(k=0;k<8;k++){
						fprintf(fd,"%8d",Mesh->Cells[Mesh->CellOffset[i]+_ch[k]]+1);
						if(k==6)fprintf(fd,"\n               ");
					}
					break;

				case RW_MESH_CELL_TYPE_VOXEL:
					fprintf(fd,"%3d",NEU_CELL_TYPE_BRICK);
					fprintf(fd,"%3d ",8);
					for(k=0;k<8;k++){
						fprintf(fd,"%8d",Mesh->Cells[Mesh->CellOffset[i]+k]+1);
						if(k==6)fprintf(fd,"\n               ");
					}

					break;
				case RW_MESH_CELL_TYPE_NONE:
					break;
				default:
					sprintf(buf,"write neu file: unknown cell type %d\n",Mesh->CellTypes[i]);
					rw_mesh_set_error(0,buf);
					return 1;
					break;
			}
			fprintf(fd,"\n");
		}
		fprintf(fd,"ENDOFSECTION\n");

		fprintf(fd,"%20s\n","ELEMENT GROUP");
		fprintf(fd,"GROUP: %10d ELEMENTS: %10d MATERIAL: %10d NFLAGS: %10d\n",1,Mesh->CountOfCells,0,1);
		fprintf(fd,"%32s\n","group");
		fprintf(fd,"%8d\n",0);
		for(i=0,j=0;i<Mesh->CountOfCells;i++,j++){
			if(j>=10){
				fprintf(fd,"\n");
				j=0;
			}
			fprintf(fd,"%8d",i+1);
		}
		fprintf(fd,"\n");
		fprintf(fd,"ENDOFSECTION\n");

    }

	for(k=0;k<Mesh->CountOfBoundaryConditions;k++){
		fprintf(fd,"%20s\n","BOUNDARY CONDITIONS");
		//sprintf(buf,"boundary_%d",Mesh->BoundaryConditions[k].Condition);
		sprintf(buf,"%d",Mesh->BoundaryConditions[k].Condition);
		fprintf(fd,"%32s%8d%8d%8d\n",buf,
				1, // Element/cell Data
				Mesh->BoundaryConditions[k].CountOfCells,
				0); // No Element/cell values
		for(i=0;i<Mesh->BoundaryConditions[k].CountOfCells;i++){
			j = Mesh->BoundaryConditions[k].Cells[i]; // cell
			fprintf(fd,"%10d",j+1);
			switch(Mesh->CellTypes[j]){
				case RW_MESH_CELL_TYPE_TETRA:
					fprintf(fd,"%5d",NEU_CELL_TYPE_TETRA);
					fprintf(fd,"%5d",Mesh->BoundaryConditions[k].Faces[i]);
					break;
				case RW_MESH_CELL_TYPE_PYRAMID:
					fprintf(fd,"%5d",NEU_CELL_TYPE_PYRAMID);
					fprintf(fd,"%5d",Mesh->BoundaryConditions[k].Faces[i]);
					break;
				case RW_MESH_CELL_TYPE_WEDGE:
					fprintf(fd,"%5d",NEU_CELL_TYPE_WEDGE);
					fprintf(fd,"%5d",Mesh->BoundaryConditions[k].Faces[i]);
					break;
				case RW_MESH_CELL_TYPE_HEXAHEDRON:
					fprintf(fd,"%5d",NEU_CELL_TYPE_BRICK);
					fprintf(fd,"%5d",Mesh->BoundaryConditions[k].Faces[i]);
					break;
				case RW_MESH_CELL_TYPE_VOXEL:
					fprintf(fd,"%5d",NEU_CELL_TYPE_BRICK);
					fprintf(fd,"%5d",Mesh->BoundaryConditions[k].Faces[i]);
					break;
				default:
					sprintf(buf,"write neu file: unknown cell type %d\n",Mesh->CellTypes[j]);
					rw_mesh_set_error(0,buf);
					return 1;
					break;
			}
			fprintf(fd,"\n");
		}
	    fprintf(fd,"ENDOFSECTION\n");

    }
    fclose(fd);

	return 0;
}

/**
 * Записать сетку в файл filename в формате neu
 * Dimension - размерность точек, допустимы значения 2 или 3
 * CountOfPoints - число точек
 * Points - массив точек размерностью CountOfPoints*Dimension
 * CountOfCells - кол-во ячеек
 * Cells - массив ячеек размерностью CellOffset[CountOfCells]
 * CellTypes - типы ячеек
 * CellSizes - размеры ячеек
 * CellOffset - смещения ячеек
 * CountOfBoundaryConditions - число граничных условий
 * BCNumber - нормера граничных условий
 * BCCountOfCells - число ячеек в каждом граничном условии
 * BCCells - ячейки граничных условий
 * BCFaces - грани ячеек граничных условий
 */
int write_format_neu_simplified(
		int Dimension, int CountOfPoints, REAL*Points,
		int CountOfCells, int*Cells, int*CellTypes, int*CellSizes, int*CellOffset,
		int CountOfBoundaryConditions, int*BCNumber,int*BCCountOfCells, int**BCCells,int**BCFaces,
		char*filename){
	int r;
	struct neu_mesh_struct*Mesh;

	Mesh = (struct neu_mesh_struct*)calloc(1,sizeof(struct neu_mesh_struct));
	neu_mesh_struct_init(Mesh);

	r = neu_mesh_struct_set_mesh(Mesh,Dimension,CountOfPoints,Points,
			CountOfCells,Cells,CellTypes,CellSizes,CellOffset);
	if(r){
		neu_mesh_struct_free(Mesh);
		ffree(Mesh);
		return r;
	}

	r = neu_mesh_struct_add_boundary_conditions(Mesh,CountOfBoundaryConditions,BCNumber,BCCountOfCells,BCCells,BCFaces);
	if(r){
		neu_mesh_struct_free(Mesh);
		ffree(Mesh);
		return r;
	}

	r = write_format_neu_struct(Mesh,filename);
	neu_mesh_struct_free(Mesh);
	ffree(Mesh);
	return r;
}

/**
 * Записать сетку в файл filename в формате neu
 * Mesh - ссылка на структуру сетки, предполагается, что в ней точки не заданы
 * Dimension - размерность точек, допустимы значения 2 или 3
 * CountOfPoints - число точек
 * Points - массив точек размерностью CountOfPoints*Dimension
 * CountOfCells - кол-во ячеек
 * Cells - массив ячеек размерностью CountOfCells * sizeof{CellType}
 * CellType - тип ячеек
 * CountOfBoundaryConditions - число граничных условий
 * BCNumber - нормера граничных условий
 * BCCountOfCells - число ячеек в каждом граничном условии
 * BCCells - ячейки граничных условий
 * BCFaces - грани ячеек граничных условий
 */
int write_format_neu_simplified_uniform(
		int Dimension, int CountOfPoints, REAL*Points,
		int CountOfCells, int*Cells, int CellType,
		int CountOfBoundaryConditions, int*BCNumber,int*BCCountOfCells, int**BCCells,int**BCFaces,
		char*filename){
	int r;
	struct neu_mesh_struct*Mesh;

	Mesh = (struct neu_mesh_struct*)calloc(1,sizeof(struct neu_mesh_struct));
	neu_mesh_struct_init(Mesh);

	r = neu_mesh_struct_set_mesh_uniform(Mesh,Dimension,CountOfPoints,Points,
			CountOfCells,Cells,CellType);
	if(r){
		neu_mesh_struct_free(Mesh);
		ffree(Mesh);
		return r;
	}

	r = neu_mesh_struct_add_boundary_conditions(Mesh,CountOfBoundaryConditions,BCNumber,BCCountOfCells,BCCells,BCFaces);
	if(r){
		neu_mesh_struct_free(Mesh);
		ffree(Mesh);
		return r;
	}

	r = write_format_neu_struct(Mesh,filename);
	neu_mesh_struct_free(Mesh);
	ffree(Mesh);
	return r;
}

/**
 * Прочитать из потока первую строку, пропуская комментарии
 * В формате neu комментарии начинаются с символа /
 */
int read_line_skip_comments_neu (FILE * STREAM, char *line,int*linecounter){
	int r;
	do{
		r=rw_mesh_read_line(STREAM,line);
		if(linecounter)*linecounter = *linecounter+1;
	}while(r>=0 && line[0]=='/');
	return r;
}
void string_empty(char *line,int length){
	memset(line,0,length*sizeof(char));
}

int string_cut_length(char*string,char*word,int length){
	int i,l,r;

	if(length==0){
		word[0] = '\0';
		return 0;
	}

	for(i=0;i<length && string[i]!='\0';i++){
		word[i] = string[i];
	}
	word[i] = '\0';

	r = (i<length)?1:0;

	for(l=0;string[i]!='\0';i++,l++){
		string[l] = string[i];
	}
	string[l] = '\0';

	return r;
}

int read_format_neu_struct(struct neu_mesh_struct*Mesh, char*filename){
	__save_locale;
	int current_line;
	int i,j,k,s;
	FILE*fd;
	REAL3 Point;
	char line[256],buf1[256],buf2[256];
	char section_name[256],section_version[256];
	int n,n2,n3,n4;
	int ct;
	int section_end_skipped;
#define max_cells_size 8
	int current_BC;

	neu_mesh_struct_init(Mesh);

	rw_mesh_set_filename(filename);
	fd=fopen(filename,"r");if(fd==NULL){
		rw_mesh_set_error(0,"Can't find file");
		return 1;
	}
	current_line = 0;

	//read HEADER SECTION
	{
		//:CONTROL INFO 2.3.16
		read_line_skip_comments_neu(fd,line,&current_line);
		string_empty(section_name,256);
		string_empty(section_version,256);
		k = sscanf(line,"%20c%20c",section_name,section_version);
		if(k!=1 && k!=2){
			rw_mesh_set_error(current_line,"Incorrect header section");
			close(fd);
			return 1;
		}
		string_trim(section_name);
		if(strcmp(section_name,"CONTROL INFO")!=0){
			rw_mesh_set_error(current_line,"Incorrect header section");
			close(fd);
			return 1;
		}

		//:** GAMBIT NEUTRAL FILE
		read_line_skip_comments_neu(fd,line,&current_line);
		string_trim(line);
		if(strcmp(line,"** GAMBIT NEUTRAL FILE")!=0){
			rw_mesh_set_error(current_line,"Incorrect header section");
			close(fd);
			return 1;
		}

		// skip information about title, date, time, and program
		//:%Title%
		read_line_skip_comments_neu(fd,line,&current_line);

		//:PROGRAM:    %ProgramName%     VERSION:  %ProgramVersion%
		read_line_skip_comments_neu(fd,line,&current_line);

		//:%Date% %Time%
		read_line_skip_comments_neu(fd,line,&current_line);

		//:NUMNP     NELEM     NGRPS    NBSETS     NDFCD     NDFVL
		read_line_skip_comments_neu(fd,line,&current_line);
		if(strcmp(line,"     NUMNP     NELEM     NGRPS    NBSETS     NDFCD     NDFVL")!=0){
			rw_mesh_set_error(current_line,"Incorrect header section");
			close(fd);
			return 1;
		}

		//:%NUMNP%     %NELEM%     %NGRPS%    %NBSETS%     %NDFCD%     %NDFVL%
		read_line_skip_comments_neu(fd,line,&current_line);
		if(sscanf(line,"%10d%10d%10d%10d%10d%10d\n",
				&(Mesh->CountOfPoints),
				&(Mesh->CountOfCells),
				&n, // Number of element groups
				&(Mesh->CountOfBoundaryConditions), //Number of boundary condition sets
				&(Mesh->Dimension), // Number of coordinate directions (2 or 3)
				&n // Number of coordinate directions (2 or 3)
				)!=6){
			rw_mesh_set_error(current_line,"Incorrect header section");
			close(fd);
			return 1;
		}
		current_BC = 0;

		if(Mesh->Dimension!=2 && Mesh->Dimension!=3){
			rw_mesh_set_error(current_line,"Incorrect coordinate directions (dimension)");
			close(fd);
			return 1;
		}


		//:ENDOFSECTION
		read_line_skip_comments_neu(fd,line,&current_line);
		if(strcmp(line,"ENDOFSECTION")!=0){
			rw_mesh_set_error(current_line,"Incorrect header section");
			close(fd);
			return 1;
		}

	}

	//read sections
	while(read_line_skip_comments_neu(fd,line,&current_line)>=0){
		string_empty(section_name,256);
		string_empty(section_version,256);
		k = sscanf(line,"%20c%20c",section_name,section_version);
		if(k!=1 && k!=2){
			rw_mesh_set_error(current_line,"Incorrect section name");
			close(fd);
			return 1;
		}
		section_end_skipped = 0;
		string_trim(section_name);
		if(strcmp(section_name,"NODAL COORDINATES")==0){
			if(Mesh->CountOfPoints>0){
				Mesh->Points = (REAL*)calloc(Mesh->CountOfPoints*Mesh->Dimension,sizeof(REAL));

				for(i=0;i<Mesh->CountOfPoints;i++){
					if(read_line_skip_comments_neu(fd,line,&current_line)<0){
						rw_mesh_set_error(current_line,"Incorrect section NODAL COORDINATES. Not enough lines");
						close(fd);
						return 1;
					}
					if(Mesh->Dimension==2){
						if(sscanf(line,"%10d%20lf%20lf\n",&j,Point+0,Point+1)!=3){
							rw_mesh_set_error(current_line,"Incorrect section NODAL COORDINATES");
							close(fd);
							return 1;
						}
					}else{
						if(sscanf(line,"%10d%20lf%20lf%20lf\n",&j,Point+0,Point+1,Point+2)!=4){
							rw_mesh_set_error(current_line,"Incorrect section NODAL COORDINATES");
							close(fd);
							return 1;
						}
					}
					//Ignore global node number
					/*j = j - 1;
					if(j<0 || j>=Mesh->CountOfPoints){
						rw_mesh_set_error(current_line,"Incorrect section NODAL COORDINATES. Incorrect node number");
						close(fd);
						return 1;
					}*/
					for(k=0;k<Mesh->Dimension;k++)
						Mesh->Points[Mesh->Dimension*i+k] = Point[k];
				}
			}
		}else if(strcmp(section_name,"ELEMENTS/CELLS")==0){
			if(Mesh->CountOfCells>0){
				Mesh->Cells = (int*)calloc(Mesh->CountOfCells,max_cells_size*sizeof(int));
				Mesh->CellSizes = (int*)calloc(Mesh->CountOfCells,sizeof(int));
				Mesh->CellOffset = (int*)calloc((Mesh->CountOfCells+1),sizeof(int));
				Mesh->CellTypes = (int*)calloc(Mesh->CountOfCells,sizeof(int));

				Mesh->CellOffset[0] = 0;
				for(i=0;i<Mesh->CountOfCells;i++){

					if(read_line_skip_comments_neu(fd,line,&current_line)<0){
						rw_mesh_set_error(current_line,"Incorrect section ELEMENTS/CELLS. Not enough lines");
						close(fd);
						return 1;
					}

					// global number of cell
					string_cut_length(line,buf1,8);
					if(sscanf(buf1,"%8d",&j)!=1){
						rw_mesh_set_error(current_line,"Incorrect section ELEMENTS/CELLS");
						close(fd);
						return 1;
					}

					// cell type
					string_cut_length(line,buf1,3);
					if(sscanf(buf1,"%3d",&ct)!=1){
						rw_mesh_set_error(current_line,"Incorrect section ELEMENTS/CELLS");
						close(fd);
						return 1;
					}

					// cell size
					string_cut_length(line,buf1,3);
					if(sscanf(buf1,"%3d",&(Mesh->CellSizes[i]))!=1){
						rw_mesh_set_error(current_line,"Incorrect section ELEMENTS/CELLS");
						close(fd);
						return 1;
					}

					string_cut_length(line,buf1,1);

					switch(ct){
						case NEU_CELL_TYPE_EDGE:
							Mesh->CellTypes[i] = RW_MESH_CELL_TYPE_EDGE;
							break;

						case NEU_CELL_TYPE_TRIANGLE:
							Mesh->CellTypes[i] = RW_MESH_CELL_TYPE_TRIANGLE;
							break;

						case NEU_CELL_TYPE_QUAD:
							Mesh->CellTypes[i] = RW_MESH_CELL_TYPE_QUAD;
							break;

						case NEU_CELL_TYPE_TETRA:
							Mesh->CellTypes[i] = RW_MESH_CELL_TYPE_TETRA;
							break;

						case NEU_CELL_TYPE_PYRAMID:
							Mesh->CellTypes[i] = RW_MESH_CELL_TYPE_PYRAMID;
							break;

						case NEU_CELL_TYPE_WEDGE:
							Mesh->CellTypes[i] = RW_MESH_CELL_TYPE_WEDGE;
							break;

						case NEU_CELL_TYPE_BRICK:
							Mesh->CellTypes[i] = RW_MESH_CELL_TYPE_VOXEL;
							break;

						default:
							rw_mesh_set_error(current_line,"Incorrect section ELEMENTS/CELLS. Unknown cell type");
							close(fd);
							return 1;
							break;
					}

					s = rw_mesh_cell_type_size(Mesh->CellTypes[i]);
					Mesh->CellOffset[i+1] = Mesh->CellOffset[i]+s;
					for(k=0;k<s;k++){
						if(k>0 && k%7==0){
							if(read_line_skip_comments_neu(fd,line,&current_line)<0){
								rw_mesh_set_error(current_line,"Incorrect section ELEMENTS/CELLS. Not enough lines");
								close(fd);
								return 1;
							}
							if(string_cut_length(line,buf1,15)){
								rw_mesh_set_error(current_line,"Incorrect section ELEMENTS/CELLS");
								close(fd);
								return 1;
							}
						}
						if(string_cut_length(line,buf1,8)){
							rw_mesh_set_error(current_line,"Incorrect section ELEMENTS/CELLS");
							close(fd);
							return 1;
						}

						if(sscanf(buf1,"%8d",&j)!=1){
							rw_mesh_set_error(current_line,"Incorrect section ELEMENTS/CELLS");
							close(fd);
							return 1;
						}
						Mesh->Cells[Mesh->CellOffset[i]+k] = j-1;
					}

					switch(ct){
						case NEU_CELL_TYPE_PYRAMID:
							j = Mesh->Cells[Mesh->CellOffset[i]+2];
							Mesh->Cells[Mesh->CellOffset[i]+2] = Mesh->Cells[Mesh->CellOffset[i]+3];
							Mesh->Cells[Mesh->CellOffset[i]+3] = j;
							break;

						case NEU_CELL_TYPE_WEDGE:
							j = Mesh->Cells[Mesh->CellOffset[i]+1];
							Mesh->Cells[Mesh->CellOffset[i]+1] = Mesh->Cells[Mesh->CellOffset[i]+2];
							Mesh->Cells[Mesh->CellOffset[i]+2] = j;
							j = Mesh->Cells[Mesh->CellOffset[i]+4];
							Mesh->Cells[Mesh->CellOffset[i]+4] = Mesh->Cells[Mesh->CellOffset[i]+5];
							Mesh->Cells[Mesh->CellOffset[i]+5] = j;
							break;
					}
				}
			}

//		}else if(strcmp(section_name,"ELEMENT GROUP")==0){
			//skip ELEMENT GROUP section
//		}else if(strcmp(section_name,"APPLICATION DATA")==0){
			//skip APPLICATION DATA section
		}else if(strcmp(section_name,"BOUNDARY CONDITIONS")==0){
			if(!Mesh->CountOfBoundaryConditions){
				rw_mesh_set_error(current_line,"Incorrect section BOUNDARY CONDITIONS. Count of Boundary Conditions set zero");
				close(fd);
				return 1;
			}
			if(current_BC>=Mesh->CountOfBoundaryConditions){
				rw_mesh_set_error(current_line,"Incorrect section BOUNDARY CONDITIONS. Too many Boundary Conditions sections");
				close(fd);
				return 1;
			}

			if(!Mesh->BoundaryConditions){
				Mesh->BoundaryConditions = (struct neu_boundary_condition_struct*)calloc(Mesh->CountOfBoundaryConditions,sizeof(struct neu_boundary_condition_struct));
				for(i=0;i<Mesh->CountOfBoundaryConditions;i++){
					Mesh->BoundaryConditions[i].Condition = 0;
					Mesh->BoundaryConditions[i].CountOfCells = 0;
					Mesh->BoundaryConditions[i].Cells = NULL;
					Mesh->BoundaryConditions[i].Faces = NULL;
				}
			}

			//:                         boundary       1     588       0       6
			//: NAME  ITYPE NENTRY NVALUES [ IBCODE1 IBCODE2 IBCODE3 IBCODE4 IBCODE5]
			// skip IBCODES
			if(read_line_skip_comments_neu(fd,line,&current_line)<0){
				rw_mesh_set_error(current_line,"Incorrect section BOUNDARY CONDITIONS. Not enough lines");
				close(fd);
				return 1;
			}

			string_empty(buf1,256);
			if(sscanf(line,"%32c%10d%10d%10d",buf1,&n,&n2,&n3)!=4){
				rw_mesh_set_error(current_line,"Incorrect section ELEMENTS/CELLS");
				close(fd);
				return 1;
			}
			//if boundary name is number
			if(sscanf(buf1,"%d",&j)==1){
				Mesh->BoundaryConditions[current_BC].Condition = j;
			}else{
				Mesh->BoundaryConditions[current_BC].Condition = current_BC+1;
			}

			if(n!=1){ //ITYPE element/cell
				//TODO itype = 0
				rw_mesh_set_error(current_line,"Incorrect section ELEMENTS/CELLS. ITYPE not equal 1");
				close(fd);
				return 1;
			}

			// n2 Number of data records in boundary-condition set
			Mesh->BoundaryConditions[current_BC].CountOfCells = n2;

			// n3 Number of values for each data record

			if(Mesh->BoundaryConditions[current_BC].CountOfCells){
				Mesh->BoundaryConditions[current_BC].Cells = (int*)calloc(Mesh->BoundaryConditions[current_BC].CountOfCells,sizeof(int));
				Mesh->BoundaryConditions[current_BC].Faces = (int*)calloc(Mesh->BoundaryConditions[current_BC].CountOfCells,sizeof(int));

				for(i=0;i<Mesh->BoundaryConditions[current_BC].CountOfCells;i++){
					if(read_line_skip_comments_neu(fd,line,&current_line)<0){
						rw_mesh_set_error(current_line,"Incorrect section BOUNDARY CONDITIONS. Not enough lines");
						close(fd);
						return 1;
					}

					//: ELEM	ELEMENT TYPE	FACE 	[VALUES]
					if(sscanf(line,"%10d%5d%5d",&(Mesh->BoundaryConditions[current_BC].Cells[i]),&j,
							&(Mesh->BoundaryConditions[current_BC].Faces[i]))!=3){
						rw_mesh_set_error(current_line,"Incorrect section BOUNDARY CONDITIONS");
						close(fd);
						return 1;
					}

					Mesh->BoundaryConditions[current_BC].Cells[i] = Mesh->BoundaryConditions[current_BC].Cells[i] - 1;
					Mesh->BoundaryConditions[current_BC].Faces[i] = Mesh->BoundaryConditions[current_BC].Faces[i] - 1;
				}
			}
			current_BC++;

//		}else if(strcmp(section_name,"FACE CONNECTIVITY")==0){
			//skip FACE CONNECTIVITY section
//		}else if(strcmp(section_name,"TIMESTEPDATA")==0){
			//skip TIMESTEPDATA section
			// which ended with ENDOFTIMESTEP
		}else{
			//skip unknown section
			while((n=read_line_skip_comments_neu(fd,line,&current_line))>=0 && strcmp(line,"ENDOFSECTION")!=0);

			if(strcmp(line,"ENDOFSECTION") != 0){
				rw_mesh_set_error(current_line,"Unknown section don't have end");
				close(fd);
				return 1;
			}
			section_end_skipped = 1;
		}

		if(!section_end_skipped){
			//:ENDOFSECTION
			read_line_skip_comments_neu(fd,line,&current_line);
			if(strcmp(line,"ENDOFSECTION")!=0){
				rw_mesh_set_error(current_line,"Section don't have end");
				close(fd);
				return 1;
			}
		}
	}

	fclose(fd);

	return 0;
}
