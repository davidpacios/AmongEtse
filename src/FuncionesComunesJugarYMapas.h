#ifndef AMONGFINAL_FUNCIONESCOMUNESJUGARYMAPAS_H
#define AMONGFINAL_FUNCIONESCOMUNESJUGARYMAPAS_H

#include "abb.h"
#include "grafo.h"


/////////Struct para nuestra matriz P
struct camino {
    int valor;
    char flagRol;
};

/**
 * Funcion EXTRA implementa la funcionalidad mitica de "ENTER PARA CONTINUAR"
 * @return 1
 */
int presionarContinuar();

/**
 * Funcion privada que posibilita introducir el nickname introducido o no
 * @param nombreJugador
 * @return 1 en caso de éxito
 */
int condicionNickname(tipoelem jugador);

/**
 * Funcion privada para solicitar el Nickname y lo devuelve.
 * @return tipoelem
 */
tipoelem solicitarNickname();

/**
 * Función que añade un jugador al árbol(opcion A)
 * @param Usuarios
 */
void altaJugador(abb *Usuarios);

/**
 * Función que elimina un jugador del árbol(opcion B)
 * @param Usuarios
 */
int bajaJugador(abb *Usuarios);

/**
 * Recorrido recursivo que llama a _imprimirJugadorIndividual()
 * @param Usuarios
 */
void listadoJugadores(abb Usuarios,int flafFinPartida);

/**
 * Funcion mkdir para ./Mapas y ./Listados
 * @param nombreCarpeta
 */
int crearCarpeta(char *nombreCarpeta);

/**
 * Funcion ls de shell implementada
 */
void archivosEn(char *directorio);

/**
 * Llama a system() para limpiar la terminal
 */
void limpiarPantallaTerminal();

/**
 * Funcion privada que calcula el Algoritmo de Floyd y Warshall
 * @param G
 * @param flagRol
 * @param numeroVertices
 * @param D
 * @param P
 */
void AlgoritmoFloyd_Warshall(grafo G, int flagRol, int numeroVertices, int D[][MAXVERTICES], struct camino P[][MAXVERTICES]);
#endif //AMONGFINAL_FUNCIONESCOMUNESJUGARYMAPAS_H
