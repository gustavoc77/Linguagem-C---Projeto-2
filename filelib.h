#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#define NOME_ARQUIVO     "USUARIOS.DAT"
#define NOME_ARQUIVO_TMP "USUARIOS.TMP"
#define false            0
#define true             1

typedef int 	bool;

typedef  struct
{
    char    nome[40],
    		prontuario[10];
    bool     repositor;
}
registro;

void     cadastra_registro    (registro);
void     consulta_registros   (void);
int      quantidade_registros (void);
void     deleta_registro      (void);
void     altera_registro      (void);
int      seleciona_registro   (void);
void     troca_registros      (int, int);
registro retorna_registro     (int);
int      particiona           (int, int);
void     quick_sort	          (int, int);
int      busca_binaria        (registro proc, int *);

FILE * ARQ;

void    cadastra_registro(registro novo_registro)
{   
    strcpy(novo_registro.nome, strupr(novo_registro.nome));
    strcpy(novo_registro.prontuario, strupr(novo_registro.prontuario));
    novo_registro.repositor = false;                      
    ARQ = fopen ( NOME_ARQUIVO , "a" );

    if(ARQ == NULL)
    {
       system("cls");
       system("color 04");
       printf("\n\n\tARQUIVO USUARIOS.DAT INEXISTENTE!");
       getch();
       return;          
    }
    
    fwrite ( &novo_registro, sizeof(registro), 1, ARQ );
    fclose ( ARQ );
    quick_sort( 1, quantidade_registros() );//Ordena arquivo
}

void consulta_registros(void)
{                          
    registro    reg_lido;
    int         numero_registro=0;

    ARQ = fopen ( NOME_ARQUIVO , "r" ); 
    
    if(ARQ == NULL)
    {
       system("cls");
       system("color 04");
       printf("\n\n\tARQUIVO USUARIOS.DAT INEXISTENTE!");
       getch();
       return;          
    }

    while ( !feof(ARQ) )
    {
        
       fread ( &reg_lido, sizeof(reg_lido), 1, ARQ );
       if ( !feof(ARQ) )
       {
            numero_registro++;
            printf ("%i\tNOME: %s\n\tPRONTUARIO: %s\n\tREPOSITOR: ", numero_registro, reg_lido.nome, reg_lido.prontuario);
            if(reg_lido.repositor) printf("SIM\n\n");
            else    printf("NAO\n\n");
       }
    }

    fclose ( ARQ );
}

int quantidade_registros (void)
{
    registro    temp;
    int         nrRegistros;
    
    ARQ = fopen ( NOME_ARQUIVO , "r" );
    
    if(ARQ == NULL)
    {
       system("cls");
       system("color 04");
       printf("\n\n\tARQUIVO USUARIOS.DAT INEXISTENTE!");
       getch();
       return 0;          
    }

    fseek(ARQ, 0L, SEEK_END); // VAI ATÉ O FIM
    nrRegistros = ftell(ARQ) / sizeof(registro);//NUMERO DE BYTES ALOCADOS
    fclose ( ARQ );
    return nrRegistros;
}


void    deleta_registro(void)
{
    FILE      * ARQ_TMP;
    registro  reg_lido;
    int       numero_registro=0;
    int       registro_a_deletar;
    char SN;
    
    registro_a_deletar = seleciona_registro();
    
    printf("\n\nDeseja realmente deletar o registro %i? [S/N] ", registro_a_deletar);
    
    do
    {
       SN = getche();
    }while(SN != 'S' && SN != 's' && SN != 'n' && SN != 'N');
    
    if(SN == 'N' || SN == 'n') return;
    
    ARQ = fopen ( NOME_ARQUIVO , "r" ); 
    if ( ARQ == NULL )
    {
        system("cls");
        system("color 04");
        printf("\n\n\tARQUIVO USUARIOS.DAT INEXISTENTE!");
        getch();
        return;
    }

    ARQ_TMP = fopen ( NOME_ARQUIVO_TMP , "w" );
    if(ARQ_TMP == NULL)
    {
        printf ("\n\nERRO AO ABRIR ARQUIVO AUXILIAR PARA DELETAR!");
        getch();
        return;          
    }
    numero_registro=0;
    
    while ( !feof(ARQ) )
    {
        
        fread ( &reg_lido, sizeof(reg_lido), 1, ARQ );
        
        if ( !feof(ARQ) )
        {
            numero_registro++;
            if ( registro_a_deletar != numero_registro )
            {
                fwrite ( &reg_lido, sizeof(reg_lido), 1, ARQ_TMP );
            }
        }
    }
    
    fclose ( ARQ );
    fclose ( ARQ_TMP );
    system ("DEL  USUARIOS.DAT");
    system ("REN  USUARIOS.TMP USUARIOS.DAT");
    printf ("\n\nRegistro %i removido com sucesso!", registro_a_deletar );
    printf ("\n\n\t\t[PRESS. ENTER]");
    getch();
}

void    altera_registro(void)
{
    registro    reg_lido;
    int         numero_registro=0;
    int         registro_a_alterar;
    
    registro_a_alterar = seleciona_registro();

    ARQ = fopen ( NOME_ARQUIVO , "r+" );
    
    if(ARQ == NULL)
    {
       system("cls");
       system("color 04");
       printf("\n\n\tARQUIVO USUARIOS.DAT INEXISTENTE!");
       getch();
       return;          
    }
    
    fseek(ARQ,(registro_a_alterar-1)*sizeof(reg_lido),SEEK_SET);
    
    if ( fread(&reg_lido, sizeof(reg_lido),1, ARQ)!=1 )
    {
         printf("\nErro ao ler registro!");
         getch();
         return;         
    }   

    fclose (ARQ);
    
    printf ("\n\nRegistro atual:\n%i.\tNOME [%s]\n\tPRONTUARIO [%s]\n", registro_a_alterar , reg_lido.nome, reg_lido.prontuario);
    if(reg_lido.repositor)   printf("\tREPOSITOR: SIM\n");
    else    printf("\tREPOSITOR: NAO\n");
    printf ("\nNovos valores:\nNOME: "); fflush(stdin); gets(reg_lido.nome);
    printf ("\nPRONTUARIO: "); fflush(stdin); gets(reg_lido.prontuario);
    printf ("\nPERMISSOES DE REPOSITOR [SIM = 1 / NAO = 0]: ");
    
    do
    {
       fflush(stdin);
       scanf("%i", &reg_lido.repositor);
    }while(reg_lido.repositor != 1 && reg_lido.repositor != 0);

    strcpy(reg_lido.nome, strupr(reg_lido.nome));
    strcpy(reg_lido.prontuario, strupr(reg_lido.prontuario));
    
    ARQ = fopen ( NOME_ARQUIVO , "r+" );
    if(ARQ == NULL)
    {
       system("cls");
       system("color 04");
       printf("\n\n\tARQUIVO USUARIOS.DAT INEXISTENTE!");
       getch();
       return;          
    }
    
    fseek(ARQ,(registro_a_alterar-1)*sizeof(reg_lido),SEEK_SET);        
    
    if ( fwrite(&reg_lido, sizeof(registro),1, ARQ)!=1 )
    {
         printf("\nErro ao regravar NOME: %s\n\tPRONTUARIO: %s ...", reg_lido.nome, reg_lido.prontuario);
         getch();         
    }
    
    fclose ( ARQ );

    printf ("\n\nRegistro %i alterado com sucesso!", registro_a_alterar );
    quick_sort( 1, quantidade_registros() );//Ordena arquivo    
}

int seleciona_registro (void)
{
    registro    reg_lido;
    int         registro_selecionado,
                numero_registro;
    
    consulta_registros();
    
    numero_registro = quantidade_registros();
    
    do
    {
        printf ("\nNumero do cadastro que deseja selecionar: ");
        fflush (stdin);     scanf  ("%i", &registro_selecionado);
    }
    while ( registro_selecionado < 1 || registro_selecionado > numero_registro );
    
    return registro_selecionado;
}

void troca_registros (int A, int B)
{
     registro reg_a,
              reg_b;

     ARQ = fopen ( NOME_ARQUIVO , "r+" );
    
     if(ARQ == NULL)
    {
       system("cls");
       system("color 04");
       printf("\n\n\tARQUIVO USUARIOS.DAT INEXISTENTE!");
       getch();
       return;          
    }
    
     fseek(ARQ,(A-1)*sizeof(registro),SEEK_SET);
    
     if ( fread(&reg_a, sizeof(registro),1, ARQ)!=1 )
     {
          printf("\nErro ao ler registro!");
          getch();
          return;         
     }
     
     fseek(ARQ,(B-1)*sizeof(registro),SEEK_SET);
     
     if ( fread(&reg_b, sizeof(registro),1, ARQ)!=1 )
     {
          printf("\nErro ao ler registro!");
          getch();
          return;         
     }
     
     fseek(ARQ,(A-1)*sizeof(registro),SEEK_SET);        
    
     if ( fwrite(&reg_b, sizeof(registro),1, ARQ)!=1 )
     {
          printf("\nErro ao regravar");
          getch();         
     }
     
     fseek(ARQ,(B-1)*sizeof(registro),SEEK_SET);        
    
     if ( fwrite(&reg_a, sizeof(registro),1, ARQ)!=1 )
     {
          printf("\nErro ao regravar");
          getch();         
     }

     fclose (ARQ);
}

registro retorna_registro (int posi)
{
     registro reg_lido;
     
     ARQ = fopen ( NOME_ARQUIVO , "r" );
    
     if(ARQ == NULL)
    {
       system("cls");
       system("color 04");
       printf("\n\n\tARQUIVO USUARIOS.DAT INEXISTENTE!");
       getch();
       exit(0);    
    }
     
     fseek(ARQ,(posi-1)*sizeof(registro),SEEK_SET);
     
     if ( fread(&reg_lido, sizeof(registro),1, ARQ)!=1 )
     {
          printf("\nErro ao ler registro!");
          getch();
          exit(0);
     }
     
     fclose (ARQ);
     
     return reg_lido;
}

int particiona (int ini, int pivo) 
{    
    int   i;
    int   p_maior = ini;  
    registro auxiliar_1,
             auxiliar_2;
    
    for ( i=ini; i<pivo; i++ )
    {  
       auxiliar_1 = retorna_registro(i);
       auxiliar_2 = retorna_registro(pivo);
       if ( 0 > strcmp(auxiliar_1.nome, auxiliar_2.nome) )
       {            
            troca_registros(  i, p_maior );
            p_maior++;
       }    
   }     
  
   troca_registros(  p_maior, pivo ); 
   return p_maior;
}


void quick_sort	(int ini, int fim) 
{    
   int pivo;
   if ( ini<fim )
   {        
        pivo = particiona (ini, fim );
        quick_sort(ini,    pivo-1 );
        quick_sort(pivo+1, fim);
   } 
}

int busca_binaria (registro proc, int * posi)
{
     registro * vetor;
     int tamanho,
         ini = 0,
         meio,
         iteracoes = 0;
         
     strcpy(proc.nome, strupr(proc.nome));
     strcpy(proc.prontuario, strupr(proc.prontuario));
     
     tamanho = quantidade_registros();
     ARQ = fopen ( NOME_ARQUIVO , "r" );
     if(ARQ == NULL)
    {
       system("cls");
       system("color 04");
       printf("\n\n\tARQUIVO USUARIOS.DAT INEXISTENTE!");
       getch();
       exit(0);
       return;          
    }
     
     vetor = (registro *) malloc (tamanho * sizeof(registro));
     fread ( vetor, tamanho * sizeof(registro), 1, ARQ );

     if(vetor == NULL)
     {
        system("cls");
        system("color 04");
        //system("MODE 7 50")
        printf("\n\n\tmalloc devolveu NULL!");
        getch();
        exit(0);
     }

     fclose(ARQ);
             
     while (ini<=tamanho)    
     {        
         meio=(ini+tamanho)/2; /* descobre qual é a posição do meio do vetor     */

	     if( ( strcmp(proc.nome, vetor[meio].nome) ) == 0 && ( strcmp(proc.prontuario, vetor[meio].prontuario) ) == 0){/* se o elemento do meio do vetor for o buscado...*/           
             free(vetor);     /* ...libera memória      */
             * posi = meio;
             return true;//...retorna 1
         }
         if( (strcmp(vetor[meio].nome, proc.nome)) > 0) /* se o elemento do meio do vetor for maior que o buscado...*/        
             tamanho=meio-1;      /* ...o que se busca está ANTES do meio; logo o novo fim do vetor será uma posição antes do meio*/  
         else              /* senão o que se busca está DEPOIS do meio...*/
             ini=meio+1;      /* ... logo o novo início do vetor será uma posição depois do meio */  
     }

     free(vetor);
     return false; /* só retorna 0 quando fez sucessivas divisões do vetor e NÃO ACHOU o que se busca */
}
