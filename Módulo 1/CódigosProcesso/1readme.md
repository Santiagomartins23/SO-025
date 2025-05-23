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

Neste arquivo, foram implementados algoritmos de escalonamento utilizando chamadas de sistema para controle de processos. O programa demonstra diferentes técnicas de escalonamento, permitindo a gestão de múltiplos processos em um sistema operacional. O escalonamento de processos é um conceito fundamental em sistemas operacionais, pois define a ordem em que os processos são executados pela CPU, buscando otimizar o uso de recursos e melhorar o desempenho geral do sistema. O programa utiliza a implementação dos seguintes algoritimos :

- **FIFO**: Execução sequencial de processos. Em c :
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

- **Round-Robin**: Alternância entre processos com quantum fixo. Em c :

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
- **Prioridades**: Ordenação por níveis de prioridade. Em c :

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

- *wait4* (67.25%): O escalonador está gastando um tempo significativo aguardando processos terminarem, o que pode indicar que os processos criados possuem diferentes tempos de execução e precisam de sincronização. No código, a chamada `wait()` aparece após cada `fork()`, garantindo que o processo pai aguarde a conclusão do filho antes de continuar. Isso é especialmente perceptível no FIFO, onde cada processo é iniciado e concluído sequencialmente.

- *clone* (20.63%): O uso do `fork()` para criação de múltiplos processos tem um custo proporcional ao número de processos gerados. No código, cada algoritmo cria um novo processo para cada tarefa, aumentando o número de chamadas `clone()`. E por fim, no Round-Robin, cada processo pode executar por um tempo menor (definido pelo quantum), o que pode levar à criação de múltiplos processos para simular revezamento, tornando "clone" mais frequente.

- *execve* (5.11%): A presença de execve indica que alguns processos estão substituindo sua imagem de execução, o que pode ser útil para rodar diferentes tarefas dentro do mesmo programa.


### Arquivo treatsignal.c

Neste arquivo, foi implementado o tratamento de sinais entre processos utilizando systems calls primitivas do sistema Linux. O tratamento de sinais é um aspecto crucial na programação de sistemas, pois permite que processos possam comunicar-se entre si, controlar sua execução e responder a eventos específicos durante o tempo de execução. O programa demonstra como capturar e manipular sinais usando as funções `handle_signal(), kill() e sleep()`, além de fornecer uma abordagem simples de comunicação interprocessos através de sinais.

**Funcionalidades principais:**
- Registro de handlers personalizados com `signal()` :
```
void handle_signal(int signal) {
    switch (signal) {
        case SIGUSR1:
            printf("Filho recebeu SIGUSR1!\n");
            break;
        case SIGUSR2:
            printf("Filho recebeu SIGUSR2!\n");
            break;
        case SIGTERM:
            printf("Filho recebeu SIGTERM. Saindo...\n");
            exit(0); //terminate the child process
            break;
        default:
            printf("Filho recebeu um sinal desconhecido: %d\n", signal);
            break;
    }
}
```
- Tratamento de sinais entre processos.
- Espera síncrona por sinais com `sleep()`



##### Usando o comando " strace -c ./treatsignal.c ", vamos conseguir estatísticas relacionadas ao uso das chamadas ao sistema e o tempo gasto por elas: 
![capturaTratamentoDeSinal](https://github.com/user-attachments/assets/5c50d952-e9d0-419f-b5ec-763d7892233c)
<p>&nbsp;</p>

## Observações:

- *wait4* (37.69%): A sincronização pós-tratamento de sinais pode estar causando bloqueios no programa.

- *kill* (5.33%): O tempo gasto no envio de sinais entre processos mostra que o programa está utilizando `kill()` para comunicação entre processos, além disso representa eficiência na entrega destes sinais devido ao baixo tempo de `kill()`

- *clock_nanosleep* (5.31%): Esse tempo está associado ao uso de `sleep()` para esperar sinais. 



