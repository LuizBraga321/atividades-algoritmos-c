#include <stdio.h>

int main(void)
{
    for (;;) {
        int opcao, n;
        puts("1 - Par ou ímpar\n2 - Sinal\n3 - Quadrado\n4 - Sair");
        if (scanf("%d", &opcao) != 1) return 1;
        if (opcao >= 1 && opcao <= 3) {
            printf("Digite um inteiro: ");
            if (scanf("%d", &n) != 1) return 1;
        }
        switch (opcao) {
            case 1:
                puts(n % 2 == 0 ? "Par" : "Ímpar");
                /* A opção 1 também deve informar o sinal. */
                /* fall through */
            case 2:
                puts(n > 0 ? "Positivo" : n < 0 ? "Negativo" : "Zero");
                break;
            case 3: printf("Quadrado: %lld\n", 1LL*n*n); break;
            case 4: return 0;
            default: puts("Opção inválida.");
        }
    }
    return 0;
}
