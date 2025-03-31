### CPU Bound
Analisando alguns resultados como tempo total, percentual de uso da CPU, tempo em modo kernel, tempo em modo usuário, trocas de contexto voluntárias e trocas de contexto involuntárias, notamos que o arquivo cpubound.c (programa criado para operações intensivas de CPU) é um processo CPU Bound. O código é CPU-bound porque executa um grande número de operações matemáticas intensivas na função `cpu_intensive_task`, onde cada iteração realiza cálculos como `3.14159265359 * i * i`. O uso da palavra-chave volatile impede que o compilador otimize e elimine os cálculos, garantindo que todas as operações sejam realmente processadas pela CPU. Além disso, o programa utiliza `fork()` para criar um processo filho, permitindo que tanto o processo pai quanto o filho executem tarefas computacionais simultaneamente, aumentando ainda mais a carga na CPU. Como não há chamadas bloqueantes de E/S, como`sleep()`, `scanf()`, ou operações de arquivo/rede, o tempo de execução é dominado exclusivamente pelo uso da CPU, sem períodos significativos de espera externa.

<p>&nbsp;</p>
Usando o comando time ./cpubound 10000. Obtemos:


![timecpu](https://github.com/user-attachments/assets/0cfb8432-cb92-47eb-87ad-1095e1ab00cc)

O tempo real (0.005) é próximo do tempo de CPU (user + sys = 0.005), indicando que quase todo o tempo foi gasto executando cálculos, sem esperar por operações de E/S.

<p>&nbsp;</p>


Usando: `/usr/bin/time -v ./cpubound 10000` , obtemos informações sobre tempo total, percentual de uso da CPU, tempo em modo kernel, tempo em modo usuário, trocas de contexto voluntárias e trocas de contexto involuntárias.





![time2cpu](https://github.com/user-attachments/assets/0160af5e-a600-4fee-809b-8d2c5c043e96)

User time (seconds): 0.0s → Tempo em modo usuário

System time (seconds): 0.005s → Tempo em modo kernel

Percent of CPU this job got: 75% → Percentual de uso da CPU

Elapsed (wall clock) time (h:mm:ss or m:ss): 0.0 → Tempo total (real)

Voluntary context switches: 5 → Trocas de contexto voluntárias

Involuntary context switches: 4 → Trocas de contexto involuntárias

