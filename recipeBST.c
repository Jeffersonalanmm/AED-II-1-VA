#include "recipeBST.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

recipeBST addRecordBST(recipeBST root, char recipe[80], char ingredients[200], char preparation[200], int servings, char difficulty[20]) {
    if (root == NULL) {
        recipeBST node = (recipeBST)malloc(sizeof(struct bstNode));
        strncpy(node->recipe, recipe, sizeof(node->recipe) - 1);
        strncpy(node->ingredients, ingredients, sizeof(node->ingredients) - 1);
        strncpy(node->preparation, preparation, sizeof(node->preparation) - 1);
        node->servings = servings;  // Armazena o valor de servings diretamente
        strncpy(node->difficulty, difficulty, sizeof(node->difficulty) - 1);
        node->left = NULL;
        node->right = NULL;
        return node;
    }
    int cmp = strcmp(recipe, root->recipe);
    if (cmp > 0) {
        root->right = addRecordBST(root->right, recipe, ingredients, preparation, servings, difficulty);
    } else if (cmp < 0) {
        root->left = addRecordBST(root->left, recipe, ingredients, preparation, servings, difficulty);
    }
    return root;
}

void printRecordBST(recipeBST root) {
    if (root != NULL) {
        printRecordBST(root->left);
        printf("recipe: %s\nIngredients: %s\nPreparation: %s\nservings: %d\nDifficulty: %s\n\n", root->recipe, root->ingredients, root->preparation, root->servings, root->difficulty);
        printRecordBST(root->right);
    }
}

recipeBST removeRecordBST(recipeBST root, char recipe[80]) {
    if (root == NULL) {
        printf("recipe with name %s not found.\n", recipe);
        return root;
    }
    int cmp = strcmp(recipe, root->recipe);
    if (cmp < 0) {
        root->left = removeRecordBST(root->left, recipe);
    } else if (cmp > 0) {
        root->right = removeRecordBST(root->right, recipe);
    } else {
        if (root->left == NULL) {
            recipeBST temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            recipeBST temp = root->left;
            free(root);
            return temp;
        }
        recipeBST temp = minValueNode(root->right);
        strncpy(root->recipe, temp->recipe, sizeof(root->recipe) - 1);
        strncpy(root->ingredients, temp->ingredients, sizeof(root->ingredients) - 1);
        strncpy(root->preparation, temp->preparation, sizeof(root->preparation) - 1);
        root->servings = temp->servings;  // Atribui diretamente o valor de servings
        strncpy(root->difficulty, temp->difficulty, sizeof(root->difficulty) - 1);
        root->right = removeRecordBST(root->right, temp->recipe);
    }
    return root;
}

recipeBST minValueNode(recipeBST node) {
    while (node != NULL && node->left != NULL) {
        node = node->left;
    }
    return node;
}

void removeEnter(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

void clearBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}


void searchRecordBST(recipeBST root, char recipe[80]) {
    if (root == NULL) {
        printf("recipe with name %s not found.\n", recipe);
        return;
    }

    int cmp = strcmp(recipe, root->recipe);
    if (cmp == 0) {
        printf("recipe found:\n");
        printf("recipe: %s\nIngredients: %s\nPreparation: %s\nservings: %s\nDifficulty: %s\n", root->recipe, root->ingredients, root->preparation, root->servings, root->difficulty);
    } else if (cmp < 0) {
        searchRecordBST(root->left, recipe);
    } else {
        searchRecordBST(root->right, recipe);
    }
}

void saveFile(char *nome, recipeBST a) {
	FILE *arq;
	arq = fopen(nome, "wb");
	if(arq != NULL) {
		saveAux(a, arq);
		fclose(arq);
	}
}

void saveAux(recipeBST root, FILE *arq){
	if(root != NULL) {
		// Save the data of the current node
		fwrite(root, sizeof(bstNode), 1, arq);

		// Recursively save the left and right subtrees
		saveAux(root->left, arq);
		saveAux(root->right, arq);
	}
}

recipeBST uploadFile(char* name, recipeBST a) {
    FILE* file;
    file = fopen(name, "rb");
    recipeBST temp;
    if (file != NULL) {
        temp = (recipeBST)malloc(sizeof(bstNode));
        while (fread(temp, sizeof(bstNode), 1, file)) {
            a = addRecordBST(a, temp->recipe, temp->ingredients, temp->preparation, temp->servings, temp->difficulty);
            temp = (recipeBST)malloc(sizeof(bstNode));
        }
        fclose(file);
    }
    return a;
}
