include_directories(${CMAKE_SOURCE_DIR}/include/ast)
include_directories(${CMAKE_SOURCE_DIR}/include/parser)
include_directories(${CMAKE_SOURCE_DIR}/build/src)

AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR} source)

find_program(FLEX "flex")
if(FLEX)
    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/pmysql.cc ${CMAKE_SOURCE_DIR/include/parser}/pmysql.hh
        COMMAND flex -o ${CMAKE_CURRENT_SOURCE_DIR}/pmysql.cc --header-file=${CMAKE_SOURCE_DIR}/include/parser/pmysql.hh ${CMAKE_CURRENT_SOURCE_DIR}/pmysql.l
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/pmysql.l
        )
else()
    message(FATAL_ERROR "FLEX NOT FOUND.")
endif()

find_program(BISON "bison")
if(BISON)
    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/pmysql.tab.cc ${CMAKE_SOURCE_DIR}/include/parser/pmysql.tab.hh
        COMMAND bison -vd ${CMAKE_CURRENT_SOURCE_DIR}/pmysql.yy && mv pmysql.tab.hh ${CMAKE_SOURCE_DIR}/include/parser && mv pmysql.tab.cc ${CMAKE_CURRENT_SOURCE_DIR}
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/pmysql.yy
        )
else()
    message(FATAL_ERROR "BISON NOT FOUND.")
endif()

add_library(libparser pmysql.tab.cc pmysql.cc)
target_compile_options(libparser PUBLIC -Wall -Wextra)
set_target_properties(libparser PROPERTIES OUTPUT_NAME "parser")
target_include_directories(libparser PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
