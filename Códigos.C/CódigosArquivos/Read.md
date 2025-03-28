
Arquivos e E/S



 Primeiramente, cabe explicar que foi criado dois programas de E/S onde um (read.c) utiliza das funções open(), close() e read(), no qual lê o que ta escrito em um arquivo de texto e imprime na tela e o outro (write.c) 
utiliza open(), close() e write(), no qual o usuário consegue escrever um texto e imprimir dentro do arquivo de texto. A chamada open() abre o arquivo de texto para ser lido e/ou escrito, o close() fecha o arquivo de texto
liberando os recursos associados ao arquivo aberto.

        
-Arquivo Read.c:

![read](https://github.com/user-attachments/assets/732edeac-cf66-443d-9775-a7f31a9c83ee)
 

-Usando o comando -strace -c ./read, vamos conseguir estatísticas relacionadas ao uso das chamadas ao sistem:

 ![strace read](https://github.com/user-attachments/assets/f5e4214f-0cd0-4b2f-b228-f4f0a0c37af1)


 -Arquivo Write.c: Inicialmente pede para o usuário escrever o que deseja imprimir no arquivo de texto.
 
 ![write1](https://github.com/user-attachments/assets/b12ed9d1-8cd5-47f0-adbf-555f0f6b16ec)


  Usando um texto de exemplo:
  
 ![write2](https://github.com/user-attachments/assets/0422deb9-7f7e-48a2-9c78-3ea60a74c904)


-Usando o comando -strace -c ./write:

 ![strace write](https://github.com/user-attachments/assets/d4ea01d3-0dd5-4bad-aca0-aeb5744b0254)





 

 

   
    


