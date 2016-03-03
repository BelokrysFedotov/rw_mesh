add_executable (test_mesh_read test_mesh_read.c)
target_link_libraries (test_mesh_read ${LIBS})
add_test (test_mesh_read test_mesh_read)
