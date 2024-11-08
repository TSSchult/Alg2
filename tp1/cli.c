#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include "algoritmos.h"

#define MAX_SIZE 1024
#define MAX_NUM 2048
#define VEZES 1000 
#define MAX_SLOTS 4 

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

int out = 0, clean = 0;
int *vetores[MAX_SLOTS] = {NULL};
int ordenado[MAX_SLOTS] = {0};

void desenhar_quadro(int comp, double tempo, int trocas, int larg, int op, 
                     int medias,double dp_comp, double dp_tempo, 
                     double dp_trocas) {
    char b_comp[larg], b_tempo[larg], b_trocas[larg];

    if (medias){
        snprintf(b_comp, sizeof(b_comp), "Comparações: %d +- %.2f", comp, dp_comp);
        snprintf(b_tempo, sizeof(b_tempo), "Tempo: %.3f +- %.2fms", tempo, dp_tempo);
        if (op == 1)
            snprintf(b_trocas, sizeof(b_trocas), "Trocas: %d +- %.2f", trocas, dp_trocas);
        else
            snprintf(b_trocas, sizeof(b_trocas), "Casa: %d +- %.2f", trocas, dp_trocas);
    }
    else {
        snprintf(b_comp, sizeof(b_comp), "Comparações: %d", comp);
        snprintf(b_tempo, sizeof(b_tempo), "Tempo: %.3fms", tempo);
        if (op == 1)
            snprintf(b_trocas, sizeof(b_trocas), "Trocas: %d", trocas);
        else
            snprintf(b_trocas, sizeof(b_trocas), "Casa: %d", trocas);

    }

    int p_comp = (larg - (int)strlen(b_comp) - 2) / 2;
    int p_tempo = (larg - (int)strlen(b_tempo) - 2) / 2;
    int p_trocas = (larg - (int)strlen(b_trocas) - 2) / 2;

    printf("┌");
    for (int i = 0; i < larg - 2; i++) printf("─");
    printf("┐\n");

    printf("│%*s│\n", larg - 2, "");
    printf("│ "GRN"%*s%s%*s"RESET"│\n", p_comp, "", b_comp, larg - p_comp - (int)strlen(b_comp) - 1, "");
    printf("│"YEL"%*s%s%*s"RESET"│\n", p_trocas, "", b_trocas, larg - p_trocas - (int)strlen(b_trocas) - 2, "");
    printf("│"BLU"%*s%s%*s"RESET"│\n", p_tempo, "", b_tempo, larg - p_tempo - (int)strlen(b_tempo) - 2, "");
    printf("│%*s│\n", larg - 2, "");

    printf("└");
    for (int i = 0; i < larg - 2; i++) printf("─");
    printf("┘\n");
}

void preencher_vetor(int slot){
    for (int i = 0; i < MAX_SIZE; i++)
        vetores[slot][i] = (rand() % MAX_NUM) + 1;
}

int get_answer(int min, int max, int check_exists){
    int res = 0;
    if (scanf("%d", &res) != 1) {  
        printf(RED "\nEntrada inválida, tente novamente.\n\n" RESET);
        while (getchar() != '\n');  
    } 

    if (res < min || res > max)
        return -1;

    if (check_exists)
        if (vetores[res] == NULL)
            return -2;
    
    return res;
}

int vetor_ordenado(int slot){
    for (int i = 0; i < MAX_SIZE - 1; i++)
        if (vetores[slot][i] > vetores[slot][i+1])
            return 0;
    return 1;
}

void aloca_vetor(int **v){
    if (*v == NULL)
        *v = malloc(MAX_SIZE * sizeof(int));

    if (*v != NULL)
        for (int i = 0; i < MAX_SIZE; i++)
            (*v)[i] = (rand() % MAX_NUM) + 1; 
}

void cria_vetor(int slot) {
    char res;
    if (vetores[slot] != NULL){
        printf("\nSlot já ocupado, gostaria de sobrescrever [N/y]? ");
        getchar();
        res = getchar();
        if (res == 'Y' || res == 'y'){
            preencher_vetor(slot);
            ordenado[slot] = 0;
            printf(GRN"\nVetor sobrescrito\n"RESET);
        } 
        else {
            printf(RED"\nOperação abortada\n"RESET);
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
    if (slot < 0 || slot > MAX_SLOTS - 1){
        printf(RED"Slot inválido, abortando"RESET);
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

void esvazia_vetor(int slot){
    free(vetores[slot]);
    vetores[slot] = NULL;
}

int todos_vazios(){
    for (int i = 0; i < MAX_SLOTS; i++)
        if (vetores[i] != NULL)
            return 0;
    return 1;
}

void destroi_vetores(){
    for (int i = 0; i < MAX_SLOTS; i++)
        esvazia_vetor(i);
}

void copia_vetor(int orig, int dest){
    if (vetores[dest] == NULL){
        cria_vetor(dest);
    }

    for (int i = 0; i < MAX_SIZE; i++){
        vetores[dest][i] = vetores[orig][i];
    }
}


void calcula_medias(int com[], double tempos[], int swaps[], double *m_te, 
                    double *m_c, double *m_tr, double *dp_te, double *dp_c,
                    double *dp_tr){
    long long n_c = 0, n_tr = 0;
    double n_te = 0;
    for (int i = 0; i < VEZES; i++){
        n_c += com[i];
        n_te += tempos[i];
        n_tr += swaps[i];
    }
    *m_c = (double)n_c / VEZES;
    *m_te = n_te / VEZES;
    *m_tr = (double)n_tr / VEZES;

    double aux = 0;
    for (int i = 0; i < VEZES; i++){
        aux = com[i] - *m_c;
        *dp_c += aux*aux; 
        aux = tempos[i] - *m_te;
        *dp_te += aux*aux; 
        aux = swaps[i] - *m_tr;
        *dp_tr += aux*aux; 
    }
    *dp_c = *dp_c/VEZES;
    *dp_te = *dp_te/VEZES;
    *dp_tr = *dp_tr/VEZES;

    *dp_c = sqrt(*dp_c);
    *dp_te = sqrt(*dp_te);
    *dp_tr = sqrt(*dp_tr);
}

void imprime_escolha(int alg, int tipo, int slot){
    printf("\nExecutando ");
    if (alg == 1) {
        printf("Quicksort ");
        if (tipo == 1) printf("com o pivô sendo o último elemento ");
        if (tipo == 2) printf("com o pivô sendo o primeiro elemento ");
        if (tipo == 3) printf("com o pivô sendo o elemento do meio");
        if (tipo == 4) printf("com o pivô sendo a mediana");
    }
    if (alg == 2) {
        printf("Shellsort ");
        if (tipo == 1) printf("com o espaçamento customizado");
        if (tipo == 2) printf("com o espacamento 3k + 1");
        if (tipo == 3) printf("com o espacamento 2^n");
    }
    if (alg == 3) printf("Bubblesort ");
    if (alg == 4) printf("Insertsort ");
    if (alg == 5) printf("Selectionsort ");

    printf("no vetor número %d:\n", slot);
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
        ordenado[slot] = 1;
        double tempo = ((double)t)/CLOCKS_PER_SEC*1000;
        imprime_escolha(alg, tipo, slot);
        desenhar_quadro(res.comp, tempo, res.trocas, 30, op, 0,0,0,0);
        return;
    }
    if (tipo == 0)
        tipo = (rand() % MAX_NUM) + 1;

    if (alg == 1) {
        printf("Procurando pelo número %d com busca sequencial:\n", tipo);
        t = clock();
        res = buscaSeq(vetores[slot], tipo, MAX_SIZE);
        t = clock() - t;
    } else {
        printf("Procurando pelo número %d com busca binária:\n", tipo);
        t = clock();
        res = buscaBin(vetores[slot], tipo, 0, MAX_SIZE - 1);
        t = clock() - t;
    }
    double tempo = ((double)t)/CLOCKS_PER_SEC*1000;
    desenhar_quadro(res.comp, tempo, res.trocas + 1, 30, op,0,0,0,0);
}

void roda_mil(int alg){
    struct cont res;
    clock_t t;
    int *v = NULL, swaps[VEZES], com[VEZES];
    double tempos[VEZES];
    aloca_vetor(&v);
    if (v == NULL){
        printf(RED"\nFalha na alocação! abortando...\n"RESET);
        return;
    }
    for (int i = 0; i < VEZES; i++){
        aloca_vetor(&v);
        switch (alg){
            case 1:
                t = clock();
                res = quicksort(v, 0, MAX_SIZE - 1, 1);
                t = clock() - t;
                break;
            case 2:
                t = clock();
                res = quicksort(v, 0, MAX_SIZE - 1, 2);
                t = clock() - t;
                break;
            case 3:
                t = clock();
                res = quicksort(v, 0, MAX_SIZE - 1, 3);
                t = clock() - t;
                break;
            case 4:
                t = clock();
                res = quicksort(v, 0, MAX_SIZE - 1, 4);
                t = clock() - t;
                break;
            case 5:
                t = clock();
                res = shellsort(v, MAX_SIZE, 1);
                t = clock() - t;
                break;
            case 6:
                t = clock();
                res = shellsort(v, MAX_SIZE, 2);
                t = clock() - t;
                break;
            case 7:
                t = clock();
                res = shellsort(v, MAX_SIZE, 3);
                t = clock() - t;
                break;
            case 8:
                t = clock();
                res = bubbleSort(v, MAX_SIZE);
                t = clock() - t;
                break;
            case 9:
                t = clock();
                res = insertSort(v, MAX_SIZE);
                t = clock() - t;
                break;
            case 10:
                t = clock();
                res = selectionSort(v, MAX_SIZE);
                t = clock() - t;
                break;
            case 11:
                t = clock();
                res = buscaSeq(v,(rand() % MAX_NUM) + 1, MAX_SIZE);
                t = clock() - t;
                break;
            case 12:
                quicksort(v, 0, MAX_SIZE - 1, 4);
                t = clock();
                res = buscaBin(v, (rand() % MAX_NUM) + 1,0, MAX_SIZE - 1);
                t = clock() - t;
                break;
        }
        double tempo = ((double)t)/CLOCKS_PER_SEC*1000;
        tempos[i] = tempo;
        swaps[i] = res.trocas;
        com[i] = res.comp;
    }
    double m_te=0, m_c=0, m_tr=0, dp_te=0, dp_c=0, dp_tr=0; 
    calcula_medias(com, tempos, swaps, &m_te, &m_c, 
                   &m_tr, &dp_te, &dp_c, &dp_tr);
    int op = 1;
    if (alg == 11 || alg == 12) op = 0;
    printf("\nResultados:\n");
    desenhar_quadro(m_c, m_te, m_tr, 42, op, 1, dp_c, dp_te, dp_tr);
    free(v);
}

void imprime_opcoes(){
    printf("Opções:\n");
    printf(GRN"1 - Ordenar algum vetor         \n");
    printf(GRN"2 - Buscar em algum vetor       \n");
    printf(YEL"3 - Criar novo vetor aleatório  \n");
    printf(YEL"4 - Copiar vetor em outro espaço\n");
    printf(YEL"5 - Imprimir um intervalo       \n");
    printf(BLU"6 - Esvaziar vetor              \n");
    printf(BLU"7 - Limpar tela                 \n");
    printf(RED "8 - Rodar %dx             \n" RESET, VEZES);
    printf("9 - Sair                        \n\n");
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
    printf("3 - k = 2^n                                     \n\n");
}

void busca_escolhas(){
    printf("\n1 - Busca Sequencial\n");
    printf("2 - Busca Binária\n\n");
}

void algoritmos_escolhas(){
    printf("\n1 -  Quicksort (ultimo)\n");
    printf("2 -  Quicksort(primeiro)\n");
    printf("3 -  Quicksort(meio)\n");
    printf("4 -  Quicksort(mediana)\n");
    printf("5 -  Shellsort(custom)\n");
    printf("6 -  Shellsort(3k+1)\n");
    printf("7 -  Shellsort(2^n)\n");
    printf("8 -  BubbleSort\n");
    printf("9 -  InsertionSort\n");
    printf("10 - SelectionSort\n\n");
    printf("11 - Busca Sequencial\n");
    printf("12 - Busca Binária\n");
}

void faz_escolha(){
    int op = 1, alg = 1, ok = 0, tipo = 1, slot = 0, orig, dest, v;
    while (!ok){
        printf("O que você gostaria de fazer [1-9]? ");
        op = get_answer(1, 9, 0);
        if (op != -1 && op != -2) 
            ok = 1;
    }
    ok = 0;
    switch (op){
        case 1:
            if (todos_vazios()){
                printf(RED"\nNão existem vetores para ordenar, abortando...\n\n"RESET);
                return;
            }
            while (!ok){
                ordena_escolhas();
                alg = get_answer(1, 5, 0);
                if (alg != -1 && alg != -2)
                    ok = 1;
            }
            ok = 0;
            switch (alg){
                case 1:
                    quick_escolhas();
                    while (!ok){
                        printf("Qual tipo de pivô gostaria de utilizar[1-4]? ");
                        tipo = get_answer(1, 4, 0);
                        if (tipo != -1 && tipo != -2)
                            ok = 1;
                    } 
                    break;
                case 2:
                    shell_escolhas();
                    while (!ok){
                        printf("Qual tipo de k gostaria de utilizar[1-3]? ");
                        tipo = get_answer(1, 3, 0);
                        if (tipo != -1 && tipo != -2)
                            ok = 1;
                    }  
                    break;
            }
            break;
        case 2:
            if (todos_vazios()){
                printf(RED"\nNão existem vetores para buscar, abortando...\n\n"RESET);
                return;
            }
            busca_escolhas();
            while (!ok){
                printf("\nQual tipo de busca gostaria de utilizar[1/2]? ");
                alg = get_answer(1, 2, 0);
                if (alg != -1 && alg != -2)
                    ok = 1;
            }
            ok = 0;
            while (!ok){
                printf("\n1 - Número aleatório\n2 - Pesquisa customizada\n \
                       \nGostaria de escolher o valor a ser procurado[1/2]? ");
                tipo = get_answer(1, 2, 0);
                if (tipo != -1 && tipo != -2)
                    ok = 1;
            }
            ok = 0;
            while (!ok){
                if (tipo == 2){
                    printf("\n\nPor qual número procurar[1-2048]? ");
                    dest = get_answer(1,MAX_NUM,0);
                    if (dest != -1 && dest != -2)
                        ok = 1;
                }
                else {
                    dest = 0;
                    ok = 1;
                }
            }
            tipo = dest;
            break;
        case 3:
            while(!ok){
                printf("\nEm qual slot guardar o vetor[0-%d]? ", MAX_SLOTS - 1);
                slot = get_answer(0,MAX_SLOTS - 1, 0);
                if (slot != -1 && slot != -2)
                    ok = 1;
            }
            cria_vetor(slot);
            break;
        case 4:
            if(todos_vazios()){
                printf(RED"\nNenhum vetor para copiar :D\n\n"RESET);
                return;
            }
            while(!ok){
                printf("\nQual vetor gostaria de copiar[0-%d]? ", MAX_SLOTS - 1);
                orig = get_answer(1, MAX_SLOTS - 1, 1);
                if (orig != -1 && orig != -2)
                    ok = 1;
            }
            ok = 0;
            while(!ok){
                printf("\nPara qual vetor copiar[0-%d]? ", MAX_SLOTS - 1);
                dest = get_answer(0, MAX_SLOTS - 1, 0);
                if (dest != -1 && dest != -2)
                    ok = 1;
            }
            copia_vetor(orig, dest);
            break;
        case 5:
            if(todos_vazios()){
                printf(RED"\nNenhum vetor para consultar :D\n\n"RESET);
                return;
            }
            while(!ok){
                printf("\nQual vetor consultar[0-%d]? ", MAX_SLOTS - 1);
                v = get_answer(0, MAX_SLOTS - 1, 1);
                if (v != -1 && v != -2)
                    ok = 1;
            }
            ok = 0;
            while(!ok){
                printf("\nDe qual posição[1-%d]? ", MAX_SIZE);
                orig = get_answer(1, MAX_SIZE, 0);
                if (orig != -1 && orig != -2)
                    ok = 1;
            }
            ok = 0;
            while(!ok){
                printf("\nAté que posição[%d-%d]? ", orig, MAX_SIZE);
                dest = get_answer(1, MAX_SIZE, 0);
                if (dest < orig)
                    printf(RED"\nFim inválido, tente novamente\n"RESET);
                else if (dest != -1 && dest != -2)
                    ok = 1;
            }
            printf("\n");
            mostra_intervalo(v, orig, dest);
            printf("\n");
            break;
        case 6:
            if(todos_vazios()){
                printf(RED"\nNenhum vetor para esvaziar :D\n\n"RESET);
                return;
            }
            while(!ok){
                printf("\nQue vetor esvaziar[0-%d]? ", MAX_SLOTS - 1);
                v = get_answer(0, MAX_SLOTS - 1, 0);
                if (v != -1 && v != -2)
                    ok = 1;
            }
            if (vetores[v] == NULL)
                printf(YEL"\nVetor já está vazio!\n" RESET);
            else 
                esvazia_vetor(v);
            ordenado[v] = 0;
            break;
        case 7:
            system("clear");
            clean = 1;
            break;
        case 8:
            while(!ok){
                algoritmos_escolhas();
                printf("\nQual algoritmo gostaria de rodar %d vezes[1-12]? ", VEZES);
                alg = get_answer(1, 12, 0);
                if (alg != -1 && alg != -2)
                    ok = 1;
            }
            roda_mil(alg);
            break;
        case 9:
            out = 1;
            printf(GRN"Bye!\n\n"RESET);
            return;
    }
    ok = 0;
    if (op == 1 || op == 2){
        while(!ok){
            printf("\nEm qual vetor fazer a operação[0-%d]? ", MAX_SLOTS - 1);
            v = get_answer(0, MAX_SLOTS - 1, 1);
            if (v != -1 && v != -2)
                ok = 1;
        }
        if (op == 2 && alg == 2 && !ordenado[v]){
            printf(RED"\nVetor escolhido não está ordenado, abortando...\n\n"RESET);
            return;
        }
        benchmark(op, alg, tipo, v);
    }
}

int main(){
    srand(time(NULL));
    cria_vetor(0);
    preencher_vetor(0);
    system("clear");
    while (!out){
        imprime_vetores();
        imprime_opcoes();
        faz_escolha();
        if (out) break;
        if (!clean){
            printf("Gostaria de limpar a tela[y/N]? ");
            getchar();
            char c = getchar();
            if (c == 'y' || c == 'Y')
                system("clear");
            clean = 0;
        }
    }
    destroi_vetores();
    return 0;
}
