#include <stdio.h>
#include <stdlib.h>
#include <time.h> //para utilizar la semilla aleatoria time(0)


#include "Listado.h"
#include "Mapas.h"
#include "Jugar.h"
#include "FuncionesComunesJugarYMapas.h"



int main(int argc, char **argv) {
    srand((unsigned int) time(NULL)); //semilla
    int primeraEjecucion = 1;
    char opcion;
    grafo G; //grafo
    crear_grafo(&G);//Creo el grafo

    abb Usuarios;//arbol
    crear(&Usuarios);//Crear el árbol de jugadores

    if(crearCarpeta(carpetaListados) || crearCarpeta(carpetaMapas)){
        presionarContinuar();
    }

    //Menú
    do {
        limpiarPantallaTerminal();
        if(primeraEjecucion){
            leerArchivo_Mapa(&G,1);
            leerArchivo_Jugadores(&Usuarios,1);
            primeraEjecucion = 0;
        }

        printf("--------------------------------------------------------------------------\n");
        printf("--------------------------------------------------------------------------\n");
        printf("--------------------------"BOLD"Bienvenidos a AmongEtse"RESET_COLOR"--------------------------\n");
        if(lCargada() == 1){
            printf("--------------------------"BOLD VERDE_T"Lista Cargada:%s"RESET_COLOR"--------------------------\n",
                    nombreListCargada());
        }
        if(mCargado() == 1){
            printf("--------------------------"BOLD VERDE_T"Mapa Cargado:%s"RESET_COLOR"--------------------------\n",
                    recuperarNombre(G));
        }
        printf("a) Jugar\n");
        printf("b) Jugadores\n");
        printf("c) Mapa\n");
        printf("s) Salir\n\n");

        printf("Opcion:");
        scanf(" %c", &opcion);
        printf("--------------------------------------------------------------------------\n");
        printf("--------------------------------------------------------------------------\n");

        switch (opcion) {

            case 'a':
                if(mCargado() && lCargada()  ){
                    if(condicionJugar(G, Usuarios)){
                        menuAntesJugar(G,Usuarios);
                    } else{
                        printf("Debes tener %d Tareas %d Jugadores y %d Habitaciones con Comunicaciones SUFICIENTES\n",minTareas,minJugadores,minVertices);
                        presionarContinuar();
                    }

                }else{
                    printf("Debes seleccionar un mapa y una lista de jugadores antes de _jugar\n");
                    presionarContinuar();
                }

                break;
            case 'b':
                menuJugadoresPrincipal(&Usuarios);
                break;
            case 'c':
                menuMapaPrincipal(&G);
                break;
            case 's':
                printf("Hasta pronto...\n");
                break;
            default:
                printf("Introduce una opcion valida\n");
                presionarContinuar();
                break;
        }


    } while (opcion != 'S' && opcion != 's');

    borrar_grafo(&G);//Al salir, liberamos la memoria del TAD, lo destruimos
    destruir(&Usuarios);//Destruir el árbol al finalizar
    return (EXIT_SUCCESS);
}
