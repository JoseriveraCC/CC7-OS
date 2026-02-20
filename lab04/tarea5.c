#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main() {
    // 1. Crear el segmento de memoria compartida
    int shmid = shmget(IPC_PRIVATE, 1024, 0666 | IPC_CREAT);
    
    // 2. Adjuntar la memoria al espacio del proceso
    char *str = (char*) shmat(shmid, (void*)0, 0);

    pid_t pid = fork();

    if (pid > 0) {
        // Proceso Padre: Escribe en la memoria compartida
        strcpy(str, "Shared Memory Example");
        printf("Parent Process: Writing \"%s\"\n", str);
        
        wait(NULL); // Espera a que el hijo lea
        
        // Desvincular y destruir la memoria compartida
        shmdt(str);
        shmctl(shmid, IPC_RMID, NULL);
    } 
    else if (pid == 0) {
        // Proceso Hijo: Lee de la memoria compartida
        sleep(1); // Breve pausa para asegurar que el padre escriba primero
        printf("Child Process: Read \"%s\"\n", str);
        
        // Desvincular la memoria en el hijo
        shmdt(str);
    }

    return 0;
}