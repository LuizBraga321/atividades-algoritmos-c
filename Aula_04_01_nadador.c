#include <stdio.h>

int main(void)
{
    int idade;
    const char *categoria;
    printf("Idade do nadador: ");
    if (scanf("%d", &idade) != 1 || idade < 0) return 1;
    if (idade < 5) {
        puts("Idade fora das categorias.");
        return 0;
    }
    if (idade <= 7) categoria = "Infantil A";
    else if (idade <= 10) categoria = "Infantil B";
    else if (idade <= 13) categoria = "Juvenil A";
    else if (idade <= 17) categoria = "Juvenil B";
    else categoria = "Sênior";
    printf("Nadador de idade %d é da categoria %s.\n", idade, categoria);
    return 0;
}
