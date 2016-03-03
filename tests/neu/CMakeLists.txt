add_executable (read_neu_file read_neu_file.c)
target_link_libraries (read_neu_file ${LIBS})
add_test (test_read_neu1 read_neu_file ../../../files/neu/mesh.neu)

#add_executable(test_read_neu test_neu_read.cpp)
#add_definitions(-DWORK_DIRECTORY="${CMAKE_SOURCE_DIR}/")
#target_link_libraries (test_neu_read ${GLIBS})
#add_test(test_neu_read test_neu_read)