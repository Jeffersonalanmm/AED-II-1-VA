#ifndef BST_H
#define BST_H
#include <stdlib.h>

typedef struct bstIndex {
	char* name;
	int index; //posição em bytes do primeiro byte do regsitro no arquivo de dados
}bstIndex;

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
bstIndex * minElementBst(bst root);
void preOrderBst(bst root);
void posOrderBst(bst root);
void inOrderBst(bst root);
bst removeBst(bst root, char* name);
void printElementBst(bst root);
int searchElementBst(bst root, char* name);

#endif