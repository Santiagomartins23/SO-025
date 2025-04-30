
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

//structure for a process
typedef struct {
    int id;
    int burst_time;
    int priority;
} Process;

//function to simulate the execution of a process
void run_process(Process p) {
    printf("Processo %d (Prioridade: %d) iniciado. Tempo de execução: %d segundos.\n", p.id, p.priority, p.burst_time);
    sleep(p.burst_time); //simulates execution time
    printf("Processo %d finalizado.\n", p.id);
}

//FIFO Scheduler
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

//Round-Robin Scheduler
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

// Priority Scheduler
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

int main() {
    //list of fictitious processes
    Process processes[] = {
        {1, 5, 2}, //process 1: burst_time = 5s, priority = 2
        {2, 3, 1}, //process 2: burst_time = 3s, priority = 1
        {3, 8, 3}, //process 3: burst_time = 8s, priority = 3
        {4, 4, 2}  //process 4: burst_time = 4s, priority = 2
    };
    int n = sizeof(processes) / sizeof(processes[0]);

    //FIFO Scheduling
    schedule_fifo(processes, n);

    //Round-Robin Scheduling (quantum = 2 seconds)
    schedule_rr(processes, n, 2);

    //Priority Scheduling
    schedule_priority(processes, n);

    return 0;
}
