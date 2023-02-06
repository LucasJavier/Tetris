/*
Esta clase es la encargada de mostrar al usuario, y permitirle cambiar tambien, parametros del juego: los colores de las formas;
la dificultad de la partida; los controlos para mover y rotar la pieza; y con que tecla poder poner en pausa el juego.
*/
#ifndef OPCIONES_H
#define OPCIONES_H
#include "Escenas.h"
#include "Menu.h"
#include "Globales.h"
#include "Archivo.h"
#include "Fuentes.h"
using namespace sf;
class Juego;

class Opciones : public Escenas {
public:
	Opciones(RenderWindow &ventana);
	void Dibujar (RenderWindow & ventana) override;
	void Actualizar (Juego & juego, RenderWindow & ventana) override;
private:
	int y, opcionSeleccionada;/*
		y: posicion vertical del texto.
		opcionSeleccionada: que opcion ha seleccionado el usuario dentro de las posibles (cambiar la dificultad; mover a la izquierda;
		mover a la derecha; mover hacia abajo; rotar forma; hardDrop; poner pausa).
	*/
	bool guardarArchivo,modificarDificultad,modificarColor,modificarPrendido;
	/*
		guardarArchivo: verificar si llama o no al metodo 'ModificarArchivo' de la clase 'Archivo'.
		modificarDificultad: verificar si se debe cambiar los textos relacionadas a la dificultad.
		modificarColor: verificar si se debe cambiar los textos relacionados a el color.
	*/
	Font fuenteOpciones, fuenteEleccion,fuenteInformacion; //Las fuentes usadas por los textos.
	vector<Text> vectorTexto; //Vector de textos usados.
	Vector2i posicionMouse; //Variable que guarda la posicionMouse x-y del mouse.
	FloatRect posicionOpcion; //Variable que guarda la posicion total de los distintos textos/textura de la ventana.
	Clock reloj,relojInformacion; /*Clock para verificar, en el primer caso, que no hagan muchos clicks en un corto periodo de tiempo y,
									en el segundo caso, si se ha superado una cantidad establecida de tiempo.*/
	Texture tFlecha; //La imagen de la flecha usada.
	Sprite sFlecha; //Para poder modificar aspectos de la imagen.
	Archivo archivo; //Objeto de la clase 'Archivo' para hacer uso del metodo 'ModificarArchvio'.
	Fuentes fuente; //Objeto de la clase 'Fuentes' utilizado para verificar y usar en los textos la fuente deaseada.
	Image icono; //Utilizado para cargar la imagen del icono de programa.
	void CrearTexto();
	void OpcionesGenerales(Text &texto,Font &fuente,bool estatico,string titulo,int tamanio,Color color);
	int ObtenerOpcionElegida(Vector2i posicionMouse);
	void RealizarCambio(int opcionSeleccionada);
	void CambiarTexto(int opcionSeleccionada);
	void ChequearMouse(RenderWindow &ventana);
};

#endif

