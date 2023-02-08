#ifndef AMONGFINAL_LISTADO_H
#define AMONGFINAL_LISTADO_H

#include "abb.h"
#include "FuncionesComunesJugarYMapas.h"


/**
 * Devuelve la variable global Lista Cargada
 * @return
 */
int lCargada();

/**
* Devuelve la variable global NombreLista
* @return
*/
char* nombreListCargada();

/**
 * lee un archivo de jugadores de un fichero
 * @param Usuarios
 * @param flagInicio para cargar DEFAULTLIST
 */
void leerArchivo_Jugadores(abb *Usuarios,int flagInicio);

/**
 * Menu Listado Jugadores
 * @param Usuarios
 */
void menuJugadoresPrincipal(abb *Usuarios);


#endif //AMONGFINAL_LISTADO_H
