cmake_minimum_required (VERSION 3.0)
project (tiny-python)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Run FA2LEXER to generate the lexer
add_custom_command (
  OUTPUT lexer.cpp
  COMMAND fa2lexer --hdr tokens.h --src lexer.cpp --tpl ${PROJECT_SOURCE_DIR}/automatons/tiny-python.fa2l
  MAIN_DEPENDENCY ${PROJECT_SOURCE_DIR}/automatons/tiny-python.fa2l
  DEPENDS ${PROJECT_SOURCE_DIR}/automatons/start.jff
          ${PROJECT_SOURCE_DIR}/automatons/number.jff
          ${PROJECT_SOURCE_DIR}/automatons/ArithOperators.jff
          ${PROJECT_SOURCE_DIR}/automatons/RelOperators.jff
          ${PROJECT_SOURCE_DIR}/automatons/literal.jff
          ${PROJECT_SOURCE_DIR}/automatons/comment.jff
          ${PROJECT_SOURCE_DIR}/automatons/function_elems.jff
          ${PROJECT_SOURCE_DIR}/automatons/array_elems.jff
  )

INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR})
include_directories(${PROJECT_SOURCE_DIR})

add_executable(${PROJECT_NAME} main.cpp lexer.cpp parser.cpp)

