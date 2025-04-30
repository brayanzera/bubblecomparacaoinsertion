#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// Número de repetições
#define REPETICOES 30

// Funções de ordenação
void bubbleSort(int *v, int n) {
    int trocou = 1;
    while(trocou) {
        trocou = 0;
        for(int i = 0; i < n-1; i++) {
            if(v[i] > v[i+1]) {
                int temp = v[i];
                v[i] = v[i+1];
                v[i+1] = temp;
                trocou = 1;
            }
        }
    }
}

void insertionSort(int *v, int n) {
    for(int i = 1; i < n; i++) {
        int chave = v[i];
        int j = i - 1;
        while(j >= 0 && v[j] > chave) {
            v[j+1] = v[j];
            j--;
        }
        v[j+1] = chave;
    }
}

// Funções para gerar os vetores
void gerarMelhorCaso(int *v, int n) {
    for(int i = 0; i < n; i++) {
        v[i] = i;
    }
}

void gerarPiorCaso(int *v, int n) {
    for(int i = 0; i < n; i++) {
        v[i] = n - i;
    }
}

void gerarCasoMedio(int *v, int n) {
    for(int i = 0; i < n; i++) {
        v[i] = rand();
    }
}

// Função para medir o tempo de execução 30 vezes
void medirTempo(void (*func)(int *, int), void (*gerar)(int *, int), int *v, int n, const char *nomeAlgoritmo, const char *caso, int tamanho) {
    double tempos[REPETICOES];
    double soma = 0.0;

    for(int r = 0; r < REPETICOES; r++) {
        gerar(v, n);

        int *copia = (int *)malloc(sizeof(int) * n);
        memcpy(copia, v, sizeof(int) * n);

        clock_t inicio = clock();
        func(copia, n);
        clock_t fim = clock();

        double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;
        tempos[r] = tempo;
        soma += tempo;

        free(copia);
    }

    double media = soma / REPETICOES;

    printf("\n==============================\n");
    printf("Algoritmo: %s | Caso: %s | N = %d\n", nomeAlgoritmo, caso, tamanho);
    printf("------------------------------\n");
    for(int i = 0; i < REPETICOES; i++) {
        printf("Teste %02d: %.4f segundos\n", i+1, tempos[i]);
    }
    printf("------------------------------\n");
    printf("MÉDIA FINAL: %.4f segundos\n", media);
    printf("==============================\n\n");
}

int main() {
    // Início da medição do tempo total
    clock_t inicioTotal = clock();

    // Redireciona a saída para o arquivo
    FILE *arquivo = freopen("resultado.txt", "w", stdout);
    if (arquivo == NULL) {
        perror("Erro ao redirecionar a saída para o arquivo");
        return 1;
    }

    srand(time(NULL)); // Inicializa o gerador de números aleatórios

    int tamanhos[] = {10000, 50000, 100000, 200000};
    int numTamanhos = sizeof(tamanhos)/sizeof(tamanhos[0]);

    for(int t = 0; t < numTamanhos; t++) {
        int n = tamanhos[t];
        int *v = (int *)malloc(sizeof(int) * n);

        if(v == NULL) {
            printf("Erro ao alocar memória\n");
            exit(1);
        }

        printf("\n######################################\n");
        printf("########## Testando N = %d ##########\n", n);
        printf("######################################\n");

        // Melhor caso
        medirTempo(bubbleSort, gerarMelhorCaso, v, n, "BubbleSort", "Melhor Caso", n);
        medirTempo(insertionSort, gerarMelhorCaso, v, n, "InsertionSort", "Melhor Caso", n);

        // Caso médio
        medirTempo(bubbleSort, gerarCasoMedio, v, n, "BubbleSort", "Caso Médio", n);
        medirTempo(insertionSort, gerarCasoMedio, v, n, "InsertionSort", "Caso Médio", n);

        // Pior caso
        medirTempo(bubbleSort, gerarPiorCaso, v, n, "BubbleSort", "Pior Caso", n);
        medirTempo(insertionSort, gerarPiorCaso, v, n, "InsertionSort", "Pior Caso", n);

        free(v);
    }

    // Fim da medição do tempo total
    clock_t fimTotal = clock();
    double tempoTotal = (double)(fimTotal - inicioTotal) / CLOCKS_PER_SEC;

    printf("\nTempo total de execução: %.2f segundos\n", tempoTotal);

    fclose(arquivo);
    return 0;
}

