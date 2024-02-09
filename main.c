#include <stdio.h>
#include <stdlib.h>
#include "tabela/tabela.h"
#include "tabela/tabela.c"
#include "trees/bst.h"
#include "trees/avl.h"
#include "trees/rb.h"

int op_menu;

int menu();
int menu_buscar();
int menu_listar();
int menu_remover();
int voltar();
void limpar_tela();

int main(int argc, char* argv[]) {

	table tab;
	int indice, codigo;
	data pokemon;

	char nome[50], *descricao =  (char*) malloc(sizeof(char)*256);


	if(inicializarTabela(&tab)) {
		do{
			switch(menu()) {
				int valor;
				case 1:
					printf("Insira os dados do Pokemon\n\n");
					if(inserir_pokemon(&tab, ler_dados())) 
						printf("Sucesso ao inserir, argh fez o basico!\n");
					else 
						printf("pokemon nao inserido\n");
					break;
					voltar();
				case 2:
					while (1) {
						switch (menu_buscar()) {
							case 1:
								printf("Buscar Pokemon por nome\n\n");
								printf("Nome: ");
								fgets(nome, sizeof(nome), stdin);
								tirar_enter(nome);
								limpar_tela();
								indice = buscaIndiceBst(tab.indiceBst, nome);
								if(indice >= 0) {
									pokemon = buscar_pokemon(tab.arquivo_dados, indice);
									printf("Buscar Pokemon por nome\n\n");
									if(!pokemon.removido)
										imprimir_elemento(pokemon);
									else
										printf("Erro ao buscar\n");
								} else
									printf("Nao encontrado\n");
								break;
							case 2:
								printf("Buscar Pokemon por codigo\n\n");
								printf("Codigo: ");
								scanf("%d", &codigo);
								while (getchar() != '\n');
								limpar_tela();
								indice = buscar_indice_avl(tab.indice_avl, codigo);
								if(indice >= 0) {
									pokemon = buscar_pokemon(tab.arquivo_dados, indice);
									printf("Buscar Pokemon por codigo\n\n");
									if(!pokemon.removido)
										imprimir_elemento(pokemon);
									else
										printf("Erro ao buscar");
								} else
									printf("Nao encontrado\n");
									break;
							case 3:
								printf("Buscar Pokemon pela descricao\n\n");
								printf("Descricao: ");
								fgets(descricao, 255, stdin);
								tirar_enter(descricao);
								limpar_tela();
								indice = buscar_indice_rb(tab.indice_rb, descricao);
								if(indice >= 0) {
									pokemon = buscar_pokemon(tab.arquivo_dados, indice);
									printf("Buscar Pokemon pela descricao\n\n");
									if(!pokemon.removido)
										imprimir_elemento(pokemon);
									else
										printf("Erro ao buscar");
								} else
									printf("Nao encontrado\n");
								break;
							case 4:
								goto fim_buscar;
								break;
								
							default:		
								printf("Opcao invalida!\n");
								break;
							}
							voltar();
					}
					fim_buscar:;
					break;
					case 3 :
						while (1) {
							switch (menu_listar())	{
							case 1:
								printf("Listando Pokemon por nome \n\n");
								listar_por_nome(tab.arquivo_dados, tab.indiceBst);
								break;
							case 2:
								printf("Listando Pokemon por codigo \n\n");
								listar_por_codigo(tab.arquivo_dados, tab.indice_avl);
								break;
							case 3:
								printf("Listando Pokemon por descricao\n\n");
								listar_por_descricao(tab.arquivo_dados, tab.indice_rb);
								break;
							case 4:
								goto fim_listar;
								break;
							default:
								break;
							}
							voltar();
						}
						fim_listar:;
						break;
					case 4:
						while (1) {
							switch (menu_remover()) {
								case 1:
									printf("Remover por nome: ");
									fgets(nome, sizeof(nome), stdin);
									tirar_enter(nome);
									indice = buscaIndiceBst(tab.indiceBst, nome);
									if(indice >= 0) {
										remover_pokemon(&tab, indice);
									} else
										printf("Nao encontrado\n");
									break;

								case 2:
									printf("Remover por codigo: ");
									scanf("%d", &codigo);
									indice = buscar_indice_avl(tab.indice_avl, codigo);
									if(indice >= 0) {
										remover_pokemon(&tab, indice);
									} else
										printf("Nao encontrado\n");
									break;
								case 3:
									printf("Remover por descricao: ");
									fgets(descricao, 255, stdin);
									tirar_enter(descricao);
									indice = buscar_indice_rb(tab.indice_rb, descricao);
									if(indice >= 0) {
										remover_pokemon(&tab, indice);
									} else
										printf("Nao encontrado\n");
									break;
								case 4:
									goto fim_remover;
									break;
								default:
									printf("Opcao invalida");
									break;
								}
								voltar();
						}
						fim_remover:;
					break;

					case 5:
						finalizar(&tab);
						exit(1);
					break;

					default:
						printf("Opcao invalida");
			}

		} while(1);
	} 
    return 0;
}

int menu() {
	limpar_tela();
	printf("|----Sistema de Armazenamento de Pokemons----|\n");
	printf("1 - Inserir\n");
	printf("2 - Buscar\n");
	printf("3 - Listar\n");
	printf("4 - Remover\n");
	printf("5 - Sair\n");
	printf("Opcao : ");
	scanf("%d", &op_menu);
	limpar_tela();
	getchar();
	return op_menu;
}	

int menu_buscar() {
	limpar_tela();
	printf("----Buscar----\n");
	printf("1 - Por nome: \n");
	printf("2 - Por codigo: \n");
	printf("3 - Por descricao: \n");
	printf("4 - Voltar para o menu principal\n");
	printf("Opcao : ");
	scanf("%d", &op_menu);
	limpar_tela();
	getchar();
	return op_menu;
}

int menu_listar() {
	limpar_tela();
	printf("----Listagem---\n");
	printf("1 - Listagem ordenada por nome\n");
	printf("2 - Listagem ordenada por codigo\n");
	printf("3 - Listagem ordenada por descricao\n");
	printf("4 - Voltar para o menu principal\n");
	printf("Opcao : ");
	scanf("%d", &op_menu);
	limpar_tela();
	getchar();
	return op_menu;
}

int menu_remover() {
	limpar_tela();
	printf("----Remocao---\n");
	printf("1 - Por nome: \n");
	printf("2 - Por codigo: \n");
	printf("3 - Por descricao: \n");
	printf("4 - Voltar para o menu principal\n");
	printf("Opcao : ");
	scanf("%d", &op_menu);
	limpar_tela();
	getchar();
	return op_menu;
}

int voltar() {
	printf("\n\n");
	printf("Digite qualquer tecla para voltar: ");
	while(!getchar());
}

void limpar_tela(){
	system("clear");
}