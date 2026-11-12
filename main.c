/*
 * Trabalho 3 - Numeros Astronomicos
 * SCC0502 - Algoritmos e Estruturas de Dados I
 *
 * Clara Bomfim Moraes            - 17884439
 * Davi de Alcantara Lima         - 16861647
 * Guilherme Picolo Scacabarozi   - 17894159
 * Joao de Camargo Pinto          - 17965022
 * Uili Mare da Silva Pereira     - 14757341
 */
#include <stdio.h>
#include "Lista.h"
#include <string.h>


int main()
{
    int n; // o n representa o número de operações que a gente vai fazer
    scanf("%d", &n);

    for(int i = 0; i < n; i++)
    {
        char palavra[6];

        scanf("%5s", palavra);  // os comandos possíveis (soma, maior, menor, igual) têm no máximo 5 letras,
                                // e o vetor tem 6 posições por causa do '\0'
                                // o %5s impede o scanf de escrever pra 'fora' do vetor 
        Lista *a = lista_ler();
        Lista *b = lista_ler();

        if (a == NULL || b == NULL)  // lista_ler devolve NULL se faltar memória ou a entrada acabar
                                    // nesses casos, libera o que existe e encerra com erro (return 1)
        {
            lista_liberar(a);
            lista_liberar(b);
            return 1;
        }
        printf("Resultado:: ");

        if(strcmp(palavra, "soma") == 0)
        {
            Lista *r = lista_somar(a,b);

            if (r == NULL)       // a soma aloca um nó por bloco
                                // se faltar memoria, devolve NULL
                                // e o programa libera a e b e encerra com erro
            {
                lista_liberar(a);
                lista_liberar(b);
                return 1;
            }
            

            lista_imprimir(r);
            lista_liberar(r);  // r some no fim do if da soma, mas a lista que ele aponta continua no heap;
                               // liberamos aqui, enquanto ainda temos o endereço, pra não dar memory leak
        }
        else
        {
            int comp = lista_comparar(a,b); // comp: -1 se a < b, 0 se iguais, 1 se a > b
                                            // verdadeiro guarda a resposta de cada comando, pra imprimir True/False num lugar só
            int verdadeiro = 0;

            if (strcmp(palavra,"igual") == 0)
            {
                verdadeiro = (comp == 0);
            }

            if (strcmp(palavra, "maior") == 0)
            {
                verdadeiro = (comp == 1);
            }

            if (strcmp(palavra, "menor") == 0)
            {
                verdadeiro = (comp == -1);
            }

            if (verdadeiro == 1)
            {
                printf("True");
            }
            
            else
            {
                printf("False");
            }
        }

        printf("\n"); 
        lista_liberar(a);  // a e b somem no fim de cada volta, mas as listas continuam no heap
        lista_liberar(b);   // sem liberar aqui, cada volta acumularia 2 listas perdidas no heap (memory leak)

    }
    return 0;
}