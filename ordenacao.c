#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

int escolha;

void quick_sort(int *a, int left, int right) {
    int i, j, x, y;
     
    i = left;
    j = right;
    x = a[(left + right) / 2];
     
    while(i <= j) {
        while(a[i] < x && i < right) {
            i++;
        }
        while(a[j] > x && j > left) {
            j--;
        }
        if(i <= j) {
            y = a[i];
            a[i] = a[j];
            a[j] = y;
            i++;
            j--;
        }
    }
     
    if(j > left) {
        quick_sort(a, left, j);
    }
    if(i < right) {
        quick_sort(a, i, right);
    }
}

void *ordenacao(void *vetor_filho){//função a ser executada quando uma thread for criada
    int * check = (int *)vetor_filho;
    quick_sort(check, 0, floor(escolha/2)); // onde n é o tamanho do vetor 
    pthread_exit(NULL); //finalização da thread
}
 
int main(){
    pthread_t t1, t2, t3; //criando t1, t2 e t3 do tipo thread
    int create; //variável que recebe o retorno da função pthread_create()
    long num = 1; //identificador da thread
    printf("Quantos numeros deseja ordenar?\n");
    scanf("%i", &escolha);
    int *vetor_pai = (int*)malloc(escolha*sizeof(int));
    int *vetor_final = (int*)malloc(escolha*sizeof(int));
    int *vetor_filho1 = (int*)malloc((floor(escolha/2))*sizeof(int));
    int *vetor_filho2 = (int*)malloc((floor(escolha/2))*sizeof(int));

    printf("Digite os numeros que deseja ordenar\n");
    for(int i = 0; i<escolha; i++) {
        scanf("%i", &vetor_pai[i]);
    }

    for(int i = 0; i<floor(escolha/2); i++) {
        vetor_filho1[i] = vetor_pai[i];
    }
    int j = 0;
    for(int i = floor(escolha/2); i<escolha; i++) {
        vetor_filho2[j] = vetor_pai[i];
        j++;
    }

    printf("Main: criando a thread filho 1\n");
    create = pthread_create(&t1, NULL, ordenacao, (void *)vetor_filho1); //criando thread 1

    num = 2;
    printf("Main: criando a thread filho 2\n");
    create = pthread_create(&t2, NULL, ordenacao, (void *)vetor_filho2); //criando thread 2

    // juntar as duas partes no vetor final e faz quick sort de novo

    for(int i = 0; i<floor(escolha/2); i++) {
        vetor_final[i] = vetor_filho1[i];
    }
    j = 0;
    for(int i = floor(escolha/2); i<escolha; i++) {
        vetor_final[i] = vetor_filho2[j];
        j++;
    }

    printf("Main: criando a thread final\n");
    create = pthread_create(&t3, NULL, ordenacao, (void *)vetor_final); //criando thread final

    // imprime
    for(int i = 1; i<escolha+1; i++) {
        printf("%d ", vetor_final[i]);
    }
    printf("\n");
    pthread_exit(NULL); //finalização da thread
    printf("Main: finalizando\n");
    return 0;
}