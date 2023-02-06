/*
Esta clase es en la cual se iniciar siempre el juego: un Menu con diferentes opciones para que el usuario eliga. 
En la ventana de esta clase se puede cambiar, a eleccion del usuario, a la clase Partida', 'Opciones, 'Puntajes', 
e incluso salir del programa.
*/
#ifndef MENU_H
#define MENU_H
#include "Escenas.h"
#include "Partida.h"
#include "Opciones.h"
#include "Puntajes.h"
#include "Globales.h"
class Juego;
using namespace sf;

class Menu : public Escenas{
public:
	Menu(RenderWindow &ventana);
	void Actualizar(Juego &juego,RenderWindow &ventana) override;
	void Dibujar(RenderWindow &ventana) override;
private:
	Clock reloj,relojMusica; /*Clock para verificar, en el primer paso, que pasado el tiempo necesario para crear otra forma y, en
							el segundo caso, para evitar multiples clicks en un corto periodo de tiempo.*/
	Text jugar, opciones, puntaje, salir; //los textos que se van a mostrar en pantalla.
	Texture tTetrisSimbolo,tMusicaPrendida,tMusicaApagada,tEfectoPrendido,tEfectoApagado; //Las imagenes que se utilizaran.
	Sprite sCargarSimbolo,sMusicaPrendida,sMusicaApagada,sEfectoPrendido,sEfectoApagado; //Para configurar las imagenes cargadas.
	vector<Vector2i> posicionRectangulo; /*En este vector de vectores2i se guardaran la posicion de los 4 rectangulos creados en la 
										funcion 'CrearForma'*/
	vector<RectangleShape> vectorFormas; //Con este vector se crearan y configuraran los distintos 'RectangleShape' creados.
	Vector2i posicionMouse; //En esta variable se guarda la posicion x-y del mouse.
	FloatRect coordenadasTexto; //En esta variable se guardan las distintas variables realacionadas a la posicion de un objeto en la ventana
	int y,segundos,n_forma,offset;/*
		y: hace referencia a la posicion de los RectangleShape creados.
		segundos: es la cantidad de tiempo a esperar para crear otra forma.
		n_forma: es el numero obtenido de manera pseudo-aleatoria para crear una determinada forma.
		offset: es cuanto ha de cambiar/mover la posicion de los RectangleShape.
	*/
	bool mover,cambioDeLado,debeCrear,sinFuente,esFuenteInfo,esFuenteTetris;/*
		mover: verificar si la forma creada debe moverse o no.
		cambioDeLado: verificar si debe cambiar el rango de creacion del numero pseudo-aleatorio.
		debeCrear: verificar si se debe llamar al metodo 'CrearForma' o no.
		esFuenteInfo: verifica si la fuente seteada es 'Informacion.ttf'.
		esFuenteTetris: verifica si la fuente setead es 'Tetris.ttf'.
		sinFuente: verifica si se ha podido cargar o no una fuente.
	*/
	float rojo,verde,azul; //Para la combinacion de colores RGB.
	Fuentes fuente; //Objeto de la clase 'Fuentes' utilizado para verificar y usar en los textos la fuente deaseada.
	Image icono; //Utilizado para cargar la imagen del icono de programa.
	void CrearTexto();
	void CrearCombinacion();
	bool CliqueoJugar(Vector2i &posicionMouse);
	bool CliqueoOpciones(Vector2i &posicionMouse);
	bool CliqueoScore(Vector2i &posicionMouse);
	bool CliqueoSalir(Vector2i &posicionMouse);
	bool CliqueoMusica(Vector2i &posicionMouse);
	bool CliqueoEfecto(Vector2i &posicionMouse);
	void CrearForma();
	void MoverForma();
	void DibujarForma(RenderWindow &ventana);
	void AspectosGeneralesTexto(Text &texto,string titulo,int tamanio,Color color);
	void ChequearMouse(RenderWindow &ventana);
	void CrearColores(bool colorNormal);
	void CargarTexturas();
};

#endif

