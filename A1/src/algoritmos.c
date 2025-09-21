#include <stdio.h>
#include <float.h>
#include <stdlib.h>
#include "../include/fila.h"
#include "../include/grafo.h"
#include "../include/bfs.h"

void bfs(const grafo_t *g, const vertice_t *s) {
    //declarando as variáveis locais
    unsigned int num_vertices = g->num_vertices;
    unsigned int id_s = s->id;

    vertice_t **conj_vertices = g->conj_vertices;
    //inicializando os vetores C, D e A

    bool conhecidos[num_vertices] = {};
    float distancias[num_vertices];

    for (unsigned int i = 0; i < num_vertices; i++) {
        distancias[i] = FLT_MAX;
    }
    //configurando o vértice de origem
    conhecidos[id_s - 1] = true;
    distancias[id_s - 1] = 0;
    //preparando a fila de visitas
    fila_t *fila = criar_fila(id_s);
    //propagação das visitas
    unsigned int profundidade = 0;
    printf("Nível de profundidade: %d\n", profundidade);
    printf("Conhecendo a raiz com idd %d\n", id_s);
    profundidade++;
    while (!vazia(fila)) {
        //visitando o vértice u
        //printf("nova iteração do laço principal\n");
        //printf("fila antes da remoção: \n");
        //imprimir(fila);
        const unsigned int id_u = remover(fila);
        //printf("fila após remoção: \n");
        //imprimir(fila);
        const vertice_t *u = obter_vertice(id_u, conj_vertices, num_vertices);
        //printf("idd de u = %d\n", idd_u);
        vertice_t **vizinhos_u = u->vizinhos;
        const unsigned int grau_u = u->grau;
        for (unsigned int i = 0; i < grau_u; i++) {
            vertice_t *v = vizinhos_u[i];
            unsigned int id_v = v->id;
            //printf("idd de v = %d\n", idd_v);
            if (!conhecidos[id_v - 1]) {
                unsigned int ancestrais[num_vertices] = {};
                printf("Nível de profundidade: %d\n", profundidade);
                printf("conhecendo vértice com idd %d\n", id_v);
                //conhecendo o vértice v
                conhecidos[id_v - 1] = true;
                distancias[id_v - 1] = distancias[id_u - 1] + 1;
                ancestrais[id_v - 1] = id_u;
                inserir(id_v, fila);
            }
        }
        profundidade++;
    }
    return;
}

bfs_t *inicializar_bfs(unsigned int num_vertices, float *distancias, unsigned int *ancestrais) {
    /*
    float d[num_vertices];
    unsigned int a[num_vertices];
    for (int i = 0; i<num_vertices; i++)
    {
      d[i] = distancias[i];
      a[i] = ancestrais[i];
    }
  */
    bfs_t *bfs = malloc(sizeof(bfs_t) + (num_vertices * ((sizeof(float) + sizeof(unsigned int)))));
    bfs->distancias = distancias;
    bfs->ancestrais = ancestrais;
    return bfs;
}

int main() {
    grafo_t *g = ler("teste.txt");
    //imprimir_grafo(g);
    //testando as funções que medem a cardinalidade de V e E
    unsigned int card_V = qtdVertices(g);
    unsigned int card_E = qtdArestas(g);
    printf("Quantidade de Vértices: %d\n", card_V);
    printf("Quantidade de Arestas: %d\n", card_E);
    vertice_t *s = g->conj_vertices[0];
    unsigned int idd_s = s->id;
    printf("id de s: %d\n", idd_s);
    bfs(g, s);
    return 0;
}
