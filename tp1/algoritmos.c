#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "algoritmos.h"

int primos[] = {
    511, 256, 127, 53, 23, 11, 3, 1
};

// troca 2 inteiros e atualiza a contagem 
void swap(int *a, int *b, struct cont *c) {
    int temp = *a;
    *a = *b;
    *b = temp;
    c->trocas++;  
}

// retorna a mediana entre 3 inteiros
int getmiddle(int v[], int a, int b, int c) {
    if (v[a] > v[b]) {
        if (v[b] > v[c]) {
            return b;
        } else if (v[a] > v[c]) {
            return c;
        } else {
            return a;
        }
    } else {
        if (v[a] > v[c]) {
            return a;
        } else if (v[b] > v[c]) {
            return c;
        } else {
            return b;
        }
    }
}

// funcao de particionamento para o quicksort. Toma 
// como pivo o ultimo elemento de um sub-vetor
int p_ult(int v[], int esq, int dir, struct cont *c) {
    int pivo = v[dir];
    int i = esq - 1;

    for (int j = esq; j < dir; j++) {
        c->comp += 2;  
        if (v[j] < pivo) {
            i++;
            swap(&v[i], &v[j], c);
        }
    }
    c->comp++;

    swap(&v[i + 1], &v[dir], c);
    return (i + 1);
}

// funcao de particionamento para o quicksort. Toma 
// como pivo o primeiro elemento de um sub-vetor
int p_ini(int v[], int esq, int dir, struct cont *c) {
    int pivo = v[esq];
    int i = esq + 1;

    for (int j = esq + 1; j <= dir; j++) {
        c->comp += 2; 
        if (v[j] < pivo) {
            swap(&v[i], &v[j], c);
            i++;
        }
    }
    c->comp++;

    swap(&v[esq], &v[i - 1], c);
    return (i - 1);
}

// funcao de particionamento para o quicksort. Toma 
// como pivo o elemento do meio de um sub-vetor
int p_meio(int v[], int esq, int dir, struct cont *c) {
    int meio = esq + (dir - esq) / 2;
    int pivo = v[meio];
    swap(&v[meio], &v[dir], c);

    int i = esq - 1;
    for (int j = esq; j < dir; j++) {
        c->comp += 2;
        if (v[j] < pivo) {
            i++;
            swap(&v[i], &v[j], c);
        }
    }
    c->comp++;

    swap(&v[i + 1], &v[dir], c);
    return (i + 1);
}

// funcao de particionamento para o quicksort. Toma como 
// pivo a mediana entre 3 elementos aleatorios de um sub-vetor
int p_medi(int v[], int esq, int dir, struct cont *c) {
    int x = esq + rand() % (dir - esq + 1);
    int y = esq + rand() % (dir - esq + 1);
    int z = esq + rand() % (dir - esq + 1);
    int ipivo = getmiddle(v, x, y, z);
    c->comp += 2;
    int pivo = v[ipivo];

    swap(&v[ipivo], &v[dir], c);
    int i = esq - 1;

    for (int j = esq; j < dir; j++) {
        c->comp += 2;
        if (v[j] < pivo) {
            i++;
            swap(&v[i], &v[j], c);
        }
    }
    c->comp++;

    swap(&v[i + 1], &v[dir], c);
    return (i + 1);
}

// funcao de controle do quicksort. Escolhe entre o tipo de particionamento
// e mantém a contagem das comparacoes e trocas feitas
struct cont quicksort(int v[], int esq, int dir, int tipo) {
    struct cont c = {0,0};  

    if (esq < dir) {
        int pivo;
        switch (tipo) {
            case 1:
                pivo = p_ult(v, esq, dir, &c);
                break;
            case 2:
                pivo = p_ini(v, esq, dir, &c);
                break;
            case 3:
                pivo = p_meio(v, esq, dir, &c);
                break;
            case 4:
                pivo = p_medi(v, esq, dir, &c);
                break;
            default:
                printf("Tipo de pivo inválido\n");
                return c;
        }

        struct cont l = quicksort(v, esq, pivo - 1, tipo);
        struct cont r = quicksort(v, pivo + 1, dir, tipo);

        c.comp += l.comp + r.comp;
        c.trocas += l.trocas + r.trocas;
    }

    return c;
}

// executa o shellsort com primos otimizados para 1024 elementos
struct cont shellsort_primos(int v[], int tam){
    struct cont c = {0,0};
    int num = sizeof(primos) / sizeof(primos[0]);

    for (int k = 0; k < num; k++) {
        c.comp++;
        int gap = primos[k];
        if (gap >= tam) continue; 
        c.comp++;

        for (int i = gap; i < tam; i++) {
            c.comp++;
            int aux = v[i];
            int j;
            for (j = i; j >= gap && v[j - gap] > aux; j -= gap) {
                c.comp += 2;
                v[j] = v[j - gap];
                c.trocas++;
            }
            c.comp++;
            v[j] = aux;
            if (j != i) c.trocas++;
        }
        c.comp++;
    }
    c.comp++;
    return c;
}

// executa o shellsort utilizando k = 3k + 1 como gerador de espacamentos
struct cont shellsort_3kmais1(int v[], int tam) {
    struct cont c = {0,0};
    int i, j, aux;
    int k = 1;

    while (k <= tam / 3) {
        k = 3 * k + 1;
        c.comp++;
    }
    c.comp++;

    while (k > 0) {
        for (i = k; i < tam; i++) {  
            aux = v[i];
            c.comp++;
            for (j = i; j >= k && v[j - k] > aux; j -= k) {
                v[j] = v[j - k];
                c.comp += 2;  
                c.trocas++; 
            }
            v[j] = aux;  
            c.comp += 1;
            if (j != i) c.trocas++;
        }
        c.comp++;
        k = k / 3;
    }
    c.comp++;
    return c;
}

// executa o shellsort com potencias de 2 como espacamento
struct cont shellsort_pot2(int v[], int tam){

    struct cont c = {0,0};
    int i, j, aux;
    int k = 1;

    while (k <= (tam / 2) + 1) {
        k = 2 * k;
        c.comp++;
    }
    c.comp++;

    while (k > 0) {
        for (i = k; i < tam; i++) {  
            aux = v[i];
            c.comp++;
            for (j = i; j >= k && v[j - k] > aux; j -= k) {
                v[j] = v[j - k];
                c.comp += 2;  
                c.trocas++; 
            }
            v[j] = aux;  
            c.comp += 1;
            if (j != i) c.trocas++;
        }
        c.comp++;
        k = k / 2;
    }
    c.comp++;
    return c;
}

// funcao unificadora do shellsort para escolher o tipo
struct cont shellsort(int v[], int tam, int tipo){
    switch (tipo){
        case 1:
            return shellsort_primos(v, tam);
            break;
        case 3:
            return shellsort_pot2(v, tam);
            break;
        default:
            return shellsort_3kmais1(v, tam);
    }
}

struct cont bubbleSort(int v[], int tam){
    struct cont c = {0,0};
    for (int i = 0; i < tam - 1; i++) {
        c.comp++;
        for (int j = 0; j < tam - i - 1; j++) {
            c.comp += 2;
            if (v[j] > v[j + 1]) {
                swap(&v[j], &v[j+1], &c);
            }
        }
    }
    return c;
}

struct cont insertSort(int v[], int tam){
    struct cont c = {0,0};
    for (int i = 1; i < tam; i++) {
        c.comp++;
        int chave = v[i];
        int j = i - 1;

        while (j >= 0 && v[j] > chave) {
            c.comp += 2;
            v[j + 1] = v[j];
            j--;
        }
        c.comp += 2;
        v[j + 1] = chave;

        if (j + 1 != i) {
            c.trocas++;
        }
    }
    c.comp++;
    return c;
}

struct cont selectionSort(int v[], int tam){
    struct cont c = {0,0};
    for (int i = 0; i < tam - 1; i++) {
        c.comp++;
        int min = i;
        
        for (int j = i + 1; j < tam; j++) {
            if (v[j] < v[min]) {
                min = j;
            }
            c.comp += 2;
        }
        c.comp++;
        
        swap(&v[i], &v[min], &c);
    }
    c.comp++;
    return c;
}

struct cont buscaBin(int v[], int elem, int esq, int dir) {
    struct cont c = {0, -2};  
    if (dir >= esq) {
        int meio = esq + (dir - esq) / 2;
        c.comp++;  
        
        if (v[meio] == elem) {   
            c.comp++;
            c.trocas = meio;    
            return c;
        }
        c.comp++;

        if (v[meio] < elem) {  
            c.comp++;
            struct cont res = buscaBin(v, elem, meio + 1, dir);
            res.comp += c.comp; 
            return res;
        }
        c.comp++;

        struct cont res = buscaBin(v, elem, esq, meio - 1);
        res.comp += c.comp;    
        return res;
    }
    c.comp++;  
    return c;
}


struct cont buscaSeq(int v[], int elem, int tam) {
    struct cont c = {0, -2};

    for (int i = 0; i < tam; i++) {
        c.comp++;             
        if (v[i] == elem) {   
            c.trocas = i;    
            return c;         
        }
    }

    return c;
}
