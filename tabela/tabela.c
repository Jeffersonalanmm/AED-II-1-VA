#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include "tabela.h"
#include <string.h>

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
void removeEnter(char *string) {
	string[strlen(string) -1] = '\0';
}

int insertTable(table *table, data *data) {
    int positionNewRecord, retorno;
    retorno = table->dataFile != NULL;
    if (retorno) {
        fseek(table->dataFile, 0L, SEEK_END);
        // Diz onde está o cursor
        positionNewRecord = ftell(table->dataFile);
        
        // BST
        bstIndex *newIndexBst = initializeIndexBst(positionNewRecord, data->name);
        if (newIndexBst == NULL) {
            printf("Erro ao inicializar o índice na árvore binária de busca (BST).\n");
            return 0;
        }
        table->indexBST = insertBst(newIndexBst, table->indexBST);
        
        // AVL
        table->indexAVL = insertAvl(table->indexAVL, initializeIndexAvl(positionNewRecord, data->servings), NULL);
        
        // RB
        insertRb(&table->indexRB, initializeIndexRb(positionNewRecord, data->difficulty));
        
        // Salvar no arquivo de dados
        saveData(table->dataFile, data);
    }
    return retorno;
}


void saveData(FILE *arq, data *data) {
	fprintf(arq, "%d;%s;%s;%s;%d;%s;", data->removido,data->name, data->ingredients, data->preparation, data->servings, data->difficulty);
}       

void removeData(table* table, int key) {
	if(table != NULL){
		data data = searchData(table->dataFile, key);
		if(!data.removido){
			table->indexBST = removeBst(table->indexBST, data.name);
			table->indexAVL = removeElementAvl(table->indexAVL, data.servings, NULL);
			removeRb(&table->indexRB,&table->indexRB, data.difficulty);
			fseek(table->dataFile, key, SEEK_SET);
			char str[] = "1";
			fwrite(str, 1, sizeof(char), table->dataFile);
			fseek(table->dataFile, 0L, SEEK_END);
		}
		else
			printf("Erro ao remover\n");
            
	}
}

data* readData() {
	data* new = (data*) malloc(sizeof(data));
	if (new == NULL) {
		printf("Erro de alocação de memória!\n");
		return NULL;
	}

	printf("Recipe Title: ");
	scanf(" %[^\n]%*c", new->name);

	printf("Ingredients: ");
	scanf(" %[^\n]%*c", new->ingredients);

	printf("Preparation: ");
	scanf(" %[^\n]%*c", new->preparation);

	printf("servings: ");
	scanf("%d", &(new->servings));

	printf("Difficulty: ");
	scanf(" %[^\n]%*c", new->difficulty);

	new->removido = 0;
	return new;
}

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

data searchData(FILE *file, int key) {
    data temp;
    if (key >= 0 && file != NULL) { 
        fseek(file, key, SEEK_SET);
        
        // Lê uma linha do arquivo
        char buffer[256];
        if (fgets(buffer, sizeof(buffer), file) != NULL) {
            // Divide a linha em campos usando o delimitador ";"
            char *ptr = strtok(buffer, ";");
            if (ptr != NULL) {
                // Converte o primeiro campo (removido) para um inteiro
                temp.removido = atoi(ptr);

                // Move para o próximo campo
                ptr = strtok(NULL, ";");
                if (ptr != NULL) {
                    // Copia o segundo campo (name) para temp.name
                    strcpy(temp.name, ptr);

                    // Move para o próximo campo
                    ptr = strtok(NULL, ";");
                    if (ptr != NULL) {
                        // Copia o terceiro campo (ingredients) para temp.ingredients
                        strcpy(temp.ingredients, ptr);

                        // Move para o próximo campo
                        ptr = strtok(NULL, ";");
                        if (ptr != NULL) {
                            // Copia o quarto campo (preparation) para temp.preparation
                            strcpy(temp.preparation, ptr);

                            // Move para o próximo campo
                            ptr = strtok(NULL, ";");
                            if (ptr != NULL) {
                                // Converte o quinto campo (servings) para um inteiro
                                temp.servings = atoi(ptr);

                                // Move para o próximo campo
                                ptr = strtok(NULL, ";");
                                if (ptr != NULL) {
                                    // Copia o sexto campo (difficulty) para temp.difficulty
                                    strcpy(temp.difficulty, ptr);

                                    // Retorna a estrutura de dados preenchida
                                    return temp;
                                }
                            }
                        }
                    }
                }
            }
        } else {
            printf("Erro ao ler linha do arquivo.\n");
        }
    } else {
        printf("Arquivo inválido ou índice inválido!\n");
    }
    
    // Se algo deu errado, retorne uma estrutura de dados vazia ou com valores padrão
    data emptyData = {0}; // Inicializa todos os campos com 0
    return emptyData;
}


void printData(data data){
	printf("Recipe Title: %s\n", data.name);
	printf("Ingredients: %s\n", data.ingredients);
	printf("Preparation: %s\n", data.preparation);
	printf("Servings: %d\n", data.servings);
    printf("Difficulty: %s\n", data.difficulty);
	printf("\n");
}

void printByRecipe(FILE *file, bst root) {
	if(root != NULL) {
		printByRecipe(file, root->left);
		printf("\n-----------------------------------------------\n\n");
		printData(searchData(file, root->data->index));
			// imprimir_elemento(pok);
		printByRecipe(file, root->right);
	}
}

void printByServings(FILE *arq, avl root) {
	if(root != NULL) {
		printByServings(arq, root->left);
		printf("\n-----------------------------------------------\n\n");
		printData(searchData(arq, root->data->index));
		printByServings(arq, root->right);
	}
}

void printByDifficulty(FILE *arq, rb root) {
	if(root != NULL) {
		printByDifficulty(arq, root->left);
		printf("\n-----------------------------------------------\n\n");
		//dado pok = 
		printData(searchData(arq, root->data->index));
		//if(!pok.removido)
			
		printByDifficulty(arq, root->right);
	}
}


void saveFileBST(char* name, bst root) {
    FILE *file = fopen(name, "w+");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo %s para escrita.\n", name);
        return;
    }
    saveAuxFileBST(file, root);
    fclose(file);
}

void saveAuxFileBST(FILE* arq, bst root) {
    if (root != NULL) {
        fprintf(arq, "%d;%s\n", root->data->index, root->data->name);
        saveAuxFileBST(arq, root->left);
        saveAuxFileBST(arq, root->right);
    }
}

void saveFileAVL(char* name, avl a) {
	FILE *arq;
	arq = fopen(name, "w+");
	if(arq != NULL) {
		saveAuxFileAVL(arq, a);
		fclose(arq);
	}
}

void saveAuxFileAVL(FILE *arq, avl root){
	if(root != NULL) {
		fprintf(arq, "%d;%d\n", root->data->index, root->data->servings);
		saveAuxFileAVL(arq, root->left);
		saveAuxFileAVL(arq, root->right);
	}

}

void saveFileRB(char* name, rb a) {
	FILE *arq;
	arq = fopen(name, "w+");
	if(arq != NULL) {
		saveAuxFileRB(arq, a);
		fclose(arq);
	}
}

void saveAuxFileRB(FILE* arq, rb root){
	if(root != NULL) {
		fprintf(arq, "%d;%s\n", root->data->index, root->data->difficulty);
		saveAuxFileRB(arq, root->left);
		saveAuxFileRB(arq, root->right);
	}

}

void finalizar (table* table) {
	fclose(table->dataFile);
	saveFileBST("indexbst.dat", table->indexBST);
	saveFileAVL("indexavl.dat", table->indexAVL);
	saveFileRB("indexrb.dat", table->indexRB);
}