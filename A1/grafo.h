#ifndef GRAFO_H
#define GRAFO_H
#include "aresta.h"

typedef struct {
  vertice_t** conj_vertices;
  unsigned int num_vertices;
  unsigned int num_arestas;
  char* rotulo;
  aresta_t** conj_arestas;
} grafo_t;

//Declarando as funções utilizadas:
void imprimir_vertice(vertice_t* vertice);
void imprimir_aresta(aresta_t* aresta);
unsigned int qtdVertices(grafo_t* g);
unsigned int qtdArestas(grafo_t* g);
int haAresta(vertice_t* u, vertice_t* v);
float peso(vertice_t* u, vertice_t* v);
grafo_t* ler(char* filename);
char* gerar_string_a_partir_do_arquivo(char* filename);
vertice_t* obter_vertice(unsigned int idd, vertice_t** vertices, unsigned int cardinality);
void imprimir_grafo(grafo_t* grafo);
#endif
