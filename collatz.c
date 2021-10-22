#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

void *threadBody(void *n) {
    int number = (int) n;
    printf("%d", number);

    // Calculo do algoritmo de Collatz
    while (number != 1) {
        if (number % 2 == 0) {
            number /= 2;
            printf(", %d", (int) number);
        } else {
            number = 3 * number + 1;
            printf(", %d", (int) number);
        }
    }

    printf("\n");

    // Termino da execução da thread filho
    pthread_exit(NULL);
}

int main() {
    int n;

    float entry;
    printf("Insira o número que deseja calcular:\n");

    // Tratamento da entrada para verificar se foi inserido um int positivo
    bool isPositiveInt = false;
    while (!isPositiveInt) {
        scanf("%f", &entry);

        if (entry == (int) entry && (int) entry > 0) {
            n = (int) entry;
            isPositiveInt = true;
        } else {
            printf("Por favor insira um inteiro positivo:\n");
        }
    }

    n = (int) entry;

    // Variavel que irá armazenar a thread filho
    pthread_t child_thread;

    // Variavel que irá armezenar os atributos da thread
    pthread_attr_t attr;

    // Inicializando os atributos da thread passando a variavel que irá armazenar os atributos como parametro
    pthread_attr_init(&attr);

    // Setando atributo com "joinable", ou seja, a thread que for criada com esse atributo pode retornar a thread pai
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    // Variavel status responsavel por pegar a resposta do metodo de criação da thread filho
    long status;

    // Criação da thread filho
    status = pthread_create(&child_thread, &attr, threadBody, (void *) n);

    // Tratamento de erro da criação da thread filho
    if (status) {
        perror("pthread_create");
        exit(1);
    }

    // Thread pai espera a execução da thread filho
    status = pthread_join(child_thread, NULL);

    // Tratamento de erro do metodo pthread_join
    if (status) {
        perror("pthread_join");
        exit(1);
    }

    // Destruindo atributos de criação de thread
    pthread_attr_destroy(&attr);

    // Termino da execução da thread pai
    pthread_exit(NULL);
}