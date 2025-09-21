#include <stdio.h>
#include <float.h>
#include <stdlib.h>
#include "fila.h"
#include "grafo.h"
#include "buscalargura.h"

void bfs(grafo_t* g, vertice_t* s)
{
  //declarando as variáveis locais
  unsigned int num_vertices = g->num_vertices;
  unsigned int idd_s = s->idd;
  unsigned int idd_u;
  unsigned int idd_v;
  unsigned int grau_u;
  unsigned int grau_v;
  vertice_t* u;
  vertice_t* v;
  vertice_t** conj_vertices = g->conj_vertices;
  vertice_t** vizinhos_u;
  vertice_t** vizinhos_v;
  //inicializando os vetores C, D e A
  bool conhecidos[num_vertices] = {};
  unsigned int ancestrais[num_vertices] = {};
  float distancias[num_vertices];
  for (unsigned int i = 0; i<num_vertices; i++)
  {
    distancias[i] = FLT_MAX;
  }
  //configurando o vértice de origem
  conhecidos[idd_s-1] = true;
  distancias[idd_s-1] = 0;
  //preparando a fila de visitas
  fila_t* fila = criar_fila(idd_s);
  //propagação das visitas
  unsigned int profundidade = 0;
  printf("Nível de profundidade: %d\n", profundidade);
  printf("Conhecendo a raiz com idd %d\n", idd_s);
  profundidade++;
  while(!vazia(fila))
  {
    //visitando o vértice u
    //printf("nova iteração do laço principal\n");
    //printf("fila antes da remoção: \n");
    //imprimir(fila);
    idd_u = remover(fila);
    //printf("fila após remoção: \n");
    //imprimir(fila);
    u = obter_vertice(idd_u, conj_vertices, num_vertices);
    //printf("idd de u = %d\n", idd_u);
    vizinhos_u = u->vizinhos;
    grau_u = u->grau;
    for (unsigned int i=0; i<grau_u; i++)
    {
      v = vizinhos_u[i];
      grau_v = v->grau;
      idd_v = v->idd;
      //printf("idd de v = %d\n", idd_v);
      if (!conhecidos[idd_v-1])
      {
        printf("Nível de profundidade: %d\n", profundidade);
        printf("conhecendo vértice com idd %d\n", idd_v);
        //conhecendo o vértice v
        conhecidos[idd_v-1] = true;
        distancias[idd_v-1] = distancias[idd_u-1] + 1;
        ancestrais[idd_v-1] = idd_u;
        inserir(idd_v, fila);
      }
    }
    profundidade++;
  }
  return;
}

bfs_t* inicializar_bfs(unsigned int num_vertices, float* distancias, unsigned int* ancestrais)
{
  /*
  float d[num_vertices];
  unsigned int a[num_vertices];
  for (int i = 0; i<num_vertices; i++)
  {
    d[i] = distancias[i];
    a[i] = ancestrais[i];
  }
*/
  bfs_t* bfs = malloc(sizeof(bfs_t) + (num_vertices*((sizeof(float)+sizeof(unsigned int)))));
  bfs->distancias = distancias;
  bfs->ancestrais = ancestrais;
  return bfs;
}
int main()
{
  grafo_t* g = ler("teste.txt");
  //imprimir_grafo(g);
  //testando as funções que medem a cardinalidade de V e E
  unsigned int card_V = qtdVertices(g);
  unsigned int card_E = qtdArestas(g);
  printf("Quantidade de Vértices: %d\n", card_V);
  printf("Quantidade de Arestas: %d\n", card_E);
  vertice_t* s = g->conj_vertices[0];
  unsigned int idd_s = s->idd;
  printf("id de s: %d\n", idd_s);
  bfs(g, s);
  return 0;
}
