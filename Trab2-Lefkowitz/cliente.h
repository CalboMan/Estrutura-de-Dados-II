#ifndef CLIENTE_H
#define CLIENTE_H

typedef struct Cliente {
    
    int cod;
    char Nome[50];
    int idade;

} Cliente;

void gerarTabelaA1Cliente();

#endif // CLIENTE_H