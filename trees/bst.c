#include <stdio.h>
#include <stdlib.h>
#include "bst.h"
#include <string.h>

// Inicializa um novo índice BST
bstIndex* initializeIndexBst(int index, char* name)  {
	bstIndex *novo = (bstIndex*) malloc(sizeof(bstIndex));
	novo->index = index;
	novo->name = (char *) malloc(sizeof(name));
	strcpy(novo->name, name);
	return novo;
}

// Insere um novo nó BST
bst insertBst(bstIndex *valor, bst raiz) {
    if(raiz == NULL) {
		bst novo = (bst) malloc(sizeof(struct bstNode));
		novo->data = valor;
		novo->left = NULL;
		novo->right = NULL;
		return novo;
	}

	if(strcmp(valor->name, raiz->data->name) > 0) {
		raiz->right = insertBst(valor, raiz->right);
	} else {
		raiz->left = insertBst(valor, raiz->left);
	}
	return raiz;
}

// Remove um nó BST
bst removeBst(bst raiz, char* name) {
  if (raiz == NULL)
    return raiz;

  if (strcmp(raiz->data->name, name) == 0) {
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
      raiz->data = maxElementBst(raiz->left);
      raiz->left = removeBst(raiz->left, raiz->data->name);
  } else {
	if (strcmp(name, raiz->data->name) > 0)
    	raiz->right = removeBst(raiz->right, name);
	else
		raiz->left = removeBst(raiz->left, name);
  }
  return raiz;
}

// Retorna a altura da árvore BST
int heightBst(bst raiz) {
	if(raiz == NULL) {
		return 0;
	}
	return 1 + max(heightBst(raiz->right), heightBst(raiz->left));
}

// Retorna o maior entre dois números
int max(int a, int b) {
	if(a > b)
		return a;
	else
		return b;
}

// Retorna o elemento máximo na árvore BST
bstIndex* maxElementBst(bst raiz) {
	if(raiz == NULL)
		return NULL;
	if(raiz->right == NULL)
		return raiz->data;
	else
		return maxElementBst(raiz->right);
}

// Retorna o elemento mínimo na árvore BST
bstIndex* minElementBst(bst raiz) {
	if(raiz == NULL)
		return NULL;
	if(raiz->left == NULL)
		return raiz->data;
	else
		return minElementBst(raiz->left);
}

// Percorre a árvore BST em ordem
void inOrderBst(bst raiz) {
	if(raiz != NULL) {
		inOrderBst(raiz->left);
		printElementBst(raiz);
		inOrderBst(raiz->right);
	}
}

// Imprime um elemento da árvore BST
void printElementBst(bst raiz) {
	if(raiz != NULL) {
		printf("%s | ", raiz->data->name);

	}
}

// Procura um índice BST com base no nome
int searchIndexBst(bst root, char* name) {
    if (root == NULL) {
        return -1; // Retorna -1 se a árvore estiver vazia ou se o elemento não for encontrado
    }
    
    int cmp = strcmp(name, root->data->name);
    
    if (cmp == 0) {
        return root->data->index; // Retorna o índice se o nome for encontrado
    } else if (cmp < 0) {
        return searchIndexBst(root->left, name); // Procura na subárvore esquerda se o nome for menor
    } else {
        return searchIndexBst(root->right, name); // Procura na subárvore direita se o nome for maior
    }
}