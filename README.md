# TDA-Tarea1

Planificador de Tareas

Descripción
Este código es un Planificador de Tareas en C que organiza actividades por Categorías.

Su objetivo es gestionar tareas usando una estructura de "Cola dentro de una Lista": las categorías se guardan en una lista y cada una tiene su propia cola de tareas (orden FIFO). Permite registrar, eliminar y filtrar tareas, además de usar el tiempo real ("time_t") para identificar y atender siempre la tarea más antigua de todo el sistema.

Como compilar
Se debe descargar el repositorio y usar replit y subiendo los archivos a este, crenado un shell donde se utilizaran los comandos:
  gcc tdas/*.c tarea1.c -Wno-unused-result -o tarea1
  y
  ./tarea1

  
