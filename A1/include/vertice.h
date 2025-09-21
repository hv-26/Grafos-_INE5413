#ifndef VERTICE_H
#define VERTICE_H


//Estrutura descrevendo um vértice
typedef struct vertice_t vertice_t;
struct vertice_t
{
  unsigned int idd;
  unsigned int grau;
  char* rotulo;
  vertice_t** vizinhos;
};

#endif
