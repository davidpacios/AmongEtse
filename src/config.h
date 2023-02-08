#ifndef CONFIG_H
#define CONFIG_H

#include "colors.h"
//El mapa default es Skeld.txt y la lista de jugadores Jugadores.txt
/////////////////////////DEFAULT:
#define DEFAULTMAP "Skeld.txt"
#define DEFAULTLIST "Jugadores.txt"

/////////////////////////ESTADOS DE UN JUGADOR:
#define NADA 0
#define MOVIENDOSE 1
#define TAREA 2
#define ESPERANDO 3
#define TERMINADO 4
/////////////////////////CONFIGURACIONES DE LA PARTIDA:
#define jugadoresXpartida 10                //jugadores por partida
#define task_numberT 5                      //Numero tareas Tripulantes
#define task_numberI 7                      //Numero tareas Impostores
#define pasosTXronda 3                      //Pasos sumados por ronda Tripulantes
#define pasosIXronda 3                      //Pasos sumados por ronda Impostor
#define pasosTerminado 2                    //Pasos sumados para los jugadores que han acabado
#define minJugadores 5                      //Minimo Jugadores por partida
#define minVertices 5                       //Minimo Habitaciones por partida
#define minTareas 5                         //Minimo de tareas introucidas en un mapa

/////////////////////////Lengths:
#define name_length 20
#define location_length 25                   //caracteres maximos para una habitacion
#define task_length 35                      //caracteres maximos para un nombre de tarea
#define fichero_length 25

/////////////////////////Rutas:
#define carpetaMapas "./Mapas/"
#define carpetaListados "./Listados/"

/////////////////////////Valores:
#define infinito 1000
#define MAXVERTICES 100 /*maximo numero de nodos*/
#define MAXTAREAS 30 //Numero maximo de tareas para un mapa
#define enTarea "REALIZANDO TAREA"
#define tareasTerminadas "TERMINADO"

#endif //CONFIG_H
