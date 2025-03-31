 ## I/O Bound

 Analisando alguns resultados como tempo total, percentual de uso da CPU, tempo em modo kernel, tempo em modo usuário, trocas de contexto voluntárias e trocas de contexto involuntárias notamos que o arquivo/programa write.c (programa criado para processos de E/S) é um processo I/O Bound.
<p>&nbsp;</p>

#### Time:

 ![real time](https://github.com/user-attachments/assets/4a6b4a1c-3eef-4d2a-be6c-78d7f9a5dc77)



 
Tempo real (34.29s) >> Tempo de CPU (user + sys = 0.005s), ou seja, quase todo o tempo foi gasto em espera (provavelmente por I/O, como escrita em disco ou entrada do usuário).

<p>&nbsp;</p>

#### Usando: /usr/bin/time -v ./write, obtemos informações sobre tempo total, percentual de uso da CPU, tempo em modo kernel, tempo em modo usuário, trocas de contexto voluntárias e trocas de contexto involuntárias. 

 ![write final](https://github.com/user-attachments/assets/a504c660-f710-410d-9b3b-fdd92330e8e2)




   User time (seconds): 0.00       → Tempo em modo usuário
   
   System time (seconds): 0.004     → Tempo em modo kernel
   
   Percent of CPU this job got: 0% → Percentual de uso da CPU
   
   Elapsed (wall clock) time (h:mm:ss or m:ss): 0:53.00 → Tempo total (real)
   
   Voluntary context switches: 9    → Trocas de contexto voluntárias
   
   Involuntary context switches: 1  → Trocas de contexto involuntárias
