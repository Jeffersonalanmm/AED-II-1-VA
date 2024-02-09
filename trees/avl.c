#include <stdio.h>
#include <stdlib.h>
#include "avl.h"

int altura_avl(avl a) {
	if(a == NULL) {
		return 0;
	} else {
		int left = altura_avl(a->left);
		int right = altura_avl(a->right);
	
		if(left > right)
			return left + 1;
		else 
			return right + 1;
		
	}
}

avlIndex* inicializar_indice_avl(int index, int difficulty)  {
	avlIndex *novo = (avlIndex*) malloc(sizeof(avlIndex));
	novo->index = index;
	novo->difficulty = difficulty;
	return novo;
}

int buscar_indice_avl(avl raiz, int difficulty) {
	if(raiz != NULL) {
		if(raiz->data->difficulty == difficulty) {
			return raiz->data->index;
		}
		else {
			if(raiz->data->difficulty > difficulty)
				return buscar_indice_avl(raiz->left, difficulty);
			else 
				return buscar_indice_avl(raiz->right, difficulty);
		}
	} else {
		return -1;
	}
}


avl adicionar_avl(avl a, avlIndex *v, int *cresceu) {
    if(cresceu == NULL) {
        int c = 1;
        cresceu = &c;
    }
	//caso base a raiz eh vazia, ent cria um novo no
	if(a == NULL) {
		avl new = (avl) malloc(sizeof(avlNode));
		new->data = v;
		new->left = NULL;
		new->right = NULL;
		new->fb = 0;
		*cresceu = 1;

		return new;
		
	} //caso não esteja, buscar onde inserir
	else {
		//se o valor for maior que a raiz, add elemento a righteita
		if(v->difficulty > a->data->difficulty) {
			a->right = adicionar_avl(a->right, v, cresceu);

			//calcular os reajustes do fator de balanco
			if(*cresceu) {
				switch(a->fb){
					//arvore totalmente balancada e agr tem um fator no lado righteito
					case 0:
						a->fb = 1;
					   *cresceu = 1;
					    break;
					
					//arvore tava pendendo pra leftuerda, agr ficou equilibrada
					case -1:
						a->fb = 0;
						*cresceu = 0;
						break;

					//caso o fb no lado righteito 1 e agr se torne 2, logo precisa ser balanceada
					case 1:
						//a->fb = 2;
						*cresceu = 0; 
						a = rotacao_avl(a);//como pendeu pra righteita, rota para leftuerda
						break;

				}
			}
		} 
		else {
			//elemento menor que a raiz, add elemento leftuerda
			a->left = adicionar_avl(a->left, v, cresceu);

			//calcular os reajustes do fator de balanco
			if(*cresceu) {
				switch(a->fb) {
					//arvore balanceada e agr pende 1 para righteita
					case 0:
						a->fb = -1;
						*cresceu = 1;
						break;
					//arvore pende um para leftuerda e agr equilibra
					case 1:
						a->fb = 0;
						*cresceu = 0;
						break;

					//pendia 1 para leftuerda e agr desbalanceia
					case -1:
						//a->fb = -2;
						*cresceu = 0;
						a = rotacao_avl(a);
						break;

				}
			}
		}
	}
	return a;
}
avl remover_elemento_avl(avl a, int v, int *diminuiu) {
//caso base: raiz vazia
	if(a == NULL) {
        *diminuiu = 0;
        return a;
    }

    if(diminuiu == NULL) {
        int d = 0;
        diminuiu = &d;
    }

  //valor encontrado, hora de remover
  	if(a->data->difficulty == v) {
    		if(a->left == NULL && a->right == NULL) {
    			free(a);
    			*diminuiu = 1;
    			return NULL;
    		}
    		//dois filhos
    		if(a->left != NULL && a->right != NULL) {
    			avl aux = a->left;
    			//procura o sucesso para substituir o valor a ser removido
    			while(aux->right != NULL)
    				aux = aux->right;

    			a->data = aux->data;

    			a->left = remover_elemento_avl(a->left, aux->data->difficulty, diminuiu);

                switch(a->fb){
                        case 0:
                            a->fb = 1;
                            *diminuiu = 0;
                            break;
                        case -1:
                            a->fb = 0;
                            *diminuiu = 1;
                            break;
                        case 1:
                            *diminuiu = 1;
                            a = rotacao_avl(a);
                    }
                    return a;

    		}
    		else { //caso a raiz so tenha um filho
    			avl aux;
    			*diminuiu = 1;
    			if(a->right != NULL && a->left == NULL){
    				aux = a->right;
    			}
    			else {
    				aux = a->left;
    				
    			}
    			free(a);
    			return aux;
    		} 
		}


		else {
			if(v > a->data->difficulty) {
		    		a->right = remover_elemento_avl(a->right, v, diminuiu);

		    		if(*diminuiu) 
		    		{
		    			switch(a->fb)
		    			{	
		    				//raiz pendendo leftuerda, agr que removeu ela, gerou -2. Ou seja
		    					//rotacao_avl
		    				case -1:
		    					a->fb = -2;
		    					if(a->left->fb == 0)
		    					{
		    						*diminuiu = 0;
		    					}
		    					else { 
		    						*diminuiu = 1; 
		    					}
		    					a = rotacao_avl(a);
		    					break;

		    				//raiz balanceadaaaaaaaa, agr pende pra leftuerda
		    				case 0:
		    					a->fb = -1;
		    					*diminuiu = 0;
		    					break;

		    				//
		    				case 1:
		    					a->fb = 0;
		    					*diminuiu = 1;
		    					break;
		    			}
		    		}
		    	}
		    else {
		    		a->left = remover_elemento_avl(a->left, v, diminuiu);

		    		if(*diminuiu) 
		    		{
		    			switch(a->fb) 
		    			{
		    				//raiz estava pendendo pra leftuerda, agr equilibrou
		    				case -1:
		    					a->fb = 0;
		    					*diminuiu = 1;
		    					break;

		    				//a raiz estava balanceada, apos remover ela passa a pendera righteita
		    				case 0:
		    					a->fb = 1;
		    					*diminuiu = 0;
		    					break;

		    				//a raiz pendia para righteita, apos remover outro na leftuerda, ela passa
		    					//a a pender 2. Logo precisa ser rotacionada
		    				case 1:
		    					a->fb = 2;
		    					if(a->right->fb == 0){
		    						*diminuiu = 0;
		    					}
		    					else { *diminuiu = 1; }

		    					a = rotacao_avl(a);//rotacao_avl leftuerda
		    					break;
		    			}
		    		}
		    	}
		}
		    	//valor a ser removido maior que a raiz
    	
	return a;

}
//chamar todas rotacoes
//e verificar qual o desbalanceamento atraves dos ifs
//e ajustar para cada caso especifico
avl rotacao_avl(avl a){
    if (a->fb > 0){
        if(a->right->fb >= 0){
            return rotacao_simples_leftuerda_avl(a);
        }else{
            return rotacao_dupla_leftuerda_avl(a);
        }
    }else{
        if (a->left->fb <= 0){
            return rotacao_simples_righteita_avl(a);
        }else{
            return rotacao_dupla_righteita_avl(a);
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

avl rotacao_simples_leftuerda_avl(avl a) {
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


avl rotacao_simples_righteita_avl(avl a) {
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

avl rotacao_dupla_leftuerda_avl(avl a){
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


avl rotacao_dupla_righteita_avl(avl a) {
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

void pre_order_avl(avl raiz) {
	if(raiz != NULL) {
		imprimir_elemento_avl(raiz);
		pre_order_avl(raiz->left);
		pre_order_avl(raiz->right);
	}
}

void pos_order_avl(avl raiz) {
	if(raiz != NULL) {
		pos_order_avl(raiz->left);
		pos_order_avl(raiz->right);
		imprimir_elemento_avl(raiz);
	}
}

void in_order_avl(avl raiz) {
	if(raiz != NULL) {
		in_order_avl(raiz->left);
		imprimir_elemento_avl(raiz);
		in_order_avl(raiz->right);
	}
}

void imprimir_elemento_avl(avl raiz) {
	if(raiz != NULL) {
		printf("%d | ", raiz->data->difficulty);

	}
}