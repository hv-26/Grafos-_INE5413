#include <stdlib.h>
#include "../include/nodo.h"

/*int main()
{
  return 0;
}
*/
nodo_t *criar_nodo(int novo_elemento, nodo_t *proximo) {
    nodo_t *novo_nodo = malloc(sizeof(nodo_t));
    novo_nodo->val = novo_elemento;
    novo_nodo->proximo = proximo;
    return novo_nodo;
}

void destruir_nodo(nodo_t *vitima) {
    free(vitima);
    return;
}
