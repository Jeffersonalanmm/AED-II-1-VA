#ifndef AVL_H
#define AVL_H
#include <stdlib.h>

typedef struct avlIndex {
	int servings;
	int index; 
}avlIndex;
												//struct para índice e nó AVL
typedef struct avlNode {
	int fb;
    avlIndex *data;
	struct avlNode *left, *right;
}avlNode;

typedef avlNode* avl;

avlIndex* initializeIndexAvl(int index, int servings);
avl insertAvl(avl a, avlIndex *v, int *grow);
int searchIndexAvl(avl root, int servings);
avl removeElementAvl(avl a, int v, int *down);
avl rotationAvl(avl a);
avl simpleRightRotationAvl(avl a);
avl simpleLeftRotationAvl(avl a);						//Assinatura das funções AVL
avl doubleRightRotationAvl(avl a);
avl doubleLeftRotationAvl(avl a);
void inOrderAvl(avl root);
void printElementAvl(avl root);

#endif