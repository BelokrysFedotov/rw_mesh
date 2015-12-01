cmake_minimum_required ( VERSION 2.8 )
#project ( RW_MESH CXX )

option ( CGNS_LIB "Enable cgns support" ON )
option ( BUILD_TESTING "Enable/disable testing" ON )

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


add_subdirectory(src)


INCLUDE(CTest)

if ( BUILD_TESTING )
	message ( "-- Testing enabled" )
	add_subdirectory(gtest)
	add_subdirectory(tests)
endif ( BUILD_TESTING )