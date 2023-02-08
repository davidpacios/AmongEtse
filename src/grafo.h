#ifndef GRAFO_H

#define GRAFO_H

#include "config.h"
/*
 * Implementación estática del TAD grafo con una matriz
 * de adjacencias con máximo número de vértices MAXVERTICES 
 */

/////////////////////////////////////////////////////////// TIPOS DE DATOS
#define location_length 25
//Información que se almacena en cada vértice
typedef struct{
    char habitacion[location_length];
} tipovertice;
//Informacion de las tareas del mapa
typedef struct{
    char nombreTarea[task_length];
    int costeTarea;
}tipotareas;

typedef struct tipografo * grafo;

//////////////////////////////////////////////////////////////// FUNCIONES

/**
 * Creación del grafo con 0 nodos
 * @param G
 */
void crear_grafo(grafo *G);

/**
 * Devuelve la posición del vértice Vert en el vector VERTICES del grafo G,Si devuelve -1 es porque no encontró el vértice
 * @param G
 * @param Vert
 * @return
 */
int posicion(grafo G, tipovertice Vert);

/**
 * Devuelve 1 si el grafo G existe y 0 en caso contrario
 * @param G
 * @return
 */
int existe(grafo G);

/**
 * Devuelve 1 si el vértice Vert existe en el grafo G
 * @param G
 * @param Vert
 * @return
 */
int existe_vertice(grafo G, tipovertice Vert);

/**
 * Inserta un vértice en el grafo,Devuelve la posición en el que ha sido insertado el,vértice o -1 si no se ha conseguido insertar el vértice
 * @param G
 * @param Vert
 * @return
 */
int insertar_vertice(grafo *G, tipovertice Vert);

/**
 * Borra un vértice del grafo
 * @param G
 * @param Vert
 */
void borrar_vertice(grafo *G, tipovertice Vert);

/**
 * Crea el arco de relación entre VERTICES(pos1) y VERTICES(pos2) de los Tripulantes
 * @param G
 * @param pos1
 * @param pos2
 * @param valor
 */
void crear_arco_T(grafo *G, int pos1, int pos2, int valor);

/**
 * Crea el arco de relación entre VERTICES(pos1) y VERTICES(pos2) de los Impostores
 * @param G
 * @param pos1
 * @param pos2
 * @param valor
 */
void crear_arco_I(grafo *G, int pos1, int pos2, int valor);

/**
 * Borra el arco de relación entre VERTICES(pos1) y VERTICES(pos2) de los Tripulantes
 * @param G
 * @param pos1
 * @param pos2
 */
void borrar_arco_T(grafo *G, int pos1, int pos2);

/**
 * Borra el arco de relación entre VERTICES(pos1) y VERTICES(pos2) de los Impostores
 * @param G
 * @param pos1
 * @param pos2
 */
void borrar_arco_I(grafo *G, int pos1, int pos2);

/**
 * Devuelve el valor si VERTICES(pos1) y VERTICES(pos2) son vértices adyacentes de los Tripulantes
 * @param G
 * @param pos1
 * @param pos2
 * @return
 */
int son_adyacentes_T(grafo G, int pos1, int pos2);

/**
 * Devuelve el valor si VERTICES(pos1) y VERTICES(pos2) son vértices adyacentes de los Impostores
 * @param G
 * @param pos1
 * @param pos2
 * @return
 */
int son_adyacentes_I(grafo G, int pos1, int pos2);

/**
 * Destruye el grafo
 * @param G
 */
void borrar_grafo(grafo *G);

/**
 * Devuelve el número de vértices del grafo G
 * @param G
 * @return
 */
int num_vertices(grafo G);

/**
 * Devuelve el vector de vértices VERTICES del grafo G
 * @param G
 * @return
 */
tipovertice* array_vertices(grafo G);

/**
 * Devuelve el numero de Tareas
 * @param G
 * @return
 */
int num_tareas(grafo G);

/**
 * Devuelve el array TAREAS
 * @param G
 * @return
 */
tipotareas * array_tareas(grafo G);

/**
 * Funcion para Insertar las tareas asignadas al grafo, devuelve -1 si esta lleno el grafo
 * @param G
 * @param tarea
 * @return
 */
int insertar_tareas(grafo *G, tipotareas tarea);

/**
 * Funcion para introducir nombre a un grafo
 * @param G
 * @param nombre
 */
void introducirNombre_Grafo(grafo G, char* nombre);

/**
 * Recupera el Nombre del Grafo
 * @param G
 * @return
 */
char* recuperarNombre(grafo G);

/**
 * Eliminar las tareas del grafo, devuelve -1 si esta vacio
 * @param G
 * @param posicion
 * @return
 */
int eliminar_tareas(grafo *G, int posicion);

/**
 * Comprueba que todos los vertices tienen algun arco, que no sea un loop
 * @param G
 * @return
 */
int todosVadyacentes(grafo G);
#endif	/* GRAFO_H */
