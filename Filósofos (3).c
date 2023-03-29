/*Bibliotecas*/
#include "stdio.h"
#include "unistd.h"
#include "stdlib.h"
#include "pthread.h"          /*Para Threads*/
#include "semaphore.h"        /*Para Semáforos*/
/*Definições*/
#define TRUE 1
#define N 5
#define LEFT (i+N-1)%N
#define RIGHT (i+1)%N
#define THINKING 0
#define HUNGRY 1
#define EATING 2

/*Área Crítica*/
int state[N];      /*Array de estados de cada Filósofo*/
sem_t mutex;       /*Mutex para exclusão multua*/
sem_t s[N];        /*Array com semáforo de cada filósofo*/

/*Protótipos*/
void eat();
void think();
void test(int i);
void take_forks(int i);
void put_forks(int i);
void* philosopher(void *arg);


/*Main*/
int main(){
/*Iniciando Semafóros*/
sem_init(&mutex,TRUE,  1);
for(int x = 0;x<5;x++){sem_init(&s[x],TRUE,  0);}

/*Criando e unindo as Threads*/
pthread_t p1,p2,p3,p4,p5;
int n1=0,n2=1,n3=2,n4=3,n5=4;
pthread_create(&p1,NULL,(void *)philosopher,&n1);
pthread_create(&p2,NULL,(void *)philosopher,&n2);
pthread_create(&p3,NULL,(void *)philosopher,&n3);
pthread_create(&p4,NULL,(void *)philosopher,&n4);
pthread_create(&p5,NULL,(void *)philosopher,&n5);
pthread_join(p1, 0);
pthread_join(p2, 0);
pthread_join(p3, 0);
pthread_join(p4, 0);
pthread_join(p5, 0);
return 0;
}

/*Funções vazias que foram usadas para garantir pequeno delay(demora um tempo para comer e pensar)*/
void eat(){
clock_t start = clock();
  do; 
 while (clock() < start + 2000000);}
 
void think(){
clock_t start = clock();
  do; 
 while (clock() < start + 2000000);}
 
 void test(int i){  /*Teste para saber se algum dos colegas do lado está comendo*/
if(state[i] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING){
state[i] = EATING;/*se nenhum dos colegas do lado está comendo e o filósofo está com fome, ele começa a comer*/
printf("\n Philosopher %i is EATING",i); /*Para avaliar o decorrer do código*/
sem_post(&s[i]);}} /*libera o semáforo do filósofo*/

void take_forks(int i){
sem_wait(&mutex); /*pega a área crítica para si*/
state[i] = HUNGRY; /*Muda estado*/
printf("\n Philosopher %i is HUNGRY",i); /*Para avaliar o decorrer do código*/
test(i); /*Chama função*/
sem_post(&mutex); /*Libera Mutex*/
sem_wait(&s[i]);} /*Dorme caso não tenha conseguido comer*/

void put_forks(int i){
sem_wait(&mutex); /*pega a área crítica para si*/
state[i] = THINKING; /*Muda estado*/
printf("\n Philosopher %i is THINKING",i); /*Para avaliar o decorrer do código*/
/*Testa os dois colegas do lado para acorda-los*/
test(LEFT);
test(RIGHT);
sem_post(&mutex);} /*Libera a área crítica para si*/


void* philosopher(void *arg){
int i = *((int*)arg); /*transformando 'void *' em inteiro por cast(para evitar 'warnings' na hora da compilação)*/
while(TRUE){ /*loop infinito*/

/*Chamando as funções do Filósofo*/
think();
take_forks(i);
eat();
put_forks(i);}}


