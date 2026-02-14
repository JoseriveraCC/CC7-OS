# Lab 03: Manejo de Interrupciones

Este laboratorio implementa un sistema básico de manejo de interrupciones en la **BeagleBone Black**. Demuestra la las interrupciones mediante el uso de **Hardware Timers** y **Rutinas de Servicio de Interrupción (ISR)**.

## 🎯 Objetivo
Demostrar cómo el flujo normal de ejecución de un programa puede ser pausado momentáneamente por un evento de hardware, atendido, y reanudado sin afectar la lógica principal.

## ⚙️ Funcionamiento
El sistema consta de dos hilos de ejecución:
1.  **Bucle Principal (Main Loop):** Genera e imprime números aleatorios en la consola UART continuamente.
2.  **Interrupción (ISR):** Configurada mediante el **DMTIMER2**. Cada 2 segundos, el timer desborda, el CPU pausa el bucle principal, salta a la rutina de atención, imprime **"Tick"**, y regresa al punto exacto donde se quedó.

## 🛠️ Arquitectura del Software
* **`root.s` (Assembly):** Contiene la **Tabla de Vectores de Excepción**. Maneja el resguardo de registros (Context Saving) antes de pasar el control a C.
* **`os.c` (C):**
    * Configura la UART (Serial).
    * Configura el Controlador de Interrupciones (INTC).
    * Configura el Timer de Hardware (DMTIMER).
    * Contiene la lógica del `main` y el `timer_irq_handler`.
* **`linker.ld`:** Define el mapa de memoria para cargar el programa en la RAM (`0x82000000`).

## 🚀 Cómo Ejecutar (Hardware Real)
1.  **Compilar:**
    Ejecutar el script de construcción:
    ```bash
    ./build_and_run.sh
    ```
    Esto generará `bin/program.bin`.

2.  **Cargar en BeagleBone:**
    * Conectar la BeagleBone vía cable FTDI (Serial).
    * Abrir terminal CoolTerm (para este laboratorio) a **115200 baudios**.
    * Reiniciar la placa e interrumpir el arranque para entrar a **U-Boot**.
    * Ejecutar: `loady 0x82000000`.
    * Transferir `program.bin` usando protocolo **YMODEM**.
    * Ejecutar: `go 0x82000000`.

![alt text](/imagenes/image.png)

## 📋 Requisitos
* Toolchain: `arm-none-eabi-gcc`
* Hardware: BeagleBone Black
* Cable: USB a TTL Serial (FTDI)