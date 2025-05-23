#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    size_t size = 4096;

    
    int *mem = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0); //Aloca memória com mmap

    if (mem == MAP_FAILED) {
        perror("\nmmap falhou");
        return 1;
    }

    printf("\nMemoria alocada em: %p\n\n", mem);

  
    for (int i = 0; i < size / sizeof(int); i++) { //Preenche a memória com valores
        mem[i] = i * 10;
    }

    =
    printf("\nValores armazenados:\n");
    for (int i = 0; i < 10; i++) {
        printf("\nmem[%d] = %d\n", i, mem[i]);
    }

    =
    if (munmap(mem, size) == -1) {
        perror("munmap falhou");
        return 1;
    }

    printf("\n\nMemoria liberada com sucesso.\n");

    return 0;
}
