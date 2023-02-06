/*
Esta clase abstracta es usada para llamar a un mismo metodo que variara su funcion dependiendo de la escena seteada.
*/
#ifndef ESCENAS_H
#define ESCENAS_H
#include "Juego.h"
#include <SFML/Graphics/RenderWindow.hpp>
using namespace sf;
class Juego;

class Escenas {
public:
	Escenas(RenderWindow &ventana);
	virtual void Actualizar(Juego &juego,RenderWindow &ventana) = 0;
	virtual void Dibujar(RenderWindow &ventana) = 0;
private:
};

#endif

