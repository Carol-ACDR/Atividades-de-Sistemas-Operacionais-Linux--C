#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define MAX 1000000
pthread_mutex_t the_mutex;
pthread_cond_t condc, condp;
int buffer[3]; /* O buffer agora é uma lista, ou seja, é permitido mais de um número nele */
int count= 0; /*variavel implementada justamente para saber quantos números há na lista */

const clock_t MAXDELAY = 200000;

void delay(clock_t ticks) { /* a "busy" delay */
  clock_t start = clock();
  do
    ; while (clock() < start + ticks);
}

void *producer(void *ptr) {
    int i;

    /* usado para sinalizacao */
    /* buffer usado entre produtor e consumidor */
    /* dados do produtor */
    for (i = 1; i <= MAX; i++) {
        pthread_mutex_lock(&the_mutex); /* obtem acesso exclusivo ao buffer */
        while (count==3) pthread_cond_wait(&condp, &the_mutex); /*Produtor dorme se o Buffer está cheio*/
        buffer[count] = i; /*coloca item no buffer */
        printf("Colocando em posição %d, o número %d no buffer\n", count, buffer[count]); /*print posição na lista buffer */
        count +=1;
        delay(MAXDELAY);
        pthread_cond_signal(&condc); /* acorda consumidor */
        pthread_mutex_unlock(&the_mutex);/* libera acesso ao buffer */
   }

    pthread_exit(0);
}

void *consumer(void *ptr) {
    int i;

    /* quantos numeros produzir */
    /* dados do consumidor */
    for (i = 1; i <= MAX; i++) {
        pthread_mutex_lock(&the_mutex); /* obtem acesso exclusivo ao buffer */
        while (count == 0) pthread_cond_wait(&condc, &the_mutex); /*consumidor dorme se não há nada no buffer*/
        count -= 1;
        printf("Retirando da posição %d, o número %d no buffer\n", count, buffer[count]);/*print posição na lista buffer */
        buffer[count] = 0; /* retira o item do buffer */
        delay(MAXDELAY);
        /* acorda o produtor */
        pthread_cond_signal(&condp);
        pthread_mutex_unlock(&the_mutex);/* libera acesso ao buffer */
    }
    pthread_exit(0);

}

int main(int argc, char **argv)
{
    pthread_t pro, con;
    pthread_mutex_init(&the_mutex, 0);
    pthread_cond_init(&condc, 0);
    pthread_cond_init(&condp, 0);
    pthread_create(&con, 0, consumer, 0);
    pthread_create(&pro, 0, producer, 0);
    pthread_join(pro, 0);
    pthread_join(con, 0);
    pthread_cond_destroy(&condc);
    pthread_cond_destroy(&condp);
    pthread_mutex_destroy(&the_mutex);
}
