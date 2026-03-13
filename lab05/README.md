# Analizador de Logs Web Multihilo

Una herramienta de procesamiento de logs de alto rendimiento, paralelizada y escrita en **C**. Este proyecto demuestra el poder del multihilo (multi-threading) y de las estructuras de datos eficientes al analizar millones de registros de servidores web para extraer estadísticas clave en cuestión de segundos.

## 🚀 Características

-   **Procesamiento en Paralelo**: Utiliza la biblioteca `pthread` para dividir un archivo de log de gran tamaño en fragmentos (chunks), procesándolos simultáneamente en múltiples núcleos de CPU.
-   **Escalabilidad sin Bloqueos**: Implementa una arquitectura "Shared-Nothing" donde cada hilo mantiene sus propias estadísticas locales, eliminando la necesidad de bloqueos Mutex costosos durante la fase de procesamiento intensivo.
-   **Estructuras de Datos de Alta Velocidad**: Utiliza **Tablas Hash con Encadenamiento** (Hash Tables with Chaining) desarrolladas a medida para lograr tiempos de búsqueda e inserción de O(1) en promedio para IPs y URLs únicas.
-   **Arquitectura Map-Reduce**: Sigue el patrón clásico de Map-Reduce, donde los hilos de trabajo (Map) generan resultados locales que son combinados por el hilo principal (Reduce) al final.

## 📊 Métricas Analizadas

La herramienta extrae la siguiente información de los archivos `access.log`:
1.  **Total de IPs Únicas**: Cuenta cuántos visitantes distintos accedieron al servidor.
2.  **URL más Visitada**: Identifica el endpoint más popular y el número de visitas que recibió.
3.  **Conteo de Errores HTTP**: Resume todas las peticiones que devolvieron un código de estado 4xx o 5xx.

## 🏗️ Descripción de la Arquitectura

El programa opera en tres fases distintas:

1.  **Particionamiento**: El hilo principal calcula el tamaño del archivo y asigna los desplazamientos de bytes (offsets) a cada hilo de trabajo.
2.  **Procesamiento en Paralelo**: Los hilos se posicionan en su desplazamiento asignado y analizan las líneas del log en sus tablas hash privadas.
3.  **Fase de Mezcla (Merging)**: Una vez que todos los hilos terminan, el hilo principal recorre las tablas hash locales y las combina en una estructura global para el reporte final.

> **Nota sobre Integridad de Datos**: El analizador incluye lógica para omitir líneas parciales al inicio de un fragmento y leer más allá del final del mismo, garantizando que ninguna entrada de log se cuente dos veces o se pierda.

## 📂 Estructura del Proyecto

```text
├── main_linux.c      # Orquestación de hilos y particionamiento de archivos
├── log_processor.c   # Lógica de análisis de logs, tablas hash y mezcla
├── log_processor.h   # Estructuras de datos y prototipos de funciones
├── Makefile          # Sistema de construcción automatizado
├── run.sh            # Script de ayuda para limpiar, compilar y ejecutar
└── access.log        # Archivo de log de muestra para pruebas
```

## 🛠️ Cómo Ejecutar

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
Ejecuta el analizador proporcionando la ruta a un archivo de log:
```bash
./log_analyzer access.log
```

Alternativamente, puedes usar el script de ayuda:
```bash
./run.sh
```

## 📈 Consejo de Rendimiento
Puedes ajustar el número de hilos cambiando la macro `NUM_THREADS` en `main_linux.c` para que coincida con el número de núcleos de tu CPU y obtener el máximo rendimiento.

---
*Desarrollado como parte del Laboratorio 05 de Sistemas Operativos CC7.*
