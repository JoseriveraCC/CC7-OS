#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    printf("Parent Process: PID=%d\n", getpid());

    for (int i = 1; i <= 3; i++) {
        if (fork() == 0) {
            printf("Child %d: PID=%d, Parent PID=%d\n", i, getpid(), getppid());
            return 0; // Salimos para no crear mas hijos
        }
    }

    // El padre espera a que sus 3 hijos terminen
    for (int i = 0; i < 3; i++) {
        wait(NULL);
    }

    return 0;
}