# Reporte de Laboratorio 004: Procesos

## 1. Pasos para la Creación de Procesos

La creación de un nuevo proceso se realiza mediante la clonación de un proceso existente utilizando la llamada al sistema `fork()`. Los pasos que realiza el Sistema Operativo son:

* **Invocación:** El proceso padre ejecuta `fork()`, haciendo una transición de contexto del Modo Usuario al Modo Kernel.
* **Asignación de Recursos:** El OS crea un nuevo Bloque de Control de Proceso (PCB) para el hijo y le asigna un Identificador de Proceso (PID) único.
* **Clonación de Memoria:** Se duplica el espacio de direcciones de memoria del padre para el hijo. Ambos tienen el mismo código y los mismos valores en sus variables en el momento exacto de la clonación, pero residen en espacios aislados.
* **Retorno Dual:** La función `fork()` retorna dos veces. En el proceso padre devuelve el PID del nuevo hijo, y en el proceso hijo devuelve `0`. Esto permite utilizar condicionales (`if/else`) para utilizarlos.

## 2. Funcionamiento de la Sincronización de Procesos


La sincronización es vital para mantener el orden de ejecución y en este laboratorio se utilizo `wait()`.

* **Bloqueo del Padre:** Cuando el padre ejecuta `wait()`, el Sistema Operativo cambia su estado a "Bloqueado". El padre pausa su ejecución y le cede el tiempo de CPU a otros procesos (como a su propio hijo).
* **Terminación del Hijo:** El hijo realiza su trabajo y termina. El OS libera su memoria, pero mantiene su estado de salida a la espera de su padre.
* **Recolección (Reaping):** El OS notifica al proceso padre que su hijo ha terminado. El padre "despierta", recoge el estado de salida del hijo, y el SO elimina finalmente al hijo de la tabla del sistema. El padre entonces reanuda su ejecución.

## 3. Funcionamiento de los Mecanismos IPC (Comunicación Entre Procesos)
Dado que los procesos en Linux tienen espacios de memoria estrictamente aislados por seguridad, el OS proporciona mecanismos IPC para que puedan intercambiar datos. En este laboratorio exploramos dos métodos:

### Tuberías (Pipes)

* **Descripción:** Es un canal de comunicación unidireccional gestionado por el kernel que utiliza una arquitectura FIFO (First In, First Out).
* **Funcionamiento:** Se crea con la llamada `pipe(fd)`, lo que genera dos descriptores de archivo: `fd[1]` sirve como la entrada de datos (escritura) y `fd[0]` como la salida (lectura). Los datos escritos en el tubo viajan hacia el kernel, se almacenan temporalmente en un búfer, y el proceso receptor los lee desde ese búfer.

### Memoria Compartida (Shared Memory)

* **Descripción:** Es el mecanismo IPC más rápido disponible porque elimina la necesidad de que el kernel copie los mensajes entre los procesos.
* **Funcionamiento:** 
    1. Se solicita al OS la creación de un segmento de memoria en la RAM usando `shmget()`.
    2. Se adjunta ese segmento físico al espacio de memoria virtual de los procesos involucrados utilizando `shmat()`.
    3. A partir de ese momento, los procesos pueden leer y escribir directamente en esa RAM compartida utilizando punteros de C normales.
    4. A diferencia de los pipes, la memoria compartida requiere que el programador haga una limpieza explícita (usando `shmdt` y `shmctl`) para liberar la RAM y destruir el segmento cuando ya no se utilice.