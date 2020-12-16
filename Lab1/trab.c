#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>


//data struct

struct param {
    int tamanho;
    int *array;
};


void *change_array(void *arg);



int main (int argc, char** argv) {

int n = atoi(argv[1]);

if(n < 10 || n > 99){
    printf("Valores fora do escopo permitido, programa será encerrado\n");
    exit(-1);
}

struct param args;
pthread_t tids[2];


args.tamanho = atoi(argv[1]);
args.array = malloc(args.tamanho*sizeof(int));


//pupulate the array with number 0
for(int i = 0; i < args.tamanho; i++) {
    args.array[i] = 0;
}

//print the array before change values
for(int i = 0; i < args.tamanho; i++) {
    printf("%d ", args.array[i]);
}

printf("\n");

for(int i = 0; i<2; i++) {
    pthread_create(&tids[i], NULL ,change_array, &args);

}


for(int i = 0; i < 2; i++)
 {
     pthread_join(tids[i], NULL);
 }

for(int i = 0; i < args.tamanho; i++) {
    printf("%d ", args.array[i]);
}



free(args.array);

return 0;

}

//thread function to increase 1 in array

void *change_array(void *arg) {
    struct param * arg_struct = (struct param *) arg;
   



    for(int i = 0; i <= arg_struct->tamanho; i++) {
        if(arg_struct->array[i] == 0) {
            arg_struct->array[i] += 1;
        }
    }
    

    return NULL;
}