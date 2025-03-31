#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

// Função que consome CPU intensivamente
void cpu_intensive_task(int iterations) {
    for (int i = 0; i < iterations; i++) {
        // Cálculo pesado (não-otimizável pelo compilador)
        volatile double x = 3.14159265359 * i * i;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <iterações_por_processo>\n", argv[0]);
        return 1;
    }

    int iterations = atoi(argv[1]);
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork falhou");
        return 1;
    }
    if (pid == 0) { //processo filho
        printf("Filho (PID %d) iniciando tarefa CPU-bound...\n", getpid());
        cpu_intensive_task(iterations);
        printf("Filho (PID %d) concluído.\n", getpid());
        exit(0);
    } else { //processo pai
        printf("Pai (PID %d) iniciando tarefa CPU-bound...\n", getpid());
        cpu_intensive_task(iterations);
        wait(NULL);
        printf("Pai (PID %d) concluído.\n", getpid());
    }

    return 0;
}
