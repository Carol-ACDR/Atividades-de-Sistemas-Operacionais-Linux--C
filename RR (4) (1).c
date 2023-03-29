//Bibliotecas e definicoes
#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "string.h"
#include "sys/stat.h"
#define TRUE 1
const char* filename = "processos.txt";
void Process();
void Dormindo(char *s);
int Exe(char *s,int p, int * restantes,int au);
int Tempo[6];
char c1[5],c2[5],c3[5],c4[5],c5[5],c6[5];

int main()
{   //leitura do arquivo
    FILE *in_file = fopen(filename, "r");
    struct stat sb;
    stat(filename, &sb);
    char *token;
    char *file_contents = malloc(sb.st_size);
    int nlinhas = 0;

    //separando nomes e tempo
    while (fscanf(in_file, "%[^\n] ", file_contents) != EOF) {
      int count =0;
      char aux[10];
      token = strtok(file_contents, ";");
        while( token != NULL ) {
          if (count ==0){
          if (nlinhas ==0){strcpy(c1,token);count++;}
          else if(nlinhas==1){strcpy(c2,token);count++;}
          else if(nlinhas==2){strcpy(c3,token);count++;}
          else if(nlinhas==3){strcpy(c4,token);count++;}
          else if(nlinhas==4){strcpy(c5,token);count++;}
          else if(nlinhas==5){strcpy(c6,token);count++;}}
          else{strcpy(aux,token);sscanf(aux, "%d", &Tempo[nlinhas]); }
          token = strtok(NULL, ";");}
         nlinhas +=1;
    }     

    //fechando a pagina e chamando o processo
    fclose(in_file);
    free(file_contents);
    Process();
    
    return 0;
    }


//pequena função pra simular a "perda" da CPU e necessidade de espera.
void Dormindo(char *s){
printf("%s está esperando sua vez\n",s);
}

//Processo de execussao com clock em segundos e quantum=2(chamada repercussiva)
int Exe(char *s,int p,int *restantes, int au){
clock_t start = clock();
printf("Processo %s está executando, faltam %d quantuns para o término\n",s,Tempo[p]-1);
 while (clock() < start + 1000){};
 Tempo[p] -=1;
 if (Tempo[p]==0){
   printf("Processo %s concluido!\n",s);
   if(*restantes==1){return 2;}
   else{*restantes-=1;return 1;}
 }
 else if(au==1){
   int auxE = Exe(s,p,restantes,0);
   if(auxE ==1){return 1;}
   else if (auxE==2){return 2;}
   return 0;}
 return 0;
 }
 
 //round-robin feito com listas de chegada(como chegaram na mesma hora, foi por escolha), só um processo executa e ao final de 2 quantuns ele libera a CPU para outro.
void Process(){
char *Nomes[6]= {c1,c2,c3,c4,c5,c6};
int restantes = 6;
int proximo =0;
int count =0;
while(TRUE){ /*loop infinito*/
  if(Tempo[proximo]>0){
  count = Exe(Nomes[proximo],proximo,&restantes,1);
  //Quando Exe devolve 1, é para indicar que o processo terminou sua execussao e pode sair antes do segundo quantum.
  if (count== 1){
    proximo = (proximo+1)%6;
  }
  //Quando chega-se a apenas um processo restante e ele termina de executar, a função Exe devolve 2, que faz com que o codigo pare.
  else if(count== 2){
    break;
  }
  else{
    //if para que o processo restante não durma mais
    if(restantes==1){} 
    else{Dormindo(Nomes[proximo]);
    proximo = (proximo+1)%6;}
  }}
  else{proximo = (proximo+1)%6;}
  }
  }
