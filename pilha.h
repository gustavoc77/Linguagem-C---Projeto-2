#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <malloc.h>

#define MAX 5

/* Definindo o TIPOCHAVE */
typedef int TIPOCHAVE;
//typedef int 	bool; /* defini��o do tipo booleano */

/* Definindo o REGISTRO */
typedef struct   
{         
   TIPOCHAVE  chave; //Chave de identifica��o
   /* campos do registro */        
   char       NOMEPROD[100+1];
   char       DESCRICAO [200+1];
   char       PESO[5+1];
   float      PRECO; 
} 
ITEM;

/* Definindo o ELEMENTO */
typedef struct aux
{  
   ITEM item;
   struct 	    aux * PROX;  /* prox APONTA para o endere�o de mem�ria  */
}                                            /* de um elemento com essa mesma*/ 
ELEMENTO, * PONT;                                    /*  estrutura */ 

/* Definindo a PILHA */
typedef struct 
{
   PONT topo;    /* topo APONTA para o endere�o do ELEMENTO do TOPO da PILHA */
   int qtdItens;
   int nroPrat;
} 
PRATELEIRA;
/******************************************************************************/

//ROTINAS DE GERENCIAMENTO DA PILHA
void inicializarPilha (PRATELEIRA *, int);
void exibirPrateleira (PRATELEIRA *);
int inserirItemPrateleira(PRATELEIRA *, ITEM);
void newItemPrat(PRATELEIRA *);
int excluirItemPrateleira(PRATELEIRA *, ITEM *);
void adicionarAoCarrinho (PRATELEIRA *, PRATELEIRA *);
void reinicializarPilha (PRATELEIRA *);

//VARI�VEIS GLOBAIS

/**************************INICIALIZAR A ESTRUTURA*****************************/
/* 
Para inicializar uma pilha j� criada pelo usu�rio,  precisamos apenas acertar o 
 valor do campo topo.
J� que o topo conter� o endere�o do elemento que est� no topo da pilha e a pilha
est� vazia, iniciaremos esse campo com valor NULL. 
*/
void inicializarPilha (PRATELEIRA* p, int auxi)
{
  p->nroPrat = auxi;
  p->topo = NULL;
  p->qtdItens = 0;
  //printf("\n\nPrateleira %i criada!\n\n\t[PRESS. ENTER]", p->nroPrat);
  //getch();
  //sistem("cls");
}
/******************************************************************************/

/*************************EXIBIR OS ELEMENTOS DA ESTRUTURA*********************/
/* Para exibir os elementos da estrutura precisaremos  percorrer os elementos 
(iniciando pelo elemento do topo da pilha) e, por exemplo, imprimir suas chaves.
 */
void exibirPrateleira (PRATELEIRA* p) 
{  
   PONT end = p->topo;  
   if( (p->qtdItens == MAX) && (p->nroPrat != 20) ) printf("PRATELEIRA %i CHEIA===============================================\n", p->nroPrat);
   if( (p->qtdItens == 0) && (p->nroPrat != 20) ) printf("PRATELEIRA %i VAZIA===============================================\n", p->nroPrat);
   if(p->nroPrat == 20) printf("CARRINHO ========================================================\n", p->nroPrat);
   if( (p->qtdItens != 0) && (p->nroPrat != 20) && (p->qtdItens < MAX)) printf("PRATELEIRA %i ====================================================\n", p->nroPrat);
   while (end != NULL) 
   {
    printf("\n\t%s\n%i\t%s\n\t%s\n\tR$ %.2f\n", end->item.NOMEPROD, end->item.chave, end->item.DESCRICAO, end->item.PESO, end->item.PRECO);
    end = end->PROX;
   }
   printf("\n=================================================================\n\n");
}
/******************************************************************************/

/*****************INSERIR ELEMENTOS NA ESTRUTURA (PUSH)************************/
/* 
Inser��o de um elemento (push)
O usu�rio passa como par�metro um registro a ser inserido na pilha, o elemento 
ser� inserido no topo da pilha, ou  melhor, 
�acima� do elemento que est� no topo da  pilha.
O novo elemento ir� apontar para o elemento que estava no topo da pilha.
*/
int inserirItemPrateleira(PRATELEIRA* p, ITEM reg) 
{  
   if(p->qtdItens == MAX && p->nroPrat != 20){//verifica se n�o � o carrinho, pois ele n�o tem restri��es de quantidade
       printf("\n\nPRATELEIRA CHEIA!!!\n\n");
       return false;
       }
   PONT novo  = (PONT) malloc(sizeof(ELEMENTO));//Aloca um endere�o na mem�ria
   if(novo == NULL) return false;
   novo->item  = reg;//Insere o novo registro nesse endere�o
   novo->PROX = p->topo;//O pr�ximo do novo registro se torna 
                        //o at� ent�o primeiro
   p->topo    = novo;//O topo da lista se torna o novo endere�o alocado
   p->qtdItens++;
   if (p->nroPrat != 20) novo->item.chave = p->qtdItens;//a chave deve permanecer a mesma quando o item � inserido no carrinho
   return true;
}

void newItemPrat(PRATELEIRA *p)
{
    system("cls");// inserir novo registro na pilha
    ITEM novoItem;
    printf("\t\tINSERIR NOVO PRODUTO\n\n");
    printf("Nome do produto: ");
    fflush(stdin);
    gets(novoItem.NOMEPROD);
    printf("\nDescricao: ");
    gets(novoItem.DESCRICAO);
    printf("\nPeso: ");
    gets(novoItem.PESO);
    printf("\nPreco: ");
    scanf("%f", &novoItem.PRECO);
    printf("\n\n");
    if( inserirItemPrateleira(p, novoItem) )
    {
        printf("\nProduto inserido com sucesso\n\n\t\t[PRESS. ENTER]");
    }
    else{
        printf("Erro ao alocar novo produto\n\n\t\t[PRESS. ENTER]");
    }
}
/******************************************************************************/

/*********************EXCLUIR ELEMENTOS DA ESTRUTURA (POP)*********************/
/*
Exclus�o de um elemento (pop)
O usu�rio solicita a exclus�o do elemento do topo da pilha:
Se a pilha n�o estiver vazia, al�m de excluir esse elemento da pilha, iremos 
copi�-lo para um local indicado pelo usu�rio.
*/
/*Recebe a pilha da qual queremos inserir o primeiro elemento e,
o enadere�o para onde o endere�o excluido da pilha vai*/
int excluirItemPrateleira(PRATELEIRA* p, ITEM* reg) 
{  
   if ( p->topo == NULL)//Lista vazia: retorna false
      return false;
   *reg        = p->topo->item;//Copia o registro do topo para o endere�o de 
                              // mem�ria indicado pelo usu�rio
   PONT apagar = p->topo;//Copia o endere�o do elemento do topo para apagar
   p->topo     = apagar->PROX;//no campo topo coloca o endere�o do pr�ximo
                              // elemento
   free(apagar);//libera a mem�ria do elemento que era o topo.
   p->qtdItens--;
   return true;
}

void adicionarAoCarrinho (PRATELEIRA * p, PRATELEIRA * carrinho)
{
    ITEM addCar;
    excluirItemPrateleira(p, &addCar);
    if(inserirItemPrateleira(carrinho, addCar))
    {
        printf("\n\nAdicionado ao carrinho com sucesso!\n\n");
    }
    else{
        printf("\n\nAdicionado ao carrinho com sucesso!\n\n");
        }
}
/******************************************************************************/

/*************************REINICIALIZAR A ESTRUTURA****************************/
/*
S� tem sentido, se a pilha foi inicializada alguma vez.
Para reinicializar a pilha, precisamos excluir todos os seus elementos e colocar
 NULL no campo topo.
*/
void reinicializarPilha (PRATELEIRA* p) 
{
   PONT apagar;//Registro atual que ser� apagado
   PONT posicao = p->topo;//Recebe o topo da lista
   while (posicao != NULL)//percorre toda a lista
   {
      apagar  = posicao;//A posi��o atual vai para apagar
      posicao = posicao->PROX;//posi��o recebe o endere�o do pr�ximo
      free(apagar);//a posi��o atual � apagada
   }
   p->topo = NULL;//percorreu toda a lista: o topo recebe NULL
   p->qtdItens = 0;
   //p->nroPrat = 0;
}
/******************************************************************************/
