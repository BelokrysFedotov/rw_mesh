add_executable(test_read_vtk test_vtk_read.cpp)
add_definitions(-DWORK_DIRECTORY="${CMAKE_SOURCE_DIR}/")
target_link_libraries (test_read_vtk ${GLIBS})
add_test(test_read_vtk test_read_vtk)

add_executable (read_vtk_file read_vtk_file.c)
target_link_libraries (read_vtk_file ${LIBS})

add_executable (write_vtk_structed_file write_vtk_structed_file.c)
target_link_libraries (write_vtk_structed_file ${LIBS})

add_executable (write_vtk_unstructed_file write_vtk_unstructed_file.c)
target_link_libraries (write_vtk_unstructed_file ${LIBS})

add_executable (vtk_convert_to_ascii vtk_convert_to_ascii.c)
target_link_libraries (vtk_convert_to_ascii ${LIBS})

add_executable (vtk_convert_to_binary vtk_convert_to_binary.c)
target_link_libraries (vtk_convert_to_binary ${LIBS})