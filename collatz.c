#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *threadBody(void *n) {
    int numero = (int) n;
    printf("%d", numero);

    while (numero != 1) {
        if (numero % 2 == 0) {
            numero /= 2;
            printf(", %d", (int) numero);
        } else {
            numero = 3 * numero + 1;
            printf(", %d", (int) numero);
        }
    }

    printf("\n");

    // Termino da execução da thread filho
    pthread_exit(NULL);
}

int main() {
    int n;

    printf("Insira o número que deseja calcular:\n");
    scanf("%d", &n);
    //TODO adicionar tratamento da entrada para verificar se eh um int

    // Variavel que irá armazenar a thread filho
    pthread_t thread_filho;

    // Variavel que irá armezenar os atributos da thread
    pthread_attr_t attr;

    // Inicializando os atributos da thread passando a variavel que irá armazenar os atributos como parametro
    pthread_attr_init(&attr);

    // Setando atributo com "joinable", ou seja, a thread que for criada com esse atributo pode retornar a thread pai
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    // Variavel status responsavel por pegar a resposta do metodo de criação da thread filho
    long status;

    // Criação da thread filho
    status = pthread_create(&thread_filho, &attr, threadBody, (void *) n);

    // Tratamento de erro da criação da thread filho
    if (status) {
        perror("pthread_create");
        exit(1);
    }

    // Thread pai espera a execução da thread filho
    status = pthread_join(thread_filho, NULL);

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