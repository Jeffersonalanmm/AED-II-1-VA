#include <stdio.h>
#include <stdlib.h>
#include "tabela.h"
#include <string.h>

int initializeTable(table *table) {
	table->dataFile = fopen("dados.dat", "r+b");
	table->indexBST = NULL;
	//table->indexAVL = NULL;
	//inicializar_rb(&table->indexRB);
	uploadFile(table, BST);
	//uploadFile(table, AVL);
	//uploadFile(table, RB);
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
	if(retorno) {
		fseek(table->dataFile, 0L, SEEK_END);
		//diz onde está o cursor
		positionNewRecord = ftell(table->dataFile);
		
		//bst
		table->indexBST = (inicializarIndiceBst(positionNewRecord, data->recipe), table->indexBST);

		//avl
		//table->indexAVL = adicionar_avl(table->indexAVL, inicializar_indice_avl(positionNewRecord, pok->codigo), NULL);
		
		// //rb
		//inserir_rb(&table->indexRB, inicializar_indice_rb(positionNewRecord, pok->descricao));
		
		//salvar no arquivo de dados
		salvar_pokemon(table->dataFile, data);
	}

	return retorno;
}

/*void saveData(FILE *arq, data *data) {
	fprintf(arq, "%d;", data->recipe);
	for(int i = 0; i<2; i++){
		fprintf(arq, "%d,", (int) pok->tipo[i]);
	}
	fprintf(arq, ";%f;%s;%d\n", pok->altura, pok->descricao, pok->codigo);
}       


                        VER DEPOIS!!!!!!!!!!!!*/

/*void removeData(table* table, int key) {
	if(table != NULL){
		data data = searchData(table->dataFile, key);
		if(!pok.removido){
			table->indexBST = remover_bst(table->indexBST, pok.nome);
			table->indexAVL = remover_elemento_avl(table->indexAVL, pok.codigo, NULL);
			remover_rb(&table->indexRB,&table->indexRB, pok.descricao);
			fseek(table->dataFile, indice, SEEK_SET);
			char str[] = "1";
			fwrite(str, 1, sizeof(char), table->dataFile);
			fseek(table->dataFile, 0L, SEEK_END);
		}
		else
			printf("Erro ao remover\n");
            
            VER DEPOIS!!!!
	}
}*/

data* readData() {
	int type, option;
	data* new = (data*) malloc(sizeof(data));
	char * buffer = (char *) malloc(256 * sizeof(char));

	printf("Recipe Title: ");
	scanf(" %[^\n]%*c", new->recipe);
    
    printf("Ingredients: ");
	scanf(" %[^\n]%*c", new->ingredients);
    fgets(buffer, 256, stdin);
    strcpy(new->ingredients, buffer);

	// TIRAR SE DER CERTO COM STRCPY new->ingredients = strdup(buffer);
	tirar_enter(new->ingredients);
    
    printf("Preparation: ");
	scanf(" %[^\n]%*c", new->preparation);
    fgets(buffer, 256, stdin);
    strcpy(new->preparation, buffer);

	// TIRAR SE DER CERTO COM STRCPYnew->preparation = strdup(buffer);
	tirar_enter(new->preparation);
    
    printf("servings: ");
	scanf(" %[^\n]%d", new->servings);
    printf("Difficulty: ");
	scanf(" %[^\n]%d", new->difficulty);
}

void uploadFile(table* table, treeType tipo) {
	FILE *file;
	size_t len;
	char name[16], *line = (char*) malloc(len), delim[] = ";";
	switch (tipo) {
	case BST:
		strcpy(name, "indexbst.dat");
		break;
	/*case AVL:
		strcpy(name, "indexavl.dat");
		break;
	case RB:
		strcpy(name, "indexrb.dat");
		break;*/
	}
	file = fopen(name, "r+");
  if(file != NULL){
		while(getdelim(&line, &len, '\n', file) > 0){
  			char *ptr;
			ptr = strtok(line, delim);
			int index = atoi(ptr);
			ptr = strtok(NULL, delim);
			tirar_enter(ptr);
			switch (tipo) {
				case BST:
					table->indexBST = adicionarBst(inicializarIndiceBst(index, ptr), table->indexBST);
					break;
				/*case AVL:
					table->indexAVL = adicionar_avl(table->indexAVL, inicializar_indice_avl(indice, atoi(ptr)), NULL);
					break;
				case RB:
					inserir_rb(&table->indexRB, inicializar_indice_rb(indice, ptr));
					break;*/
			}
		}
		fclose(file);
 	}
 	free(line);
}

data searchData(FILE *file, int key){
    data temp;
	if(key >= 0 ){ 
		if(file != NULL){
            data temp;
			long len = sizeof(char) * 256;
			char *buffer = (char *) malloc(len);
			char delim[] = ";";

			fseek(file, key, SEEK_SET);

			getline(&buffer, &len, file);
			
			char *ptr = strtok(buffer, delim);
			//temp.removido = atoi(ptr);

			ptr = strtok(NULL, delim);
			strcpy(temp.recipe, ptr);
			
			ptr = strtok(NULL, ",");
			ptr = strtok(NULL, delim);
			//temp.ingredients = (char*) malloc(sizeof(ptr));
			//strcpy(temp.ingredients, ptr);

			ptr = strtok(NULL, delim);
			temp.servings = atoi(ptr);

            ptr = strtok(NULL, delim);
			//temp.difficulty = atoi(ptr);
			return temp;
		}
		printf("Invalid File!\n");
	} else 
		printf("Invalid Index!\n");
	//temp.removido = 1;
	return temp;
} 

void printData(data data){
	printf("Recipe Title: %d\n", data.recipe);
	printf("Ingredients: %s\n", data.ingredients);
	printf("Preparation: %f\n", data.preparation);
	printf("Servings: %s", data.servings);
    Printf("Difficulty: %d", data.difficulty);
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

/*void listar_por_codigo(FILE *arq, avl root) {
	if(root != NULL) {
		listar_por_codigo(arq, root->esq);
		printf("\n-----------------------------------------------\n\n");
		imprimir_elemento(buscar_pokemon(arq, root->dado->indice));
		//if(!pok.removido)
			// imprimir_elemento(pok);
		listar_por_codigo(arq, root->dir);
	}
}

void listar_por_descricao(FILE *arq, rb root) {
	if(root != NULL) {
		listar_por_descricao(arq, root->esq);
		printf("\n-----------------------------------------------\n\n");
		//dado pok = 
		imprimir_elemento(buscar_pokemon(arq, root->dado->indice));
		//if(!pok.removido)
			
		listar_por_descricao(arq, root->dir);
	}
}

                VER DEPOIS!!!!*/ 

void saveFileBST(char *nome, bst a) {
	FILE *file;
	file = fopen(nome, "w+");
	if(file != NULL) {
		saveAuxFileBST(file, a);
		fclose(file);
	}
}

void saveAuxFileBST(FILE* arq, bst root){
	if(root != NULL) {
		fprintf(arq, "%d;%s\n", root->data->index, root->data->recipe);
		salvar_auxiliarBst(root->left, arq);
		salvar_auxiliarBst(root->right, arq);
	}
}

/*void salvar_arquivo_avl(char *nome, avl root) {
	FILE *arq;
	arq = fopen(nome, "w+");
	if(arq != NULL) {
		salvar_auxiliar_avl(root, arq);
		fclose(arq);
	}
}

void salvar_auxiliar_avl(avl root, FILE *arq){
	if(root != NULL) {
		fprintf(arq, "%d;%d\n", root->dado->indice, root->dado->codigo);
		salvar_auxiliar_avl(root->esq, arq);
		salvar_auxiliar_avl(root->dir, arq);
	}

}

void salvar_arquivo_rb(char *nome, rb root) {
	FILE *arq;
	arq = fopen(nome, "w+");
	if(arq != NULL) {
		salvar_auxiliar_rb(root, arq);
		fclose(arq);
	}
}

void salvar_auxiliar_rb(rb root, FILE *arq){
	if(root != NULL) {
		fprintf(arq, "%d;%s\n", root->dado->indice, root->dado->descricao);
		salvar_auxiliar_rb(root->esq, arq);
		salvar_auxiliar_rb(root->dir, arq);
	}

}*/

void finalizar (table* table) {
	fclose(table->dataFile);
	salvar_arquivoBst("indicesbst.dat", table->indexBST);
//salvar_arquivo_avl("indicesavl.dat", table->indexAVL);
//	salvar_arquivo_rb("indicesrb.dat", table->indexRB);
}