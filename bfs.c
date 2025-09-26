#include <stdio.h>
#include <limits.h>
#include <float.h>
#include <stdlib.h>
#include <stdbool.h>
#include "fila.h"
#include "grafo.h"
#include "buscalargura.h"

void bfs(grafo_t* g, vertice_t* s)
{
  //declarando as variáveis locais
  bool primeira_vez = true;
  bool nivel_concluido = false;
  unsigned int nivel_atual = 0;
  unsigned int contador_nivel_atual = 0;
  unsigned int tam_nivel_atual = 0;
  unsigned int tam_prox_nivel = 0;
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
  unsigned int distancias[num_vertices];
  for (unsigned int i = 0; i<num_vertices; i++)
  {
    distancias[i] = UINT_MAX;
  }
  //configurando o vértice de origem
  conhecidos[idd_s-1] = true;
  distancias[idd_s-1] = 0;
  //preparando a fila de visitas
  fila_t* fila = criar_fila(idd_s);
  //propagação das visitas
  printf("%d: ", nivel_atual);
  printf("%d\n", idd_s);
  nivel_atual++;
  unsigned int level_buffer[num_vertices - 1] = {};
  unsigned int num = 0;
  unsigned int j=0;
  while(!vazia(fila))
  {
    //printf("Nível de profundidade: %d\n", nivel_atual);
    //visitando o vértice u
    idd_u = remover(fila);
    u = obter_vertice(idd_u, conj_vertices, num_vertices);
    //printf("idd de u = %d\n", idd_u);
    vizinhos_u = u->vizinhos;
    grau_u = u->grau;
    if (!primeira_vez) 
    {
      if (nivel_concluido)
      {
        //printf("tam_nivel_atual = %d\n", tam_nivel_atual);
        tam_nivel_atual = tam_prox_nivel;
      }
      //printf("tam_nivel_atual = %d\n", tam_nivel_atual);
      tam_prox_nivel = 0;
      //printf("tam_prox_nivel = %d\n", tam_prox_nivel);
    }
    else
    {
      //printf("tam_nivel_atual = %d\n", tam_nivel_atual);
      tam_nivel_atual = grau_u;
      //printf("tam_nivel_atual = %d\n", tam_nivel_atual);
    }
    for (unsigned int i=0; i<grau_u; i++)
    {
      v = vizinhos_u[i];
      grau_v = v->grau;
      idd_v = v->idd;
      //printf("idd de v = %d\n", idd_v);
      if (!conhecidos[idd_v-1])
      {
        level_buffer[j] = idd_v;
        j++;
        //printf("contador_nivel_atual = %d\n", contador_nivel_atual);
        contador_nivel_atual ++;
        //printf("contador_nivel_atual = %d\n", contador_nivel_atual);
        //if (!primeira_vez)
        //{
        //printf("tam_prox_nivel = %d\n", tam_prox_nivel);
        tam_prox_nivel += (grau_v-1);
        //printf("tam_prox_nivel = %d\n", tam_prox_nivel);
        //}
        //printf("conhecendo vértice com idd %d\n", idd_v);
        //conhecendo o vértice v
        conhecidos[idd_v-1] = true;
        distancias[idd_v-1] = distancias[idd_u-1] + 1;
        ancestrais[idd_v-1] = idd_u;
        inserir(idd_v, fila);
      }
    }
    primeira_vez = false;
    nivel_concluido = (contador_nivel_atual == tam_nivel_atual) ? true : false;
    //printf("Percorridos: %d\nTotal: %d\n", contador_nivel_atual, tam_nivel_atual);
    if (((nivel_concluido) && (tam_nivel_atual)))
    {
      //printf("nivel %d concluído!\n", nivel_atual);
      //printf("tam_nivel_atual = %d\n", tam_nivel_atual);
      printf("%d: ", nivel_atual);
      //printf("Vertices visitados:\n");
      if (tam_nivel_atual == 1)
      {
        num = level_buffer[0];
        printf("%d\n", num);
      }
      else
      {
        for (int i=0; i<tam_nivel_atual-1; i++)
        {
          num = level_buffer[i];
          printf("%d, ", num);
        }
        num = level_buffer[tam_nivel_atual-1];
        printf("%d\n", num);
      }
      j = 0;
      //printf("tam_prox_nivel = %d\n", tam_prox_nivel);
      nivel_atual++;
      tam_nivel_atual = tam_prox_nivel;
      contador_nivel_atual = 0;
      //printf("tam_nivel_atual = %d\n", tam_prox_nivel);
      //printf("tam_prox_nivel = %d\n", tam_prox_nivel);
      if (tam_prox_nivel == 0) 
      {
        break;
      }
    }
    if (!nivel_concluido) continue;
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
int main(int argc, char* argv[])
{
  if (argc != 3) return 1;
  char* alvo = argv[1];
  unsigned int idd_alvo = atoi(argv[2]);
  grafo_t* g = ler(alvo);
  //imprimir_grafo(g);
  //testando as funções que medem a cardinalidade de V e E
  unsigned int card_V = qtdVertices(g);
  unsigned int card_E = qtdArestas(g);
  //printf("Quantidade de Vértices: %d\n", card_V);
  //printf("Quantidade de Arestas: %d\n", card_E);
  vertice_t ** conj_vertices = g->conj_vertices;
  unsigned int num_vertices = g->num_vertices;
  vertice_t* s = obter_vertice(idd_alvo, conj_vertices, num_vertices);
  unsigned int idd_s = s->idd;
  //printf("id de s: %d\n", idd_s);
  bfs(g, s);
  return 0;
}
