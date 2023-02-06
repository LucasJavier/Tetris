#include "Globales.h"

/* Lo que representa cada indice de los vectores 'vectorEstado' (tipo bool), 'vectorCadenas' (tipo string), y 'vectorTeclas' (tipo Key) es lo siguiente:
	0) bool: si es la primera vez que se entra al juego, o si se vuelve a la clase 'Menu' desde el la ventana de Game Over - string: nombre de la tecla para mover la forma a la izquierda - Key: tecla para mover la forma hacia la izquierda.
	1) bool: si debe terminar o no el juego (Game Over) - string: nombre de la tecla para mover la forma a la derecha - Key: tecla para mover la forma hacia la derecha.
	2) bool: si debe crear o no una forma en la matriz - string: nombre de la tecla para mover la forma hacia abajo - Key: tecla para mover la forma hacia abajo.
	3) bool: si el usuario se encuentra en la ventana de la clase 'Opciones' - string: nombre de la tecla para rotar la forma - Key: tecla para rotar la forma.
	4) bool: si el usuario cliqueo o no para modificar una tecla - string: nombre de la tecla para realizar un HardDrop - Key: tecla para realizar un HardDrop.
	5) bool: si se debe o no realizar la modificacion de una tecla - string: nombre de la tecla para poner pausa. - Key: tecla para poner pausa.
	6) bool: si el usuario se encuentra en la ventana de la clase 'GameOver'. string: el nombre/alias escrito por el usuario en la ventana de la clase 'GameOver'.
	7) bool: que combinacion de colores se debe cambiar - string: nombre del tipo de color actual.
	8) bool: si debe o no reproducir un efecto de sonido - string: nombre de la dificultad actual del juego.
	9) bool: si la musica esta activada o no. string: muestra si las filas piedras estan activas o no.
	10) bool: si los efectos estan activados o no.
	11) bool: si el juego se encuentra en estado de play o pausa.
	12) bool: si el juego fue despausado.
	13) bool: si se movio la pieza.
	14) bool: si el agregado al juego de filas piedras esta activado o no.
*/

vector<bool> Globales::vectorEstados; vector<string> Globales::vectorCadenas; vector<Color> Globales::vectorColores;
int Globales::dificultad; int Globales::opcionLetra; Color Globales::fondo; Text Globales::nombreUsuario;
vector<Keyboard::Key> Globales::vectorTeclas; //Declaracion de los distintas variables static de la clase.

//En su constructor se setea los distintos vectores de la clase.
Globales::Globales() {
	vectorEstados.resize(15); vectorCadenas.resize(10); vectorColores.resize(8); vectorTeclas.reserve(6);
}

//Este metodo devuelve el estado de la variable de tipo 'bool' contenida en el vector.
bool Globales::VerEstado(int i){
	return vectorEstados[i];
}

//Este metodo cambia el estado de la variable de tipo 'bool' contenida en el vector.
void Globales::CambiarEstado(int i,bool estado){
	vectorEstados[i] = estado;
}

//Este metodo permite ver la cadena de texto contenida en el vector.
string Globales::VerCadena(int i){
	return vectorCadenas[i];
}

//Este metodo permite cambiar la cadena de texto contenida en el vector.
void Globales::CambiarCadena(int i,string cadena){
	vectorCadenas[i] = cadena;
}

//Este metodo permite ver la dificultad del juego.
int Globales::VerDificultad(){
	return dificultad;
}

//Este metodo permite cambiar la dificultad del juego.
void Globales::CambiarDificultad(int numDificultad){
	dificultad = numDificultad;
}

//Este metodo permite ver que letra ha sido tecleada.
int Globales::VerOpcionLetra(){
	return opcionLetra;
}

//Este metodo permite cambiar la letra por la nueva tecleada.
void Globales::CambiarOpcionLetra(int unicode){
	opcionLetra = unicode;
}

//Este metodo permite ver el color del fondo de la ventana.
Color Globales::VerColor(){
	return fondo;
}

//Este metodo permite ver el color almacenado en el vector.
Color Globales::VerColor(int i){
	return vectorColores[i];
}

//Este metodo permite cambiar el color de fondo de la ventana.
void Globales::CambiarColor(Color color){
	fondo = color;
}

//Este metodo permite cambiar el color almacenado en el vector.
void Globales::CambiarColor(int i,Color color){
	vectorColores[i] = color;
}

//Este metodo permite ver el contenido almacenado en el texto 'nombreUsuario'.
Text& Globales::VerNombreUsuario(){
	return nombreUsuario;
}

//Este metodo permite cambiar el contenido almacenado en el texto 'nombreUsuario'.
void Globales::CambiarNombreUsuario(string cadena){
	nombreUsuario.setString(cadena);
}

//Este metodo permite ver las Key-tecla asociada a una accion.
Keyboard::Key Globales::VerTecla(int i){
	return vectorTeclas[i];
}

//Este metodo permite cambiar la Key-tecla asociada a una accion.
void Globales::CambiarTecla(int i,Keyboard::Key tecla){
	vectorTeclas[i] = tecla;
}
