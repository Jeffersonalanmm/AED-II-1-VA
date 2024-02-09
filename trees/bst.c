#include <stdio.h>
#include <stdlib.h>
#include "bst.h"
#include <string.h>

bstIndex* inicializarIndiceBst(int index, char* recipe)  {
	bstIndex *novo = (bstIndex*) malloc(sizeof(bstIndex));
	novo->index = index;
	novo->recipe = (char *) malloc(sizeof(recipe));
	strcpy(novo->recipe, recipe);
	return novo;
}

bst adicionarBst(bstIndex *valor, bst raiz) {
    if(raiz == NULL) {
		bst novo = (bst) malloc(sizeof(struct bstNode));
		novo->data = valor;
		novo->left = NULL;
		novo->right = NULL;
		return novo;
	}

	if(strcmp(valor->recipe, raiz->data->recipe) > 0) {
		raiz->right = adicionarBst(valor, raiz->right);
	} else {
		raiz->left = adicionarBst(valor, raiz->left);
	}
	return raiz;
}

bst remover_bst(bst raiz, char* recipe) {
  if (raiz == NULL)
    return raiz;

  if (strcmp(raiz->data->recipe, recipe) == 0) {
      if (raiz->right == NULL && raiz->left == NULL) {
			free(raiz->data);
	    	free (raiz);
	    	return NULL;
	    }
      if (raiz->right == NULL && raiz->left != NULL) {
		bst aux = raiz->left;
		free (raiz);
		return aux;
      }
      if (raiz->left == NULL && raiz->right != NULL) {
        bst aux = raiz->right;
        free (raiz);
        return aux;
      }
      raiz->data = maior_elemento_bst(raiz->left);
      raiz->left = remover_bst(raiz->left, raiz->data->recipe);
  } else {
	if (strcmp(recipe, raiz->data->recipe) > 0)
    	raiz->right = remover_bst(raiz->right, recipe);
	else
		raiz->left = remover_bst(raiz->left, recipe);
  }
  return raiz;
}

int altura_bst(bst raiz) {
	if(raiz == NULL) {
		return 0;
	}
	return 1 + maior(altura_bst(raiz->right), altura_bst(raiz->left));
}

int maior(int a, int b) {
	if(a > b)
		return a;
	else
		return b;
}

bstIndex* maior_elemento_bst(bst raiz) {
	if(raiz == NULL)
		return NULL;
	if(raiz->right == NULL)
		return raiz->data;
	else
		return maior_elemento_bst(raiz->right);
}

bstIndex* menor_elemento_bst(bst raiz) {
	if(raiz == NULL)
		return NULL;
	if(raiz->left == NULL)
		return raiz->data;
	else
		return menor_elemento_bst(raiz->left);
}

void pre_order_bst(bst raiz) {
	if(raiz != NULL) {
		imprimir_elemento_bst(raiz);
		pre_order_bst(raiz->left);
		pre_order_bst(raiz->right);
	}
}

void pos_order_bst(bst raiz) {
	if(raiz != NULL) {
		pos_order_bst(raiz->left);
		pos_order_bst(raiz->right);
		imprimir_elemento_bst(raiz);
	}
}

void in_order_bst(bst raiz) {
	if(raiz != NULL) {
		in_order_bst(raiz->left);
		imprimir_elemento_bst(raiz);
		in_order_bst(raiz->right);
	}
}

void imprimir_elemento_bst(bst raiz) {
	if(raiz != NULL) {
		printf("%s | ", raiz->data->recipe);

	}
}

int buscaIndiceBst(bst raiz, char* recipe) {
	if(recipe != NULL && raiz != NULL) {
		int cmp = strcmp(recipe, raiz->data->recipe);
		if(cmp == 0) {
			return raiz->data->index;
		}
		else {
			if(cmp > 0)
				return buscaIndiceBst(raiz->right, recipe);
			else 
				return buscaIndiceBst(raiz->left, recipe);
		}
	} else {
		return -1;
	}
}
