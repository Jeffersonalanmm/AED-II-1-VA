#ifndef SERVINGS_H 
#define SERVINGS_H
#include <stdio.h>

enum color
{
  RED,
  BLACK
};

enum direction
{
  LEFT = 1,
  RIGHT = -1
};

typedef struct rbNode
{
  char recipe[80];
  char ingredients[200];
  char preparation[300];
  int servings;
  int difficulty;
  enum color color;
  struct rbNode* parent;
  struct rbNode* left;
  struct rbNode* right;
} rbNode;

typedef rbNode* servingsRB;

servingsRB addRecipeRB(servingsRB root, char recipe[80], char ingredients[200], char preparation[300], int servings, int difficulty);
servingsRB removeRecipeRB(servingsRB root, char recipe[80]);
servingsRB searchRecipeRB(servingsRB root, char recipe[80]);
void printRecipeRB(servingsRB root);
void saveFile(char* name, servingsRB a);
servingsRB uploadFile(char* name, servingsRB a);
void clearBuffer();
void removeEnter(char str[50]);
int height(servingsRB root);

#endif
