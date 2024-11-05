#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include "algoritmos.h"

#define MAX_SIZE 1024
#define MAX_NUM 2048
#define MAX_SLOTS 4 

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

int out = 0;
int *vetores[MAX_SLOTS] = {NULL};
int ordenado[MAX_SLOTS] = {0};

void desenhar_quadro(int comp, double tempo, int trocas, int larg) {
    char b_comp[27], b_tempo[20], b_trocas[20];

    snprintf(b_comp, sizeof(b_comp), "Comparações: %d", comp);
    snprintf(b_tempo, sizeof(b_tempo), "Tempo: %.3f", tempo);
    snprintf(b_trocas, sizeof(b_trocas), "Trocas: %d", trocas);

    int p_comp = (larg - (int)strlen(b_comp) - 2) / 2;
    int p_tempo = (larg - (int)strlen(b_tempo) - 2) / 2;
    int p_trocas = (larg - (int)strlen(b_trocas) - 2) / 2;

    printf("┌");
    for (int i = 0; i < larg - 2; i++) printf("─");
    printf("┐\n");

    printf("│%*s│\n", larg - 2, "");
    printf("│ %*s%s%*s│\n", p_comp, "", b_comp, larg - p_comp - (int)strlen(b_comp) - 1, "");
    printf("│%*s%s%*s│\n", p_tempo, "", b_tempo, larg - p_tempo - (int)strlen(b_tempo) - 2, "");
    printf("│%*s%s%*s│\n", p_trocas, "", b_trocas, larg - p_trocas - (int)strlen(b_trocas) - 2, "");
    printf("│%*s│\n", larg - 2, "");

    printf("└");
    for (int i = 0; i < larg - 2; i++) printf("─");
    printf("┘\n");
}

void preencher_vetor(int slot){
    for (int i = 0; i < MAX_SIZE; i++)
        vetores[slot][i] = (rand() % MAX_NUM) + 1;
}

void cria_vetor(int slot) {
    char res;
    if (slot < 0 || slot > 3) {
        printf("Slot inválido, abortando");
        return;
    }
    if (vetores[slot] != NULL){
        printf("\nSlot já ocupado, gostaria de sobrescrever [N/y]? ");
        scanf("%c", &res);
        if (res == 'Y' || res == 'y'){
            preencher_vetor(slot);
            ordenado[slot] = 0;
            printf("\nVetor sobrescrito");
        } 
        else {
            printf("Operação abortada");
        }
        return;
    }

    int *v = malloc(MAX_SIZE * sizeof(int));
    vetores[slot] = v;
    preencher_vetor(slot);
}

void mostra_intervalo(int slot, int ini, int fim){
    if (fim < ini || fim > MAX_SIZE || ini > MAX_SIZE) {
        printf(RED "Intervalo inválido, abortando\n" RESET);
        return;
    }
    if (slot < 0 || slot > 3){
        printf("Slot inválido, abortando");
        return;
    }
    printf("[");
    if (ini != 1){
        printf("... ");
    }
    if (vetores[slot] != NULL){
        for (int i = ini-1; i < fim-1; i++){
            printf("%d, ", vetores[slot][i]);
        }
        printf("%d", vetores[slot][fim-1]);
        if (fim != MAX_SIZE){
            printf(", ...");
        }
    }
    else printf(" ");
    printf("]\n");
}

void imprime_vetores(){
    printf("Vetores:\n");
    for (int i = 0; i < MAX_SLOTS; i++){
        printf("%d - ", i);
        mostra_intervalo(i, 1, 10);
    }
    printf("\n");
}

void destroi_vetores(){
    for (int i = 0; i < MAX_SLOTS; i++){
        free(vetores[i]);
        vetores[i] = NULL;
    }
}

void copia_vetor(int orig, int dest){
    if (vetores[dest] == NULL){
        cria_vetor(dest);
    }

    for (int i = 0; i < MAX_SIZE; i++){
        vetores[dest][i] = vetores[orig][i];
    }
}

void esvazia_vetor(int slot){
    free(vetores[slot]);
    vetores[slot] = NULL;
}

void benchmark(int op, int alg, int tipo, int slot){
    struct cont res;
    clock_t t;
    if (op == 1){
        switch (alg){
            case 1:
                t = clock();
                res = quicksort(vetores[slot], 0, MAX_SIZE - 1, tipo);  
                t = clock() - t;
                break;
            case 2:
                t = clock();
                res = shellsort(vetores[slot], MAX_SIZE, tipo);  
                t = clock() - t;
                break;
            case 3:
                t = clock();
                res = bubbleSort(vetores[slot], MAX_SIZE);
                t = clock() - t;
                break;
            case 4:
                t = clock();
                res = insertSort(vetores[slot], MAX_SIZE);
                t = clock() - t;
                break;
            case 5:
                t = clock();
                res = selectionSort(vetores[slot], MAX_SIZE);
                t = clock() - t;
                break;
        }
        double tempo = ((double)t)/CLOCKS_PER_SEC*1000;
        desenhar_quadro(res.comp, tempo, res.trocas, 30);
    }

}

void imprime_opcoes(){
    printf("Opções:\n");
    printf("1 - Ordenar algum vetor         \n");
    printf("2 - Buscar em algum vetor       \n");
    printf("3 - Criar novo vetor aleatório  \n");
    printf("4 - Copiar vetor em outro espaço\n");
    printf("5 - Imprimir um intervalo       \n");
    printf("6 - Esvaziar vetor              \n");
    printf("7 - Limpar tela                 \n");
    printf(RED "8 - Rodar 1000              \n" RESET);
    printf("9 - Sair                        \n");
}

void ordena_escolhas(){
    printf("\n1 - Quicksort\n");
    printf("2 - Shellsort\n");
    printf("3 - BubbleSort\n");
    printf("4 - InsertionSort\n");
    printf("5 - SelectionSort\n\n");
    printf("Qual algoritmo gostaria de utilizar [1-5]? ");
}

void quick_escolhas(){
    printf("\n1 - Pivô é o último elemento        \n");
    printf("2 - Pivô é o primeiro elemento      \n");
    printf("3 - Pivô é o elemento do meio       \n");
    printf("4 - Pivô é a mediana entre 3      \n\n");
}

void shell_escolhas(){
    printf("\n1 - k = primos otimizados para 1024 elementos \n");
    printf("2 - k = 3k + 1                                  \n");
    printf("3 - k = 2^n                                     \n");
}

void busca_escolhas(){
    printf("\n1 - Busca Sequencial\n");
    printf("2 - Busca Binária\n");
}

void faz_escolha(){
    int op, alg, ok = 0, tipo = 1, slot, orig, dest, v;
    while (!ok){
        printf("\nO que voce gostaria de fazer [1-9]? ");
        scanf("%d", &op);
        if (op < 1 || op > 9)
            printf(RED "\nOperação inválida, tente novamente\n" RESET);
        else 
            ok = 1;
    }
    ok = 0;
    switch (op){
        case 1:
            while (!ok){
                ordena_escolhas();
                scanf("%d", &alg);
                if (alg < 1 || alg > 5)
                    printf(RED"\nAlgoritmo inválido, tente novamente\n"RESET);
                else 
                    ok = 1;
            }
            ok = 0;
            switch (alg){
                case 1:
                    quick_escolhas();
                    while (!ok){
                        printf("Qual tipo de pivô gostaria de utilizar[1-4]? ");
                        scanf("%d", &tipo);
                        if (tipo < 1 || tipo > 4)
                            printf(RED"\nEscolha inválida, tente novamente\n"RESET);
                        else 
                            ok = 1;
                    }  
                    break;
                case 2:
                    shell_escolhas();
                    while (!ok){
                        printf("Qual tipo de k gostaria de utilizar[1-3]? ");
                        scanf("%d", &tipo);
                        if (tipo < 1 || tipo > 3)
                            printf(RED"\nEscolha inválida, tente novamente\n"RESET);
                        else 
                            ok = 1;
                    }  
                    break;
            }
            break;
        case 2:
            busca_escolhas();
            while (!ok){
                printf("\nQual tipo de busca gostaria de utilizar[1/2]? ");
                scanf("%d", &tipo); 
                if (tipo != 1 || tipo != 2)
                    printf(RED"\nEscolha inválida, tente novamente\n"RESET);
                else 
                    ok = 1;
            }
            break;
        case 3:
            while(!ok){
                printf("\nEm qual slot guardar o vetor[0-3]? ");
                scanf("%d", &slot);
                if (slot < 0 || slot > 3)
                    printf(RED"\nSlot inválido, tente novamente\n"RESET);
                else 
                    ok = 1;
            }
            cria_vetor(slot);
            break;
        case 4:
            while(!ok){
                printf("\nQual vetor gostaria de copiar[0-3]? ");
                scanf("%d", &orig);
                if (orig < 0 || orig > 3 || vetores[orig] == NULL)
                    printf(RED"\nSlot inválido, tente novamente\n"RESET);
                else 
                    ok = 1;
            }
            ok = 0;
            while(!ok){
                printf("\nPara qual vetor copiar[0-3]? ");
                scanf("%d", &dest);
                if (dest < 0 || dest > 3)
                    printf(RED"\nSlot inválido, tente novamente\n"RESET);
                else 
                    ok = 1;
            }
            copia_vetor(orig, dest);
            break;
        case 5:
            while(!ok){
                printf("\nQual vetor consultar[0-3]? ");
                scanf("%d", &v);
                if (v < 0 || v > 3 || vetores[v] == NULL)
                    printf(RED"\nSlot inválido, tente novamente\n"RESET);
                else 
                    ok = 1;
            }
            ok = 0;
            while(!ok){
                printf("\nDe qual posição[1-1024]? ");
                scanf("%d", &orig);
                if (orig < 1 || orig > 1024)
                    printf(RED"\nInício inválido, tente novamente\n"RESET);
                else 
                    ok = 1;
            }
            ok = 0;
            while(!ok){
                printf("\nAté que posição[%d-1024]? ", orig);
                scanf("%d", &dest);
                if (dest < 1 || dest > 1024 || dest < orig)
                    printf(RED"\nFim inválido, tente novamente\n"RESET);
                else 
                    ok = 1;
            }
            mostra_intervalo(v, orig, dest);
            break;
        case 6:
            while(!ok){
                printf("\nQue vetor esvaziar[0-3]? ");
                scanf("%d", &v);
                if (v < 0 || v > 3)
                    printf(RED"\nSlot inválido, tente novamente\n"RESET);
                else 
                    ok = 1;
            }
            if (vetores[v] == NULL)
                printf("\nVetor já está vazio!");
            else 
                esvazia_vetor(v);
            break;
        case 7:
            system("clear");
            break;
        case 8:
            while(!ok){
                system("clear");
                //algoritmos_escolha();
                printf("\nQual algoritmo gostaria de rodar 1000 vezes[1-12]? ");
                scanf("%d", &alg);
                if (alg < 1 || alg > 12)
                    printf(RED"\nEscolha inválida, tente novamente\n"RESET);
                else 
                    ok = 1;
            }
            //roda_mil(alg);
            break;
        case 9:
            out = 1;
            return;
    }
    if (op == 1 || op == 2){
        while(!ok){
            printf("\nEm qual vetor fazer a operação[0-3]? ");
            scanf("%d", &v);
            if (v < 0 || v > 3)
                printf(RED"\nSlot inválido, tente novamente\n"RESET);
            else 
                ok = 1;
        }
        benchmark(op, alg, tipo, v);
    }
}

int main(){
    srand(time(NULL));
    cria_vetor(0);
    preencher_vetor(0);
    while (!out){
        system("clear");
        imprime_vetores();
        imprime_opcoes();
        faz_escolha();
        getchar();
    }
    destroi_vetores();
    return 0;
}
