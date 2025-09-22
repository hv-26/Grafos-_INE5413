#ifndef GRAFO_H
#define GRAFO_H
#include "aresta.h"

typedef struct {
    vertice_t **conj_vertices;
    unsigned int num_vertices;
    unsigned int num_arestas;
    char *rotulo;
    aresta_t **conj_arestas;
} grafo_t;

//Declarando as funções utilizadas:
void imprimir_vertice(const vertice_t *vertice);

void imprimir_aresta(const aresta_t *aresta);

unsigned int qtdVertices(const grafo_t *g);

unsigned int qtdArestas(const grafo_t *g);

char *rotulo(const vertice_t *v);

unsigned int grau(const vertice_t *v);

vertice_t **vizinhos(const vertice_t *v);

bool haAresta(const grafo_t *g, const vertice_t *u, const vertice_t *v);

float peso(const grafo_t *g, const vertice_t *u, const vertice_t *v);

grafo_t *ler(const char *filename);

char *gerar_string_a_partir_do_arquivo(const char *filename);

vertice_t *obter_vertice(unsigned int idd, vertice_t **vertices, unsigned int cardinality);

void imprimir_grafo(const grafo_t *grafo);

#endif
