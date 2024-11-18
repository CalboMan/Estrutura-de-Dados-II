
//Por pedir apenas o arquivo de índices, aplicamos o Algoritmo de Lefkowitz apenas até obtermos A5, na quarta etapa. 



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cliente.h"
#include "cliente.c"

// Estrutura para os Arquivos A2, A3 e A4
typedef struct 
{
    int cod; 
    int idade; 
    long enderecoDisco; 

} MoldeA2_A3_A4;

// Estrutura com ponteiros (usado em A5 e A6)
typedef struct 
{
    int idade; 
    int quantidade; 
    long primeEnd; 
    
} MoldeA5_A6;


FILE* criarArquivoA2(FILE* a1); 
FILE* criarArquivosA3(); 
FILE* ordenarArquivosA3(); 
void criarArquivosA5A6(); 
FILE* imprimirArquivoA5();

/*
FILE* ordenarArquivoA6(); 
void imprimirArquivoA7();
*/

int main() 
{

    gerarTabelaA1(); // Arquivo A1 em cliente.c
    FILE *arqOrig = fopen("TabelaA1.dat", "rb");
    if (arqOrig == NULL) 
    {
        printf("Não foi possível abrir o Arquivo A1 para leitura!\n");
        exit(EXIT_FAILURE);
    }

    FILE* A2 = criarArquivoA2(arqOrig); // Primeira Etapa
    FILE* A3 = criarArquivosA3(); // Segunda Etapa
    FILE* A4 = ordenarArquivosA3(); // Terceira Etapa
    criarArquivosA5A6(); // Quarta Etapa
    FILE* A5  = imprimirArquivoA5(); // Resultado desejado

    //FILE* A7 = ordenarArquivoA6(); 
    //imprimirArquivoA7(); 

    return 0;
}


FILE* criarArquivoA2(FILE *a1) 
{

    FILE *arquivoA2 = fopen("A2.dat", "wb");

    if (a1 == NULL)
    { 
        printf("Erro ao criar o Arquivo A1 (Original).\n");
        exit(EXIT_FAILURE);
    }

    if(arquivoA2 == NULL)
    {
        printf("Erro ao criar o Arquivo A2.\n");
        exit(EXIT_FAILURE);
    }

    Cliente cliente; 
    long endereco = 0;
    // Para cada registro, será salvo no formato do Molde, junto com o endereço, e escrito no Arquivo A2, até A1 terminar.
    while (fread(&cliente, sizeof(Cliente), 1, a1) > 0) 
    {
        MoldeA2_A3_A4 molde;  
        molde.cod = cliente.cod;
        molde.idade = cliente.idade;
        molde.enderecoDisco = endereco;

        fwrite(&molde, sizeof(MoldeA2_A3_A4), 1, arquivoA2);
        endereco += sizeof(Cliente);
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
    
    // Como há apenas o indice secundário da Idade, será gerado o mesmo Molde do anterior, sem endereço. 
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
    printf("Arquivo A4 gerado e ordenado por idade.\n");
}

void criarArquivosA5A6() 
{
    FILE *a4 = fopen("A4.dat", "rb");
    FILE *arquivoA5 = fopen("A5.dat", "wb");
    FILE *arquivoA6 = fopen("A6.dat", "wb");

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

    if (arquivoA6 == NULL) 
    {
        printf("Erro ao criar o Arquivo A6.\n");
        exit(EXIT_FAILURE);
    }

    MoldeA2_A3_A4 Molde_antigo, Molde_atual;
    MoldeA5_A6 indice = {0, 0, 0};
    long ultimoEndereco = -1;

    int prim = 1;
    while (fread(&Molde_atual, sizeof(MoldeA2_A3_A4), 1, a4) > 0) 
    {
        if (prim || Molde_atual.idade != Molde_antigo.idade) {
            if (!prim) 
            {
                fwrite(&indice, sizeof(MoldeA5_A6), 1, arquivoA5);
            }
            indice.idade = Molde_atual.idade;
            indice.quantidade = 1;
            indice.primeEnd = Molde_atual.enderecoDisco;
        } 
        else {
            indice.quantidade++;
        }

        fwrite(&ultimoEndereco, sizeof(long), 1, arquivoA6); // Encadeamento
        ultimoEndereco = Molde_atual.enderecoDisco;
        Molde_antigo = Molde_atual;
        prim = 0;
    }

    fwrite(&indice, sizeof(MoldeA5_A6), 1, arquivoA5); // Salvar o último índice
    fwrite(&ultimoEndereco, sizeof(long), 1, arquivoA6); // Salvar o último encadeamento

    fclose(a4);
    fclose(arquivoA5);
    fclose(arquivoA6);
    printf("Arquivos A5 e A6 gerados com êxito.\n");
}

FILE* imprimirArquivoA5() 
{
    FILE *a5 = fopen("A5.dat", "rb");
    if (a5 == NULL) 
    {
        printf("Erro ao abrir o arquivo A5.\n");
        exit(EXIT_FAILURE);
    }

    printf("\n--- Partição do Arquivo A5 ---\n\n");
    MoldeA5_A6 registro;
    while (fread(&registro, sizeof(MoldeA5_A6), 1, a5) > 0) 
    {
        printf("Idade: %d, PT: %ld, Q: %d\n",
               registro.idade, registro.primeEnd, registro.quantidade);
    }

    fclose(a5);
    return a5;
}


/*

FILE* ordenarArquivoA6() {
    FILE *a6 = fopen("A6.dat", "rb");
    if (a6 == NULL) 
    {
        printf("Erro ao abrir o arquivo A6 em sua ordenação.\n");
        exit(EXIT_FAILURE);
    }

     // Ler o arquivo A6 em memória
    long enderecos[100]; // Supondo um limite arbitrário
    int count = 0;

    while (fread(&enderecos[count], sizeof(long), 1, a6) > 0) {
        count++;
    }
    fclose(a6);

    // Abrir o arquivo A1 (TabelaA1Cliente.dat) para buscar informações completas
    FILE *arquivoA1 = fopen("TabelaA1.dat", "rb");
    if (arquivoA1 == NULL) 
    {
        printf("Erro ao abrir o arquivo A1.\n");
        exit(EXIT_FAILURE);
    }

    // Estrutura para armazenar os registros do A7

    MoldeA2_A3_A4 registrosA7[100];

    // Recuperar informações do A1 com base nos endereços
    for (int i = 0; i < count; i++) 
    {
        fseek(arquivoA1, enderecos[i], SEEK_SET);
        Cliente cliente;
        fread(&cliente, sizeof(Cliente), 1, arquivoA1);

        registrosA7[i].cod = cliente.cod;
        registrosA7[i].idade = cliente.idade;
        registrosA7[i].enderecoDisco = enderecos[i];
    }
    fclose(arquivoA1);

    // Ordenar os registros por cod
    for (int i = 0; i < count - 1; i++) 
    {
        for (int j = i + 1; j < count; j++) 
        {
            if (registrosA7[i].cod > registrosA7[j].cod) 
            {
                // Troca os registros
                typeof(registrosA7[0]) temp = registrosA7[i];
                registrosA7[i] = registrosA7[j];
                registrosA7[j] = temp;
            }
        }
    }

    // Salvar no arquivo A7
    FILE *arquivoA7 = fopen("A7.dat", "wb");
    if (arquivoA7 == NULL) 
    {
        printf("Erro ao criar o arquivo A7.\n");
        exit(EXIT_FAILURE);
    }
    fwrite(registrosA7, sizeof(registrosA7[0]), count, arquivoA7);
    fclose(arquivoA7);

    printf("Arquivo A7 gerado e ordenado por chave primária.\n");
    return arquivoA7;
}


void imprimirArquivoA7() 
{
    FILE *a7 = fopen("A7.dat", "rb");
    if (a7 == NULL) 
    {
        printf("Erro ao abrir o arquivo A7 para leitura.\n");
        exit(EXIT_FAILURE);
    }

    MoldeA2_A3_A4 registro;

    printf("\n--- Conteúdo do Arquivo A7 ---\n");
    while (fread(&registro, sizeof(registro), 1, a7) > 0) 
    {
        printf("Chave Primária: %d, Idade: %d, Endereço: %ld\n",
               registro.cod, registro.idade, registro.enderecoDisco);
    }

    fclose(a7);
}

*/
