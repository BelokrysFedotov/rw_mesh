# Проверка чтения сторок из файла
add_executable (test_string_readline readline.c)
target_link_libraries (test_string_readline ${LIBS})
add_test (test_string_readline1 test_string_readline ../../../files/mesh/all-in-one.mesh " MeshVersionFormatted 1")
add_test (test_string_readline2 test_string_readline ../../../files/mesh/all-in-one.mesh " 0 0 7 0" 5)
#TODO добавить тестов

# Тест функций 
add_executable(test_string_gtest string.cpp)
target_link_libraries (test_string_gtest ${GLIBS})
add_test(test_string_gtest test_string_gtest)
