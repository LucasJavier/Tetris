#include "Fuentes.h"

/*
El constructor carga (si es posible) las distintas fuentes a utilizar en la partida y setea sus estados:
	true: se encontro y se cargo.
	false: no se encontro y no se pudo cargar.
*/
Fuentes::Fuentes() {
	if(!fuenteMenu.loadFromFile("Fuentes/Menu.ttf")) { 
		cerr<<"No se pudo abrir la fuente 'Menu.ttf'"<<endl;
		estadoFuenteMenu = false;
	}
	else estadoFuenteMenu = true;
	if(!fuenteTetris.loadFromFile("Fuentes/Tetris.ttf")) {
		cerr<<"No se pudo abrir la fuente 'Tetris.ttf"<<endl;
		estadoFuenteTetris = false;
	}
	else estadoFuenteTetris = true;
	if(!fuenteOpciones.loadFromFile("Fuentes/Opciones.ttf")) {
		cerr<<"No se pudo abrir la fuente 'Opciones.ttf"<<endl;
		estadoFuenteOpciones = false;
	}
	else estadoFuenteOpciones = true;
	if(!fuenteInformacion.loadFromFile("Fuentes/Informacion.ttf")) {
		cerr<<"No se pudo abrir la fuente 'Informacion.ttf'"<<endl;
		estadoFuenteInfo = false;
	}
	else estadoFuenteInfo = true;
}

//Esta funcion sirve para devolver el estado de carga de fuente 'Menu.ttf'.
bool Fuentes::EstadoFuenteMenu(){
	return estadoFuenteMenu;
}

//Esta funcion sirve para devolver el estado de carga de fuente 'Tetris.ttf'.
bool Fuentes::EstadoFuenteTetris(){
	return estadoFuenteMenu;
}

//Esta funcion sirve para devolver el estado de carga de fuente 'Opciones.ttf'.
bool Fuentes::EstadoFuenteOpciones(){
	return estadoFuenteMenu;
}

//Esta funcion sirve para devolver el estado de carga de fuente 'Informacion.ttf'.
bool Fuentes::EstadoFuenteInformacion(){
	return estadoFuenteMenu;
}

//Este metodo sirve para devolver la fuente 'Menu.ttf', de manera de ser usara para un texto.
Font& Fuentes::VerFuenteMenu(){
	return fuenteMenu;
}

//Este metodo sirve para devolver la fuente 'Tetris.ttf', de manera de ser usara para un texto.
Font& Fuentes::VerFuenteTetris(){
	return fuenteTetris;
}

//Este metodo sirve para devolver la fuente 'Opciones.ttf', de manera de ser usara para un texto.
Font& Fuentes::VerFuenteOpciones(){
	return fuenteOpciones;
}

//Este metodo sirve para devolver la fuente 'Informacion.ttf', de manera de ser usara para un texto.
Font& Fuentes::VerFuenteInformacion(){
	return fuenteInformacion;
}
