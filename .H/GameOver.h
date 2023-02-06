/*
Esta clase es la encargada de mostrar una ventana de 'juego terminado' al usuario cuando este pierde. Si el jugado ha superado
algun puntaje, la ventana mostrara y permitira el ingreso de teclas por el usuario, con el objetivo de guardar su nombre/alias,
como asi tambien su puntaje.
*/
#ifndef GAMEOVER_H
#define GAMEOVER_H
#include "Escenas.h"
#include "Escalas.h"
#include "Globales.h"
#include "Archivo.h"
#include "Fuentes.h"
using namespace sf;
class Juego;

class GameOver : public Escenas {
public:
	GameOver(RenderWindow &ventana,int score);
	void Actualizar(Juego &juego, RenderWindow &ventana) override;
	void Dibujar(RenderWindow &ventana) override;
private:
	float rojo,verde,azul; //Para la combinacion de colores RGB.
	int posicionEscribir, m_score, tamanioArchivo, y, scoreArchivo;
	/*
		posicionEscribir: la posicion donde debe (o no) ser escrito el nuevo puntaje hecho por el usuario.
		m_score: el puntaje hecho por el usuario durante la partida.
		tamanioArchivo: tamaño del archivo de puntajes, usado para recorrerlo en su totalidad.
		scoreArchivo: la puntuacion mas alta hecha, ya sea por el usuario o una anteriormente realizada y guardada.
	*/
	bool dejarDeEntrar; // Variable tipo bool para realizar el cambio de texto una sola vez.
	vector<Text> vectorTexto; //Vector contenedor de los textos mostrados en la ventana.
	Archivo archivo; //Objeto de la clase 'Archivo' utilizado para llamar a los metodos 'LeerHayMejorPuntaje' y 'EscribirPuntaje'.
	string palabraScore, m_scoreArchivo, nJugador; 
	/*
		palabraScore: el puntaje hecho por el usuario convertido de tipo int a tipo string.
		m_scoreArchivo: el puntaje mas alto hecho (ya sea por el usuario o una anterior guardada) convertido de tipo int a tipo string.
		nJugador: posicion de un puntaje guardado superado por el usuario, de manera de informar a quien ha adelantado.
	*/
	Music musica; //La musica utilizada en la ventana.
	Fuentes fuente; //Objeto de la clase 'Fuentes' utilizado para verificar y usar en los textos la fuente deaseada.
	Image icono; //Utilizado para cargar la imagen del icono de programa.
	void CrearTexto();
	void CrearCombinacion();
	void EscribirPuntaje(int m_score,int posicionEscribir);
	void AspectosGeneralesTexto(Text &texto,Font &fuente,string titulo,bool esTitulo,int tamanio,Color color);
};

#endif

