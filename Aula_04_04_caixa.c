#include <stdio.h>

/* Se uma combinação de notas grandes falhar, tenta usar menos delas. */
static int montar(const int notas[], const int estoque[], int entrega[],
                  int i, int falta)
{
    if (i == 6) return falta == 0;
    int limite = falta / notas[i];
    int disponiveis = estoque[i] > 0 ? estoque[i] - 1 : 0;
    if (limite > disponiveis) limite = disponiveis;
    if (i == 5 && limite < falta / notas[i]) return 0;
    for (int qtd = limite; qtd >= 0; qtd--) {
        entrega[i] = qtd;
        if (montar(notas, estoque, entrega, i + 1, falta - qtd * notas[i])) {
            return 1;
        }
    }
    return 0;
}

int main(void)
{
    const int notas[] = {200, 100, 50, 20, 10, 5};
    int estoque[6], entrega[6] = {0}, saque;
    for (int i = 0; i < 6; i++) {
        printf("Quantidade de notas de R$ %d: ", notas[i]);
        if (scanf("%d", &estoque[i]) != 1 || estoque[i] < 0) return 1;
    }
    printf("Valor inteiro do saque: ");
    if (scanf("%d", &saque) != 1 || saque <= 0 || saque % 5) {
        puts("Valor inválido.");
        return 1;
    }
    if (!montar(notas, estoque, entrega, 0, saque)) {
        puts("Saque indisponível com a reserva de cédulas.");
        return 0;
    }
    int total = 0;
    for (int i = 0; i < 6; i++) {
        if (entrega[i]) printf("%d nota(s) de R$ %d\n", entrega[i], notas[i]);
        total += entrega[i];
    }
    printf("Total de cédulas: %d\n", total);
    return 0;
}
