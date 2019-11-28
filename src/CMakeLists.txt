include_directories(${CMAKE_SOURCE_DIR}/include/ast)
include_directories(${CMAKE_SOURCE_DIR}/include/parser)
include_directories(${CMAKE_SOURCE_DIR}/build/src)

AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR} source)

add_executable(pymsql ${source})

target_compile_options(pymsql PUBLIC -Wall -Wextra)
target_link_libraries(pymsql PUBLIC libparser libast coverage_config)
