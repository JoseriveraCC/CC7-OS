# Analizador de Logs utilizando Multi-Threading

Una herramienta de procesamiento de logs, paralelizada y escrita en **C**. Este proyecto demuestra el poder del multi-threading y de las estructuras de datos eficientes al analizar cantidades grandes de registros.

## Características

-   **Procesamiento en Paralelo**: Utiliza la biblioteca `pthread` para dividir un archivo de log de gran tamaño en fragmentos (chunks), procesándolos simultáneamente en múltiples núcleos de CPU.
-   **Escalabilidad sin Bloqueos**: Implementa una arquitectura donde cada hilo mantiene sus propias estadísticas locales, el cual elimina la necesidad de utilizar mutex.
-   **Estructuras de Datos de Alta Velocidad**: Utiliza **Tablas Hash con Encadenamiento**  desarrolladas a medida para lograr tiempos de búsqueda e inserción de O(1) en promedio para IPs y URLs únicas.

## Métricas Analizadas

La herramienta extrae la siguiente información del archivo `access.log`:
1.  **Total de IPs Únicas**: Cuenta cuántos visitantes distintos accedieron al servidor.
2.  **URL más Visitada**: Identifica el endpoint más popular y el número de visitas que recibió.
3.  **Conteo de Errores HTTP**: Resume todas las peticiones que devolvieron un código de estado 4xx o 5xx.

## Descripción de la Arquitectura

El programa opera en tres fases distintas:

1. El hilo principal calcula el tamaño del archivo y asigna los desplazamientos de bytes (offsets) a cada hilo de trabajo.
2. Los hilos se posicionan en su desplazamiento asignado y analizan las líneas del log en sus tablas hash privadas.
3. Una vez que todos los hilos terminan, el hilo principal recorre las tablas hash locales y las combina en una estructura global para el reporte final.

## Estructura del Proyecto

```text
├── main_linux.c      # Orquestación de hilos y particionamiento de archivos
├── log_processor.c   # Lógica de análisis de logs, tablas hash y mezcla
├── log_processor.h   # Estructuras de datos y prototipos de funciones
├── Makefile          # Sistema de construcción automatizado
├── run.sh            # Script de ayuda para limpiar, compilar y ejecutar
└── access.log        # Archivo de log de muestra para pruebas
```

## Cómo Ejecutar

### Prerrequisitos
-   Un entorno Linux (o WSL en Windows).
-   Compilador `gcc`.
-   Utilidad `make`.

### Compilación
Construye el proyecto utilizando el Makefile proporcionado:
```bash
make
```

### Ejecución
Ejecuta el script:
```bash
./run.sh
```

## Consejo de Rendimiento
Puedes ajustar el número de hilos cambiando la macro `NUM_THREADS` en `main_linux.c` para que coincida con el número de núcleos de tu CPU y obtener el máximo rendimiento.
