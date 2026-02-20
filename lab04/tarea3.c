#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main() {
    int fd[2]; // fd[0] es para leer, fd[1] es para escribir
    pipe(fd); 
    char buffer[100];

    pid_t pid = fork();

    if (pid < 0) {
        printf("Error en proceso");
        return 1;
    } else if (pid == 0) {
        // Proceso Hijo
        close(fd[1]); // Cerramos el extremo de escritura que no usamos
        read(fd[0], buffer, sizeof(buffer));
        printf("Child Process: Received \"%s\"\n", buffer);
        close(fd[0]); // Cerramos tras leer
    } else {
        // Proceso Padre
        close(fd[0]); // Cerramos el extremo de lectura que no usamos
        char msg[] = "Hello from Parent";
        printf("Parent Process: Writing \"%s\"\n", msg);
        write(fd[1], msg, strlen(msg) + 1);
        close(fd[1]); // Cerramos tras escribir
    }

    return 0;
}