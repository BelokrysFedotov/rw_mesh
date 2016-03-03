add_executable (write_cgns_structed_file write_cgns_structed_file.c)
target_link_libraries (write_cgns_structed_file ${LIBS})

add_executable (write_cgns_unstructed_file write_cgns_unstructed_file.c)
target_link_libraries (write_cgns_unstructed_file ${LIBS})