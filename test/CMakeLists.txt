include_directories(${CMAKE_SOURCE_DIR}/include/ast)
include_directories(${CMAKE_SOURCE_DIR}/include/parser)
include_directories(${CMAKE_SOURCE_DIR}/build/src)

AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR} source)
add_executable(test_pmysql ${source})

target_compile_options(test_pmysql PUBLIC -Wall -Wextra)
target_link_libraries(test_pmysql PUBLIC libparser libast coverage_config)

add_test(NAME simple_exec COMMAND ./test_pmysql)
add_test(NAME Simple_tableless COMMAND ./test_pmysql "select high_priority sql_small_result 1 + 1 as a;")
add_test(NAME simple_select COMMAND ./test_pmysql "select * from a;")
add_test(NAME select_alias COMMAND ./test_pmysql "select * from a as b;")
add_test(NAME select_opt_alias COMMAND ./test_pmysql "select distinct row1 as b from a;")
add_test(NAME select_opt_alias_where COMMAND ./test_pmysql "select distinct row1 as b from a where c = 'abc';")
