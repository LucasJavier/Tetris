/*
Esta clase es la principal del proyecto, ya que de esta corre lo que es el juego en si. Es la encargada de mostrar en pantalla
la matriz caracteristica del tetris e ir creando y mostrando, y modificando las distintas formas. Asimismo, muestra distintos 
textos informativos para el usuario.
*/
#ifndef PARTIDA_H
#define PARTIDA_H
#include "Juego.h"
#include "Escenas.h"
#include "Formas.h"
#include "Jugador.h"
#include "Cuadrado.h"
#include "EleIzq.h"
#include "EleDer.h"
#include "Linea.h"
#include "EseIzq.h"
#include "EseDer.h"
#include "Te.h"
#include "GameOver.h"
#include "Globales.h"
#include "Archivo.h"
#include "Fuentes.h"
#include <sstream>
using namespace std;
using namespace sf;
class Juego;

class Partida : public Escenas {
public:
	Partida(RenderWindow &ventana);
	void Actualizar(Juego &juego,RenderWindow &ventana) override;
	void Dibujar(RenderWindow &ventana) override;
	~Partida();
private:
	vector<vector<int>> matrizCeldas, matrizFormaSiguiente; /*
		matrizCeldas: es la matriz encargada de:
			1) Mostras las formas creadas.
			2) Mostrar las formas consolidadas.
			3) Mostrar los movimientos de las formas, hechas por el usuario.
			4) Mostrar la sombra de la forma creada.
			5) Mostrar los efectos de las filas a ser borradas.
		matrizFormaSiguiente: es la encargada de mostrar la forma que vendra a continuacion de ultima creada.
	*/
	vector<RectangleShape> celdasFormas; /*Vector de RectangleShape donde cada uno tiene un color especifico. Es el usado para
										dibujar las celdas de la matriz 'matrizCeldas'.*/
	vector<Text> vectorTexto; //Vector de los textos mostrados en pantalla.
	vector<int> vectorJotas,vectorFilas,guardarFila; /*
		vectorJotas: vector que almacena las filas que hayan sido llenadas.
		vectorFilas: vector que almacena las filas que contienen formas consolidadas.
		guardarFila: vector que almacena las filas que contienen formas consolidadas. La diferencia con la anterior, es que se lo
		utiliza para invertir el orden de los datos.
	*/
	vector<Time> vectorTiempo; //Vector de objeto tipo 'Time' para ir almacenando los tiempos de las filas con formas consolidadas.
	vector<bool> vectorEstadoSonido; //Vectos de tipo 'bool' para almacenar el estado de la carga de un archivo musical.
	Vector2i posicionMouse; //Vector2i que guarda la posicion x-y del mouse.
	bool caidaRapida,cambioMusica,borrarLinea,esGameOver,puedeEntrar,esperar,m_si,m_no,cambioTiempo,
	primeraVez,primeraVezPausa,puedeFestejar;
	/*
		caidaRapida: estado que chequea si el usuario ha apretado o no la tecla correspondiente al HardDrop.
		cambioMusica: estado que corrobora a que archivo musical ha de cambiar la reproduccion.
		borrarLinea: estado que corrobora si se ha de 'limpiar' una fila.
		cargoMusicaUno: estado que corrobora si cargo el archivo musical 'Original Tetris theme.wav'.
		cargoMusicaDos: estado que corrobora si cargo el archivo musical 'Bradinsky.flac'
		esGameOver: estado que corrobora si el juego debe o no terminar.
		puedeEntrar: estado que corrobora si se debe guardar la fila con filas consolidadas.
		cargoFuente: estado que corrobora si se ha cargado la fuente 'Opciones.ttf'.
		esperar: estado que corroborar si el usuario ha apretado o no la tecla 'Escape', pausando el juego.
		m_si: estado que corrobora si el usuario ha cliqueado en la opcion 'SI'.
		m_no: estado que corrobora si el usuario ha cliqueado en la opcion 'NO'.
		cambioTiempo: estado que corrobora si ha cambio el tiempo de caida de las formas.
		primeraVez: estado que corrobora si es la primera vez que se llamo a la funcion 'GuardarTiempos' o si ha reducido
		el vector 'vectorFilas' a cero '0'.
		primeraVezPausa: estado que corrobora si se ha puesto pausa, de manera de realizar las acciones correspondientes y, luego, 
		cambiar su estado para dejar de hacerlas.
		puedeFestejar: estado que corrobora si ya se ha reproducido el archivo musical 'SpeeUp.wav'.
	*/
	int contadorLineas,score,repetir,n_forma,valorForma,lineaLlena,tiempoDeCaida,mejorPuntaje,cantMusica,
	hora,min,seg,mili,numFilas,tiempoPiedra;
	/*
		contadorLineas: contador de las cantidad de filas despejadas por el usuario.
		score: contador del puntaje del usuario.
		repetir: numero indicativo de la cantidad de veces que debe realizarle el dibujado de la matriz 'matrizCeldas'.
		n_forma: numero que indica que forma debe crearse. A su vez, que forma viene despues.
		valorForma: numero que guarda que forma ha sido creada.
		lineaLlena: contador de celdas con contenido numerico mayor a uno '1' en la matriz 'matrizCeldas'.
		tiempoDeCaida: numero que representa el tiempo de caida de las formas.
		mejorPuntaje: numero que indica el mejor puntaje hecho.
		cantMusica: numero que indica la cantidad de objetos tipo Music que deben ser creados/eliminados.
		hora: cantidad de horas transcurridas en el juego.
		min: cantidad de minutos transcurridos en el juego.
		seg: cantidad de segundos transcurridos en el juego.
		mil: cantidad de milisegundos transcurridos en el juego.
		numFilas: el numero de filas que contienen formas consolidadas.
		tiempoPiedra: el tiempo que tiene que superar una fila con formas consolidadas.
	*/
	string m_puntaje,m_lineas,m_mejorPuntaje,tiempoCadena;
	/*
		m_puntaje: el puntaje del usuario, convertido a cadena de texto.
		m_lineas: la cantidad de filas despejadas por el usuario, convertido a cadena de texto.
		m_mejorPuntaje: el mejor puntaje hecho hasta el momento, convertido a cadena de texto.
		tiempoCadena: el tiempo que ha transcurrido hasta el momento, convertido a cadena de texto.
	*/
	Music *punteroMusica; //Vector de punteros de tipo Music.
	RectangleShape ContenedorSiguienteForma,FondoSegundaVentana,Sombra,FondoTercerVentana,FondoPausa,FondoPreguntar;
	/*
		ContenedorSiguienteForma: RectangleShape donde se dibujaran las celdas vacias y la forma del objeto que se creara a 
		continuacion.
		FondoSegundaVentana: RectangleShape donde se dibujaran los distintos textos encontrados a la derecha de la partida, y
		la forma que se creara a continuacion.
		FondoTercerVentana: RectangleShape donde se dibujaran los distintos textos encontrados a la izquierda de la partida, y 
		la imagen de Play/Pausa.
		FondoPausa: RectangleShape que contiene al texto indicativo de juego en estado de Pausa.
		FondoPreguntar: RectangleShape que contiene los textos relacionados a salir o no de la partida.
		Sombra: RectangleShape que muestra donde 'chocaria' la forma creada.
	*/
	Formas forma;  //Objeto de la clase 'Formas', utilizado para llamar a los metodos 'Actualizar', 'LimpiarSombra', y 'MuestraPieza'.
	Jugador jugador; //Objeto de la clase 'Jugador', utilizado para llamar a los metodos 'Actualizar' y 'TecleoPausa'.
	Archivo archivo; //Objeto de la clase 'Archivo', utilizado para llamar al metodo 'VerMejorPuntaje'.
	Clock reloj,relojCambioMusica,relojEsperar,relojMouse,relojTiempo,relojEscape,*relojFilas; 
	/*
		reloj: Clock que se utiliza para:
			1) Mientras la partida no haya terminado, para esperar a que el tiempo sea mayor al tiempo de caida establecido.
			2) Si la partida termino, para ir cambiando el color del texto mostrado.
		relojCambioMusica: Clock que se utiliza para, una vez superado el tiempo especificado, cambiar la musica reproducida.
		relojEsperar: Clock que se utiliza para hacer esperar al pricipio a la forma creada, de manera que no comience a bajar 
		inmediatamente.
		relojMouse: Clock que se utiliza para evitar muchos click izquierdos seguidos en un corto perioro de tiempo.
		relojTiempo: Clock utilizado para medir el tiempo transcurrido desde que se inicio la partida.
		relojEscape: Clock que se utiliza para evitar multiples entradas seguidas en corto perdiodo de tiempo al momento de apretar
		'Escape'.
		*relojFilas: Clock que almacena el tiempo de cada fila que contiene formas consolidadas.
	*/
	Time tiempo; //Objeto de la clase 'Time' para almacenar el tiempo transcurrido hasta el momento de ser llamado.
	Texture tPausa,tPlay; //Las imanes de Play y Pausa.
	Sprite sPausa,sPlay; //Para setear los aspectos de las imagens Play y Pausa.
	stringstream ss; //Para concatenar string con variables de tipo int, de manera de crear una unica variable de tipo string.
	Fuentes fuente; //Objeto de la clase 'Fuentes' utilizado para verificar y usar en los textos la fuente deaseada.
	Image icono; //Utilizado para cargar la imagen del icono de programa.
	void Generar(vector<vector<int>> &matrizCeldas);
	void VerificarLinea(vector<vector<int>> &matrizCeldas);
	void LimpiarLinea(vector<vector<int>> &matrizCeldas);
	void BajarCeldas(vector<vector<int>> &matrizCeldas);
	void MostrarSiguiente(int forma);
	void CrearRectangulos();
	void CrearTexto();
	void ModificarVariables(int n);
	bool ChequearGameOver(int n_forma);
	void AspectosTextoGenerales(Text &texto,Font &fuente,string titulo, bool esTitulo,int tamanio);
	void ModificarDificultad();
	void MostrarMejorPuntaje();
	void CambiarColorFormas();
	void EfectoLinea(vector<vector<int>> &matrizCeldas);
	void CambiarNumeros();
	void CargarMusica();
	bool CliqueoPausa(RenderWindow &ventana);
	void ObtenerTiempo();
	void MostrarVelocidad();
	void RespuestaUsuario(RenderWindow &ventana);
	void GuardarTiempos();
	void ChequearTiempoFilas();
	void CambiarNumeros(int j);
	void EliminarTiempos(int fila,int posicion,bool seEliminoFila);
};

#endif

