#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <float.h>
#include "grafo.h"

int main()
{
  grafo_t* g = ler("teste.txt");
  imprimir_grafo(g);
  //testando as funções que medem a cardinalidade de V e E
  unsigned int card_V = qtdVertices(g);
  unsigned int card_E = qtdArestas(g);
  printf("Quantidade de Vértices: %d\n", card_V);
  printf("Quantidade de Arestas: %d\n", card_E);
  //testando as funções que dizem respeito aos vértices
  vertice_t** conj_vertices = g->conj_vertices;
  vertice_t** vizinhanca;
  vertice_t* vertice_atual;
  char* label_vertice_atual;
  unsigned int grau_vertice_atual;

  for (int i=0; i<card_V; i++)
  {
    vertice_atual = conj_vertices[i];
    label_vertice_atual = rotulo(vertice_atual);
    grau_vertice_atual = grau(vertice_atual);
    printf("Grau do vértice %s: %d\n", label_vertice_atual, grau_vertice_atual);
    printf("Vizinhos do vértice %s:\n", label_vertice_atual);
    vizinhanca = vizinhos(vertice_atual);
    for (int j=0; j<grau_vertice_atual; j++)
    {
      printf("Vizinho [%d] de %s: \n", j, label_vertice_atual);
      imprimir_vertice(vizinhanca[j]);
    }
  }

  //testando as funções que dizem respeito às arestas
  aresta_t** conj_arestas = g->conj_arestas;
  aresta_t* aresta_atual;
  vertice_t* u;
  vertice_t* v;
  char rotulo_aresta[6] = "";
  float peso_aresta;
  for (int i=0; i<card_E; i++)
  {
    aresta_atual = conj_arestas[i];
    u = aresta_atual->vertice_1;
    v = aresta_atual->vertice_2;
    strncpy(rotulo_aresta, aresta_atual->rotulo, 7);
    //rotulo_aresta = aresta_atual->rotulo;
    if (!haAresta(g, u, v))
    {
      printf("ERRO: ARESTA %s NÃO DETECTADA!\n", rotulo_aresta);
      return 1;
    }
    peso_aresta = peso(g, u, v);
    printf("Aresta atual: %s, com peso %f\n", rotulo_aresta, peso_aresta);
  }
  return 0;
}


grafo_t* ler(char* filename)
{
  //lista de ingredientes do grafo
  unsigned int num_vertices;
  unsigned int num_max_arestas = (num_vertices*(num_vertices-1))/2;
  unsigned int num_arestas = 0;
  unsigned int idd_novo_vertice;
  unsigned int idd_vertice_1;
  unsigned int idd_vertice_2;
  char* rotulo;
  aresta_t** conj_arestas = malloc(num_max_arestas*sizeof(aresta_t));
  struct vertice_t* vertice_1 = NULL;
  struct vertice_t* vertice_2 = NULL;
  float peso;
  char* rotulo_vertice_1;
  char* rotulo_vertice_2;
  char nome_do_grafo[20];

  char* input_string = gerar_string_a_partir_do_arquivo(filename);
  //início do parsing (token_atual = "vertices")
  char* token_atual = strtok(input_string, "*vertices ");

  //token_atual = n
  num_vertices = (unsigned int) strtol(token_atual, (char**) NULL, 10);
  vertice_t** conj_vertices = malloc(num_vertices*sizeof(vertice_t));

  //criando os vértices do grafo
  for (unsigned int i = 0; i < (num_vertices << 1); i++)
  {
    //idd de um vértice ou fim da seção "*vertices"
    token_atual = strtok(NULL, " ");
    if (i % 2 == 0)
    {
      idd_novo_vertice = (int) strtol(token_atual, (char**) NULL, 10);
    }
    //label de um vértice
    else
    {
      rotulo = token_atual;
      vertice_t** vizinhos = malloc(sizeof(vertice_t)*(num_vertices-1));
      vertice_t* novo_vertice = malloc(sizeof(vertice_t)); //free me!
      novo_vertice->idd = idd_novo_vertice;
      novo_vertice->grau = 0;
      novo_vertice->vizinhos = vizinhos;
      novo_vertice->rotulo = rotulo;
      printf("VERTICE NOVO:\n IDD: %d,\n GRAU: %d, \n LABEL: %s\n", novo_vertice->idd, novo_vertice->grau, novo_vertice->rotulo);
      conj_vertices[i>>1] = novo_vertice;
    }
  }
  //criando as arestas do grafo
  //token atual é "NULL" se e somente se a string terminou
  token_atual = strtok(NULL, " ");
  while (1)
  {
    token_atual = strtok(NULL, " ");
    if (token_atual == NULL)
    {
      break;
    }
    char rotulo_aresta[6] = "{";
    unsigned int id_aresta[2];
    rotulo_vertice_1 = token_atual;
    idd_vertice_1 = atoi(token_atual);
    id_aresta[0] = idd_vertice_1;
    token_atual = strtok(NULL, " ");
    rotulo_vertice_2 = token_atual;
    idd_vertice_2 = atoi(token_atual);
    id_aresta[1] = idd_vertice_2;
    vertice_1 = obter_vertice(idd_vertice_1, conj_vertices, num_vertices);
    vertice_2 = obter_vertice(idd_vertice_2, conj_vertices, num_vertices);
    peso = strtof(token_atual, NULL);
    token_atual = strtok(NULL, " ");
    peso = strtof(token_atual, NULL);

    aresta_t* nova_aresta = malloc(sizeof(aresta_t));
    nova_aresta->rotulo = malloc(sizeof(char)*6);
    strncat(rotulo_aresta, rotulo_vertice_1, 5);
    strncat(rotulo_aresta, ", ", 5);
    strncat(rotulo_aresta, rotulo_vertice_2, 5);
    strncat(rotulo_aresta, "}", 5);
    nova_aresta->rotulo = strdup(rotulo_aresta);
    nova_aresta->idd[0] = id_aresta[0];
    nova_aresta->idd[1] = id_aresta[1];
    nova_aresta->vertice_1 = vertice_1;
    nova_aresta->vertice_2 = vertice_2;
    nova_aresta->peso = peso;

    //atualizando os dados dos vértices envolvidos 
    vertice_1->vizinhos[vertice_1->grau++] = vertice_2;
    vertice_2->vizinhos[vertice_2->grau++] = vertice_1;

    conj_arestas[num_arestas++] = nova_aresta;
  }

  //criando o grafo que será retornado
  printf("Nomeie o grafo:\n");
  while (1)
  {

    if (scanf("%19s", nome_do_grafo) != 1)
    {
      printf("ERRO: Nome muito longo. Tente novamente:");
      printf("Nomeie o grafo:\n");
    }
    else
      break;
  }
  printf("Nome do grafo: %s\n", nome_do_grafo);
  grafo_t* grafo = malloc(sizeof(grafo_t) + num_arestas*sizeof(aresta_t));
  grafo->rotulo = nome_do_grafo;
  grafo->conj_vertices = conj_vertices;
  grafo->conj_arestas = conj_arestas;
  grafo->num_vertices = num_vertices;
  grafo->num_arestas = num_arestas;
  return grafo;
}

char* gerar_string_a_partir_do_arquivo(char* filename)
{
  FILE* arquivo = fopen(filename, "r");

  if (arquivo == NULL) return NULL;

  fseek(arquivo, 0, SEEK_END);
  int tam_arquivo = ftell(arquivo);
  fseek(arquivo, 0, SEEK_SET);

  // +1 byte para o caractere terminal
  char* string = malloc(sizeof(char)*(tam_arquivo+1));
  char c;
  int i = 0;
  while ((c = fgetc(arquivo)) != EOF)
  {
    if (c=='\n') c = ' ';
    string[i++] = c;
  }
  // sinalizando o fim da string
  string[i] = '\0';
  return string;
}

vertice_t* obter_vertice(unsigned int idd, vertice_t** vertices, unsigned int cardinality)
{
  for (int i = 0; i < cardinality; i++)
  {
    if (vertices[i]->idd == idd) return vertices[i];
  }
  printf("obter vertice retornou NULL :(\n");
  return NULL;
}

aresta_t* identificar_aresta (grafo_t* g, vertice_t* u, vertice_t* v)
{
  aresta_t* aresta_identificada;
  aresta_t** conj_arestas = g->conj_arestas;
  unsigned int num_arestas = g->num_arestas;
  unsigned int idd_alvo_1[2] = {u->idd, v->idd};
  unsigned int idd_alvo_2[2] = {v->idd, u->idd};
  unsigned int idd_atual[2];
  for (int i = 0; i<num_arestas; i++)
  {
    idd_atual[0] = conj_arestas[i]->idd[0];
    idd_atual[1] = conj_arestas[i]->idd[1];
    if (idd_atual == idd_alvo_1 || idd_atual == idd_alvo_2)
    {
      return conj_arestas[i];
    }
  }
  return NULL;
}

float peso(grafo_t* g, vertice_t* u, vertice_t* v)
{
  aresta_t* aresta = identificar_aresta(g, u, v);
  if (aresta == NULL) return FLT_MAX;
  return aresta->peso;
}

void imprimir_grafo(grafo_t* grafo)
{
  vertice_t** vertices = grafo->conj_vertices;
  aresta_t** arestas = grafo->conj_arestas;
  unsigned int num_vertices = grafo->num_vertices;
  unsigned int num_arestas = grafo->num_arestas;
  char* nome_grafo = grafo->rotulo;
  printf("IMPRIMINDO GRAFO: %s\n", nome_grafo);
  printf("Número de vértices: %d\n", num_vertices);
  printf("Lista de vértices:\n");
  for (unsigned int i = 0; i<num_vertices; i++) imprimir_vertice(vertices[i]);
  printf("Número de arestas: %d\n", num_arestas);
  printf("Lista de arestas:\n");
  for (unsigned int i = 0; i<num_arestas; i++) imprimir_aresta(arestas[i]);
  return;
}

void imprimir_aresta (aresta_t* aresta)
{
  float peso = aresta->peso;
  char label[6] = "";
  strncpy(label, aresta->rotulo, 7);
  printf("Aresta %s, com peso: %f\n", label, peso);
  return;
}

void imprimir_vertice(vertice_t* vertice)
{
  char nome[64] = "";
  strncpy(nome, vertice->rotulo, 64);
  unsigned int idd = vertice->idd;
  unsigned int grau = vertice->grau;
  vertice_t** vizinhos = vertice->vizinhos;
  printf("Vértice %s:\n IDD: %d\n Grau: %d\n", nome, idd, grau);
  /*
  if (grau)
  {
    printf("Vizinhos de %s:\n", nome);
    for (int i=0; i<grau; i++)
    {
      printf("Vizinho[%d]: %s\n", i, vizinhos[i]->rotulo);
    }
  }
  */
  return;
}

unsigned int qtdVertices(grafo_t* g)
{
  return g->num_vertices;
}

unsigned int qtdArestas(grafo_t* g)
{
  return g->num_arestas;
}

unsigned int grau(vertice_t* v)
{
  return v->grau;
}

char* rotulo(vertice_t* v)
{
  return v->rotulo;
}

vertice_t** vizinhos(vertice_t* v)
{
  return v->vizinhos;
}

bool haAresta (grafo_t* g, vertice_t* u, vertice_t* v)
{
  printf("EXECUTANDO haAresta\n");
  aresta_t** conj_arestas = g->conj_arestas;
  unsigned int num_arestas = g->num_arestas;
  unsigned int idd_alvo_1[2] = {u->idd, v->idd};
  printf("idd alvo 1: (%d, %d)\n", idd_alvo_1[0], idd_alvo_1[1]);
  unsigned int idd_alvo_2[2] = {v->idd, u->idd};
  printf("idd alvo 2: (%d, %d)\n", idd_alvo_2[0], idd_alvo_2[1]);
  unsigned int idd_atual[2];
  for (int i = 0; i<num_arestas; i++)
  {
    idd_atual[0] = conj_arestas[i]->idd[0];
    idd_atual[1] = conj_arestas[i]->idd[1];
    printf("idd atual: (%d, %d)\n", idd_atual[0], idd_atual[1]);
    if ((idd_atual[0] == idd_alvo_1[0] && idd_atual[1] == idd_alvo_1[1]) || (idd_atual[0] == idd_alvo_2[0] && idd_atual[1] == idd_alvo_2[1]))
    {
      return true;
    }
  }
  return false;
}
