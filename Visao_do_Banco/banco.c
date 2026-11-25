#include "meu-banco.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <math.h>

/* Leitura validada, sem misturar scanf com fgets. */
static void lerLinha(const char *mensagem, char *buf, size_t tam) {
    int c;
    printf("%s", mensagem);
    if (!fgets(buf, (int)tam, stdin)) {
        puts("\nEntrada encerrada.");
        exit(EXIT_SUCCESS);
    }
    if (strchr(buf, '\n')) buf[strcspn(buf, "\n")] = '\0';
    else while ((c = getchar()) != '\n' && c != EOF) { }
}

int lerInteiro(const char *mensagem, int minimo, int maximo) {
    char buf[128], *fim;
    long valor;
    for (;;) {
        lerLinha(mensagem, buf, sizeof buf);
        errno = 0;
        valor = strtol(buf, &fim, 10);
        if (fim != buf && !errno) {
            while (isspace((unsigned char)*fim)) fim++;
            if (!*fim && valor >= minimo && valor <= maximo) return (int)valor;
        }
        puts("Opcao invalida. Tente novamente.");
    }
}

static double lerValor(const char *mensagem) {
    char buf[128], *fim, *p;
    double valor;
    for (;;) {
        lerLinha(mensagem, buf, sizeof buf);
        for (p = buf; *p; p++) if (*p == ',') *p = '.';
        errno = 0;
        valor = strtod(buf, &fim);
        if (fim != buf && !errno && isfinite(valor)) {
            while (isspace((unsigned char)*fim)) fim++;
            if (!*fim && valor >= 0.01 && valor <= 1e12)
                return round(valor * 100.0) / 100.0;
        }
        puts("Digite um valor positivo valido (ate R$ 1 trilhao).");
    }
}

static const char *nomeTipo(int tipo) {
    switch (tipo) {
        case CORRENTE: return "Corrente";
        case SALARIO: return "Salario";
        case POUPANCA: return "Poupanca";
        default: return "Desconhecida";
    }
}

static double saldoTotal(const Cliente *c) {
    return c->contas[0].saldo + c->contas[1].saldo + c->contas[2].saldo;
}

static void registrar(Conta *c, const char *descricao, double valor) {
    Transacao *t;
    if (c->qtdHistorico == HIST_MAX) {
        memmove(c->historico, c->historico + 1, (HIST_MAX - 1) * sizeof(Transacao));
        c->qtdHistorico--;
    }
    t = &c->historico[c->qtdHistorico++];
    snprintf(t->descricao, sizeof t->descricao, "%s", descricao);
    t->valor = valor;
    t->saldoFinal = c->saldo;
}

static void movimentar(Conta *c, double valor, const char *descricao) {
    c->saldo = round((c->saldo + valor) * 100.0) / 100.0;
    registrar(c, descricao, valor);
}

void iniciarBanco(Banco *b) {
    memset(b, 0, sizeof *b);
    b->proximoNumero = 1000;
}

void liberarBanco(Banco *b) {
    free(b->clientes);
    free(b->premios);
    iniciarBanco(b);
}

static void alertaCliente(const Cliente *c) {
    const Conta *a = &c->contas[CORRENTE - 1];
    if (a->numero && a->saldo < 0)
        printf("ALERTA: %s | %04d-01 | saldo negativo: R$ %.2f\n", c->nome, a->numero, a->saldo);
}

void alertarNegativos(const Banco *b) {
    size_t i;
    for (i = 0; i < b->quantidade; i++) alertaCliente(&b->clientes[i]);
}

static int criarConta(Banco *b, Cliente *c, int tipo) {
    Conta *a = &c->contas[tipo - 1];
    if (a->numero) { puts("O cliente ja possui esse tipo de conta."); return 0; }
    if ((tipo == CORRENTE && c->contas[SALARIO - 1].numero) ||
        (tipo == SALARIO && c->contas[CORRENTE - 1].numero)) {
        puts("Conta corrente e salario nao podem coexistir."); return 0;
    }
    if (b->proximoNumero > 9999) { puts("Limite de numeracao atingido."); return 0; }
    a->numero = b->proximoNumero++;
    a->tipo = (TipoConta)tipo;
    a->saldo = 0;
    registrar(a, "Abertura de conta", 0);
    printf("Conta criada: %04d-%02d (%s) | R$ 0.00\n", a->numero, tipo, nomeTipo(tipo));
    return 1;
}

static int cadastrarCliente(Banco *b) {
    Cliente novo = {0};
    Cliente *lista;
    int tipo;
    if (b->proximoNumero > 9999) { puts("Nao ha numeros de conta disponiveis."); return 0; }
    lista = realloc(b->clientes, (b->quantidade + 1) * sizeof *lista);
    if (!lista) { puts("Memoria insuficiente."); return 0; }
    b->clientes = lista;
    do { lerLinha("Nome do cliente: ", novo.nome, sizeof novo.nome); } while (!novo.nome[0]);
    puts("1 - Corrente | 2 - Salario | 3 - Poupanca");
    tipo = lerInteiro("Tipo da primeira conta: ", 1, 3);
    criarConta(b, &novo, tipo);
    if (tipo != POUPANCA && b->proximoNumero <= 9999 &&
        lerInteiro("Abrir tambem poupanca? (1-Sim, 0-Nao): ", 0, 1))
        criarConta(b, &novo, POUPANCA);
    b->clientes[b->quantidade++] = novo;
    return 1;
}

static Cliente *selecionarCliente(Banco *b) {
    size_t i;
    if (!b->quantidade) { puts("Nenhum cliente cadastrado."); return NULL; }
    for (i = 0; i < b->quantidade; i++) printf("%zu - %s\n", i + 1, b->clientes[i].nome);
    i = (size_t)lerInteiro("Cliente (0 cancela): ", 0, (int)b->quantidade);
    return i ? &b->clientes[i - 1] : NULL;
}

static void mostrarContas(const Cliente *c) {
    int i;
    printf("\nCliente: %s\n", c->nome);
    for (i = 0; i < 3; i++) {
        const Conta *a = &c->contas[i];
        if (a->numero) printf("%d - %04d-%02d | %s | R$ %.2f\n", i + 1, a->numero, i + 1, nomeTipo(i + 1), a->saldo);
    }
}

static Conta *selecionarConta(Cliente *c) {
    int tipo;
    mostrarContas(c);
    tipo = lerInteiro("Tipo da conta (0 cancela): ", 0, 3);
    if (!tipo) return NULL;
    if (!c->contas[tipo - 1].numero) { puts("Conta inexistente."); return NULL; }
    return &c->contas[tipo - 1];
}

static double limiteEmprestimo(const Cliente *c) {
    double total = saldoTotal(c);
    if (!c->contas[CORRENTE - 1].numero || !c->contas[POUPANCA - 1].numero ||
        c->contas[POUPANCA - 1].saldo <= 0 || c->emprestimoRealizado || total <= 0) return 0;
    /* Interpretacao: saldo total do cliente, corrente + poupanca. */
    return floor(total * 250.0 + 1e-7) / 100.0;
}

static int aptoTitulo(const Cliente *c) {
    const Conta *a = &c->contas[SALARIO - 1];
    return a->numero && a->saldo >= 1000.0 && a->saldo <= 1500.0 && !c->titulo;
}

static void emprestimo(Cliente *c) {
    double valor, limite = limiteEmprestimo(c);
    if (limite < 0.01) { puts("Cliente nao apto a emprestimo."); return; }
    printf("Limite de emprestimo: R$ %.2f\n", limite);
    valor = lerValor("Valor solicitado: R$ ");
    if (valor > limite) { puts("Valor superior ao limite."); return; }
    movimentar(&c->contas[CORRENTE - 1], valor, "Emprestimo");
    c->emprestimoRealizado = 1;
    puts("Emprestimo concedido.");
}

static void comprarTitulo(Cliente *c) {
    if (!aptoTitulo(c)) { puts("Titulo indisponivel: verifique saldo salario ou titulo ja adquirido."); return; }
    /* O enunciado nao estabelece preco para o titulo. */
    c->titulo = 1;
    registrar(&c->contas[SALARIO - 1], "Aquisicao de titulo", 0);
    puts("Titulo de capitalizacao adquirido (sem debito, preco nao informado).");
}

static void menuCliente(Banco *b, Cliente *c) {
    int op;
    Conta *a;
    double valor;
    do {
        mostrarContas(c);
        alertaCliente(c);
        puts("1 - Depositar\n2 - Sacar\n3 - Solicitar emprestimo\n4 - Adquirir titulo\n5 - Abrir outra conta\n0 - Voltar");
        op = lerInteiro("Opcao: ", 0, 5);
        switch (op) {
            case 1:
            case 2:
                a = selecionarConta(c);
                if (!a) break;
                valor = lerValor("Valor: R$ ");
                if (op == 2 && a->tipo != CORRENTE && valor > a->saldo) {
                    puts("Saldo insuficiente."); break;
                }
                movimentar(a, op == 1 ? valor : -valor, op == 1 ? "Deposito" : "Saque");
                puts("Operacao realizada."); break;
            case 3: emprestimo(c); break;
            case 4: comprarTitulo(c); break;
            case 5: criarConta(b, c, lerInteiro("1-Corrente 2-Salario 3-Poupanca: ", 1, 3)); break;
        }
    } while (op != 0);
}

void abrirBanco(Banco *b) {
    int n, i, op;
    Cliente *c;
    if (!b->aberto) {
        n = lerInteiro("Quantidade inicial de clientes: ", 1, 9000);
        for (i = 0; i < n; i++) {
            printf("\nCadastro %d de %d\n", i + 1, n);
            if (!cadastrarCliente(b)) break;
        }
        if (!b->quantidade) return;
        b->aberto = 1;
    }
    do {
        alertarNegativos(b);
        puts("\n== BANCO ABERTO ==\n1 - Acessar cliente\n2 - Cadastrar cliente\n0 - Voltar ao menu principal");
        op = lerInteiro("Opcao: ", 0, 2);
        if (op == 1) { c = selecionarCliente(b); if (c) menuCliente(b, c); }
        else if (op == 2) cadastrarCliente(b);
    } while (op != 0);
}

void consultarHistorico(const Banco *b) {
    size_t i;
    int j, k, encontrou = 0;
    double total = 0, limite;
    const Cliente *c;
    const Conta *a;
    for (i = 0; i < b->quantidade; i++) total += saldoTotal(&b->clientes[i]);
    printf("\n== Saldo Total do banco ==\n%zu | R$ %.2f\n", b->quantidade, total);
    puts("\n== Clientes Aptos a Emprestimos ==");
    for (i = 0; i < b->quantidade; i++) {
        c = &b->clientes[i]; limite = limiteEmprestimo(c);
        if (limite >= 0.01) { printf("%s | %04d-01 | R$ %.2f\n", c->nome, c->contas[CORRENTE - 1].numero, limite); encontrou = 1; }
    }
    if (!encontrou) puts("Nenhum.");
    encontrou = 0;
    puts("\n== Clientes com Saldo Negativo na Conta Corrente ==");
    for (i = 0; i < b->quantidade; i++) {
        c = &b->clientes[i]; a = &c->contas[CORRENTE - 1];
        if (a->numero && a->saldo < 0) { printf("%s | %04d-01 | R$ %.2f\n", c->nome, a->numero, a->saldo); encontrou = 1; }
    }
    if (!encontrou) puts("Nenhum.");
    encontrou = 0;
    puts("\n== Clientes Aptos a Compra de Titulos ==");
    for (i = 0; i < b->quantidade; i++) {
        c = &b->clientes[i]; a = &c->contas[SALARIO - 1];
        if (aptoTitulo(c)) { printf("%s | %04d-02 | R$ %.2f\n", c->nome, a->numero, a->saldo); encontrou = 1; }
    }
    if (!encontrou) puts("Nenhum.");
    puts("\n== Clientes Contemplados no Sorteio ==");
    if (!b->qtdPremios) puts("Nenhum.");
    for (i = 0; i < b->qtdPremios; i++) printf("%s | %04d-02 | R$ %.2f\n", b->premios[i].nome, b->premios[i].numero, b->premios[i].premio);
    puts("\n== Historico de transacoes (ultimos 5 por conta) ==");
    for (i = 0; i < b->quantidade; i++) {
        c = &b->clientes[i];
        for (j = 0; j < 3; j++) {
            a = &c->contas[j]; if (!a->numero) continue;
            printf("%s | %04d-%02d\n", c->nome, a->numero, j + 1);
            for (k = 0; k < a->qtdHistorico; k++) {
                const Transacao *t = &a->historico[k];
                printf("  %s | %+.2f | Saldo R$ %.2f\n", t->descricao, t->valor, t->saldoFinal);
            }
        }
    }
}

/* Variante de Fibonacci: escolhe um indice pela sequencia modulo 5.
   O algoritmo exato utilizado em sala nao foi fornecido. */
static unsigned fibonacci(unsigned n) {
    unsigned a = 0, b = 1, t, i;
    for (i = 0; i < n; i++) { t = (a + b) % 5; a = b; b = t; }
    return a;
}

void efetuarSorteio(Banco *b) {
    Cliente *top[5] = {NULL};
    Cliente *c;
    Conta *a;
    Premio *lista;
    size_t i;
    int j, k;
    double premio;
    unsigned indice;
    if (b->quantidade < 5) { puts("Sorteio exige pelo menos cinco clientes."); return; }
    /* Ordena somente os cinco maiores saldos totais, preservando ordem de cadastro em empates. */
    for (i = 0; i < b->quantidade; i++) {
        c = &b->clientes[i];
        for (j = 0; j < 5; j++) {
            if (!top[j] || saldoTotal(c) > saldoTotal(top[j])) {
                for (k = 4; k > j; k--) top[k] = top[k - 1];
                top[j] = c; break;
            }
        }
    }
    puts("\n== Cinco maiores saldos ==");
    for (j = 0; j < 5; j++) printf("%s | R$ %.2f | Titulo: %s\n", top[j]->nome, saldoTotal(top[j]), top[j]->titulo ? "Sim" : "Nao");
    for (j = 0; j < 5; j++) if (!top[j]->titulo) { puts("Sorteio cancelado: todos os cinco devem possuir titulo."); return; }
    /* Periodo de Fibonacci modulo 5 = 20. Cada indice ocorre quatro vezes. */
    indice = fibonacci((unsigned)(rand() % 20));
    c = top[indice];
    a = &c->contas[SALARIO - 1];
    premio = round(a->saldo * 1000.0) / 100.0;
    lista = realloc(b->premios, (b->qtdPremios + 1) * sizeof *lista);
    if (!lista) { puts("Memoria insuficiente. Sorteio cancelado."); return; }
    b->premios = lista;
    snprintf(lista[b->qtdPremios].nome, NOME_MAX, "%s", c->nome);
    lista[b->qtdPremios].numero = a->numero;
    lista[b->qtdPremios].premio = premio;
    b->qtdPremios++;
    movimentar(a, premio, "Premio do sorteio");
    printf("Vencedor: %s | %04d-02 | Premio: R$ %.2f | Novo saldo: R$ %.2f\n", c->nome, a->numero, premio, a->saldo);
}
