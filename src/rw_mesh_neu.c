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

/*int readUnstructedMeshFromNEUFile(struct unstructedMesh*Mesh,char*filename){
	int i,j,k;
	FILE*fd;
	char s[256],s2[256];
	int n,n2,n3,n4;
	int ct;
	int line=0;
#define max_cells_size 8

	initUnstructedMesh(Mesh);

	fd = fopen (filename, "r");

	line++;read_line(fd,s); //:CONTROL INFO 2.3.16
	//if(!slre_match_one("^[\\s*]?CONTROL[\\s]+INFO[\\s]+[\\d]+\\.[\\d]+\\.[\\d]+[\\s*]?$",s)){printf("Error parsing on line %d\n",line);return line;}
	line++;read_line(fd,s); //:** GAMBIT NEUTRAL FILE
	//if(!slre_match_one("^[\\s*]?..[\\s]+GAMBIT[\\s]+NEUTRAL[\\s]+FILE[\\s*]?$",s)){printf("Error parsing on line %d\n",line);return line;}
	line++;read_line(fd,s);//:EXAMPLE FILE
	line++;read_line(fd,s);//:PROGRAM:               Testing     VERSION:  0.1
	line++;read_line(fd,s);//:Oct 2011
	line++;read_line(fd,s);//:NUMNP     NELEM     NGRPS    NBSETS     NDFCD     NDFVL

	fscanf(fd,"%d",&(Mesh->CountOfPoints));
	fscanf(fd,"%10d",&(Mesh->CountOfCells));
	fscanf(fd,"%10d",&n); // Number of element groups
	fscanf(fd,"%10d",&Mesh->CountOfBoundaryConditions ); //Number of boundary condition sets
	fscanf(fd,"%10d",&n); // Number of coordinate directions (2 or 3)
	fscanf(fd,"%10d\n",&n); // Number of coordinate directions (2 or 3)

	line++;read_line(fd,s); //:ENDOFSECTION

	line++;read_line(fd,s); //:NODAL COORDINATES %[^\n]s\n
	if(Mesh->CountOfPoints>0){
		Mesh->Points = (REAL*)calloc(Mesh->CountOfPoints,3*sizeof(REAL));
		for(i=0;i<Mesh->CountOfPoints;i++){
			fscanf(fd," %10d %lf %lf %lf\n",&j,&Mesh->Points[i*3],&Mesh->Points[i*3+1],&Mesh->Points[i*3+2]);
		}
	}
	line++;read_line(fd,s); //:ENDOFSECTION

	line++;read_line(fd,s); //:ELEMENTS/CELLS 2.3.16

	if(Mesh->CountOfCells>0){
		Mesh->Cells = (int*)calloc(Mesh->CountOfCells,max_cells_size*sizeof(int));
		Mesh->CellsMask = (int*)calloc(Mesh->CountOfCells,sizeof(int));
		Mesh->CellsSizes = (int*)calloc(Mesh->CountOfCells,sizeof(int));
		Mesh->CellsOffset = (int*)calloc(Mesh->CountOfCells,sizeof(int));
		Mesh->CellsTypes = (int*)calloc(Mesh->CountOfCells,sizeof(int));

		for(i=0;i<Mesh->CountOfCells;i++){

			fscanf(fd,"%8d",&j);
			fscanf(fd,"%3d",&ct);
			fscanf(fd,"%3d ",&Mesh->CellsSizes[i]);
	    	switch(ct){
	    		case NEU_CELL_TYPE_TETRA:
	    			Mesh->CellsTypes[i] = CELL_TYPE_TETRA;
	    			Mesh->CellsOffset[i] = i*max_cells_size;
	    			for(k=0;k<4;k++){
	    				fscanf(fd,"%8d",&j);
	    				Mesh->Cells[Mesh->CellsOffset[i]+k] = j-1;
	    			}
	    			break;

	    		case NEU_CELL_TYPE_PYRAMID:
	    			Mesh->CellsTypes[i] = CELL_TYPE_PYRAMID;
	    			Mesh->CellsOffset[i] = i*max_cells_size;
	    			for(k=0;k<5;k++){
	    				fscanf(fd,"%8d",&j);
	    				Mesh->Cells[Mesh->CellsOffset[i]+_cp[k]] = j-1;
	    			}
	    			break;

	    		case NEU_CELL_TYPE_WEDGE:
	    			Mesh->CellsTypes[i] = CELL_TYPE_WEDGE;
	    			Mesh->CellsOffset[i] = i*max_cells_size;
	    			for(k=0;k<6;k++){
	    				fscanf(fd,"%8d",&j);
	    				Mesh->Cells[Mesh->CellsOffset[i]+_cw[k]] = j-1;
	    			}
	    			break;

	    		case NEU_CELL_TYPE_BRICK:
	    			Mesh->CellsTypes[i] = CELL_TYPE_VOXEL;
	    			Mesh->CellsOffset[i] = i*max_cells_size;
	    			for(k=0;k<Mesh->CellsSizes[i];k++){
	    				fscanf(fd,"%8d",&j);
	    				Mesh->Cells[Mesh->CellsOffset[i]+k] = j-1;
	    			}
	    			break;

	    		default:
	    			log_write("readUnstructedMeshToNEUFile: unknown cell type %d\n",ct);
	    			Mesh->CellsTypes[i] = CELL_TYPE_NONE;
	    			Mesh->CellsOffset[i] = i*max_cells_size;
	    			break;
	    	}
	    	fscanf(fd,"\n");
	    }
		line++;read_line(fd,s); //:ENDOFSECTION
	}

	line++;read_line(fd,s); //:ELEMENT GROUP 2.3.16
	line++;read_line(fd,s); //:GROUP:          1 ELEMENTS:       6468 MATERIAL:          0 NFLAGS:          1
	line++;read_line(fd,s); //:group
	fscanf(fd,"%8d\n",&n);
	for(i=0,j=0;i<Mesh->CountOfCells;i++,j++){
		if(j>=10){
			fscanf(fd,"\n");
			j=0;
		}
		fscanf(fd,"%8d",&n);
	}
	fscanf(fd,"\n");
	line++;read_line(fd,s); //:ENDOFSECTION

	if(Mesh->CountOfBoundaryConditions){
		Mesh->BoundaryConditions = (struct boundaryCondition*)calloc(Mesh->CountOfBoundaryConditions,sizeof(struct boundaryCondition));
		for(k=0;k<Mesh->CountOfBoundaryConditions;k++){
			initBoundaryCondition(&Mesh->BoundaryConditions[k]);
			line++;read_line(fd,s); //: BOUNDARY CONDITIONS 2.3.16
			//line++;read_line(fd,s); //:                         boundary       1     588       0       6
			fscanf(fd,"%s %d %d %d %d\n",s,&n,&n2,&n3,&n4);
			Mesh->BoundaryConditions[k].CountOfCells = n2;
			Mesh->BoundaryConditions[k].Condition = k+1;
			if(Mesh->BoundaryConditions[k].CountOfCells){
				Mesh->BoundaryConditions[k].Cells = (int*)calloc(Mesh->BoundaryConditions[k].CountOfCells,sizeof(int));
				Mesh->BoundaryConditions[k].Faces = (int*)calloc(Mesh->BoundaryConditions[k].CountOfCells,sizeof(int));

				for(i=0;i<Mesh->BoundaryConditions[k].CountOfCells;i++){
					fscanf(fd,"%10d",&n);
					Mesh->BoundaryConditions[k].Cells[i] = n-1; // cell
					fscanf(fd,"%5d",&n);
					fscanf(fd,"%5d",&n);
					Mesh->BoundaryConditions[k].Faces[i] = n;
					fscanf(fd,"\n");
				}
			}
			line++;read_line(fd,s); //:ENDOFSECTION

		}
	}

	fclose(fd);
	compressUnstructedMesh(Mesh);

	return 0;
}*/
