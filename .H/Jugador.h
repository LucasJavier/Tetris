/*
Esta clase es usada para efectuar todos los cambios correspondientes a los hechos por el usuario cuando aprieta una tecla.
*/
#ifndef JUGADOR_H
#define JUGADOR_H
#include "Escalas.h"
#include "Formas.h"
#include "Globales.h"
using namespace sf;

class Jugador {
public:
	Jugador();
	bool Actualizar(vector<vector<int>> &matrizCeldas,int forma,int &score);
	bool TecleoPausa();
private:
	int puedeMoverse,numRotaciones,debeRotar;/*
		puedeMoverse: si la suma llega a la establecida, se permite realizar la accion de mover la pieza.
		numRotaciones: contador para las rotaciones de las piezas.
		debeRotar: si la suma llega a la establecida, se permite verificar si se puede realizar la rotacion.
	*/
	bool cambioDeLado,primeraVezPausa,cargoSonidoUno,cargoSonidoDos; 
	/*
		cambioDeLado: si se debe modificar el 'centro' de rotacion de la pieza.
		primeraVezPausa: estado que informa si es la primera vez que se ha puesto en Pausa desde que se encontraba el juego corriendo.
		cargoSonidoUno y cargoSonidoDos: estado usado para corroborar si los archivos musicales pudieron ser cargados.
	*/
	Clock reloj; //Clock usado para que las acciones no se hagan 'muy rapido'.
	Formas formas; //Objeto de la clase 'Formas' usado para llamar a los metodos 'VerificarMovimientos' y 'VerificarColision'.
	Music musicaUno,musicaDos; //Los efectos de sonido utilizados en esta clase.
	void MoverIzq(vector<vector<int>> &matrizCeldas);
	void MoverDer(vector<vector<int>> &matrizCeldas);
	void MoverAbajo(vector<vector<int>> &matrizCeldas,int &score);
	bool CorroborarMover(vector<vector<int>> &matrizCeldas,int i,int j,int lado);
	void RotarForma(vector<vector<int>> &matrizCeldas,int forma);
	bool CorroborarRotar(vector<vector<int>> &matrizCeldas,int valorForma);
	bool CorroborarObjeto(vector<vector<int>> &matrizCeldas,int valorForma);
	void BajarForma(vector<vector<int>> &matrizCeldas,int valorForma,int &score);
};

#endif

