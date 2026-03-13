#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "log_processor.h"

// Numero the threads
#define NUM_THREADS 4

// Funcion que ejecuta cada thread
void *thread_func(void *arg) {
    ThreadArgs *t_args = (ThreadArgs *)arg;
    FILE *fp = fopen(t_args->filename, "r");
    if (!fp) {
        perror("Failed to open file");
        return NULL;
    }

    fseek(fp, t_args->start_offset, SEEK_SET);

    // Si el thread no empieza a leer desde el inicio del archivo, lee y descarta la primera linea
    if (t_args->start_offset != 0) {
        char buffer[1024];
        if (fgets(buffer, sizeof(buffer), fp) == NULL) {
            fclose(fp);
            return NULL;
        }
    }

    // Lee cada linea dentro del segmento asignado
    char line[1024];
    while (ftell(fp) < t_args->end_offset && fgets(line, sizeof(line), fp)) {
        process_log_line(line, t_args->stats);
    }

    fclose(fp);
    return NULL;
}

int main() {
    char *filename = "access.log";

    // Tamano del archivo en bytes para dividirlo entre threads
    struct stat st;
    if (stat(filename, &st) != 0) {
        perror("Failed to get file status");
        return 1;
    }

    long file_size = st.st_size;
    long chunk_size = file_size / NUM_THREADS;

    pthread_t threads[NUM_THREADS];
    ThreadArgs t_args[NUM_THREADS];
    LogStats thread_stats[NUM_THREADS];

    // Creamos los threads con sus respectivos trabajos
    for (int i = 0; i < NUM_THREADS; i++) {
        init_stats(&thread_stats[i]);

        t_args[i].filename = filename;
        t_args[i].start_offset = i * chunk_size;
        t_args[i].end_offset = (i == NUM_THREADS - 1) ? file_size : (i + 1) * chunk_size;
        t_args[i].stats = &thread_stats[i];

        // Crea el thread y asigna su trabajo
        if (pthread_create(&threads[i], NULL, thread_func, &t_args[i]) != 0) {
            perror("Failed to create thread");
            return 1;
        }
    }

    LogStats global_stats;
    init_stats(&global_stats);

    // Thread termina y hace merge de sus resultados al global stats cuando termina su parte.
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
        merge_stats(&global_stats, &thread_stats[i]);
        free_stats(&thread_stats[i]);
    }
    print_results(&global_stats);
    free_stats(&global_stats);
    return 0;
}
