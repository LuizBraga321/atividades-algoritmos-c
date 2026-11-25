#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "meu-banco.h"

int main(void) {
    Banco banco;
    int opcao;
    iniciarBanco(&banco);
    srand((unsigned)time(NULL));

    do {
        puts("\n========== VISAO DO BANCO ==========");
        puts("1 - Abrir o banco");
        puts("2 - Consultar historico");
        puts("3 - Efetuar sorteio");
        puts("0 - Sair");
        opcao = lerInteiro("Opcao: ", 0, 3);
        if (opcao == 1) abrirBanco(&banco);
        else if (opcao == 2 || opcao == 3) {
            if (!banco.aberto) puts("Abra o banco primeiro.");
            else {
                alertarNegativos(&banco);
                if (opcao == 2) consultarHistorico(&banco);
                else efetuarSorteio(&banco);
            }
        }
    } while (opcao != 0);

    liberarBanco(&banco);
    puts("Banco encerrado.");
    return 0;
}
