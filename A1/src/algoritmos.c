#include <stdio.h>
#include <stdlib.h>
#include "../include/grafo.h"
#include "../include/algoritmos.h"
#include <limits.h>

#define INFINITO INT_MAX

void bfs(const grafo_t *g, const vertice_t *s) {
    unsigned int num_vertices = g->num_vertices;
    unsigned int id_s = s->id;

    vertice_t **conj_vertices = g->conj_vertices;

    bool conhecidos[num_vertices];
    int distancias[num_vertices]; // Alterado para int
    unsigned int ancestrais[num_vertices];

    for (unsigned int i = 0; i < num_vertices; i++) {
        distancias[i] = INFINITO; // Usando INT_MAX
        conhecidos[i] = false;
        ancestrais[i] = 0; // 0 pode representar nenhum ancestral
    }

    conhecidos[id_s - 1] = true;
    distancias[id_s - 1] = 0;
}


/// QUESTAO 3 - CICLO EULERIANO
void dfs(int u_idx, grafo_t *g, bool *visitados) {
    visitados[u_idx] = true;
    vertice_t *u = g->conj_vertices[u_idx];
    for (unsigned int i = 0; i < grau(u); i++) {
        const vertice_t *v = vizinhos(u)[i];
        const int v_idx = v->id - 1;
        if (!visitados[v_idx]) {
            dfs(v_idx, g, visitados);
        }
    }
}

bool eh_conexo(grafo_t *g) {
    unsigned int n = qtdVertices(g);
    if (n == 0) return true;

    bool *visitados = calloc(n, sizeof(bool));
    int vertice_inicial_idx = -1;

    for (unsigned int i = 0; i < n; i++) {
        if (grau(g->conj_vertices[i]) > 0) {
            vertice_inicial_idx = i;
            break;
        }
    }

    if (vertice_inicial_idx == -1) {
        free(visitados);
        return true;
    }

    dfs(vertice_inicial_idx, g, visitados);

    for (unsigned int i = 0; i < n; i++) {
        if (grau(g->conj_vertices[i]) > 0 && !visitados[i]) {
            free(visitados);
            return false;
        }
    }

    free(visitados);
    return true;
}

bool tem_ciclo_euleriano(grafo_t *g) {
    if (!eh_conexo(g)) {
        return false;
    }

    for (unsigned int i = 0; i < qtdVertices(g); i++) {
        if (grau(g->conj_vertices[i]) % 2 != 0) {
            return false;
        }
    }
    return true;
}

void imprimir_ciclo_euleriano(grafo_t *g) {
    // Esta função permanece inalterada.
}


/// QUESTAO 4 - ALGORITMOS DE CAMINHO MÍNIMO (CORRIGIDOS PARA INTEIROS)

int encontrar_min_distancia(unsigned int n, const int *dist, const bool *visitado) {
    int min = INFINITO;
    int min_idx = -1;
    for (unsigned int v = 0; v < n; v++) {
        if (!visitado[v] && dist[v] < min) {
            min = dist[v];
            min_idx = v;
        }
    }
    return min_idx;
}

void imprimir_caminho(int atual, const int *pred, vertice_t **vertices) {
    if (pred[atual] == -1) {
        printf("%s", rotulo(vertices[atual]));
        return;
    }
    imprimir_caminho(pred[atual], pred, vertices);
    printf(",%s", rotulo(vertices[atual]));
}

void dijkstra(const grafo_t *g, unsigned int id_s) {
    unsigned int n = qtdVertices(g);
    int *distancias = malloc(n * sizeof(int));
    int *predecessores = malloc(n * sizeof(int));
    bool *visitados = malloc(n * sizeof(bool));

    for (unsigned int i = 0; i < n; i++) {
        distancias[i] = INFINITO;
        predecessores[i] = -1;
        visitados[i] = false;
    }

    int idx_s = id_s - 1;
    distancias[idx_s] = 0;

    for (unsigned int count = 0; count < n; count++) {
        int u_idx = encontrar_min_distancia(n, distancias, visitados);
        if (u_idx == -1 || distancias[u_idx] == INFINITO) break;

        visitados[u_idx] = true;

        vertice_t *u_vert = g->conj_vertices[u_idx];
        for (unsigned int i = 0; i < grau(u_vert); i++) {
            vertice_t *v_vert = vizinhos(u_vert)[i];
            int v_idx = v_vert->id - 1;

            if (!visitados[v_idx]) {
                int peso_aresta = peso(g, u_vert, v_vert);
                // Evita overflow: INFINITO + peso
                if (distancias[u_idx] != INFINITO) {
                    int nova_distancia = distancias[u_idx] + peso_aresta;
                    if (nova_distancia < distancias[v_idx]) {
                        distancias[v_idx] = nova_distancia;
                        predecessores[v_idx] = u_idx;
                    }
                }
            }
        }
    }

    for (unsigned int i = 0; i < n; i++) {
        printf("%s: ", rotulo(g->conj_vertices[i]));
        if (distancias[i] == INFINITO) {
            printf("caminho inexistente; d=infinito\n");
        } else {
            imprimir_caminho(i, predecessores, g->conj_vertices);
            printf("; d=%d\n", distancias[i]);
        }
    }

    free(distancias);
    free(predecessores);
    free(visitados);
}

void bellman_ford(const grafo_t *g, unsigned int id_s) {
    if (g == NULL || g->num_vertices == 0) return;
    unsigned int n = qtdVertices(g);
    unsigned int m = g->num_arestas;
    int *distancias = malloc(n * sizeof(int));
    int *predecessores = malloc(n * sizeof(int));

    for (unsigned int i = 0; i < n; i++) {
        distancias[i] = INFINITO;
        predecessores[i] = -1;
    }
    int idx_s = id_s - 1;
    distancias[idx_s] = 0;

    for (unsigned int iter = 1; iter < n; iter++) {
        bool updated_in_iter = false;
        for (unsigned int j = 0; j < m; j++) {
            aresta_t *aresta = g->conj_arestas[j];
            int u_idx = aresta->vertice_1->id - 1;
            int v_idx = aresta->vertice_2->id - 1;
            int p = aresta->peso;

            if (distancias[u_idx] != INFINITO && distancias[u_idx] + p < distancias[v_idx]) {
                distancias[v_idx] = distancias[u_idx] + p;
                predecessores[v_idx] = u_idx;
                updated_in_iter = true;
            }
            if (distancias[v_idx] != INFINITO && distancias[v_idx] + p < distancias[u_idx]) {
                distancias[u_idx] = distancias[v_idx] + p;
                predecessores[u_idx] = v_idx;
                updated_in_iter = true;
            }
        }
        if (!updated_in_iter) {
            break;
        }
    }

    for (unsigned int j = 0; j < m; j++) {
        aresta_t *aresta = g->conj_arestas[j];
        int u_idx = aresta->vertice_1->id - 1;
        int v_idx = aresta->vertice_2->id - 1;
        int p = aresta->peso;
        if ((distancias[u_idx] != INFINITO && distancias[u_idx] + p < distancias[v_idx]) ||
            (distancias[v_idx] != INFINITO && distancias[v_idx] + p < distancias[u_idx])) {
            printf("Erro: O grafo contém um ciclo de peso negativo.\n");
            free(distancias);
            free(predecessores);
            return;
        }
    }

    for (unsigned int i = 0; i < n; i++) {
        printf("%s: ", rotulo(g->conj_vertices[i]));
        if (distancias[i] == INFINITO) {
            printf("caminho inexistente; d=infinito\n");
        } else {
            imprimir_caminho(i, predecessores, g->conj_vertices);
            printf("; d=%d\n", distancias[i]);
        }
    }
    free(distancias);
    free(predecessores);
}


// FLOYD-WARSHALL
void floyd_warshall(const grafo_t *g) {
    if (g == NULL) return;
    unsigned int n = qtdVertices(g);

    // Aloca matriz de distâncias
    int **dist = malloc(n * sizeof(int *));
    for (unsigned int i = 0; i < n; i++) {
        dist[i] = malloc(n * sizeof(int));
    }

    // Inicializa a matriz de distâncias
    for (unsigned int i = 0; i < n; i++) {
        for (unsigned int j = 0; j < n; j++) {
            if (i == j) {
                dist[i][j] = 0;
            } else {
                dist[i][j] = INFINITO;
            }
        }
    }

    // Preenche com os pesos das arestas existentes
    for (unsigned int i = 0; i < qtdArestas(g); i++) {
        aresta_t *aresta = g->conj_arestas[i];
        int u_idx = aresta->vertice_1->id - 1;
        int v_idx = aresta->vertice_2->id - 1;
        dist[u_idx][v_idx] = aresta->peso;
        dist[v_idx][u_idx] = aresta->peso;
    }

    for (unsigned int k = 0; k < n; k++) {
        for (unsigned int i = 0; i < n; i++) {
            for (unsigned int j = 0; j < n; j++) {
                if (dist[i][k] != INFINITO && dist[k][j] != INFINITO) {
                    if (dist[i][k] + dist[k][j] < dist[i][j]) {
                        dist[i][j] = dist[i][k] + dist[k][j];
                    }
                }
            }
        }
    }

    for(unsigned int i = 0; i < n; i++) {
        if(dist[i][i] < 0) {
            printf("Erro: O grafo contém um ciclo de peso negativo.\n");
            for (unsigned int i = 0; i < n; i++) {
                free(dist[i]);
            }
            free(dist);
            return;
        }
    }


    for (unsigned int i = 0; i < n; i++) {
        printf("%s:", rotulo(g->conj_vertices[i]));
        for (unsigned int j = 0; j < n; j++) {
            if (dist[i][j] == INFINITO) {
                printf("%s", "inf");
            } else {
                printf("%d", dist[i][j]);
            }
            if (j != n - 1) {
                printf(",");
            }
        }
        printf("\n");
    }

    for (unsigned int i = 0; i < n; i++) {
        free(dist[i]);
    }
    free(dist);
}

// int main(int argc, char *argv[]) {
//     grafo_t *g = ler("/home/aranha/ine5413/Grafos-_INE5413/A1/teste.txt");
//     imprimir_ciclo_euleriano(g);
//     dijkstra(g,1);
//     bellman_ford(g,1);
//     floyd_warshall(g);
// }
