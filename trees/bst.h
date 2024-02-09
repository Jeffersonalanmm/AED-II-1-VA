#ifndef BST_H
#define BST_H
#include <stdlib.h>

typedef struct bstIndex {
	char* recipe;
	int index; //posição em bytes do primeiro byte do regsitro no arquivo de dados
}bstIndex;

typedef struct bstNode {
	bstIndex *data;
	struct bstNode *left, *right;
} no_bst;

typedef bstNode * bst;

bstIndex * inicializarIndiceBst(int indice, char* nome);
bst adicionarBst(bstIndex *valor, bst raiz);
int altura(bst raiz);
int maior(int a, int b);
bstIndex * maior_elemento_bst(bst raiz);
bstIndex * menor_elemento_bst(bst raiz);
void pre_order_bst(bst raiz);
void pos_order_bst(bst raiz);
void in_order_bst(bst raiz);
bst remover_bst(bst raiz, char* nome);
void imprimir_elemento_bst(bst raiz);
int buscaIndiceBst(bst raiz, char* nome);

#endif