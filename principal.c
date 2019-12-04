/*BIBLIOTECAS*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <locale.h>
#include "filelib.h"
#include "pilha.h"
//#include <malloc.h>

#define MAX_PILHAS 10

/*TIPOS PRÉ-DEFINIDOS*/
registro solicita_reg (void);
void menu_gerenciar_usuarios (void);
void menu_opcoes(void);
void menu_gondolas (void);
void inicia_prateleiras (void);
void mostrar_prateleiras(void);
void reiniciar_tudo(void);

/*VARIÁVEIS GLOBAIS*/
registro usuario_logado;
PRATELEIRA prat[MAX_PILHAS],
           car;

registro solicita_reg (void)
{   
    registro lido;

	printf("NOME DO USUARIO: ");
	gets(lido.nome);

	printf("\nPRONTUARIO DO USUARIO: ");
	gets(lido.prontuario);

	return lido;
}

void menu_gerenciar_usuarios (void)
{
    char opc;
    do{
        opc = 0;
        do{   system ("cls");
              printf ("            SISTEMA GERENCIADOR DE SUPERMERCADO\n");
              printf ("=========== MENU DE GERENCIAMENTO DE USUARIOS ==============");
              printf ("\n\t\t1. cadastro");
              printf ("\n\t\t2. consulta");
              printf ("\n\t\t3. excluir registro");
              printf ("\n\t\t4. alterar registro");
              printf ("\n\t\tR. voltar ao menu inicial");    
              printf ("\n============================================================");
              printf ("\n\t\tEscolha: "); fflush(stdin); opc = getche();
        }while ( opc != '1' && 
                 opc != '2' && 
                 opc != '3' && 
                 opc != '4' && 
                 opc != '5' &&
                 opc != 'r' && 
                 opc != 'R' );
					    
         switch ( opc )
         {
             case '1' : system ("cls");
                        printf("==================CADASTRO DE USUARIO================\n\n");
                        cadastra_registro( solicita_reg() );
                        printf("\n\nREGISTRO CADASTRADO COM SUCESSO\n\n\t\t[PRESS. ENTER]");
                        getch();
                        break;
                                         
             case '2' : system ("cls");
                        printf ("=============== REGISTROS CADASTRADOS ==============\n\n");
                        consulta_registros();
                        printf ("\t\t[PRESS. ENTER]");
                        getch();
                        system ("cls");
                        break;
                                         
             case '3' : system ("cls");
                        printf("====================DELETAR CADASTRO=================\n");
                        deleta_registro();
                        break;
                                         
             case '4' : system ("cls");
                        printf("====================ALTERAR CADASTRO=================\n");
                        altera_registro();
                        printf ("\n\n\t\t[PRESS. ENTER]");
                        getch();
                        system ("cls");
                        break;
                                         
             case 'r' : 
             case 'R' : break;        
         }
         system ("cls");
       }while ( opc != 'r' && opc != 'R');
}

void menu_opcoes(void)
{
	system("color 70");
	char opc;
	do{
    	do{
            printf("            SISTEMA GERENCIADOR DE SUPERMERCADO\n");
    	    printf("======================MENU DE OPCOES=======================");
    	    printf("\n\t\t1.Gondolas");
    	    printf("\n\t\t2.Caixa/PDV (nao implementado)");
    	    printf("\n\t\t3.Entregas a domicilio (nao implementado)");
    	    printf("\n\t\t4.Gerenciar usuarios (apenas repositores)");
    	    printf("\n\t\tS.Sair");
    	    printf ("\n============================================================");
    	    printf("\n\t\tEscolha: "); fflush(stdin);opc = getche ();
    	    system("cls");
    	}while ( (opc != '1' &&
                  opc != '2' && 
                  opc != '3' &&
                  opc != '4' &&
                  opc != 's' &&
                  opc != 'S') ||
                 ((opc == '4') && (usuario_logado.repositor == false)));//restrições de acesso
                
    	switch(opc)
    	{   
    	    case '1': menu_gondolas();
                      break;
    	    
    	    case '2': break;
    	    
    	    case '3': break;
    	    
    	    case '4': menu_gerenciar_usuarios();
                      break;
            case 's': 
            case 'S': break;
    	}
	}while ( opc != 's' && opc != 'S');
}

void menu_gondolas (void)
{
    char opc;
    int auxiliar=0;
    do{
        opc = 0;
        do{   system ("cls");
              printf ("            SISTEMA GERENCIADOR DE SUPERMERCADO\n");
              printf ("========================== GONDOLAS ==========================");
              printf ("\n\t\t1. mostrar carrinho");
              printf ("\n\t\t2. abastecer prateleiras (apenas repositores)");
              printf ("\n\t\t3. adicionar produto ao carrinho");
              printf ("\n\t\t4. mostrar prateleiras");
              printf ("\n\t\t5. reiniciar tudo");
              printf ("\n\t\tQ. voltar ao menu anterior");    
              printf ("\n============================================================");
              printf ("\n\t\tEscolha: "); fflush(stdin); opc = getche();
        }while ( (opc != '1' && 
                 opc != '2' && 
                 opc != '3' && 
                 opc != '4' && 
                 opc != '5' &&
                 opc != 'q' && 
                 opc != 'Q') ||
                 ( (opc == '4') && (usuario_logado.repositor == false) ) );//Restrição de acesso
					    
         switch ( opc )
         {
             case '1' : system ("cls");//mostrar carrinho
                        exibirPrateleira(&car);
                        printf("\t\t[PRESS. ENTER]");
                        getch();
                        break;
                                         
             case '2' : system ("cls");//abastecer prateleira
                        mostrar_prateleiras();
                        printf("\nDigite a prateleira que sera abastecida: ");
                        scanf("%i", &auxiliar);
                        newItemPrat(&prat[auxiliar]);
                        getch();
                        break;
                                         
             case '3' : system ("cls");//Pegar produto da prateleira
                        mostrar_prateleiras();
                        printf("\nDigite o numero da prateleira de onde sera retirado o produto: ");
                        scanf("%i", &auxiliar);
                        adicionarAoCarrinho (&prat[auxiliar], &car);
                        printf ("\t\t[PRESS. ENTER]");
                        getch();
                        break;
                        
             case '4' : system ("cls");//Mostrar prateleira
                        mostrar_prateleiras();
                        printf ("\t\t[PRESS. ENTER]");
                        getch();
                        break;
                        
             case '5' : system ("cls");//Reiniciar tudo
                        reiniciar_tudo();
                        printf ("\t\t[PRESS. ENTER]");
                        getch();
                        break;

             case 'r' : 
             case 'R' : break;        
         }
         system ("cls");
       }while ( opc != 'q' && opc != 'Q');
}

void inicia_prateleiras(void)
{
    int aux=0;
    for(aux=0; aux<MAX_PILHAS; aux++){
               inicializarPilha(&prat[aux], aux);
               }
    inicializarPilha(&car, 20);// a pilha com nroPrat=20 diferencia o carrinho das outras pilhas (prateleiras)
}

void mostrar_prateleiras(void)
{
    int aux=0;
    for(aux=0; aux<MAX_PILHAS; aux++){
               exibirPrateleira(&prat[aux]);
               }
}

void reiniciar_tudo(void)
{
    int aux=0;
    for(aux=0; aux<MAX_PILHAS; aux++){
               reinicializarPilha(&prat[aux]);
               }
    reinicializarPilha(&car);
}

/*CORPO DO PROGRAMA*/
int main(){
    int aux=0,
        posi_user_log;
    system("color 90");
    usuario_logado = solicita_reg();
    aux = busca_binaria( usuario_logado , &posi_user_log);
    usuario_logado = retorna_registro(posi_user_log+1);
	if( aux != 0){
        inicia_prateleiras();
        system("cls");
	    menu_opcoes();
     }
    else{
        system("cls");
        system("color 40");
        printf("\n\n\tUSUARIO E/OU PRONTUARIO INVALIDO!");
        getch();
        }
	exit(0);
}
