#include <stdio.h>

int main(void)
{
    int a, b, c, maior, menor, meio;
    if (scanf("%d %d %d", &a, &b, &c) != 3) return 1;
    maior = a;
    menor = a;
    if (b > maior) maior = b;
    if (c > maior) maior = c;
    if (b < menor) menor = b;
    if (c < menor) menor = c;
    if (a >= menor && a <= maior && ((a <= b && a >= c) || (a >= b && a <= c))) meio = a;
    else if ((b >= a && b <= c) || (b <= a && b >= c)) meio = b;
    else meio = c;
    printf("Maior: %d\nMenor: %d\nIntermediário: %d\n", maior, menor, meio);
    printf("Repetidos: %s\n", a == b || a == c || b == c ? "sim" : "não");
    printf("Todos iguais: %s\n", a == b && b == c ? "sim" : "não");
    printf("Crescente: %s\n", a < b && b < c ? "sim" : "não");
    printf("Decrescente: %s\n", a > b && b > c ? "sim" : "não");
    return 0;
}
