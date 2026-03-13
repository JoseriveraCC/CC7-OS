#include "log_processor.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// djb2 Hash
unsigned int hash(const char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash % HASH_SIZE;
}

// Inicializar las tablas hash y memoria en 0
void init_stats(LogStats *stats) {
    memset(stats->ip_table, 0, sizeof(stats->ip_table));
    memset(stats->url_table, 0, sizeof(stats->url_table));
    stats->error_count = 0;
    stats->unique_ips = 0;
}

// Funcion para extraer el IP, URL y Status 
void process_log_line(char *line, LogStats *stats) {
    char ip[MAX_IP_LEN];
    char url[MAX_URL_LEN];
    int status;
    
    // Extraer el IP
    if (sscanf(line, "%15s", ip) != 1) return;
    
    // Parte entre comillas
    char *quote_start = strchr(line, '\"');
    if (!quote_start) return;
    char *quote_end = strchr(quote_start + 1, '\"');
    if (!quote_end) return;
    
    // Copia todo lo que esta entre comillas 
    char request[512];
    int len = quote_end - quote_start - 1;
    if (len >= 512) len = 511;
    strncpy(request, quote_start + 1, len);
    request[len] = '\0';
    
    // Extrae el URL que esta despues del primer espacio
    char *url_part = strchr(request, ' ');
    if (url_part) {
        strncpy(url, url_part + 1, MAX_URL_LEN - 1);
        url[MAX_URL_LEN - 1] = '\0';
    }
    
    // Extrae el status que esta despues de la segunda comilla
    status = atoi(quote_end + 1);
    
    // Busca el IP en la tabla hash de IPs y actualiza su conteo
    unsigned int h_ip = hash(ip);
    IPNode *curr_ip = stats->ip_table[h_ip]; 
    while (curr_ip) {
        if (strcmp(curr_ip->ip, ip) == 0) {
            curr_ip->count++; 
            break;
        }
        curr_ip = curr_ip->next;
    }
    if (!curr_ip) {
        // Si no encuentra IP crea un nuevo nodo 
        curr_ip = malloc(sizeof(IPNode));
        strncpy(curr_ip->ip, ip, MAX_IP_LEN);
        curr_ip->count = 1;
        curr_ip->next = stats->ip_table[h_ip];
        stats->ip_table[h_ip] = curr_ip;
        stats->unique_ips++;
    }
    
    // Busca el URL en la tabla hash de URLs y actualiza su conteo
    unsigned int h_url = hash(url);
    URLNode *curr_url = stats->url_table[h_url];
    while (curr_url) {
        if (strcmp(curr_url->url, url) == 0) {
            curr_url->count++;
            break;
        }
        curr_url = curr_url->next;
    }
    if (!curr_url) {
        // Si no encuentra URL crea un nuevo nodo
        curr_url = malloc(sizeof(URLNode));
        strncpy(curr_url->url, url, MAX_URL_LEN);
        curr_url->count = 1;
        curr_url->next = stats->url_table[h_url];
        stats->url_table[h_url] = curr_url;
    }
    
    // Incrementa el error count si el status esta entre 400 y 599
    if (status >= 400 && status <= 599) {
        stats->error_count++;
    }
}

// Combina los datos obtenidos de los threads nuestra vairable global_stats
void merge_stats(LogStats *global_stats, LogStats *local_stats) {
    // IPs
    for (int i = 0; i < HASH_SIZE; i++) { 
        IPNode *local_curr = local_stats->ip_table[i]; 
        while (local_curr) { 
            unsigned int h = hash(local_curr->ip); 
            IPNode *global_curr = global_stats->ip_table[h]; 
            
            // Busca el IP en la tabla global
            while (global_curr) { 
                if (strcmp(global_curr->ip, local_curr->ip) == 0) { 
                    global_curr->count += local_curr->count; 
                    break; 
                }
                global_curr = global_curr->next; 
            }
            
            // Significa el IP no se encuentra en la tabla global y la agregamos
            if (!global_curr) { 
                IPNode *new_node = malloc(sizeof(IPNode)); 
                strncpy(new_node->ip, local_curr->ip, MAX_IP_LEN); 
                new_node->count = local_curr->count; 
                new_node->next = global_stats->ip_table[h]; 
                global_stats->ip_table[h] = new_node; 
                global_stats->unique_ips++; 
            }
            local_curr = local_curr->next; 
        }
    }
    
    // URLs
    for (int i = 0; i < HASH_SIZE; i++) {
        URLNode *local_curr = local_stats->url_table[i]; 
        while (local_curr) { 
            unsigned int h = hash(local_curr->url); 
            URLNode *global_curr = global_stats->url_table[h]; 
            

            while (global_curr) {
                if (strcmp(global_curr->url, local_curr->url) == 0) { 
                    global_curr->count += local_curr->count; 
                    break; 
                }
                global_curr = global_curr->next;
            }
            

            if (!global_curr) {
                URLNode *new_node = malloc(sizeof(URLNode));
                strncpy(new_node->url, local_curr->url, MAX_URL_LEN);
                new_node->count = local_curr->count; 
                new_node->next = global_stats->url_table[h]; 
                global_stats->url_table[h] = new_node; 
            }
            local_curr = local_curr->next; 
        }
    }
    
    global_stats->error_count += local_stats->error_count; 
}


// Imprime todo los datos obtenidos
void print_results(LogStats *stats) {
    char max_url[MAX_URL_LEN] = "None";
    int max_count = 0;
    
    // Encuentra el URL mas visitado
    for (int i = 0; i < HASH_SIZE; i++) {
        URLNode *curr = stats->url_table[i];
        while (curr) {
            if (curr->count > max_count) {
                max_count = curr->count;
                strncpy(max_url, curr->url, MAX_URL_LEN);
            }
            curr = curr->next;
        }
    }
    
    printf("Total Unique IPs: %d\n", stats->unique_ips);
    printf("Most Visited URL: %s (%d times)\n", max_url, max_count);
    printf("HTTP Errors: %d\n", stats->error_count);
}

// Libera toda la memoria utilizada por las tablas hash
void free_stats(LogStats *stats) {
    for (int i = 0; i < HASH_SIZE; i++) {
        IPNode *ip_curr = stats->ip_table[i];
        while (ip_curr) {
            IPNode *tmp = ip_curr;
            ip_curr = ip_curr->next;
            free(tmp);
        }
        URLNode *url_curr = stats->url_table[i];
        while (url_curr) {
            URLNode *tmp = url_curr;
            url_curr = url_curr->next;
            free(tmp);
        }
    }
}
