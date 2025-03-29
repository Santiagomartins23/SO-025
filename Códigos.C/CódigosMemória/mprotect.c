#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>

int main() {
    size_t size = 4096;

    void *mem = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (mem == MAP_FAILED) {
        perror("mmap falhou");
        return 1;
    }

    printf("\nMemoria alocada com sucesso: %p\n", mem);

    strcpy(mem, "Texto inicial na memoria.");
    printf("\nConteudo inicial na memoria: %s\n", (char *)mem);

    if (mprotect(mem, size, PROT_READ) == -1) {
        perror("\nmprotect falhou ao tornar a memoria somente leitura");
        return 1;
    }
    printf("\nMemoria agora e somente leitura.\n");

    if (mprotect(mem, size, PROT_READ) != -1) {
        printf("\n\nTentando escrever na memoria protegida...\n");
        strcpy(mem, "Tentativa de escrita em memoria somente leitura.");
    } else {
        printf("\nA escrita na memoria esta proibida por conta da protecao.\n");
    }

    printf("Leitura apos protecao: %s\n", (char *)mem);

    if (mprotect(mem, size, PROT_READ | PROT_WRITE) == -1) {
        perror("\nmprotect falhou ao tornar a memoria gravavel novamente");
        return 1;
    }
    printf("\nMemoria agora e gravavel novamente.\n");

    strcpy(mem, "Agora a escrita e permitida novamente.");
    printf("\n\n apos escrever novamente: %s\n", (char *)mem);

    if (mprotect(mem, size, PROT_NONE) == -1) {
        perror("\nmprotect falhou ao tornar a memoria sem permissao de acesso");
        return 1;
    }
    printf("\nMemoria agora sem permissao de acesso (nenhuma leitura ou escrita).\n");

    printf("\n\nTentando ler de memoria sem permissao (nao deve acontecer): ");
    if (mprotect(mem, size, PROT_READ) == 0) {
        printf("%s\n", (char *)mem);
    } else {
        printf("\nNao foi possivel ler a memoria devido a falta de permissao.\n");
    }

    if (mprotect(mem, size, PROT_WRITE) == 0) {
        strcpy(mem, "Tentando escrever em memoria sem permissao.");
    } else {
        printf("\nNao foi possivel escrever na memoria devido a falta de permissao.\n");
    }

    if (munmap(mem, size) == -1) {
        perror("munmap falhou");
        return 1;
    }

    printf("\nMemoria liberada com sucesso.\n");
    return 0;
}
