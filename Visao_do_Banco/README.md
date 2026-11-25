# Visão do Banco — Parte I

Simulador de carteira digital em C (C99), baseado no enunciado da atividade.

## Como executar no Code::Blocks

1. Extraia o ZIP.
2. Abra o arquivo `Visao_do_Banco.cbp` no Code::Blocks com o compilador GCC/MinGW instalado.
3. Pressione F9 (Build and Run).

Alternativa pelo terminal: `gcc -std=c99 -Wall -Wextra main.c banco.c -lm -o banco`

## Arquivos

- `main.c`: menu principal e inicialização.
- `banco.c`: cadastro, contas, operações, empréstimo, títulos, sorteio e relatórios.
- `meu-banco.h`: estruturas, constantes e protótipos.
- `Visao_do_Banco.cbp`: projeto do Code::Blocks.

## Interpretações e decisões de implementação

- O texto exige alerta para saldo negativo na conta corrente, mas o exemplo do relatório menciona a conta salário. Foi seguida a regra do texto: conta corrente.
- O limite do empréstimo é 25% de dez vezes a soma dos saldos da corrente e poupança (2,5 vezes a soma). É concedido no máximo um por cliente e creditado na corrente.
- O algoritmo Fibonacci usado em sala não foi anexado. Foi utilizada uma variante que seleciona um termo aleatório de um período completo da sequência de Fibonacci módulo 5 (20 termos, com cada resultado de 0 a 4 aparecendo quatro vezes).
- O sorteio classifica todos os clientes pelo saldo total das contas. Os cinco primeiros precisam ter título; caso contrário, o sorteio é cancelado. Empates seguem a ordem de cadastro. O prêmio é dez vezes o saldo da conta salário, creditado nessa mesma conta.
- O preço de aquisição do título não foi informado; não há débito, e registra-se a aquisição de um título por cliente.
- Depósitos e saques permitem movimentar os saldos inicialmente zerados. Apenas a corrente pode ficar negativa. O histórico conserva as cinco últimas movimentações por conta; o relatório mantém todos os prêmios concedidos.
- Cada conta tem uma numeração própria de quatro dígitos, de 1000 a 9999, seguida de seu código de tipo. Os clientes são alocados dinamicamente, respeitando esse limite de numeração.
- Dados mantidos em memória durante a execução. Valores monetários em double arredondados para centavos, adequados à simulação didática, não a um sistema bancário real.

## Validação

Compilado com GCC usando `-std=c99 -Wall -Wextra -Wpedantic -Werror`. Testes de fluxo via terminal verificaram bloqueio antes de abrir o banco, saldo inicial, incompatibilidade de contas, empréstimo e bloqueio de empréstimo repetido, alerta de saldo negativo, limites de títulos, sorteio com e sem todos os títulos, valor do prêmio e histórico de cinco movimentações. O Code::Blocks em si não foi executado neste ambiente.
