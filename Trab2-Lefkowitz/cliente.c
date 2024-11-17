#include "cliente.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define QUANTIDADE_CLIENTES 54

void gerarTabelaA1() {
    Cliente clientes[QUANTIDADE_CLIENTES] = {
        {29, "Alice Silva", 35},
        {14, "Bruno Souza", 42},
        {76, "Carlos Pereira", 27},
        {75, "Daniela Costa", 50},
        {59, "Eduardo Lima", 33},
        {6, "Fernanda Alves", 28},
        {7, "Gabriel Rocha", 45},
        {74, "Helena Martins", 39},
        {48, "Igor Fernandes", 25},
        {46, "Juliana Ribeiro", 31},
        {10, "Lucas Oliveira", 22},
        {18, "Mariana Santos", 29},
        {56, "Nicolas Mendes", 40},
        {20, "Olivia Araujo", 36},
        {26, "Pedro Carvalho", 24},
        {4, "Quintino Barros", 38},
        {21, "Rafaela Teixeira", 30},
        {65, "Samuel Correia", 43},
        {22, "Tatiana Almeida", 26},
        {49, "Ubirajara Nunes", 51},
        {11, "Valentina Gomes", 19},
        {16, "Wagner Dias", 48},
        {8, "Xavier Pinto", 34},
        {15, "Yasmin Ferreira", 21},
        {5, "Zeca Moraes", 37},
        {19, "Ana Paula", 28},
        {50, "Bruna Figueiredo", 41},
        {55, "Caio Batista", 33},
        {25, "Diana Lopes", 46},
        {66, "Elias Castro", 49},
        {57, "Fabiana Silva", 22},
        {77, "Gustavo Almeida", 29},
        {12, "Heloisa Santos", 20},
        {30, "Isabela Costa", 32},
        {17, "João Pedro", 44},
        {9, "Karina Souza", 23},
        {54, "Leonardo Lima", 47},
        {78, "Marcos Rocha", 25},
        {43, "Natália Ribeiro", 31},
        {38, "Otávio Fernandes", 50},
        {51, "Patrícia Oliveira", 29},
        {32, "Renato Mendes", 27},
        {58, "Sabrina Araujo", 45},
        {13, "Thiago Correia", 33},
        {73, "Ursula Teixeira", 40},
        {79, "Vitor Almeida", 30},
        {27, "Wesley Nunes", 22},
        {1, "Ximena Gomes", 19},
        {3, "Yuri Dias", 26},
        {60, "Zara Pinto", 38},
        {36, "Arthur Ferreira", 28},
        {47, "Beatriz Moraes", 25},
        {0, "César Barros", 44},
        {2, "Débora Batista", 35}
    };

    FILE *out = fopen("TabelaA1.dat", "wb+");

    if(out == NULL){
        printf("Erro ao gerar arquivo de tabela cliente");
        return;
    }

    fwrite(clientes, sizeof(Cliente), QUANTIDADE_CLIENTES, out);

    fclose(out);
}
