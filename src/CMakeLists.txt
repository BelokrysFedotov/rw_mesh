cmake_minimum_required ( VERSION 2.8 )
project ( RW_MESH_LIB C )

set ( LIBRARY_NAME "rw_mesh_lib" )
set ( LIBRARY_HEADER 
	rw_mesh.h
)
set ( LIBRARY_SOURCE_FILES  
	rw_mesh_header.c
	rw_mesh_string.c
	rw_mesh_off.c
	rw_mesh_mesh.c
	rw_mesh_vtk.c
	rw_mesh_plot3d.c
	rw_mesh.c
)

set ( LIBRARY_TYPE "STATIC" CACHE PATH "Static or shared version of library" )

if ( MSVC )
	add_definitions ( -D_CRT_SECURE_NO_WARNINGS )
endif ( MSVC )

add_library ( ${LIBRARY_NAME} ${LIBRARY_TYPE} ${LIBRARY_SOURCE_FILES} )