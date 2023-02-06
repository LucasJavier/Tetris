#include "Linea.h"

/*
El constructor modifica el contenido numerico de la matriz pasada, con el objetivo de cambiar de color ciertas celdas,
de manera de dar la ilusion de creacion de una forma tipo 'Linea'. Dependiendo del estado de la variable tipo bool, 'matrizPrincipal',
la modificacion se hara en la matriz 'matrizCelda' (matriz principal del juego), o en la matriz 'matrizFormaSiguiente' (matriz que
muestra la forma que se creara a continuacion).
*/
Linea::Linea(vector<vector<int>> &matriz,bool matrizPrincipal) {
	if(matrizPrincipal){
		matriz[columnas/2][1] = 1;
		matriz[columnas/2][0] = 1;
		matriz[columnas/2][2] = 1;
		matriz[columnas/2][3] = 1;
	}else{
		matriz[2][1] = 5;
		matriz[2][2] = 5;
		matriz[2][3] = 5;
		matriz[2][4] = 5;
	}
}

