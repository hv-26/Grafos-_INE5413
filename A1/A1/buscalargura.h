#ifndef BFS_H
#define BFS_H
#include "grafo.h"

typedef struct {
  float* distancias;
  unsigned int* ancestrais;
} bfs_t;

//Assinatura da função que implementa a busca em largura
bfs_t* inicializar_bfs(unsigned int num_vertices, float* distancias, unsigned int* ancestrais);
void bfs(grafo_t* g, vertice_t* s);

#endif
