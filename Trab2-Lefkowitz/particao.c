
//Por pedir apenas o arquivo de índices, aplicamos o Algoritmo de Lefkowitz apenas até obtermos A5, na quarta etapa. 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cliente.h"

// Estrutura para os Arquivos A2, A3 e A4
typedef struct 
{
    int reg;
    int cod; 
    int idade; 
    int ender;

} MoldeA2_A3_A4;

// Estrutura com ponteiros (usado em A5 e A6)
typedef struct 
{
    int idade; 
    int primeEnd; 
    int qtde; 
    
} MoldeA5;

typedef struct 
{
    int reg;
    int cod;
    int idade; 
    int ender;

} MoldeA6;

FILE* criarArquivoA2(FILE* a1); 
FILE* criarArquivosA3(); 
FILE* ordenarArquivosA3(); 
FILE* criarArquivoA5();
FILE* criarArquivoA6();

void imprimirArquivoA1();
void imprimirArquivoA2();
void imprimirArquivoA3();
void imprimirArquivoA4(); 
void imprimirArquivoA5();
void imprimirArquivoA6();

int main() 
{

    gerarTabelaA1(); // Arquivo A1 em cliente.c
    FILE *arqOrig = fopen("TabelaA1.dat", "rb");
    if (arqOrig == NULL) 
    {
        printf("Não foi possível abrir o Arquivo A1\n");
        exit(EXIT_FAILURE);
    }

    FILE* A2 = criarArquivoA2(arqOrig); // Primeira Etapa
    FILE* A3 = criarArquivosA3(); // Segunda Etapa
    FILE* A4 = ordenarArquivosA3(); // Terceira Etapa
    FILE* A5 = criarArquivoA5(); // Quarta Etapa
    FILE *A6 = criarArquivoA6(); // Quarta Etapa

    /*imprimirArquivoA1();
    imprimirArquivoA2();
    imprimirArquivoA3();
    imprimirArquivoA4();
    imprimirArquivoA5();*/
    imprimirArquivoA6();

    return 0;
}


FILE* criarArquivoA2(FILE *a1) 
{

    FILE *arquivoA2 = fopen("A2.dat", "wb");

    if(arquivoA2 == NULL)
    {
        printf("Erro ao criar o Arquivo A2.\n");
        exit(EXIT_FAILURE);
    }

    Cliente cliente; 
    // Cada registro será salvo no formato do Molde, até A1 terminar.
    int count = 1;
    while (fread(&cliente, sizeof(Cliente), 1, a1) > 0) 
    {
        MoldeA2_A3_A4 molde;  
        molde.reg = count;
        molde.cod = cliente.cod;
        molde.idade = cliente.idade;

        fwrite(&molde, sizeof(MoldeA2_A3_A4), 1, arquivoA2);
        count++;

    }

    fclose(a1);
    fclose(arquivoA2);

    printf("\nArquivo A2 gerado com êxito.\n");
    return arquivoA2;
}

FILE* criarArquivosA3() 
{
    FILE *a2 = fopen("A2.dat", "rb");
    FILE *arquivoA3 = fopen("A3.dat", "wb");

    if (a2 == NULL) 
    {
        printf("Erro ao abrir o Arquivo A2 em sua decomposição.\n");
        exit(EXIT_FAILURE);
    }

    if (arquivoA3 == NULL) 
    {
        printf("Erro ao criar o Arquivo A3.\n");
        exit(EXIT_FAILURE);
    }

    MoldeA2_A3_A4 molde;
    
    // Como há apenas o molde_a5 secundário da Idade, será gerado o mesmo Molde do anterior
    while (fread(&molde, sizeof(MoldeA2_A3_A4), 1, a2) > 0) 
    {
        fwrite(&molde, sizeof(MoldeA2_A3_A4), 1, arquivoA3);
    }

    fclose(a2);
    fclose(arquivoA3);
    printf("Arquivo A3 gerado com êxito.\n");

    return arquivoA3;
}

FILE* ordenarArquivosA3() 
{
    FILE *a3 = fopen("A3.dat", "rb");
    if (a3 == NULL) 
    {
        printf("Erro ao abrir o Arquivo A3 em sua ordenação.\n");
        exit(EXIT_FAILURE);
    }

    // Carregar registros em memórias. O contador nos informará quantos clientes temos
    MoldeA2_A3_A4 clientes[100];
    int count = 0;

    while (fread(&clientes[count], sizeof(MoldeA2_A3_A4), 1, a3) > 0) 
    {
        count++;
    }
    fclose(a3);

    // Ordenar os clientes por idade. Gambiarra
    for (int i = 0; i < count - 1; i++) 
    {
        for (int j = i + 1; j < count; j++) {
            if (clientes[i].idade > clientes[j].idade) 
            {
                MoldeA2_A3_A4 temp = clientes[i];
                clientes[i] = clientes[j];
                clientes[j] = temp;
            }
        }
    }

    FILE *arquivoA4 = fopen("A4.dat", "wb");
    if (arquivoA4 == NULL) 
    {
        printf("Erro ao criar o Arquivo A4.\n");
        exit(EXIT_FAILURE);
    }
    fwrite(clientes, sizeof(MoldeA2_A3_A4), count, arquivoA4);
    
    fclose(arquivoA4);
    printf("Arquivo A4 criado e ordenado por idade.\n");


}

FILE* criarArquivoA5() 
{
    FILE *a4 = fopen("A4.dat", "rb");
    FILE *arquivoA5 = fopen("A5.dat", "wb");

    if (a4 == NULL) 
    {
        printf("Erro ao abrir o Arquivo A4.\n");
        exit(EXIT_FAILURE);
    }

    if (arquivoA5 == NULL) 
    {
        printf("Erro ao criar o Arquivo A5.\n");
        exit(EXIT_FAILURE);
    }

    MoldeA2_A3_A4 molde_anterior, molde_atual; //É uma busca de comparação, onde se houver idades iguais Salvaremos as informações de PT e Q.
    MoldeA5 molde_a5 = {0, 0, 0};

    int prim = 1; 
    while (fread(&molde_atual, sizeof(MoldeA2_A3_A4), 1, a4) > 0) 
    {
        if (prim || molde_atual.idade != molde_anterior.idade) //Só é possível pois está ORDENADO por Idade como visto em A4
        {
            if (!prim) // Antes de escrever direto no arquivo A5, devemos verificar se é o primeiro registro a ser salvo com Idade X, para salvar em PT.
            {
                fwrite(&molde_a5, sizeof(MoldeA5), 1, arquivoA5);
            }
            molde_a5.idade = molde_atual.idade;
            molde_a5.qtde = 1;
            molde_a5.primeEnd = molde_atual.reg;
        } 
        else // Não é o primeiro registro de Idade X, e tem a mesma idade do anterior. Logo, Q da Idade X é aumentado.
        {
            molde_a5.qtde++;
        }

        molde_anterior = molde_atual; //Atualizamos o molde atual para o antigo e prosseguimos com o próximo até o fim do arquivo.
        prim = 0;
    }

    fwrite(&molde_a5, sizeof(MoldeA5), 1, arquivoA5); // Salvar o último índice

    fclose(a4);
    fclose(arquivoA5);
    printf("Arquivo A5 criado com êxito.\n");
    return arquivoA5;
}

FILE* criarArquivoA6()
{
    FILE *a4 = fopen("A4.dat", "rb");
    FILE *arquivoA6 = fopen("A6.dat", "wb");

    if (a4 == NULL) 
    {
        printf("Erro ao abrir o Arquivo A4.\n");
        exit(EXIT_FAILURE);
    }

    if (arquivoA6 == NULL) 
    {
        printf("Erro ao criar o Arquivo A6.\n");
        exit(EXIT_FAILURE);
    }

    MoldeA2_A3_A4 molde_atual, molde_prox; //Simlar ao anterior, onde se houver idades iguais, salvaremo o endereço anterior com o ED do atual.
 
    fread(&molde_prox, sizeof(MoldeA2_A3_A4), 1, a4);
    molde_atual = molde_prox;

    while (fread(&molde_prox, sizeof(MoldeA2_A3_A4), 1, a4) > 0) 
    {
        if(molde_atual.idade != molde_prox.idade)
        {
            molde_atual.ender = -1;
            fwrite(&molde_atual, sizeof(MoldeA2_A3_A4), 1, arquivoA6);
            //printf("Idade: %d\t Registro: %d\t Codigo: %d\t Endereço: %d\n", molde_atual.idade, molde_atual.reg, molde_atual.cod, molde_atual.ender);
        }
        else
        {
            molde_atual.ender = molde_prox.reg;
            fwrite(&molde_atual, sizeof(MoldeA2_A3_A4), 1, arquivoA6);
            //printf("Idade: %d\t Registro: %d\t Codigo: %d\t Endereço: %d\n", molde_atual.idade, molde_atual.reg, molde_atual.cod, molde_atual.ender);
        }
        molde_atual = molde_prox;

    } 
    
    fclose(arquivoA6);
    printf("Arquivo A6 criado com êxito.\n");
    return arquivoA6;

}


//IMPRESSÃO DOS ARQUIVOS

void imprimirArquivoA1()
{
    FILE *a1 = fopen("TabelaA1.dat", "rb");
    if (a1 == NULL) 
    {
        printf("Erro ao abrir o Arquivo A1 na impressão.\n");
        exit(EXIT_FAILURE);
    }

    printf("\n----------- Partição do Arquivo A1 -----------\n\n");

    Cliente molde;
    while (fread(&molde, sizeof(Cliente), 1, a1) > 0) 
    {
        printf("Codigo: %d\t Nome: %s\t Idade: %d\n", molde.cod, molde.nome, molde.idade);
    }
    fclose(a1);
}

void imprimirArquivoA2()
{
    FILE *a2 = fopen("A2.dat", "rb");
    if (a2 == NULL) 
    {
        printf("Erro ao abrir o Arquivo A2 na impressão.\n");
        exit(EXIT_FAILURE);
    }

    printf("\n----------- Partição do Arquivo A2 -----------\n\n");

    MoldeA2_A3_A4 molde;
    while (fread(&molde, sizeof(MoldeA2_A3_A4), 1, a2) > 0) 
    {
        printf("Registro: %d\t Codigo: %d\t Idade: %d\n", molde.reg, molde.cod, molde.idade);
    }
    fclose(a2);
}

void imprimirArquivoA3()
{
    FILE *a3 = fopen("A3.dat", "rb");
    if (a3 == NULL) 
    {
        printf("Erro ao abrir o Arquivo A3 na impressão.\n");
        exit(EXIT_FAILURE);
    }

    printf("\n----------- Partição do Arquivo A3 -----------\n\n");

    MoldeA2_A3_A4 molde;
    while (fread(&molde, sizeof(MoldeA2_A3_A4), 1, a3) > 0) 
    {
        printf("Idade: %d\t Registro: %d\t Codigo: %d\n ", molde.idade, molde.cod, molde.reg);
    }

    fclose(a3);
}

void imprimirArquivoA4()
{
    FILE *a4 = fopen("A4.dat", "rb");
    if (a4 == NULL) 
    {
        printf("Erro ao abrir o Arquivo A4 na impressão.\n");
        exit(EXIT_FAILURE);
    }

    printf("\n----------- Partição do Arquivo A4 -----------\n\n");

    MoldeA2_A3_A4 molde;
    while (fread(&molde, sizeof(MoldeA2_A3_A4), 1, a4) > 0) 
    {
        printf("Idade: %d\t Registro: %d\t Codigo: %d\n", molde.idade, molde.reg, molde.cod );
    }
    fclose(a4);

}



void imprimirArquivoA5() 
{
    FILE *a5 = fopen("A5.dat", "rb");
    if (a5 == NULL) 
    {
        printf("Erro ao abrir o arquivo A5.\n");
        exit(EXIT_FAILURE);
    }

    printf("\n----------- Partição do Arquivo A5 -----------\n\n");

    MoldeA5 registro;
    while (fread(&registro, sizeof(MoldeA5), 1, a5) > 0) 
    {
        printf("Idade: %d\t PT: %d\t\t Q: %d\n", registro.idade, registro.primeEnd, registro.qtde);
    }

    fclose(a5);

}

void imprimirArquivoA6() 
{
    FILE *a6 = fopen("A6.dat", "rb");

    if (a6 == NULL) {
        printf("Erro ao abrir o arquivo A6.\n");
        exit(EXIT_FAILURE);
    }

    MoldeA2_A3_A4 molde; // Para se adaptar conforme o Molde, primeEnd será o ENDEREÇO, não o PRIMEIRO ENDEREÇO. 

    printf("\n----------- Partição do Arquivo A6 -----------\n\n");

    while (fread(&molde, sizeof(MoldeA2_A3_A4), 1, a6) > 0) 
    {
        printf("Idade: %d\t Registro: %d\t Codigo: %d\t Endereço: %d\n", molde.idade, molde.reg, molde.cod, molde.ender);
    }

    fclose(a6);

}
