# Practica4_Primer_Parcial
# Control de Servo con FreeRTOS y ESP-IDF

## Alumno

**Eduardo Cerón**

---

## Descripción del proyecto

Esta práctica implementa una aplicación multitarea utilizando FreeRTOS y ESP-IDF sobre una tarjeta ESP32. El sistema utiliza un sensor analógico para determinar la posición objetivo de un servomotor. La lectura del sensor es procesada mediante un filtro de mediana y posteriormente enviada al TaskManager mediante colas de FreeRTOS.

El TaskManager es el único encargado de tomar decisiones sobre el estado general del sistema, habilitando o suspendiendo tareas mediante TaskHandle_t. La aplicación incluye un botón de inicio, un botón de control de velocidad, una tarea para el sensor, una tarea para el servomotor y una tarea de indicación mediante LED.

---

## Tabla de pines

| Elemento         | GPIO   |
| ---------------- | ------ |
| Sensor analógico | GPIO34 |
| Servo            | GPIO18 |
| Botón Start      | GPIO21 |
| Botón Speed      | GPIO19 |
| LED Ready        | GPIO2  |

---

## Funcionamiento

1. El sistema inicia en estado READY.
2. El LED de listo parpadea indicando disponibilidad.
3. Al presionar el botón Start se inicia la operación.
4. Se habilitan las tareas Sensor, Servo y Speed.
5. El sensor es leído y filtrado mediante un filtro de mediana.
6. Dependiendo de la lectura filtrada se genera una posición objetivo para el servomotor.
7. El servomotor se desplaza gradualmente hasta alcanzar la posición indicada.
8. Al llegar al objetivo el sistema permanece en espera durante 8 segundos.
9. Finalizado el tiempo de espera, el sistema suspende las tareas activas y vuelve al estado READY.

---

## Conceptos utilizados

* FreeRTOS
* TaskHandle_t
* xTaskCreate()
* vTaskSuspend()
* vTaskResume()
* Colas de FreeRTOS
* Filtro de mediana
* Comunicación entre tareas
* ESP-IDF
* Servomotores PWM

---

## Evidencia

Se adjunta el archivo:

```text
Serial_Monitor_Practica4.txt
```

el cual contiene la salida completa del monitor serial durante la ejecución del sistema.

---

## Conclusión

Durante la práctica se implementó un sistema multitarea basado en FreeRTOS utilizando colas para la comunicación entre tareas y TaskHandle_t para el control de ejecución. El filtro de mediana permitió estabilizar las lecturas del sensor y el TaskManager centralizó las decisiones del sistema, logrando una arquitectura modular y robusta para aplicaciones embebidas.
