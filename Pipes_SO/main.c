#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

#define VECTOR_SIZE 9
int separar(int vet[], int deck[], int carta);

int main(void)
{
    int fd[2];
    pid_t pid;

    srand(time(NULL));

    int vetor[VECTOR_SIZE] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
        int deckPai[5] = {-1,-1,-1,-1,-1};
        int deckFilho[5] = {-1,-1,-1,-1,-1};
        int carta = 0;
        int vez = 0;


        while (carta < 5) {
            separar(vetor, deckPai, carta);
            separar(vetor, deckFilho, carta);
            carta++;
        }

    carta = 0;

    for(int i = 0; i < 5; i++) {
        printf("Deck Pai: %d\n", deckPai[i]);
    }

    for(int i = 0; i < 5; i++) {
        printf("Deck Filho: %d\n", deckFilho[i]);
    }

    if(pipe(fd) < 0) {
        perror("pipe");
        return -1;
    }

    if((pid = fork()) < 0) {
        perror("fork");
        exit(1);
    }

    if(pid > 0) { // Processo Pai, Jogador 1
        close(fd[0]);


        printf("Vetor enviado pelo pai no Pipe: ");
        for(int i = 0; i < VECTOR_SIZE; i++) {
            printf("%d ", vetor[i]);
        }
        printf("\n");

        write(fd[1], vetor, sizeof(vetor)); // Escreve o vetor no pipe
        close(fd[1]); // Fecha a extremidade de escrita do pipe
        exit(0);
    }

    else { // Processo Filho, Jogador 2

        int vetor_recebido[VECTOR_SIZE];

        close(fd[1]); // Fecha a extremidade de escrita do pipe

        read(fd[0], vetor_recebido, sizeof(vetor_recebido)); // Lê o vetor do pipe

        printf("Vetor lido pelo filho no Pipe: ");
        for(int i = 0; i < VECTOR_SIZE; i++) {
            printf("%d ", vetor_recebido[i]);
        }
        printf("\n");

        close(fd[0]); // Fecha a extremidade de leitura do pipe
        exit(0);
    }

    return 0;
}


int separar(int *vet, int *deck, int carta) {

    int cont = 0;
    int indiceAleatorio = 0;
    int catraca = 1;


    while(catraca == 1){
        indiceAleatorio = rand() % 9;

        if (!vet[indiceAleatorio] == 0){
            deck[carta] = vet[indiceAleatorio];
            vet[indiceAleatorio] = 0;
            catraca = 0;
        }

        for(int i = 0;i < 9; i++) {
            if(vet[i] == 0)
                cont++;
        }

        if(cont == 9)
            catraca = 0;
        else
            cont = 0;

    }

    return 1;
}
