
 ## Arquivos e E/S



 Primeiramente, cabe explicar que foi criado dois programas de E/S, onde um (read.c) utiliza das funções open(), close() e read(), no qual abre um arquivo de texto , lê o que ta escrito nele e imprime na tela e o outro (write.c) utiliza open(), close() e write(), no qual abre um arquivo de texto, lê o texto que o usuário deseja escrever e imprime dentro desse arquivo. A chamada open() abre o arquivo de texto para ser lido e/ou escrito e o close() fecha o arquivo de texto
liberando os recursos associados ao arquivo aberto.

---

        
### Arquivo Read.c:

 Nessa parte do código, é utilizado a função open() no qual tenta abrir um arquivo (arqw.txt) no modo leitura, caso não de nenhum erro, o arquivo de texto é aberto.

  - **Open()**:
 
```
    char buffer[100]; / / Declara um buffer (armazenamento temporário) de 100 caracteres
    
    int fd = open("arqr.txt", O_RDONLY);  // Tenta abrir o arquivo "arqr.txt" em modo somente leitura
    
    // Verifica se houve erro na abertura do arquivo (fd == -1 indica erro)
    if (fd == -1) {
        printf("Erro ao abrir o arquivo txt\n");
        exit(1);
    }

    // Se chegou aqui, o arquivo foi aberto com sucesso
    printf("Arquivo de texto aberto.\n\n");
```

![read](https://github.com/user-attachments/assets/732edeac-cf66-443d-9775-a7f31a9c83ee)

Nesse programa, o arquivo arqr.txt é aberto pela função open() e em seguida é chamada a função read() para ler o texto escrito dentro desse arquivo, no qual é imprimido na tela, no final, o arquivo de texto é fechado pelo close(). Por se tratar de um programa focalizado na função read(), é utilizado o printf para imprimir o texto na tela, assim priorizando a chamada primitiva read().
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

  ![Captura de tela 2025-03-30 160131](https://github.com/user-attachments/assets/5e95092b-8e29-4438-ba62-d7bb49eab5e2)


  O texto que o usuário escreveu é lido, por meio da função fgets(), e escrito no arquivo de texto pela função write() e após isso o arquivo é fechado.

 <p>&nbsp;</p>

#### Usando o comando -strace -c ./write:

 ![Captura de tela 2025-03-30 160124](https://github.com/user-attachments/assets/ab46a789-9c42-459c-89c0-21c1944305b9)

 <p>&nbsp;</p>

### Observações:

openat (31.97%): A chamada openat representa uma parte significativa do tempo de execução, indicando que o programa passou uma quantidade considerável de tempo abrindo o arquivo arq.txt para leitura e escrita.

write (11.60%): A chamada write representa uma boa parte do tempo de execução, já que o programa tem foco na gravação dos dados no arquivo. No contexto do programa, isso está relacionado ao momento em que o texto digitado pelo usuário é efetivamente gravado dentro do arquivo de texto.

mprotect (11.00%): A chamada mprotect representa uma parte significativa do tempo de execução, essa função é usada para definir as permissões de leitura, escrita e execução de uma região específica da memória.


