#include <Arduino.h>

// Declaramos los "manejadores" (handles) para nuestros semáforos
SemaphoreHandle_t semaforoEncender;
SemaphoreHandle_t semaforoApagar;

// Prototipos de las tareas
void tareaEncender(void * parameter);
void tareaApagar(void * parameter);

void setup() {
  Serial.begin(115200);
  
  // Configuramos el LED integrado de la placa como salida
  pinMode(LED_BUILTIN, OUTPUT);

  // 1. Creamos los dos semáforos binarios
  semaforoEncender = xSemaphoreCreateBinary();
  semaforoApagar = xSemaphoreCreateBinary();

  // 2. Le damos el "testigo" inicial al semáforo de encender
  xSemaphoreGive(semaforoEncender);

  // 3. Creamos la tarea que enciende el LED
  xTaskCreate(
    tareaEncender,    /* Función de la tarea */
    "Tarea Encender", /* Nombre de la tarea */
    2048,             /* Tamaño de la pila */
    NULL,             /* Parámetros */
    1,                /* Prioridad */
    NULL              /* Handle */
  );

  // 4. Creamos la tarea que apaga el LED
  xTaskCreate(
    tareaApagar,      /* Función de la tarea */
    "Tarea Apagar",   /* Nombre de la tarea */
    2048,             /* Tamaño de la pila */
    NULL,             /* Parámetros */
    1,                /* Prioridad */
    NULL              /* Handle */
  );
}

void loop() {
  // Mantenemos el loop ligero
  vTaskDelay(1000 / portTICK_PERIOD_MS); 
}

/* --- TAREA 1: ENCARGADA DE ENCENDER --- */
void tareaEncender(void * parameter) {
  for(;;) {
    // Esperamos a que el semáforo esté disponible
    xSemaphoreTake(semaforoEncender, portMAX_DELAY);
    
    Serial.println("Tarea 1: Encendiendo el LED...");
    digitalWrite(LED_BUILTIN, HIGH);
    
    // Pausa de 1 segundo
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    
    // Pasamos el control a la tarea de apagar
    xSemaphoreGive(semaforoApagar);
  }
}

/* --- TAREA 2: ENCARGADA DE APAGAR --- */
void tareaApagar(void * parameter) {
  for(;;) {
    // Esperamos a que el semáforo esté disponible
    xSemaphoreTake(semaforoApagar, portMAX_DELAY);
    
    Serial.println("Tarea 2: Apagando el LED...");
    digitalWrite(LED_BUILTIN, LOW);
    
    // Pausa de 1 segundo
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    
    // Devolvemos el control a la tarea de encender
    xSemaphoreGive(semaforoEncender);
  }
}