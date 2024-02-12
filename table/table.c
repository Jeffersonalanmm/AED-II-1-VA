#include <stdio.h>
#include <stdlib.h>
#include "table.h"
#include <string.h>

// Inicializa a tabela de receitas, abrindo o arquivo de dados e os índices BST, AVL e RB.
int initializeTable(table *table) {
    table->dataFile = fopen("dados.dat", "r+b");
    table->indexBST = NULL;
    table->indexAVL = NULL;
    initializeRb(&table->indexRB);
    uploadFile(table, BST);
    uploadFile(table, AVL);
    uploadFile(table, RB);
    if(table->dataFile != NULL)
        return 1;
    else
        return 0;
}

// Remove o caractere de nova linha ('\n') de uma string.
void removeEnter(char *string) {
    string[strlen(string) -1] = '\0';
}

// Insere uma nova receita na tabela, atualizando os índices e salvando os dados no arquivo.
int insertTable(table *table, data *data) {
    int positionNewRecord, retorno;
    retorno = table->dataFile != NULL;
    if (retorno) {
        fseek(table->dataFile, 0L, SEEK_END);
        positionNewRecord = ftell(table->dataFile);
        
        bstIndex *newIndexBst = initializeIndexBst(positionNewRecord, data->name);
        if (newIndexBst == NULL) {
            printf("Error initializing index in binary search tree (BST).\n");
            return 0;
        }
        table->indexBST = insertBst(newIndexBst, table->indexBST);
        
        table->indexAVL = insertAvl(table->indexAVL, initializeIndexAvl(positionNewRecord, data->servings), NULL);
        
        insertRb(&table->indexRB, initializeIndexRb(positionNewRecord, data->difficulty));
        
        saveData(table->dataFile, data);
    }
    return retorno;
}

// Salva os dados de uma receita no arquivo de dados.
void saveData(FILE *arq, data *data) {
    fprintf(arq, "%d;%s;%s;%s;%d;%s;", data->removed,data->name, data->ingredients, data->preparation, data->servings, data->difficulty);
}

// Remove uma receita da tabela, atualizando os índices e marcando como removida no arquivo.
void removeData(table* table, int key) {
    if(table != NULL){
        data data = searchData(table->dataFile, key);
        if(!data.removed){
            table->indexBST = removeBst(table->indexBST, data.name);
            table->indexAVL = removeElementAvl(table->indexAVL, data.servings, NULL);
            removeRb(&table->indexRB,&table->indexRB, data.difficulty);
            fseek(table->dataFile, key, SEEK_SET);
            char str[] = "1";
            fwrite(str, 1, sizeof(char), table->dataFile);
            fseek(table->dataFile, 0L, SEEK_END);
        }
        else
            printf("Error removing\n");
    }
}

// Lê os dados de uma nova receita fornecidos pelo usuário.
data* readData() {
    data* new = (data*) malloc(sizeof(data));
    if (new == NULL) {
        printf("Memory allocation error!\n");
        return NULL;
    }

    printf("Recipe Title: ");
    scanf(" %[^\n]%*c", new->name);

    printf("Ingredients: ");
    scanf(" %[^\n]%*c", new->ingredients);

    printf("Preparation: ");
    scanf(" %[^\n]%*c", new->preparation);

    printf("Servings: ");
    scanf("%d", &(new->servings));

    printf("Difficulty [facil, medio ou dificil]: ");
    scanf(" %[^\n]%*c", new->difficulty);

    new->removed = 0;
    return new;
}

// Carrega os índices de uma árvore (BST, AVL ou RB) de um arquivo.
void uploadFile(table* table, treeType tipo) {
    FILE *file;
    char name[16], line[256];
    char delim[] = ";";
    switch (tipo) {
    case BST:
        strcpy(name, "indexbst.dat");
        break;
    case AVL:
        strcpy(name, "indexavl.dat");
        break;
    case RB:
        strcpy(name, "indexrb.dat");
        break;
    }
    file = fopen(name, "r+");
    if (file != NULL) {
        while (fgets(line, sizeof(line), file) != NULL) {
            char *ptr;
            ptr = strtok(line, delim);
            int index = atoi(ptr);
            ptr = strtok(NULL, delim);
            removeEnter(ptr);
            switch (tipo) {
            case BST:
                table->indexBST = insertBst(initializeIndexBst(index, ptr), table->indexBST);
                break;
            case AVL:
                table->indexAVL = insertAvl(table->indexAVL, initializeIndexAvl(index, atoi(ptr)), NULL);
                break;
            case RB:
                insertRb(&table->indexRB, initializeIndexRb(index, ptr));
                break;
            }
        }
        fclose(file);
    }
}

// Busca os dados de uma receita no arquivo pelo índice.
data searchData(FILE *file, int key) {
    data temp;
    if (key >= 0 && file != NULL) { 
        fseek(file, key, SEEK_SET);
        
        char buffer[256];
        if (fgets(buffer, sizeof(buffer), file) != NULL) {
            char *ptr = strtok(buffer, ";");
            if (ptr != NULL) {
                temp.removed = atoi(ptr);

                ptr = strtok(NULL, ";");
                if (ptr != NULL) {
                    strcpy(temp.name, ptr);

                    ptr = strtok(NULL, ";");
                    if (ptr != NULL) {
                        strcpy(temp.ingredients, ptr);

                        ptr = strtok(NULL, ";");
                        if (ptr != NULL) {
                            strcpy(temp.preparation, ptr);

                            ptr = strtok(NULL, ";");
                            if (ptr != NULL) {
                                temp.servings = atoi(ptr);

                                ptr = strtok(NULL, ";");
                                if (ptr != NULL) {
                                    strcpy(temp.difficulty, ptr);
                                    return temp;
                                }
                            }
                        }
                    }
                }
            }
        } else {
            printf("Error reading line from file.\n");
        }
    } else {
        printf("Invalid file or index!\n");
    }
    data emptyData = {0};
    return emptyData;
}

// Imprime os dados de uma receita.
void printData(data data){
    printf("Recipe Title: %s\n", data.name);
    printf("Ingredients: %s\n", data.ingredients);
    printf("Preparation: %s\n", data.preparation);
    printf("Servings: %d\n", data.servings);
    printf("Difficulty: %s\n", data.difficulty);
    printf("\n");
}

// Imprime as receitas em ordem alfabética pelo título.
void printByRecipe(FILE *file, bst root) {
    if(root != NULL) {
        printByRecipe(file, root->left);
        printf("\n-----------------------------------------------\n\n");
        printData(searchData(file, root->data->index));
        printByRecipe(file, root->right);
    }
}

// Imprime as receitas em ordem crescente de porções.
void printByServings(FILE *arq, avl root) {
    if(root != NULL) {
        printByServings(arq, root->left);
        printf("\n-----------------------------------------------\n\n");
        printData(searchData(arq, root->data->index));
        printByServings(arq, root->right);
    }
}

// Imprime as receitas em ordem crescente de dificuldade.
void printByDifficulty(FILE *arq, rb root) {
    if(root != NULL) {
        printByDifficulty(arq, root->left);
        printf("\n-----------------------------------------------\n\n");
        printData(searchData(arq, root->data->index));
        printByDifficulty(arq, root->right);
    }
}

// Salva os índices de uma árvore BST em um arquivo.
void saveFileBST(char* name, bst root) {
    FILE *file = fopen(name, "w+");
    if (file == NULL) {
        printf("Error opening file %s for writing.\n", name);
        return;
    }
    saveAuxFileBST(file, root);
    fclose(file);
}

// Auxiliar para salvar os índices de uma árvore BST em um arquivo.
void saveAuxFileBST(FILE* arq, bst root) {
    if (root != NULL) {
        fprintf(arq, "%d;%s\n", root->data->index, root->data->name);
        saveAuxFileBST(arq, root->left);
        saveAuxFileBST(arq, root->right);
    }
}

// Salva os índices de uma árvore AVL em um arquivo.
void saveFileAVL(char* name, avl a) {
    FILE *arq;
    arq = fopen(name, "w+");
    if(arq != NULL) {
        saveAuxFileAVL(arq, a);
        fclose(arq);
    }
}

// Auxiliar para salvar os índices de uma árvore AVL em um arquivo.
void saveAuxFileAVL(FILE *arq, avl root){
    if(root != NULL) {
        fprintf(arq, "%d;%d\n", root->data->index, root->data->servings);
        saveAuxFileAVL(arq, root->left);
        saveAuxFileAVL(arq, root->right);
    }
}

// Salva os índices de uma árvore RB em um arquivo.
void saveFileRB(char* name, rb a) {
    FILE *arq;
    arq = fopen(name, "w+");
    if(arq != NULL) {
        saveAuxFileRB(arq, a);
        fclose(arq);
    }
}

// Auxiliar para salvar os índices de uma árvore RB em um arquivo.
void saveAuxFileRB(FILE* arq, rb root){
    if(root != NULL) {
        fprintf(arq, "%d;%s\n", root->data->index, root->data->difficulty);
        saveAuxFileRB(arq, root->left);
        saveAuxFileRB(arq, root->right);
    }
}

// Finaliza o programa, fechando os arquivos e salvando os índices.
void finish (table* table) {
    fclose(table->dataFile);
    saveFileBST("indexbst.dat", table->indexBST);
    saveFileAVL("indexavl.dat", table->indexAVL);
    saveFileRB("indexrb.dat", table->indexRB);
}
