
 ## Arquivos e E/S



 Primeiramente, cabe explicar que foi criado dois programas de E/S, onde um (read.c) utiliza das funções open(), close() e read(), no qual abre um arquivo de texto , lê o que ta escrito nele e imprime na tela e o outro (write.c) utiliza open(), close() e write(), no qual abre um arquivo de texto, lê o texto que o usuário deseja escrever e imprime dentro desse arquivo. A chamada open() abre o arquivo de texto para ser lido e/ou escrito e o close() fecha o arquivo de texto
liberando os recursos associados ao arquivo aberto.

---

        
### Arquivo Read.c:

![read](https://github.com/user-attachments/assets/732edeac-cf66-443d-9775-a7f31a9c83ee)

Nesse programa, o arquivo arqr.txt é aberto pela função open() e é chamada a função read() para ler o texto escrito dentro desse arquivo, no qual é imprimido na tela, no final, o arquivo de texto é fechado pelo close().
<p>&nbsp;</p>

#### Usando o comando -strace -c ./read, vamos conseguir estatísticas relacionadas ao uso das chamadas ao sistema e o tempo gasto por elas:

 ![strace read](https://github.com/user-attachments/assets/8c07de01-0c0f-4882-939a-6cd1aa8a9552)
<p>&nbsp;</p>

### Observações:

execve (48.47%): Quase metade do tempo de execução foi dedicada à chamada de sistema execve, que é responsável por carregar e executar programas.
mmap (16.10%): O mmap representa o segundo maior consumo de tempo, mapeando arquivos ou dispositivos diretamente na memória.
read (8.52%):  A chamada read é responsável pela leitura de dados de arquivos ou dispositivos, e está consumindo uma parte considerável do tempo de execução. Isso indica que o programa está, de fato, realizando a leitura do arquivo de texto.
write (5.99%): Embora o programa tenha o foco principal na leitura do arquivo de texto, o fato de gastar tempo com write sugere que também há uma etapa de processamento que gera dados a serem gravados.

---


### Arquivo Write.c: 
 Inicialmente o arquivo chamado arqw.txt é aberto pela função open() e em seguida o programa pede para o usuário escrever o que deseja imprimir nesse arquivo.
 
 ![write1](https://github.com/user-attachments/assets/b12ed9d1-8cd5-47f0-adbf-555f0f6b16ec)
 
 <p>&nbsp;</p>

#### Usando um texto de exemplo:

  ![write2](https://github.com/user-attachments/assets/dd8979bc-31cd-4559-b1be-617804612df3)

  O texto que o usuário escreveu é lido e escrito no arquivo de texto pela função write() e o arquivo é fechado.

 <p>&nbsp;</p>

#### Usando o comando -strace -c ./write:

 ![strace write](https://github.com/user-attachments/assets/0a5016e9-124e-422f-9424-ef26580b64e4)

   
    


