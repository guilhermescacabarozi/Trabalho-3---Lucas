#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> //para "isspace"
#include <stdint.h> //para "size_t" e "SIZE_MAX"
#include "Lista.h"

//cada nó guarda um bloco de até quatro dígitos
typedef struct No { //cria a struct do nó
    int bloco; 
    struct No *proximo;
} No;

//a lista representa o número completo
struct Lista { //cria a struct lista
    No *head; // cria o ponteiro do head
    int sinal;  //vamos assumir o valor 1 para positivo ou zero e -1 para negativo
};

// compara os módulos, sem considerar os sinais
// recebe listas válidas e sem zeros excedentes no final
static int comparar_modulos(const Lista *a, const Lista *b) {
    
    const No *pa = a->head;
    const No *pb = b->head;

    int resultado = 0;

    while (pa != NULL && pb != NULL) {
        // uma diferença em um bloco mais significativo
        // substitui a diferença encontrada anteriormente
        if (pa->bloco > pb->bloco) {
            resultado = 1;
        } else if (pa->bloco < pb->bloco) {
            resultado = -1;
        }

        pa = pa->proximo;
        pb = pb->proximo;
    }

    // se a ainda tem blocos, seu módulo é maior
    if (pa != NULL)
        return 1;

    // se b ainda tem blocos, seu módulo é maior
    if (pb != NULL)
        return -1;

    return resultado;
}


// adiciona um bloco ao final da lista
// retorna 1 se conseguiu ou 0 se houve falha
static int adicionar_bloco(Lista *lista, No **ultimo, int bloco) {
    if (lista == NULL || ultimo == NULL) //verifica a alocação
        return 0;

    if (bloco < 0 || bloco > 9999) //verifica se o bloco está dentro dos limites
        return 0;

    // se a lista não está vazia, precisamos do último nó
    if (lista->head != NULL && *ultimo == NULL){
        return 0;

    }

    No *novo = malloc(sizeof(No));

    if (novo == NULL) //verifica se o novo nó criado alocou
        return 0;

    novo->bloco = bloco; //o valor do bloco vira o valor do bloco do novo
    novo->proximo = NULL; //como é o ultimo, aponta pra nulo

    if (lista->head == NULL) {
        // o novo nó será o primeiro da lista
        lista->head = novo;
    } else {
        // conecta o último nó existente ao novo
        (*ultimo)->proximo = novo;
    }

    // atualiza o ponteiro ultimo da função que chamou
    *ultimo = novo;

    return 1;
}


// remove zeros excedentes na parte mais significativa
// mantém um único nó com valor 0 quando o resultado é zero
static void normalizar(Lista *lista) {
    if (lista == NULL || lista->head == NULL)
        return; //verifica se está alocado

    No *atual = lista->head; //primeiro da lista
    No *ultimo_nao_zero = NULL; 

    // encontra o último nó cujo bloco não é zero
    while (atual != NULL) {
        if (atual->bloco != 0)
            ultimo_nao_zero = atual;

        atual = atual->proximo; //avançando
    }

    if (ultimo_nao_zero == NULL) {
        // todos os blocos são zero: mantém apenas o primeiro
        atual = lista->head->proximo;
        lista->head->proximo = NULL;

        // zero sempre fica com sinal positivo
        lista->sinal = 1;
    } else {
        // separa os nós que vêm depois do último bloco não zero
        atual = ultimo_nao_zero->proximo;
        ultimo_nao_zero->proximo = NULL;
    }

    // libera os nós excedentes
    while (atual != NULL) {
        No *proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }
}

// le um número da entrada e devolve sua representação em lista
Lista *lista_ler(void) {
    // ler o numero como um texto
    size_t capacidade = 16; // armazenar a quantidade de posições disponíveis
    size_t tamanho = 0;    // armazenar a quantidade de caracteres guardados

    char *texto = malloc(capacidade * sizeof(char));  //criar um vetor que aloque a quantidade de caracteres necessários para o programa

    if (texto == NULL){ //se o texto for nulo
        return NULL; //retorna
    }
        
    int caractere;

    // pula espaços e quebras de linha antes do número
    do {
        caractere = getchar();
    } while (caractere != EOF &&
             isspace((unsigned char) caractere)); //executa enquanto ainda existir caractere e enquanto não tiver espaços entre o texto

    // agora, se não existe outro número na entrada, libera o vetor
    if (caractere == EOF) {
        free(texto); // libera o texto
        return NULL; //e retorna nulo
    }

    // le ate encontrar um espaço, quebra de linha ou fim da entrada
    while (caractere != EOF && !isspace((unsigned char) caractere)) { //executa enquanto ainda existir caractere e enquanto não tiver espaços entre o texto

        if (tamanho == capacidade - 1) { // -1 nesse caso é pra tirar o espaço de "\0"

            // evita ultrapassar o maior tamanho representável
            if (capacidade > SIZE_MAX / 2) { //se a capacidade dobrar, vai dar overflow?
                free(texto); // se der, libera
                return NULL; // função retorna nulo
            }

            size_t nova_capacidade = capacidade * 2; //multiplica a capacidade por 2 para o realloc

            // se realloc falhar usamos um ponteiro temporario para não perder o original
            char *novo_texto = realloc(texto, nova_capacidade);

            if (novo_texto == NULL) { //verifica a nova alocação
                free(texto); //libera o texto se nao der pra alocar
                return NULL; //retorna nulo
            }

            texto = novo_texto; //se valor de novo_texto vai pra texto
            capacidade = nova_capacidade; //e a capacidade vira a nova capacidade
        }

        
        texto[tamanho] = (char) caractere; // guarda o caractere na proxima posição
        tamanho++; //incrementa

        caractere = getchar(); // le o próximo caractere para continuar o laço
    }

    texto[tamanho] = '\0'; // fim da string

    Lista *lista = malloc(sizeof(Lista)); //cria uma lista

    if (lista == NULL) { // se a lista não for alocada
        free(texto); //libera o texto
        return NULL; //retorna
    }

    lista->head = NULL; // ainda não existem nos
    lista->sinal = 1;     // inicialmente consideramos não negativo para facilitar

    size_t inicio = 0; //posição inicial

    if (texto[0] == '-') { //se o texto for negativo
        lista->sinal = -1; // o sinal fica negativo
        inicio = 1; //e o começo se inicia pela segunda posição
    }

    while (inicio < tamanho - 1 && texto[inicio] == '0') { //verifica se nao tem "0" a esquerda do número
        inicio++; //incrementa
    }

    if (inicio == tamanho - 1 && texto[inicio] == '0') {
        lista->sinal = 1; //é 0 (0 = -0)
    }

    //vamos transformar o texto em blocos!

    // guarda o último nó para facilitar a inserção no final
    No *ultimo = NULL;

    // fim indica a posição imediatamente depois do trecho restante
    size_t fim = tamanho;

    // processa o número da direita para a esquerda
    while (fim > inicio) {  
        size_t comeco;

        if (fim - inicio > 4) {
            comeco = fim - 4; //se restam mais de quatro dígitos, pega os últimos quatro
        } else {
            comeco = inicio; // caso contrário pega todos os dígitos restantes
        }

        int bloco = 0;

        for (size_t i = comeco; i < fim; i++) { //convertendo da esquerda pra direita
            bloco = bloco * 10 + (texto[i] - '0'); //abre espaço para o proximo digito
        }

        No *novo = malloc(sizeof(No)); //cria um novo nó
        
        if (novo == NULL) { //se o nó não conseguir ser alocado
            lista_liberar(lista); // libera também os nós que já foram criados
            free(texto); //libera o texto
            return NULL; //encerra a lista
        }

        novo->bloco = bloco; //o nó do bloco assume o valor do bloco
        novo->proximo = NULL; //e o novo->proximo aponta pra nulo

        if (lista->head == NULL) {
            lista->head = novo; //se a lista está vazia, este é o primeiro nó
        } else {
            ultimo->proximo = novo; //liga o último nó existente ao novo no
            
        }

        //o nó recém-criado passa a ser o último
        ultimo = novo;

        //na próxima volta, processa o trecho anterior a este bloco
        fim = comeco;
    }

    // libera texto
    free(texto);

    //retorna lista
    return lista;
}


// libera todos os nós e depois a estrutura da lista
void lista_liberar(Lista *lista) { 
    if (lista == NULL){
        //verifica se a lista está alocada
        return;
    } 

    No *atual = lista->head; // cria um nó que aponta para o inicio da lista

    while (atual != NULL) { 
        // guardamos o próximo antes de liberar o nó atual
        No *proximo = atual->proximo;

        // libera a lista atual
        free(atual);
        atual = proximo; //avança para o proximo
    }

    free(lista); //dá free na lista
}

//comparando quais dos numeros é maior
int lista_comparar(const Lista *a, const Lista *b) {
    if (a == NULL || b == NULL) { //se algum ponteiro de lista for nulo
        return 2; //retorna 2 indicando erro
    }

    if (a->head == NULL || b->head == NULL) { //se alguma lista não tiver nós
        return 2; //retorna 2 indicando erro
    }

    //primeiro, vamos comparar os sinais
    if (a->sinal > b->sinal) {
        return 1; // a é maior
    }

    if (a->sinal < b->sinal) {
        return -1; //a é menor
    }

    //se os sinais sao iguais, conferimos a quantidade de digitos de cada um
    size_t digitos_a = 0;
    size_t digitos_b = 0;

    //vamos começar pelos primeiros nós
    const No *atual_a = a->head;
    const No *atual_b = b->head;

    //vamos contar os digitos do numero a
    while (atual_a != NULL) { //enquanto o ponteiro aponta para um nó
        if (atual_a->proximo != NULL) { //se não for o ultimo nó
            digitos_a += 4; //mais quatro digitos

        } else {
            // se for o ultimo bloco: conta somente os digitos que ele possui
            int bloco = atual_a->bloco; //copia o valor para conferir os digitos

            //vamos verificar as 4 casas decimais
            if (bloco < 10) { //se for menor que 10
                digitos_a += 1; // incrementa 1 digito
            } else if (bloco < 100) { // se for menor que 100
                digitos_a += 2; //incrementa 2 digitos
            } else if (bloco < 1000) { //se for menor que 1000
                digitos_a += 3; //incrementa 3 digitos
            } else {
                digitos_a += 4; //incrementa 4
            }
        }

        atual_a = atual_a->proximo; //vamos para o próximo; se era o ultimo, recebe NULL
    }

    //vamos contar os digitos do numero b
    while (atual_b != NULL) { //enquanto o ponteiro aponta para um nó
        if (atual_b->proximo != NULL) { //se não for o ultimo nó
            digitos_b += 4; //mais quatro digitos
        } else {
            // ultimo bloco: conta somente os digitos que ele possui
            int bloco = atual_b->bloco; //copia o valor para conferir os digitos

            //vamos verificar as casas decimais
            if (bloco < 10) { // se for menor que 10
                digitos_b += 1; //incrementa 1
            } else if (bloco < 100) { //se for menor que 100
                digitos_b += 2; //incrementa 2
            } else if (bloco < 1000) { //se for menor que 1000
                digitos_b += 3; //incrementa 3
            } else {
                digitos_b += 4; //incrementa 4
            }
        }

        atual_b = atual_b->proximo; //vamos para o próximo; se era o ultimo, recebe NULL
    }

    // caso 1: ambos positivos ou zero
    if (a->sinal == 1) {
        if (digitos_a > digitos_b) { //se a tiver mais digitos que b
            return 1; //a é maior, retorna 1
        }

        if (digitos_a < digitos_b) { //se b tiver mais digitos que a
            return -1; //a é menor, retorna -1
        }

        //para a mesma quantidade de digitos vamos comparar os blocos
        atual_a = a->head; //volta para o primeiro nó de a
        atual_b = b->head; //volta para o primeiro nó de b

        int resultado = 0; //começa considerando iguais, ate encontrar uma diferença

        //percorre do bloco menos significativo para o mais significativo
        while (atual_a != NULL && atual_b != NULL) {
            if (atual_a->bloco > atual_b->bloco) { //compara os blocos de uma mesma posição
                resultado = 1; //se o bloco de a for maior, guarda 1
            } else if (atual_a->bloco < atual_b->bloco) {
                resultado = -1; //se o bloco de a for menor, guarda -1
            }

            //se os blocos forem iguais, mantem o resultado anterior
            //sempre vai para os próximos, porque uma diferença depois tem mais importancia
            atual_a = atual_a->proximo;
            atual_b = atual_b->proximo;
        }

        return resultado; //retorna a ultima diferença encontrada, ou 0 se forem iguais
    }

    // caso 2: ambos negativos
    else {
        if (digitos_a > digitos_b) { //indica um numero negativo menor
            return -1; //retorna -1
        }

        if (digitos_a < digitos_b) { //indica um numero negativo maior
            return 1; //retorna 1
        }

        //para a mesma quantidade de digitos vamos comparar os blocos
        atual_a = a->head; //volta para o primeiro nó de a
        atual_b = b->head; //volta para o primeiro nó de b

        int resultado = 0; //começa considerando iguais, ate encontrar uma diferença

        //percorre do bloco menos significativo para o mais significativo
        while (atual_a != NULL && atual_b != NULL) {
            if (atual_a->bloco > atual_b->bloco) {
                resultado = -1; //como são negativos, bloco maior em a indica a menor
            } else if (atual_a->bloco < atual_b->bloco) {
                resultado = 1; //como são negativos, bloco menor em a indica a maior
            }

            //se os blocos forem iguais, mantem o resultado anterior
            //sempre vai para os próximos, porque uma diferença depois tem mais importancia
            atual_a = atual_a->proximo;
            atual_b = atual_b->proximo;
        }

        return resultado; //retorna a ultima diferença encontrada, ou 0 se forem iguais
    }
}

// imprime o número sem alterar a lista original
void lista_imprimir(const Lista *lista) {
    if (lista == NULL || lista->head == NULL) //verifica se está nula
        return; //retorna

    const No *atual = lista->head; // cria um nó pra atual
    No *invertida = NULL; // cria um nó para inverter

    // copia os blocos, inserindo cada novo nó no início
    // assim, a cópia fica na ordem contrária à original
    while (atual != NULL) {
        No *novo = malloc(sizeof(No)); //aloca um nó novo

        if (novo == NULL) {
            // libera os nós auxiliares se faltar memória
            while (invertida != NULL) {
                No *proximo = invertida->proximo; //vetor auxiliar pra percorrer
                free(invertida); //libera a invertida
                invertida = proximo; //assume o proximo como a invertida
            }

            return;
        }
        
        //cria como se fosse uma lista encadeada, so que começando pelo final
        //se o novo não for nulo
        novo->bloco = atual->bloco; //copia o valor do bloco atual
        novo->proximo = invertida; //o próximo se torna a invertida;
        invertida = novo; // e a invertida se torna o novo

        atual = atual->proximo; //a atual se torna atual->proximo
    }

    // identifica o zero para nunca imprimir -0
    int eh_zero = (invertida->bloco == 0 &&
                   invertida->proximo == NULL);

    if (lista->sinal == -1 && !eh_zero)
        printf("-");

    //invertida está na primeira posição do nó ja arrumado para a impressão
    No *aux = invertida;

    // imprime o bloco mais significativo sem zeros de preenchimento
    printf("%d", aux->bloco);
    aux = aux->proximo;

    // os demais blocos precisam ocupar exatamente quatro posições
    while (aux != NULL) {
        printf("%04d", aux->bloco);
        aux = aux->proximo;
    }

    // libera a cópia auxiliar depois da impressão
    while (invertida != NULL) {
        No *proximo = invertida->proximo;
        free(invertida);
        invertida = proximo;
    }
}

// soma dois números e retorna uma nova lista
// as listas a e b não são modificadas
Lista *lista_somar(const Lista *a, const Lista *b) {

    if (a == NULL || b == NULL){
        return NULL;   // verifica os ponteiros antes de acessar seus campos
    }

    // nesta implementação, todo número deve ter pelo menos um nó
    if (a->head == NULL || b->head == NULL)
        return NULL;

    if ((a->sinal != 1 && a->sinal != -1) || (b->sinal != 1 && b->sinal != -1)){// verifica se os sinais são válidos
        return NULL;
    }

    // cria a lista que vai guardar o resultado
    Lista *resultado = malloc(sizeof(Lista));

    if (resultado == NULL){ //verifica a alocação
        return NULL;
    }

    resultado->head = NULL; //lista está vazia
    resultado->sinal = 1; //sinal positivo

    // acompanha o último nó para inserir os próximos no final
    No *ultimo = NULL;

    if (a->sinal == b->sinal) {
        // sinais iguais: soma os módulos e mantém o sinal
        // exemplo: -10 + (-3) = -13
        resultado->sinal = a->sinal;

        const No *pa = a->head;
        const No *pb = b->head;

        int vai_um = 0;

        // continua enquanto houver blocos ou um valor para transportar
        while (pa != NULL || pb != NULL || vai_um != 0) {

            // começa com o transporte da soma anterior
            int soma = vai_um;

            // adiciona o bloco de a, se ainda existir
            if (pa != NULL) {
                soma += pa->bloco;
                pa = pa->proximo;
            }

            // adiciona o bloco de b, se ainda existir
            if (pb != NULL) {
                soma += pb->bloco;
                pb = pb->proximo;
            }

            // mantém apenas os quatro últimos dígitos no bloco
            int bloco = soma % 10000;

            // transporta o excesso para o próximo bloco
            vai_um = soma / 10000;

            // cria e conecta um nó com o bloco calculado
            if (!adicionar_bloco(resultado, &ultimo, bloco)) {
                lista_liberar(resultado);
                return NULL;
            }
        }

    } else {
        // sinais diferentes: subtrai o menor módulo do maior
        // exemplo: -10 + 3 = -(10 - 3) = -7
        int comparacao = comparar_modulos(a, b);

        const Lista *maior;
        const Lista *menor;

        if (comparacao >= 0) {
            maior = a;
            menor = b;
        } else {
            maior = b;
            menor = a;
        }

        // mantém o sinal do número com maior módulo
        resultado->sinal = maior->sinal;

        const No *pa = maior->head;
        const No *pb = menor->head;

        //pegar emprestado para realizar a subtração
        int emprestimo = 0;

        while (pa != NULL) {
            // desconta o empréstimo feito pelo bloco anterior
            int diferenca = pa->bloco - emprestimo;

            // subtrai o bloco correspondente, se existir
            if (pb != NULL) {
                diferenca -= pb->bloco;
                pb = pb->proximo;
            }

            if (diferenca < 0) {
                // empresta uma unidade da próxima posição
                // cada unidade dessa posição equivale a 10000
                diferenca += 10000;
                emprestimo = 1;
            } else {
                emprestimo = 0;
            }

            // guarda a diferença em um novo nó
            if (!adicionar_bloco(resultado, &ultimo, diferenca)) {
                lista_liberar(resultado);
                return NULL;
            }

            pa = pa->proximo;
        }
    }

    // remove blocos de zero excedentes na parte mais significativa
    // se o resultado for zero, deixa apenas um nó e sinal positivo
    normalizar(resultado);

    return resultado;
}
