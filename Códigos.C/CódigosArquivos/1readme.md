
 ## Arquivos e E/S



 Primeiramente, cabe explicar que foi criado dois programas de E/S, onde um (read.c) utiliza das funções open(), close() e read(), no qual abre um arquivo de texto , lê o que ta escrito nele e imprime na tela e o outro (write.c) utiliza open(), close() e write(), no qual abre um arquivo de texto, lê o texto que o usuário deseja escrever e imprime dentro desse arquivo. A chamada open() abre o arquivo de texto para ser lido e/ou escrito e o close() fecha o arquivo de texto
liberando os recursos associados ao arquivo aberto.

---

        
### Arquivo Read.c:

 Na primeira parte do código, é utilizado a função open() no qual tenta abrir um arquivo (arqw.txt) no modo leitura, caso não de nenhum erro, o arquivo de texto é aberto. Após isso, é utilizado a função read() dentro da estrutura de repetição While, assim o programa irá ler pedaços de 99 bytes (buffer - 1) e imprimir na tela até que chegar ao fim do texto/arquivo. Assim, o programa é finalizado e a função close() é usada para fechar o arquivo de texto.

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

- **Read() e Close()**:
  
```
    ssize_t bytes_lidos;  // Variável para armazenar quantos bytes foram lidos em cada operação
    
    
    while ((bytes_lidos = read(fd, buffer, sizeof(buffer) - 1)) > 0) {   // Lê pedaços de até 99 bytes (sizeof(buffer)-1) de cada vez
        buffer[bytes_lidos] = '\0';
        
         
        printf("%s", buffer);   // Imprime o conteúdo lido
    }

    printf("\nFechando arquivo...\n");
    
    close(fd); // Fecha o arquivo (liberando os recursos do sistema)
```
<p>&nbsp;</p>

#### Arquivo compilado:


Utilizando um texto de exemplo:


![read](https://github.com/user-attachments/assets/732edeac-cf66-443d-9775-a7f31a9c83ee)

 
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

 - **Open()**:

 ```
// Abre o arquivo "arqw.txt" com as seguintes flags:
// O_WRONLY - Aberto apenas para escrita
// O_CREAT - Cria o arquivo se não existir
// O_TRUNC - Apaga o conteúdo existente se o arquivo já existir
// As permissões são definidas como S_IRUSR | S_IWUSR (leitura e escrita para o dono)
int file = open("arqw.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

if (file == -1) {
    write(1, "Erro ao abrir o arquivo txt\n", strlen("Erro ao abrir o arquivo txt\n"));
    exit(1);
}

write(1, "Digite o que deseja escrever no arquivo:\n", strlen("Digite o que deseja escrever no arquivo:\n"));

```
 
 <p>&nbsp;</p>

 O texto que o usuário escreveu é lido, por meio da função fgets(), e escrito no arquivo de texto pela função write() e após isso o arquivo é fechado.
 
- **Read() e Close()**:


```

if (fgets(ler, sizeof(ler), stdin) == NULL) {   // Lê a entrada do usuário usando fgets
    
    write(1, "Erro na leitura do input\n", strlen("Erro na leitura do input\n"));
    
    exit(1);
}


if (write(file, ler, strlen(ler)) == -1) {  // Escreve o conteúdo lido (ler) no arquivo aberto
   
    write(1, "Erro ao escrever no arquivo\n", strlen("Erro ao escrever no arquivo\n"));
    
    close(file);
    
    exit(1);
}

write(1, "\nProcesso executado e fechando arquivo...\n\n", 
     strlen("\nProcesso executado e fechando arquivo...\n\n"));

close(file);
```
<p>&nbsp;</p>

 #### Arquivo compilado:


![write1](https://github.com/user-attachments/assets/b12ed9d1-8cd5-47f0-adbf-555f0f6b16ec)


#### Usando um texto de exemplo:

  ![Captura de tela 2025-03-30 160131](https://github.com/user-attachments/assets/5e95092b-8e29-4438-ba62-d7bb49eab5e2)


  
 <p>&nbsp;</p>

#### Usando o comando -strace -c ./write:

 ![Captura de tela 2025-03-30 160124](https://github.com/user-attachments/assets/ab46a789-9c42-459c-89c0-21c1944305b9)

 <p>&nbsp;</p>

### Observações:

openat (31.97%): A chamada openat representa uma parte significativa do tempo de execução, indicando que o programa passou uma quantidade considerável de tempo abrindo o arquivo arq.txt para leitura e escrita.

write (11.60%): A chamada write representa uma boa parte do tempo de execução, já que o programa tem foco na gravação dos dados no arquivo. No contexto do programa, isso está relacionado ao momento em que o texto digitado pelo usuário é efetivamente gravado dentro do arquivo de texto.

mprotect (11.00%): A chamada mprotect representa uma parte significativa do tempo de execução, essa função é usada para definir as permissões de leitura, escrita e execução de uma região específica da memória.


