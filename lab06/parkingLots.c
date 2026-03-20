#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define CARS 20
#define SPOTS 5
#define LOG_SIZE 10

sem_t parking_lot; 
pthread_mutex_t lock; // Initialize the mutex statically
int spots[SPOTS]; 
int waiting_count = 0;
int log_index = 0;
char log_buffer[LOG_SIZE][100];

int total_cars = 0;
double total_wait_time = 0.0;

void get_timestamp(char *buffer) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(buffer, 26, "%a %b %d %H:%M:%S %Y", t);
}

void agregar_log(const char *format, int car_id, double val) {
    char timestamp[26];
    get_timestamp(timestamp);

    char mensaje[100];
    if (val >= 0) {
        snprintf(mensaje, 100, "[%s] Carro %d: %s (%.2f)", timestamp, car_id, format, val);
    } else {
        snprintf(mensaje, 100, "[%s] Carro %d: %s", timestamp, car_id, format);
    }

    strncpy(log_buffer[log_index], mensaje, 100);
    log_index = (log_index + 1) % LOG_SIZE;
}

void interface_grafica() {
    printf("\033[H\033[J");
    printf("   ===================== \n");
    printf("   SIMULACION DE PARQUEO \n");
    printf("   ===================== \n");

    printf("Parqueos (%d total):\n", SPOTS);
    printf("+---------------------------+\n");
    for (int i = 0; i < SPOTS; i++)
    {
        if(spots[i] == -1) {
            printf("[\033[32m LIBRE \033[0m] ");
        } else {
            printf("[\033[31m CARRO %d \033[0m] ", spots[i]);
        }
    }
    printf(" \n+---------------------------+\n\n");

    printf("Carros en espera: [%d]\n", waiting_count);

    printf("Datos: Total de carros: %d | Avg Espera: %.2fs\n", total_cars, (total_cars > 0) ? total_wait_time / total_cars : 0.0);
    printf("\n---- Actividad Reciente: ---- \n");

    int start = log_index;
    for (int i = 0; i < LOG_SIZE; i++) {
        int idx = (start + i) % LOG_SIZE;
        if (strlen(log_buffer[idx]) > 0) {
            printf("%s\n", log_buffer[idx]);
        }
    }
    fflush(stdout);
}

void* car_thread(void* arg) {
    int id = *(int*)arg;
    int my_spot = -1;

    pthread_mutex_lock(&lock);
    waiting_count++;
    agregar_log("Carro nuevo en espera...", id, -1.0);
    interface_grafica();
    pthread_mutex_unlock(&lock);

    struct timespec start_wait, end_wait;
    clock_gettime(CLOCK_MONOTONIC, &start_wait);

    sem_wait(&parking_lot); 

    clock_gettime(CLOCK_MONOTONIC, &end_wait);
    double wait_time = (end_wait.tv_sec - start_wait.tv_sec) + (end_wait.tv_nsec - start_wait.tv_nsec) / 1e9;

    pthread_mutex_lock(&lock);
    waiting_count--;
    for (int i = 0; i < SPOTS; i++) {
        if (spots[i] == -1) {
            spots[i] = id;      
            my_spot = i;   
            break;
        }
    }
    total_cars++;
    total_wait_time += wait_time;
    agregar_log("PARQUEADO", id, wait_time);
    interface_grafica();
    pthread_mutex_unlock(&lock);

    sleep(rand() % 5 + 3); 

    pthread_mutex_lock(&lock);
    if(my_spot != -1) {
        spots[my_spot] = -1;
    }
    agregar_log("SALIO DEL PARQUEO", id, -1.0);
    interface_grafica();
    pthread_mutex_unlock(&lock);

    sem_post(&parking_lot); 
    return NULL;
}

int main() {
    srand(time(NULL));
    pthread_t cars[CARS];
    int ids[CARS];

    pthread_mutex_init(&lock, NULL);
    
    sem_init(&parking_lot, 0, SPOTS); 

    for(int i=0; i<SPOTS; i++) {
        spots[i] = -1;
    }
    for(int i=0; i<LOG_SIZE; i++) {
        strcpy(log_buffer[i], "");
    }

    for (int i = 0; i < CARS; i++) {
        ids[i] = i + 1;
        pthread_create(&cars[i], NULL, car_thread, &ids[i]);
        usleep((rand() % 1000000)); 
    }

    for (int i = 0; i < CARS; i++) {
        pthread_join(cars[i], NULL);
    }

    sem_destroy(&parking_lot);
    pthread_mutex_destroy(&lock);
    return 0;
}
