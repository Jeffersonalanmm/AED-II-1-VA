#include <stdio.h>
#include <stdlib.h>
#include "./table/table.h"
#include "./table/table.c"
#include "./trees/bst.h"
#include "./trees/avl.h"
#include "./trees/rb.h"

int option; // Variável global para armazenar a opção do menu

// Protótipos das funções do menu
int menu();
int menuSearch();
int menuList();
int menuRemove();
int back();
void cleanScreen();

int main(int argc, char* argv[]) {

	// Declaração das variáveis locais
	table table;
	int index, servings;
	data recipe;
	char name[50], *difficulty =  (char*) malloc(sizeof(char)*256);

	// Inicialização da tabela de receitas
	if(initializeTable(&table)) {
		do {
			switch(menu()) {
				int value;
				case 1:
					// Inserção de uma nova receita na tabela
					printf("Enter recipe details\n\n");
					if(insertTable(&table, readData())) 
						printf("Exit to insert!\n");
					else 
						printf("Recipe isn't inserted!\n");
					break;
				case 2:
					// Menu de busca de receitas
					while (1) {
						switch (menuSearch()) {
							case 1:
								// Busca por nome da receita
								printf("Search recipe by name\n\n");
								printf("Name: ");
								fgets(name, sizeof(name), stdin);
								removeEnter(name);
								cleanScreen();
								index = searchIndexBst(table.indexBST, name);
								if(index >= 0) {
									recipe = searchData(table.dataFile, index);
									printf("Search recipe by name\n\n");
									if(!recipe.removed)
										printData(recipe);
									else
										printf("Error when searching\n");
								} else
									printf("Recipe not found\n");
								break;
							case 2:
								// Busca por número de porções
								printf("Search recipe by servings\n\n");
								printf("Servings: ");
								scanf("%d", &servings);
								while (getchar() != '\n');
								cleanScreen();
								index = searchIndexAvl(table.indexAVL, servings);
								if(index >= 0) {
									recipe = searchData(table.dataFile, index);
									printf("Search recipe by servings\n\n");
									if(!recipe.removed)
										printData(recipe);
									else
										printf("Error when searching");
								} else
									printf("Recipe not found.\n");
									break;
							case 3:
								// Busca por dificuldade
								printf("Search recipe by difficulty\n\n");
								printf("Difficulty [facil, medio ou dificil]: ");
								fgets(difficulty, 255, stdin);
								removeEnter(difficulty);
								cleanScreen();
								index = searchIndexRb(table.indexRB, difficulty);
								if(index >= 0) {
									recipe = searchData(table.dataFile, index);
									printf("Search recipe by difficulty\n\n");
									if(!recipe.removed)
										printData(recipe);
									else
										printf("Error when searching");
								} else
									printf("Recipe not found.\n");
								break;
							case 4:
								goto endSearch;
								break;
							default:		
								printf("Invalid option!\n");
								break;
						}
						back();
					}
					endSearch:;
					break;
				case 3 :
					// Menu de listagem de receitas
					while (1) {
						switch (menuList())	{
							case 1:
								// Listagem por nome
								printf("Listing recipe by name \n\n");
								printByRecipe(table.dataFile, table.indexBST);
								break;
							case 2:
								// Listagem por porções
								printf("Listing recipe by servings \n\n");
								printByServings(table.dataFile, table.indexAVL);
								break;
							case 3:
								// Listagem por dificuldade
								printf("Listing recipe by difficulty\n\n");
								printByDifficulty(table.dataFile, table.indexRB);
								break;
							case 4:
								goto endListing;
								break;
							default:
								break;
						}
						back();
					}
					endListing:;
					break;
				case 4:
					// Menu de remoção de receitas
					while (1) {
						switch (menuRemove()) {
							case 1:
								// Remoção por nome
								printf("Remove recipe by name: ");
								fgets(name, sizeof(name), stdin);
								removeEnter(name);
								index = searchIndexBst(table.indexBST, name);
								if(index >= 0) {
									removeData(&table, index);
								} else
									printf("Recipe not found.\n");
								break;
							case 2:
								// Remoção por porções
								printf("Remove by servings: ");
								scanf("%d", &servings);
								index = searchIndexAvl(table.indexAVL, servings);
								if(index >= 0) {
									removeData(&table, index);
								} else
									printf("Recipe not found.\n");
								break;
							case 3:
								// Remoção por dificuldade
								printf("Remove by difficulty [facil, medio ou dificil]: ");
								fgets(difficulty, 255, stdin);
								removeEnter(difficulty);
								index = searchIndexRb(table.indexRB, difficulty);
								if(index >= 0) {
									removeData(&table, index);
								} else
									printf("Recipe not found.\n");
								break;
							case 4:
								goto endRemove;
								break;
							default:
								printf("Invalid option.");
								break;
						}
						back();
					}
					endRemove:;
					break;
				case 5:
					// Finaliza o programa
					finish(&table);
					exit(1);
					break;
				default:
					printf("Invalid option.");
			}

		} while(1);
	} 
    return 0;
}

// Função do menu principal
int menu() {
	cleanScreen();
	printf("|----Culinary recipe storage system----|\n");
	printf("1 - Insert\n");
	printf("2 - Search\n");
	printf("3 - List\n");
	printf("4 - Remove\n");
	printf("5 - Exit\n");
	printf("Opcao : ");
	scanf("%d", &option);
	cleanScreen();
	getchar();
	return option;
}	

// Função do menu de busca
int menuSearch() {
	cleanScreen();
	printf("----Search----\n");
	printf("1 - by name: \n");
	printf("2 - by servings: \n");
	printf("3 - by difficulty [facil, medio ou dificil]: \n");
	printf("4 - return to main menu\n");
	printf("Option : ");
	scanf("%d", &option);
	cleanScreen();
	getchar();
	return option;
}

// Função do menu de listagem
int menuList() {
	cleanScreen();
	printf("----Listing---\n");
	printf("1 - by name\n");
	printf("2 - by servings\n");
	printf("3 - by difficulty [facil, medio ou dificil]\n");
	printf("4 - Return to main menu\n");
	printf("Option : ");
	scanf("%d", &option);
	cleanScreen();
	getchar();
	return option;
}

// Função do menu de remoção
int menuRemove() {
	cleanScreen();
	printf("----Removal---\n");
	printf("1 - by name\n");
	printf("2 - by servings\n");
	printf("3 - by difficulty [facil, medio ou dificil]\n");
	printf("4 - Return to main menu\n");
	printf("Option : ");
	scanf("%d", &option);
	cleanScreen();
	getchar();
	return option;
}

// Função de retorno ao menu anterior
int back() {
	printf("\n\n");
	printf("type any key to continue: ");
	while(!getchar());
}

// Função para limpar a tela
void cleanScreen(){
	system("cls");
}
