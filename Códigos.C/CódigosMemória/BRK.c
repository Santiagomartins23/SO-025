#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {
    void *heap_inicio = sbrk(0);
    printf("\nEndereco inicial da heap: %p\n", heap_inicio);

    size_t tamanho = 4096;

    void *novo_limite = sbrk(tamanho);

    if (novo_limite == (void *)-1) {
        perror("\nFalha ao expandir heap com sbrk");
        return 1;
    }

    printf("\nHeap expandida. Novo limite: %p\n", novo_limite);

    memset(novo_limite, 0, tamanho);

    char *mensagem = (char *)novo_limite;
    strcpy(mensagem, "Ola, esta string foi armazenada na heap!");

    printf("String armazenada na heap: %s\n", mensagem);

    if (sbrk(-tamanho) == (void *)-1) {
        perror("Falha ao liberar heap com sbrk");
        return 1;
    }

    printf("\nHeap reduzida ao estado inicial.\n");
    return 0;
}
