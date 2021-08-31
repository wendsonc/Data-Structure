
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>

#define SIZE 50273
#define CONST 11

struct Node{
char palavra[100];
struct Node* next;
};
void InsertAfter(struct Node *n, char *value){
  struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));
  strcpy(new_node->palavra,value);
  new_node->next = n->next;
  n->next = new_node;
}

void TabelaHash(struct Node *hash[], char entrada[], int chave){ 
	struct Node* aux;

	if(hash[chave]==NULL){
		hash[chave] = (struct Node*) malloc(sizeof(struct Node));
		strcpy(hash[chave]->palavra, entrada); 
		hash[chave]->next = NULL;
		
	}else{
		aux = (struct Node*) malloc(sizeof(struct Node));
		strcpy(aux->palavra,entrada);
		aux->next = hash[chave];
		hash[chave] = aux;
		
	}
}

int Pesquisa(struct Node *hash[],char *entrada,long unsigned int chave){
	struct Node *aux;
	int x=0,y=0;
	aux = hash[chave];
	
	while(aux != NULL){
		if(strcmp(entrada,aux->palavra)==0){
			x = 1;
			return 0;
		}
		aux=aux->next;
	}
		return 1;

}
unsigned long int Chave(int keysize, unsigned char *word){
	int i;
	unsigned long key=5381;
	

	while (i = *word++){
		key = ((key<<5)+key)+i;/* hash * 33 + c */

	}
	
	key %= SIZE;
	
	return key; 
	
}
  
int main(void){
  FILE *dicionario, *teste,*resultado;
	struct Node *hash[SIZE],*palavrasErd;
  palavrasErd = (struct Node *)malloc(sizeof(struct Node));
	palavrasErd->next=NULL;
  clock_t Ticks[2];
  char Linha[100], *result,*token;
	unsigned char *teste1;
  int size, flag,erro=0,total=0,i;
  unsigned long  indice=0;


  for(i=0; i<SIZE; i++){
  	hash[i] = NULL;
  }
	dicionario = fopen("ascii_noaccent_noduplicates_FIXED_v2.txt", "r");
  teste = fopen("testegrande1.txt", "r");
	resultado = fopen("resultado3.txt","w");
  printf("CARREGANDO\n\n");
  if(!dicionario){
		printf("Nao ha nada no dicionario\n");
	}else{
		while(!feof(dicionario)){
			result = fgets(Linha, sizeof(Linha), dicionario);  // o 'fgets' lê até 99 caracteres ou até o '\n'
			token = strtok(result," ,.!'\n''\r'");
			teste1 = strtok(result," ,.!'\n''\r'");
			while(token!=NULL){
			  size = strlen(token);
				indice = Chave (size, teste1);
				TabelaHash(hash, result, indice);	
			  token=strtok(NULL," ,.!'\n'");
			}
		}
		
	}
	fclose(dicionario);
  Ticks[0]=clock();
  if(!teste){
		printf("Nao foi possivel carregar o dicionario");
	}else{
		while(!feof(teste)){
			result=fgets(Linha,100,teste);
			token = strtok(result," ,.!'\n'");
			while(token!=NULL){
			 size = strlen(token);
				indice = Chave (size, token);
				flag = Pesquisa(hash,token, indice);
        total++;	
        if(flag==1){
          erro++;
          InsertAfter(palavrasErd,token);
        } 	
				token=strtok(NULL," ,.!'\n'");
			}
		
		}
	Ticks[1] = clock();
  double Tempo = (Ticks[1]-Ticks[0])*1000.0/CLOCKS_PER_SEC;
    fprintf(resultado,"Numero total de palavras do texto: %d\n",total);  
    fprintf(resultado,"Tempo total de verificação: %lf ms\n",Tempo);
    fprintf(resultado,"Numero de palavras que falharam no spell check: %d\n",erro);  
    fprintf(resultado,"lista de alavras que falharam\n");
    while (palavrasErd!=NULL){
      fprintf(resultado,"Palavra: %s\n",palavrasErd->palavra);
      palavrasErd = palavrasErd->next;
    }
    
  }
	
	return 0;
}
