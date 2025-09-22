#ifndef ALGORITMOS_H
#define ALGORITMOS_H
#include "grafo.h"

typedef struct {
  float* distancias;
  unsigned int* ancestrais;
} bfs_t;

//Assinatura da função que implementa a busca em largura
bfs_t* inicializar_bfs(unsigned int num_vertices, float* distancias, unsigned int* ancestrais);
void bfs(const grafo_t *g, const vertice_t *s);
void dfs(int u_idx, grafo_t *g, bool *visitados);
bool eh_conexo(grafo_t *g);
bool tem_ciclo_euleriano(grafo_t *g);
void imprimir_ciclo_euleriano(grafo_t *g);
#endif
