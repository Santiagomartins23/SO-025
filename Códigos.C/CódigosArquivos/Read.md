
Arquivos e E/S



 Primeiramente, cabe explicar que foi criado dois programas de E/S onde um (read.c) utiliza das funções open(), close() e read(), no qual lê o que ta escrito em um arquivo de texto e imprime na tela e o outro (write.c) 
utiliza open(), close() e write(), no qual o usuário consegue escrever um texto e imprimir dentro do arquivo de texto. A chamada open() abre o arquivo de texto para ser lido e/ou escrito, o close() fecha o arquivo de texto
liberando os recursos associados ao arquivo aberto.

        
-Arquivo Read.c:

![read](https://github.com/user-attachments/assets/732edeac-cf66-443d-9775-a7f31a9c83ee)


 

-Usando o comando -strace -c ./read, vamos conseguir estatísticas relacionadas ao uso das chamadas ao sistema e o tempo gasto por elas:

 ![strace read](https://github.com/user-attachments/assets/8c07de01-0c0f-4882-939a-6cd1aa8a9552)



 -Arquivo Write.c: Inicialmente pede para o usuário escrever o que deseja imprimir no arquivo de texto.
 
 ![write1](https://github.com/user-attachments/assets/b12ed9d1-8cd5-47f0-adbf-555f0f6b16ec)



  Usando um texto de exemplo:

  ![write2](https://github.com/user-attachments/assets/dd8979bc-31cd-4559-b1be-617804612df3)


  


-Usando o comando -strace -c ./write:

 ![strace write](https://github.com/user-attachments/assets/0a5016e9-124e-422f-9424-ef26580b64e4)

 



-I/O Bound

 Analisando alguns resultados como tempo total, percentual de uso da CPU, tempo em modo kernel, tempo em modo usuário, trocas de contexto voluntárias e trocas de contexto involuntárias notamos que o arquivo/programa Write
é um processo I/O Bound.


-Time:

 ![time write](https://github.com/user-attachments/assets/eece7677-5b17-44aa-961c-0a03fe76a655)
 
Tempo real (6.92s) >> Tempo de CPU (user + sys = 0.004s)
Quase todo o tempo foi gasto em espera (provavelmente por I/O, como escrita em disco ou entrada do usuário).




-Usando esse comando: /usr/bin/time -v ./write, obtemos informações sobre tempo total, percentual de uso da CPU, tempo em modo kernel, tempo em modo usuário, trocas de contexto voluntárias e trocas de contexto involuntárias. 

 ![time write2](https://github.com/user-attachments/assets/d8aeeec9-13fd-4134-944a-8a2ade3811f5)

   User time (seconds): 0.01       → Tempo em modo usuário
   System time (seconds): 0.02     → Tempo em modo kernel
   Percent of CPU this job got: 5% → Percentual de uso da CPU
   Elapsed (wall clock) time (h:mm:ss or m:ss): 0:05.03 → Tempo total (real)
   Voluntary context switches: 1    → Trocas de contexto voluntárias
   Involuntary context switches: 2  → Trocas de contexto involuntárias

 












 

 

   
    


