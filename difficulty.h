#ifndef DIFFICULTY_H 
#define DIFFICULTY_H
#include <stdio.h>

typedef struct avlNode {
    int difficulty; // 1 for easy, 2 for medium, 3 for hard
    struct avlNode* left, *right;
} avlNode;

typedef struct avlNode* difficultyAVL;

difficultyAVL addRecipeAVL(difficultyAVL root, char recipe[80], char ingredients[200], char preparation[200], int servings, int difficulty);
difficultyAVL removeRecipeAVL(difficultyAVL root, char recipe[80]); 
void searchRecipeAVL(difficultyAVL root, difficulty); 
void printRecipeAVL(difficultyAVL root); 
void saveFile(char * name, difficultyAVL a);
void saveAux(difficultyAVL root, FILE* file);
difficultyAVL uploadFile(char* name, difficultyAVL a);
difficultyAVL insert(difficultyAVL newRecipe, difficultyAVL tree);
difficultyAVL findMin(difficultyAVL root);
void clearBuffer();
void removeEnter(char str[200]);

#endif
