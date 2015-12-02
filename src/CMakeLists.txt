cmake_minimum_required ( VERSION 2.8 )
project ( RW_MESH_LIB C )


option ( CGNS_LIB "Enable cgns support" ON )

if(CGNS_LIB)
	message ( "-- CGNS lib enabled" )
	
	#set(CGNS_BUILD_CGNSTOOLS OFF)
	#set(CGNS_BUILD_SHARED ON)
	#set(CGNS_ENABLE_64BIT ON)
	#set(CGNS_ENABLE_FORTRAN OFF)
	#set(CGNS_ENABLE_HDF5 OFF)
	#set(CGNS_ENABLE_LEGACY OFF)
	#set(CGNS_ENABLE_SCOPING OFF)
	#set(CGNS_ENABLE_TEST OFF)
	#set(CGNS_USE_SHARED OFF)
	
	set ( CGNS_BUILD_CGNSTOOLS	OFF	CACHE INTERNAL "" FORCE )
	set ( CGNS_BUILD_SHARED		OFF	CACHE INTERNAL "" FORCE )
	set ( CGNS_ENABLE_64BIT		ON	CACHE INTERNAL "" FORCE )
	set ( CGNS_ENABLE_FORTRAN	OFF	CACHE INTERNAL "" FORCE )
	set ( CGNS_ENABLE_HDF5		OFF	CACHE INTERNAL "" FORCE )
	set ( CGNS_ENABLE_LEGACY	OFF	CACHE INTERNAL "" FORCE )
	set ( CGNS_ENABLE_SCOPING	OFF	CACHE INTERNAL "" FORCE )
	set ( CGNS_ENABLE_TESTS		OFF	CACHE INTERNAL "" FORCE )
	set ( CGNS_USE_SHARED 		OFF	CACHE INTERNAL "" FORCE )
	
	add_subdirectory(extlib/cgnslib)
	
endif()

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
if(CGNS_LIB)
	set ( LIBRARY_SOURCE_FILES ${LIBRARY_SOURCE_FILES} rw_mesh_cgns.c ) 
endif()

set ( LIBRARY_TYPE "STATIC" CACHE PATH "Static or shared version of library" )

if ( MSVC )
	add_definitions ( -D_CRT_SECURE_NO_WARNINGS )
endif ( MSVC )

add_library ( ${LIBRARY_NAME} ${LIBRARY_TYPE} ${LIBRARY_SOURCE_FILES} )

if(CGNS_LIB)
	target_link_libraries (${LIBRARY_NAME} cgns_static)
	include_directories("${CMAKE_SOURCE_DIR}/src/extlib/cgnslib/src/")
	include_directories("${CMAKE_BINARY_DIR}/src/extlib/cgnslib/src/")
endif()