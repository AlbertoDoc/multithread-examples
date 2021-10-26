#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

int escolha;

int comparador(const void* a, const void* b) {
    int f = *((int*) a);
    int s = *((int*) b);
    if (f < s) return -1;
    if (f > s) return 1;
    return 0;
}

void* ordena_filho(void* original) {
    int* vetor = (int*) original;

    qsort(vetor, (int) floor(escolha/2), sizeof(int), comparador);

    pthread_exit(NULL);
}

void* ordena_final(void* vetor_filho) {
    int* check = (int*) vetor_filho;

    qsort(check, escolha, sizeof(int), comparador);

    pthread_exit(NULL);
}
 
int main() {
    pthread_t t1, t2, t3;

    printf("Quantos numeros deseja ordenar?\n");
    scanf("%i", &escolha);

    int *vetor_pai = (int*)malloc(escolha*sizeof(int));
    int *vetor_final = (int*)malloc(escolha*sizeof(int));
    int *vetor_filho1 = (int*)malloc((floor(escolha/2))*sizeof(int));
    int *vetor_filho2 = (int*)malloc((floor(escolha/2))*sizeof(int));


    printf("Digite os numeros que deseja ordenar\n");
    for(int i = 0; i < escolha; i++) {
        scanf("%i", &vetor_pai[i]);
    }

    for(int i = 0; i < (int) floor(escolha/2); i++) {
        vetor_filho1[i] = vetor_pai[i];
    }
    
    int j = 0;
    for(int i = (int) floor(escolha/2); i < escolha; i++) {
        vetor_filho2[j] = vetor_pai[i];
        j++;
    }

    if (pthread_create(&t1, NULL, ordena_filho, (void *)vetor_filho1)) {
        perror("pthread_create");
        exit(1);
    }

    if (pthread_join(t1, NULL)) {
        perror("pthread_join");
        exit(1);
    }

    if (pthread_create(&t2, NULL, ordena_filho, (void *)vetor_filho2)) {
        perror("pthread_create");
        exit(1);
    }

    if (pthread_join(t2, NULL)) {
        perror("pthread_join");
        exit(1);
    }

    for(int i = 0; i<floor(escolha/2); i++) {
        vetor_final[i] = vetor_filho1[i];
    }

    j = 0;
    for(int i = floor(escolha/2); i<escolha; i++) {
        vetor_final[i] = vetor_filho2[j];
        j++;
    }

    if (pthread_create(&t3, NULL, ordena_final, (void *)vetor_final)) {
        perror("pthread_create");
        exit(1);
    }

    if (pthread_join(t3, NULL)) {
        perror("pthread_join");
        exit(1);
    }

    for (int i = 0; i < escolha; i++) {
        printf("%d ", vetor_final[i]);
    }

    printf("\n");

    free(vetor_pai);
    free(vetor_final);
    free(vetor_filho1);
    free(vetor_filho2);

    pthread_exit(NULL);
    return 0;
}