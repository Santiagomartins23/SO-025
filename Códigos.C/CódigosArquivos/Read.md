
 ## Arquivos e E/S



 Primeiramente, cabe explicar que foi criado dois programas de E/S, onde um (read.c) utiliza das funções open(), close() e read(), no qual abre um arquivo de texto , lê o que ta escrito nele e imprime na tela e o outro (write.c) utiliza open(), close() e write(), no qual abre um arquivo de texto, lê o texto que o usuário deseja escrever e imprime dentro desse arquivo. A chamada open() abre o arquivo de texto para ser lido e/ou escrito e o close() fecha o arquivo de texto
liberando os recursos associados ao arquivo aberto.

---

        
### Arquivo Read.c:

![read](https://github.com/user-attachments/assets/732edeac-cf66-443d-9775-a7f31a9c83ee)

Nesse programa, o arquivo arqr.txt é aberto pela função open() e em seguida é chamada a função read() para ler o texto escrito dentro desse arquivo, no qual é imprimido na tela, no final, o arquivo de texto é fechado pelo close().
<p>&nbsp;</p>

#### Usando o comando -strace -c ./read, vamos conseguir estatísticas relacionadas ao uso das chamadas ao sistema e o tempo gasto por elas:

 ![strace read](https://github.com/user-attachments/assets/8c07de01-0c0f-4882-939a-6cd1aa8a9552)
<p>&nbsp;</p>

### Observações:

execve (48.47%): Quase metade do tempo de execução foi dedicada à chamada de sistema execve, que é responsável por carregar e executar programas.

mmap (16.10%): O mmap representa o segundo maior consumo de tempo, mapeando arquivos ou dispositivos diretamente na memória.

read (8.52%):  A chamada read está consumindo uma parte considerável do tempo de execução. Isso indica que o programa está, de fato, realizando a leitura do arquivo de texto.

write (5.99%): Embora o programa tenha o foco principal na leitura do arquivo de texto, o fato de gastar tempo com write sugere que também há uma etapa de processamento que gera dados a serem gravados.

---
<p>&nbsp;</p>

### Arquivo Write.c: 
 Inicialmente o arquivo chamado arqw.txt é aberto pela função open() e em seguida o programa pede para o usuário escrever o que deseja imprimir nesse arquivo.
 
 ![write1](https://github.com/user-attachments/assets/b12ed9d1-8cd5-47f0-adbf-555f0f6b16ec)
 
 <p>&nbsp;</p>

#### Usando um texto de exemplo:

  ![write2](https://github.com/user-attachments/assets/dd8979bc-31cd-4559-b1be-617804612df3)

  O texto que o usuário escreveu é lido e escrito no arquivo de texto pela função write() e após isso o arquivo é fechado.

 <p>&nbsp;</p>

#### Usando o comando -strace -c ./write:

 ![strace write](https://github.com/user-attachments/assets/0a5016e9-124e-422f-9424-ef26580b64e4)
 <p>&nbsp;</p>

### Observações:

openat (23.98%): A chamada openat representa uma parte significativa do tempo de execução, indicando que o programa passou uma quantidade considerável de tempo abrindo o arquivo arq.txt para leitura e escrita.

mmap (15.72%): A chamada mmap mapeia o arquivo ou parte dele diretamente para a memória.

write (10.30%): A chamada write representa uma boa parte do tempo de execução, já que o programa tem foco na gravação dos dados no arquivo. No contexto do programa, isso está relacionado ao momento em que o texto digitado pelo usuário é efetivamente gravado dentro do arquivo de texto.

close (8.26%): Embora o tempo dedicado ao close seja menor, ele ainda é uma etapa crucial para liberar os recursos do sistema e garantir que o arquivo seja fechado corretamente após a operação.

read.c

```
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
```

<p>&nbsp;</p>

write.c


```
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




   
    


