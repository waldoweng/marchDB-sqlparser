include_directories(${CMAKE_SOURCE_DIR}/include/ast)
include_directories(${CMAKE_SOURCE_DIR}/)

AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR} source)

add_library(libast ${source})
target_compile_options(libast PUBLIC -Wall -Wextra --coverage)
set_target_properties(libast PROPERTIES OUTPUT_NAME "ast")
target_include_directories(libast PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
