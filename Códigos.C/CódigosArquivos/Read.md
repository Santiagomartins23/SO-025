
 ## Arquivos e E/S



 Primeiramente, cabe explicar que foi criado dois programas de E/S onde um (read.c) utiliza das funções open(), close() e read(), no qual lê o que ta escrito em um arquivo de texto e imprime na tela e o outro (write.c) 
utiliza open(), close() e write(), no qual o usuário consegue escrever um texto e imprimir dentro do arquivo de texto. A chamada open() abre o arquivo de texto para ser lido e/ou escrito, o close() fecha o arquivo de texto
liberando os recursos associados ao arquivo aberto.

        
### Arquivo Read.c:

![read](https://github.com/user-attachments/assets/732edeac-cf66-443d-9775-a7f31a9c83ee)
<p>&nbsp;</p>

#### Usando o comando -strace -c ./read, vamos conseguir estatísticas relacionadas ao uso das chamadas ao sistema e o tempo gasto por elas:

 ![strace read](https://github.com/user-attachments/assets/8c07de01-0c0f-4882-939a-6cd1aa8a9552)
<p>&nbsp;</p>

#### Arquivo Write.c: Inicialmente pede para o usuário escrever o que deseja imprimir no arquivo de texto.
 
 ![write1](https://github.com/user-attachments/assets/b12ed9d1-8cd5-47f0-adbf-555f0f6b16ec)



#### Usando um texto de exemplo:

  ![write2](https://github.com/user-attachments/assets/dd8979bc-31cd-4559-b1be-617804612df3)

  O texto que o usuário escreveu foi escrito no arquivo de texto.


  


#### Usando o comando -strace -c ./write:

 ![strace write](https://github.com/user-attachments/assets/0a5016e9-124e-422f-9424-ef26580b64e4)

 




   

 












 

 

   
    


