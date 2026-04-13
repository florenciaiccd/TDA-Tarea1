#include "tdas/list.h"
#include "tdas/extra.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "tdas/queue.h"
#include <time.h>

typedef struct{
  char Nombre[50];
  Queue* Tareas;
} Categorias;

typedef struct{
  char Nombre[50];
  char Hora[20];
  time_t tiempo_raw;
} Tareas;


// Menú principal
void mostrarMenuPrincipal() {
  limpiarPantalla();
  puts("========================================");
  puts("     Planificador de Tareas");
  puts("========================================");

  puts("1) Nueva Categoría");
  puts("2) Eliminar Categoría");
  puts("3) Mostrar Categorías");
  puts("4) Nueva Tarea");
  puts("5) Atender Siguiente");
  puts("6) Visualización del Tablero General");
  puts("7) Filtrado por Categoría");
  
  puts("8) Salir");
}

void convertir_mayus(char * nombre){
  for(int i = 0; nombre[i] !=  '\0'; i++){
    nombre[i] = toupper(nombre[i]);  
  }
} 

bool estaenLista(List *Lista_categorias, char * nombre){ 
  
  Categorias * aux = list_first(Lista_categorias);

  convertir_mayus(nombre);
  convertir_mayus(aux -> Nombre);

  while(aux != NULL){
    if( strcmp(aux -> Nombre, nombre) == 0){
      return true;
    }
    aux = list_next(Lista_categorias);
  }
  return false;
}

void burbujas_sort(List * ListaTotaltareas){
  if(ListaTotaltareas == NULL || list_size(ListaTotaltareas) <= 1){
    return;
  }
  bool intercambio;

  do{
    intercambio = false;
    Tareas *actual = list_first(ListaTotaltareas);
    Tareas *siguiente = list_next(ListaTotaltareas);

    while(siguiente != NULL){
      if (actual->tiempo_raw > siguiente->tiempo_raw){
        Tareas temp = *actual;
        *actual = *siguiente;
        *siguiente = temp;
        intercambio = true;
      }
      actual = siguiente;
      siguiente = list_next(ListaTotaltareas);
    }
  }
  while (intercambio);
}


void registrar_categorias(List *Lista_categorias) {
  char nombre_categoria[50];
  Categorias *categoria = (Categorias*)malloc(sizeof(Categorias));
  categoria -> Tareas =  queue_create(NULL); // se crea una cola para cada categoria :3
  
  printf("Ingrese el nombre de la categoría\n");
  scanf(" %[^\n]", nombre_categoria);

  if(list_size(Lista_categorias) == 0){
    strcpy(categoria -> Nombre, nombre_categoria);
    list_pushBack(Lista_categorias, categoria);
  }

  else if(estaenLista(Lista_categorias, nombre_categoria) == 1){
    printf("La categoría ya ha sido ingresada\n");
    return;
  }

  else{
    strcpy(categoria -> Nombre, nombre_categoria);
    list_pushBack(Lista_categorias, categoria);
  }
}


void mostrar_categorias(List *Lista_categorias) {
  if(list_first(Lista_categorias) == NULL){
    printf("No existen categorías previas. \n");
  }

  else{
    Categorias * aux = list_first(Lista_categorias);
    printf("Categorías:\n");
    while (aux != NULL){
      printf("%s \n", aux ->Nombre);
      aux = list_next(Lista_categorias);
    }
  }
}

void eliminar_categoria(List * Lista_categorias){
  char nombre_categoria[50];
  printf("Ingrese el nombre de la categoría a eliminar\n");
  scanf(" %[^\n]", nombre_categoria);
  convertir_mayus(nombre_categoria);

  if(list_first(Lista_categorias) == NULL ){
    printf("No existen categorías previas. \n");
    return;
  }

  else{
    Categorias * aux = list_first(Lista_categorias);
    while(aux != NULL){

      if( strcmp(aux -> Nombre, nombre_categoria) == 0){ 
        if(aux -> Tareas != NULL){
          queue_clean(aux ->Tareas);
        }
        list_popCurrent(Lista_categorias);                
        return;
      }
        aux = list_next(Lista_categorias);
      }
    printf("No existen categorías con ese nombre, inténtelo nuevamente.\n");
    }
}

void registrar_tareas(List* Lista_categorias){
  char nombre_categoria[50];
  printf("Ingrese el nombre de la categoría que pertenece la tarea\n");
  scanf(" %[^\n]", nombre_categoria);
  convertir_mayus(nombre_categoria);

  if(list_first(Lista_categorias) == NULL ){
    printf("No existen categorías previas. \n");
    return;
  }

  else{
    char nombre_tarea[50];
    Tareas *tarea = (Tareas*)malloc(sizeof(Tareas));
    Categorias * aux = list_first(Lista_categorias);

    time_t horaActual = time(NULL);  
    tarea ->tiempo_raw = horaActual;   
    struct tm *infoTiempo = localtime(&horaActual);
    
    while(aux != NULL){

      if( strcmp(aux -> Nombre, nombre_categoria) == 0){ 
        printf("Ingrese el nombre de la tarea: \n");
        scanf(" %[^\n]", nombre_tarea);
        strcpy(tarea -> Nombre, nombre_tarea);
        queue_insert(aux -> Tareas, tarea);
        strftime(tarea -> Hora, sizeof(tarea ->Hora), "%H:%M:%S", infoTiempo); // ns pq no me da la hora....
        return;
      }
        aux = list_next(Lista_categorias);
    }
    printf("No existen categorías con ese nombre, inténtelo nuevamente.\n");
  }
}

void atender_siguiente(List * Lista_categorias){
  if(list_first(Lista_categorias) == NULL){
    printf("No existen categorías previas. \n");
    return;
  }
    Categorias *aux = list_first(Lista_categorias);
    Tareas *masAntigua = NULL;
    Categorias *Ganadora = NULL;
    
    while(aux != NULL){
      Tareas *actual = queue_front(aux->Tareas); // reviso solo la del frente pq es la más antigua duh
      
      if(actual != NULL){
        if (masAntigua == NULL || actual -> tiempo_raw < masAntigua-> tiempo_raw) {
          masAntigua = actual;
          Ganadora = aux;
        }
      }
      aux = list_next(Lista_categorias);
    }

    if (masAntigua != NULL) {
      printf("Atendiendo: %s | Categoría: %s | Registrada a las: %s\n", masAntigua->Nombre, Ganadora->Nombre, masAntigua->Hora);
      queue_remove(Ganadora -> Tareas);
    }

    else{
      printf("¡Libre de pendientes!");
    }
}

void tablero_general(List *Lista_categorias){
  List *ListaTotaltareas = list_create();
  Categorias *aux = list_first(Lista_categorias);

  while(aux != NULL){
    List * Lista_cola = (List *)aux->Tareas; // no es lo mismo recorrer una lista a una cola y no sabía....
    Tareas* actual = list_first(Lista_cola);
    
    while(actual != NULL){
      list_pushBack(ListaTotaltareas, actual);
      actual = list_next(Lista_cola);
    }
    aux = list_next(Lista_categorias);
  }

  if(ListaTotaltareas == NULL){
    printf("No existen tareas previas. \n");
  }

  else{
    burbujas_sort(ListaTotaltareas);
    Tareas * aux = list_first(ListaTotaltareas);
    printf("Tablero general :\n");
    while (aux != NULL){
      printf("%s \n", aux -> Nombre );
      aux = list_next(ListaTotaltareas);
    }
  }
}

void filtracion(List * Lista_categorias){
  char nombre_categoria[50];
  printf("Ingrese el nombre de la categoría a mostrar\n");
  scanf(" %[^\n]", nombre_categoria);
  convertir_mayus(nombre_categoria);
  
  if(list_first(Lista_categorias) == NULL){
    printf("No existen categorías previas. \n");
    return;
  }

  Categorias *aux = list_first(Lista_categorias);
  while(aux != NULL){
    
    if(strcmp(aux -> Nombre, nombre_categoria) == 0){
      
      List * Lista_tareasXcategoria = (List *)aux->Tareas;
      Tareas *actual = list_first(Lista_tareasXcategoria);
      
      if(actual == NULL){
        printf("No existen tareas para esta categoría. \n");
        return;
      }

      while(actual != NULL){
        printf(" %s\n", actual -> Nombre);
        actual = list_next(Lista_tareasXcategoria);
      }
    }
    aux = list_next(Lista_categorias);
  }
}

int main() {
  char opcion;
  List *Lista_categorias = list_create(); // Lista para almacenar categorías o.O
  

  do {
    mostrarMenuPrincipal();
    printf("Ingrese su opción: ");
    scanf(" %c", &opcion); // Nota el espacio antes de %c para consumir el
                           // newline anterior

    switch (opcion) {
    case '1':
      registrar_categorias(Lista_categorias);
      break;
    case '2':
      eliminar_categoria(Lista_categorias);
      break;
    case '3':
      mostrar_categorias(Lista_categorias);
      break;
    case '4':
      registrar_tareas(Lista_categorias);
      break;
    case '5':
      atender_siguiente(Lista_categorias);
      break;
    case '6':
      tablero_general(Lista_categorias);
      break;
    case '7':
      filtracion(Lista_categorias);
      break;
    case '8':
      puts("Saliendo del planificador de tareas...");
      break;
    default:
      puts("Opción no válida. Por favor, intente de nuevo.");
    }
    presioneTeclaParaContinuar();

  } while (opcion != '8');

  // Liberar recursos, si es necesario
  list_clean(Lista_categorias);

  return 0;
}
