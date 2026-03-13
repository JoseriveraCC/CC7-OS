#ifndef LOG_PROCESSOR_H
#define LOG_PROCESSOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_IP_LEN 16
#define MAX_URL_LEN 256
#define HASH_SIZE 1024

// Estructura para los nodos IP
typedef struct IPNode {
    char ip[MAX_IP_LEN];   
    int count;               
    struct IPNode *next;     
} IPNode;

// Estructura para los nodos URL 
typedef struct URLNode {
    char url[MAX_URL_LEN];  
    int count;               
    struct URLNode *next;    
} URLNode;

// Objeto para guardar todos los datos obtenidos
typedef struct {
    IPNode *ip_table[HASH_SIZE];   
    URLNode *url_table[HASH_SIZE]; 
    int error_count;               
    int unique_ips;                
} LogStats;

// Argumentos de los threads para procesar un segmento del archivo
typedef struct {
    char *filename;      
    long start_offset;   
    long end_offset;     
    LogStats *stats;     
} ThreadArgs;

unsigned int hash(const char *str);
void init_stats(LogStats *stats);
void process_log_line(char *line, LogStats *stats);
void merge_stats(LogStats *global_stats, LogStats *local_stats);
void print_results(LogStats *stats);
void free_stats(LogStats *stats);

#endif
