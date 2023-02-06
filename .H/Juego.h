/*
Esta clase es la encargada de iniciar el juego con sus parametros, cambiar a las distintas escenas-clases cuando sea necesario, y
capturar los eventos que ocurran en la ventana.
*/
#ifndef JUEGO_H
#define JUEGO_H
#include "Escenas.h"
#include "Escalas.h"
#include "Globales.h"
#include "Archivo.h"
using namespace sf;
class Escenas;

class Juego {
public:
	Juego();
	void Run();	
	void SetiarEscena(Escenas *escena);
	~Juego();
private:
	Archivo archivo; //Objeto de la clase 'Archivos', creado para utilizar los metodos 'LeerControl' y 'LeerTeclas'.
	RenderWindow ventana;
	Escenas *escenaActual, *escenaSiguiente = nullptr; /*Mediante estos punteros se iran seteando las diferentes clases, cambiando, asi,
														que metodos 'Actualizar' y 'Dibujar' deben llamarse.*/
	vector<string> vectorCadenas; //En este vector se guardaran las cadenas de textos de los archivos abiertos.
	map<string,int> teclado; //Con este map se asociaran los distintas teclas con un valor.
	Clock reloj; //Clock usado para evitar multiples entradas de texto por apretar una tecla en un corto periodo de tiempo.
	void ProcesarEvento();
	void Actualizar();
	void Dibujar();
	void SetearControles();
	int RetornarValor(string control);
	void valoresTeclas();
};

#endif

