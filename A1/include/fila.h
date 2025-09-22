#ifndef FILA_H
#define FILA_H
#include "nodo.h"

//Estrutura descrevendo uma fila implementada via lista ligada

typedef struct {
  unsigned int tamanho;
  nodo_t* primeiro;
  nodo_t* ultimo;
} fila_t;

//Assinaturas das funções relacionada à fila

fila_t* criar_fila(unsigned int primeiro_elemento);
void destruir_fila(fila_t* fila);
void imprimir_fila(fila_t* fila);
void inserir(unsigned int elemento, fila_t* fila);
unsigned int remover(fila_t* fila);
bool vazia(fila_t* fila);
#endif
