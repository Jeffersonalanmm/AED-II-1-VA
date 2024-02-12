#include <stdio.h>
#include <stdlib.h>
#include "avl.h"

// Retorna a altura da árvore AVL
int heightAvl(avl a) {
	if(a == NULL) {
		return 0; // Retorna 0 se a árvore estiver vazia
	} else {
		int left = heightAvl(a->left);
		int right = heightAvl(a->right);
	
		if(left > right)
			return left + 1; // Retorna a altura da subárvore esquerda mais 1
		else 
			return right + 1; // Retorna a altura da subárvore direita mais 1
		
	}
}

// Inicializa um novo índice AVL
avlIndex* initializeIndexAvl(int index, int servings)  {
	avlIndex *novo = (avlIndex*) malloc(sizeof(avlIndex));
	novo->index = index;
	novo->servings = servings;
	return novo;
}

// Procura um índice AVL com base no número de porções
int searchIndexAvl(avl root, int servings) {
    if (root == NULL) {
        return -1; // Retorna -1 se a árvore estiver vazia
    }
    
    if (servings == root->data->servings) {
        return root->data->index; // Retorna o índice se o número de porções for encontrado
    } else if (servings < root->data->servings) {
        return searchIndexAvl(root->left, servings); // Procura na subárvore esquerda se o número de porções for menor
    } else {
        return searchIndexAvl(root->right, servings); // Procura na subárvore direita se o número de porções for maior
    }
}

// Insere um novo nó AVL
avl insertAvl(avl a, avlIndex *v, int *grow) {
    if(grow == NULL) {
        int c = 1;
        grow = &c;
    }
	// Caso base: a raiz é vazia, então cria um novo nó
	if(a == NULL) {
		avl new = (avl) malloc(sizeof(avlNode));
		new->data = v;
		new->left = NULL;
		new->right = NULL;
		new->fb = 0;
		*grow = 1;

		return new;
		
	} // Caso contrário, busca onde inserir
	else {
		// Se o valor for maior que a raiz, adiciona o elemento à direita
		if(v->servings > a->data->servings) {
			a->right = insertAvl(a->right, v, grow);

			// Calcula os reajustes do fator de balanceamento
			if(*grow) {
				switch(a->fb){
					// Árvore totalmente balanceada e agora tem um fator no lado direito
					case 0:
						a->fb = 1;
					   *grow = 1;
					    break;
					
					// Árvore pendendo para a esquerda e agora ficou equilibrada
					case -1:
						a->fb = 0;
						*grow = 0;
						break;

					// Caso o fator de balanceamento no lado direito seja 1 e agora se torne 2, precisa ser balanceada
					case 1:
						// a->fb = 2;
						*grow = 0; 
						a = rotationAvl(a); // Como pendeu para a direita, rotaciona para a esquerda
						break;

				}
			}
		} 
		else {
			// Elemento menor que a raiz, adiciona o elemento à esquerda
			a->left = insertAvl(a->left, v, grow);

			// Calcula os reajustes do fator de balanceamento
			if(*grow) {
				switch(a->fb) {
					// Árvore balanceada e agora pende 1 para a direita
					case 0:
						a->fb = -1;
						*grow = 1;
						break;
					// Árvore pende um para a esquerda e agora equilibra
					case 1:
						a->fb = 0;
						*grow = 0;
						break;

					// Pendia 1 para a esquerda e agora desequilibra
					case -1:
						// a->fb = -2;
						*grow = 0;
						a = rotationAvl(a);
						break;

				}
			}
		}
	}
	return a;
}

// Remove um elemento da árvore AVL
avl removeElementAvl(avl a, int v, int *down) {
    if (a == NULL) {
        *down = 0;
        return a;
    }

    if (down == NULL) {
        int d = 0;
        down = &d;
    }

    // Um ou nenhum filho
    if (v == a->data->servings) {
        if (a->left == NULL || a->right == NULL) {
            avl temp = a->left ? a->left : a->right;
            if (temp == NULL) {
                temp = a;
                a = NULL;
            } else {
                *a = *temp;
            }
            free(temp);
            *down = 1;
            return a;
        }
        // Dois filhos
        avl aux = a->left;
        while (aux->right != NULL)
            aux = aux->right;

        a->data = aux->data;
        a->left = removeElementAvl(a->left, aux->data->servings, down);

        // Verifica se a altura da subárvore esquerda foi alterada
        if (*down) {
            switch (a->fb) {
                case 0:
                    a->fb = 1;
                    *down = 0;
                    break;
                case -1:
                    a->fb = 0;
                    *down = 1;
                    break;
                case 1:
                    *down = 1;
                    a = rotationAvl(a);
            }
        }
        return a;
    }

    // Remoção na subárvore esquerda ou direita
    if (v < a->data->servings) {
        a->left = removeElementAvl(a->left, v, down);
    } else {
        a->right = removeElementAvl(a->right, v, down);
    }

    // Atualiza o fator de balanceamento
    if (*down) {
        switch (a->fb) {
            case 0:
                a->fb = 1;
                *down = 0;
                break;
            case -1:
                a->fb = 0;
                *down = 1;
                break;
            case 1:
                *down = 1;
                a = rotationAvl(a);
        }
    }
    return a;
}

// Executa uma rotação AVL
avl rotationAvl(avl a){
    if (a->fb > 0){
        if(a->right->fb >= 0){
            return simpleLeftRotationAvl(a);
        }else{
            return doubleLeftRotationAvl(a);
        }
    }else{
        if (a->left->fb <= 0){
            return simpleRightRotationAvl(a);
        }else{
            return doubleRightRotationAvl(a);
        }

    }
}

// Executa uma rotação simples para a esquerda
avl simpleLeftRotationAvl(avl a) {
	// O nó desbalanceado é p, fb(p) == 2 e fb(u) == 1
	avl p, u;
	p = a;
	u = p->right;

	// Corrige os fatores de balanceamento
	u->fb = 0;
	p->fb = 0;

	// Rotação AVL
	p->right = u->left;
	u->left = p;

	return u;
}

// Executa uma rotação simples para a direita
avl simpleRightRotationAvl(avl a) {
	// fb(p) == -2 e fb(u) == -1
	avl p, u;

	p = a;
	u = p->left;

	// Corrige os fatores de balanceamento
	u->fb = 0;
	p->fb = 0;
	
	// Rotação
	p->left = u->right;
	u->right = p;

	return u;
}

// Executa uma rotação dupla para a esquerda
avl doubleLeftRotationAvl(avl a){
 	// fb(p) = 2 e fb(u) = -1
    avl p, u, v;

    p = a;
    u = p->right;
    v = u->left;

    // Rotação à direita   
    u->left = v->right;
    v->right = u;
    p->right = v;

    // Rotação à esquerda
    p->right = v->left;
    v->left = p;
    
    // Atualizando os fatores de balanceamento
    if(v->fb==0){  
        u->fb = 0;
        v->fb = 0;
        p->fb=0;
    
    }    
    else if(v->fb==-1){  

        u->fb = 1;
        v->fb = 0;
        p->fb = 0;
    }   
     else{  
        u->fb = 0;
        v->fb = 0;
        p->fb =-1;
    }
   return v;
}

// Executa uma rotação dupla para a direita
avl doubleRightRotationAvl(avl a) {
	// fb(p) = -2 e fb(u) = 1
	avl p, u, v;

	p = a;
	u = p->left;
    v = u->right;

    // Rotação à esquerda    
    u->right = v->left;
    v->left = u;
    p->left = v;
    
    // Rotação à direita
    p->left=v->right;
    v->right = p;
    
    // Atualizando os fatores de balanceamento
    if(v->fb == 0){ 

        u->fb = 0;
        p->fb = 0;
        v->fb = 0;
    }
    else if(v->fb==-1){

        u->fb = 0;
        v->fb = 0;
        p->fb = 1;

    }
    else {

        u->fb = -1;
        v->fb = 0;
        p->fb = 0;

    }
    return v;
}

// Percorre a árvore AVL em ordem
void inOrderAvl(avl raiz) {
	if(raiz != NULL) {
		inOrderAvl(raiz->left);
		printElementAvl(raiz);
		inOrderAvl(raiz->right);
	}
}

// Imprime um elemento da árvore AVL
void printElementAvl(avl raiz) {
	if(raiz != NULL) {
		printf("%d | ", raiz->data->servings);

	}
}