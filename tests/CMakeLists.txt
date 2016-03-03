enable_testing()

set(LIBS ${LIBS} rw_mesh_lib)
set(GLIBS ${LIBS} gtest gtest_main)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_subdirectory(string)

add_subdirectory(off)
add_subdirectory(mesh)
add_subdirectory(vtk)
add_subdirectory(plot3d)
add_subdirectory(cgns)
add_subdirectory(neu)