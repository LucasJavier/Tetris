/*
Esta clase es la encargada de almacenar y modificar variables que sean usadas por distintas clases durante el juego, con el objetivo de
no tener que hacer uso de tantos pasos de parametros en funciones.
*/
#ifndef GLOBALES_H
#define GLOBALES_H
#include <vector>
#include <string>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Keyboard.hpp>
using namespace std;
using namespace sf;

class Globales {
public:
	Globales();
	static bool VerEstado(int i);
	static void CambiarEstado(int i,bool estado);
	static string VerCadena(int i);
	static void CambiarCadena(int i,string cadena);
	static int VerDificultad();
	static void CambiarDificultad(int numDificultad);
	static int VerOpcionLetra();
	static void CambiarOpcionLetra(int unicode);
	static Color VerColor();
	static Color VerColor(int i);
	static void CambiarColor(Color color);
	static void CambiarColor(int i,Color color);
	static Text& VerNombreUsuario();
	static void CambiarNombreUsuario(string cadena);
	static Keyboard::Key VerTecla(int i);
	static void CambiarTecla(int i,Keyboard::Key tecla);
private:
	static vector<bool> vectorEstados; //vector de tipo 'bool' para cambiar y ver los distintos estados utilizados en el juego.
	static vector<string> vectorCadenas; //vector de tipo 'string' para cambiar y ver las distintas cadenas de texto utilizadas en el juego.
	static vector<Color> vectorColores; //vector de tipo Color para cambiar y ver los colores utilizados en el juego.
	static vector<Keyboard::Key> vectorTeclas; //vector de tipo Key para cambiar y ver las teclas usadas en el juego.
	static int dificultad,opcionLetra; 
	/*
		dificultad: para guardar la dificultad del juego.
		opcionLetra: parar guardar la tecla apretada.
	*/
	static Color fondo; //Para guardar el color que debe tener el fondo de la ventana.
	static Text nombreUsuario; //Para guardar el nombre/alis que el usuario teclee en la ventana 'GameOver'.
};

#endif

