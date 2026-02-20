#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main() {
    // Crear memoria compartida
    int shmid = shmget(IPC_PRIVATE, 1024, 0666 | IPC_CREAT);
    
    // Adjuntar la memoria 
    char *str = (char*) shmat(shmid, (void*)0, 0);

    pid_t pid = fork();

    if (pid < 0) {
        printf("Error al crear el proceso.\n");
        return 1;
    } else if (pid == 0) {
        // Hijo lee de la memoria compartida
        sleep(1); // Breve pausa para asegurar que el padre escriba primero
        printf("Child Process: Read \"%s\"\n", str);
        
        // Desvincular la memoria en el hijo
        shmdt(str);
    } else {
        // Padre escribe en la memoria compartida
        strcpy(str, "Shared Memory Example");
        printf("Parent Process: Writing \"%s\"\n", str);
        
        wait(NULL); // Esoeramos
        
        // Desvincular y destruir la memoria compartida
        shmdt(str);
        shmctl(shmid, IPC_RMID, NULL);
        
    }


    return 0;
}