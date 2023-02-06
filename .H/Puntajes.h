/*
Esta clase es la encargada de mostrar en pantalla los puntajes guardados, dependiendo la dificultad establecida por el usuario.
*/
#ifndef PUNTAJES_H
#define PUNTAJES_H
#include "Escenas.h"
#include "Menu.h"
#include "Globales.h"
#include "Archivo.h"
#include "Fuentes.h"
using namespace sf;

struct Usuario;

class Puntajes : public Escenas {
public:
	Puntajes(RenderWindow &ventana);
	void Actualizar (Juego &juego, RenderWindow &ventana) override;
	void Dibujar (RenderWindow &ventana) override;
private:
	Archivo archivo; //Objeto de la clase 'Archivo' para hacer uso del metodo 'LeerScore'.
	int tamanioArchivo, y, dificultadAux;
	/*
		tamanioArchivo: variable que guarda la dimension del archivo abierto.
		y: posicion vertical del texto en el momento de ser posicionado.
		dificultadAux: segun su valor guardado, cambia los puntajes mostrados, si cambiar la dificultad del juego.
	*/
	string m_usuario, m_puntos; //Variables que guardan el nombre y el puntaje del jugador.
	vector<Usuario> vectorUsuario; //Cector de Usuario (Usuario es un struct conformado por un char[20] y un int);
	vector<Text> vectorTexto; //Cector de textos.
	Vector2i posicionMouse; //Cariable que guarda la posicionMouse x-y del mouse.
	FloatRect posicionOpcion; //Cariable que guarda la posicion total de los distintos textos/textura de la ventana.
	Font fuenteDificultad,fuenteInformacion; //Las fuentes que utilizan los textos.
	Texture tFlecha; //Imagen de la flecha.
	Sprite sFlecha; //Para cambiar aspectos de la imagen.
	Clock reloj; //Clock para que no haya muchos clicks seguidos en un corto periodo de tiempo.
	Fuentes fuente; //Objeto de la clase 'Fuentes' utilizado para verificar y usar en los textos la fuente deaseada.
	Image icono; //Utilizado para cargar la imagen del icono de programa.
	void CrearTexto(Text &textoNombre, Text &textoPunto,  string nombre, string score);
	void DibujarTexto(RenderWindow &ventana);
	bool CliqueoDificultad(Vector2i posicionMouse);
	bool CliqueoFlecha(Vector2i posicionMouse);
	void CambiarArchivo(int dificultadAux);
};

#endif

