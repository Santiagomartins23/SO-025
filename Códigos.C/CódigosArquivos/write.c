#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int main() {
    char ler[1000];
    write(1, "Digite o que deseja escrever no arquivo:\n", strlen("Digite o que deseja escrever no arquivo:\n"));

    if (fgets(ler, sizeof(ler), stdin) == NULL) {
        write(1, "Erro na leitura do input\n", strlen("Erro na leitura do input\n"));
        exit(1);
    }

    FILE *file = fopen("arqw.txt", "w");
    if (file == NULL) {
        write(1, "Erro ao abrir o arquivo txt\n", strlen("Erro ao abrir o arquivo txt\n"));
        exit(1);
    }

    fwrite(ler, 1, strlen(ler), file);


    write(1, "\nProcesso executado e fechando arquivo...\n\n", strlen("\nProcesso executado e fechando arquivo...\n\n"));
    fclose(file);
    return 0;
}
