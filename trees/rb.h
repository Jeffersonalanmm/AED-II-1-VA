#ifndef RB_H
#define RB_H
#include <stdlib.h>

enum color { RED, BLACK, DOUBLE_BLACK };

typedef struct rbIndex {
	char *difficulty;
	int index; //posição em bytes do primeiro byte do regsitro no arquivo de dados
}rbIndex;

typedef struct rbNode {
	rbIndex *data;
	enum color color;
	struct rbNode *left, *right, *father;
}rbNode;

typedef rbNode* rb;

//funcoes principais
rbIndex* initializeIndexRb(int index, char *difficulty);
void initializeRb(rb *root);
void insertRb(rb *root, rbIndex *valor);
void removeRb(rb *root, rb* root_relativa, char *difficulty);
int heightRb(rb root);
void printRb(rb root);
void printElementRb(rb root);
int searchIndexRb(rb root, char* difficulty);

//funcoes auxiliares

enum color colorRb(rb element);
int isRootRb(rb element);
int isLeftChildRb(rb element);
int isRightChildRb(rb element);
rbIndex* maxElementRb(rb root);
rb isBrotherRb(rb element);
rb uncleRb(rb element);
int maxRb(int a, int b);

//ajustes
void adjustRb(rb *root, rb element);
void simpleRightRotationRb(rb *root, rb pivot);
void simpleLeftRotationRb(rb *root, rb pivot);
void readjustRb(rb *root, rb element);
void removeDoubleBlackRb(rb *root, rb element);
void preOrderRb(rb a);
#endif