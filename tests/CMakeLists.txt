enable_testing()

set(LIBS ${LIBS} rw_mesh_lib)
set(GLIBS ${LIBS} gtest gtest_main)

add_executable (test_mesh_read mesh/test_mesh_read.c)
target_link_libraries (test_mesh_read ${LIBS})
add_test (test_mesh_read test_mesh_read)

add_executable (test_string_readline string/readline.c)
target_link_libraries (test_string_readline ${LIBS})
add_test (test_string_readline1 test_string_readline ../../files/mesh/all-in-one.mesh " MeshVersionFormatted 1")
add_test (test_string_readline2 test_string_readline ../../files/mesh/all-in-one.mesh " 0 0 7 0" 5)
#TODO добавить тестов

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(test_string_gtest string/string.cpp)
target_link_libraries (test_string_gtest ${GLIBS})
add_test(test_string_gtest test_string_gtest)

add_executable(test_read_off off/test_off_read.cpp)
add_definitions(-DWORK_DIRECTORY="${CMAKE_SOURCE_DIR}/")
target_link_libraries (test_read_off ${GLIBS})
add_test(test_read_off test_read_off)
#add_test(test_read_off.Test.cube3off test_read_off --gtest_filter Test.cube3off)
#add_test(test_read_off.Test.octaoff test_read_off --gtest_filter Test.octaoff)

add_executable(test_read_vtk vtk/test_vtk_read.cpp)
add_definitions(-DWORK_DIRECTORY="${CMAKE_SOURCE_DIR}/")
target_link_libraries (test_read_vtk ${GLIBS})
add_test(test_read_vtk test_read_vtk)

add_executable (read_vtk_file vtk/read_vtk_file.c)
target_link_libraries (read_vtk_file ${LIBS})

add_executable (write_vtk_structed_file vtk/write_vtk_structed_file.c)
target_link_libraries (write_vtk_structed_file ${LIBS})

