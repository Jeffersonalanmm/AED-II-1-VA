#ifndef TABELA_H
#define TABELA_H
#include <stdio.h>
#include <stdlib.h>
#include "../trees/bst.h"
#include "../trees/bst.c"
#include "../trees/rb.h"
#include "../trees/rb.c"
#include "../trees/avl.h"
#include "../trees/avl.c"

#define MAX 1000 // Defina o máximo de entradas de dados que você espera

enum treeType {BST, AVL, RB};
typedef enum treeType treeType;

typedef struct recipeBook {
    int removido;
    char name[80];
    char ingredients[200];
    char preparation[200];
    int  servings;
    char difficulty[20];
} data;

/*typedef struct bstNode{
    bstIndex *data;
    struct bstNode *left, *right;
}bstNode;

typedef struct bstNode* bst;

typedef struct avlNode{
	int fb;
    avlIndex *dado;
	struct avlNode *left, *right;
}avlNode;

typedef struct avlNode* avl;

typedef struct rbNode{
	rbIndex *dado;
	enum cor cor;
	struct rbNode *left, *right, *father;
}rbNode;

typedef struct rbNode* rb;*/

typedef struct table{
    FILE *dataFile;
    bst indexBST;
    avl indexAVL;
    rb indexRB;
}table;

int initializeTable(table* table);

int insertTable(table* table, data* data);
data searchData(FILE* file, int key);
void removeData(table* table, int key);
void printData(data data);

void saveData(FILE* arq, data* data);
void printByRecipe(FILE* arq, bst root);
void printByServings(FILE* arq, avl root);
void printByDifficulty(FILE* arq, rb root);
void saveFileBST(char* nome, bst a);
void saveAuxFileBST(FILE* arq, bst root);
void saveFileAVL(char* nome, avl a);
void saveAuxFileAVL(FILE* arq, avl root);
void saveFileRB(char* nome, rb a);
void saveAuxFileRB(FILE* arq, rb root);

data* readData();
void uploadFile(table* table, treeType type);
void removeEnter(char *string);
void finish(table *table);
#endif