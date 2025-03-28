
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

  ![write2](https://github.com/user-attachments/assets/de884c4f-4ea5-48c2-88ba-426c46e5b8cc)

  


-Usando o comando -strace -c ./write:

 ![strace write](https://github.com/user-attachments/assets/568cfaf3-1038-4b6f-99c4-dec48158a1f8)






 

 

   
    


