#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    size_t size = 4096;  // 4 KB

    // Aloca mem�ria com mmap
    int *mem = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (mem == MAP_FAILED) {
        perror("\nmmap falhou");
        return 1;
    }

    printf("\nMemoria alocada em: %p\n\n", mem);

    // Preenche a mem�ria com valores
    for (int i = 0; i < size / sizeof(int); i++) {
        mem[i] = i * 10;
    }

    // Exibe os primeiros 10 valores armazenados
    printf("\nValores armazenados:\n");
    for (int i = 0; i < 10; i++) {
        printf("\nmem[%d] = %d\n", i, mem[i]);
    }

    // Libera a mem�ria alocada
    if (munmap(mem, size) == -1) {
        perror("munmap falhou");
        return 1;
    }

    printf("\n\nMemoria liberada com sucesso.\n");

    return 0;
}
