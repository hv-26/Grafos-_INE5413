#ifndef ARESTA_H
#define ARESTA_H
#include "vertice.h"

//Estrutura descrevendo uma aresta

typedef struct {
  vertice_t* vertice_1;
  vertice_t* vertice_2;
  float peso;
  char* rotulo;
} aresta_t;

#endif
