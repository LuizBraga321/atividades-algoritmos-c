#include <stdio.h>

int main(void)
{
    int a, b, c;
    printf("Digite três lados inteiros: ");
    if (scanf("%d %d %d", &a, &b, &c) != 3) return 1;
    if (a > b) { int aux = a; a = b; b = aux; }
    if (b > c) { int aux = b; b = c; c = aux; }
    if (a > b) { int aux = a; a = b; b = aux; }
    if (a <= 0 || (long long)a + b <= c) {
        puts("Os lados não formam um triângulo.");
        return 0;
    }
    if (a == c) puts("Equilátero");
    else if (a == b || b == c) puts("Isósceles");
    else puts("Escaleno");
    long long soma = 1LL*a*a + 1LL*b*b;
    long long maior = 1LL*c*c;
    if (soma == maior) puts("Retângulo");
    else if (soma > maior) puts("Acutângulo");
    else puts("Obtusângulo");
    return 0;
}
