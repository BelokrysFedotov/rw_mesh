#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "rw_mesh_string.h"
#include "rw_mesh_vtk.h"

int str_divide( char* input, const char* separator, char* before, char* after);
int str_trim(char* s);
int char_is_space(char c);
int vtk_read_line (FILE * STREAM, char *line);


int str_divide( char* input, const char* separator, char* before, char* after)
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
}


int rw_mesh_vtk_struct_init(struct RW_MESH_VTK_UNSTRUCTURED_GRID_STRUCT*VTK){
	rw_mesh_vtk_struct_clean(VTK);
	return 0;
}
int rw_mesh_vtk_struct_clean(struct RW_MESH_VTK_UNSTRUCTURED_GRID_STRUCT*VTK){
	VTK->CountOfPoints = 0;
	VTK->Points = NULL;

	VTK->CountOfCells = 0;
	VTK->Cells = NULL;
	VTK->CellOffset = NULL;
	VTK->CellSizes = NULL;
	VTK->CellTypes = NULL;

	VTK->CountOfPointsData = 0;
	VTK->PointsData = NULL;

	VTK->CountOfCellsData = 0;
	VTK->CellsData = NULL;

	VTK->CountOfData = 0;
	VTK->Data = NULL;

	return 0;
}
int rw_mesh_vtk_struct_free(struct RW_MESH_VTK_UNSTRUCTURED_GRID_STRUCT*VTK){
	int i;
	ffree(VTK->Points);
	ffree(VTK->Cells);
	ffree(VTK->CellOffset);
	ffree(VTK->CellSizes);
	ffree(VTK->CellTypes);
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

void*_vtk_type_allocate(int type,int count){
	if(type==RW_MESH_VTK_TYPE_BIT || type==RW_MESH_VTK_TYPE_UNSIGNED_CHAR || type==RW_MESH_VTK_TYPE_UNSIGNED_CHAR){
		return calloc(count,sizeof(char));
	}else if(type==RW_MESH_VTK_TYPE_UNSIGNED_SHORT){
		return calloc(count,sizeof(unsigned short));
	}else if(type==RW_MESH_VTK_TYPE_SHORT){
		return calloc(count,sizeof(short));
	}else if(type==RW_MESH_VTK_TYPE_UNSIGNED_INT){
		return calloc(count,sizeof(unsigned int));
	}else if(type==RW_MESH_VTK_TYPE_INT){
		return calloc(count,sizeof(int));
	}else if(type==RW_MESH_VTK_TYPE_UNSIGNED_LONG){
		return calloc(count,sizeof(unsigned long int));
	}else if(type==RW_MESH_VTK_TYPE_LONG){
		return calloc(count,sizeof(long int));
	}else if(type==RW_MESH_VTK_TYPE_FLOAT){
		return calloc(count,sizeof(float));
	}else if(type==RW_MESH_VTK_TYPE_DOUBLE){
		return calloc(count,sizeof(double));
	}
	return 0;
}

int _read_vtk_type_offset(int type,char*string,void*value,int offset){
	int rn;
	char c;
	if(type==RW_MESH_VTK_TYPE_BIT || type==RW_MESH_VTK_TYPE_UNSIGNED_CHAR || type==RW_MESH_VTK_TYPE_UNSIGNED_CHAR){
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

int _read_vtk_type(int type,char*string,void*value){
	return _read_vtk_type_offset(type,string,value,0);
}

int _read_vtk_type_as_REAL_offset(int type,char*string,REAL*value,int offset){
	int rn;
	char c;
	unsigned short us;
	short s;
	unsigned int ui;
	int i;
	unsigned long int ul;
	long int l;
	float f;
	double d;

	rn = 0;
	if(type==RW_MESH_VTK_TYPE_BIT || type==RW_MESH_VTK_TYPE_UNSIGNED_CHAR || type==RW_MESH_VTK_TYPE_UNSIGNED_CHAR){
		rn = _read_vtk_type(type,string,&c);
		if(rn==1)value[offset]=c;
	}else if(type==RW_MESH_VTK_TYPE_UNSIGNED_SHORT){
		rn = _read_vtk_type(type,string,&us);
		if(rn==1)value[offset]=us;
	}else if(type==RW_MESH_VTK_TYPE_SHORT){
		rn = _read_vtk_type(type,string,&s);
		if(rn==1)value[offset]=s;
	}else if(type==RW_MESH_VTK_TYPE_UNSIGNED_INT){
		rn = _read_vtk_type(type,string,&ui);
		if(rn==1)value[offset]=ui;
	}else if(type==RW_MESH_VTK_TYPE_INT){
		rn = _read_vtk_type(type,string,&i);
		if(rn==1)value[offset]=i;
	}else if(type==RW_MESH_VTK_TYPE_UNSIGNED_LONG){
		rn = _read_vtk_type(type,string,&ul);
		if(rn==1)value[offset]=ul;
	}else if(type==RW_MESH_VTK_TYPE_LONG){
		rn = _read_vtk_type(type,string,&l);
		if(rn==1)value[offset]=l;
	}else if(type==RW_MESH_VTK_TYPE_FLOAT){
		rn = _read_vtk_type(type,string,&f);
		if(rn==1)value[offset]=f;
	}else if(type==RW_MESH_VTK_TYPE_DOUBLE){
		rn = _read_vtk_type(type,string,&d);
		if(rn==1)value[offset]=d;
	}
	return rn;
}

int _read_vtk_array_type(FILE*fd,int count,int type,void*value,int*current_line,char*first_line){
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

		if(_read_vtk_type_offset(type,word,value,i)!=1){
			rw_mesh_set_error(*current_line,"Error in reading values.");
			return 1;
		}
		i++;
	}

	return 0;
}

int _read_vtk_array_as_REAL_type(FILE*fd,int count,int type,REAL*value,int*current_line){
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

		if(_read_vtk_type_as_REAL_offset(type,word,value,i)!=1){
			rw_mesh_set_error(*current_line,"Error in reading values.");
			return 1;
		}
		i++;
	}

	return 0;
}

struct RW_MESH_VTK_DATASET_STRUCT*_vtk_add_data(struct RW_MESH_VTK_UNSTRUCTURED_GRID_STRUCT*VTK,int data_type){
	int d;
	int*CountOfData;
	struct RW_MESH_VTK_DATASET_STRUCT**Datas;
	struct RW_MESH_VTK_DATASET_STRUCT*Data;

	if(data_type==1){
		CountOfData = &(VTK->CountOfPointsData);
		Datas = &(VTK->PointsData);
	}else if(data_type==2){
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

int read_format_vtk_unstructured_in_struct(struct RW_MESH_VTK_UNSTRUCTURED_GRID_STRUCT*VTK,char filename[256],int flags){

	__save_locale;

	FILE*fd;
	int i,j,k;
	int a,b,c;
	int p,f,d;
	int rn;
	int size;
	int type;
	int data_type,data_size;

	char line[256];
	char word[256];
	int current_line;
	int linepos;
	char line_error[256];

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

	if(strcmp(line,"BINARY")==0){
		rw_mesh_set_error(current_line,"File in Binary format. Current version of reader don't work with it");
		return 1;
	}

	if(strcmp(line,"ASCII")!=0){
		rw_mesh_set_error(current_line,"Unknown format of file. It must be ASCII or BINARY.");
		return 1;
	}

	//read DATASET type and DATASET
	read_line_trim(fd,line); current_line++;
	string_cut_word(line,word);
	if(strcmp(word,"DATASET")!=0){
		rw_mesh_set_error(current_line,"Line must be \"DATASET type\".");
		return 1;
	}

	string_cut_word(line,word);
	if(strcmp(word,"STRUCTURED_POINTS")==0){
		//RW_MESH_VTK_TYPE_STRUCTURED_POINTS;
		rw_mesh_set_error(current_line,"Type of DATASET is STRUCTURED_POINTS. It isn't supported in current version of reader.");
		return 1;
	}else if(strcmp(word,"STRUCTURED_GRID")==0){
		//RW_MESH_VTK_TYPE_STRUCTURED_GRID;
		rw_mesh_set_error(current_line,"Type of DATASET is STRUCTURED_GRID. It isn't supported in current version of reader.");
		return 1;
	}else if(strcmp(word,"RECTILINEAR_GRID")==0){
		//RW_MESH_VTK_TYPE_RECTILINEAR_GRID;
		rw_mesh_set_error(current_line,"Type of DATASET is RECTILINEAR_GRID. It isn't supported in current version of reader.");
		return 1;
	}else if(strcmp(word,"POLYDATA")==0){
		//RW_MESH_VTK_TYPE_POLYDATA;
		rw_mesh_set_error(current_line,"Type of DATASET is POLYDATA. It isn't supported in current version of reader.");
		return 1;
	}else if(strcmp(word,"UNSTRUCTURED_GRID")==0){
		//RW_MESH_VTK_TYPE_UNSTRUCTURED_GRID;

		/**
		 * data_type = 0; - data
		 * data_type = 1; - points data
		 * data_type = 2; - cells data
		 */
		data_type = 0;
		data_size = 0;

		// read blocks
		while(read_line_skip_empty(fd,line,&current_line)>=0){
			// read name of block
			string_cut_word(line,word);
			if(strcmp(word,"POINTS")==0){
				// first word POINTS
				// next word must be CountOfPoints
				string_cut_word(line,word);
				if(sscanf(word,"%d",&a)!=1){
					rw_mesh_set_error(current_line,"Can't read count of points.");
					return 1;
				}
				VTK->CountOfPoints = a;

				// the ending of line must be a vtk type
				type = _parse_vtk_type(line);
				if(type==RW_MESH_VTK_TYPE_NONE){
					rw_mesh_set_error(current_line,"Unknown points type.");
					return 1;
				}

				VTK->Points = (REAL*)calloc(VTK->CountOfPoints*3,sizeof(REAL));
//				for(i=0;i<VTK->CountOfPoints;i++){
//					read_line_trim(fd,line); current_line++;
//
//					for(k=0;k<3;k++){
//						string_cut_word(line,word);
//						if(_read_vtk_type_as_REAL_offset(type,word,VTK->Points,i*3+k)!=1){
//							rw_mesh_set_error(current_line,"Error in reading points");
//							return 1;
//						}
//					}
//
//				}
				if(_read_vtk_array_as_REAL_type(fd,VTK->CountOfPoints*3,type,VTK->Points,&current_line)){
					return 1;
				}

			}else if(strcmp(word,"CELLS")==0){
				// first word CELLS
				//next word must be CountOfCells
				string_cut_word(line,word);
				if(sscanf(word,"%d",&a)!=1){
					rw_mesh_set_error(current_line,"Can't read count of cells.");
					return 1;
				}
				VTK->CountOfCells = a;

				//next word must be size
				string_cut_word(line,word);
				if(sscanf(word,"%d",&size)!=1){
					rw_mesh_set_error(current_line,"Can't read count of cells.");
					return 1;
				}

				if(VTK->CountOfCells){

					size = size - VTK->CountOfCells;

					VTK->CellOffset = (int*)calloc(VTK->CountOfCells+1,sizeof(int));
					VTK->CellSizes = (int*)calloc(VTK->CountOfCells,sizeof(int));
					VTK->Cells = (int*)calloc(size,sizeof(int));

					VTK->CellOffset[0] = 0;

					for(i=0;i<VTK->CountOfCells;i++){
						//read numPointsi , p, j, k, l, ...
						read_line_trim(fd,line); current_line++;

						string_cut_word(line,word);
						if(sscanf(word,"%d",&a)!=1){
							rw_mesh_set_error(current_line,"Can't read cell size.");
							return 1;
						}
						VTK->CellSizes[i] = a;

						VTK->CellOffset[i+1] = VTK->CellOffset[i]+VTK->CellSizes[i];
						if(i+1==VTK->CountOfCells){
							if(VTK->CellOffset[i+1] != size){
								rw_mesh_set_error(current_line,"Summary size of cells don't equal size of cells.");
								return 1;
							}
						}else{
							if(VTK->CellOffset[i+1] > size){
								rw_mesh_set_error(current_line,"Summary size of cells more than size of cells.");
								return 1;
							}
						}
						for(j=0;j<VTK->CellSizes[i];j++){
							string_cut_word(line,word);
							if(sscanf(word,"%d",&a)!=1){
								rw_mesh_set_error(current_line,"Can't read cell vertex.");
								return 1;
							}
							VTK->Cells[VTK->CellOffset[i]+j] = a;
						}
					}
				}
			}else if(strcmp(word,"CELL_TYPES")==0){
				// first word CELL_TYPES
				if(VTK->CountOfCells==0){
					rw_mesh_set_error(current_line,"Block CELL_TYPES can't be before block CELLS.");
					return 1;
				}

				//next word must be CountOfCells
				string_cut_word(line,word);
				if(sscanf(word,"%d",&a)!=1){
					rw_mesh_set_error(current_line,"Can't read count of cells (CELL_TYPES).");
					return 1;
				}
				if(VTK->CountOfCells != a){
					rw_mesh_set_error(current_line,"Count of cells type don't equal count of cells.");
					return 1;
				}

				VTK->CellTypes = (int*)calloc(VTK->CountOfCells,sizeof(int));

				for(i=0;i<VTK->CountOfCells;i++){
					//read type_i
					read_line_trim(fd,line); current_line++;
					if(sscanf(line,"%d",&a)!=1){
						rw_mesh_set_error(current_line,"Can't read count of cell type.");
						return 1;
					}
					VTK->CellTypes[i] = a;
				}
			}else if(strcmp(word,"POINT_DATA")==0){
				//read countOfPoints
				string_cut_word(line,word);
				if(sscanf(word,"%d",&a)!=1){
					rw_mesh_set_error(current_line,"Can't read countOfPoints of POINT_DATA.");
					return 1;
				}
				if(VTK->CountOfPoints==0){
					rw_mesh_set_error(current_line,"POINT_DATA block must be after POINTS block.");
					return 1;
				}
				if(a!=VTK->CountOfPoints){
					rw_mesh_set_error(current_line,"POINT_DATA countOfPoints don't equal POINTS countOfPoints.");
					return 1;
				}
				data_type = 1;
				data_size = a;
			}else if(strcmp(word,"CELL_DATA")==0){
				//read countOfCells
				string_cut_word(line,word);
				if(sscanf(word,"%d",&a)!=1){
					rw_mesh_set_error(current_line,"Can't read countOfCells of CELL_DATA.");
					return 1;
				}
				if(VTK->CountOfCells==0){
					rw_mesh_set_error(current_line,"CELL_DATA block must be after CELLS block.");
					return 1;
				}
				if(a!=VTK->CountOfCells){
					rw_mesh_set_error(current_line,"CELL_DATA countOfCells don't equal CELLS countOfCells.");
					return 1;
				}
				data_type = 1;
				data_size = a;
			}else if(strcmp(word,"FIELD")==0){
				// first word FIELD

				struct RW_MESH_VTK_DATASET_STRUCT*Data;
				struct RW_MESH_VTK_DATA_FIELD_STRUCT*Field;

				Data = _vtk_add_data(VTK,data_type);

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
					rw_mesh_set_error(current_line,"Can't read size of FIELD.");
					return 1;
				}
				Field->numArrays = size;

				if(Field->numArrays){
					Field->Arrays = (struct RW_MESH_VTK_DATA_FIELD_ARRAY_STRUCT*)calloc(Field->numArrays,sizeof(struct RW_MESH_VTK_DATA_FIELD_ARRAY_STRUCT));
					for(i=0;i<Field->numArrays;i++){
						//read arrays

						//read arrayNamei numComponents numTuples dataType
						read_line_trim(fd,line);current_line++;
						// read arrayNamei
						string_cut_word(line,word);
						Field->Arrays[i].name = (char*)calloc(strlen(word)+1,sizeof(char));
						strcpy(Field->Arrays[i].name,word);

						//read numComponents
						string_cut_word(line,word);
						if(sscanf(word,"%d",&a)!=1){
							rw_mesh_set_error(current_line,"Can't read numComponents of FIELD Array.");
							return 1;
						}
						Field->Arrays[i].numComponents = a;

						//read numTuples
						string_cut_word(line,word);
						if(sscanf(word,"%d",&a)!=1){
							rw_mesh_set_error(current_line,"Can't read numTuples of FIELD Array.");
							return 1;
						}
						Field->Arrays[i].numTuples = a;

						//read dataType
						string_cut_word(line,word);
						Field->Arrays[i].dataType = _parse_vtk_type(word);

						//f 00 f 01 ... f 0(numComponents-1)
						if(Field->Arrays[i].numComponents*Field->Arrays[i].numTuples){
							Field->Arrays[i].values = _vtk_type_allocate(Field->Arrays[i].dataType,Field->Arrays[i].numComponents*Field->Arrays[i].numTuples);
							if(_read_vtk_array_type(fd,Field->Arrays[i].numComponents*Field->Arrays[i].numTuples,Field->Arrays[i].dataType,Field->Arrays[i].values,&current_line,0)){
								return 1;
							}
						}

					}
				}

			}else if(strcmp(word,"SCALARS")==0){
				// first word SCALARS
				struct RW_MESH_VTK_DATASET_STRUCT*Data;
				struct RW_MESH_VTK_DATA_SCALARS_STRUCT*Scalars;

				Data = _vtk_add_data(VTK,data_type);
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
					rw_mesh_set_error(current_line,"Unknown type.");
					return 1;
				}

				//next word MAY be numComp
				if(sscanf(line,"%u",&a)==1){
					Scalars->numComp = a;
				}else{
					Scalars->numComp = 1;
				}

				read_line_trim(fd,line);current_line++;

				string_get_word(line,word);
				if(strcmp(word,"LOOKUP_TABLE")==0){
					string_cut_word(line,word);
					string_cut_word(line,word);
					Scalars->LookupTable = (char*)calloc(strlen(word)+1,sizeof(char));
					strcpy(Scalars->LookupTable,word);
				}else{
					line[0] = 0;
				}

				Scalars->values = _vtk_type_allocate(Scalars->dataType,Data->Counts*Scalars->numComp);
				if(_read_vtk_array_type(fd,Data->Counts*Scalars->numComp,Scalars->dataType,Scalars->values,&current_line,line)){
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

	}else if(strcmp(word,"FIELD")==0){
		RW_MESH_VTK_TYPE_FIELD;
		rw_mesh_set_error(current_line,"Type of DATASET is FIELD. It isn't supported in current version of reader.");
		return 1;
	}else{
		rw_mesh_set_error(current_line,"Unknown type of DATASET. It must be STRUCTURED_POINTS, STRUCTURED_GRID, RECTILINEAR_GRID, POLYDATA, UNSTRUCTURED_GRID or FIELD.");
		return 1;
	}

	return 0;
}

int write_format_vtk(int nv, REAL* v, int *mskv, int dim,
					 int ncells, int* cells, int* cell_sizes,
					 int* cell_types, int *cell_mask, char filename[256])
{
	char word[256];
	int i, k;
	FILE *LUN; 

	/* the numbering in index arrays is started from 1 here! */
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
		case RW_MESH_VTK_TYPE_FLOAT:	Function[size*i+offset] = ((float*)data)[data_size*i+offset];break;
		case RW_MESH_VTK_TYPE_DOUBLE:	Function[size*i+offset] = ((double*)data)[data_size*i+offset];break;
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
	struct RW_MESH_VTK_UNSTRUCTURED_GRID_STRUCT VTK;
	rw_mesh_vtk_struct_init(&VTK);

	rn = read_format_vtk_unstructured_in_struct(&VTK,filename,flags);
	if(rn){
		rw_mesh_vtk_struct_free(&VTK);
		return 1;
	}

	if(CountOfPoints){
		*CountOfPoints = VTK.CountOfPoints;
	}

	if(Points){
		if(VTK.CountOfPoints){
			*Points = (REAL3*)calloc(VTK.CountOfPoints,sizeof(REAL3));
			for(i=0;i<VTK.CountOfPoints;i++)
				for(k=0;k<3;k++)
					(*Points)[i][k] = VTK.Points[i*3+k];

		}else{
			*Points = NULL;
		}
	}

	if(VTK.CountOfPointsData){
		_simplify_data_to_masks(VTK.CountOfPoints,VTK.CountOfPointsData,VTK.PointsData,CountOfPointMasks,PointMasks);
		_simplify_data_to_functions(VTK.CountOfPoints,VTK.CountOfPointsData,VTK.PointsData,CountOfPointFunctions,PointFunctions);
	}else{
		if(CountOfPointMasks)*CountOfPointMasks=0;
		if(PointMasks)*PointMasks=NULL;
		if(CountOfPointFunctions)*CountOfPointFunctions=0;
		if(PointFunctions)*PointFunctions=NULL;
	}

	if(CountOfCells){
		*CountOfCells = VTK.CountOfCells;
	}

	if(Cells){
		if(VTK.CountOfCells && VTK.Cells && VTK.CellOffset && VTK.CellOffset[VTK.CountOfCells]){
			*Cells = (int*)calloc(VTK.CellOffset[VTK.CountOfCells],sizeof(int));
			for(i=0;i<VTK.CellOffset[VTK.CountOfCells];i++)
				(*Cells)[i] = VTK.Cells[i];
		}else{
			*Cells = NULL;
		}
	}

	if(CellSizes){
		if(VTK.CountOfCells && VTK.CellSizes){
			*CellSizes = (int*)calloc(VTK.CountOfCells,sizeof(int));
			for(i=0;i<VTK.CountOfCells;i++)
				(*CellSizes)[i] = VTK.CellSizes[i];
		}else{
			*CellSizes = NULL;
		}
	}

	if(CellTypes){
		if(VTK.CountOfCells && VTK.CellTypes){
			*CellTypes = (int*)calloc(VTK.CountOfCells,sizeof(int));
			for(i=0;i<VTK.CountOfCells;i++)
				(*CellTypes)[i] = VTK.CellTypes[i];
		}else{
			*CellTypes = NULL;
		}
	}

	if(CellOffset){
		if(VTK.CountOfCells && VTK.CellOffset){
			*CellOffset = (int*)calloc(VTK.CountOfCells+1,sizeof(int));
			for(i=0;i<=VTK.CountOfCells;i++)
				(*CellOffset)[i] = VTK.CellOffset[i];
		}else{
			*CellOffset = NULL;
		}
	}

	if(VTK.CountOfCellsData){
		_simplify_data_to_masks(VTK.CountOfCells,VTK.CountOfCellsData,VTK.CellsData,CountOfCellMasks,CellMasks);
		_simplify_data_to_functions(VTK.CountOfCells,VTK.CountOfCellsData,VTK.CellsData,CountOfCellFunctions,CellFunctions);
	}else{
		if(CountOfCellMasks)*CountOfCellMasks=0;
		if(CellMasks)*CellMasks=NULL;
		if(CountOfCellFunctions)*CountOfCellFunctions=0;
		if(CellFunctions)*CellFunctions=NULL;
	}

	return 0;
}


int write_format_vtk_points_function(int nv, REAL* v, int *mskv,REAL*fv, int dim,
					 int ncells, int* cells, int* cell_sizes,
					 int* cell_types, int *cell_mask, char filename[256])
{
	char word[256];
	int i, k;
	FILE *LUN;

	/* the numbering in index arrays is started from 1 here! */
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
}

