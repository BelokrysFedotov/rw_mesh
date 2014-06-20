cmake_minimum_required ( VERSION 2.8 )
#project ( RW_MESH CXX )



add_subdirectory(src)

option ( BUILD_TESTING "Enable/disable testing" ON )

INCLUDE(CTest)

if ( BUILD_TESTING )
	add_subdirectory(gtest)
	add_subdirectory(tests)
endif ( BUILD_TESTING )