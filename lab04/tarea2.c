#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    // 1. Crear un proceso hijo usando fork()
    pid_t pid = fork();

    if (pid < 0) {
        printf("Error al crear el proceso.\n");
        return 1;
    } else if (pid == 0) {

        // Imprime su propio ID y el ID de su padre
        printf("Child Process: PID=%d, Parent PID=%d\n", getpid(), getppid());

        // Observamos el orden de la ejecucion
        sleep(1);
    } else {
        // Código del proceso Padre
        printf("Parent Process: PID=%d\n", getpid());

        //El padre espera a que el hijo termine
        waitpid(pid, NULL, 0); 
        printf("Parent Process: Child has finished execution.\n");
        
    }

    return 0;
}