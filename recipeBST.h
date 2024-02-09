#ifndef RECIPEBST_H 
#define RECIPEBST_H
#include <stdio.h>

typedef struct bstNode {
    char recipe[80];
    struct bstNode* left, * right;
} bstNode;

typedef struct bstNode* recipeBST;


recipeBST addRecordBST(recipeBST root, char recipe[80], char ingredients[200], char preparation[200], int servings, char difficulty[20]);
recipeBST removeRecordBST(recipeBST root, char recipe[80]);
void searchRecordBST(recipeBST root, char recipe[80]);
void printRecordBST(recipeBST root);
void saveFile(char* name, recipeBST a);
void saveAux(recipeBST root, FILE* file);
recipeBST uploadFile(char* name, recipeBST a);
void removeEnter(char *string);
recipeBST adicionar(recipeBST new_node, recipeBST tree);
recipeBST minValueNode(recipeBST node);

#endif
