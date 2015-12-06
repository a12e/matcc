project(matcc)
cmake_minimum_required(VERSION 3.1)
find_package(FLEX)
find_package(BISON)
set(CMAKE_C_STANDARD 99) # pour les déclarationd d'itérateurs dans le for

FLEX_TARGET(lexer src/lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lexer.c)
BISON_TARGET(parser src/parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.c)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

ADD_FLEX_BISON_DEPENDENCY(lexer parser)

# Création de l'exécutable final
add_executable(matcc
		src/utility.c
		src/hash_table.c
		${FLEX_lexer_OUTPUTS}
		${BISON_parser_OUTPUTS} src/instructions.c src/instructions.h src/actions.c src/actions.h)

include_directories(src)

# Link des bibliothèques
target_link_libraries(matcc
	libfl.a
	${BISON_LIBRARIES})

