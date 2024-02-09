#ifndef RB_H
#define RB_H
#include <stdlib.h>

enum cor { VERMELHO, PRETO, DUPLO_PRETO };

typedef struct rbIndex {
	char *servings;
	int index; //posição em bytes do primeiro byte do regsitro no arquivo de dados
}rbIndex;

typedef struct rbNode {
	rbIndex *data;
	enum cor cor;
	struct rbNode *left, *right, *father;
}rbNode;

typedef rbNode* rb;

//funcoes principais
rbIndex* inicializar_indice_rb(int indice, char *servings);
void inicializar_rb(rb *raiz);
void inserir_rb(rb *raiz, rbIndex *valor);
void remover_rb(rb *raiz, rb* raiz_relativa, char *servings);
int altura_rb(rb raiz);
void imprimir_rb(rb raiz);
void imprimir_elemento_rb(rb raiz);
int buscar_indice_rb(rb raiz, char* servings);

//funcoes auxiliares

enum cor cor_rb(rb elemento);
int eh_raiz_rb(rb elemento);
int eh_filho_esquerdo_rb(rb elemento);
int eh_filho_direito_rb(rb elemento);
rbIndex* maior_elemento_rb(rb raiz);
rb eh_irmao_rb(rb elemento);
rb tio_rb(rb elemento);
int maior_rb(int a, int b);

//ajustes
void ajustar_rb(rb *raiz, rb elemento);
void rotacao_simples_direita_rb(rb *raiz, rb pivo);
void rotacao_simples_esquerda_rb(rb *raiz, rb pivo);
void reajustar_rb(rb *raiz, rb elemento);
void retira_duplo_preto_rb(rb *raiz, rb elemento);
void preorder_rb(rb a);
#endif