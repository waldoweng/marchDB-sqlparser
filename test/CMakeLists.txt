include_directories(${CMAKE_SOURCE_DIR}/include/ast)
include_directories(${CMAKE_SOURCE_DIR}/include/parser)
include_directories(${CMAKE_SOURCE_DIR}/build/src)

AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR} source)
add_executable(test_pmysql ${source})

target_compile_options(test_pmysql PUBLIC -Wall -Wextra)
target_link_libraries(test_pmysql PUBLIC libparser libast coverage_config)

add_test(NAME test_pmysql COMMAND ./test_pmysql)
