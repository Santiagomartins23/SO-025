#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main() {
    char ler[1000];

    int file = open("arqw.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (file == -1) {
        write(1, "Erro ao abrir o arquivo txt\n", strlen("Erro ao abrir o arquivo txt\n"));
        exit(1);
    }

    write(1, "Digite o que deseja escrever no arquivo:\n", strlen("Digite o que deseja escrever no arquivo:\n"));

    if (fgets(ler, sizeof(ler), stdin) == NULL) {
        write(1, "Erro na leitura do input\n", strlen("Erro na leitura do input\n"));
        exit(1);
    }





    if (write(file, ler, strlen(ler)) == -1) {
        write(1, "Erro ao escrever no arquivo\n", strlen("Erro ao escrever no arquivo\n"));
        close(file);
        exit(1);
    }

    write(1, "\nProcesso executado e fechando arquivo...\n\n", strlen("\nProcesso executado e fechando arquivo...\n\n"));


    close(file);
    return 0;
}
