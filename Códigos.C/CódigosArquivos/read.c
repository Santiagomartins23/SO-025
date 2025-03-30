#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <locale.h>

int main() {
    char buffer[100];

    printf("Arquivo de texto aberto.\n\n");


    int fd = open("arqr.txt", O_RDONLY);
    if (fd == -1) {
        printf("Erro ao abrir o arquivo txt\n");
        exit(1);
    }

    printf("Texto lido:\n");


    ssize_t bytes_lidos;
    while ((bytes_lidos = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
        buffer[bytes_lidos] = '\0';
        printf("%s", buffer);
    }

    printf("\nFechando arquivo...\n");
    close(fd);

    return 0;
}
