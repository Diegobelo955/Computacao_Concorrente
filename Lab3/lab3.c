#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<math.h>
#include "timer.h"



//declarar variáveis globais
float *array;
int nthreads;
long long int dim;

struct parametros {
    long long int id;
    long long int dimensao;
};


//funcao thread

void * calcularPI(void * arg) {
    struct parametros *tArgs = (struct parametros *) arg;
    float *localsoma;
    long long int tamBloco = tArgs->dimensao/nthreads; //tamanho do bloco de cada thread
    long long int ini = tArgs->id * tamBloco; // elemento inicial do bloco da thread
    long long int final;
    
    localsoma = (float *) malloc(sizeof(float));
    *localsoma = 0;

    if(tArgs->id == nthreads -1) {
        final = tArgs->dimensao;
    }

    else {
        final = ini + tamBloco;
    }
  
    for(long long int i = ini ; i < final; i++) {
        if(i%2 == 0) {
        *localsoma = *localsoma + array[i];
        }
        else {
        *localsoma = *localsoma - array[i];

        }
    }

    pthread_exit((void*) localsoma);
}


//função main

int main (int argc, char** argv) {
    
    //declaracao de variaveis
    float seqPI = 0;
    float concPI = 0;
    float *retorno;
    float valorComparado;
    struct parametros *args;
    pthread_t *tid;
    float finalPi;
    double inicio, fim, delta,ganhoDesempenho, tempoConcorrente = 0, tempoSequencial = 0;
    
    



//checar argumentos e receber parametros

if(argc < 3) {
    fprintf(stderr, "ERRO -- Num argumentos");
    return -1;
}
    
dim = atoll(argv[1]);
nthreads = atoi(argv[2]);

//alocar estrutura de dados


array = (float *) malloc(sizeof(float)*dim);
if(array == NULL) {
    fprintf(stderr, "ERRO -- MALLOC");
    return 3;
}
//Inicializar estrutura de dados com os valores da sequencia 1/3, 1/5, 1/7.....
for(long long int i = 0; i < dim; i++) {
    array[i] = 1.0/(2*i + 1);
}

GET_TIME(inicio)
//Calcular Pi sequencialmente

for(long long int i = 0; i < dim; i++) {
    if(i%2 == 0) {
        seqPI = seqPI+ array[i];
        }
        else {
        seqPI = seqPI- array[i];
        }

}

finalPi = 4*seqPI;
valorComparado = M_PI - finalPi;



printf("Valor de pi calculado sequencialmente: %.14f\n", finalPi);

printf("Diferenca entre o PI da funcao(seq) x o PI da biblioteca math.c : %f\n", valorComparado);


GET_TIME(fim)

delta = fim - inicio;
tempoSequencial = delta;

printf("Tempo gasto para calcular pi sequencialmente: %f\n", tempoSequencial);
printf("------------------------------------------------------------------------------------------\n\n");



GET_TIME(inicio)

//criacao das threads
args = (struct parametros *) malloc(sizeof(struct parametros)*nthreads);
tid = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
if(tid == NULL) {
    fprintf(stderr, "ERRO--malloc\n");
    return 2;
}

for(long long int i = 0; i < nthreads; i++) {
    (args+i)->dimensao = dim;
    (args+i)->id = i;
    if(pthread_create((tid+i), NULL, calcularPI, (void *) (args+i))){
        fprintf(stderr, "Erro--Create_Thread\n");
        return 3;
    }

}


//aguarda o termino das threads

for(long int i = 0; i < nthreads; i++) {
    if(pthread_join(*(tid +i), (void**) &retorno)) {
        fprintf(stderr, "Erro--Create_Join\n");
        return 3;
    }
    concPI += *retorno;
    free(retorno);
    }

finalPi = 4*concPI;


printf("Valor de PI Calculado com computacao concorrente: %.14f\n", finalPi);
valorComparado = M_PI - finalPi;
printf("Diferenca entre o PI da funcao(conc) x o PI da biblioteca math.c : %f\n", valorComparado);


//liberacao da memoria alocada

free(tid);

GET_TIME(fim);

delta = fim - inicio;
tempoConcorrente = delta;
printf("Tempo gasto para calcular pi paralelamente: %f\n", tempoConcorrente);


printf("-------------------------------------------------------------------------------------\n");
printf("Tempo total: %f\n\n\n\n", tempoSequencial+tempoConcorrente);

ganhoDesempenho = tempoSequencial/tempoConcorrente;

printf("Ganho de desempenho: %f\n\n", ganhoDesempenho);





return 0;



}