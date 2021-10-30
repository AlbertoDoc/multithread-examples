#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

int escolha;

typedef struct {
    int* v1;
    int* v2;
    int* v3;
} final;

void mergeArrays(int arr1[], int arr2[], int n1, int n2, int arr3[]) {
    int i = 0, j = 0, k = 0;
 
    while (i < n1 && j < n2) {
        if (arr1[i] < arr2[j]) {
            arr3[k++] = arr1[i++];
        } else {
            arr3[k++] = arr2[j++];
        }
    }
 
    while (i < n1) {
        arr3[k++] = arr1[i++];
    }
 
    while (j < n2) {
        arr3[k++] = arr2[j++];
    }
}

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

void* merge(void* v_final) {
    final* v = (final*) v_final;

    int tamanho = (int) floor(escolha/2);

    mergeArrays(v->v1, v->v2, tamanho, tamanho, v->v3);

    pthread_exit(NULL);
}
 
int main() {
    pthread_t t1, t2, t3;

    printf("Quantos numeros deseja ordenar?\n");
    scanf("%i", &escolha);

    int *vetor_pai = (int*)malloc(escolha*sizeof(int));
    int *vetor_final = (int*)malloc(escolha*sizeof(int));
    int *vetor_filho1 = (int*)malloc((int) (floor(escolha/2))*sizeof(int));
    int *vetor_filho2 = (int*)malloc((int) (floor(escolha/2))*sizeof(int));


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

    for(int i = 0; i < (int) floor(escolha/2); i++) {
        vetor_final[i] = vetor_filho1[i];
    }

    j = 0;
    for(int i = (int) floor(escolha/2); i<escolha; i++) {
        vetor_final[i] = vetor_filho2[j];
        j++;
    }

    final* v_final = (final*) malloc(sizeof(final));

    v_final->v1 = vetor_filho1;
    v_final->v2 = vetor_filho2;
    v_final->v3 = vetor_final;

    if (pthread_create(&t3, NULL, merge, (void *)v_final)) {
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
    free(v_final);
    
    pthread_exit(NULL);
    return 0;
}