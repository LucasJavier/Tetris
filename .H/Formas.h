/*
Esta clase es la encargada de verificar la colision de las formas con otros objetos; de verificar que estas no se salgan
de la matriz; y de moverlas verticalmente hacia abajo. Asimismo, se encarga de crear y mostrar hasta que punto puede caer la
forma con la orientacion que tenga en ese momento.
*/
#ifndef FORMAS_H
#define FORMAS_H
#include "Escalas.h"
#include "Globales.h"
using namespace sf;

class Formas {
public:
	Formas();
	void Actualizar(vector<vector<int>> &matrizCeldas,int valorForma,int &score);
	void Dibujar(RenderWindow &ventana);
	bool VerificarMovimientos(vector<vector<int>> &matrizCeldas,int valorForma);
	bool VerificarColision(vector<vector<int>> &matrizCeldas,int valorForma);
	void MuestraPieza(vector<vector<int>> &matrizCeldas,bool caidaRapida);
	void LimpiarSombra(vector<vector<int>> &matrizCeldas);
private:
	Music musicaUno,musicaDos; //Los efectos de sonido a reproducirse.
	vector<Vector2i> guardarNueves,guardarUnos,guardarPosicion; /*Vectores donde se guardaran los '1' de la matriz; los '1' corridos
																hacia abajo; y los '-9'.*/
	bool puedePasar,debeGuardar,debeRealizar,cambiarSonido,cargoSonidoUno,cargoSonidoDos;
	/*
		puedePasar: estado usado para verificar si ciertas acciones deben efectuarse o no
		debeGuardar: estado usado para determinar que una nueva sombra ha de ser dibujada, ya que el usuario a movido la forma.
		debeRealizar: estado usado para realizar una unica vez el proceso de guardado de la posicion de la sombra.
		cambiarSonido: estado usado para cambiar entre los 2 efectos de sonidos disponibles.
		cargoSonidoUno y cargoSonidoDos: estado usado para saber si los archivos musicales fueron cargados con normalidad.
	*/
	bool Mover(vector<vector<int>> &matrizCeldas,int valorForma,int &score);
	void ConsolidarForma(vector<vector<int>> &matrizCeldas,int valorForma);
	bool UltimaColuma(const vector<vector<int>> &matrizCeldas);
	bool ObtenerColision(const vector<vector<int>> &matrizCeldas);
	bool UltimaColuma(const vector<Vector2i> posicionesUnos);
	bool ObtenerColision(const vector<vector<int>> &matrizCeldas,const vector<Vector2i> posicionesUnos);
};

#endif

