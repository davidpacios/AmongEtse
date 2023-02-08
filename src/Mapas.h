#ifndef AMONGFINAL_MAPAS_H
#define AMONGFINAL_MAPAS_H

#include "grafo.h"
#include "FuncionesComunesJugarYMapas.h"


/**
 * Devuele la variable global de mapa Cargado
 * @return
 */
int mCargado();

/**
 * Menu Principal seccion de Mapa del menu en el main.c
 * @param G
 */
void menuMapaPrincipal(grafo *G);

/**
 * Lee las habitaciones,las comunicaciones y las tareas de un archivo e introducirlos en el mapa
 * @param G
 * @param flagInicio para cargar DEFAULTMAP
 */
void leerArchivo_Mapa(grafo *G,int flagInicio);

#endif //AMONGFINAL_MAPAS_H
