#include <stdio.h>

int main(void)
{
    int mes, dias;
    printf("Número do mês: ");
    if (scanf("%d", &mes) != 1) return 1;
    switch (mes) {
        case 2: dias = 28; break;
        case 4: case 6: case 9: case 11: dias = 30; break;
        case 1: case 3: case 5: case 7: case 8: case 10: case 12:
            dias = 31;
            break;
        default: puts("Mês inválido."); return 1;
    }
    printf("Dias: %d\n", dias);
    return 0;
}
