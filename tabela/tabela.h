#ifndef TABELA.H
#define TABELA.H
#include <stdio.h>
#include <stdlib.h>
#include "../trees/bst.h"
#include "../trees/bst.c"
#include "../trees/rb.h"
#include "../trees/rb.c"
#include "../trees/avl.h"
#include "../trees/avl.c"

enum treeType {BST, AVL, RB};
typedef enum treeType treeType;

typedef struct recipeBook {
    char recipe[80];
    char ingredients[200];
    char preparation[200];
    int  servings;
    char difficulty[20];
} data;

typedef struct bstNode{
    indexBST *data;
    struct bstNode *left, *right;
}bstNode;

typedef struct bstNode* bst;

/*typedef struct avlNode{

}avlNode;

typedef struct avlNode* avl;

typedef struct rbNode{

}rbNode;

typedef struct rbNode* rb;*/
typedef struct table{
    FILE *dataFile;
    bst indexBST;
    /*avl indexAVL;
    rb indexRB;*/
}table;

int initializeTable(table* table);

int insertTable(table* table, data* data);
data searchData(FILE* file, int key);
void removeData(table* table, int key);
void printData(data* data);

void saveData(FILE* arq, data* data);
void printByRecipe(FILE* arq, bst root);
/*void printByServings(FILE* arq, avl raiz);
void printByDifficulty(FILE* arq, rb raiz);*/
void saveFileBST(FILE* arq, bst a);
void saveAuxFileBST(FILE* arq, bst root);
/*void saveFileAVL(FILE* arq, avl a);
void saveAuxFileAVL(FILE* arq, avl raiz);
void saveFileRB(FILE* arq, rb a);
void saveAuxFileRB(FILE* arq, rb raiz);*/

data* readData();
void uploadFile(table* table, treeType type);
void removeEnter(char *string);
void finish(table *table);
#endif