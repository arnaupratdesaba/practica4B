# 🚦 Sincronización de Tareas con Semáforos en FreeRTOS

Este proyecto demuestra cómo coordinar y sincronizar múltiples tareas en un ESP32 utilizando **semáforos binarios** de FreeRTOS. Es un excelente ejemplo para entender cómo hacer que dos hilos de ejecución paralelos trabajen en equipo de forma ordenada, pasándose el "testigo" entre ellos para evitar conflictos.

## 📌 Descripción General

El programa crea dos tareas separadas: una encargada exclusivamente de encender el LED integrado de la placa, y otra encargada de apagarlo. Para asegurar que actúen de forma secuencial y no se ejecuten desordenadamente, utilizan dos semáforos binarios. Cada tarea espera su turno y, tras completar su acción, le da permiso a la otra tarea para que proceda.

## 🧠 Estructura del Código

El archivo principal (`main.cpp`) implementa la siguiente lógica paso a paso:

* **Declaraciones Iniciales:** Se definen los manejadores (*handles*) globales `semaforoEncender` y `semaforoApagar` para que ambas tareas puedan acceder a ellos.
* **`setup()`:** * Inicializa el Monitor Serial y configura el pin del LED.
    * Crea los dos semáforos mediante `xSemaphoreCreateBinary()`.
    * **Paso clave:** Le entrega el "testigo" inicial al semáforo de encender usando `xSemaphoreGive()`. Si no se hiciera esto, ambas tareas arrancarían bloqueadas esperando un permiso que nunca llegaría.
    * Lanza las dos tareas concurrentes (`tareaEncender` y `tareaApagar`).
* **`tareaEncender()`:** Utiliza `xSemaphoreTake()` para quedarse bloqueada hasta conseguir el permiso. Cuando lo obtiene, imprime un mensaje, enciende el LED, espera un segundo y utiliza `xSemaphoreGive()` sobre el semáforo de apagar para despertar a la otra tarea.
* **`tareaApagar()`:** Funciona como un espejo. Se bloquea hasta obtener el permiso de apagado, apaga el LED, espera un segundo y vuelve a liberar el semáforo de encendido, reiniciando el ciclo infinitamente.

## ⚙️ Configuración y Uso

Sigue estos pasos para observar la sincronización en acción:

1. **Carga el código**: Compila y sube el proyecto a tu placa ESP32 a través de PlatformIO.
2. **Abre el Monitor Serial**: Asegúrate de configurar la velocidad a `115200` baudios.
3. **Observa el resultado**: Verás cómo el LED de tu placa parpadea cada segundo. En el Monitor Serial, notarás que las tareas se turnan perfectamente, demostrando la sincronización:
   ```text
   Tarea 1: Encendiendo el LED...
   Tarea 2: Apagando el LED...
   Tarea 1: Encendiendo el LED...
   Tarea 2: Apagando el LED...
