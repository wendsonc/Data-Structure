#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>

#define SIZE 50273

struct Node{
char palavra[100];
struct Node* next;
};

__uint32_t PreHash (const __uint8_t keysize, unsigned char *word){
		size_t i=0, j;    
		__uint32_t key=0;
	
	while(i != keysize){
		key += word[i++];
		key +=  key<<10;
		key ^= key>>6;
		
	}
	return key;	
}

//Função de hash Jenkins one at time
__uint32_t HashFunction(__uint32_t key){
	
		key += key<<3;
		key ^= key>>11;
		key += key<<15;
		key%=SIZE;
		
	return key;  
	
}

int Search(struct Node *hash[],char *entrada,long unsigned int chave){
	struct Node *aux;
	aux = hash[chave];
	
	while(aux != NULL){
		if(strcmp(entrada,aux->palavra)==0){
			return 0;
		}
		aux=aux->next;
	}

		return 1;
}

void InsLista(struct Node *n, char *value){
  struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));
  strcpy(new_node->palavra,value);
  new_node->next = n->next;
  n->next = new_node;
}

void TabelaHash(struct Node *hash[], char entrada[], unsigned int chave){ 
	struct Node* aux;
	
	if(hash[chave]==NULL){
		hash[chave] = (struct Node*) malloc(sizeof(struct Node));
		strcpy(hash[chave]->palavra, entrada); 
		hash[chave]->next = NULL;
	
	//Tratamento de colisão por separate chaining, mas com inserção no começo da lista
	}else{
		aux = (struct Node*) malloc(sizeof(struct Node));
		strcpy(aux->palavra,entrada);
		aux->next = hash[chave];
		hash[chave] = aux;
		
		
	}
}
/*
//Função de hash MOD
int HashFunction(int keysize, char word[]){
	int i;
	int key=0;

	for(i=0; i < keysize; i++){
		key = ((key + word[i])*CONST)%SIZE;
	}

	
	return key; 
	
}
  */
/*
//Função de Hash DJB2
unsigned long int HashFunction(int keysize, unsigned char *word){
	int i;
	unsigned long key=5381;
	

	while (i = *word++){
		key = ((key<<5)+key)+i;/* hash * 33 + c 

	}
	
	key %= SIZE;
	
	return key; 
	
}
*/

int main(void){

  FILE *dicionario, *teste,*resultado;
	struct Node *hash[SIZE],*PalavrasErradas;
  PalavrasErradas = (struct Node *)malloc(sizeof(struct Node));
	PalavrasErradas->next=NULL;
  clock_t Ticks[2];
  char Linha[100], *result,*token;
	unsigned char *teste1;
  int size, flag,erro=0,total=0,i;
	__uint32_t pre;
  unsigned long  indice=0;

  for(i = 0; i<SIZE; i++){
  	hash[i] = NULL;
  }

	dicionario = fopen("ascii_noaccent_noduplicates_FIXED_v2.txt", "r");
  teste = fopen("testegrande1.txt", "r");
	resultado = fopen("resultado8.txt","w");
  printf("CARREGANDO O DICIONARIO\n\n");
  if(!dicionario){
		printf("Nao existe nada no dicionario\n");
	}else{
		
		while(!feof(dicionario)){
			result = fgets(Linha, sizeof(Linha), dicionario);  // o 'fgets' lê até 99 caracteres ou até o '\n'
			token = strtok(result," ,.!'\n''\r'");
			teste1 = strtok(result," ,.!'\n''\r'");
			while(token!=NULL){
			  size = strlen(token);
				pre = PreHash(size,teste1);
				indice = HashFunction (pre);
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
				pre = PreHash(size,token);
				indice = HashFunction (pre);
				flag = Search(hash,token, indice);
        total++;	
        if(flag==1){
          erro++;
          InsLista(PalavrasErradas,token);
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
	
    while (PalavrasErradas!=NULL){
      fprintf(resultado,"Palavra: %s\n",PalavrasErradas->palavra);
      PalavrasErradas = PalavrasErradas->next;
    }
    
  }
	
	return 0;
}
