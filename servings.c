#include "servings.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

servingsRB createNode(char recipe[80], char ingredients[200], char preparation[300], int servings, int difficulty) {
    servingsRB newNode = (servingsRB)malloc(sizeof(rbNode));
    if (newNode != NULL) {
        strcpy(newNode->recipe, recipe);
        strcpy(newNode->ingredients, ingredients);
        strcpy(newNode->preparation, preparation);
        newNode->servings = servings;
        newNode->difficulty = difficulty;
        newNode->color = RED;
        newNode->parent = NULL;
        newNode->left = NULL;
        newNode->right = NULL;
    }
    return newNode;
}

void freeNode(servingsRB node) {
    if (node != NULL) {
        free(node);
    }
}

int height(servingsRB root) {
    if (root == NULL) {
        return 0;
    }

    int leftHeight = height(root->left);
    int rightHeight = height(root->right);

    return (leftHeight > rightHeight) ? leftHeight + 1 : rightHeight + 1;
}

void rotate(servingsRB target, enum direction type) {
    if (target != NULL) {
        servingsRB parent = target->parent;
        servingsRB child;
        servingsRB grandchild = NULL;
        if (type == LEFT) {
            child = target->right;
        } else {
            child = target->left;
        }
        if (child != NULL) {
            if (type == LEFT) {
                grandchild = child->left;
                target->right = grandchild;
                child->left = target;
            } else {
                grandchild = child->right;
                target->left = grandchild;
                child->right = target;
            }
            if (parent != NULL) {
                if (parent->left == target) {
                    parent->left = child;
                } else {
                    parent->right = child;
                }
            }
            if (grandchild != NULL) {
                grandchild->parent = target;
            }
            child->parent = parent;
            target->parent = child;
        }
    }
}

servingsRB addRecordRB(servingsRB root, char recipe[80], char ingredients[200], char preparation[300], int servings, int difficulty) {
    if (root == NULL) {
        return createNode(recipe, ingredients, preparation, servings, difficulty);
    }

    servingsRB current = root;
    servingsRB parent = NULL;

    while (current != NULL) {
        parent = current;
        if (strcmp(current->recipe, recipe) > 0) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    servingsRB target = createNode(recipe, ingredients, preparation, servings, difficulty);
    target->parent = parent;

    if (parent == NULL) {
        root = target;
    } else {
        if (strcmp(parent->recipe, recipe) > 0) {
            parent->left = target;
        } else {
            parent->right = target;
        }

        while (target->color == RED && parent != NULL && parent->color == RED) {
            servingsRB grandparent = parent->parent;

            if (grandparent == NULL) {
                parent->color = BLACK;
            } else {
                servingsRB uncle;

                if (grandparent->left == parent) {
                    uncle = grandparent->right;
                } else {
                    uncle = grandparent->left;
                }

                if (uncle == NULL || uncle->color == BLACK) {
                    enum direction type;

                    if (grandparent->left == parent) {
                        type = LEFT;
                    } else {
                        type = RIGHT;
                    }

                    if ((parent->left == target && type == RIGHT) || (parent->right == target && type == LEFT)) {
                        rotate(parent, type);
                        servingsRB auxiliar = target;
                        target = parent;
                        parent = auxiliar;
                    }

                    rotate(grandparent, -type);
                    grandparent->color = RED;
                    parent->color = BLACK;

                    if (parent->parent == NULL) {
                        root = parent;
                    }
                } else {
                    grandparent->color = RED;
                    parent->color = BLACK;
                    uncle->color = BLACK;
                    target = grandparent;
                    parent = target->parent;
                }
            }
        }
    }

    return root;
}

servingsRB removeRecordRB(servingsRB root, char recipe[80]) {
    servingsRB target = root;
    servingsRB current; // Adicionando a declaração de current

    while (target != NULL && strcmp(target->recipe, recipe) != 0) {
        if (strcmp(target->recipe, recipe) > 0) {
            target = target->left;
        } else {
            target = target->right;
        }
    }

    if (target != NULL) {
        if (target->left != NULL && target->right != NULL) {
            // Encontrar o predecessor
            servingsRB predecessor = target->left;
            while (predecessor->right != NULL) {
                predecessor = predecessor->right;
            }

            // Trocar os valores
            char tempRecipe[80];
            strcpy(tempRecipe, target->recipe);
            strcpy(target->recipe, predecessor->recipe);
            strcpy(predecessor->recipe, tempRecipe);

            // Recursivamente remover o predecessor
            root = removeRecordRB(root, recipe);
        } else if (target->left != NULL || target->right != NULL) {
            // Caso com apenas um filho
            servingsRB parent = target->parent;
            servingsRB child;

            if (target->left != NULL) {
                child = target->left;
            } else {
                child = target->right;
            }

            if (parent != NULL) {
                if (parent->left == target) {
                    parent->left = child;
                } else {
                    parent->right = child;
                }
            } else {
                root = child;
            }

            if (child != NULL) {
                child->parent = parent;
                child->color = BLACK;
            }

            freeNode(target);
        } else {
            // Caso sem filhos
            servingsRB parent = target->parent;

            if (parent == NULL) {
                root = NULL;
            } else {
                if (target->color == BLACK) {
                    // Caso de correção de cores
                    servingsRB sibling;
                    servingsRB close;
                    servingsRB distant;
                    enum direction type;

                    while (parent != NULL) {
                        if (parent->left == target) {
                            sibling = parent->right;
                            close = sibling->left;
                            distant = sibling->right;
                            type = LEFT;
                        } else {
                            sibling = parent->left;
                            close = sibling->right;
                            distant = sibling->left;
                            type = RIGHT;
                        }

                        if (parent->color == BLACK && sibling->color == BLACK && (close == NULL || close->color == BLACK) && (distant == NULL || distant->color == BLACK)) {
                            sibling->color = RED;
                            current = parent;
                            parent = current->parent;
                            continue;
                        }

                        if (parent->color == BLACK && sibling->color == RED) {
                            rotate(parent, type);
                            parent->color = RED;
                            sibling->color = BLACK;

                            if (sibling->parent == NULL) {
                                root = sibling;
                            }

                            sibling = close;

                            if (type == LEFT) {
                                close = sibling->left;
                                distant = sibling->right;
                            } else {
                                close = sibling->right;
                                distant = sibling->left;
                            }
                        }

                        if (parent->color == RED && sibling->color == BLACK && (close == NULL || close->color == BLACK) && (distant == NULL || distant->color == BLACK)) {
                            sibling->color = RED;
                            parent->color = BLACK;
                            break;
                        }

                         if (sibling->color == BLACK && (distant != NULL && distant->color == RED)) {
                            rotate(parent, type);
                            sibling->color = parent->color;
                            distant->color = BLACK;
                            parent->color = BLACK;

                            if (sibling->parent == NULL) {
                                root = sibling;
                            }

                            break;
                        }
                    }
                }

                // Remover o nó
                if (parent->left == target) {
                    parent->left = NULL;
                } else {
                    parent->right = NULL;
                }
            }

            freeNode(target);
        }
    }

    return root;
}

servingsRB searchRecordRB(servingsRB root, char recipe[80]) {
    if (root == NULL || strcmp(root->recipe, recipe) == 0) {
        return root;
    }

    if (strcmp(root->recipe, recipe) > 0) {
        return searchRecordRB(root->left, recipe);
    } else {
        return searchRecordRB(root->right, recipe);
    }
}

void printRecordRBRecursion(servingsRB root, int line, int lines, char ***stringsReference) {
    if (root != NULL) {
        int lineIndex;
        int spaceIndex;
        int length;
        char value[50];

        printRecordRBRecursion(root->left, line + 1, lines, stringsReference);

        if (root->servings == 0) {
            length = 1;
        } else {
            length = floor(log10(abs(root->servings))) + 1;
            if (root->servings < 0) {
                length = length + 1;
            }
        }

        sprintf(value, "%d", root->servings);
        strcat((*stringsReference)[line], value);

        for (lineIndex = 0; lineIndex < lines; lineIndex++) {
            if (lineIndex != line) {
                for (spaceIndex = 0; spaceIndex < length; spaceIndex++) {
                    strcat((*stringsReference)[lineIndex], " ");
                }
            }
        }

        printRecordRBRecursion(root->right, line + 1, lines, stringsReference);
    }
}

void printRecordRB(servingsRB root) {
    if (root != NULL) {
        int index;
        int lines = height(root);
        char **strings = (char **)malloc(lines * sizeof(char *));
        for (index = 0; index < lines; index++) {
            strings[index] = (char *)malloc(50 * sizeof(char));
            strings[index][0] = '\0';
        }

        char ***stringsReference = &strings;
        printRecordRBRecursion(root, 0, lines, stringsReference);

        for (index = 0; index < lines; index++) {
            printf("%s", strings[index]);
            if (index != lines - 1) {
                printf("\n");
            }
            free(strings[index]);
        }

        free(strings);
    }
}

// Funções restantes (printColors, saveFile, saveAux, uploadFile, removeEnter, clearBuffer) permanecem inalteradas


void printColorsRecursion(servingsRB root, int line, int lines, char ***stringsReference) {
    if (root != NULL) {
        int lineIndex;
        int spaceIndex;
        int length;
        char value[50];

        printColorsRecursion(root->left, line + 1, lines, stringsReference);

        for (lineIndex = 0; lineIndex < lines; lineIndex++) {
            if (lineIndex != line) {
                strcat((*stringsReference)[lineIndex], " ");
            } else {
                if (root->color == RED) {
                    strcat((*stringsReference)[lineIndex], "R");
                } else {
                    strcat((*stringsReference)[lineIndex], "B");
                }
            }
        }

        printColorsRecursion(root->right, line + 1, lines, stringsReference);
    }
}

void printColors(servingsRB root) {
    if (root != NULL) {
        int index;
        int lines = height(root);
        char **strings = (char **)malloc(lines * sizeof(char *));
        for (index = 0; index < lines; index++) {
            strings[index] = (char *)malloc(50 * sizeof(char));
            strings[index][0] = '\0';
        }

        char ***stringsReference = &strings;
        printColorsRecursion(root, 0, lines, stringsReference);

        for (index = 0; index < lines; index++) {
            printf("%s", strings[index]);
            if (index != lines - 1) {
                printf("\n");
            }
            free(strings[index]);
        }

        free(strings);
    }
}

void saveFile(char* name, servingsRB a) {
    FILE* file;
    file = fopen(name, "wb");
    if (file != NULL) {
        saveAux(a, file);
        fclose(file);
    }
}

// Função auxiliar para salvar dados da árvore AVL em um arquivo binário
void saveAux(servingsRB root, FILE* file) {
    if (root != NULL) {
        // Pre-order traversal
        fwrite(root, sizeof(rbNode), 1, file);
        saveAux(root->left, file);
        saveAux(root->right, file);
    }
}

// Função para carregar dados da árvore AVL a partir de um arquivo binário
servingsRB uploadFile(char* name, servingsRB a) {
    FILE* file;
    file = fopen(name, "rb");
    servingsRB temp;
    if (file != NULL) {
        temp = (servingsRB)malloc(sizeof(rbNode));
        while (fread(temp, sizeof(rbNode), 1, file)) {
            a = addRecordRB(a, temp->recipe, temp->ingredients, temp->preparation, temp->servings, temp->difficulty);
            temp = (servingsRB)malloc(sizeof(rbNode));
        }
        fclose(file);
    }
    return a;
}


void removeEnter(char str[50]) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}
void clearBuffer() {
    while (getchar() != '\n');
}
