#ifndef NODO_H
#define NODO_H


//Estrutura descrevendo um nodo em uma lista ligada
typedef struct nodo_t nodo_t;
struct nodo_t
{
  unsigned int val;
  nodo_t* proximo;
};

nodo_t* criar_nodo(int novo_elemento, nodo_t* proximo_nodo);
void destruir_nodo(nodo_t* nodo_vitima);

#endif
