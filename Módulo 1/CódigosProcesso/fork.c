#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int num_processes = 5; //number of child processes to be created
    pid_t pid;

    printf("Processo pai iniciado (PID: %d)\n", getpid());

    for (int i = 0; i < num_processes; i++) {
        pid = fork(); //creates a child process

        if (pid == -1) {
            perror("Erro ao criar processo filho");
            return 1;
        }

        if (pid == 0) {
            //code executed by the child process
            printf("Processo filho %d criado (PID: %d)\n", i + 1, getpid());
            sleep(2); //simulates work in the child process
            printf("Processo filho %d terminando (PID: %d)\n", i + 1, getpid());
            return 0; //terminates the child process
        }
    }

    //code executed only by the parent process
    for (int i = 0; i < num_processes; i++) {
        wait(NULL); //waits for all child processes to finish
    }

    printf("Processo pai terminando (PID: %d)\n", getpid());
    return 0;
}
