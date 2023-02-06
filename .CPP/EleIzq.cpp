#include "EleIzq.h"

/*
El constructor modifica el contenido numerico de la matriz pasada, con el objetivo de cambiar de color ciertas celdas,
de manera de dar la ilusion de creacion de una forma tipo 'EleIzq'. Dependiendo del estado de la variable tipo bool, 'matrizPrincipal',
la modificacion se hara en la matriz 'matrizCelda' (matriz principal del juego), o en la matriz 'matrizFormaSiguiente' (matriz que
muestra la forma que se creara a continuacion).
*/
EleIzq::EleIzq(vector<vector<int>> &matriz,bool matrizPrincipal) {
	if(matrizPrincipal){
		matriz[columnas/2-1][1] = 1;
		matriz[columnas/2-1][0] = 1;
		matriz[columnas/2][1] = 1;
		matriz[columnas/2+1][1] = 1;
	}else{
		matriz[2][2] = 3;
		matriz[2][3] = 3;
		matriz[3][3] = 3;
		matriz[4][3] = 3;
	}
}

