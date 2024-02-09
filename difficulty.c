#include "difficulty.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Função para calcular a altura de um nó
int height(difficultyAVL node) {
    if (node == NULL)
        return 0;
    return node->height;
}

// Função para calcular o máximo de dois números inteiros
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Função para criar um novo nó AVL
difficultyAVL newNode(char recipe[80], char ingredients[200], char preparation[200], int servings, int difficulty) {
    difficultyAVL node = (difficultyAVL)malloc(sizeof(avlNode));
    strcpy(node->recipe, recipe);
    strcpy(node->ingredients, ingredients);
    strcpy(node->preparation, preparation);
    node->servings = servings;
    node->difficulty = difficulty;
    node->left = node->right = NULL;
    node->height = 1;  // Novo nó tem altura 1
    return node;
}

// Função para realizar a rotação simples à direita
difficultyAVL rightRotation(difficultyAVL y) {
    difficultyAVL x = y->left;
    difficultyAVL T2 = x->right;

    // Realiza a rotação
    x->right = y;
    y->left = T2;

    // Atualiza alturas
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

// Função para realizar a rotação simples à esquerda
difficultyAVL leftRotation(difficultyAVL x) {
    difficultyAVL y = x->right;
    difficultyAVL T2 = y->left;

    // Realiza a rotação
    y->left = x;
    x->right = T2;

    // Atualiza alturas
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

// Função para obter o fator de equilíbrio de um nó
int balanceFactor(difficultyAVL node) {
    if (node == NULL)
        return 0;
    return height(node->left) - height(node->right);
}

// Função para inserir um novo nó na árvore AVL
difficultyAVL addRecordAVL(difficultyAVL root, char recipe[80], char ingredients[200], char preparation[200], int servings, int difficulty) {
    if (root == NULL)
        return newNode(recipe, ingredients, preparation, servings, difficulty);

    // Executa a inserção como em uma BST
    int compareResult = strcmp(recipe, root->recipe);

    if (compareResult < 0)
        root->left = addRecordAVL(root->left, recipe, ingredients, preparation, servings, difficulty);
    else if (compareResult > 0)
        root->right = addRecordAVL(root->right, recipe, ingredients, preparation, servings, difficulty);
    else
        return root;  // Dados duplicados não são permitidos em AVL

    // Atualiza a altura do nó atual
    root->height = 1 + max(height(root->left), height(root->right));

    // Obtém o fator de equilíbrio para verificar se é necessário reequilibrar
    int balance = balanceFactor(root);

    // Casos de desequilíbrio:

    // Rotação à esquerda
    if (balance > 1 && compareResult < 0)
        return rightRotation(root);

    // Rotação à direita
    if (balance < -1 && compareResult > 0)
        return leftRotation(root);

    // Rotação dupla à esquerda-direita
    if (balance > 1 && compareResult > 0) {
        root->left = leftRotation(root->left);
        return rightRotation(root);
    }

    // Rotação dupla à direita-esquerda
    if (balance < -1 && compareResult < 0) {
        root->right = rightRotation(root->right);
        return leftRotation(root);
    }

    return root;
}

// Função para remover um nó da árvore AVL
difficultyAVL removeRecordAVL(difficultyAVL root, char recipe[80]) {
    if (root == NULL)
        return root;

    // Executa a remoção como em uma BST
    int compareResult = strcmp(recipe, root->recipe);

    if (compareResult < 0)
        root->left = removeRecordAVL(root->left, recipe);
    else if (compareResult > 0)
        root->right = removeRecordAVL(root->right, recipe);
    else {
        // Nó com apenas um filho ou nenhum filho
        if ((root->left == NULL) || (root->right == NULL)) {
            difficultyAVL temp = (root->left != NULL) ? root->left : root->right;

            // Caso sem filho
            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else  // Caso de um filho
                *root = *temp;  // Copia o conteúdo do filho não nulo

            free(temp);
        } else {
            // Nó com dois filhos, obtemos o sucessor in-order (menor nó à direita)
            difficultyAVL temp = findMin(root->right);

            // Copia o sucessor in-order para este nó
            strcpy(root->recipe, temp->recipe);
            strcpy(root->ingredients, temp->ingredients);
            strcpy(root->preparation, temp->preparation);
            root->servings = temp->servings;
            root->difficulty = temp->difficulty;

            // Remove o sucessor in-order
            root->right = removeRecordAVL(root->right, temp->recipe);
        }
    }

    // Se a árvore tinha apenas um nó, retorna
    if (root == NULL)
        return root;

    // Atualiza a altura do nó atual
    root->height = 1 + max(height(root->left), height(root->right));

    // Obtém o fator de equilíbrio para verificar se é necessário reequilibrar
    int balance = balanceFactor(root);

    // Casos de desequilíbrio:

    // Rotação à esquerda
    if (balance > 1 && balanceFactor(root->left) >= 0)
        return rightRotation(root);

    // Rotação à direita
    if (balance < -1 && balanceFactor(root->right) <= 0)
        return leftRotation(root);

    // Rotação dupla à esquerda-direita
    if (balance > 1 && balanceFactor(root->left) < 0) {
        root->left = leftRotation(root->left);
        return rightRotation(root);
    }

    // Rotação dupla à direita-esquerda
    if (balance < -1 && balanceFactor(root->right) > 0) {
        root->right = rightRotation(root->right);
        return leftRotation(root);
    }

    return root;
}

// Restante do código permanece inalterado...

// Função para buscar um registro na árvore AVL
void searchRecordAVL(difficultyAVL root, char recipe[80]) {
    if (root == NULL) {
        printf("Record with recipe '%s' not found.\n", recipe);
        return;
    }

    int compareResult = strcmp(recipe, root->recipe);

    if (compareResult == 0) {
        // Print the satellite data of the found record
        printf("Recipe found:\n");
        printf("Recipe: %s\n", root->recipe);
        printf("Ingredients: %s\n", root->ingredients);
        printf("Preparation: %s\n", root->preparation);
        printf("Servings: %d\n", root->servings);
        printf("Difficulty: %d\n", root->difficulty);
    } else if (compareResult < 0) {
        searchRecordAVL(root->left, recipe);
    } else {
        searchRecordAVL(root->right, recipe);
    }
}

// Função para imprimir registros em ordem (inorder) na árvore AVL
void printRecordAVL(difficultyAVL root) {
    if (root != NULL) {
        printRecordAVL(root->left);
        printf("Recipe: %s, Ingredients: %s, Preparation: %s, Servings: %d, Difficulty: %d\n", root->recipe, root->ingredients, root->preparation, root->servings, root->difficulty);
        // Add specific fields of your record here
        printRecordAVL(root->right);
    }
}

// Função para salvar dados da árvore AVL em um arquivo binário
void saveFile(char * name, difficultyAVL a) {
    FILE* file;
    file = fopen(name, "wb");
    if (file != NULL) {
        saveAux(a, file);
        fclose(file);
    }
}

difficultyAVL ler_dados() {
	difficultyAVL *new = (difficultyAVL) malloc(sizeof(avlNode));
	getchar() ;
	printf("Titulo: ");
	fgets(novo->titulo, 80,  stdin);
	tirar_enter(novo->titulo);
	printf("Autor: ");
	fgets(novo->autor, 50,  stdin);
	printf("Isbn: ");
	fgets(novo->isbn, 20,  stdin);
	printf("Codigo: ");
	scanf("%d", &novo->codigo);
	return novo;
}


// Função auxiliar para salvar dados da árvore AVL em um arquivo binário
void saveAux(difficultyAVL root, FILE* file) {
    if (root != NULL) {
        // Pre-order traversal
        fwrite(root, sizeof(avlNode), 1, file);
        saveAux(root->left, file);
        saveAux(root->right, file);
    }
}

// Função para carregar dados da árvore AVL a partir de um arquivo binário
difficultyAVL uploadFile(char* name, difficultyAVL a) {
    FILE* file;
    file = fopen(name, "rb");
    difficultyAVL temp;
    if (file != NULL) {
        temp = (difficultyAVL)malloc(sizeof(avlNode));
        while (fread(temp, sizeof(avlNode), 1, file)) {
            a = addRecordAVL(a, temp->recipe, temp->ingredients, temp->preparation, temp->servings, temp->difficulty);
            temp = (difficultyAVL)malloc(sizeof(avlNode));
        }
        fclose(file);
    }
    return a;
}

void clearBuffer() {
    while (getchar() != '\n');
}

void removeEnter(char str[50]) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

difficultyAVL findMin(difficultyAVL node) {
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}
