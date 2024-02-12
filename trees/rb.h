#ifndef RB_H
#define RB_H
#include <stdlib.h>

enum color { RED, BLACK, DOUBLE_BLACK };		//enumeração para cada cor da árvore RB

typedef struct rbIndex {
	char *difficulty;
	int index;
}rbIndex;
												//struct para p índice e o nó
typedef struct rbNode {
	rbIndex *data;
	enum color color;
	struct rbNode *left, *right, *father;
}rbNode;

typedef rbNode* rb;

rbIndex* initializeIndexRb(int index, char *difficulty);
void initializeRb(rb *root);
void insertRb(rb *root, rbIndex *valor);
void removeRb(rb *root, rb* root_relativa, char *difficulty);
int heightRb(rb root);
void inOrderRb(rb root);
void printElementRb(rb root);
int searchIndexRb(rb root, char* difficulty);
enum color colorRb(rb element);
int isRootRb(rb element);											//Assinatura das funções
int isLeftChildRb(rb element);
int isRightChildRb(rb element);
rbIndex* maxElementRb(rb root);
rb isBrotherRb(rb element);
rb uncleRb(rb element);
int maxRb(int a, int b);
void adjustRb(rb *root, rb element);
void simpleRightRotationRb(rb *root, rb pivot);
void simpleLeftRotationRb(rb *root, rb pivot);
void readjustRb(rb *root, rb element);
void removeDoubleBlackRb(rb *root, rb element);
#endif