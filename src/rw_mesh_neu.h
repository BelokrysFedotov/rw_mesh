/*
 * rw_mesh_neu.h
 *
 *  Created on: 03.02.2016
 *      Author: Belokrys-Fedotov A.I.
 */

#ifndef RW_MESH_NEU_H_
#define RW_MESH_NEU_H_

#include "rw_mesh_header.h"

#define EXTENTION_NEU "*\.neu"

/**
 * Стуктура граничного условия
 */
struct neu_boundary_condition_struct{
	// Номер ячейки
	int*Cells;
	// Номер грани ячейки (с 1)
	int*Faces;
	// Кол-во ячеек
	int CountOfCells;
	// Номер граничного условия (имя)
	int Condition;
};

/**
 * Структура сетки для записи/чтения в формате neu
 */
struct neu_mesh_struct{
	int CountOfPoints;
	REAL*Points;
	int Dimension;

	int CountOfCells;
	int*Cells;
	int*CellTypes;
	int*CellSizes;
	int*CellOffset;

	int CountOfBoundaryConditions;
	struct neu_boundary_condition_struct*BoundaryConditions;
};

/**
 * Инициализация структуры neu_mesh_struct
 */
void neu_mesh_struct_init(struct neu_mesh_struct*Mesh);

/**
 * Очистка структуры neu_mesh_struct
 */
void neu_mesh_struct_free(struct neu_mesh_struct*Mesh);

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
		int CountOfCells, int*Cells, int*CellTypes, int*CellSizes, int*CellOffset);

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
		int CountOfCells, int*Cells, int CellType);


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
int neu_mesh_struct_add_boundary_conditions(struct neu_mesh_struct*Mesh, int CountOfBoundaryConditions, int*ConditionNumber,int*CountOfCells, int**Cells,int**Faces);

/**
 * Добавить в структуру neu_mesh_struct данные о граничых условиях из массива стуктур граничных условий
 * 	граничные условия добавляются к текущим
 * Mesh - ссылка на структуру сетки
 * CountOfBoundaryConditions - число граничных условий
 * BoundaryConditions - массив структур граничных условий
 */
int neu_mesh_struct_add_boundary_conditions_struct(struct neu_mesh_struct*Mesh, int CountOfBoundaryConditions, struct neu_boundary_condition_struct*BoundaryConditions);


/**
 * Записать сетку Mesh в файл filename в формате neu
 */
int write_format_neu_struct(struct neu_mesh_struct*Mesh, char*filename);

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
		char*filename);

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
		char*filename);

/**
 * Прочитать сетку в Mesh из файла filename в формате neu
 */
int read_format_neu_struct(struct neu_mesh_struct*Mesh, char*filename);

#endif /* RW_MESH_NEU_H_ */
