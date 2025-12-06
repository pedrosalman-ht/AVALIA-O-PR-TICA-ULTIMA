#include <stdio.h>
#include <string.h>

struct Conta {
    int numero;
    char nome[50];
    float saldo;
};

int genNum(FILE *arq) {
    struct Conta c;
    int ult = 0;

    rewind(arq);

    while (fread(&c, sizeof(struct Conta), 1, arq) == 1) {
        ult = c.numero;
    }

    return ult + 1;
}

int buscar(FILE *arq, int num, struct Conta *out, long *pos) {
    struct Conta temp;
    rewind(arq);
    *pos = -1;

    while (fread(&temp, sizeof(struct Conta), 1, arq) == 1) {
        if (temp.numero == num) {
            *out = temp;
            *pos = ftell(arq) - sizeof(struct Conta);
            return 1;
        }
    }
    return 0;
}

int main() {
    FILE *arq = fopen("contas.dat", "r+b");
    if (arq == NULL) {
        arq = fopen("contas.dat", "w+b");
    }

    struct Conta c, ori, des;
    long p1, p2;
    int op, n1, n2;
    float val;

    char senha[20] = "2605";  
    char senhaDig[20];

    do {
        printf("\nBANCO SALMAN\n");
        printf("1 Criar conta\n");
        printf("2 Extrato\n");
        printf("3 Deposito\n");
        printf("4 Saque\n");
        printf("5 Transferencia\n");
        printf("6 Listar contas\n");
        printf("7 Sair\n");
        printf("Opcao: ");
        scanf("%d", &op);
        getchar();

        if (op == 1) {
            c.numero = genNum(arq);
            c.saldo = 0;

            printf("Nome: ");
            fgets(c.nome, 50, stdin);
            c.nome[strcspn(c.nome, "\n")] = 0;

            fseek(arq, 0, SEEK_END);
            fwrite(&c, sizeof(struct Conta), 1, arq);

            printf("Conta criada! Numero: %d\n", c.numero);
        }

        else if (op == 2) {
            printf("Numero: ");
            scanf("%d", &n1);

            if (buscar(arq, n1, &c, &p1)) {
                printf("Nome: %s\n", c.nome);
                printf("Saldo: R$ %.2f\n", c.saldo);
            } else {
                printf("Conta nao encontrada.\n");
            }
        }

        else if (op == 3) {
            printf("Numero: ");
            scanf("%d", &n1);
            printf("Valor: ");
            scanf("%f", &val);

            if (buscar(arq, n1, &c, &p1)) {
                c.saldo += val;

                fseek(arq, p1, SEEK_SET);
                fwrite(&c, sizeof(struct Conta), 1, arq);

                printf("Deposito feito.\n");
            } else {
                printf("Conta nao encontrada.\n");
            }
        }

        else if (op == 4) {
            printf("Numero: ");
            scanf("%d", &n1);
            printf("Valor: ");
            scanf("%f", &val);

            if (buscar(arq, n1, &c, &p1)) {
                if (c.saldo >= val) {
                    c.saldo -= val;

                    fseek(arq, p1, SEEK_SET);
                    fwrite(&c, sizeof(struct Conta), 1, arq);

                    printf("Saque feito.\n");
                } else {
                    printf("Saldo insuficiente.\n");
                }
            } else {
                printf("Conta nao encontrada.\n");
            }
        }

        else if (op == 5) {
            printf("Origem: ");
            scanf("%d", &n1);
            printf("Destino: ");
            scanf("%d", &n2);
            printf("Valor: ");
            scanf("%f", &val);

            if (!buscar(arq, n1, &ori, &p1)) {
                printf("Conta origem nao existe.\n");
                continue;
            }
            if (!buscar(arq, n2, &des, &p2)) {
                printf("Conta destino nao existe.\n");
                continue;
            }
            if (ori.saldo < val) {
                printf("Sem saldo.\n");
                continue;
            }

            ori.saldo -= val;
            des.saldo += val;

            fseek(arq, p1, SEEK_SET);
            fwrite(&ori, sizeof(struct Conta), 1, arq);

            fseek(arq, p2, SEEK_SET);
            fwrite(&des, sizeof(struct Conta), 1, arq);

            printf("Transferencia OK.\n");
        }

        else if (op == 6) {
            printf("Senha: ");
            scanf("%s", senhaDig);

            if (strcmp(senhaDig, senha) != 0) {
                printf("Senha errada.\n");
                continue;
            }

            rewind(arq);
            printf("\nTODAS AS CONTAS:\n");

            while (fread(&c, sizeof(struct Conta), 1, arq) == 1) {
                printf("Conta %d / %s / R$ %.2f\n",
                       c.numero, c.nome, c.saldo);
            }
        }

    } while (op != 7);

    fclose(arq);
    return 0;
}



