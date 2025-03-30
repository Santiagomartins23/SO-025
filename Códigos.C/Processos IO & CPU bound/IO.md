 ## I/O Bound

 Analisando alguns resultados como tempo total, percentual de uso da CPU, tempo em modo kernel, tempo em modo usuário, trocas de contexto voluntárias e trocas de contexto involuntárias notamos que o arquivo/programa Write é um processo I/O Bound.
<p>&nbsp;</p>

#### Time:

 ![Captura de tela 2025-03-30 160223](https://github.com/user-attachments/assets/1cbf1b5a-e589-482a-a09c-ece5dd2f2983)


 
Tempo real (34.29s) >> Tempo de CPU (user + sys = 0.005s), ou seja, quase todo o tempo foi gasto em espera (provavelmente por I/O, como escrita em disco ou entrada do usuário).

<p>&nbsp;</p>

#### Usando: /usr/bin/time -v ./write, obtemos informações sobre tempo total, percentual de uso da CPU, tempo em modo kernel, tempo em modo usuário, trocas de contexto voluntárias e trocas de contexto involuntárias. 

 ![Captura de tela 2025-03-30 160415](https://github.com/user-attachments/assets/265ff0de-566f-4bfb-bd88-42fd7ee2b284)



   User time (seconds): 0.001       → Tempo em modo usuário
   
   System time (seconds): 0.004     → Tempo em modo kernel
   
   Percent of CPU this job got: 0% → Percentual de uso da CPU
   
   Elapsed (wall clock) time (h:mm:ss or m:ss): 0:34.90 → Tempo total (real)
   
   Voluntary context switches: 8    → Trocas de contexto voluntárias
   
   Involuntary context switches: 1  → Trocas de contexto involuntárias
