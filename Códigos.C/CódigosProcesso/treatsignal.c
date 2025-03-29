
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

//signal handler function
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

int main() {
    pid_t pid;

    //create the child process
    pid = fork();

    if (pid == -1) {
        perror("Erro ao criar processo filho");
        return 1;
    }

    if (pid == 0) {
        //code executed by the child process
        printf("Filho esperando por sinal...\n");

        //set up signal handlers
        signal(SIGUSR1, handle_signal); // Handle SIGUSR1
        signal(SIGUSR2, handle_signal); // Handle SIGUSR2
        signal(SIGTERM, handle_signal); // Handle SIGTERM

        //infinite loop to keep the child process alive
        while (1) {
            sleep(1);
        }
    } else {
        //code executed by the parent process
        printf("Pai enviando SIGUSR1 para filho (PID: %d)\n", pid);

        //wait a bit before sending the first signal
        sleep(2);

        //send SIGUSR1 to the child process
        kill(pid, SIGUSR1);

        //wait a bit before sending the second signal
        sleep(2);

        //send SIGUSR2 to the child process
        kill(pid, SIGUSR2);

        //wait a bit before sending the termination signal
        sleep(2);

        //send SIGTERM to terminate the child process
        kill(pid, SIGTERM);

        //wait for the child process to terminate
        wait(NULL);
        printf("Pai finalizado.\n");
    }

    return 0;
}
