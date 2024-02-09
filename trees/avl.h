#ifndef AVL_H
#define AVL_H
#include <stdlib.h>

typedef struct avlIndex {
	int difficulty;
	int index; //posição em bytes do primeiro byte do regsitro no arquivo de dados
}avlIndex;

typedef struct avlNode {
	int fb;
    avlIndex *data;
	struct avlNode *left, *right;
}avlNode;

typedef avlNode* avl;

avlIndex* inicializar_indice_avl(int indice, int codigo);

avl adicionar_avl(avl a, avlIndex *v, int *cresceu);

int buscar_indice_avl(avl raiz, int codigo);

avl remover_elemento_avl(avl a, int v, int *diminuiu);

avl rotacao_avl(avl a);

avl rotacao_simples_direita_avl(avl a);

avl rotacao_simples_esquerda_avl(avl a);

avl rotacao_dupla_direita_avl(avl a);

avl rotacao_dupla_esquerda_avl(avl a);

void pre_order_avl(avl raiz);
void pos_order_avl(avl raiz);
void in_order_avl(avl raiz);
void imprimir_elemento_avl(avl raiz);

#endif