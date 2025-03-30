 ## I/O Bound

 Analisando alguns resultados como tempo total, percentual de uso da CPU, tempo em modo kernel, tempo em modo usuário, trocas de contexto voluntárias e trocas de contexto involuntárias notamos que o arquivo/programa Write
é um processo I/O Bound.
<p>&nbsp;</p>

## Time:

 ![time write](https://github.com/user-attachments/assets/eece7677-5b17-44aa-961c-0a03fe76a655)
 
Tempo real (6.92s) >> Tempo de CPU (user + sys = 0.004s)
Quase todo o tempo foi gasto em espera (provavelmente por I/O, como escrita em disco ou entrada do usuário).

<p>&nbsp;</p>

-Usando esse comando: /usr/bin/time -v ./write, obtemos informações sobre tempo total, percentual de uso da CPU, tempo em modo kernel, tempo em modo usuário, trocas de contexto voluntárias e trocas de contexto involuntárias. 

 ![time write2](https://github.com/user-attachments/assets/410ca477-fe84-4518-9641-c5398b47b222)


   User time (seconds): 0.000       → Tempo em modo usuário
   
   System time (seconds): 0.005     → Tempo em modo kernel
   
   Percent of CPU this job got: 0% → Percentual de uso da CPU
   
   Elapsed (wall clock) time (h:mm:ss or m:ss): 0:11.17 → Tempo total (real)
   
   Voluntary context switches: 8    → Trocas de contexto voluntárias
   
   Involuntary context switches: 3  → Trocas de contexto involuntárias
