#include "Cuadrado.h"

/*
El constructor modifica el contenido numerico de la matriz pasada, con el objetivo de cambiar de color ciertas celdas,
de manera de dar la ilusion de creacion de una forma tipo 'Cuadrado'. Dependiendo del estado de la variable tipo bool, 'matrizPrincipal',
la modificacion se hara en la matriz 'matrizCelda' (matriz principal del juego), o en la matriz 'matrizFormaSiguiente' (matriz que
muestra la forma que se creara a continuacion).
*/
Cuadrado::Cuadrado(vector<vector<int>> &matriz,bool matrizPrincipal){
	if(matrizPrincipal){
		matriz[(columnas/2)-1][0] = 1;
		matriz[columnas/2][0] = 1;
		matriz[(columnas/2)-1][1] = 1;
		matriz[columnas/2][1] = 1;
	}else{
		matriz[2][2] = 2;
		matriz[3][2] = 2;
		matriz[2][3] = 2;
		matriz[3][3] = 2;
	}
}


