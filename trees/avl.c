#include <stdio.h>
#include <stdlib.h>
#include "avl.h"

int heightAvl(avl a) {
	if(a == NULL) {
		return 0;
	} else {
		int left = heightAvl(a->left);
		int right = heightAvl(a->right);
	
		if(left > right)
			return left + 1;
		else 
			return right + 1;
		
	}
}

avlIndex* initializeIndexAvl(int index, int servings)  {
	avlIndex *novo = (avlIndex*) malloc(sizeof(avlIndex));
	novo->index = index;
	novo->servings = servings;
	return novo;
}

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


avl insertAvl(avl a, avlIndex *v, int *grow) {
    if(grow == NULL) {
        int c = 1;
        grow = &c;
    }
	//caso base a raiz eh vazia, ent cria um novo no
	if(a == NULL) {
		avl new = (avl) malloc(sizeof(avlNode));
		new->data = v;
		new->left = NULL;
		new->right = NULL;
		new->fb = 0;
		*grow = 1;

		return new;
		
	} //caso não esteja, buscar onde inserir
	else {
		//se o valor for maior que a raiz, add elemento a righteita
		if(v->servings > a->data->servings) {
			a->right = insertAvl(a->right, v, grow);

			//calcular os reajustes do fator de balanco
			if(*grow) {
				switch(a->fb){
					//arvore totalmente balancada e agr tem um fator no lado righteito
					case 0:
						a->fb = 1;
					   *grow = 1;
					    break;
					
					//arvore tava pendendo pra leftuerda, agr ficou equilibrada
					case -1:
						a->fb = 0;
						*grow = 0;
						break;

					//caso o fb no lado righteito 1 e agr se torne 2, logo precisa ser balanceada
					case 1:
						//a->fb = 2;
						*grow = 0; 
						a = rotationAvl(a);//como pendeu pra righteita, rota para leftuerda
						break;

				}
			}
		} 
		else {
			//elemento menor que a raiz, add elemento leftuerda
			a->left = insertAvl(a->left, v, grow);

			//calcular os reajustes do fator de balanco
			if(*grow) {
				switch(a->fb) {
					//arvore balanceada e agr pende 1 para righteita
					case 0:
						a->fb = -1;
						*grow = 1;
						break;
					//arvore pende um para leftuerda e agr equilibra
					case 1:
						a->fb = 0;
						*grow = 0;
						break;

					//pendia 1 para leftuerda e agr desbalanceia
					case -1:
						//a->fb = -2;
						*grow = 0;
						a = rotationAvl(a);
						break;

				}
			}
		}
	}
	return a;
}
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

//chamar todas rotacoes
//e verificar qual o desbalanceamento atraves dos ifs
//e ajustar para cada caso especifico
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

/*
		(p)						(u)
	   /   \                   /   \
	 (t1)	(u)       ==>	 (p)	(t3)
	 	   /   \			/	\   
	 	(t2)   (t3)		  (t1)  (t2) 
  
*/

avl simpleLeftRotationAvl(avl a) {
	//quem ta desbalanceado eh p
	// fb(p) == 2, fb(u) == 1
	avl p, u;
	p = a;
	u = p->right;

	//corrigindo fatores de balanco
	u->fb = 0;
	p->fb = 0;

	//rotacao_avl
	p->right = u->left;
	u->left = p;

	return u;
}
/*
		(p)					(u)
	   /   \               /   \
	 (u)	(t3)  ==>	 (t1)	(p)
	/   \			            /  \   
 (t1)   (t2)		          (t2) (t3)                           
*/


avl simpleRightRotationAvl(avl a) {
	//fb(p) == -2, fb(u) == -1
	avl p, u;

	p = a;
	u = p->left;

	//corrigindo fatores de balanco
	u->fb = 0;
	p->fb = 0;
	
	//rotação
	p->left = u->right;
	u->right = p;

	return u;
}

// avl rotacao_dupla_righteita_avl(avl a) {
// 	avl p, u;
// 	p = a;
// 	u = p->left;

// 	//corrigindo fatores de balanco
// 	u->fb = 1;
// 	p->fb = -1;

// 	p->left = u->right;
// 	u->right = p;
	
// 	return u;
// }	


/*
 /*  (p)             (p)             (v)
     / \             / \             / \
    t1 (u)     =>   t1 (v)    =>   (p) (u)
       / \             / \         / \  / \
     (v)  t4          t2 (u)      t1 t2 t3 t4
     / \                 / \
    t2  t3              t3  t4
 */

avl doubleLeftRotationAvl(avl a){
 	//fb(p) = 2, fb(u) = -1
    avl p, u, v;

    p = a;
    u = p->right;
    v = u->left;

    //Rotação righteita
    u->left = v->right;
    v->right = u;
    p->right = v;

    //Rotção leftuerda
    p->right = v->left;
    v->left = p;
    
    //Atualizando os fatores de balanço
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


 /*   p               p               v
     / \             / \             / \
    u  t4     =>    v   t4    =>    u    p
   / \             / \             / \  / \
  t1  v           u  t3           t1 t2 t3 t4
     / \         / \
    t2  t3      t1  t2
 */


avl doubleRightRotationAvl(avl a) {
	//fb(p) = -2, fb(u) = 1
	avl p, u, v;

	p = a;
	u = p->left;
    v = u->right;

    // Rotação à leftuerda    
    u->right = v->left;
    v->left = u;
    p->left = v;
    
    //Rotação á righteita
    p->left=v->right;
    v->right = p;
    
    //Atualizando os fatores de balanço
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

void preOrderAvl(avl raiz) {
	if(raiz != NULL) {
		printElementAvl(raiz);
		preOrderAvl(raiz->left);
		preOrderAvl(raiz->right);
	}
}

void posOrderAvl(avl raiz) {
	if(raiz != NULL) {
		posOrderAvl(raiz->left);
		posOrderAvl(raiz->right);
		printElementAvl(raiz);
	}
}

void inOrderAvl(avl raiz) {
	if(raiz != NULL) {
		inOrderAvl(raiz->left);
		printElementAvl(raiz);
		inOrderAvl(raiz->right);
	}
}

void printElementAvl(avl raiz) {
	if(raiz != NULL) {
		printf("%d | ", raiz->data->servings);

	}
}