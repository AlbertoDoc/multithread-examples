#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

int n;                          // tamanho da sequencia
int *fibseq;                    // array contendo os valores de cada termo
int i;                          // numero de threads

void *threadBody() {
    // Primeiro termo fibonacci
    if (i == 0) {
        fibseq[i] = 0;
        pthread_exit(0);
    }

    // Segundo termo fibonacci
    if (i == 1) {
        fibseq[i] = 1;
        pthread_exit(0);
    } else {
        // Restante dos termos
        fibseq[i] = fibseq[i - 1] + fibseq[i - 2];
        pthread_exit(0);
    }
}

int main() {
    int n;

    float entry;
    printf("Insira a quantidade de números de Fibonacci:\n");

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

    // Variavel que irá armezenar os atributos da thread
    pthread_attr_t attr;
    // alocando memoria para os arrays que irao armazenar os valores
    fibseq = (int *)malloc(n * sizeof(int));
    // Alocando memoria para as threads
    pthread_t *threads = (pthread_t *) malloc(n * sizeof(pthread_t));

    // Inicializando os atributos da thread passando a variavel que irá armazenar os atributos como parametro
    pthread_attr_init(&attr);

    // Setando atributo com "joinable", ou seja, a thread que for criada com esse atributo pode retornar a thread pai
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    
    long status;
    // Criação das threads
    for (i = 0; i < n; i++) {
        status = pthread_create(&threads[i], &attr, threadBody, NULL);

        // Tratamento de erro da criação da thread filho
        if (status) {
            perror("pthread_create");
            exit(1);
        }

        // Aguardando as threads retornarem
        status = pthread_join(threads[i], NULL);

        // Tratamento de erro do metodo pthread_join
        if (status) {
            perror("pthread_join");
            exit(1);
        }
    }

    // Imprimindo a sequencia
    printf("A sequencia de Fibonacci eh: ");

    for (int k = 0; k < n; k++) {
        printf("%d,", fibseq[k]);
    }
    printf("\n");

    // Destruindo atributos de criação de thread
    pthread_attr_destroy(&attr);

    // Termino da execução da thread pai
    pthread_exit(NULL);

    return 0;
}