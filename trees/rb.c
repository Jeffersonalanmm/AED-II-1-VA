#include "rb.h"
#include <stdio.h>
#include <stdlib.h>

rb no_null;

rbIndex* inicializar_indice_rb(int index, char *servings)  {
	rbIndex *novo = (rbIndex*) malloc(sizeof(rbIndex));
	novo->index = index;
	novo->servings = (char*) malloc(sizeof(char));
	strcpy(novo->servings, servings);
	return novo;
}

void inicializar_rb(rb *raiz) {
	*raiz = NULL;
	no_null = (rb) malloc(sizeof(struct rbNode));
	no_null->cor = DUPLO_PRETO;
	no_null->data = 0;
	no_null->left = NULL;
	no_null->right = NULL;
}

int eh_raiz_rb(rb elemento) {
	return (elemento->father == NULL);
}

int eh_filho_leftuerdo_rb(rb elemento) {
	return (elemento->father != NULL && elemento == elemento->father->left);
}
int eh_filho_righteito_rb(rb elemento) {
	return (elemento->father != NULL && elemento == elemento->father->right);
}

rb eh_irmao_rb(rb elemento) {
	//se ele eh o filho leftuerdo, retorna o filho do nodo acima dele, a righteita
	if(eh_filho_leftuerdo_rb(elemento))
		return elemento->father->right;
	else
		return elemento->father->left;
}

rb tio_rb(rb elemento) {
	return eh_irmao_rb(elemento->father);
}


enum cor cor_rb(rb elemento) {
	enum cor c;
	if(elemento == NULL)
		c = PRETO;
	else
		c = elemento->cor;
	return c;
}
	
int altura_rb(rb a) {
	if(a == NULL) 
		return 0;
	else {
		int left, right;
		left = altura_rb(a->left);
		right = altura_rb(a->right);

		if(left > right)
			return (left + 1);
		else
			return (right + 1);
	}
}

rbIndex* maior_elemento_rb(rb raiz) {
	if(raiz == NULL)
		return NULL;
	if(raiz->right == NULL)
		return raiz->data;
	else
		return maior_elemento_rb(raiz->right);
}

void imprimir_rb(rb raiz) {
	if(raiz != NULL) {
		imprimir_rb(raiz->left);
		imprimir_elemento_rb(raiz);
		imprimir_rb(raiz->right);
	}
}

void imprimir_elemento_rb(rb raiz) {
	switch(raiz->cor) {
		case PRETO:
			printf("\x1b[30m[%s]\x1b[0m", raiz->data->servings);
			break;
		case VERMELHO:
			printf("\x1b[31m[%s]\x1b[0m", raiz->data->servings);
			break;
		case DUPLO_PRETO:
			printf("\x1b[32m[%s]\x1b[0m", raiz->data->servings);
			break;
	}
}


void inserir_rb(rb *raiz, rbIndex *valor) {
	rb aux, father, novo;
	aux = *raiz; 
	father = NULL;

	while(aux != NULL) {
		father = aux;
		//verificando para que lado devemos percorrer
		if(strcmp(aux->data->servings, valor->servings) >= 0)
			aux = aux->left;
		else
			aux = aux->right;
	}

	novo = (rb) malloc(sizeof(struct rbNode));
	novo->data = valor;
	novo->left = NULL;
	novo->right = NULL;
	novo->father = father;
	novo->cor = VERMELHO;

	if(eh_raiz_rb(novo)) {
		*raiz = novo;
	} else {
		if(strcmp(valor->servings, father->data->servings) > 0)
			father->right = novo;
		else  
			father->left = novo; 
	}

	//essa eh a raiz de toda a arvore, não a raiz relativa;
	//apos inserido o elemento, precisamos verificar a cor dele e afins
	ajustar_rb(raiz, novo);

}

void remover_rb(rb *raiz, rb* raiz_relativa, char* servings) {
	rb aux = *raiz_relativa;

	while(aux != NULL) {
		if(strcmp(servings, aux->data->servings) == 0) {
			//sem filhos
			if(aux->left == NULL && aux->right == NULL) {
				//remover_rb raiz sem filhos
				if(eh_raiz_rb(aux)) {
					*raiz = NULL;
					break;
				}
				//sem filhos, mas nao raiz
				if(aux->cor == VERMELHO) {
					if(eh_filho_leftuerdo_rb(aux))
						aux->father->left = NULL;
					else
						aux->father->right = NULL;
					break;

				} else {
					/*se o elemento for preto, 
					precisa substituir pelo nonull */
					no_null->father = aux->father;

					if(eh_filho_leftuerdo_rb(aux))
						aux->father->left = no_null;
					else 
						aux->father->right = no_null;

					reajustar_rb(raiz, no_null);
					break;
				}
			}
			//um filho

			//apenas o filho leftuerdo
			if(aux->right == NULL) {
				aux->left->cor = PRETO;
				if(eh_raiz_rb(aux)) {
					*raiz = aux->left;
				} else {
					if(eh_filho_leftuerdo_rb(aux))
						aux->father->left = aux->left;
					else 
						aux->father->right = aux->left;
				}
				break;
			}
			//apenas filho righteito
			if(aux->left == NULL) {
				aux->right->cor = PRETO;
				if(eh_raiz_rb(aux)) {
					*raiz = aux->right;
				} else {
					if(eh_filho_leftuerdo_rb(aux))
						aux->father->left = aux->right;
					else
						aux->father->right = aux->right;
				}
				break;
			}
			//dois filhos
			if(aux->right != NULL && aux->left != NULL) {
				aux->data = maior_elemento_rb(aux->left);
				remover_rb(raiz, &(aux->left), aux->data->servings);
				break;
			}

		} else {
			if(strcmp(servings, aux->data->servings) > 0)
				aux = aux->right;
			else
				aux = aux->left;
		}
	}
}

void reajustar_rb(rb *raiz, rb elemento) {
	
	//caso 1: eh a raiz
	if(eh_raiz_rb(elemento)) {
		elemento->cor = PRETO;

		return;
	}

	//caso 2: father preto, irmao vermelho e sobrinhos preto
	if(cor_rb(elemento->father) == PRETO &&
	 cor_rb(eh_irmao_rb(elemento)) == VERMELHO &&
	 (cor_rb(eh_irmao_rb(elemento)->right) == PRETO || eh_irmao_rb(elemento)->right == NULL) &&
	 (cor_rb(eh_irmao_rb(elemento)->left) == PRETO || eh_irmao_rb(elemento)->left == NULL)) {
		if(eh_filho_leftuerdo_rb(elemento))
			rotacao_simples_leftuerda_rb(raiz, elemento);
		else 
			rotacao_simples_righteita_rb(raiz, elemento);

		elemento->father->father->cor = PRETO;
		elemento->father->cor = VERMELHO;

		return;
	 }

	 //caso 3: father, irmaos e sobrinhos pretos
	 if(cor_rb(elemento -> father) == PRETO && cor_rb(eh_irmao_rb(elemento)) == PRETO &&
		(cor_rb(eh_irmao_rb(elemento)-> right)  == PRETO || eh_irmao_rb(elemento) -> right == NULL) && 
		(cor_rb(eh_irmao_rb(elemento) -> left)  == PRETO || eh_irmao_rb(elemento) -> left == NULL)){	
		eh_irmao_rb(elemento) -> cor = VERMELHO;
		retira_duplo_preto_rb(raiz, elemento);
		reajustar_rb(raiz, elemento->father);

		return;
	}

	//caso 4: father vermelho, irmao e sobrinhps preto
	if(cor_rb(elemento -> father) == VERMELHO && 
		(cor_rb(eh_irmao_rb(elemento)) == PRETO || eh_irmao_rb(elemento) == NULL) &&
		(cor_rb(eh_irmao_rb(elemento)-> right)  == PRETO || eh_irmao_rb(elemento) -> right == NULL) && (cor_rb(eh_irmao_rb(elemento) -> left) == PRETO || eh_irmao_rb(elemento) -> left == NULL)){

			elemento -> father -> cor = PRETO;
			eh_irmao_rb(elemento) -> cor = VERMELHO;
			
			retira_duplo_preto_rb(raiz, elemento); 

			return;

	}
	// CASO 5: irmão preto e um dos sobrinhos vermelho

	//caso 5a
	if(eh_filho_leftuerdo_rb(elemento) && cor_rb(eh_irmao_rb(elemento)) == PRETO &&
		(cor_rb(eh_irmao_rb(elemento) -> right) == PRETO || eh_irmao_rb(elemento) -> right == NULL) && cor_rb(eh_irmao_rb(elemento) -> left) == VERMELHO){
			
			rotacao_simples_righteita_rb(raiz, eh_irmao_rb(elemento));
			eh_irmao_rb(elemento) -> cor = PRETO;
			eh_irmao_rb(elemento) -> right -> cor = VERMELHO;
			
			reajustar_rb(raiz, elemento);

			return;
		}

	//caso 5b
	if(eh_filho_righteito_rb(elemento) && cor_rb(eh_irmao_rb(elemento)) == PRETO &&
		(cor_rb(eh_irmao_rb(elemento) -> left) == PRETO || eh_irmao_rb(elemento) -> left == NULL) 		
		&& cor_rb(eh_irmao_rb(elemento) -> right) == VERMELHO) {
		
			rotacao_simples_leftuerda_rb(raiz, eh_irmao_rb(elemento));
			eh_irmao_rb(elemento) -> cor = PRETO;
			eh_irmao_rb(elemento) -> left -> cor = VERMELHO;
			
			reajustar_rb(raiz, elemento);

			return;
	}

	//caso 6: irmao preto e um dos sobrinhos vermelho

	//caso 6a:
	if(eh_filho_leftuerdo_rb(elemento) && 
		cor_rb(eh_irmao_rb(elemento)) == PRETO && 
		cor_rb(eh_irmao_rb(elemento) -> right) == VERMELHO) {
		
		enum cor cor_original_father = cor_rb(elemento -> father);
		
		rotacao_simples_leftuerda_rb(raiz, elemento -> father);
		
		elemento -> father -> father -> cor = cor_original_father;
		elemento -> father -> cor = PRETO;
		tio_rb(elemento) -> cor = PRETO;		
		
		retira_duplo_preto_rb(raiz, elemento);

		return;
	}

	//caso 6b
	if(eh_filho_righteito_rb(elemento) && 
		cor_rb(eh_irmao_rb(elemento)) == PRETO && 
		cor_rb(eh_irmao_rb(elemento) -> left) == VERMELHO) {

		enum cor cor_original_father = cor_rb(elemento -> father);
		
		rotacao_simples_righteita_rb(raiz, elemento -> father);
		
		elemento -> father -> father -> cor = cor_original_father;
		elemento -> father -> cor = PRETO;
		tio_rb(elemento) -> cor = PRETO;		
		
		retira_duplo_preto_rb(raiz, elemento);

		return;
	}
}

void retira_duplo_preto_rb(rb *raiz, rb elemento) {
	if(elemento == no_null) {
		if(eh_filho_leftuerdo_rb(elemento))
			elemento->father->left = NULL;
		else
			elemento->father->right = NULL;
	} else {
		elemento->cor = PRETO;
	}
}


void ajustar_rb(rb* raiz, rb novo) {
	
	//se o father e o elemento forem vermelhos, precisa ajustar_rb
	while(cor_rb(novo->father) == VERMELHO && cor_rb(novo) == VERMELHO) {
		//caso 1
		if(cor_rb(tio_rb(novo)) == VERMELHO){
			//tornando o tio_rb e father do elemento inseido preto
			tio_rb(novo)->cor = PRETO;
			novo->father->cor = PRETO;
			novo->father->father->cor = VERMELHO;

			//agr eu quero que verifique se o avo do elemento inserido
			//que se tornou vermelho, está na cor correta, ent eu atualizo
			novo = novo->father->father;

			continue;
		}
		//case 2a: rotacao simples righteita
		if(eh_filho_leftuerdo_rb(novo) && eh_filho_leftuerdo_rb(novo->father)) {
			//rotacao simples passando a raiz e o avo do elemento
			rotacao_simples_righteita_rb(raiz, novo->father->father);
			novo->father->cor = PRETO;
			novo->father->right->cor = VERMELHO;

			continue;
		}
		//caso 2b: rotacao simples leftuerda
		if(eh_filho_righteito_rb(novo) && eh_filho_righteito_rb(novo->father)) {
			rotacao_simples_leftuerda_rb(raiz, novo->father->father);
			novo->father->cor = PRETO;
			novo->father->left->cor = VERMELHO;

			continue;
		}
		//caso 3a: rotacao dupla righteita
		if(eh_filho_righteito_rb(novo) && eh_filho_leftuerdo_rb(novo->father)) {
			rotacao_simples_leftuerda_rb(raiz, novo->father);
			rotacao_simples_righteita_rb(raiz, novo->father);
			novo->right->cor = VERMELHO;
			novo->left->cor = VERMELHO;
			novo->cor = PRETO;

			continue;
		}
		//caso 3b: rotacao dupla leftuerda
		if(eh_filho_leftuerdo_rb(novo) && eh_filho_righteito_rb(novo->father)) {
            rotacao_simples_righteita_rb(raiz, novo->father);
            rotacao_simples_leftuerda_rb(raiz, novo->father);
            novo->right->cor = VERMELHO;
            novo->left->cor = VERMELHO;
            novo->cor = PRETO;
            
            continue;

		}
	}
	//Após todas as correções a raiz pode ter ficado na cor vermelha, portanto passamos ela novamente para cor preta
	(*raiz)->cor = PRETO;
}

/*
		(p)					(u)
	   /   \               /   \
	 (u)	(t3)  ==>	 (t1)	(p)
	/   \			            /  \   
 (t1)   (t2)		          (t2) (t3)                           
*/


void rotacao_simples_righteita_rb(rb *raiz, rb pivo) {
	rb p, u, t2;
	p = pivo;
	u = pivo->left;
	t2 = u->right;

	//para fazer a ligacao da raiz a sub-arvore com seu father,
	//eh preciso saber se o pivo era um filho leftuerdo ou righteito

	int posicao_pivo_left = eh_filho_leftuerdo_rb(pivo);
	//pois ele precisa ser inserido em um dos lados do father

	//trocando o lado do t2
	p->left = t2;

	//se ele n for null, precisa linká-lo novamente
	if(t2 != NULL) 
		t2->father = pivo;

	//agora fazendo a rotação dos elementos restantes
	u->right = pivo;
	u->father = p->father;
	p->father = u;

	p->cor = VERMELHO;
	u->cor = PRETO;

	//testando se u eh raiz, pois se for, a raiz da arvore recebe ele
	if(eh_raiz_rb(u))
		*raiz = u;

	//caso n seja, vamos descobrir se o pivo (valor que agr o u ocupa o lugar)
	//era filho leftuerdo ou n para posicioná-lo corretamente
	//em relação ao father
	else {
		if(posicao_pivo_left)
			u->father->left = u;
		else
			u->father->right = u;
	}
}

/*
		(p)						(u)
	   /   \                   /   \
	 (t1)	(u)       ==>	 (p)	(t3)
	 	   /   \			/	\   
	 	(t2)   (t3)		  (t1)  (t2) 
  
*/
void rotacao_simples_leftuerda_rb(rb *raiz, rb pivo) {
	rb p, u, t2;
	p = pivo;
	u = p->right;
	t2 = u->left;

	int posicao_pivo_right = eh_filho_righteito_rb(pivo);

	p->right = t2;

	if(t2 != NULL)
		t2->father = p;

	u->left = p;
	u->father = p->father;
	p->father = u;

	p->cor = VERMELHO;
	u->cor = PRETO;
	
	if(eh_raiz_rb(u))
		*raiz = u;
	else {
		if(posicao_pivo_right)
			u->father->right = u;
		else
			u->father->left = u;
	}
}

void preorder_rb (rb raiz) {
  if (raiz != NULL) {
      imprimir_elemento_rb(raiz);
      preorder_rb (raiz->left);
      preorder_rb (raiz->right);
    }
}

int buscar_indice_rb(rb raiz, char* servings) {
	if(servings != NULL && raiz != NULL) {
		int cmp = strcmp(servings, raiz->data->servings);
		if(cmp == 0) {
			return raiz->data->index;
		}
		else {
			if(cmp > 0)
				return buscar_indice_rb(raiz->right, servings);
			else 
				return buscar_indice_rb(raiz->left, servings);
		}
	} else {
		return -1;
	}
}