/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 4 */
/* Codigo: Uso de variáveis de condição e suas operações básicas para sincronização por condição */

/***** Condicao logica da aplicacao: as threads Bomdia e TudoBem so podem imprimir depois que as threads ateMais e boaTarde imprimirem  ****/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS  4

/* Variaveis globais */
int x = 0;
int y = 0;
pthread_mutex_t x_mutex;
pthread_cond_t x_cond;

/* imprime tudo bem */
void *tudoBem (void *t) {
  int boba1, boba2;


  /* faz alguma coisa para gastar tempo... */
  boba1=10000; boba2=-10000; while (boba2 < boba1) boba2++;
  
  printf("Tudo Bem\n");

  pthread_mutex_lock(&x_mutex);

  x++;
  if (x==2) {
  pthread_cond_broadcast(&x_cond);
  }
  pthread_mutex_unlock(&x_mutex);

  pthread_exit(NULL);
}

/* imprime bom dia */
void *bomDia (void *t) {
  int boba1, boba2;
  
  /* faz alguma coisa para gastar tempo... */
  boba1=10000; boba2=-10000; while (boba2 < boba1) boba2++;
  
  printf("Bom dia\n");

  pthread_mutex_lock(&x_mutex);

  x++;
  if (x==2) {
  pthread_cond_broadcast(&x_cond);
  }
  pthread_mutex_unlock(&x_mutex);

  pthread_exit(NULL);
}

/* Imprime ate mais */
void *ateMais (void *t) {

  pthread_mutex_lock(&x_mutex);
  if (x < 2) { 
     //printf("A: vai se bloquear...\n");
     pthread_cond_wait(&x_cond, &x_mutex);
     //printf("A: sinal recebido e mutex realocado, x = %d\n", x);
  }

  pthread_mutex_unlock(&x_mutex); 
  printf("Até mais\n");
  pthread_exit(NULL);
  
  

}

/* imprime boa tarde */
void *boaTarde (void *t) {

  pthread_mutex_lock(&x_mutex);
  if (x < 2) { 
     //printf("A: vai se bloquear...\n");
     pthread_cond_wait(&x_cond, &x_mutex);
     //printf("A: sinal recebido e mutex realocado, x = %d\n", x);
  }

  pthread_mutex_unlock(&x_mutex); 
  printf("Boa tarde\n");
  pthread_exit(NULL);
  
  

}

/* Funcao principal */
int main(int argc, char *argv[]) {
  int i; 
  pthread_t threads[NTHREADS];

  /* Inicilaiza o mutex (lock de exclusao mutua) e a variavel de condicao */
  pthread_mutex_init(&x_mutex, NULL);
  pthread_cond_init (&x_cond, NULL);

  /* Cria as threads */
  pthread_create(&threads[1], NULL, tudoBem, NULL);
  pthread_create(&threads[3], NULL, bomDia, NULL);
  pthread_create(&threads[0], NULL, ateMais, NULL);
  pthread_create(&threads[2], NULL, boaTarde, NULL);

  /* Espera todas as threads completarem */
  for (i = 0; i < NTHREADS; i++) {
    pthread_join(threads[i], NULL);
  }
  printf ("\nFIM\n");

  /* Desaloca variaveis e termina */
  pthread_mutex_destroy(&x_mutex);
  pthread_cond_destroy(&x_cond);
  return 0;
}
