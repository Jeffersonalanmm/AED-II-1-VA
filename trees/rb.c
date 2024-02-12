#include "rb.h"
#include <stdio.h>
#include <stdlib.h>

rb no_null;

// Inicializa um novo índice RB
rbIndex* initializeIndexRb(int index, char *difficulty)  {
	rbIndex *new = (rbIndex*) malloc(sizeof(rbIndex));
	new->index = index;
	new->difficulty = (char*) malloc(sizeof(char));
	strcpy(new->difficulty, difficulty);
	return new;
}

// Inicializa uma árvore RB
void initializeRb(rb *root) {
	*root = NULL;
	no_null = (rb) malloc(sizeof(struct rbNode));
	no_null->color = DOUBLE_BLACK;
	no_null->data = 0;
	no_null->left = NULL;
	no_null->right = NULL;
}

// Verifica se um elemento é a raiz da árvore RB
int isRootRb(rb element) {
	return (element->father == NULL);
}

// Verifica se um elemento é filho esquerdo na árvore RB
int isLeftChildRb(rb element) {
	return (element->father != NULL && element == element->father->left);
}

// Verifica se um elemento é filho direito na árvore RB
int isRightChildRb(rb element) {
	return (element->father != NULL && element == element->father->right);
}

// Retorna o irmão de um elemento na árvore RB
rb isBrotherRb(rb element) {
	if(isLeftChildRb(element))
		return element->father->right;
	else
		return element->father->left;
}

// Retorna o tio de um elemento na árvore RB
rb uncleRb(rb element) {
	return isBrotherRb(element->father);
}

// Retorna a cor de um elemento na árvore RB
enum color colorRb(rb element) {
	enum color c;
	if(element == NULL)
		c = BLACK;
	else
		c = element->color;
	return c;
}
	
// Retorna a altura da árvore RB
int heightRb(rb a) {
	if(a == NULL) 
		return 0;
	else {
		int left, right;
		left = heightRb(a->left);
		right = heightRb(a->right);

		if(left > right)
			return (left + 1);
		else
			return (right + 1);
	}
}

// Retorna o elemento máximo na árvore RB
rbIndex* maxElementRb(rb root) {
	if(root == NULL)
		return NULL;
	if(root->right == NULL)
		return root->data;
	else
		return maxElementRb(root->right);
}

// Imprime a árvore RB em ordem
void inOrderRb(rb root) {
	if(root != NULL) {
		inOrderRb(root->left);
		printElementRb(root);
		inOrderRb(root->right);
	}
}

// Imprime um elemento da árvore RB
void printElementRb(rb root) {
	switch(root->color) {
		case BLACK:
			printf("\x1b[30m[%s]\x1b[0m", root->data->difficulty);
			break;
		case RED:
			printf("\x1b[31m[%s]\x1b[0m", root->data->difficulty);
			break;
		case DOUBLE_BLACK:
			printf("\x1b[32m[%s]\x1b[0m", root->data->difficulty);
			break;
	}
}

// Insere um novo elemento na árvore RB
void insertRb(rb *root, rbIndex *valor) {
	rb aux, father, new;
	aux = *root; 
	father = NULL;

	while(aux != NULL) {
		father = aux;
		if(strcmp(aux->data->difficulty, valor->difficulty) >= 0)
			aux = aux->left;
		else
			aux = aux->right;
	}

	new = (rb) malloc(sizeof(struct rbNode));
	new->data = valor;
	new->left = NULL;
	new->right = NULL;
	new->father = father;
	new->color = RED;

	if(isRootRb(new)) {
		*root = new;
	} else {
		if(strcmp(valor->difficulty, father->data->difficulty) > 0)
			father->right = new;
		else  
			father->left = new; 
	}

	adjustRb(root, new);
}

// Remove um elemento da árvore RB
void removeRb(rb *root, rb* root_relativa, char* difficulty) {
    rb aux = *root_relativa;

    while(aux != NULL) {
        if(strcmp(difficulty, aux->data->difficulty) == 0) {
            if(aux->left == NULL && aux->right == NULL) {
                if(isRootRb(aux)) {
                    free(aux->data->difficulty);
                    free(aux->data);
                    free(aux);
                    *root = NULL;
                    break;
                }
                if(aux->color == RED) {
                    if(isLeftChildRb(aux))
                        aux->father->left = NULL;
                    else
                        aux->father->right = NULL;
                    free(aux->data->difficulty);
                    free(aux->data);
                    free(aux);
                    break;

                } else {
                    rb no_null_father = aux->father;
                    rb no_null_child = aux;

                    if(isLeftChildRb(aux))
                        aux->father->left = no_null;
                    else 
                        aux->father->right = no_null;

                    readjustRb(root, no_null_child);
                    break;
                }
            }
            if(aux->right == NULL) {
                aux->left->color = BLACK;
                if(isRootRb(aux)) {
                    *root = aux->left;
                } else {
                    if(isLeftChildRb(aux))
                        aux->father->left = aux->left;
                    else 
                        aux->father->right = aux->left;
                }
                free(aux->data->difficulty);
                free(aux->data);
                free(aux);
                break;
            }
            if(aux->left == NULL) {
                aux->right->color = BLACK;
                if(isRootRb(aux)) {
                    *root = aux->right;
                } else {
                    if(isLeftChildRb(aux))
                        aux->father->left = aux->right;
                    else
                        aux->father->right = aux->right;
                }
                free(aux->data->difficulty);
                free(aux->data);
                free(aux);
                break;
            }
            if(aux->right != NULL && aux->left != NULL) {
                aux->data = maxElementRb(aux->left);
                removeRb(root, &(aux->left), aux->data->difficulty);
                break;
            }

        } else {
            if(strcmp(difficulty, aux->data->difficulty) > 0)
                aux = aux->right;
            else
                aux = aux->left;
        }
    }
}

// Reajusta a árvore RB após a remoção de um elemento
void readjustRb(rb *root, rb element) {
	if(isRootRb(element)) {
		element->color = BLACK;

		return;
	}

	if(colorRb(element->father) == BLACK &&
	 colorRb(isBrotherRb(element)) == RED &&
	 (colorRb(isBrotherRb(element)->right) == BLACK || isBrotherRb(element)->right == NULL) &&
	 (colorRb(isBrotherRb(element)->left) == BLACK || isBrotherRb(element)->left == NULL)) {
		if(isLeftChildRb(element))
			simpleLeftRotationRb(root, element);
		else 
			simpleRightRotationRb(root, element);

		element->father->father->color = BLACK;
		element->father->color = RED;

		return;
	 }

	 if(colorRb(element -> father) == BLACK && colorRb(isBrotherRb(element)) == BLACK &&
		(colorRb(isBrotherRb(element)-> right)  == BLACK || isBrotherRb(element) -> right == NULL) && 
		(colorRb(isBrotherRb(element) -> left)  == BLACK || isBrotherRb(element) -> left == NULL)){	
		isBrotherRb(element) -> color = RED;
		removeDoubleBlackRb(root, element);
		readjustRb(root, element->father);

		return;
	}

	if(colorRb(element -> father) == RED && 
		(colorRb(isBrotherRb(element)) == BLACK || isBrotherRb(element) == NULL) &&
		(colorRb(isBrotherRb(element)-> right)  == BLACK || isBrotherRb(element) -> right == NULL) && (colorRb(isBrotherRb(element) -> left) == BLACK || isBrotherRb(element) -> left == NULL)){

			element -> father -> color = BLACK;
			isBrotherRb(element) -> color = RED;
			
			removeDoubleBlackRb(root, element); 

			return;

	}

	if(isLeftChildRb(element) && colorRb(isBrotherRb(element)) == BLACK &&
		(colorRb(isBrotherRb(element) -> right) == BLACK || isBrotherRb(element) -> right == NULL) && colorRb(isBrotherRb(element) -> left) == RED){
			
			simpleRightRotationRb(root, isBrotherRb(element));
			isBrotherRb(element) -> color = BLACK;
			isBrotherRb(element) -> right -> color = RED;
			
			readjustRb(root, element);

			return;
		}

	if(isRightChildRb(element) && colorRb(isBrotherRb(element)) == BLACK &&
		(colorRb(isBrotherRb(element) -> left) == BLACK || isBrotherRb(element) -> left == NULL) 		
		&& colorRb(isBrotherRb(element) -> right) == RED) {
		
			simpleLeftRotationRb(root, isBrotherRb(element));
			isBrotherRb(element) -> color = BLACK;
			isBrotherRb(element) -> left -> color = RED;
			
			readjustRb(root, element);

			return;
	}

	if(isLeftChildRb(element) && 
		colorRb(isBrotherRb(element)) == BLACK && 
		colorRb(isBrotherRb(element) -> right) == RED) {
		
		enum color color_original_father = colorRb(element -> father);
		
		simpleLeftRotationRb(root, element -> father);
		
		element -> father -> father -> color = color_original_father;
		element -> father -> color = BLACK;
		uncleRb(element) -> color = BLACK;		
		
		removeDoubleBlackRb(root, element);

		return;
	}

	if(isRightChildRb(element) && 
		colorRb(isBrotherRb(element)) == BLACK && 
		colorRb(isBrotherRb(element) -> left) == RED) {

		enum color color_original_father = colorRb(element -> father);
		
		simpleRightRotationRb(root, element -> father);
		
		element -> father -> father -> color = color_original_father;
		element -> father -> color = BLACK;
		uncleRb(element) -> color = BLACK;		
		
		removeDoubleBlackRb(root, element);

		return;
	}
}

// Remove um nó duplamente preto da árvore RB
void removeDoubleBlackRb(rb *root, rb element) {
	if(element == no_null) {
		if(isLeftChildRb(element))
			element->father->left = NULL;
		else
			element->father->right = NULL;
	} else {
		element->color = BLACK;
	}
}

// Realiza o ajuste da árvore RB após inserção
void adjustRb(rb* root, rb new) {
	while(colorRb(new->father) == RED && colorRb(new) == RED) {
		if(colorRb(uncleRb(new)) == RED){
			uncleRb(new)->color = BLACK;
			new->father->color = BLACK;
			new->father->father->color = RED;
			new = new->father->father;
			continue;
		}
		if(isLeftChildRb(new) && isLeftChildRb(new->father)) {
			simpleRightRotationRb(root, new->father->father);
			new->father->color = BLACK;
			new->father->right->color = RED;
			continue;
		}
		if(isRightChildRb(new) && isRightChildRb(new->father)) {
			simpleLeftRotationRb(root, new->father->father);
			new->father->color = BLACK;
			new->father->left->color = RED;
			continue;
		}
		if(isRightChildRb(new) && isLeftChildRb(new->father)) {
			simpleLeftRotationRb(root, new->father);
			simpleRightRotationRb(root, new->father);
			new->right->color = RED;
			new->left->color = RED;
			new->color = BLACK;
			continue;
		}
		if(isLeftChildRb(new) && isRightChildRb(new->father)) {
            simpleRightRotationRb(root, new->father);
            simpleLeftRotationRb(root, new->father);
            new->right->color = RED;
            new->left->color = RED;
            new->color = BLACK;
            continue;
		}
	}
	(*root)->color = BLACK;
}

// Realiza uma rotação simples para a direita na árvore RB
void simpleRightRotationRb(rb *root, rb pivo) {
	rb p, u, t2;
	p = pivo;
	u = pivo->left;
	t2 = u->right;

	int posicao_pivo_left = isLeftChildRb(pivo);

	p->left = t2;

	if(t2 != NULL) 
		t2->father = pivo;

	u->right = pivo;
	u->father = p->father;
	p->father = u;

	p->color = RED;
	u->color = BLACK;

	if(isRootRb(u))
		*root = u;

	else {
		if(posicao_pivo_left)
			u->father->left = u;
		else
			u->father->right = u;
	}
}

// Realiza uma rotação simples para a esquerda na árvore RB
void simpleLeftRotationRb(rb *root, rb pivo) {
	rb p, u, t2;
	p = pivo;
	u = p->right;
	t2 = u->left;

	int posicao_pivo_right = isRightChildRb(pivo);

	p->right = t2;

	if(t2 != NULL)
		t2->father = p;

	u->left = p;
	u->father = p->father;
	p->father = u;

	p->color = RED;
	u->color = BLACK;
	
	if(isRootRb(u))
		*root = u;
	else {
		if(posicao_pivo_right)
			u->father->right = u;
		else
			u->father->left = u;
	}
}

// Busca o índice de um elemento na árvore RB
int searchIndexRb(rb root, char* difficulty) {
    if (root == NULL) {
        return -1; 
    }
    
    int cmp = strcmp(difficulty, root->data->difficulty);
    
    if (cmp == 0) {
        return root->data->index; // Retorna o índice se a dificuldade for encontrada
    } else if (cmp < 0) {
        return searchIndexRb(root->left, difficulty); // Procura na subárvore esquerda se a dificuldade for menor
    } else {
        return searchIndexRb(root->right, difficulty); // Procura na subárvore direita se a dificuldade for maior
    }
}
