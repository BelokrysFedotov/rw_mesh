cmake_minimum_required ( VERSION 2.8 )
#project ( RW_MESH CXX )

option ( BUILD_TESTING "Enable/disable testing" ON )

add_subdirectory(src)

INCLUDE(CTest)

if ( BUILD_TESTING )
	message ( "-- Testing enabled" )
	add_subdirectory(gtest)
	add_subdirectory(tests)
endif ( BUILD_TESTING )