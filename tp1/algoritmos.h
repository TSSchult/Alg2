/* 
 * Trabalho Pratico I - Algoritmos II 
 * Arquivo de header para Algoritmos utilizados no trabalho.
 * Iniciado em 30/10/2024, por Theo Simonetti Schult.
 *
 * Este arquivo contém os tipos de dados e protótipos das funções necessárias
 * para ordenacao e busca em um vetor.
 *
*/

#ifndef ALGORITMOS
#define ALGORITMOS

/* Struct para retorno do numero de trocas e comparacoes, 
 * nas funcoes de busca, como nao ha trocas, o parametro
 * contém o indice do elemento encontrado*/
struct cont {
    int comp;
    int trocas;
};

/* Ordena um vetor utilizando o algoritmo quicksort o pivo 
 * eh escolhido por meio do parametro tipo, caso ele seja:
 * 1: o pivo eh o ultimo elemento do subvetor
 * 2: o pivo eh o primeiro elemento do subvetor
 * 3: o pivo eh o elemento do meio do subvetor
 * 4: o pivo eh a mediana entre 3 elementos aleatorios do
 *    subvetor, eh necessaria uma seed ja iniciada
 * Retorna uma struct cont com o n° de comparacoes e trocas*/
struct cont quicksort(int v[], int esq, int dir, int tipo); 

/* Ordena um vetor utilizando o algoritmo shellsort
 * os espaçamentos sao escolhidos pelo parametro tipo:
 * 1: o espacamento sao numeros primos pre-calculados
 * 2: o espacamento sao numeros dados por k(i+1) = 3k(i) + 1
 * 3: o espacamento sao numeros dados por k(i+1) = 2k(i)
 * Retorna uma struct cont com o n° de comparacoes e trocas*/
struct cont shellsort(int v[], int tam, int tipo);

/* Ordena um vetor utilizando o algoritmo bubblesort
 * Retorna uma struct cont com o n° de comparacoes e trocas*/
struct cont bubbleSort(int v[], int tam);

/* Ordena um vetor utilizando o algoritmo insertionsort
 * Retorna uma struct cont com o n° de comparacoes e trocas*/
struct cont insertSort(int v[], int tam);

/* Ordena um vetor utilizando o algoritmo bubblesort
 * Retorna uma struct cont com o n° de comparacoes e trocas*/
struct cont selectionSort(int v[], int tam);

/* Procura por um item dentro do vetor v utilizando busca 
 * binaria, retorna uma  struct com o n° de comparacoes e 
 * no campo trocas  da struct o  indice do elemento, caso 
 * esse seja -1, o elemento nao existe no vetor*/
struct cont buscaBin(int v[], int elem, int esq, int dir);

/* Procura por um item  dentro do vetor v utilizando busca 
 * sequencial (com sentinela), retorna uma struct com o n° 
 * de comparacoes e  no campo trocas da struct o indice do 
 * elemento, caso esse seja -1, a chave nao foi encontrada*/
struct cont buscaSeq(int v[], int elem, int tam);

#endif
