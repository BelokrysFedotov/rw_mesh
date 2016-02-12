#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "rw_mesh_string.h"
#include "rw_mesh_vtk.h"

void le2be16 (unsigned char *d) {
    unsigned char c;
    c = d[0];d[0] = d[1];d[1] = c;
}
void be2le16 (unsigned char *d) {le2be16(d);}

void le2be32 (unsigned char *d) {
    unsigned char c;
    c = d[0];d[0] = d[3];d[3] = c;
    c = d[1];d[1] = d[2];d[2] = c;
}
void be2le32 (unsigned char *d) {le2be32(d);}

void le2be64 (unsigned char *d) {
    unsigned char c;
    c = d[0];d[0] = d[7];d[7] = c;
    c = d[1];d[1] = d[6];d[6] = c;
    c = d[2];d[2] = d[5];d[5] = c;
    c = d[3];d[3] = d[4];d[4] = c;
}
void  be2le64 (unsigned char *d) {le2be64(d);}

void le2be (unsigned char *d,size_t size) {
	if(size==2){
		le2be16(d);
	}else if(size==4){
		le2be32(d);
	} if(size==8){
		le2be64(d);
	}
}
void be2le (unsigned char *d,size_t size) {le2be(d,size);}

int rw_mesh_vtk_struct_init(struct RW_MESH_VTK_STRUCT*VTK){
	rw_mesh_vtk_struct_clean(VTK);
	return 0;
}
int rw_mesh_vtk_struct_clean(struct RW_MESH_VTK_STRUCT*VTK){
	VTK->type = RW_MESH_VTK_TYPE_NONE;
	VTK->Grid = NULL;

	VTK->CountOfPointsData = 0;
	VTK->PointsData = NULL;

	VTK->CountOfCellsData = 0;
	VTK->CellsData = NULL;

	VTK->CountOfData = 0;
	VTK->Data = NULL;
	return 0;
}
int rw_mesh_vtk_struct_free(struct RW_MESH_VTK_STRUCT*VTK){
	int i;

	if(VTK->Grid)
		switch(VTK->type){
		case RW_MESH_VTK_TYPE_STRUCTURED_POINTS:
			rw_mesh_vtk_structured_points_struct_free((struct RW_MESH_VTK_STRUCTURED_POINTS_STRUCT*)(VTK->Grid));
			break;
		case RW_MESH_VTK_TYPE_STRUCTURED_GRID:
			rw_mesh_vtk_structured_grid_struct_free((struct RW_MESH_VTK_STRUCTURED_GRID_STRUCT*)(VTK->Grid));
			break;
		case RW_MESH_VTK_TYPE_RECTILINEAR_GRID:
			rw_mesh_vtk_rectiliner_grid_struct_free((struct RW_MESH_VTK_RECTILINER_GRID_STRUCT*)(VTK->Grid));
			break;
		case RW_MESH_VTK_TYPE_POLYGONAL_DATA:
			rw_mesh_vtk_polygonal_data_struct_free((struct RW_MESH_VTK_POLYGONAL_DATA_STRUCT*)(VTK->Grid));
			break;
		case RW_MESH_VTK_TYPE_UNSTRUCTURED_GRID:
			rw_mesh_vtk_unstructured_grid_struct_free((struct RW_MESH_VTK_UNSTRUCTURED_GRID_STRUCT*)(VTK->Grid));
			break;
		}

	ffree(VTK->Grid);

	for(i=0;i<VTK->CountOfPointsData;i++)
		rw_mesh_vtk_dataset_struct_free(VTK->PointsData+i);
	ffree(VTK->PointsData);
	for(i=0;i<VTK->CountOfCellsData;i++)
		rw_mesh_vtk_dataset_struct_free(VTK->CellsData+i);
	ffree(VTK->CellsData);
	for(i=0;i<VTK->CountOfData;i++)
		rw_mesh_vtk_dataset_struct_free(VTK->Data+i);
	ffree(VTK->Data);

	rw_mesh_vtk_struct_clean(VTK);
	return 0;
}

int rw_mesh_vtk_structured_points_struct_init(struct RW_MESH_VTK_STRUCTURED_POINTS_STRUCT*VTK){
	rw_mesh_vtk_structured_points_struct_clean(VTK);
	return 0;
}
int rw_mesh_vtk_structured_points_struct_clean(struct RW_MESH_VTK_STRUCTURED_POINTS_STRUCT*VTK){
	//TODO
	return 0;
}
int rw_mesh_vtk_structured_points_struct_free(struct RW_MESH_VTK_STRUCTURED_POINTS_STRUCT*VTK){
	//TODO
	return 0;
}

int rw_mesh_vtk_structured_grid_struct_init(struct RW_MESH_VTK_STRUCTURED_GRID_STRUCT*VTK){
	rw_mesh_vtk_structured_grid_struct_clean(VTK);
	return 0;
}
int rw_mesh_vtk_structured_grid_struct_clean(struct RW_MESH_VTK_STRUCTURED_GRID_STRUCT*VTK){
	VTK->Nx = 0;
	VTK->Ny = 0;
	VTK->Nz = 0;
	VTK->Points = NULL;
	return 0;
}
int rw_mesh_vtk_structured_grid_struct_free(struct RW_MESH_VTK_STRUCTURED_GRID_STRUCT*VTK){
	ffree(VTK->Points);
	rw_mesh_vtk_structured_grid_struct_clean(VTK);
	return 0;
}

int rw_mesh_vtk_rectiliner_grid_struct_init(struct RW_MESH_VTK_RECTILINER_GRID_STRUCT*VTK){
	rw_mesh_vtk_rectiliner_grid_struct_clean(VTK);
	return 0;
}
int rw_mesh_vtk_rectiliner_grid_struct_clean(struct RW_MESH_VTK_RECTILINER_GRID_STRUCT*VTK){
	//TODO
	return 0;
}
int rw_mesh_vtk_rectiliner_grid_struct_free(struct RW_MESH_VTK_RECTILINER_GRID_STRUCT*VTK){
	//TODO
	return 0;
}

int rw_mesh_vtk_polygonal_data_struct_init(struct RW_MESH_VTK_POLYGONAL_DATA_STRUCT*VTK){
	rw_mesh_vtk_polygonal_data_struct_clean(VTK);
	return 0;
}
int rw_mesh_vtk_polygonal_data_struct_clean(struct RW_MESH_VTK_POLYGONAL_DATA_STRUCT*VTK){
	//TODO
	return 0;
}
int rw_mesh_vtk_polygonal_data_struct_free(struct RW_MESH_VTK_POLYGONAL_DATA_STRUCT*VTK){
	//TODO
	return 0;
}

int rw_mesh_vtk_unstructured_grid_struct_init(struct RW_MESH_VTK_UNSTRUCTURED_GRID_STRUCT*VTK){
	rw_mesh_vtk_unstructured_grid_struct_clean(VTK);
	return 0;
}
int rw_mesh_vtk_unstructured_grid_struct_clean(struct RW_MESH_VTK_UNSTRUCTURED_GRID_STRUCT*VTK){
	VTK->CountOfPoints = 0;
	VTK->Points = NULL;

	VTK->CountOfCells = 0;
	VTK->Cells = NULL;
	VTK->CellOffset = NULL;
	VTK->CellSizes = NULL;
	VTK->CellTypes = NULL;

	return 0;
}
int rw_mesh_vtk_unstructured_grid_struct_free(struct RW_MESH_VTK_UNSTRUCTURED_GRID_STRUCT*VTK){
	int i;
	ffree(VTK->Points);
	ffree(VTK->Cells);
	ffree(VTK->CellOffset);
	ffree(VTK->CellSizes);
	ffree(VTK->CellTypes);

	rw_mesh_vtk_unstructured_grid_struct_clean(VTK);
	return 0;
}

int rw_mesh_vtk_dataset_struct_init(struct RW_MESH_VTK_DATASET_STRUCT*DATASET){
	rw_mesh_vtk_dataset_struct_clean(DATASET);
	return 0;
}
int rw_mesh_vtk_dataset_struct_clean(struct RW_MESH_VTK_DATASET_STRUCT*DATASET){
	DATASET->DataType = RW_MESH_VTK_DATASET_TYPE_NONE;
	DATASET->DataName = NULL;
	DATASET->Counts = 0;
	DATASET->Data = NULL;
	return 0;
}
int rw_mesh_vtk_dataset_struct_free(struct RW_MESH_VTK_DATASET_STRUCT*DATASET){
	int i;
	switch(DATASET->DataType){
		case RW_MESH_VTK_DATASET_TYPE_NONE:
			break;
		case RW_MESH_VTK_DATASET_TYPE_SCALARS:
			ffree( ((struct RW_MESH_VTK_DATA_SCALARS_STRUCT*)DATASET->Data)->LookupTable );
			ffree( ((struct RW_MESH_VTK_DATA_SCALARS_STRUCT*)DATASET->Data)->values );
			break;
		case RW_MESH_VTK_DATASET_TYPE_COLOR_SCALARS:
			ffree( ((struct RW_MESH_VTK_DATA_COLOR_SCALARS_STRUCT*)DATASET->Data)->values );
			break;
		case RW_MESH_VTK_DATASET_TYPE_LOOKUP_TABLE:
			ffree( ((struct RW_MESH_VTK_DATA_LOOKUP_TABLE_STRUCT*)DATASET->Data)->values );
			break;
		case RW_MESH_VTK_DATASET_TYPE_VECTORS:
			ffree( ((struct RW_MESH_VTK_DATA_VECTORS_STRUCT*)DATASET->Data)->values );
			break;
		case RW_MESH_VTK_DATASET_TYPE_NORMALS:
			ffree( ((struct RW_MESH_VTK_DATA_NORMALS_STRUCT*)DATASET->Data)->values );
			break;
		case RW_MESH_VTK_DATASET_TYPE_TEXTURE_COORDINATES:
			ffree( ((struct RW_MESH_VTK_DATA_TEXTURE_COORDINATES_STRUCT*)DATASET->Data)->values );
			break;
		case RW_MESH_VTK_DATASET_TYPE_TENSORS:
			ffree( ((struct RW_MESH_VTK_DATA_TENSORS_STRUCT*)DATASET->Data)->values );
			break;
		case RW_MESH_VTK_DATASET_TYPE_FIELD:
			for(i=0;i<((struct RW_MESH_VTK_DATA_FIELD_STRUCT*)DATASET->Data)->numArrays;i++){
				ffree( ((struct RW_MESH_VTK_DATA_FIELD_STRUCT*)DATASET->Data)->Arrays[i].name );
				ffree( ((struct RW_MESH_VTK_DATA_FIELD_STRUCT*)DATASET->Data)->Arrays[i].values );
			}
			ffree( ((struct RW_MESH_VTK_DATA_FIELD_STRUCT*)DATASET->Data)->Arrays );
			break;
	}

	ffree(DATASET->DataName);
	ffree(DATASET->Data);
	return 0;
}

/**
 * Определить тип по его названию
 * @param string название типа
 * return константу типа или RW_MESH_VTK_TYPE_NONE, если тип не определён
 */
int _parse_vtk_type(char*string){
	if(strcmp(string,"bit")==0){
		return RW_MESH_VTK_TYPE_BIT;
	}else if(strcmp(string,"unsigned_char")==0){
		return RW_MESH_VTK_TYPE_UNSIGNED_CHAR;
	}else if(strcmp(string,"char")==0){
		return RW_MESH_VTK_TYPE_CHAR;
	}else if(strcmp(string,"unsigned_short")==0){
		return RW_MESH_VTK_TYPE_UNSIGNED_SHORT;
	}else if(strcmp(string,"short")==0){
		return RW_MESH_VTK_TYPE_SHORT;
	}else if(strcmp(string,"unsigned_int")==0){
		return RW_MESH_VTK_TYPE_UNSIGNED_INT;
	}else if(strcmp(string,"int")==0){
		return RW_MESH_VTK_TYPE_INT;
	}else if(strcmp(string,"unsigned_long")==0){
		return RW_MESH_VTK_TYPE_UNSIGNED_LONG;
	}else if(strcmp(string,"long")==0){
		return RW_MESH_VTK_TYPE_LONG;
	}else if(strcmp(string,"float")==0){
		return RW_MESH_VTK_TYPE_FLOAT;
	}else if(strcmp(string,"double")==0){
		return RW_MESH_VTK_TYPE_DOUBLE;
	}
	return RW_MESH_VTK_TYPE_NONE;
}

/**
 * Определить размер типа
 * @param type тип
 * return размер элемента данного типа
 */
size_t _vtk_type_size(int type){
	if(type==RW_MESH_VTK_TYPE_BIT || type==RW_MESH_VTK_TYPE_UNSIGNED_CHAR || type==RW_MESH_VTK_TYPE_CHAR){
		return sizeof(char);
	}else if(type==RW_MESH_VTK_TYPE_UNSIGNED_SHORT){
		return sizeof(unsigned short);
	}else if(type==RW_MESH_VTK_TYPE_SHORT){
		return sizeof(short);
	}else if(type==RW_MESH_VTK_TYPE_UNSIGNED_INT){
		return sizeof(unsigned int);
	}else if(type==RW_MESH_VTK_TYPE_INT){
		return sizeof(int);
	}else if(type==RW_MESH_VTK_TYPE_UNSIGNED_LONG){
		return sizeof(unsigned long int);
	}else if(type==RW_MESH_VTK_TYPE_LONG){
		return sizeof(long int);
	}else if(type==RW_MESH_VTK_TYPE_FLOAT){
		return sizeof(float);
	}else if(type==RW_MESH_VTK_TYPE_DOUBLE){
		return sizeof(double);
	}
	return 0;
}

/**
 * Выделить память под count элементов типа type
 * @param type тип
 * @param count размер массива
 * return указатель на выделеную память
 */
void*_vtk_type_allocate(int type,int count){
	return calloc(count,_vtk_type_size(type));
}

/**
 *	Скопировать массив Data размером count элементов типа type
 *		в массив RealData размером count элементов типа REAL
 *		с приведением типов
 */
void _vtk_convert_type_array_to_REAL(int type,int count,void*Data,REAL*RealData){
	int i;
	for(i=0;i<count;i++){
		if(type==RW_MESH_VTK_TYPE_BIT || type==RW_MESH_VTK_TYPE_UNSIGNED_CHAR || type==RW_MESH_VTK_TYPE_UNSIGNED_CHAR){
			RealData[i] = ((char*)Data)[i];
		}else if(type==RW_MESH_VTK_TYPE_UNSIGNED_SHORT){
			RealData[i] = ((unsigned short*)Data)[i];
		}else if(type==RW_MESH_VTK_TYPE_SHORT){
			RealData[i] = ((short*)Data)[i];
		}else if(type==RW_MESH_VTK_TYPE_UNSIGNED_INT){
			RealData[i] = ((unsigned int*)Data)[i];
		}else if(type==RW_MESH_VTK_TYPE_INT){
			RealData[i] = ((int*)Data)[i];
		}else if(type==RW_MESH_VTK_TYPE_UNSIGNED_LONG){
			RealData[i] = ((unsigned long*)Data)[i];
		}else if(type==RW_MESH_VTK_TYPE_LONG){
			RealData[i] = ((unsigned long*)Data)[i];
		}else if(type==RW_MESH_VTK_TYPE_FLOAT){
			RealData[i] = ((float*)Data)[i];
		}else if(type==RW_MESH_VTK_TYPE_DOUBLE){
			RealData[i] = ((double*)Data)[i];
		}
	}
}

/**
 * Прочитать из строки string значение типа type в массив value по позиции offset
 * @param type тип
 * @param string строка с текстовым представлением значения
 * @param value массив
 * @param offset смещение в массиве
 * return если успешно прочиталось, то 1, иначе 0
 */
int _read_vtk_type_offset_ascii(int type,char*string,void*value,int offset){
	int rn;
	char c;
	unsigned int u;
	if(type==RW_MESH_VTK_TYPE_BIT){
		rn = sscanf(string,"%c",&c);
		if(rn==1){
			if(c=='0'){
				((char*)value)[offset] = 0;
			}else if(c=='1'){
				((char*)value)[offset] = 1;
			}else{
				return 0;
			}
			return 1;
		}
		return 0;
	}else if(type==RW_MESH_VTK_TYPE_UNSIGNED_CHAR || type==RW_MESH_VTK_TYPE_CHAR){
		rn = sscanf(string,"%c",(char*)value+offset);
		return rn;
	}else if(type==RW_MESH_VTK_TYPE_UNSIGNED_SHORT){
		rn = sscanf(string,"%hu",(unsigned short*)value+offset);
		return rn;
	}else if(type==RW_MESH_VTK_TYPE_SHORT){
		rn = sscanf(string,"%hd",(short*)value+offset);
		return rn;
	}else if(type==RW_MESH_VTK_TYPE_UNSIGNED_INT){
		rn = sscanf(string,"%u",(unsigned int*)value+offset);
		return rn;
	}else if(type==RW_MESH_VTK_TYPE_INT){
		rn = sscanf(string,"%d",(int*)value+offset);
		return rn;
	}else if(type==RW_MESH_VTK_TYPE_UNSIGNED_LONG){
		rn = sscanf(string,"%lu",(unsigned long int*)value+offset);
		return rn;
	}else if(type==RW_MESH_VTK_TYPE_LONG){
		rn = sscanf(string,"%ld",(long int*)value+offset);
		return rn;
	}else if(type==RW_MESH_VTK_TYPE_FLOAT){
		rn = sscanf(string,"%f",(float*)value+offset);
		return rn;
	}else if(type==RW_MESH_VTK_TYPE_DOUBLE){
		rn = sscanf(string,"%lf",(double*)value+offset);
		return rn;
	}
	return 0;
}

/**
 * Прочитать из строки string значение типа type в value
 * @param type тип
 * @param string строка с текстовым представлением значения
 * @param value указатель на ячейку памяти для записи данных
 * return если успешно прочиталось, то 1, иначе 0
 */
int _read_vtk_type_ascii(int type,char*string,void*value){
	return _read_vtk_type_offset_ascii(type,string,value,0);
}

/**
 * Прочитать из строки string значение типа type и конвертировать сразу в REAL в массив value по позиции offset
 * @param type тип
 * @param string строка с текстовым представлением значения
 * @param value массив
 * @param offset смещение в массиве
 * return если успешно прочиталось, то 1, иначе 0
 */
int _read_vtk_type_as_REAL_offset_ascii(int type,char*string,REAL*value,int offset){
	int rn;
	void*buffer;

	buffer = _vtk_type_allocate(type,1);
	rn = _read_vtk_type_ascii(type,string,buffer);
	if(rn==1) _vtk_convert_type_array_to_REAL(type,1,buffer,value+offset);
	return rn;
}

/**
 * Прочитать из ascii файла fd count элементов типа type
 * @param fd дескриптор файла
 * @param count число элементов
 * @param type тип элементов
 * @param value массив в который будут прочитаны элементы
 * @param current_line номер текущей строки в файле
 */
int _read_vtk_array_type_ascii(FILE*fd,int count,int type,void*value,int*current_line){
	int i;
	char line[256];
	char word[256];

	i=0;
	line[0]=0;
	word[0]=0;
	while(i<count){
		if(line[0]==0){
			read_line_trim(fd,line);(*current_line)++;
		}
		string_cut_word(line,word);

		if(_read_vtk_type_offset_ascii(type,word,value,i)!=1){
			rw_mesh_set_error(*current_line,"Error in reading values");
			return 1;
		}
		i++;
	}

	return 0;
}

/**
 * Прочитать из binary файла fd count элементов типа type
 * @param fd дескриптор файла
 * @param count число элементов
 * @param type тип элементов
 * @param value массив в который будут прочитаны элементы
 * @param current_line номер текущей строки в файле
 */
int _read_vtk_array_type_binary(FILE*fd,int count,int type,void*value,int*current_line){
	int i;
	char c;
	size_t size,read_size;

	size = _vtk_type_size(type);
	if(!size){
		return 1;
	}

	read_size = fread(value,size,count,fd);
	(*current_line)++;

	if(read_size!=count){
		rw_mesh_set_error(*current_line,"Error in reading binary data");
		return 1;
	}

	for(i=0;i<count;i++)
		be2le(value+size*i,size);

	if(fread(&c,sizeof(char),1,fd)){
		if(c=='\r'){
			if(fread(&c,sizeof(char),1,fd)){
				if(c!='\n'){
					rw_mesh_set_error(*current_line,"Error in reading binary data. Skipped line ending symbols");
				}
			}
		}else if(c!='\n'){
			rw_mesh_set_error(*current_line,"Error in reading binary data. Skipped line ending symbols");
			return 1;
		}
	}

	return 0;
}

/**
 * Прочитать из файла fd count элементов типа type
 * @param fd дескриптор файла
 * @param count число элементов
 * @param type тип элементов
 * @param value массив в который будут прочитаны элементы
 * @param current_line номер текущей строки в файле
 * @param isBinary бинарный ли файл
 */
int _read_vtk_array_type(FILE*fd,int count,int type,void*value,int*current_line,int isBinary){
	if(isBinary){
		return _read_vtk_array_type_binary(fd,count,type,value,current_line);
	}else{
		return _read_vtk_array_type_ascii(fd,count,type,value,current_line);
	}
	return 0;
}

/**
 * Прочитать из файла fd count элементов типа type и записать из как REAL в массив value
 * @param fd дескриптор файла
 * @param count число элементов
 * @param type тип элементов
 * @param value массив в который будут прочитаны элементы
 * @param current_line номер текущей строки в файле
 * @param isBinary бинарный ли файл
 */
int _read_vtk_array_as_REAL_type(FILE*fd,int count,int type,REAL*value,int*current_line,int isBinary){
	int i,r;
	size_t size;
	void*buffer;
	size = _vtk_type_size(type);
	buffer = (void*)calloc(count,size);
	r = _read_vtk_array_type(fd,count,type,buffer,current_line,isBinary);
	if(r) return r;
	_vtk_convert_type_array_to_REAL(type,count,buffer,value);
	free(buffer);
	return 0;
}


struct RW_MESH_VTK_DATASET_STRUCT*_vtk_add_data(struct RW_MESH_VTK_STRUCT*VTK,int data_object){
	int d;
	int*CountOfData;
	struct RW_MESH_VTK_DATASET_STRUCT**Datas;
	struct RW_MESH_VTK_DATASET_STRUCT*Data;

	if(data_object==1){
		CountOfData = &(VTK->CountOfPointsData);
		Datas = &(VTK->PointsData);
	}else if(data_object==2){
		CountOfData = &(VTK->CountOfCellsData);
		Datas = &(VTK->CellsData);
	}else{
		CountOfData = &(VTK->CountOfData);
		Datas = &(VTK->Data);
	}

	if(*CountOfData){
		d = *CountOfData;
		*CountOfData = *CountOfData + 1;
		*Datas = (struct RW_MESH_VTK_DATASET_STRUCT*)realloc(*Datas,*CountOfData*sizeof(struct RW_MESH_VTK_DATASET_STRUCT));
	}else{
		d = 0;
		*CountOfData = 1;
		*Datas = (struct RW_MESH_VTK_DATASET_STRUCT*)calloc(*CountOfData,sizeof(struct RW_MESH_VTK_DATASET_STRUCT));
	}
	Data = *Datas+d;
	rw_mesh_vtk_dataset_struct_init(Data);
	return Data;
}

int _vtk_read_field_data(struct RW_MESH_VTK_STRUCT*VTK,int data_object,char*line,char*word,FILE*fd,int*current_line,int isBinary){
	struct RW_MESH_VTK_DATASET_STRUCT*Data;
	struct RW_MESH_VTK_DATA_FIELD_STRUCT*Field;
	int i,a,size;

	Data = _vtk_add_data(VTK,data_object);

	Data->DataType = RW_MESH_VTK_DATASET_TYPE_FIELD;
	Data->Counts = 1;
	Field = (struct RW_MESH_VTK_DATA_FIELD_STRUCT*)calloc(Data->Counts,sizeof(struct RW_MESH_VTK_DATA_FIELD_STRUCT));
	Data->Data = Field;

	//next word must be dataName
	string_cut_word(line,word);
	Data->DataName = (char*)calloc(strlen(word)+1,sizeof(char));
	strcpy(Data->DataName,word);

	Field->numArrays = 0;
	Field->Arrays = NULL;

	//next word must be numArrays
	string_cut_word(line,word);
	if(sscanf(word,"%d",&size)!=1){
		rw_mesh_set_error(*current_line,"Can't read size of FIELD");
		return 1;
	}
	Field->numArrays = size;

	if(Field->numArrays){
		Field->Arrays = (struct RW_MESH_VTK_DATA_FIELD_ARRAY_STRUCT*)calloc(Field->numArrays,sizeof(struct RW_MESH_VTK_DATA_FIELD_ARRAY_STRUCT));
		for(i=0;i<Field->numArrays;i++){
			//read arrays

			//read arrayNamei numComponents numTuples dataType
			read_line_trim(fd,line);(*current_line)++;
			// read arrayNamei
			string_cut_word(line,word);
			Field->Arrays[i].name = (char*)calloc(strlen(word)+1,sizeof(char));
			strcpy(Field->Arrays[i].name,word);

			//read numComponents
			string_cut_word(line,word);
			if(sscanf(word,"%d",&a)!=1){
				rw_mesh_set_error(*current_line,"Can't read numComponents of FIELD Array");
				return 1;
			}
			Field->Arrays[i].numComponents = a;

			//read numTuples
			string_cut_word(line,word);
			if(sscanf(word,"%d",&a)!=1){
				rw_mesh_set_error(*current_line,"Can't read numTuples of FIELD Array");
				return 1;
			}
			Field->Arrays[i].numTuples = a;

			//read dataType
			string_cut_word(line,word);
			Field->Arrays[i].dataType = _parse_vtk_type(word);

			//f 00 f 01 ... f 0(numComponents-1)
			if(Field->Arrays[i].numComponents*Field->Arrays[i].numTuples){
				Field->Arrays[i].values = _vtk_type_allocate(Field->Arrays[i].dataType,Field->Arrays[i].numComponents*Field->Arrays[i].numTuples);
				if(_read_vtk_array_type(fd,Field->Arrays[i].numComponents*Field->Arrays[i].numTuples,Field->Arrays[i].dataType,Field->Arrays[i].values,current_line,isBinary)){
					return 1;
				}
			}

		}
	}
	return 0;
}

int read_format_vtk_struct(struct RW_MESH_VTK_STRUCT*VTK,char filename[256],int flags){

	__save_locale;

	FILE*fd;
	int i,j,k;
	int a,b,c;
	int p,f,d;
	int rn;
	int size;
	int type;
	int data_object,data_size;
	int CountOfPoints,CountOfCells;
	int isBinary;
	int*buffer;

	long int ftell_position;

	struct RW_MESH_VTK_STRUCTURED_POINTS_STRUCT*	StructuredPoints;
	struct RW_MESH_VTK_STRUCTURED_GRID_STRUCT*		StructuredGrid;
	struct RW_MESH_VTK_RECTILINER_GRID_STRUCT*		RectilinerGrid;
	struct RW_MESH_VTK_POLYGONAL_DATA_STRUCT*		PolygonalData;
	struct RW_MESH_VTK_UNSTRUCTURED_GRID_STRUCT*	UnstructuredGrid;

	char prev_line[256];
	char line[256];
	char word[256];
	int current_line;
	int linepos;
	char line_error[256];

	StructuredPoints	= NULL;
	StructuredGrid		= NULL;
	RectilinerGrid		= NULL;
	PolygonalData		= NULL;
	UnstructuredGrid	= NULL;

	rw_mesh_set_filename(filename);
	fd=fopen(filename,"r");if(fd==NULL){
		rw_mesh_set_error(0,"Can't find file");
		return 1;
	}
	current_line = 0;

	rw_mesh_vtk_struct_init(VTK);

	//read # vtk DataFile Version x.x
	read_line_trim(fd,line); current_line++;

	rn=sscanf(line,"# vtk DataFile Version %d.%d",&i,&j);// Major and Minor Version
	if(rn!=2){
		rw_mesh_set_error(current_line,"Error in header line");
		return 1;
	}
	if(i<3){
		rw_mesh_set_error(current_line,"Version of VTK must be 3.0 or greater");
		return 1;
	}

	//read title
	read_line_trim(fd,line); current_line++;
	// we can use title string

	// read ASCII | BINARY
	read_line_trim(fd,line); current_line++;

	if(strcmp(line,"ASCII")!=0 && strcmp(line,"BINARY")!=0){
		rw_mesh_set_error(current_line,"Unknown format of file. It must be ASCII or BINARY");
		return 1;
	}
	if(strcmp(line,"BINARY")==0){
		isBinary = 1;
	}else{
		isBinary = 0;
	}

	//read DATASET type and DATASET
	read_line_trim(fd,line); current_line++;
	string_cut_word(line,word);
	if(strcmp(word,"DATASET")!=0){
		rw_mesh_set_error(current_line,"Line must be \"DATASET type\"");
		return 1;
	}

	string_cut_word(line,word);
	if(strcmp(word,"STRUCTURED_POINTS")==0){
		//RW_MESH_VTK_TYPE_STRUCTURED_POINTS;
		rw_mesh_set_error(current_line,"Type of DATASET is STRUCTURED_POINTS. It isn't supported in current version of reader");
		return 1;
	}else if(strcmp(word,"STRUCTURED_GRID")==0){
		//RW_MESH_VTK_TYPE_STRUCTURED_GRID;

		VTK->type = RW_MESH_VTK_TYPE_STRUCTURED_GRID;
		StructuredGrid = (void*)calloc(1,sizeof(struct RW_MESH_VTK_STRUCTURED_GRID_STRUCT));
		rw_mesh_vtk_structured_grid_struct_init(StructuredGrid);
		VTK->Grid = StructuredGrid;


		if(!read_line_skip_empty(fd,line,&current_line)){
			rw_mesh_set_error(current_line,"Missing points block");
			return 1;
		}
		string_cut_word(line,word);

		{// Dimenstions

			//DIMENSIONS n_x n_y n_z

			// read name of block
			if(strcmp(word,"DIMENSIONS")!=0){
				rw_mesh_set_error(current_line,"Missing dimentions");
				return 1;
			}

			string_cut_word(line,word);
			if(sscanf(word,"%d",&a)!=1 || a<1){
				rw_mesh_set_error(current_line,"Can't read Nx");
				return 1;
			}
			StructuredGrid->Nx = a;

			string_cut_word(line,word);
			if(sscanf(word,"%d",&a)!=1 || a<1){
				rw_mesh_set_error(current_line,"Can't read Ny");
				return 1;
			}
			StructuredGrid->Ny = a;

			string_cut_word(line,word);
			if(sscanf(word,"%d",&a)!=1 || a<1){
				rw_mesh_set_error(current_line,"Can't read Nz");
				return 1;
			}
			StructuredGrid->Nz = a;

			// POINTS n dataType


		}

		{// Points block

			// read name of block
			if(strcmp(word,"POINTS")!=0){
				rw_mesh_set_error(current_line,"Missing points block");
				return 1;
			}

			// first word POINTS
			// next word must be CountOfPoints
			string_cut_word(line,word);
			if(sscanf(word,"%d",&a)!=1){
				rw_mesh_set_error(current_line,"Can't read count of points");
				return 1;
			}
			CountOfPoints = a;

			if(CountOfPoints!=StructuredGrid->Nx*StructuredGrid->Ny*StructuredGrid->Nz){
				rw_mesh_set_error(current_line,"Count of points not equal Nx*Ny*Nz");
				return 1;
			}

			// the ending of line must be a vtk type
			type = _parse_vtk_type(line);
			if(type==RW_MESH_VTK_TYPE_NONE){
				rw_mesh_set_error(current_line,"Unknown points type");
				return 1;
			}

			StructuredGrid->Points = (REAL*)calloc(CountOfPoints*3,sizeof(REAL));
			if(_read_vtk_array_as_REAL_type(fd,CountOfPoints*3,type,StructuredGrid->Points,&current_line,isBinary)){
				return 1;
			}

			CountOfPoints = StructuredGrid->Nx*StructuredGrid->Ny*StructuredGrid->Nz;
			CountOfCells = (StructuredGrid->Nx-1)*(StructuredGrid->Ny-1)*(StructuredGrid->Nz-1);

		}


	}else if(strcmp(word,"RECTILINEAR_GRID")==0){
		//RW_MESH_VTK_TYPE_RECTILINEAR_GRID;
		rw_mesh_set_error(current_line,"Type of DATASET is RECTILINEAR_GRID. It isn't supported in current version of reader");
		return 1;
	}else if(strcmp(word,"POLYDATA")==0){
		//RW_MESH_VTK_TYPE_POLYDATA;
		rw_mesh_set_error(current_line,"Type of DATASET is POLYDATA. It isn't supported in current version of reader");
		return 1;
	}else if(strcmp(word,"UNSTRUCTURED_GRID")==0){
		//RW_MESH_VTK_TYPE_UNSTRUCTURED_GRID;

		VTK->type = RW_MESH_VTK_TYPE_UNSTRUCTURED_GRID;
		UnstructuredGrid = (void*)calloc(1,sizeof(struct RW_MESH_VTK_UNSTRUCTURED_GRID_STRUCT));
		rw_mesh_vtk_unstructured_grid_struct_init(UnstructuredGrid);
		VTK->Grid = UnstructuredGrid;

		if(!read_line_skip_empty(fd,line,&current_line)){
			rw_mesh_set_error(current_line,"Missing points block");
			return 1;
		}
		string_cut_word(line,word);

		if(strcmp(word,"FIELD")==0){
			//it may be FIELD data before Points and Cells
			//TODO binary
			if(_vtk_read_field_data(VTK,RW_MESH_VTK_DATA_OBJECT_NONE,line,word,fd,&current_line,isBinary)){
				return 1;
			}

			if(!read_line_skip_empty(fd,line,&current_line)){
				rw_mesh_set_error(current_line,"Missing points block");
				return 1;
			}
			string_cut_word(line,word);
		}


		{// Points block

			// read name of block
			if(strcmp(word,"POINTS")!=0){
				rw_mesh_set_error(current_line,"Missing points block");
				return 1;
			}

			// first word POINTS
			// next word must be CountOfPoints
			string_cut_word(line,word);
			if(sscanf(word,"%d",&a)!=1){
				rw_mesh_set_error(current_line,"Can't read count of points");
				return 1;
			}
			UnstructuredGrid->CountOfPoints = a;

			// the ending of line must be a vtk type
			type = _parse_vtk_type(line);
			if(type==RW_MESH_VTK_TYPE_NONE){
				rw_mesh_set_error(current_line,"Unknown points type");
				return 1;
			}

			UnstructuredGrid->Points = (REAL*)calloc(UnstructuredGrid->CountOfPoints*3,sizeof(REAL));
			if(_read_vtk_array_as_REAL_type(fd,UnstructuredGrid->CountOfPoints*3,type,UnstructuredGrid->Points,&current_line,isBinary)){
				return 1;
			}

			CountOfPoints = UnstructuredGrid->CountOfPoints;
		}

		{// Cells block
			if(!read_line_skip_empty(fd,line,&current_line)){
				rw_mesh_set_error(current_line,"Missing cells block");
				return 1;
			}

			string_cut_word(line,word);
			if(strcmp(word,"CELLS")!=0){
				rw_mesh_set_error(current_line,"Missing cells block");
				return 1;
			}

			if(UnstructuredGrid->CountOfCells){
				rw_mesh_set_error(current_line,"Cells block already reading");
				return 1;
			}

			// first word CELLS
			//next word must be CountOfCells
			string_cut_word(line,word);
			if(sscanf(word,"%d",&a)!=1){
				rw_mesh_set_error(current_line,"Can't read count of cells");
				return 1;
			}
			UnstructuredGrid->CountOfCells = a;

			//next word must be size
			string_cut_word(line,word);
			if(sscanf(word,"%d",&size)!=1){
				rw_mesh_set_error(current_line,"Can't read count of cell");
				return 1;
			}

			if(UnstructuredGrid->CountOfCells){

				size = size - UnstructuredGrid->CountOfCells;

				UnstructuredGrid->CellOffset = (int*)calloc(UnstructuredGrid->CountOfCells+1,sizeof(int));
				UnstructuredGrid->CellSizes = (int*)calloc(UnstructuredGrid->CountOfCells,sizeof(int));
				UnstructuredGrid->Cells = (int*)calloc(size,sizeof(int));

				UnstructuredGrid->CellOffset[0] = 0;

				if(isBinary){
					buffer = (int*)calloc(size+UnstructuredGrid->CountOfCells,sizeof(int));
					if(_read_vtk_array_type_binary(fd,size+UnstructuredGrid->CountOfCells,RW_MESH_VTK_TYPE_INT,buffer,&current_line)){
						rw_mesh_set_error(current_line,"Can't read cells");
						return 1;
					}

					for(i=j=0;i<UnstructuredGrid->CountOfCells;i++){
						UnstructuredGrid->CellSizes[i] = buffer[j]; j++;
						UnstructuredGrid->CellOffset[i+1] = UnstructuredGrid->CellOffset[i]+UnstructuredGrid->CellSizes[i];
						for(k=0;k<UnstructuredGrid->CellSizes[i];k++){
							UnstructuredGrid->Cells[UnstructuredGrid->CellOffset[i]+k] = buffer[j]; j++;
						}
					}
					free(buffer);
				}else{
					for(i=0;i<UnstructuredGrid->CountOfCells;i++){
						//read numPointsi , p, j, k, l, ...
						read_line_trim(fd,line); current_line++;

						string_cut_word(line,word);
						if(sscanf(word,"%d",&a)!=1){
							rw_mesh_set_error(current_line,"Can't read cell size");
							return 1;
						}
						UnstructuredGrid->CellSizes[i] = a;

						UnstructuredGrid->CellOffset[i+1] = UnstructuredGrid->CellOffset[i]+UnstructuredGrid->CellSizes[i];
						if(i+1==UnstructuredGrid->CountOfCells){
							if(UnstructuredGrid->CellOffset[i+1] != size){
								rw_mesh_set_error(current_line,"Summary size of cells don't equal size of cells");
								return 1;
							}
						}else{
							if(UnstructuredGrid->CellOffset[i+1] > size){
								rw_mesh_set_error(current_line,"Summary size of cells more than size of cells");
								return 1;
							}
						}
						for(j=0;j<UnstructuredGrid->CellSizes[i];j++){
							string_cut_word(line,word);
							if(sscanf(word,"%d",&a)!=1){
								rw_mesh_set_error(current_line,"Can't read cell vertex");
								return 1;
							}
							UnstructuredGrid->Cells[UnstructuredGrid->CellOffset[i]+j] = a;
						}
					}
				}
				CountOfCells = UnstructuredGrid->CountOfCells;
			}

			{// Cell types block
				if(!read_line_skip_empty(fd,line,&current_line)){
					rw_mesh_set_error(current_line,"Missing cell types block");
					return 1;
				}

				string_cut_word(line,word);
				if(strcmp(word,"CELL_TYPES")!=0){
					rw_mesh_set_error(current_line,"Missing cell types block");
					return 1;
				}

				// first word CELL_TYPES
				if(UnstructuredGrid->CountOfCells==0){
					rw_mesh_set_error(current_line,"Block CELL_TYPES can't be before block CELLS");
					return 1;
				}

				//next word must be CountOfCells
				string_cut_word(line,word);
				if(sscanf(word,"%d",&a)!=1){
					rw_mesh_set_error(current_line,"Can't read count of cells (CELL_TYPES)");
					return 1;
				}
				if(UnstructuredGrid->CountOfCells != a){
					rw_mesh_set_error(current_line,"Count of cells type don't equal count of cells");
					return 1;
				}

				UnstructuredGrid->CellTypes = (int*)calloc(UnstructuredGrid->CountOfCells,sizeof(int));

				if(isBinary){
					if(_read_vtk_array_type_binary(fd,UnstructuredGrid->CountOfCells,RW_MESH_VTK_TYPE_INT,UnstructuredGrid->CellTypes,&current_line)){
						rw_mesh_set_error(current_line,"Can't read count of cell type");
						return 1;
					}
				}else{
					for(i=0;i<UnstructuredGrid->CountOfCells;i++){
						//read type_i
						read_line_trim(fd,line); current_line++;
						if(sscanf(line,"%d",&a)!=1){
							rw_mesh_set_error(current_line,"Can't read count of cell type");
							return 1;
						}
						UnstructuredGrid->CellTypes[i] = a;
					}
				}
			}

		}

	}else if(strcmp(word,"FIELD")==0){
		RW_MESH_VTK_TYPE_FIELD;
		rw_mesh_set_error(current_line,"Type of DATASET is FIELD. It isn't supported in current version of reader");
		return 1;
	}else{
		rw_mesh_set_error(current_line,"Unknown type of DATASET. It must be STRUCTURED_POINTS, STRUCTURED_GRID, RECTILINEAR_GRID, POLYDATA, UNSTRUCTURED_GRID or FIELD");
		return 1;
	}

	data_object = RW_MESH_VTK_DATA_OBJECT_NONE;
	data_size = 0;

	// read blocks
	while(read_line_skip_empty(fd,line,&current_line)>=0){
		// read name of block
		string_cut_word(line,word);
		if(strcmp(word,"POINT_DATA")==0){
			//read countOfPoints
			string_cut_word(line,word);
			if(sscanf(word,"%d",&a)!=1){
				rw_mesh_set_error(current_line,"Can't read countOfPoints of POINT_DATA");
				return 1;
			}
			if(CountOfPoints==0){
				rw_mesh_set_error(current_line,"POINT_DATA block must be after POINTS block");
				return 1;
			}
			if(a!=CountOfPoints){
				rw_mesh_set_error(current_line,"POINT_DATA countOfPoints don't equal POINTS countOfPoints");
				return 1;
			}
			data_object = RW_MESH_VTK_DATA_OBJECT_POINTS;
			data_size = a;
		}else if(strcmp(word,"CELL_DATA")==0){
			//read countOfCells
			string_cut_word(line,word);
			if(sscanf(word,"%d",&a)!=1){
				rw_mesh_set_error(current_line,"Can't read countOfCells of CELL_DATA");
				return 1;
			}
			if(CountOfCells==0){
				rw_mesh_set_error(current_line,"CELL_DATA block must be after CELLS block");
				return 1;
			}
			if(a!=CountOfCells){
				rw_mesh_set_error(current_line,"CELL_DATA countOfCells don't equal CELLS countOfCells");
				return 1;
			}
			data_object = RW_MESH_VTK_DATA_OBJECT_CELLS;
			data_size = a;
		}else if(strcmp(word,"FIELD")==0){
			// first word FIELD

			_vtk_read_field_data(VTK,data_object,line,word,fd,&current_line,isBinary);

		}else if(strcmp(word,"SCALARS")==0){
			// first word SCALARS
			struct RW_MESH_VTK_DATASET_STRUCT*Data;
			struct RW_MESH_VTK_DATA_SCALARS_STRUCT*Scalars;

			Data = _vtk_add_data(VTK,data_object);
			Data->DataType = RW_MESH_VTK_DATASET_TYPE_SCALARS;
			Data->Counts = data_size;
			Scalars = (struct RW_MESH_VTK_DATA_SCALARS_STRUCT*)calloc(Data->Counts,sizeof(struct RW_MESH_VTK_DATA_SCALARS_STRUCT));
			Data->Data = Scalars;

			//next word must be dataName
			string_cut_word(line,word);
			Data->DataName = (char*)calloc(strlen(word)+1,sizeof(char));
			strcpy(Data->DataName,word);

			//next word must be type
			string_cut_word(line,word);
			Scalars->dataType = _parse_vtk_type(word);

			if(Scalars->dataType == RW_MESH_VTK_TYPE_NONE){
				rw_mesh_set_error(current_line,"Unknown type");
				return 1;
			}

			//next word MAY be numComp
			if(sscanf(line,"%u",&a)==1){
				Scalars->numComp = a;
			}else{
				Scalars->numComp = 1;
			}

			ftell_position = ftell(fd);

			//try to read line with LOOKUP_TABLE
			read_line_trim(fd,line);current_line++;
			string_get_word(line,word);
			if(strcmp(word,"LOOKUP_TABLE")==0){
				// yes it us LOOKUP_TABLE, read it
				string_cut_word(line,word);
				string_cut_word(line,word);
				Scalars->LookupTable = (char*)calloc(strlen(word)+1,sizeof(char));
				strcpy(Scalars->LookupTable,word);
			}else{
				// no, come back and read data
				current_line--;
				fseek(fd,ftell_position,SEEK_SET);
			}

			Scalars->values = _vtk_type_allocate(Scalars->dataType,Data->Counts*Scalars->numComp);
			if(_read_vtk_array_type(fd,Data->Counts*Scalars->numComp,Scalars->dataType,Scalars->values,&current_line,isBinary)){
				return 1;
			}
		}else{
			if( (flags&RW_MESH_VTK_SKIP_UNKNOWN)){
				//do nothing, skip line
			}else{
				rw_mesh_set_error(current_line,"Unknown block");
				return 1;
			}
		}
	}


	return 0;
}

int _vtk_type_is_mask(int type){
	if(type==RW_MESH_VTK_TYPE_BIT)return 1;
	if(type==RW_MESH_VTK_TYPE_UNSIGNED_CHAR)return 1;
	if(type==RW_MESH_VTK_TYPE_CHAR)return 1;
	if(type==RW_MESH_VTK_TYPE_UNSIGNED_SHORT)return 1;
	if(type==RW_MESH_VTK_TYPE_SHORT)return 1;
	if(type==RW_MESH_VTK_TYPE_UNSIGNED_INT)return 1;
	if(type==RW_MESH_VTK_TYPE_INT)return 1;
	if(type==RW_MESH_VTK_TYPE_UNSIGNED_LONG)return 1;
	if(type==RW_MESH_VTK_TYPE_LONG)return 1;
	return 0;
}

void _write_mask(int MaskSize,int*Mask,int size,int offset,int type,void*data,int data_size,int data_offset){
	int i;
	for(i=0;i<MaskSize;i++){
		switch(type){
		case RW_MESH_VTK_TYPE_BIT:				Mask[size*i+offset] = ((char*)data)[data_size*i+offset];break;
		case RW_MESH_VTK_TYPE_UNSIGNED_CHAR:	Mask[size*i+offset] = ((char*)data)[data_size*i+offset];break;
		case RW_MESH_VTK_TYPE_CHAR:				Mask[size*i+offset] = ((char*)data)[data_size*i+offset];break;
		case RW_MESH_VTK_TYPE_UNSIGNED_SHORT:	Mask[size*i+offset] = ((unsigned short*)data)[data_size*i+offset];break;
		case RW_MESH_VTK_TYPE_SHORT:			Mask[size*i+offset] = ((short*)data)[data_size*i+offset];break;
		case RW_MESH_VTK_TYPE_UNSIGNED_INT:		Mask[size*i+offset] = ((unsigned int*)data)[data_size*i+offset];break;
		case RW_MESH_VTK_TYPE_INT:				Mask[size*i+offset] = ((int*)data)[data_size*i+offset];break;
		case RW_MESH_VTK_TYPE_UNSIGNED_LONG:	Mask[size*i+offset] = ((unsigned long int*)data)[data_size*i+offset];break;
		case RW_MESH_VTK_TYPE_LONG:				Mask[size*i+offset] = ((long int*)data)[data_size*i+offset];break;
		}
	}
}

int _simplify_data_to_masks(int MaskSize,int CountOfData,struct RW_MESH_VTK_DATASET_STRUCT*Datas,int*CountOfMasks, int**Masks){
	int i,j,k,n,N;
	n=0;
	for(i=0;i<CountOfData;i++){
		if(Datas[i].DataType == RW_MESH_VTK_DATASET_TYPE_FIELD){
			for(j=0;j<((struct RW_MESH_VTK_DATA_FIELD_STRUCT*)Datas[i].Data)->numArrays;j++){
				if(_vtk_type_is_mask(((struct RW_MESH_VTK_DATA_FIELD_STRUCT*)Datas[i].Data)->Arrays[j].dataType)){
					n += ((struct RW_MESH_VTK_DATA_FIELD_STRUCT*)Datas[i].Data)->Arrays[j].numComponents;
				}
			}
		}else if(Datas[i].DataType == RW_MESH_VTK_DATASET_TYPE_SCALARS){
			if(_vtk_type_is_mask(((struct RW_MESH_VTK_DATA_SCALARS_STRUCT*)Datas[i].Data)->dataType)){
				n += ((struct RW_MESH_VTK_DATA_SCALARS_STRUCT*)Datas[i].Data)->numComp;
			}
		}
	}

	if(n==0){
		if(CountOfMasks)*CountOfMasks = 0;
		if(Masks)*Masks = NULL;
		return 0;
	}

	if(CountOfMasks)*CountOfMasks = n;
	if(!Masks) return 0;
	N=n;
	*Masks = (int*)calloc(n*MaskSize,sizeof(int));

	n=0;
	for(i=0;i<CountOfData;i++){
		if(Datas[i].DataType == RW_MESH_VTK_DATASET_TYPE_FIELD){
			for(j=0;j<((struct RW_MESH_VTK_DATA_FIELD_STRUCT*)Datas[i].Data)->numArrays;j++){
				if(_vtk_type_is_mask(((struct RW_MESH_VTK_DATA_FIELD_STRUCT*)Datas[i].Data)->Arrays[j].dataType)){
					for(k=0;k<((struct RW_MESH_VTK_DATA_FIELD_STRUCT*)Datas[i].Data)->Arrays[j].numComponents;k++){
						_write_mask(MaskSize,*Masks,N,n,((struct RW_MESH_VTK_DATA_FIELD_STRUCT*)Datas[i].Data)->Arrays[j].dataType,
								((struct RW_MESH_VTK_DATA_FIELD_STRUCT*)Datas[i].Data)->Arrays[j].values,((struct RW_MESH_VTK_DATA_FIELD_STRUCT*)Datas[i].Data)->Arrays[j].numComponents,k);
						n++;
					}
				}
			}
		}else if(Datas[i].DataType == RW_MESH_VTK_DATASET_TYPE_SCALARS){
			if(_vtk_type_is_mask(((struct RW_MESH_VTK_DATA_SCALARS_STRUCT*)Datas[i].Data)->dataType)){
				for(k=0;k<((struct RW_MESH_VTK_DATA_SCALARS_STRUCT*)Datas[i].Data)->numComp;k++){
					_write_mask(MaskSize,*Masks,N,n,((struct RW_MESH_VTK_DATA_SCALARS_STRUCT*)Datas[i].Data)->dataType,
							((struct RW_MESH_VTK_DATA_SCALARS_STRUCT*)Datas[i].Data)->values,((struct RW_MESH_VTK_DATA_SCALARS_STRUCT*)Datas[i].Data)->numComp,k);
					n++;
				}
			}
		}
	}
	return 0;
}

int _vtk_type_is_function(int type){
	if(type==RW_MESH_VTK_TYPE_FLOAT)return 1;
	if(type==RW_MESH_VTK_TYPE_DOUBLE)return 1;
	return 0;
}

void _write_function(int FunctionSize,REAL*Function,int size,int offset,int type,void*data,int data_size,int data_offset){
	int i;
	for(i=0;i<FunctionSize;i++){
		switch(type){
		case RW_MESH_VTK_TYPE_FLOAT:	Function[size*i+offset] = ((float*)data)[data_size*i+data_offset];break;
		case RW_MESH_VTK_TYPE_DOUBLE:	Function[size*i+offset] = ((double*)data)[data_size*i+data_offset];break;
		}
	}
}



int _simplify_data_to_functions(int FunctionSize,int CountOfData,struct RW_MESH_VTK_DATASET_STRUCT*Datas,int*CountOfFunctions, REAL**Functions){
	int i,j,k,n,N;
	n=0;
	for(i=0;i<CountOfData;i++){
		if(Datas[i].DataType == RW_MESH_VTK_DATASET_TYPE_FIELD){
			for(j=0;j<((struct RW_MESH_VTK_DATA_FIELD_STRUCT*)Datas[i].Data)->numArrays;j++){
				if(_vtk_type_is_function(((struct RW_MESH_VTK_DATA_FIELD_STRUCT*)Datas[i].Data)->Arrays[j].dataType)){
					n += ((struct RW_MESH_VTK_DATA_FIELD_STRUCT*)Datas[i].Data)->Arrays[j].numComponents;
				}
			}
		}else if(Datas[i].DataType == RW_MESH_VTK_DATASET_TYPE_SCALARS){
			if(_vtk_type_is_function(((struct RW_MESH_VTK_DATA_SCALARS_STRUCT*)Datas[i].Data)->dataType)){
				n += ((struct RW_MESH_VTK_DATA_SCALARS_STRUCT*)Datas[i].Data)->numComp;
			}
		}
	}

	if(n==0){
		if(CountOfFunctions)*CountOfFunctions = 0;
		if(Functions)*Functions = NULL;
		return 0;
	}

	if(CountOfFunctions)*CountOfFunctions = n;
	if(!Functions) return 0;
	N=n;
	*Functions = (REAL*)calloc(n*FunctionSize,sizeof(REAL));

	n=0;
	for(i=0;i<CountOfData;i++){
		if(Datas[i].DataType == RW_MESH_VTK_DATASET_TYPE_FIELD){
			for(j=0;j<((struct RW_MESH_VTK_DATA_FIELD_STRUCT*)Datas[i].Data)->numArrays;j++){
				if(_vtk_type_is_function(((struct RW_MESH_VTK_DATA_FIELD_STRUCT*)Datas[i].Data)->Arrays[j].dataType)){
					for(k=0;k<((struct RW_MESH_VTK_DATA_FIELD_STRUCT*)Datas[i].Data)->Arrays[j].numComponents;k++){
						_write_function(FunctionSize,*Functions,N,n,((struct RW_MESH_VTK_DATA_FIELD_STRUCT*)Datas[i].Data)->Arrays[j].dataType,
								((struct RW_MESH_VTK_DATA_FIELD_STRUCT*)Datas[i].Data)->Arrays[j].values,((struct RW_MESH_VTK_DATA_FIELD_STRUCT*)Datas[i].Data)->Arrays[j].numComponents,k);
						n++;
					}
				}
			}
		}else if(Datas[i].DataType == RW_MESH_VTK_DATASET_TYPE_SCALARS){
			if(_vtk_type_is_function(((struct RW_MESH_VTK_DATA_SCALARS_STRUCT*)Datas[i].Data)->dataType)){
				for(k=0;k<((struct RW_MESH_VTK_DATA_SCALARS_STRUCT*)Datas[i].Data)->numComp;k++){
					_write_function(FunctionSize,*Functions,N,n,((struct RW_MESH_VTK_DATA_SCALARS_STRUCT*)Datas[i].Data)->dataType,
							((struct RW_MESH_VTK_DATA_SCALARS_STRUCT*)Datas[i].Data)->values,((struct RW_MESH_VTK_DATA_SCALARS_STRUCT*)Datas[i].Data)->numComp,k);
					n++;
				}
			}
		}
	}
	return 0;
}

int read_format_vtk_unstructured_simplified(
		int*CountOfPoints, REAL3**Points,
		int*CountOfPointMasks, int**PointMasks,
		int*CountOfPointFunctions, REAL**PointFunctions,

		int*CountOfCells,int**Cells,int**CellSizes,int**CellTypes,int**CellOffset,
		int*CountOfCellMasks, int**CellMasks,
		int*CountOfCellFunctions, REAL**CellFunctions,

		char*filename,int flags){

	int i,j,k;
	int rn;
	struct RW_MESH_VTK_STRUCT*VTK;
	struct RW_MESH_VTK_UNSTRUCTURED_GRID_STRUCT*UnstructedGrid;

	VTK = (struct RW_MESH_VTK_STRUCT*)calloc(1,sizeof(struct RW_MESH_VTK_STRUCT));
	rw_mesh_vtk_struct_init(VTK);

	rn = read_format_vtk_struct(VTK,filename,flags);
	if(rn){
		rw_mesh_vtk_struct_free(VTK);
		free(VTK);
		return 1;
	}

	if(VTK->type != RW_MESH_VTK_TYPE_UNSTRUCTURED_GRID){
		rw_mesh_set_error(0,"It isn't unstructed grid, it's some other type of grid");
		return 1;
	}

	UnstructedGrid = VTK->Grid;

	if(CountOfPoints){
		*CountOfPoints = UnstructedGrid->CountOfPoints;
	}

	if(Points){
		if(UnstructedGrid->CountOfPoints){
			*Points = (REAL3*)calloc(UnstructedGrid->CountOfPoints,sizeof(REAL3));
			memcpy(*Points,UnstructedGrid->Points,UnstructedGrid->CountOfPoints*3*sizeof(REAL));
		}else{
			*Points = NULL;
		}
	}

	if(VTK->CountOfPointsData){
		_simplify_data_to_masks(UnstructedGrid->CountOfPoints,VTK->CountOfPointsData,VTK->PointsData,CountOfPointMasks,PointMasks);
		_simplify_data_to_functions(UnstructedGrid->CountOfPoints,VTK->CountOfPointsData,VTK->PointsData,CountOfPointFunctions,PointFunctions);
	}else{
		if(CountOfPointMasks)*CountOfPointMasks=0;
		if(PointMasks)*PointMasks=NULL;
		if(CountOfPointFunctions)*CountOfPointFunctions=0;
		if(PointFunctions)*PointFunctions=NULL;
	}

	if(CountOfCells){
		*CountOfCells = UnstructedGrid->CountOfCells;
	}

	if(Cells){
		if(UnstructedGrid->CountOfCells && UnstructedGrid->Cells && UnstructedGrid->CellOffset && UnstructedGrid->CellOffset[UnstructedGrid->CountOfCells]){
			*Cells = (int*)calloc(UnstructedGrid->CellOffset[UnstructedGrid->CountOfCells],sizeof(int));
			memcpy(*Cells,UnstructedGrid->Cells,UnstructedGrid->CellOffset[UnstructedGrid->CountOfCells]*sizeof(int));
		}else{
			*Cells = NULL;
		}
	}

	if(CellSizes){
		if(UnstructedGrid->CountOfCells && UnstructedGrid->CellSizes){
			*CellSizes = (int*)calloc(UnstructedGrid->CountOfCells,sizeof(int));
			memcpy(*CellSizes,UnstructedGrid->CellSizes,UnstructedGrid->CountOfCells*sizeof(int));
		}else{
			*CellSizes = NULL;
		}
	}

	if(CellTypes){
		if(UnstructedGrid->CountOfCells && UnstructedGrid->CellTypes){
			*CellTypes = (int*)calloc(UnstructedGrid->CountOfCells,sizeof(int));
			memcpy(*CellTypes,UnstructedGrid->CellTypes,UnstructedGrid->CountOfCells*sizeof(int));
		}else{
			*CellTypes = NULL;
		}
	}

	if(CellOffset){
		if(UnstructedGrid->CountOfCells && UnstructedGrid->CellOffset){
			*CellOffset = (int*)calloc(UnstructedGrid->CountOfCells+1,sizeof(int));
			memcpy(*CellOffset,UnstructedGrid->CellOffset,(UnstructedGrid->CountOfCells+1)*sizeof(int));
		}else{
			*CellOffset = NULL;
		}
	}

	if(VTK->CountOfCellsData){
		_simplify_data_to_masks(UnstructedGrid->CountOfCells,VTK->CountOfCellsData,VTK->CellsData,CountOfCellMasks,CellMasks);
		_simplify_data_to_functions(UnstructedGrid->CountOfCells,VTK->CountOfCellsData,VTK->CellsData,CountOfCellFunctions,CellFunctions);
	}else{
		if(CountOfCellMasks)*CountOfCellMasks=0;
		if(CellMasks)*CellMasks=NULL;
		if(CountOfCellFunctions)*CountOfCellFunctions=0;
		if(CellFunctions)*CellFunctions=NULL;
	}

	return 0;
}


int _get_data_type_name(int DataType,char*string){
	switch(DataType){
	case RW_MESH_VTK_TYPE_BIT:
		strcpy(string,"bit");
		break;
	case RW_MESH_VTK_TYPE_UNSIGNED_CHAR:
		strcpy(string,"unsigned_char");
		break;
	case RW_MESH_VTK_TYPE_CHAR:
		strcpy(string,"char");
		break;
	case RW_MESH_VTK_TYPE_UNSIGNED_SHORT:
		strcpy(string,"unsigned_short");
		break;
	case RW_MESH_VTK_TYPE_SHORT:
		strcpy(string,"short");
		break;
	case RW_MESH_VTK_TYPE_UNSIGNED_INT:
		strcpy(string,"unsigned_int");
		break;
	case RW_MESH_VTK_TYPE_INT:
		strcpy(string,"int");
		break;
	case RW_MESH_VTK_TYPE_UNSIGNED_LONG:
		strcpy(string,"unsigned_long");
		break;
	case RW_MESH_VTK_TYPE_LONG:
		strcpy(string,"long");
		break;
	case RW_MESH_VTK_TYPE_FLOAT:
		strcpy(string,"float");
		break;
	case RW_MESH_VTK_TYPE_DOUBLE:
		strcpy(string,"double");
		break;
	default:
		strcpy(string,"unknown");
	}
	return 0;
}

int _write_array_data_type_binary(FILE*fd,void*Data,int Count,int DataType){
#define _write(type) data = malloc(sizeof(type));for(i=0;i<Count;i++){*((type*)data) = ((type*)Data)[i];le2be(data,sizeof(type));fwrite(data,sizeof(type),1,fd);}free(data);
	int i;
	char c;
	void*data;

	switch(DataType){
	case RW_MESH_VTK_TYPE_BIT:
		for(i=0;i<Count;i++){
			c = ((char*)Data)[i]?'1':'0';
			fwrite(&c,sizeof(char),1,fd);
		}
		break;
	case RW_MESH_VTK_TYPE_UNSIGNED_CHAR:
		_write(char);
		break;
	case RW_MESH_VTK_TYPE_CHAR:
		_write(char);
		break;
	case RW_MESH_VTK_TYPE_UNSIGNED_SHORT:
		_write(unsigned short);
		break;
	case RW_MESH_VTK_TYPE_SHORT:
		_write(short);
		break;
	case RW_MESH_VTK_TYPE_UNSIGNED_INT:
		_write(unsigned int);
		break;
	case RW_MESH_VTK_TYPE_INT:
		_write(int);
		break;
	case RW_MESH_VTK_TYPE_UNSIGNED_LONG:
		_write(unsigned long);
		break;
	case RW_MESH_VTK_TYPE_LONG:
		_write(long);
		break;
	case RW_MESH_VTK_TYPE_FLOAT:
		_write(float);
		break;
	case RW_MESH_VTK_TYPE_DOUBLE:
		_write(double);
		break;
	default:
		fwrite(Data,sizeof(int),Count,fd);
		break;
	}
#undef _write
	c = '\n';
	fwrite(&c,sizeof(char),1,fd);

	return 0;
}

int _write_array_data_type(FILE*fd,void*Data,int Count,int DataType,int numsOnRow){
#define _write(format,what) for(i=0;i<Count;i++) fprintf(fd,format "%c",(what),((i+1)%numsOnRow && ((i+1)<Count))?' ':'\n');
	int i;
	switch(DataType){
	case RW_MESH_VTK_TYPE_BIT:
		_write("%c",((char*)Data)[i]?'1':'0');
		break;
	case RW_MESH_VTK_TYPE_UNSIGNED_CHAR:
		_write("%c",((char*)Data)[i]);
		break;
	case RW_MESH_VTK_TYPE_CHAR:
		_write("%c",((char*)Data)[i]);
		break;
	case RW_MESH_VTK_TYPE_UNSIGNED_SHORT:
		_write("%hu",((unsigned short*)Data)[i]);
		break;
	case RW_MESH_VTK_TYPE_SHORT:
		_write("%hd",((short*)Data)[i]);
		break;
	case RW_MESH_VTK_TYPE_UNSIGNED_INT:
		_write("%u",((unsigned int*)Data)[i]);
		break;
	case RW_MESH_VTK_TYPE_INT:
		_write("%d",((int*)Data)[i]);
		break;
	case RW_MESH_VTK_TYPE_UNSIGNED_LONG:
		_write("%lu",((unsigned long*)Data)[i]);
		break;
	case RW_MESH_VTK_TYPE_LONG:
		_write("%ld",((long*)Data)[i]);
		break;
	case RW_MESH_VTK_TYPE_FLOAT:
		_write("%f",((float*)Data)[i]);
		break;
	case RW_MESH_VTK_TYPE_DOUBLE:
		_write("%lf",((double*)Data)[i]);
		break;
	default:
		_write("%d",((int*)Data)[i]);
		break;
	}
#undef _write
	return 0;
}
int _write_data(FILE*fd,struct RW_MESH_VTK_DATASET_STRUCT*Data,int isBinary){
	int i;
	char string[256];
	struct RW_MESH_VTK_DATA_SCALARS_STRUCT*Scalar;
	struct RW_MESH_VTK_DATA_FIELD_STRUCT*Field;
	struct RW_MESH_VTK_DATA_VECTORS_STRUCT*Vector;

	switch(Data->DataType){
	case RW_MESH_VTK_DATASET_TYPE_NONE:
		break;
	case RW_MESH_VTK_DATASET_TYPE_SCALARS:
		fprintf(fd,"SCALARS");
		Scalar = (struct RW_MESH_VTK_DATA_SCALARS_STRUCT*)Data->Data;

		//dataName
		if(Data->DataName){
			fprintf(fd," %s",Data->DataName);
		}else{
			// если нет имени, пишем вместо имени адресс
			fprintf(fd," %p",Data);
		}

		//dataType
		_get_data_type_name(Scalar->dataType,string);
		fprintf(fd," %s",string);

		if(Scalar->numComp>1){
			//numComp
			fprintf(fd," %d",Scalar->numComp);
		}else{
			//default 1
		}
		fprintf(fd,"\n");

		if(Scalar->LookupTable){
			fprintf(fd,"LOOKUP_TABLE %s\n",Scalar->LookupTable);
		}else{
			//fprintf(fd,"LOOKUP_TABLE default\n");
		}

		if(isBinary){
			_write_array_data_type_binary(fd,Scalar->values,Scalar->numComp*Data->Counts,Scalar->dataType);
		}else{
			_write_array_data_type(fd,Scalar->values,Scalar->numComp*Data->Counts,Scalar->dataType,RW_MESH_VTK_DEFAULT_NUMS_ON_ROW);
		}

		break;
	case RW_MESH_VTK_DATASET_TYPE_COLOR_SCALARS:
		//fprintf(fd,"COLOR_SCALARS");
		break;
	case RW_MESH_VTK_DATASET_TYPE_LOOKUP_TABLE:
		//fprintf(fd,"LOOKUP_TABLE");
		break;
	case RW_MESH_VTK_DATASET_TYPE_VECTORS:
		//fprintf(fd,"VECTORS");

		fprintf(fd,"VECTORS");

		Vector = (struct RW_MESH_VTK_DATA_VECTORS_STRUCT*)Data->Data;

		//dataName
		if(Data->DataName){
			fprintf(fd," %s",Data->DataName);
		}else{
			// если нет имени, пишем вместо имени адресс
			fprintf(fd," %p",Data);
		}

		//dataType
		_get_data_type_name(Vector->dataType,string);
		fprintf(fd," %s\n",string);

		if(isBinary){
			_write_array_data_type_binary(fd,Vector->values,Data->Counts*3,Vector->dataType);
		}else{
			_write_array_data_type(fd,Vector->values,Data->Counts*3,Vector->dataType,RW_MESH_VTK_DEFAULT_NUMS_ON_ROW);
		}

		break;
	case RW_MESH_VTK_DATASET_TYPE_NORMALS:
		//fprintf(fd,"NORMALS");
		break;
	case RW_MESH_VTK_DATASET_TYPE_TEXTURE_COORDINATES:
		//fprintf(fd,"TEXTURE_COORDINATES");
		break;
	case RW_MESH_VTK_DATASET_TYPE_TENSORS:
		//fprintf(fd,"TENSORS");
		break;
	case RW_MESH_VTK_DATASET_TYPE_FIELD:
		fprintf(fd,"FIELD");

		Field = (struct RW_MESH_VTK_DATA_FIELD_STRUCT*)Data->Data;

		//dataName
		if(Data->DataName){
			fprintf(fd," %s",Data->DataName);
		}else{
			// если нет имени, пишем вместо имени адресс
			fprintf(fd," %p",Data);
		}

		//numArrays
		fprintf(fd," %d\n",Field->numArrays);

		for(i=0;i<Field->numArrays;i++){
			//arrayName
			if(Field->Arrays[i].name){
				fprintf(fd,"%s",Field->Arrays[i].name);
			}else{
				fprintf(fd,"Array%d",i);
			}
			//numComponents
			fprintf(fd," %d",Field->Arrays[i].numComponents);

			//numTuples
			fprintf(fd," %d",Field->Arrays[i].numTuples);

			//dataType
			_get_data_type_name(Field->Arrays[i].dataType,string);
			fprintf(fd," %s\n",string);

			if(isBinary){
				_write_array_data_type_binary(fd,Field->Arrays[i].values,Field->Arrays[i].numComponents*Field->Arrays[i].numTuples,Field->Arrays[i].dataType);
			}else{
				_write_array_data_type(fd,Field->Arrays[i].values,Field->Arrays[i].numComponents*Field->Arrays[i].numTuples,Field->Arrays[i].dataType,RW_MESH_VTK_DEFAULT_NUMS_ON_ROW);
			}

		}

		break;
	}


	return 0;
}


int write_format_vtk_struct(struct RW_MESH_VTK_STRUCT*VTK,char filename[256],int flags){

	__save_locale;

	FILE*fd;
	int i,j,k;
	int a,b,c;
	int p,f,d;
	int rn;
	int size;
	int type;
	int data_type,data_size;
	int CountOfPoints;
	int CountOfCells;
	int isBinary;
	int*cp;

	struct RW_MESH_VTK_STRUCTURED_POINTS_STRUCT*	StructuredPoints;
	struct RW_MESH_VTK_STRUCTURED_GRID_STRUCT*		StructuredGrid;
	struct RW_MESH_VTK_RECTILINER_GRID_STRUCT*		RectilinerGrid;
	struct RW_MESH_VTK_POLYGONAL_DATA_STRUCT*		PolygonalData;
	struct RW_MESH_VTK_UNSTRUCTURED_GRID_STRUCT*	UnstructuredGrid;

	StructuredPoints	= NULL;
	StructuredGrid		= NULL;
	RectilinerGrid		= NULL;
	PolygonalData		= NULL;
	UnstructuredGrid	= NULL;

	CountOfPoints = 0;
	CountOfCells = 0;

	rw_mesh_set_filename(filename);
	fd=fopen(filename,"w");if(fd==NULL){
		rw_mesh_set_error(0,"Can't open file");
		return 1;
	}

	isBinary = 0;
	if(flags&RW_MESH_VTK_BINARY)isBinary = 1;

	fprintf(fd,"# vtk DataFile Version 3.0\n");
	fprintf(fd,"File generated be rw_mesh\n");
	if(isBinary){
		fprintf(fd,"BINARY\n");
	}else{
		fprintf(fd,"ASCII\n");
	}

	if(VTK->type == RW_MESH_VTK_TYPE_STRUCTURED_POINTS){
		rw_mesh_set_error(0,"Structured point isn't supported in current version of writer");
		fclose(fd);
		return 1;
	}else if(VTK->type == RW_MESH_VTK_TYPE_STRUCTURED_GRID){
		fprintf(fd,"DATASET STRUCTURED_GRID\n");
		StructuredGrid = (struct RW_MESH_VTK_STRUCTURED_GRID_STRUCT*)VTK->Grid;
		fprintf(fd,"DIMENSIONS %d %d %d\n",StructuredGrid->Nx,StructuredGrid->Ny,StructuredGrid->Nz);
		CountOfPoints = StructuredGrid->Nx*StructuredGrid->Ny*StructuredGrid->Nz;
		CountOfCells = (StructuredGrid->Nx-1)*(StructuredGrid->Ny-1)*(StructuredGrid->Nz-1);
		fprintf(fd,"POINTS %d ",CountOfPoints);
		if(flags&RW_MESH_VTK_USE_POINTS_FLOAT){
			fprintf(fd,"float");
		}else{
			fprintf(fd,"double");
		}
		fprintf(fd,"\n");

//		for(i=0;i<CountOfPoints;i++){
//			if(flags&RW_MESH_VTK_USE_POINTS_FLOAT){
//				fprintf(fd,"%f %f %f\n",StructuredGrid->Points[i*3],StructuredGrid->Points[i*3+1],StructuredGrid->Points[i*3+2]);
//			}else{
//				fprintf(fd,"%lf %lf %lf\n",StructuredGrid->Points[i*3],StructuredGrid->Points[i*3+1],StructuredGrid->Points[i*3+2]);
//			}
//		}
		if(flags&RW_MESH_VTK_USE_POINTS_FLOAT){
			if(isBinary){
				_write_array_data_type_binary(fd,StructuredGrid->Points,CountOfPoints*3,RW_MESH_VTK_TYPE_FLOAT);
			}else{
				_write_array_data_type(fd,StructuredGrid->Points,CountOfPoints*3,RW_MESH_VTK_TYPE_FLOAT,3);
			}
		}else{
			if(isBinary){
				_write_array_data_type_binary(fd,StructuredGrid->Points,CountOfPoints*3,RW_MESH_VTK_TYPE_DOUBLE);
			}else{
				_write_array_data_type(fd,StructuredGrid->Points,CountOfPoints*3,RW_MESH_VTK_TYPE_DOUBLE,3);
			}
		}

	}else if(VTK->type == RW_MESH_VTK_TYPE_RECTILINEAR_GRID){
		rw_mesh_set_error(0,"Structured point isn't supported in current version of writer");
		fclose(fd);
		return 1;
	}else if(VTK->type == RW_MESH_VTK_TYPE_POLYGONAL_DATA){
		rw_mesh_set_error(0,"Structured point isn't supported in current version of writer");
		fclose(fd);
		return 1;
	}else if(VTK->type == RW_MESH_VTK_TYPE_UNSTRUCTURED_GRID){
		fprintf(fd,"DATASET UNSTRUCTURED_GRID\n");
		UnstructuredGrid = (struct RW_MESH_VTK_UNSTRUCTURED_GRID_STRUCT*)VTK->Grid;
		fprintf(fd,"POINTS %d ",UnstructuredGrid->CountOfPoints);
		if(flags&RW_MESH_VTK_USE_POINTS_FLOAT){
			fprintf(fd,"float");
		}else{
			fprintf(fd,"double");
		}
		fprintf(fd,"\n");

//		for(i=0;i<UnstructuredGrid->CountOfPoints;i++){
//			if(flags&RW_MESH_VTK_USE_POINTS_FLOAT){
//				fprintf(fd,"%f %f %f\n",UnstructuredGrid->Points[i*3],UnstructuredGrid->Points[i*3+1],UnstructuredGrid->Points[i*3+2]);
//			}else{
//				fprintf(fd,"%lf %lf %lf\n",UnstructuredGrid->Points[i*3],UnstructuredGrid->Points[i*3+1],UnstructuredGrid->Points[i*3+2]);
//			}
//		}
		if(flags&RW_MESH_VTK_USE_POINTS_FLOAT){
			if(isBinary){
				_write_array_data_type_binary(fd,UnstructuredGrid->Points,UnstructuredGrid->CountOfPoints*3,RW_MESH_VTK_TYPE_FLOAT);
			}else{
				_write_array_data_type(fd,UnstructuredGrid->Points,UnstructuredGrid->CountOfPoints*3,RW_MESH_VTK_TYPE_FLOAT,3);
			}
		}else{
			if(isBinary){
				_write_array_data_type_binary(fd,UnstructuredGrid->Points,UnstructuredGrid->CountOfPoints*3,RW_MESH_VTK_TYPE_DOUBLE);
			}else{
				_write_array_data_type(fd,UnstructuredGrid->Points,UnstructuredGrid->CountOfPoints*3,RW_MESH_VTK_TYPE_DOUBLE,3);
			}
		}

		fprintf(fd,"CELLS %d %d\n",UnstructuredGrid->CountOfCells,UnstructuredGrid->CountOfCells+UnstructuredGrid->CellOffset[UnstructuredGrid->CountOfCells]);
		if(isBinary){
			for(i=j=0;i<UnstructuredGrid->CountOfCells;i++)
				j+=UnstructuredGrid->CellSizes[i];
			cp = (int*)calloc(UnstructuredGrid->CountOfCells+j,sizeof(int));
			for(i=j=0;i<UnstructuredGrid->CountOfCells;i++){
				cp[j] = UnstructuredGrid->CellSizes[i];
				memcpy(cp+j+1,UnstructuredGrid->Cells+UnstructuredGrid->CellOffset[i],UnstructuredGrid->CellSizes[i]*sizeof(int));
				j+=1+UnstructuredGrid->CellSizes[i];
			}
			_write_array_data_type_binary(fd,cp,j,RW_MESH_VTK_TYPE_INT);
			free(cp);
		}else{
			for(i=0;i<UnstructuredGrid->CountOfCells;i++){
				fprintf(fd,"%d",UnstructuredGrid->CellSizes[i]);
				for(j=0;j<UnstructuredGrid->CellSizes[i];j++)
					fprintf(fd," %d",UnstructuredGrid->Cells[UnstructuredGrid->CellOffset[i]+j]);
				fprintf(fd,"\n");
			}
		}

		fprintf(fd,"CELL_TYPES %d\n",UnstructuredGrid->CountOfCells);
		if(isBinary){
			_write_array_data_type_binary(fd,UnstructuredGrid->CellTypes,UnstructuredGrid->CountOfCells,RW_MESH_VTK_TYPE_INT);
		}else{
			for(i=0;i<UnstructuredGrid->CountOfCells;i++){
				fprintf(fd,"%d%c",UnstructuredGrid->CellTypes[i],((i+1)%20&&((i+1)<UnstructuredGrid->CountOfCells))?' ':'\n');
			}
		}

		CountOfPoints = UnstructuredGrid->CountOfPoints;
		CountOfCells = UnstructuredGrid->CountOfCells;
	}
	if(VTK->CountOfData){
		for(i=0;i<VTK->CountOfData;i++)
			_write_data(fd,VTK->Data+i,isBinary);
	}

	if(VTK->CountOfPointsData){
		fprintf(fd,"POINT_DATA %d\n",CountOfPoints);
		for(i=0;i<VTK->CountOfPointsData;i++)
			_write_data(fd,VTK->PointsData+i,isBinary);
	}

	if(VTK->CountOfCellsData){
		fprintf(fd,"CELL_DATA %d\n",CountOfCells);
		for(i=0;i<VTK->CountOfCellsData;i++)
			_write_data(fd,VTK->CellsData+i,isBinary);
	}

	fclose(fd);
	return 0;
}


void vtk_data_field_add_array(struct RW_MESH_VTK_DATA_FIELD_STRUCT*Field,char*name,int Count,int numComp,int type,void*Values){
	int i,j;
	struct RW_MESH_VTK_DATA_FIELD_ARRAY_STRUCT* Array;

	if(Field->numArrays == 0){
		Field->numArrays = 1;
		Field->Arrays = (struct RW_MESH_VTK_DATA_FIELD_ARRAY_STRUCT*)calloc(1,sizeof(struct RW_MESH_VTK_DATA_FIELD_ARRAY_STRUCT));
		Array = Field->Arrays+0;
	}else{
		Field->numArrays++;
		Field->Arrays = (struct RW_MESH_VTK_DATA_FIELD_ARRAY_STRUCT*)realloc(Field->Arrays,Field->numArrays*sizeof(struct RW_MESH_VTK_DATA_FIELD_ARRAY_STRUCT));
		Array = Field->Arrays+Field->numArrays-1;
	}

	Array->dataType = type;//RW_MESH_VTK_TYPE_DOUBLE;
	Array->name = (char*)calloc(strlen(name)+1,sizeof(char));
	strcpy(Array->name,name);
	Array->numComponents = numComp;
	Array->numTuples = Count;


	Array->values = calloc(Array->numComponents*Array->numTuples,_vtk_type_size(type));
	memcpy(Array->values,Values,Array->numComponents*Array->numTuples*_vtk_type_size(type));

}

void vtk_data_field_add_array_mask(struct RW_MESH_VTK_DATA_FIELD_STRUCT*Field,char*name,int Count,int numComp,int type,void*Values,int*mask){
	int i,j;
	struct RW_MESH_VTK_DATA_FIELD_ARRAY_STRUCT* Array;

	if(!mask)return vtk_data_field_add_array(Field,name,Count,numComp,type,Values);

	if(Field->numArrays == 0){
		Field->numArrays = 1;
		Field->Arrays = (struct RW_MESH_VTK_DATA_FIELD_ARRAY_STRUCT*)calloc(1,sizeof(struct RW_MESH_VTK_DATA_FIELD_ARRAY_STRUCT));
		Array = Field->Arrays+0;
	}else{
		Field->numArrays++;
		Field->Arrays = (struct RW_MESH_VTK_DATA_FIELD_ARRAY_STRUCT*)realloc(Field->Arrays,Field->numArrays*sizeof(struct RW_MESH_VTK_DATA_FIELD_ARRAY_STRUCT));
		Array = Field->Arrays+Field->numArrays-1;
	}

	Array->dataType = type;//RW_MESH_VTK_TYPE_DOUBLE;
	Array->name = (char*)calloc(strlen(name)+1,sizeof(char));
	strcpy(Array->name,name);
	Array->numComponents = numComp;

	Array->numTuples = 0;
	for(i=0;i<Count;i++)
		if(mask[i])
			Array->numTuples++;

	Array->values = calloc(Array->numComponents*Array->numTuples,_vtk_type_size(type));
	//memcpy(Array->values,Values,Array->numComponents*Array->numTuples*_vtk_type_size(type));
	for(i=j=0;i<Count;i++)
		if(mask[i]){
			memcpy(Array->values+j*Array->numComponents*_vtk_type_size(type),Values+i*Array->numComponents*_vtk_type_size(type),Array->numComponents*_vtk_type_size(type));
			j++;
		}


}

struct RW_MESH_VTK_STRUCT* rw_mesh_vtk_create_structured_simplified(int Nx,int Ny,int Nz, REAL3*Points){
	struct RW_MESH_VTK_STRUCT*VTK;
	struct RW_MESH_VTK_STRUCTURED_GRID_STRUCT*StructuredGrid;

	VTK = (struct RW_MESH_VTK_STRUCT*)calloc(1,sizeof(struct RW_MESH_VTK_STRUCT));
	rw_mesh_vtk_struct_init(VTK);

	StructuredGrid = (struct RW_MESH_VTK_STRUCTURED_GRID_STRUCT*)calloc(1,sizeof(struct RW_MESH_VTK_STRUCTURED_GRID_STRUCT));
	rw_mesh_vtk_structured_grid_struct_init(StructuredGrid);

	VTK->type = RW_MESH_VTK_TYPE_STRUCTURED_GRID;
	VTK->Grid = StructuredGrid;

	StructuredGrid->Nx = Nx;
	StructuredGrid->Ny = Ny;
	StructuredGrid->Nz = Nz;

	StructuredGrid->Points = (REAL*)calloc(Nx*Ny*Nz,3*sizeof(REAL));
	memcpy(StructuredGrid->Points,Points,Nx*Ny*Nz*3*sizeof(REAL));

	return VTK;
}

struct RW_MESH_VTK_STRUCT* rw_mesh_vtk_create_unstructured_simplified(int CountOfPoints, REAL3*Points,int CountOfCells,int*Cells,int*CellSizes,int*CellTypes,int*CellOffset){
	struct RW_MESH_VTK_STRUCT*VTK;
	struct RW_MESH_VTK_UNSTRUCTURED_GRID_STRUCT*UnstructuredGrid;

	VTK = (struct RW_MESH_VTK_STRUCT*)calloc(1,sizeof(struct RW_MESH_VTK_STRUCT));
	rw_mesh_vtk_struct_init(VTK);

	UnstructuredGrid = (struct RW_MESH_VTK_UNSTRUCTURED_GRID_STRUCT*)calloc(1,sizeof(struct RW_MESH_VTK_UNSTRUCTURED_GRID_STRUCT));
	rw_mesh_vtk_unstructured_grid_struct_init(UnstructuredGrid);

	VTK->type = RW_MESH_VTK_TYPE_UNSTRUCTURED_GRID;
	VTK->Grid = UnstructuredGrid;

	UnstructuredGrid->CountOfPoints = CountOfPoints;
	UnstructuredGrid->Points = (REAL*)calloc(CountOfPoints,3*sizeof(REAL));
	memcpy(UnstructuredGrid->Points,Points,CountOfPoints*3*sizeof(REAL));

	UnstructuredGrid->CountOfCells = CountOfCells;

	UnstructuredGrid->CellSizes = (int*)calloc(CountOfCells,sizeof(int));
	memcpy(UnstructuredGrid->CellSizes,CellSizes,CountOfCells*sizeof(int));

	UnstructuredGrid->CellTypes = (int*)calloc(CountOfCells,sizeof(int));
	memcpy(UnstructuredGrid->CellTypes,CellTypes,CountOfCells*sizeof(int));

	UnstructuredGrid->CellOffset = (int*)calloc(CountOfCells+1,sizeof(int));
	memcpy(UnstructuredGrid->CellOffset,CellOffset,(CountOfCells+1)*sizeof(int));

	UnstructuredGrid->Cells = (int*)calloc(CellOffset[CountOfCells],sizeof(int));
	memcpy(UnstructuredGrid->Cells,Cells,CellOffset[CountOfCells]*sizeof(int));

	return VTK;
}

int rw_mesh_vtk_add_scalars(struct RW_MESH_VTK_STRUCT*VTK,int data_object,int Count,int data_type,void*values,char*name){
	struct RW_MESH_VTK_DATASET_STRUCT*Data;
	struct RW_MESH_VTK_DATA_SCALARS_STRUCT*Scalars;

	Data = _vtk_add_data(VTK,data_object);

	Data->Counts = Count;
	if(name){
		Data->DataName = (char*)calloc(strlen(name)+1,sizeof(char));
		strcpy(Data->DataName,name);
	}else{
		Data->DataName = NULL;
	}

	Data->DataType = RW_MESH_VTK_DATASET_TYPE_SCALARS;
	Scalars = (struct RW_MESH_VTK_DATA_SCALARS_STRUCT*)calloc(1,sizeof(struct RW_MESH_VTK_DATA_SCALARS_STRUCT));
	Data->Data = Scalars;

	Scalars->dataType = data_type;
	Scalars->numComp = 1;

	Scalars->LookupTable = (char*)calloc(strlen("default")+1,sizeof(char));
	strcpy(Scalars->LookupTable,"default");

	Scalars->values = calloc(Count,_vtk_type_size(data_type));
	memcpy(Scalars->values,values,Count*_vtk_type_size(data_type));

	return 0;
}

int rw_mesh_vtk_add_mask(struct RW_MESH_VTK_STRUCT*VTK,int data_object,int Count,int*Mask,char*name){
	return rw_mesh_vtk_add_scalars(VTK,data_object,Count,RW_MESH_VTK_TYPE_INT,Mask,name);
}

int rw_mesh_vtk_add_mask_points(struct RW_MESH_VTK_STRUCT*VTK,int Count,int*Mask,char*name){
	return rw_mesh_vtk_add_mask(VTK,RW_MESH_VTK_DATA_OBJECT_POINTS,Count,Mask,name);
}
int rw_mesh_vtk_add_mask_cells(struct RW_MESH_VTK_STRUCT*VTK,int Count,int*Mask,char*name){
	return rw_mesh_vtk_add_mask(VTK,RW_MESH_VTK_DATA_OBJECT_CELLS,Count,Mask,name);
}

int rw_mesh_vtk_add_function(struct RW_MESH_VTK_STRUCT*VTK,int data_object,int Count,REAL*Function,char*name){
	return rw_mesh_vtk_add_scalars(VTK,data_object,Count,RW_MESH_VTK_TYPE_DOUBLE,Function,name);
}
int rw_mesh_vtk_add_function_points(struct RW_MESH_VTK_STRUCT*VTK,int Count,REAL*Function,char*name){
	return rw_mesh_vtk_add_function(VTK,RW_MESH_VTK_DATA_OBJECT_POINTS,Count,Function,name);
}
int rw_mesh_vtk_add_function_cells(struct RW_MESH_VTK_STRUCT*VTK,int Count,REAL*Function,char*name){
	return rw_mesh_vtk_add_function(VTK,RW_MESH_VTK_DATA_OBJECT_CELLS,Count,Function,name);
}

int rw_mesh_vtk_add_vectors(struct RW_MESH_VTK_STRUCT*VTK,int data_object,int Count,int data_type,void*values,char*name){
	struct RW_MESH_VTK_DATASET_STRUCT*Data;
	struct RW_MESH_VTK_DATA_VECTORS_STRUCT*Vectors;

	Data = _vtk_add_data(VTK,data_object);

	Data->Counts = Count;
	if(name){
		Data->DataName = (char*)calloc(strlen(name)+1,sizeof(char));
		strcpy(Data->DataName,name);
	}else{
		Data->DataName = NULL;
	}

	Data->DataType = RW_MESH_VTK_DATASET_TYPE_VECTORS;
	Vectors = (struct RW_MESH_VTK_DATA_VECTORS_STRUCT*)calloc(1,sizeof(struct RW_MESH_VTK_DATA_VECTORS_STRUCT));
	Data->Data = Vectors;

	Vectors->dataType = data_type;

	Vectors->values = calloc(Count*3,_vtk_type_size(data_type));
	memcpy(Vectors->values,values,Count*3*_vtk_type_size(data_type));

	return 0;
}

int rw_mesh_vtk_add_vectors_mask(struct RW_MESH_VTK_STRUCT*VTK,int data_object,int Count,int data_type,void*values,int*mask,char*name){
	int i,j;
	struct RW_MESH_VTK_DATASET_STRUCT*Data;
	struct RW_MESH_VTK_DATA_VECTORS_STRUCT*Vectors;

	if(!mask)return rw_mesh_vtk_add_vectors(VTK,data_object,Count,data_type,values,name);

	Data = _vtk_add_data(VTK,data_object);

	Data->Counts = 0;
	for(i=0;i<Count;i++)
		if(mask[i])
			Data->Counts++;

	if(name){
		Data->DataName = (char*)calloc(strlen(name)+1,sizeof(char));
		strcpy(Data->DataName,name);
	}else{
		Data->DataName = NULL;
	}

	Data->DataType = RW_MESH_VTK_DATASET_TYPE_VECTORS;
	Vectors = (struct RW_MESH_VTK_DATA_VECTORS_STRUCT*)calloc(1,sizeof(struct RW_MESH_VTK_DATA_VECTORS_STRUCT));
	Data->Data = Vectors;

	Vectors->dataType = data_type;

	Vectors->values = calloc(Data->Counts*3,_vtk_type_size(data_type));

	for(i=j=0;i<Count;i++)
		if(mask[i]){
			memcpy(Vectors->values+j*3*_vtk_type_size(data_type),values+i*3*_vtk_type_size(data_type),3*_vtk_type_size(data_type));
			j++;
		}

	return 0;
}


struct RW_MESH_VTK_DATA_FIELD_STRUCT* vtk_data_field_add_field(struct RW_MESH_VTK_STRUCT*VTK,int data_object,char*name){

	struct RW_MESH_VTK_DATASET_STRUCT*Data;
	Data = _vtk_add_data(VTK,data_object);

	rw_mesh_vtk_dataset_struct_init(Data);
	Data->Counts = 1;
	Data->DataName = (char*)calloc(strlen(name)+1,sizeof(char));
	strcpy(Data->DataName,name);
	Data->DataType = RW_MESH_VTK_DATASET_TYPE_FIELD;
	Data->Data = (void*)calloc(1,sizeof(struct RW_MESH_VTK_DATA_FIELD_STRUCT));
	return (struct RW_MESH_VTK_DATA_FIELD_STRUCT*) Data->Data;
}


int write_format_vtk_structured_simplified(
		int Nx,int Ny,int Nz, REAL3*Points,
		int CountOfPointMasks, int*PointMasks,
		int CountOfPointFunctions, REAL*PointFunctions,

		int CountOfCellMasks, int*CellMasks,
		int CountOfCellFunctions, REAL*CellFunctions,

		char*filename,int flags){

	struct RW_MESH_VTK_STRUCT*VTK;
	int i,k;
	int CountOfPoints,CountOfCells;
	int*Mask;
	REAL*Functions;
	char name[256];

	VTK = rw_mesh_vtk_create_structured_simplified(Nx,Ny,Nz,Points);

	CountOfPoints = Nx*Ny*Nz;
	CountOfCells = (Nx-1)*(Ny-1)*(Nz-1);

	if(CountOfPointMasks){
		Mask = (int*)calloc(CountOfPoints,sizeof(int));
		for(i=0;i<CountOfPointMasks;i++){
			sprintf(name,"points_mask_%d",i+1);
			for(k=0;k<CountOfPoints;k++)
				Mask[k] = PointMasks[k*CountOfPointMasks+i];
			rw_mesh_vtk_add_mask_points(VTK,CountOfPoints,Mask,name);
		}
		ffree(Mask);
	}

	if(CountOfPointFunctions){
		Functions = (REAL*)calloc(CountOfPoints,sizeof(REAL));
		for(i=0;i<CountOfPointFunctions;i++){
			sprintf(name,"points_function_%d",i+1);
			for(k=0;k<CountOfPoints;k++)
				Functions[k] = PointFunctions[k*CountOfPointFunctions+i];
			rw_mesh_vtk_add_function_points(VTK,CountOfPoints,Functions,name);
		}
		free(Functions);
	}

	if(CountOfCellMasks){
		Mask = (int*)calloc(CountOfCells,sizeof(int));
		for(i=0;i<CountOfCellMasks;i++){
			sprintf(name,"cells_mask_%d",i+1);
			for(k=0;k<CountOfCells;k++)
				Mask[k] = CellMasks[k*CountOfCellMasks+i];
			rw_mesh_vtk_add_mask_cells(VTK,CountOfCells,Mask,name);
		}
		ffree(Mask);
	}

	if(CountOfCellFunctions){
		Functions = (REAL*)calloc(CountOfCells,sizeof(REAL));
		for(i=0;i<CountOfCellFunctions;i++){
			sprintf(name,"cells_function_%d",i+1);
			for(k=0;k<CountOfCells;k++)
				Functions[k] = PointFunctions[k*CountOfCellFunctions+i];
			rw_mesh_vtk_add_function_cells(VTK,CountOfCells,Functions,name);
		}
		free(Functions);
	}

	i = write_format_vtk_struct(VTK,filename,flags);
	rw_mesh_vtk_struct_free(VTK);
	ffree(VTK);

	return i;
}


int write_format_vtk_unstructured_simplified(
	int CountOfPoints, REAL3*Points,
	int CountOfPointMasks, int*PointMasks,
	int CountOfPointFunctions, REAL*PointFunctions,

	int CountOfCells,int*Cells,int*CellSizes,int*CellTypes,int*CellOffset,
	int CountOfCellMasks, int*CellMasks,
	int CountOfCellFunctions, REAL*CellFunctions,

	char*filename,int flags){


	struct RW_MESH_VTK_STRUCT*VTK;
	int i,k;
	int*Mask;
	REAL*Functions;
	char name[256];

	VTK = rw_mesh_vtk_create_unstructured_simplified(CountOfPoints,Points,CountOfCells,Cells,CellSizes,CellTypes,CellOffset);

	if(CountOfPointMasks){
		Mask = (int*)calloc(CountOfPoints,sizeof(int));
		for(i=0;i<CountOfPointMasks;i++){
			sprintf(name,"mask_%d",i+1);
			for(k=0;k<CountOfPoints;k++)
				Mask[k] = PointMasks[k*CountOfPointMasks+i];
			rw_mesh_vtk_add_mask_points(VTK,CountOfPoints,Mask,name);
		}
		ffree(Mask);
	}

	if(CountOfPointMasks){
		Functions = (REAL*)calloc(CountOfPoints,sizeof(REAL));
		for(i=0;i<CountOfPointMasks;i++){
			sprintf(name,"function_%d",i+1);
			for(k=0;k<CountOfPoints;k++)
				Functions[k] = PointFunctions[k*CountOfPointMasks+i];
			rw_mesh_vtk_add_function_points(VTK,CountOfPoints,Functions,name);
		}
		free(Functions);
	}

	if(CountOfPointMasks){
		Mask = (int*)calloc(CountOfPoints,sizeof(int));
		for(i=0;i<CountOfPointMasks;i++){
			sprintf(name,"mask_%d",i+1);
			for(k=0;k<CountOfPoints;k++)
				Mask[k] = PointMasks[k*CountOfPointMasks+i];
			rw_mesh_vtk_add_mask_points(VTK,CountOfPoints,Mask,name);
		}
		ffree(Mask);
	}

	if(CountOfPointMasks){
		Functions = (REAL*)calloc(CountOfPoints,sizeof(REAL));
		for(i=0;i<CountOfPointMasks;i++){
			sprintf(name,"function_%d",i+1);
			for(k=0;k<CountOfPoints;k++)
				Functions[k] = PointFunctions[k*CountOfPointMasks+i];
			rw_mesh_vtk_add_function_points(VTK,CountOfPoints,Functions,name);
		}
		free(Functions);
	}

	i = write_format_vtk_struct(VTK,filename,flags);
	rw_mesh_vtk_struct_free(VTK);
	ffree(VTK);

	return i;
}


/*
int str_divide( char* input, const char* separator, char* before, char* after);
int str_trim(char* s);
int char_is_space(char c);
int vtk_read_line (FILE * STREAM, char *line);


int str_divide( char* input, const char* separator, char* b2efore, char* after)
{
	char* s;
	int len, lens, pos;

	len = strlen(input);
	lens = strlen(separator);
	s = strstr(input, separator);

	if (!s) return (1);

	pos = s-input;
	memcpy( before, input, pos);
	before[pos] = '\0';
	str_trim(before);
	memcpy( after, s + lens, len - lens - pos);
	after[len - lens - pos] = '\0';
	str_trim(after);

	return (0);
}

int str_trim(char* s)
{
	int i, len, lf, rt;
	len = strlen(s);

	lf = 0;
	while (char_is_space(s[lf]))
		lf++;

	rt = len - 1;
	while (rt > 0 && char_is_space(s[rt]))
		rt--;

	if (rt == 0 && char_is_space(s[rt]))
	{
		s[0] = '\0';
		return (0);
	}

	if (lf > 0)
	{
		for (i = lf; i <= rt; i++)
			s[i - lf] = s[i];
	}

	s[rt - lf + 1] = '\0';
	return (lf + len - rt - 1);
}

int char_is_space(char c)
{
	if (c == ' ') return (1);
	if (c == '\t') return (1);
	if (c == '\r') return (1);
	if (c == '\n') return (1);
	return (0);
}
int vtk_read_line (FILE * STREAM, char *line)
{
  int i;

  if (fgets (line, 256, STREAM) == NULL)
      return (-1);

  i = strlen(line);
  return i;
}*/

/*int write_format_vtk(int nv, REAL* v, int *mskv, int dim,
					 int ncells, int* cells, int* cell_sizes,
					 int* cell_types, int *cell_mask, char filename[256])
{
	char word[256];
	int i, k;
	FILE *LUN;

	// the numbering in index arrays is started from 1 here!
	if (strlen(filename) == 0)
	{
		log_write("empty filename \n");
		return (-1);
	};

	LUN = fopen (filename, "w");
	if (LUN == NULL)
	{
		log_write("can't open vtk-file \n");
		return (-1);
	};

	sprintf (word, "%s", "# vtk DataFile Version 3.0");
	fprintf (LUN, "%s\n", word);
	sprintf (word, "%s", "vtk output");
	fprintf (LUN, "%s\n", word);
	sprintf (word, "%s", "ASCII");
	fprintf (LUN, "%s\n", word);
	sprintf (word, "%s", "DATASET UNSTRUCTURED_GRID");
	fprintf (LUN, "%s\n", word);

	sprintf (word, "%s %d double", "POintS", nv);

	fprintf (LUN, "%s\n", word);
	if (dim == 2)
		for (i = 0; i < nv; i++)
			fprintf (LUN, "%le %le %le \n", v[2 * i + 0],
			v[2 * i + 1], 0.0e0);

	if (dim == 3)
		for (i = 0; i < nv; i++)
			fprintf (LUN, "%le %le %le\n", v[3 * i + 0],
			v[3 * i + 1], v[3 * i + 2]);


	if(ncells > 0)
	{
		sprintf (word, "%s %d %d", "CELLS", ncells, cell_sizes[ncells] + ncells);
		fprintf (LUN, "%s\n", word);

		for (i = 0; i < ncells; i++)
		{
			fprintf (LUN, "%d ", cell_sizes[i + 1] - cell_sizes[i]);
			for (k = cell_sizes[i]; k < cell_sizes[i + 1]; k++)
				fprintf (LUN, "%d ", cells[k]);
			fprintf (LUN, "\n");
		}
	}

	if (cell_types != NULL)
	{
		sprintf (word, "%s %d", "CELL_TYPES", ncells);
		fprintf (LUN, "%s\n", word);
		for (i = 0; i < ncells; i++)
			fprintf (LUN, "%d\n", cell_types[i]);
	}

	if (mskv != NULL)
	{
		sprintf (word, "%s", "FIELD FieldData 1");
		fprintf (LUN, "%s\n", word);
		sprintf (word, "%s %d %s", "optimization-points 1 ", nv, " vtkIdType");
		fprintf (LUN, "%s\n", word);

		for (i = 0; i < nv; i++)
			fprintf (LUN, "%d\n", mskv[i]);
	}

	if (cell_mask != NULL)
	{
		sprintf (word, "%s %d", "CELL_DATA", ncells);
		fprintf (LUN, "%s\n", word);
		sprintf (word, "%s", "SCALARS scalars int");
		fprintf (LUN, "%s\n", word);
		sprintf (word, "%s", "LOOKUP_TABLE default");
		fprintf (LUN, "%s\n", word);

		for (i = 0; i < ncells; i++)
			fprintf (LUN, "%d\n", cell_mask[i]);
	}

	fclose (LUN);

	return (0);
}

/*
int write_format_vtk_points_function(int nv, REAL* v, int *mskv,REAL*fv, int dim,
					 int ncells, int* cells, int* cell_sizes,
					 int* cell_types, int *cell_mask, char filename[256])
{
	char word[256];
	int i, k;
	FILE *LUN;

	// the numbering in index arrays is started from 1 here!
	if (strlen(filename) == 0)
	{
		log_write("empty filename \n");
		return (-1);
	};

	LUN = fopen (filename, "w");
	if (LUN == NULL)
	{
		log_write("can't open vtk-file \n");
		return (-1);
	};

	sprintf (word, "%s", "# vtk DataFile Version 3.0");
	fprintf (LUN, "%s\n", word);
	sprintf (word, "%s", "vtk output");
	fprintf (LUN, "%s\n", word);
	sprintf (word, "%s", "ASCII");
	fprintf (LUN, "%s\n", word);
	sprintf (word, "%s", "DATASET UNSTRUCTURED_GRID");
	fprintf (LUN, "%s\n", word);

	sprintf (word, "%s %d double", "POintS", nv);

	fprintf (LUN, "%s\n", word);
	if (dim == 2)
		for (i = 0; i < nv; i++)
			fprintf (LUN, "%le %le %le \n", v[2 * i + 0],
			v[2 * i + 1], 0.0e0);

	if (dim == 3)
		for (i = 0; i < nv; i++)
			fprintf (LUN, "%le %le %le\n", v[3 * i + 0],
			v[3 * i + 1], v[3 * i + 2]);


	if(ncells > 0)
	{
		sprintf (word, "%s %d %d", "CELLS", ncells, cell_sizes[ncells] + ncells);
		fprintf (LUN, "%s\n", word);

		for (i = 0; i < ncells; i++)
		{
			fprintf (LUN, "%d ", cell_sizes[i + 1] - cell_sizes[i]);
			for (k = cell_sizes[i]; k < cell_sizes[i + 1]; k++)
				fprintf (LUN, "%d ", cells[k]);
			fprintf (LUN, "\n");
		}
	}

	if (cell_types != NULL)
	{
		sprintf (word, "%s %d", "CELL_TYPES", ncells);
		fprintf (LUN, "%s\n", word);
		for (i = 0; i < ncells; i++)
			fprintf (LUN, "%d\n", cell_types[i]);
	}

	if (mskv != NULL)
	{
		sprintf (word, "%s", "FIELD FieldData 1");
		fprintf (LUN, "%s\n", word);
		sprintf (word, "%s %d %s", "optimization-points 1 ", nv, " vtkIdType");
		fprintf (LUN, "%s\n", word);

		for (i = 0; i < nv; i++)
			fprintf (LUN, "%d\n", mskv[i]);
	}

	if( fv != NULL){
		fprintf(LUN,"POint_DATA %d\n",nv);
		fprintf(LUN,"SCALARS points_scalars double 1\n");
		fprintf(LUN,"LOOKUP_TABLE default\n");
		for (i = 0; i < nv; i++)
			fprintf (LUN, "%le\n", fv[i]);
	}

	if (cell_mask != NULL)
	{
		sprintf (word, "%s %d", "CELL_DATA", ncells);
		fprintf (LUN, "%s\n", word);
		sprintf (word, "%s", "SCALARS scalars int");
		fprintf (LUN, "%s\n", word);
		sprintf (word, "%s", "LOOKUP_TABLE default");
		fprintf (LUN, "%s\n", word);

		for (i = 0; i < ncells; i++)
			fprintf (LUN, "%d\n", cell_mask[i]);
	}

	fclose (LUN);

	return (0);
}

int read_format_vtk(int *out_nv, REAL3** out_v, int **out_mskv,
					int *out_ncells, int** out_cells, int** out_cell_sizes,
					int **out_cell_types, int **out_cell_mask, char filename[256])
{
    char word[256], word1[256], word2[256], word3[256];
    int i, j, elem, sbaglio;
    FILE *LUN;
    int nv, ncells, *mskv, *cells, *cell_sizes, *cell_types, *cell_mask;
    REAL *v;

	nv = 0;
	ncells = 0;

	v = NULL;
	mskv = NULL;
	cells = NULL;
	cell_sizes = NULL;
	cell_types = NULL;
	cell_mask = NULL;

	if (strlen(filename) == 0)
	{
		log_write( "Error reading file: empty filename \n");
		return (1);
	};

	LUN = fopen (filename, "r");
	if (LUN == NULL)
	{
		log_write( "Error reading file: can't open vtk-file \n");
		return (-1);
	}

	strcpy (word, " ");

	sbaglio = vtk_read_line (LUN, word);
	if (sbaglio == -1)
	{
		log_write( "Error reading file: vtk-file is empty \n");
		return (-1);
	}

	while (sbaglio != -1)
	{
      if (strstr(word, "POintS") != NULL)
	  {
    	 // log_write( "download points \n");

		  	str_divide(word, " ", word1, word3);
			str_divide(word3, " ", word1, word2);
			sscanf(word1, "%d", &nv);

			v = (REAL*) malloc(3 * nv * sizeof(REAL));
			for (i = 0; i < 3 * nv; i++)
				fscanf(LUN, "%lf", &(v[i]));

			//log_write(stderr, "download ended \n");
	  }
	  else if (strstr (word, "CELLS") != NULL)
	  {
		  //log_write( "download cells \n");
		  	str_divide(word, " ", word1, word3);
			sscanf(word3, "%d %d", &ncells, &elem);

		    cells = (int*) malloc ((elem - ncells) * sizeof(int));
			cell_sizes = (int*) malloc((ncells + 1) * sizeof(int));

			cell_sizes[0] = 0;
			for (i = 0; i < ncells; i++)
			{
				fscanf(LUN, "%d", &(cell_sizes[i + 1]));
				cell_sizes[i + 1] = cell_sizes[i] + cell_sizes[i + 1];
				for (j = cell_sizes[i]; j < cell_sizes[i + 1]; j++)
					fscanf(LUN, "%d", &(cells[j]));
			}
			//log_write(stderr, "download ended \n");
	  }
	  else if (strstr (word, "CELL_TYPES") != NULL)
	  {
		  //log_write( "download cell types \n");
			str_divide(word, " ", word1, word3);
			sscanf(word3, "%d", &ncells);

		  	cell_types = (int*) malloc(ncells * sizeof(int));
			for (i = 0; i < ncells; i++)
				fscanf(LUN, "%d", &(cell_types[i]));

			//log_write(stderr, "download ended \n");
	  }
	  else if (strstr (word, "CELL_DATA") != NULL)
	  {
		  //log_write( "download cell data \n");
			str_divide(word, " ", word1, word3);
			sscanf(word3, "%d", &ncells);
			vtk_read_line (LUN, word);
			vtk_read_line (LUN, word);

		  	cell_mask = (int*) malloc(ncells * sizeof(int));
			for (i = 0; i < ncells; i++)
				fscanf(LUN, "%d", &(cell_mask[i]));

			//log_write( "download ended \n");
	  }
	  else if (strstr (word, "FIELD")!= NULL)
	  {
		  //log_write( "download field \n");
		    vtk_read_line (LUN, word);
			str_divide(word, " ", word1, word3);
			sscanf(word3, "%d %d", &elem, &nv);

		  	mskv = (int*) malloc(nv * sizeof(int));
			for (i = 0; i < nv; i++)
				fscanf(LUN, "%d", &mskv[i]);

			//log_write( "download ended \n");
	  }
	  else if (strstr (word, "POLYGONS") != NULL)
	  {
		  //log_write( "download cells \n");
		  	str_divide(word, " ", word1, word3);
			sscanf(word3, "%d %d", &ncells, &elem);

		    cells = (int*) malloc ((elem - ncells) * sizeof(int));
			cell_sizes = (int*) malloc((ncells + 1) * sizeof(int));
			cell_types = (int*) malloc(ncells * sizeof(int));

			cell_sizes[0] = 0;
			for (i = 0; i < ncells; i++)
			{
				fscanf(LUN, "%d", &(cell_sizes[i + 1]));
				if (cell_sizes[i + 1] == 3)
					cell_types[i] = 5;
				else if (cell_sizes[i + 1] == 4)
					cell_types[i] = 9;
				else
					cell_types[i] = 7;

				cell_sizes[i + 1] = cell_sizes[i] + cell_sizes[i + 1];
				for (j = cell_sizes[i]; j < cell_sizes[i + 1]; j++)
					fscanf(LUN, "%d", &(cells[j]));
			}
			//log_write( "download ended \n");
	  }
      sbaglio = vtk_read_line (LUN, word);
  }

  if (out_nv != NULL) *out_nv = nv;

  if (out_v != NULL) *out_v = (REAL3*) v;
  else if (v != NULL) free(v);

  if (out_mskv != NULL) *out_mskv = mskv;
  else if (mskv != NULL) free(mskv);

  if (out_ncells != NULL) *out_ncells = ncells;

  if (out_cells != NULL) *out_cells = cells;
  else if (cells != NULL) free(cells);

  if (out_cell_sizes != NULL) *out_cell_sizes = cell_sizes;
  else if (cell_sizes != NULL) free(cell_sizes);

  if (out_cell_types != NULL) *out_cell_types = cell_types;
  else if (cell_types != NULL) free(cell_types);

  if (out_cell_mask != NULL) *out_cell_mask = cell_mask;
  else if (cell_mask != NULL) free(cell_mask);

  return (0);
}*/

