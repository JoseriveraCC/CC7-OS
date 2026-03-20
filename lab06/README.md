# Descripción del Proyecto

Este es un **proyecto de programación en C** que demuestra conceptos de programación concurrente utilizando hilos POSIX (pthreads) y semáforos. El proyecto implementa una **simulación de estacionamiento** donde 20 automóviles compiten por 5 espacios de estacionamiento limitados.

## Arquitectura

- **Lenguaje:** C (C99 o posterior)
- **Modelo de Hilos:** Hilos POSIX (`pthread.h`)
- **Primitivas de Sincronización:**
  - `semaphore.h` - Semáforo de cuenta para limitar el acceso concurrente a 5 espacios de estacionamiento
  - `pthread_mutex_t` - Mutex para proteger secciones críticas al acceder a datos compartidos
- **Plataforma:** Linux

## Archivos Fuente

| Archivo | Descripción |
|------|-------------|
| `parkingLots.c` | Archivo fuente principal con la simulación del estacionamiento |
| `parkingLots` | Binario ejecutable compilado |

## Parámetros de Simulación

| Constante | Valor | Descripción |
|-----------|-------|-------------|
| `CARS` | 20 | Número total de automóviles que participan |
| `SPOTS` | 5 | Número de espacios de estacionamiento disponibles |
| `LOG_SIZE` | 10 | Tamaño del buffer para el registro de actividad reciente |

## Cómo Funciona

1. **Semáforo (`parking_lot`):** Inicializado con 5 permisos, representando los 5 espacios disponibles
2. **Mutex (`lock`):** Protege el array compartido `spots[]` y las variables de estadísticas de condiciones de carrera
3. **Hilos de Automóviles:** Se crean 20 hilos, cada uno representando un automóvil
4. **Lógica de Estacionamiento:**
   - El automóvil espera en el semáforo hasta que haya un espacio disponible
   - Mide el tiempo de espera usando `clock_gettime(CLOCK_MONOTONIC)`
   - Adquiere el mutex, busca un espacio vacío (`-1`), lo marca con su ID
   - "Estaciona" durante 3-7 segundos (`rand() % 5 + 3`)
   - Libera el espacio (lo marca como `-1`) y devuelve el permiso al semáforo

## Interfaz Gráfica

El programa muestra una interfaz en tiempo real que incluye:
- Estado visual de los 5 espacios (LIBRE en verde / CARRO X en rojo)
- Contador de carros en espera
- Estadísticas: total de carros atendidos y tiempo promedio de espera
- Buffer circular con las últimas 10 actividades registradas

## Construcción y Ejecución

### Compilar

```bash
gcc -o parkingLots parkingLots.c -lpthread -lm
```

### Ejecutar

```bash
./parkingLots
```

### Salida Esperada

```
   ====================== 
   SIMULACION DE PARQUEO 
   ====================== 
Parqueos (5 total):
+---------------------------+
[ CARRO 1 ] [ CARRO 2 ] [ LIBRE ] [ LIBRE ] [ LIBRE ]  
+---------------------------+

Carros en espera: [3]
Datos: Total de carros: 5 | Avg Espera: 0.45s

---- Actividad Reciente: ---- 
[Thu Mar 19 22:05:20 2026] Carro 1: PARQUEADO (0.00)
[Thu Mar 19 22:05:21 2026] Carro 2: SALIO DEL PARQUEO
...
```

## Conceptos Clave Demostrados

- **Sincronización de Hilos:** Uso de semáforos para limitar recursos concurrentes
- **Exclusión Mutua:** Mutex protege arrays y variables compartidas
- **Prevención de Condiciones de Carrera:** Secciones críticas correctamente protegidas
- **Medición de Tiempo:** Uso de `clock_gettime()` para calcular tiempos de espera
- **Buffer Circular:** Implementación de log de actividad con índice circular
- **Interfaz Consola Dinámica:** Uso de códigos ANSI para actualizar la pantalla en tiempo real

## Convenciones de Desarrollo

- Utiliza APIs POSIX estándar para portabilidad entre sistemas Unix-like
- Diseño thread-safe con sincronización adecuada
- Limpieza correcta de recursos (`sem_destroy`, `pthread_mutex_destroy`)
- Interfaz visual con colores ANSI y emojis para claridad
- Mensajes en español para toda la interfaz de usuario
