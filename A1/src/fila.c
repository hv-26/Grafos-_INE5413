#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../include/nodo.h"
#include "../include/fila.h"


fila_t *criar_fila(unsigned int primeiro_elemento) {
    fila_t *nova_fila = malloc(sizeof(fila_t));
    nodo_t *novo_nodo = criar_nodo(primeiro_elemento, NULL);
    nova_fila->tamanho = 1;
    nova_fila->primeiro = novo_nodo;
    nova_fila->ultimo = novo_nodo;
    return nova_fila;
}

void destruir_fila(fila_t *fila) {
    unsigned int tamanho = fila->tamanho;
    if (tamanho) {
        for (unsigned int i = 0; i < tamanho; i++) {
            remover(fila);
        }
    }
    free(fila);
}

void imprimir(fila_t *fila) {
    unsigned int tamanho = fila->tamanho;
    if (!tamanho) {
        printf("FILA VAZIA\n");
        return;
    }
    nodo_t *nodo_atual = fila->primeiro;
    unsigned int valor_atual = nodo_atual->val;
    printf("Fila: [ ");
    for (unsigned int i = 0; i < tamanho; i++) {
        //printf("i = %d\n", i);
        valor_atual = nodo_atual->val;
        printf("%d, ", valor_atual);
        nodo_t *proximo_nodo = nodo_atual->proximo;
        nodo_atual = proximo_nodo;
    }
    printf("]\n");
}

void inserir(unsigned int elemento, fila_t *fila) {
    nodo_t *novo_nodo = criar_nodo(elemento, NULL);
    if (!fila->tamanho) {
        fila->primeiro = novo_nodo;
        fila->ultimo = novo_nodo;
        fila->tamanho++;
        return;
    }
    fila->ultimo->proximo = novo_nodo;
    fila->ultimo = novo_nodo;
    fila->tamanho++;
}

unsigned int remover(fila_t *fila) {
    unsigned int retval = fila->primeiro->val;
    nodo_t *primeiro = fila->primeiro;
    nodo_t *segundo = fila->primeiro->proximo;
    destruir_nodo(primeiro);
    fila->primeiro = segundo;
    fila->tamanho--;
    return retval;
}

bool vazia(fila_t *fila) {
    return !(fila->tamanho);
}

//modulo de teste
/*
int main()
{
  fila_t* fila = criar_fila(12);
  imprimir(fila); //->[12]->
  inserir(22, fila);
  imprimir(fila); //->[22, 12]->
  inserir(32, fila);
  imprimir(fila); //->[32, 22, 12]->
  unsigned int teste = remover(fila);
  imprimir(fila); //->[32, 22]->
  teste = remover(fila);
  imprimir(fila); //->[32]->
  teste = remover(fila);
  imprimir(fila); //->[]->
  inserir(42, fila);
  imprimir(fila); //->[42]->
  teste = remover(fila);
  imprimir(fila); //->[]->
  destruir_fila(fila);
  return 0;
}
*/
