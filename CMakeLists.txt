project(matcc)
cmake_minimum_required(VERSION 3.1)
set(CMAKE_C_STANDARD 99)

find_package(FLEX)
find_package(BISON)
FLEX_TARGET(lexer src/parsing/lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lexer.c)
BISON_TARGET(parser src/parsing/parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.c)
ADD_FLEX_BISON_DEPENDENCY(lexer parser)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(src)

add_executable(matcc

		${FLEX_lexer_OUTPUTS}
		${BISON_parser_OUTPUTS}

		src/mips/instruction.h
		src/mips/instruction.c
		src/mips/register_allocation.h
		src/mips/register_allocation.c
		src/mips/translation.h
		src/mips/translation.c

		src/parsing/actions.h
		src/parsing/actions.c

		src/hash_table.h
		src/hash_table.c
		src/main.c
		src/matrix.h
		src/matrix.c
		src/quad.h
		src/quad.c
		src/quad_list.h
		src/quad_list.c
		src/symbol.h
		src/symbol.c
		src/symbol_table.h
		src/symbol_table.c
		src/utility.h
		src/utility.c
		)

target_link_libraries(matcc
		${FLEX_LIBRARIES}
		${BISON_LIBRARIES})
