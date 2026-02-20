#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    printf("Parent Process: PID=%d\n", getpid());

    for (int i = 1; i <= 3; i++) {
        if (fork() == 0) {
            // Código exclusivo para el hijo
            printf("Child %d: PID=%d, Parent PID=%d\n", i, getpid(), getppid());
            return 0; // El hijo debe salir del programa para no crear sus propios hijos
        }
    }

    // El padre espera a que sus 3 hijos terminen
    for (int i = 0; i < 3; i++) {
        wait(NULL);
    }

    return 0;
}