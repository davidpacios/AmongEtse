#ifndef AMONGFINAL_JUGAR_H
#define AMONGFINAL_JUGAR_H

#include "Mapas.h"
#include "Listado.h"
#include "FuncionesComunesJugarYMapas.h"

///////////////////////////FUNCIONES:

/**
 * Menu de preparacion de la partida
 * @param G
 * @param Usuarios
 */
void menuAntesJugar(grafo G, abb Usuarios);

/**
 * Controlar el inicio del Juego, Se deberia controlar que haya suficientes arcos?
 * @param G
 * @param Jugadores
 * @return 1 en Condicion Aceptada
 */
int condicionJugar(grafo G, abb Jugadores);
#endif //AMONGFINAL_JUGAR_H
