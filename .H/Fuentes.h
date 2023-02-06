/*
Esta clase es utilizada para setear y verificar/usar distintas fuentes para el uso de textos durante la partida.
*/
#ifndef FUENTES_H
#define FUENTES_H
#include <SFML/Graphics/Font.hpp>
#include <iostream>
using namespace std;
using namespace sf;

class Fuentes {
public:
	Fuentes();
	bool EstadoFuenteMenu();
	bool EstadoFuenteTetris();
	bool EstadoFuenteOpciones();
	bool EstadoFuenteInformacion();
	Font& VerFuenteMenu();
	Font& VerFuenteTetris();
	Font& VerFuenteOpciones();
	Font& VerFuenteInformacion();
private:
	bool estadoFuenteMenu,estadoFuenteTetris,estadoFuenteOpciones,estadoFuenteInfo;
	/*
		estadoFuenteMenu: estado que verifica si la fuente 'Menu.ttf' ha sido cargada o no con normalidad.
		estadoFuenteTetris: estado que verifica si la fuente 'Tetris.ttf' ha sido cargada o no con normalidad.
		estadoFuenteOpciones: estado que verifica si la fuente 'Opciones.ttf' ha sido cargada o no con normalidad.
		estadoFuenteInfo: estado que verifica si la fuente 'Informacion.ttf' ha sido cargada o no con normalidad.
	*/
	Font fuenteMenu,fuenteTetris,fuenteOpciones,fuenteInformacion;
	/*
		Las fuentes usadas para los textos:
			1) fuenteMenu: 'Menu.ttf'.
			2) fuenteTetris: 'Tetris.ttf'.
			3) fuenteOpciones: 'Opciones.ttf'.
			4) fuenteInformacion: 'Informacion.ttf'.
	*/
};

#endif

