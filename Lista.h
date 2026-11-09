#ifndef LISTA_H
#define LISTA_H

/* Os campos da estrutura serão definidos em Lista.c. */
typedef struct Lista Lista;

/* Lê um número da entrada e devolve sua lista. */
Lista *lista_ler(void);

/* Cria uma nova lista com o resultado de a + b. */
Lista *lista_somar(const Lista *a, const Lista *b);

/*
 * GABARITO:
 * -1: a < b;
 *  0: a == b;
 *  1: a > b;
 *  2: argumento invalido.
 */
int lista_comparar(const Lista *a, const Lista *b);

/*
 * Imprime somente o numero, sem "Resultado::" e sem quebra de linha.
 *
 * Retorna:
 * 1: impressao realizada;
 * 0: argumento invalido, falha de memoria ou erro de escrita.
 * Imprime o número representado pela lista. */

void lista_imprimir(const Lista *lista);

/* Libera os nós e a própria lista. */
void lista_liberar(Lista *lista);

#endif