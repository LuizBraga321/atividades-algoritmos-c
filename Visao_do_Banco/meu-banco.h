#ifndef MEU_BANCO_H
#define MEU_BANCO_H

#include <stddef.h>

#define HIST_MAX 5
#define NOME_MAX 100

typedef enum { CORRENTE = 1, SALARIO = 2, POUPANCA = 3 } TipoConta;

typedef struct {
    char descricao[40];
    double valor;
    double saldoFinal;
} Transacao;

typedef struct {
    int numero; /* Zero indica que a conta nao existe. */
    TipoConta tipo;
    double saldo;
    Transacao historico[HIST_MAX];
    int qtdHistorico;
} Conta;

typedef struct {
    char nome[NOME_MAX];
    Conta contas[3];
    int emprestimoRealizado;
    int titulo;
} Cliente;

typedef struct {
    char nome[NOME_MAX];
    int numero;
    double premio;
} Premio;

typedef struct {
    Cliente *clientes;
    size_t quantidade;
    int aberto;
    int proximoNumero;
    Premio *premios;
    size_t qtdPremios;
} Banco;

void iniciarBanco(Banco *b);
void liberarBanco(Banco *b);
void abrirBanco(Banco *b);
void consultarHistorico(const Banco *b);
void efetuarSorteio(Banco *b);
void alertarNegativos(const Banco *b);
int lerInteiro(const char *mensagem, int minimo, int maximo);

#endif
