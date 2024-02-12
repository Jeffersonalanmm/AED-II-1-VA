#ifndef BST_H
#define BST_H
#include <stdlib.h>

typedef struct bstIndex {
	char* name;
	int index;
}bstIndex;
										//struct para o índice e o nó BST
typedef struct bstNode {
	bstIndex *data;
	struct bstNode *left, *right;
} bstNode;

typedef bstNode * bst;

bstIndex * initializeIndexBst(int index, char* name);
bst insertBst(bstIndex *value, bst root);
int height(bst root);
int max(int a, int b);
bstIndex * maxElementBst(bst root);		
bstIndex * minElementBst(bst root);						//Assinatura de cada função BST
void inOrderBst(bst root);
bst removeBst(bst root, char* name);
void printElementBst(bst root);
int searchIndexBst(bst root, char* name);

#endif