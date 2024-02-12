#include <stdio.h>
#include <stdlib.h>
#include "./tabela/tabela.h"
#include "./tabela/tabela.c"
#include "./trees/bst.h"
#include "./trees/avl.h"
#include "./trees/rb.h"

int op_menu;

int menu();
int menuSearch();
int menuList();
int menuRemove();
int back();
void cleanScreen();

int main(int argc, char* argv[]) {

	table table;
	int index, servings;
	data recipe;

	char name[50], *difficulty =  (char*) malloc(sizeof(char)*256);


	if(initializeTable(&table)) {
		do{
			switch(menu()) {
				int value;
				case 1:
					printf("Enter recipe details\n\n");
					if(insertTable(&table, readData())) 
						printf("Exit to insert!\n");
					else 
						printf("Recipe isn't inserted!\n");
					break;
					back();
				case 2:
					while (1) {
						switch (menuSearch()) {
							case 1:
								printf("Search recipe by name\n\n");
								printf("Name: ");
								fgets(name, sizeof(name), stdin);
								removeEnter(name);
								cleanScreen();
								index = searchIndexBst(table.indexBST, name);
								if(index >= 0) {
									recipe = searchData(table.dataFile, index);
									printf("Search recipe by name\n\n");
									if(!recipe.removido)
										printData(recipe);
									else
										printf("Erro ao buscar\n");
								} else
									printf("Recipe not found\n");
								break;
							case 2:
								printf("Search recipe by servings\n\n");
								printf("Servings: ");
								scanf("%d", &servings);
								while (getchar() != '\n');
								cleanScreen();
								index = searchIndexAvl(table.indexAVL, servings);
								if(index >= 0) {
									recipe = searchData(table.dataFile, index);
									printf("Search recipe by servings\n\n");
									if(!recipe.removido)
										printData(recipe);
									else
										printf("Erro ao buscar");
								} else
									printf("Recipe not found.\n");
									break;
							case 3:
								printf("Search recipe by difficulty\n\n");
								printf("Difficulty: ");
								fgets(difficulty, 255, stdin);
								removeEnter(difficulty);
								cleanScreen();
								index = searchIndexRb(table.indexRB, difficulty);
								if(index >= 0) {
									recipe = searchData(table.dataFile, index);
									printf("Buscar Pokemon pela difficulty\n\n");
									if(!recipe.removido)
										printData(recipe);
									else
										printf("Erro ao buscar");
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
						while (1) {
							switch (menuList())	{
							case 1:
								printf("Listing recipe by name \n\n");
								printByRecipe(table.dataFile, table.indexBST);
								break;
							case 2:
								printf("Listing recipe by servings \n\n");
								printByServings(table.dataFile, table.indexAVL);
								break;
							case 3:
								printf("Listing recipe by difficulty\n\n");
								printByDifficulty(table.dataFile, table.indexRB);
								break;
							case 4:
								goto fim_listar;
								break;
							default:
								break;
							}
							back();
						}
						fim_listar:;
						break;
					case 4:
						while (1) {
							switch (menuRemove()) {
								case 1:
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
									printf("Remove by servings: ");
									scanf("%d", &servings);
									index = searchIndexAvl(table.indexAVL, servings);
									if(index >= 0) {
										removeData(&table, index);
									} else
										printf("Recipe not found.\n");
									break;
								case 3:
									printf("Remove by difficulty: ");
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
						finalizar(&table);
						exit(1);
					break;

					default:
						printf("Invalid option.");
			}

		} while(1);
	} 
    return 0;
}

int menu() {
	cleanScreen();
	printf("|----Culinary recipe storage system----|\n");
	printf("1 - Insert\n");
	printf("2 - Search\n");
	printf("3 - List\n");
	printf("4 - Remove\n");
	printf("5 - Exit\n");
	printf("Opcao : ");
	scanf("%d", &op_menu);
	cleanScreen();
	getchar();
	return op_menu;
}	

int menuSearch() {
	cleanScreen();
	printf("----Search----\n");
	printf("1 - by name: \n");
	printf("2 - by servings: \n");
	printf("3 - by difficulty: \n");
	printf("4 - return to main menu\n");
	printf("Option : ");
	scanf("%d", &op_menu);
	cleanScreen();
	getchar();
	return op_menu;
}

int menuList() {
	cleanScreen();
	printf("----Listing---\n");
	printf("1 - by name\n");
	printf("2 - by servings\n");
	printf("3 - by difficulty\n");
	printf("4 - Return to main menu\n");
	printf("Option : ");
	scanf("%d", &op_menu);
	cleanScreen();
	getchar();
	return op_menu;
}

int menuRemove() {
	cleanScreen();
	printf("----Removal---\n");
	printf("1 - by name\n");
	printf("2 - by servings\n");
	printf("3 - by difficulty\n");
	printf("4 - Return to main menu\n");
	printf("Option : ");
	scanf("%d", &op_menu);
	cleanScreen();
	getchar();
	return op_menu;
}

int back() {
	printf("\n\n");
	printf("type any key to continue: ");
	while(!getchar());
}

void cleanScreen(){
	system("cls");
}
