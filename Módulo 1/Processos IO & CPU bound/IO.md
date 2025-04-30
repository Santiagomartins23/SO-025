 ## I/O Bound

 Analisando alguns resultados como tempo total, percentual de uso da CPU, tempo em modo kernel, tempo em modo usuário, trocas de contexto voluntárias e trocas de contexto involuntárias notamos que o arquivo/programa write.c (programa criado para processos de E/S) é um processo I/O Bound.
<p>&nbsp;</p>

 #### Texto utilizado:

 ![texto](https://github.com/user-attachments/assets/a1d80702-1ec9-4200-b815-56c7618e25aa)


#### Time:

 ![time real](https://github.com/user-attachments/assets/c5ba7042-2488-4766-9b4e-f1a4ccbdf76c)




 
Tempo real (52.00s) >> Tempo de CPU (user + sys = 0.004s), ou seja, quase todo o tempo foi gasto em espera (provavelmente por I/O, como escrita em disco ou entrada do usuário).

<p>&nbsp;</p>

#### Usando: /usr/bin/time -v ./write, obtemos informações sobre tempo total, percentual de uso da CPU, tempo em modo kernel, tempo em modo usuário, trocas de contexto voluntárias e trocas de contexto involuntárias. 

 ![write final](https://github.com/user-attachments/assets/a504c660-f710-410d-9b3b-fdd92330e8e2)




   User time (seconds): 0.00       → Tempo em modo usuário
   
   System time (seconds): 0.004     → Tempo em modo kernel
   
   Percent of CPU this job got: 0% → Percentual de uso da CPU
   
   Elapsed (wall clock) time (h:mm:ss or m:ss): 0:53.58 → Tempo total (real)
   
   Voluntary context switches: 9    → Trocas de contexto voluntárias
   
   Involuntary context switches: 1  → Trocas de contexto involuntárias
