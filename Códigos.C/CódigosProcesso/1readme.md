## Gerenciamento de Processos

Foram desenvolvidos 3 programas em C que utilizam primitivas de chamadas de sistema do Linux para gerenciamento de processos. As funções desenvolvidas para esta categoria foram fork, escalonador de processos e tratamento de sinais. Todos os códigos referentes a esta seção estão disponíveis nesta pasta.

---

### Arquivo fork.c

Neste arquivo, foi utilizada a função `fork()` para criar novos processos. Essa função é fundamental para multitarefa, pois permite que um processo gere um processo filho idêntico ao pai. Essa técnica é amplamente utilizada em sistemas operacionais para criar processos concorrentes, tornando possível a execução paralela de diferentes tarefas. O uso de `wait()` assegura que os processos filhos sejam finalizados corretamente, evitando a criação de processos zumbis.



##### Usando o comando " strace -c ./fork ", vamos conseguir estatísticas relacionadas ao uso das chamadas ao sistema e o tempo gasto por elas: 
<p>&nbsp;</p>







![capturaFork](https://github.com/user-attachments/assets/c20a6304-d2ed-4070-bd30-1ea86e795d57)
<p>&nbsp;</p>


## Observações:

- *wait4* (73.14%): O alto percentual de tempo gasto em wait4 indica que o processo pai está aguardando a finalização dos processos filhos, o que é esperado, pois o código utiliza `wait()` para auxiliar na sincronizaçâo.

- *clone* (25.34%): Esse tempo reflete o custo de criação de processos com `fork()`. Como fork() copia a estrutura do processo pai, ele pode ser uma operação cara, especialmente se muitos processos forem criados rapidamente.


### Arquivo scheduling.c
Neste arquivo, foram implementados algoritmos de escalonamento utilizando chamadas de sistema para controle de processos. O programa demonstra:
- **FIFO**: Execução sequencial de processos. Em c:
```
void schedule_fifo(Process processes[], int n) {
    printf("\nEscalonamento FIFO:\n");
    for (int i = 0; i < n; i++) {
        pid_t pid = fork();
        if (pid == 0) {

            run_process(processes[i]);
            exit(0); //terminates the child process
        } else if (pid > 0) {

            wait(NULL); //waits for the child process to finish
        } else {
            perror("Erro ao criar processo");
            exit(1);
        }
    }
}
```

- **Round-Robin**: Alternância entre processos com quantum fixo. Em c:

```
void schedule_rr(Process processes[], int n, int quantum) {
    printf("\nEscalonamento Round-Robin (Quantum: %d segundos):\n", quantum);
    for (int i = 0; i < n; i++) {
        pid_t pid = fork();
        if (pid == 0) {

            int remaining_time = processes[i].burst_time;
            while (remaining_time > 0) {
                int execution_time = (remaining_time > quantum) ? quantum : remaining_time;
                printf("Processo %d (Prioridade: %d) executando por %d segundos. Tempo restante: %d segundos.\n",
                       processes[i].id, processes[i].priority, execution_time, remaining_time - execution_time);
                sleep(execution_time); //simulates execution time
                remaining_time -= execution_time;
            }
            printf("Processo %d finalizado.\n", processes[i].id);
            exit(0); //terminates the child process
        } else if (pid > 0) {

            wait(NULL); //waits for the child process to finish
        } else {
            perror("Erro ao criar processo");
            exit(1);
        }
    }
}
```  
- **Prioridades**: Ordenação por níveis de prioridade. Em c:

```

void schedule_priority(Process processes[], int n) {
    printf("\nEscalonamento por Prioridades:\n");
    //sorts processes by priority (lower value = higher priority)
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (processes[i].priority > processes[j].priority) {
                Process temp = processes[i];
                processes[i] = processes[j];
                processes[j] = temp;
            }
        }
    }

    //executes processes in priority order
    for (int i = 0; i < n; i++) {
        pid_t pid = fork();
        if (pid == 0) {

            run_process(processes[i]);
            exit(0); //terminates the child process
        } else if (pid > 0) {

            wait(NULL); //waits for the child process to finish
        } else {
            perror("Erro ao criar processo");
            exit(1);
        }
    }
}

```
**Técnicas utilizadas:**
- `fork()` para criar múltiplos processos
- `waitpid()` para controle preciso de espera
- `sleep()` para simular tempo de execução

##### Usando o comando " strace -c ./scheduling ", vamos conseguir estatísticas relacionadas ao uso das chamadas ao sistema e o tempo gasto por elas: 
![capturaEscalonamento](https://github.com/user-attachments/assets/942cd56d-e4e2-401a-9aee-53e48e7ae87b)
<p>&nbsp;</p>


## Observações:

wait4 (67.25%): O escalonador está gastando um tempo significativo aguardando processos terminarem, o que pode indicar que os processos criados possuem diferentes tempos de execução e precisam de sincronização.

clone (20.63%): O uso do `fork()` para criação de múltiplos processos tem um custo proporcional ao número de processos gerados. Isso reforça a necessidade de um escalonador eficiente.

execve (5.11%): A presença de execve indica que alguns processos estão substituindo sua imagem de execução, o que pode ser útil para rodar diferentes tarefas dentro do mesmo programa.



### Arquivo treatsignal.c

Neste arquivo, foi implementado o tratamento de sinais entre processos utilizando primitivas do sistema Linux. O programa demonstra:

**Funcionalidades principais:**
- Registro de handlers personalizados com `signal()`
- Envio de sinais entre processos usando `kill()`
- Espera assíncrona por sinais com `pause()`



##### Usando o comando " strace -c ./treatsignal.c ", vamos conseguir estatísticas relacionadas ao uso das chamadas ao sistema e o tempo gasto por elas: 
![capturaTratamentoDeSinal](https://github.com/user-attachments/assets/5c50d952-e9d0-419f-b5ec-763d7892233c)
<p>&nbsp;</p>

## Observações:

kill (5.33%): O tempo gasto no envio de sinais entre processos mostra que o programa está utilizando kill() para comunicação entre processos. Esse tempo pode variar dependendo da frequência dos sinais enviados. (Eficiência na entrega dos sinais, baixo tempo de `kill()`)

clock_nanosleep (5.31%): Esse tempo está associado ao uso de `pause()` para esperar sinais. 

execve (34.73%): Esse valor anormalmente alto pode indicar que o programa está executando novos processos com `exec()`, o que pode ser ineficiente caso esteja sendo chamado repetidamente.

wait4 (37.69%): A sincronização pós-tratamento de sinais pode estar causando bloqueios no programa. 

