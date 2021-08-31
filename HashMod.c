#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>

#define SIZE 50273
#define CONST 11

struct Node{
char palavra[200];
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

int Search(struct Node *hash[],char *entrada,long unsigned int chave){
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

int HashFunction(int keysize, char word[]){
	int i;
	int key=0;

	for(i=0; i < keysize; i++){
		key = ((key + word[i])*CONST)%SIZE;
	}

	
	return key; 
	
}
  
int main(void){
  FILE *dicionario, *teste,*resultado;
	struct Node *hash[SIZE],*palavrasErradas;
  palavrasErradas = (struct Node *)malloc(sizeof(struct Node));
	palavrasErradas->next=NULL;
  clock_t Ticks[2];
  char Linha[100], *result,*token;
  int size, flag,erro=0,total=0,i;
  long unsigned int indice=0;
  

  for(i=0; i<SIZE; i++){
  	hash[i] = NULL;
  }
	dicionario = fopen("ascii_noaccent_noduplicates_FIXED_v2.txt", "r");
  teste = fopen("testegrande1.txt", "r");
	resultado = fopen("resultado.txt","w");
  printf("CARREGANDO\n\n");
  if(!dicionario){
		printf("Nao ha nada no dicionario\n");
	}else{
		while(!feof(dicionario)){
			result = fgets(Linha, sizeof(Linha), dicionario);  // o 'fgets' lê até 99 caracteres ou até o '\n'
			token = strtok(result," ,.!'\n''\r'");
			while(token!=NULL){
			  size = strlen(token);
				indice = HashFunction (size, token);
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
				indice = HashFunction (size, token);
				flag = Search(hash,token, indice);
        total++;	
        if(flag==1){
          erro++;
          InsertAfter(palavrasErradas,token);
        } 	
				token=strtok(NULL," ,.!'\n'");
			}
		
		}
	Ticks[1] = clock();
  double Tempo = (Ticks[1]-Ticks[0])*1000.0/CLOCKS_PER_SEC;
    fprintf(resultado,"Numero total de palavras do texto: %d\n",total);  
    fprintf(resultado,"Tempo total de verificação: %lf ms\n",Tempo);
    fprintf(resultado,"Numero de palavras que falharam no spell check: %d\n",erro);  
    fprintf(resultado,"lista de palavras que falharam\n");
    while (palavrasErradas!=NULL){
      fprintf(resultado,"Palavra: %s\n",palavrasErradas->palavra);
      palavrasErradas = palavrasErradas->next;
    }
    
  }
	
	return 0;
}
