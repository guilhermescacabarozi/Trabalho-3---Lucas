#include <stdio.h>
#include "Lista.h"
#include <string.h>


int main()
{
    int n;
    scanf("%d", &n);

    for(int i = 0; i < n; i++)
    {
        char palavra[6];

        scanf("%5s", palavra);

        Lista *a = lista_ler();
        Lista *b = lista_ler();

        if (a == NULL || b == NULL)
        {
            lista_liberar(a);
            lista_liberar(b);
            return 1;
        }
        printf("Resultado:: ");

        if(strcmp(palavra, "soma") == 0)
        {
            Lista *r = lista_somar(a,b);

            if (r == NULL)
            {
                lista_liberar(a);
                lista_liberar(b);
                return 1;
            }
            

            lista_imprimir(r);
            lista_liberar(r);
        }
        else
        {
            int comp = lista_comparar(a,b);
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
        lista_liberar(a);
        lista_liberar(b);

    }
    return 0;
}