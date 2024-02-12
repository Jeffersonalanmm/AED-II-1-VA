#include "rb.h"
#include <stdio.h>
#include <stdlib.h>

rb no_null;

rbIndex* initializeIndexRb(int index, char *difficulty)  {
	rbIndex *new = (rbIndex*) malloc(sizeof(rbIndex));
	new->index = index;
	new->difficulty = (char*) malloc(sizeof(char));
	strcpy(new->difficulty, difficulty);
	return new;
}

void initializeRb(rb *root) {
	*root = NULL;
	no_null = (rb) malloc(sizeof(struct rbNode));
	no_null->color = DOUBLE_BLACK;
	no_null->data = 0;
	no_null->left = NULL;
	no_null->right = NULL;
}

int isRootRb(rb element) {
	return (element->father == NULL);
}

int isLeftChildRb(rb element) {
	return (element->father != NULL && element == element->father->left);
}
int isRightChildRb(rb element) {
	return (element->father != NULL && element == element->father->right);
}

rb isBrotherRb(rb element) {
	//se ele eh o filho leftuerdo, retorna o filho do nodo acima dele, a righteita
	if(isLeftChildRb(element))
		return element->father->right;
	else
		return element->father->left;
}

rb uncleRb(rb element) {
	return isBrotherRb(element->father);
}


enum color colorRb(rb element) {
	enum color c;
	if(element == NULL)
		c = BLACK;
	else
		c = element->color;
	return c;
}
	
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

rbIndex* maxElementRb(rb root) {
	if(root == NULL)
		return NULL;
	if(root->right == NULL)
		return root->data;
	else
		return maxElementRb(root->right);
}

void printRb(rb root) {
	if(root != NULL) {
		printRb(root->left);
		printElementRb(root);
		printRb(root->right);
	}
}

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


void insertRb(rb *root, rbIndex *valor) {
	rb aux, father, new;
	aux = *root; 
	father = NULL;

	while(aux != NULL) {
		father = aux;
		//verificando para que lado devemos percolorrer
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

	//essa eh a root de toda a arvore, não a root relativa;
	//apos inserido o element, precisamos verificar a color dele e afins
	adjustRb(root, new);

}

void removeRb(rb *root, rb* root_relativa, char* difficulty) {
    rb aux = *root_relativa;

    while(aux != NULL) {
        if(strcmp(difficulty, aux->data->difficulty) == 0) {
            // Sem filhos
            if(aux->left == NULL && aux->right == NULL) {
                // Remove a root sem filhos
                if(isRootRb(aux)) {
                    free(aux->data->difficulty);
                    free(aux->data);
                    free(aux);
                    *root = NULL;
                    break;
                }
                // Sem filhos, mas não root
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
                    /* Se o elemento for BLACK, 
                       precisa substituir pelo no_null */
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
            // Um filho
            // Apenas o filho esquerdo
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
            // Apenas filho direito
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
            // Dois filhos
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


void readjustRb(rb *root, rb element) {
	
	//caso 1: eh a root
	if(isRootRb(element)) {
		element->color = BLACK;

		return;
	}

	//caso 2: father BLACK, irmao RED e sobrinhos BLACK
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

	 //caso 3: father, irmaos e sobrinhos BLACKs
	 if(colorRb(element -> father) == BLACK && colorRb(isBrotherRb(element)) == BLACK &&
		(colorRb(isBrotherRb(element)-> right)  == BLACK || isBrotherRb(element) -> right == NULL) && 
		(colorRb(isBrotherRb(element) -> left)  == BLACK || isBrotherRb(element) -> left == NULL)){	
		isBrotherRb(element) -> color = RED;
		removeDoubleBlackRb(root, element);
		readjustRb(root, element->father);

		return;
	}

	//caso 4: father RED, irmao e sobrinhps BLACK
	if(colorRb(element -> father) == RED && 
		(colorRb(isBrotherRb(element)) == BLACK || isBrotherRb(element) == NULL) &&
		(colorRb(isBrotherRb(element)-> right)  == BLACK || isBrotherRb(element) -> right == NULL) && (colorRb(isBrotherRb(element) -> left) == BLACK || isBrotherRb(element) -> left == NULL)){

			element -> father -> color = BLACK;
			isBrotherRb(element) -> color = RED;
			
			removeDoubleBlackRb(root, element); 

			return;

	}
	// CASO 5: irmão BLACK e um dos sobrinhos RED

	//caso 5a
	if(isLeftChildRb(element) && colorRb(isBrotherRb(element)) == BLACK &&
		(colorRb(isBrotherRb(element) -> right) == BLACK || isBrotherRb(element) -> right == NULL) && colorRb(isBrotherRb(element) -> left) == RED){
			
			simpleRightRotationRb(root, isBrotherRb(element));
			isBrotherRb(element) -> color = BLACK;
			isBrotherRb(element) -> right -> color = RED;
			
			readjustRb(root, element);

			return;
		}

	//caso 5b
	if(isRightChildRb(element) && colorRb(isBrotherRb(element)) == BLACK &&
		(colorRb(isBrotherRb(element) -> left) == BLACK || isBrotherRb(element) -> left == NULL) 		
		&& colorRb(isBrotherRb(element) -> right) == RED) {
		
			simpleLeftRotationRb(root, isBrotherRb(element));
			isBrotherRb(element) -> color = BLACK;
			isBrotherRb(element) -> left -> color = RED;
			
			readjustRb(root, element);

			return;
	}

	//caso 6: irmao BLACK e um dos sobrinhos RED

	//caso 6a:
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

	//caso 6b
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


void adjustRb(rb* root, rb new) {
	
	//se o father e o element forem REDs, precisa adjustRb
	while(colorRb(new->father) == RED && colorRb(new) == RED) {
		//caso 1
		if(colorRb(uncleRb(new)) == RED){
			//tornando o uncleRb e father do element inseido BLACK
			uncleRb(new)->color = BLACK;
			new->father->color = BLACK;
			new->father->father->color = RED;

			//agr eu quero que verifique se o avo do element inserido
			//que se tornou RED, está na color colorreta, ent eu atualizo
			new = new->father->father;

			continue;
		}
		//case 2a: rotacao simples righteita
		if(isLeftChildRb(new) && isLeftChildRb(new->father)) {
			//rotacao simples passando a root e o avo do element
			simpleRightRotationRb(root, new->father->father);
			new->father->color = BLACK;
			new->father->right->color = RED;

			continue;
		}
		//caso 2b: rotacao simples leftuerda
		if(isRightChildRb(new) && isRightChildRb(new->father)) {
			simpleLeftRotationRb(root, new->father->father);
			new->father->color = BLACK;
			new->father->left->color = RED;

			continue;
		}
		//caso 3a: rotacao dupla righteita
		if(isRightChildRb(new) && isLeftChildRb(new->father)) {
			simpleLeftRotationRb(root, new->father);
			simpleRightRotationRb(root, new->father);
			new->right->color = RED;
			new->left->color = RED;
			new->color = BLACK;

			continue;
		}
		//caso 3b: rotacao dupla leftuerda
		if(isLeftChildRb(new) && isRightChildRb(new->father)) {
            simpleRightRotationRb(root, new->father);
            simpleLeftRotationRb(root, new->father);
            new->right->color = RED;
            new->left->color = RED;
            new->color = BLACK;
            
            continue;

		}
	}
	//Após todas as colorreções a root pode ter ficado na color vermelha, portanto passamos ela novamente para color preta
	(*root)->color = BLACK;
}

/*
		(p)					(u)
	   /   \               /   \
	 (u)	(t3)  ==>	 (t1)	(p)
	/   \			            /  \   
 (t1)   (t2)		          (t2) (t3)                           
*/


void simpleRightRotationRb(rb *root, rb pivo) {
	rb p, u, t2;
	p = pivo;
	u = pivo->left;
	t2 = u->right;

	//para fazer a ligacao da root a sub-arvore com seu father,
	//eh preciso saber se o pivo era um filho leftuerdo ou righteito

	int posicao_pivo_left = isLeftChildRb(pivo);
	//pois ele precisa ser inserido em um dos lados do father

	//trocando o lado do t2
	p->left = t2;

	//se ele n for null, precisa linká-lo novamente
	if(t2 != NULL) 
		t2->father = pivo;

	//agora fazendo a rotação dos elements restantes
	u->right = pivo;
	u->father = p->father;
	p->father = u;

	p->color = RED;
	u->color = BLACK;

	//testando se u eh root, pois se for, a root da arvore recebe ele
	if(isRootRb(u))
		*root = u;

	//caso n seja, vamos descobrir se o pivo (valor que agr o u ocupa o lugar)
	//era filho leftuerdo ou n para posicioná-lo colorretamente
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

void preOrderRb (rb root) {
  if (root != NULL) {
      printElementRb(root);
      preOrderRb (root->left);
      preOrderRb (root->right);
    }
}

int searchIndexRb(rb root, char* difficulty) {
    if (root == NULL) {
        return -1; // Retorna -1 se a árvore estiver vazia
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