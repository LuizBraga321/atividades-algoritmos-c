#include <stdio.h>

int main(void)
{
    int idade;
    double altura;
    printf("Digite idade e altura: ");
    if (scanf("%d %lf", &idade, &altura) != 2) return 1;
    printf("Idade: %d anos\nAltura: %.2f m\n", idade, altura);
    return 0;
}
