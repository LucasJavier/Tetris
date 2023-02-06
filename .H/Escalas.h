/*
Esta clase es la encargada de declarar variables y funciones globales, como asi tambien constantes. Asimismo, contiene
los 'includes' necesarios para que el juego funcione correctamente.
*/
#ifndef ESCALAS_H
#define ESCALAS_H
#include "Globales.h"
#include <map>
#include <vector>
#include <cstring>
#include <fstream>
#include <iostream>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Audio/Music.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Image.hpp>
using namespace std;
using namespace sf;

const int columnas = 10; //Cantidad de columnas en la matriz.
const int filas = 20; //Cantidad de filas de la matriz.
const int tamanioCuadrado = 8; //Tamaño de los cuadrados contenidos en la matriz.
const int escala = 4; //Para reescalar la ventana.
extern vector<Vector2i> m_posicionUnos;  //vector de vectores2i para almacenar la posicion de los unos '1' de la matriz.
extern Music musicaMenus; //Musica global para ser compartida por la clase 'Menu', 'Opciones', y 'Puntajes'.

struct Usuario{ /*struct 'Usuario' que sera utilizado para almacenar y leer archivos binarios. En estos se guardara el nombre
				y el puntaje del usuario.*/
	char nombre[20];
	int scoreUsuario;
};


//Esta funcion almacena la posicion de los unos ('1') referentes a una matriz en un vector de vectores2i.
inline void ObtenerPosiciones(const vector<vector<int>> &matrizCeldas){
	for(int i=0;i<columnas;i++){
		for(int j=0;j<filas;j++){
			if(matrizCeldas[i][j] == 1) m_posicionUnos.push_back({i,j});
		}
	}
}
	
/*
Esta funcion es la encargada de modificar la Key-tecla con la que realiza ciertas acciones en el juego y, asimismo,
el texto correspondiente a la tecla apretada. Estos cambios se realizan cuando el usuario decide, en la pantalla de la clase
'Opciones'.
*/
inline void modificarTeclas(int valor,int numTecla){
	switch(numTecla){ //switch dependiendo que tecla ha de cambiarse.
	case 0:{ //Se guarda la tecla y el nombre para mover la forma hacia la izquierda.
		switch(valor){ //switch dependiendo el valor numerico de la tecla apretada.
		case -37: 
		case 81: /*De manera igual para todos los switches, se realiza el cambio de Key-tecla, para modificar con que tecla se realiza
				cierta accion; y se modifica el texto que muestra la tecla, para brindar ayuda visual al usuario.*/
		case 113: {Globales::CambiarTecla(0,Keyboard::Key::Q); Globales::CambiarCadena(0,"Q"); break;}
		case -36: 
		case 87:
		case 119: {Globales::CambiarTecla(0,Keyboard::Key::W); Globales::CambiarCadena(0,"W"); break;}
		case -35: 
		case 69:
		case 101: {Globales::CambiarTecla(0,Keyboard::Key::E); Globales::CambiarCadena(0,"E"); break;}
		case -34: 
		case 82:
		case 114: {Globales::CambiarTecla(0,Keyboard::Key::R); Globales::CambiarCadena(0,"R"); break;}
		case -33: 
		case 84: 
		case 116: {Globales::CambiarTecla(0,Keyboard::Key::T); Globales::CambiarCadena(0,"T"); break;}
		case -32: 
		case 89:
		case 121: {Globales::CambiarTecla(0,Keyboard::Key::Y); Globales::CambiarCadena(0,"Y"); break;}
		case -31: 
		case 85:
		case 117: {Globales::CambiarTecla(0,Keyboard::Key::U); Globales::CambiarCadena(0,"U"); break;}
		case -30: 
		case 73:
		case 105: {Globales::CambiarTecla(0,Keyboard::Key::I); Globales::CambiarCadena(0,"I"); break;}
		case -29: 
		case 79:
		case 111: {Globales::CambiarTecla(0,Keyboard::Key::O); Globales::CambiarCadena(0,"O"); break;}
		case -28: 
		case 80:
		case 112: {Globales::CambiarTecla(0,Keyboard::Key::P); Globales::CambiarCadena(0,"P"); break;}
		case -27: 
		case 65:
		case 97: {Globales::CambiarTecla(0,Keyboard::Key::A); Globales::CambiarCadena(0,"A"); break;}
		case -26:
		case 83:
		case 115: {Globales::CambiarTecla(0,Keyboard::Key::S); Globales::CambiarCadena(0,"S"); break;}
		case -25: 
		case 68:
		case 100: {Globales::CambiarTecla(0,Keyboard::Key::D); Globales::CambiarCadena(0,"D"); break;}
		case -24: 
		case 70:
		case 102: {Globales::CambiarTecla(0,Keyboard::Key::F); Globales::CambiarCadena(0,"F"); break;}
		case -23: 
		case 71:
		case 103: {Globales::CambiarTecla(0,Keyboard::Key::G); Globales::CambiarCadena(0,"G"); break;}
		case -22: 
		case 72:
		case 104: {Globales::CambiarTecla(0,Keyboard::Key::H); Globales::CambiarCadena(0,"H"); break;}
		case -21: 
		case 74:
		case 106: {Globales::CambiarTecla(0,Keyboard::Key::J); Globales::CambiarCadena(0,"J"); break;}
		case -20: 
		case 75:
		case 107: {Globales::CambiarTecla(0,Keyboard::Key::K); Globales::CambiarCadena(0,"K"); break;}
		case -19: 
		case 76:
		case 108: {Globales::CambiarTecla(0,Keyboard::Key::L); Globales::CambiarCadena(0,"L"); break;}
		case -18: 
		case 90:
		case 122: {Globales::CambiarTecla(0,Keyboard::Key::Z); Globales::CambiarCadena(0,"Z"); break;}
		case -17: 
		case 88:
		case 120: {Globales::CambiarTecla(0,Keyboard::Key::X); Globales::CambiarCadena(0,"X"); break;}
		case -16: 
		case 67: 
		case 99: {Globales::CambiarTecla(0,Keyboard::Key::C); Globales::CambiarCadena(0,"C"); break;}
		case -15: 
		case 86: 
		case 118: {Globales::CambiarTecla(0,Keyboard::Key::V); Globales::CambiarCadena(0,"V"); break;}
		case -14: 
		case 66:
		case 98: {Globales::CambiarTecla(0,Keyboard::Key::B); Globales::CambiarCadena(0,"B"); break;}
		case -13: 
		case 78:
		case 110: {Globales::CambiarTecla(0,Keyboard::Key::N); Globales::CambiarCadena(0,"N"); break;}
		case -12: 
		case 77: 
		case 109: {Globales::CambiarTecla(0,Keyboard::Key::M); Globales::CambiarCadena(0,"M"); break;}
		case -11: {Globales::CambiarTecla(0,Keyboard::Key::Left); Globales::CambiarCadena(0,"Left"); break;}
		case -10: {Globales::CambiarTecla(0,Keyboard::Key::Right); Globales::CambiarCadena(0,"Right"); break;}
		case -9: {Globales::CambiarTecla(0,Keyboard::Key::Up); Globales::CambiarCadena(0,"Up"); break;}
		case -8: {Globales::CambiarTecla(0,Keyboard::Key::Down); Globales::CambiarCadena(0,"Down"); break;}
		case -7: 
		case 32: {Globales::CambiarTecla(0,Keyboard::Key::Space); Globales::CambiarCadena(0,"Space"); break;}
		case -6: {Globales::CambiarTecla(0,Keyboard::Key::LControl); Globales::CambiarCadena(0,"LControl"); break;}
		case -5: {Globales::CambiarTecla(0,Keyboard::Key::LShift); Globales::CambiarCadena(0,"LShift"); break;}
		case -4: {Globales::CambiarTecla(0,Keyboard::Key::LAlt); Globales::CambiarCadena(0,"LAlt"); break;}
		case -3: {Globales::CambiarTecla(0,Keyboard::Key::RControl); Globales::CambiarCadena(0,"RControl"); break;}
		case -2: {Globales::CambiarTecla(0,Keyboard::Key::RShift); Globales::CambiarCadena(0,"RShift"); break;}
		case -1: {Globales::CambiarTecla(0,Keyboard::Key::RAlt); Globales::CambiarCadena(0,"RAlt"); break;}
		}
		break;
	}
	case 1:{ //Se guarda la tecla y el nombre para mover la forma hacia la derecha.
		switch(valor){
		case -37: 
		case 81:
		case 113: {Globales::CambiarTecla(1,Keyboard::Key::Q); Globales::CambiarCadena(1,"Q"); break;}
		case -36: 
		case 87:
		case 119: {Globales::CambiarTecla(1,Keyboard::Key::W); Globales::CambiarCadena(1,"W"); break;}
		case -35: 
		case 69:
		case 101: {Globales::CambiarTecla(1,Keyboard::Key::E); Globales::CambiarCadena(1,"E"); break;}
		case -34: 
		case 82:
		case 114: {Globales::CambiarTecla(1,Keyboard::Key::R); Globales::CambiarCadena(1,"R"); break;}
		case -33: 
		case 84: 
		case 116: {Globales::CambiarTecla(1,Keyboard::Key::T); Globales::CambiarCadena(1,"T"); break;}
		case -32: 
		case 89:
		case 121: {Globales::CambiarTecla(1,Keyboard::Key::Y); Globales::CambiarCadena(1,"Y"); break;}
		case -31: 
		case 85:
		case 117: {Globales::CambiarTecla(1,Keyboard::Key::U); Globales::CambiarCadena(1,"U"); break;}
		case -30: 
		case 73:
		case 105: {Globales::CambiarTecla(1,Keyboard::Key::I); Globales::CambiarCadena(1,"I"); break;}
		case -29: 
		case 79:
		case 111: {Globales::CambiarTecla(1,Keyboard::Key::O); Globales::CambiarCadena(1,"O"); break;}
		case -28: 
		case 80:
		case 112: {Globales::CambiarTecla(1,Keyboard::Key::P); Globales::CambiarCadena(1,"P"); break;}
		case -27: 
		case 65:
		case 97: {Globales::CambiarTecla(1,Keyboard::Key::A); Globales::CambiarCadena(1,"A"); break;}
		case -26:
		case 83:
		case 115: {Globales::CambiarTecla(1,Keyboard::Key::S); Globales::CambiarCadena(1,"S"); break;}
		case -25: 
		case 68:
		case 100: {Globales::CambiarTecla(1,Keyboard::Key::D); Globales::CambiarCadena(1,"D"); break;}
		case -24: 
		case 70:
		case 102: {Globales::CambiarTecla(1,Keyboard::Key::F); Globales::CambiarCadena(1,"F"); break;}
		case -23: 
		case 71:
		case 103: {Globales::CambiarTecla(1,Keyboard::Key::G); Globales::CambiarCadena(1,"G"); break;}
		case -22: 
		case 72:
		case 104: {Globales::CambiarTecla(1,Keyboard::Key::H); Globales::CambiarCadena(1,"H"); break;}
		case -21: 
		case 74:
		case 106: {Globales::CambiarTecla(1,Keyboard::Key::J); Globales::CambiarCadena(1,"J"); break;}
		case -20: 
		case 75:
		case 107: {Globales::CambiarTecla(1,Keyboard::Key::K); Globales::CambiarCadena(1,"K"); break;}
		case -19: 
		case 76:
		case 108: {Globales::CambiarTecla(1,Keyboard::Key::L); Globales::CambiarCadena(1,"L"); break;}
		case -18: 
		case 90:
		case 122: {Globales::CambiarTecla(1,Keyboard::Key::Z); Globales::CambiarCadena(1,"Z"); break;}
		case -17: 
		case 88:
		case 120: {Globales::CambiarTecla(1,Keyboard::Key::X); Globales::CambiarCadena(1,"X"); break;}
		case -16: 
		case 67: 
		case 99: {Globales::CambiarTecla(1,Keyboard::Key::C); Globales::CambiarCadena(1,"C"); break;}
		case -15: 
		case 86: 
		case 118: {Globales::CambiarTecla(1,Keyboard::Key::V); Globales::CambiarCadena(1,"V"); break;}
		case -14: 
		case 66:
		case 98: {Globales::CambiarTecla(1,Keyboard::Key::B); Globales::CambiarCadena(1,"B"); break;}
		case -13: 
		case 78:
		case 110: {Globales::CambiarTecla(1,Keyboard::Key::N); Globales::CambiarCadena(1,"N"); break;}
		case -12: 
		case 77: 
		case 109: {Globales::CambiarTecla(1,Keyboard::Key::M); Globales::CambiarCadena(1,"M"); break;}
		case -11: {Globales::CambiarTecla(1,Keyboard::Key::Left); Globales::CambiarCadena(1,"Left"); break;}
		case -10: {Globales::CambiarTecla(1,Keyboard::Key::Right); Globales::CambiarCadena(1,"Right"); break;}
		case -9: {Globales::CambiarTecla(1,Keyboard::Key::Up); Globales::CambiarCadena(1,"Up"); break;}
		case -8: {Globales::CambiarTecla(1,Keyboard::Key::Down); Globales::CambiarCadena(1,"Down"); break;}
		case -7: 
		case 32: {Globales::CambiarTecla(1,Keyboard::Key::Space); Globales::CambiarCadena(1,"Space"); break;}
		case -6: {Globales::CambiarTecla(1,Keyboard::Key::LControl); Globales::CambiarCadena(1,"LControl"); break;}
		case -5: {Globales::CambiarTecla(1,Keyboard::Key::LShift); Globales::CambiarCadena(1,"LShift"); break;}
		case -4: {Globales::CambiarTecla(1,Keyboard::Key::LAlt); Globales::CambiarCadena(1,"LAlt"); break;}
		case -3: {Globales::CambiarTecla(1,Keyboard::Key::RControl); Globales::CambiarCadena(1,"RControl"); break;}
		case -2: {Globales::CambiarTecla(1,Keyboard::Key::RShift); Globales::CambiarCadena(1,"RShift"); break;}
		case -1: {Globales::CambiarTecla(1,Keyboard::Key::RAlt); Globales::CambiarCadena(1,"RAlt"); break;}
		}
		break;
	}
	case 2:{ //Se guarda la tecla y el nombre para mover la forma hacia la abajo.
		switch(valor){
		case -37: 
		case 81:
		case 113: {Globales::CambiarTecla(2,Keyboard::Key::Q); Globales::CambiarCadena(2,"Q"); break;}
		case -36: 
		case 87:
		case 119: {Globales::CambiarTecla(2,Keyboard::Key::W); Globales::CambiarCadena(2,"W"); break;}
		case -35: 
		case 69:
		case 101: {Globales::CambiarTecla(2,Keyboard::Key::E); Globales::CambiarCadena(2,"E"); break;}
		case -34: 
		case 82:
		case 114: {Globales::CambiarTecla(2,Keyboard::Key::R); Globales::CambiarCadena(2,"R"); break;}
		case -33: 
		case 84: 
		case 116: {Globales::CambiarTecla(2,Keyboard::Key::T); Globales::CambiarCadena(2,"T"); break;}
		case -32: 
		case 89:
		case 121: {Globales::CambiarTecla(2,Keyboard::Key::Y); Globales::CambiarCadena(2,"Y"); break;}
		case -31: 
		case 85:
		case 117: {Globales::CambiarTecla(2,Keyboard::Key::U); Globales::CambiarCadena(2,"U"); break;}
		case -30: 
		case 73:
		case 105: {Globales::CambiarTecla(2,Keyboard::Key::I); Globales::CambiarCadena(2,"I"); break;}
		case -29: 
		case 79:
		case 111: {Globales::CambiarTecla(2,Keyboard::Key::O); Globales::CambiarCadena(2,"O"); break;}
		case -28: 
		case 80:
		case 112: {Globales::CambiarTecla(2,Keyboard::Key::P); Globales::CambiarCadena(2,"P"); break;}
		case -27: 
		case 65:
		case 97: {Globales::CambiarTecla(2,Keyboard::Key::A); Globales::CambiarCadena(2,"A"); break;}
		case -26:
		case 83:
		case 115: {Globales::CambiarTecla(2,Keyboard::Key::S); Globales::CambiarCadena(2,"S"); break;}
		case -25: 
		case 68:
		case 100: {Globales::CambiarTecla(2,Keyboard::Key::D); Globales::CambiarCadena(2,"D"); break;}
		case -24: 
		case 70:
		case 102: {Globales::CambiarTecla(2,Keyboard::Key::F); Globales::CambiarCadena(2,"F"); break;}
		case -23: 
		case 71:
		case 103: {Globales::CambiarTecla(2,Keyboard::Key::G); Globales::CambiarCadena(2,"G"); break;}
		case -22: 
		case 72:
		case 104: {Globales::CambiarTecla(2,Keyboard::Key::H); Globales::CambiarCadena(2,"H"); break;}
		case -21: 
		case 74:
		case 106: {Globales::CambiarTecla(2,Keyboard::Key::J); Globales::CambiarCadena(2,"J"); break;}
		case -20: 
		case 75:
		case 107: {Globales::CambiarTecla(2,Keyboard::Key::K); Globales::CambiarCadena(2,"K"); break;}
		case -19: 
		case 76:
		case 108: {Globales::CambiarTecla(2,Keyboard::Key::L); Globales::CambiarCadena(2,"L"); break;}
		case -18: 
		case 90:
		case 122: {Globales::CambiarTecla(2,Keyboard::Key::Z); Globales::CambiarCadena(2,"Z"); break;}
		case -17: 
		case 88:
		case 120: {Globales::CambiarTecla(2,Keyboard::Key::X); Globales::CambiarCadena(2,"X"); break;}
		case -16: 
		case 67: 
		case 99: {Globales::CambiarTecla(2,Keyboard::Key::C); Globales::CambiarCadena(2,"C"); break;}
		case -15: 
		case 86: 
		case 118: {Globales::CambiarTecla(2,Keyboard::Key::V); Globales::CambiarCadena(2,"V"); break;}
		case -14: 
		case 66:
		case 98: {Globales::CambiarTecla(2,Keyboard::Key::B); Globales::CambiarCadena(2,"B"); break;}
		case -13: 
		case 78:
		case 110: {Globales::CambiarTecla(2,Keyboard::Key::N); Globales::CambiarCadena(2,"N"); break;}
		case -12: 
		case 77: 
		case 109: {Globales::CambiarTecla(2,Keyboard::Key::M); Globales::CambiarCadena(2,"M"); break;}
		case -11: {Globales::CambiarTecla(2,Keyboard::Key::Left); Globales::CambiarCadena(2,"Left"); break;}
		case -10: {Globales::CambiarTecla(2,Keyboard::Key::Right); Globales::CambiarCadena(2,"Right"); break;}
		case -9: {Globales::CambiarTecla(2,Keyboard::Key::Up); Globales::CambiarCadena(2,"Up"); break;}
		case -8: {Globales::CambiarTecla(2,Keyboard::Key::Down); Globales::CambiarCadena(2,"Down"); break;}
		case -7: 
		case 32: {Globales::CambiarTecla(2,Keyboard::Key::Space); Globales::CambiarCadena(2,"Space"); break;}
		case -6: {Globales::CambiarTecla(2,Keyboard::Key::LControl); Globales::CambiarCadena(2,"LControl"); break;}
		case -5: {Globales::CambiarTecla(2,Keyboard::Key::LShift); Globales::CambiarCadena(2,"LShift"); break;}
		case -4: {Globales::CambiarTecla(2,Keyboard::Key::LAlt); Globales::CambiarCadena(2,"LAlt"); break;}
		case -3: {Globales::CambiarTecla(2,Keyboard::Key::RControl); Globales::CambiarCadena(2,"RControl"); break;}
		case -2: {Globales::CambiarTecla(2,Keyboard::Key::RShift); Globales::CambiarCadena(2,"RShift"); break;}
		case -1: {Globales::CambiarTecla(2,Keyboard::Key::RAlt); Globales::CambiarCadena(2,"RAlt"); break;}
		}
		break;
	}
	case 3:{ //Se guarda la tecla y el nombre para rotar la forma.
		switch(valor){
		case -37: 
		case 81:
		case 113: {Globales::CambiarTecla(3,Keyboard::Key::Q); Globales::CambiarCadena(3,"Q"); break;}
		case -36: 
		case 87:
		case 119: {Globales::CambiarTecla(3,Keyboard::Key::W); Globales::CambiarCadena(3,"W"); break;}
		case -35: 
		case 69:
		case 101: {Globales::CambiarTecla(3,Keyboard::Key::E); Globales::CambiarCadena(3,"E"); break;}
		case -34: 
		case 82:
		case 114: {Globales::CambiarTecla(3,Keyboard::Key::R); Globales::CambiarCadena(3,"R"); break;}
		case -33: 
		case 84: 
		case 116: {Globales::CambiarTecla(3,Keyboard::Key::T); Globales::CambiarCadena(3,"T"); break;}
		case -32: 
		case 89:
		case 121: {Globales::CambiarTecla(3,Keyboard::Key::Y); Globales::CambiarCadena(3,"Y"); break;}
		case -31: 
		case 85:
		case 117: {Globales::CambiarTecla(3,Keyboard::Key::U); Globales::CambiarCadena(3,"U"); break;}
		case -30: 
		case 73:
		case 105: {Globales::CambiarTecla(3,Keyboard::Key::I); Globales::CambiarCadena(3,"I"); break;}
		case -29: 
		case 79:
		case 111: {Globales::CambiarTecla(3,Keyboard::Key::O); Globales::CambiarCadena(3,"O"); break;}
		case -28: 
		case 80:
		case 112: {Globales::CambiarTecla(3,Keyboard::Key::P); Globales::CambiarCadena(3,"P"); break;}
		case -27: 
		case 65:
		case 97: {Globales::CambiarTecla(3,Keyboard::Key::A); Globales::CambiarCadena(3,"A"); break;}
		case -26:
		case 83:
		case 115: {Globales::CambiarTecla(3,Keyboard::Key::S); Globales::CambiarCadena(3,"S"); break;}
		case -25: 
		case 68:
		case 100: {Globales::CambiarTecla(3,Keyboard::Key::D); Globales::CambiarCadena(3,"D"); break;}
		case -24: 
		case 70:
		case 102: {Globales::CambiarTecla(3,Keyboard::Key::F); Globales::CambiarCadena(3,"F"); break;}
		case -23: 
		case 71:
		case 103: {Globales::CambiarTecla(3,Keyboard::Key::G); Globales::CambiarCadena(3,"G"); break;}
		case -22: 
		case 72:
		case 104: {Globales::CambiarTecla(3,Keyboard::Key::H); Globales::CambiarCadena(3,"H"); break;}
		case -21: 
		case 74:
		case 106: {Globales::CambiarTecla(3,Keyboard::Key::J); Globales::CambiarCadena(3,"J"); break;}
		case -20: 
		case 75:
		case 107: {Globales::CambiarTecla(3,Keyboard::Key::K); Globales::CambiarCadena(3,"K"); break;}
		case -19: 
		case 76:
		case 108: {Globales::CambiarTecla(3,Keyboard::Key::L); Globales::CambiarCadena(3,"L"); break;}
		case -18: 
		case 90:
		case 122: {Globales::CambiarTecla(3,Keyboard::Key::Z); Globales::CambiarCadena(3,"Z"); break;}
		case -17: 
		case 88:
		case 120: {Globales::CambiarTecla(3,Keyboard::Key::X); Globales::CambiarCadena(3,"X"); break;}
		case -16: 
		case 67: 
		case 99: {Globales::CambiarTecla(3,Keyboard::Key::C); Globales::CambiarCadena(3,"C"); break;}
		case -15: 
		case 86: 
		case 118: {Globales::CambiarTecla(3,Keyboard::Key::V); Globales::CambiarCadena(3,"V"); break;}
		case -14: 
		case 66:
		case 98: {Globales::CambiarTecla(3,Keyboard::Key::B); Globales::CambiarCadena(3,"B"); break;}
		case -13: 
		case 78:
		case 110: {Globales::CambiarTecla(3,Keyboard::Key::N); Globales::CambiarCadena(3,"N"); break;}
		case -12: 
		case 77: 
		case 109: {Globales::CambiarTecla(3,Keyboard::Key::M); Globales::CambiarCadena(3,"M"); break;}
		case -11: {Globales::CambiarTecla(3,Keyboard::Key::Left); Globales::CambiarCadena(3,"Left"); break;}
		case -10: {Globales::CambiarTecla(3,Keyboard::Key::Right); Globales::CambiarCadena(3,"Right"); break;}
		case -9: {Globales::CambiarTecla(3,Keyboard::Key::Up); Globales::CambiarCadena(3,"Up"); break;}
		case -8: {Globales::CambiarTecla(3,Keyboard::Key::Down); Globales::CambiarCadena(3,"Down"); break;}
		case -7: 
		case 32: {Globales::CambiarTecla(3,Keyboard::Key::Space); Globales::CambiarCadena(3,"Space"); break;}
		case -6: {Globales::CambiarTecla(3,Keyboard::Key::LControl); Globales::CambiarCadena(3,"LControl"); break;}
		case -5: {Globales::CambiarTecla(3,Keyboard::Key::LShift); Globales::CambiarCadena(3,"LShift"); break;}
		case -4: {Globales::CambiarTecla(3,Keyboard::Key::LAlt); Globales::CambiarCadena(3,"LAlt"); break;}
		case -3: {Globales::CambiarTecla(3,Keyboard::Key::RControl); Globales::CambiarCadena(3,"RControl"); break;}
		case -2: {Globales::CambiarTecla(3,Keyboard::Key::RShift); Globales::CambiarCadena(3,"RShift"); break;}
		case -1: {Globales::CambiarTecla(3,Keyboard::Key::RAlt); Globales::CambiarCadena(3,"RAlt"); break;}
		}
		break;	
	}
	case 4:{ //Se guarda la tecla y el nombre para realizar un HardDrop.
		switch(valor){
		case -37: 
		case 81:
		case 113: {Globales::CambiarTecla(4,Keyboard::Key::Q); Globales::CambiarCadena(4,"Q"); break;}
		case -36: 
		case 87:
		case 119: {Globales::CambiarTecla(4,Keyboard::Key::W); Globales::CambiarCadena(4,"W"); break;}
		case -35: 
		case 69:
		case 101: {Globales::CambiarTecla(4,Keyboard::Key::E); Globales::CambiarCadena(4,"E"); break;}
		case -34: 
		case 82:
		case 114: {Globales::CambiarTecla(4,Keyboard::Key::R); Globales::CambiarCadena(4,"R"); break;}
		case -33: 
		case 84: 
		case 116: {Globales::CambiarTecla(4,Keyboard::Key::T); Globales::CambiarCadena(4,"T"); break;}
		case -32: 
		case 89:
		case 121: {Globales::CambiarTecla(4,Keyboard::Key::Y); Globales::CambiarCadena(4,"Y"); break;}
		case -31: 
		case 85:
		case 117: {Globales::CambiarTecla(4,Keyboard::Key::U); Globales::CambiarCadena(4,"U"); break;}
		case -30: 
		case 73:
		case 105: {Globales::CambiarTecla(4,Keyboard::Key::I); Globales::CambiarCadena(4,"I"); break;}
		case -29: 
		case 79:
		case 111: {Globales::CambiarTecla(4,Keyboard::Key::O); Globales::CambiarCadena(4,"O"); break;}
		case -28: 
		case 80:
		case 112: {Globales::CambiarTecla(4,Keyboard::Key::P); Globales::CambiarCadena(4,"P"); break;}
		case -27: 
		case 65:
		case 97: {Globales::CambiarTecla(4,Keyboard::Key::A); Globales::CambiarCadena(4,"A"); break;}
		case -26:
		case 83:
		case 115: {Globales::CambiarTecla(4,Keyboard::Key::S); Globales::CambiarCadena(4,"S"); break;}
		case -25: 
		case 68:
		case 100: {Globales::CambiarTecla(4,Keyboard::Key::D); Globales::CambiarCadena(4,"D"); break;}
		case -24: 
		case 70:
		case 102: {Globales::CambiarTecla(4,Keyboard::Key::F); Globales::CambiarCadena(4,"F"); break;}
		case -23: 
		case 71:
		case 103: {Globales::CambiarTecla(4,Keyboard::Key::G); Globales::CambiarCadena(4,"G"); break;}
		case -22: 
		case 72:
		case 104: {Globales::CambiarTecla(4,Keyboard::Key::H); Globales::CambiarCadena(4,"H"); break;}
		case -21: 
		case 74:
		case 106: {Globales::CambiarTecla(4,Keyboard::Key::J); Globales::CambiarCadena(4,"J"); break;}
		case -20: 
		case 75:
		case 107: {Globales::CambiarTecla(4,Keyboard::Key::K); Globales::CambiarCadena(4,"K"); break;}
		case -19: 
		case 76:
		case 108: {Globales::CambiarTecla(4,Keyboard::Key::L); Globales::CambiarCadena(4,"L"); break;}
		case -18: 
		case 90:
		case 122: {Globales::CambiarTecla(4,Keyboard::Key::Z); Globales::CambiarCadena(4,"Z"); break;}
		case -17: 
		case 88:
		case 120: {Globales::CambiarTecla(4,Keyboard::Key::X); Globales::CambiarCadena(4,"X"); break;}
		case -16: 
		case 67: 
		case 99: {Globales::CambiarTecla(4,Keyboard::Key::C); Globales::CambiarCadena(4,"C"); break;}
		case -15: 
		case 86: 
		case 118: {Globales::CambiarTecla(4,Keyboard::Key::V); Globales::CambiarCadena(4,"V"); break;}
		case -14: 
		case 66:
		case 98: {Globales::CambiarTecla(4,Keyboard::Key::B); Globales::CambiarCadena(4,"B"); break;}
		case -13: 
		case 78:
		case 110: {Globales::CambiarTecla(4,Keyboard::Key::N); Globales::CambiarCadena(4,"N"); break;}
		case -12: 
		case 77: 
		case 109: {Globales::CambiarTecla(4,Keyboard::Key::M); Globales::CambiarCadena(4,"M"); break;}
		case -11: {Globales::CambiarTecla(4,Keyboard::Key::Left); Globales::CambiarCadena(4,"Left"); break;}
		case -10: {Globales::CambiarTecla(4,Keyboard::Key::Right); Globales::CambiarCadena(4,"Right"); break;}
		case -9: {Globales::CambiarTecla(4,Keyboard::Key::Up); Globales::CambiarCadena(4,"Up"); break;}
		case -8: {Globales::CambiarTecla(4,Keyboard::Key::Down); Globales::CambiarCadena(4,"Down"); break;}
		case -7: 
		case 32: {Globales::CambiarTecla(4,Keyboard::Key::Space); Globales::CambiarCadena(4,"Space"); break;}
		case -6: {Globales::CambiarTecla(4,Keyboard::Key::LControl); Globales::CambiarCadena(4,"LControl"); break;}
		case -5: {Globales::CambiarTecla(4,Keyboard::Key::LShift); Globales::CambiarCadena(4,"LShift"); break;}
		case -4: {Globales::CambiarTecla(4,Keyboard::Key::LAlt); Globales::CambiarCadena(4,"LAlt"); break;}
		case -3: {Globales::CambiarTecla(4,Keyboard::Key::RControl); Globales::CambiarCadena(4,"RControl"); break;}
		case -2: {Globales::CambiarTecla(4,Keyboard::Key::RShift); Globales::CambiarCadena(4,"RShift"); break;}
		case -1: {Globales::CambiarTecla(4,Keyboard::Key::RAlt); Globales::CambiarCadena(4,"RAlt"); break;}
		}
		break;
	}
	default:{ //Se guarda la tecla y el nombre para poner en pausa el juego.
		switch(valor){ 
		case -37: 
		case 81:
		case 113: {Globales::CambiarTecla(5,Keyboard::Key::Q); Globales::CambiarCadena(5,"Q"); break;}
		case -36: 
		case 87:
		case 119: {Globales::CambiarTecla(5,Keyboard::Key::W); Globales::CambiarCadena(5,"W"); break;}
		case -35: 
		case 69:
		case 101: {Globales::CambiarTecla(5,Keyboard::Key::E); Globales::CambiarCadena(5,"E"); break;}
		case -34: 
		case 82:
		case 114: {Globales::CambiarTecla(5,Keyboard::Key::R); Globales::CambiarCadena(5,"R"); break;}
		case -33: 
		case 84: 
		case 116: {Globales::CambiarTecla(5,Keyboard::Key::T); Globales::CambiarCadena(5,"T"); break;}
		case -32: 
		case 89:
		case 121: {Globales::CambiarTecla(5,Keyboard::Key::Y); Globales::CambiarCadena(5,"Y"); break;}
		case -31: 
		case 85:
		case 117: {Globales::CambiarTecla(5,Keyboard::Key::U); Globales::CambiarCadena(5,"U"); break;}
		case -30: 
		case 73:
		case 105: {Globales::CambiarTecla(5,Keyboard::Key::I); Globales::CambiarCadena(5,"I"); break;}
		case -29: 
		case 79:
		case 111: {Globales::CambiarTecla(5,Keyboard::Key::O); Globales::CambiarCadena(5,"O"); break;}
		case -28: 
		case 80:
		case 112: {Globales::CambiarTecla(5,Keyboard::Key::P); Globales::CambiarCadena(5,"P"); break;}
		case -27: 
		case 65:
		case 97: {Globales::CambiarTecla(5,Keyboard::Key::A); Globales::CambiarCadena(5,"A"); break;}
		case -26:
		case 83:
		case 115: {Globales::CambiarTecla(5,Keyboard::Key::S); Globales::CambiarCadena(5,"S"); break;}
		case -25: 
		case 68:
		case 100: {Globales::CambiarTecla(5,Keyboard::Key::D); Globales::CambiarCadena(5,"D"); break;}
		case -24: 
		case 70:
		case 102: {Globales::CambiarTecla(5,Keyboard::Key::F); Globales::CambiarCadena(5,"F"); break;}
		case -23: 
		case 71:
		case 103: {Globales::CambiarTecla(5,Keyboard::Key::G); Globales::CambiarCadena(5,"G"); break;}
		case -22: 
		case 72:
		case 104: {Globales::CambiarTecla(5,Keyboard::Key::H); Globales::CambiarCadena(5,"H"); break;}
		case -21: 
		case 74:
		case 106: {Globales::CambiarTecla(5,Keyboard::Key::J); Globales::CambiarCadena(5,"J"); break;}
		case -20: 
		case 75:
		case 107: {Globales::CambiarTecla(5,Keyboard::Key::K); Globales::CambiarCadena(5,"K"); break;}
		case -19: 
		case 76:
		case 108: {Globales::CambiarTecla(5,Keyboard::Key::L); Globales::CambiarCadena(5,"L"); break;}
		case -18: 
		case 90:
		case 122: {Globales::CambiarTecla(5,Keyboard::Key::Z); Globales::CambiarCadena(5,"Z"); break;}
		case -17: 
		case 88:
		case 120: {Globales::CambiarTecla(5,Keyboard::Key::X); Globales::CambiarCadena(5,"X"); break;}
		case -16: 
		case 67: 
		case 99: {Globales::CambiarTecla(5,Keyboard::Key::C); Globales::CambiarCadena(5,"C"); break;}
		case -15: 
		case 86: 
		case 118: {Globales::CambiarTecla(5,Keyboard::Key::V); Globales::CambiarCadena(5,"V"); break;}
		case -14: 
		case 66:
		case 98: {Globales::CambiarTecla(5,Keyboard::Key::B); Globales::CambiarCadena(5,"B"); break;}
		case -13: 
		case 78:
		case 110: {Globales::CambiarTecla(5,Keyboard::Key::N); Globales::CambiarCadena(5,"N"); break;}
		case -12: 
		case 77: 
		case 109: {Globales::CambiarTecla(5,Keyboard::Key::M); Globales::CambiarCadena(5,"M"); break;}
		case -11: {Globales::CambiarTecla(5,Keyboard::Key::Left); Globales::CambiarCadena(5,"Left"); break;}
		case -10: {Globales::CambiarTecla(5,Keyboard::Key::Right); Globales::CambiarCadena(5,"Right"); break;}
		case -9: {Globales::CambiarTecla(5,Keyboard::Key::Up); Globales::CambiarCadena(5,"Up"); break;}
		case -8: {Globales::CambiarTecla(5,Keyboard::Key::Down); Globales::CambiarCadena(5,"Down"); break;}
		case -7: 
		case 32: {Globales::CambiarTecla(5,Keyboard::Key::Space); Globales::CambiarCadena(5,"Space"); break;}
		case -6: {Globales::CambiarTecla(5,Keyboard::Key::LControl); Globales::CambiarCadena(5,"LControl"); break;}
		case -5: {Globales::CambiarTecla(5,Keyboard::Key::LShift); Globales::CambiarCadena(5,"LShift"); break;}
		case -4: {Globales::CambiarTecla(5,Keyboard::Key::LAlt); Globales::CambiarCadena(5,"LAlt"); break;}
		case -3: {Globales::CambiarTecla(5,Keyboard::Key::RControl); Globales::CambiarCadena(5,"RControl"); break;}
		case -2: {Globales::CambiarTecla(5,Keyboard::Key::RShift); Globales::CambiarCadena(5,"RShift"); break;}
		case -1: {Globales::CambiarTecla(5,Keyboard::Key::RAlt); Globales::CambiarCadena(5,"RAlt"); break;}
		}
		break;
	}
	}
}

#endif
