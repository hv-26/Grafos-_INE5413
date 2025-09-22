cmake_minimum_required(VERSION 3.20)

project(Grafos_INE5413 C)

add_executable(meu_programa
        A1/src/algoritmos.c
        A1/src/fila.c
        A1/src/grafo.c
        A1/src/nodo.c
)

target_include_directories(meu_programa PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/A1)