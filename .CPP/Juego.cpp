#include "Juego.h"
#include "Menu.h"

void modificarTeclas(int valor,int numTecla); //Se realiza la declaracion del metodo global 'modificarTeclas'.

/*
En su constructor, creamos un objeto de la clase 'Globales' para setear los vectores de esa misma clase.
Luego se llama a las funciònes 'valoresTeclas()' y 'SetearControles()', explicadadas mas debajo. Por ultimo,
se setea el puntero 'escenaActual' para que apunte a la clase 'Menu'.
*/
Juego::Juego(){
	Globales global; valoresTeclas(); SetearControles();
	Globales::CambiarEstado(0,true); 
	escenaActual = new Menu(ventana);
}

//Funcion que se encarga de setear la nueva escena-clase a la cual se debara cambiar, modificando asi la ventana y sus caracteristicas.
void Juego::SetiarEscena(Escenas *escena){ 
	escenaSiguiente = escena;
}

//Destructor de elimina los punteros de la clase 'Juego'
Juego::~Juego(){
	delete escenaActual;
	delete escenaSiguiente;
}

/*
Esta funcion es la encargada de capturar los eventos que ocurran en la ventana, sobre todo en la ventana de la clase
'GameOver' y 'Opciones'. GameOver' capturara, si se ha superado algun puntaje, las teclas que apriete el usuario para 
introducir su nombre, de manera de guardarlo en un archivo. 'Opciones' captura que tecla apreto el usuario para modificar 
los parametros del juego, ya sean los botones para mover las formas, o con que poner pausa.
*/
void Juego::ProcesarEvento(){
	Event e;
	while(ventana.pollEvent(e)) { //Capturamos los eventos que vayan ocurriendo en la ventana.
		if(e.type == Event::Closed){ //Si se cliquea sobre el signo 'x' en la parte superio derecha, se cierra la ventana.
			ventana.close();
		}
		if(Globales::VerEstado(6)){ //Condicional para la ventana-clase 'GameOver'.
			if(e.type == Event::TextEntered && reloj.getElapsedTime().asMilliseconds()>50){ /*Condicional para verificar que el 
																							evento es de tipo entrada de texto.*/
				if(e.text.unicode<256){ //Verificamos que la tecla apretada este dentro de los caracteres legibles.
					/*Si la tecla apretada por el usuario es el 'Backspace', y si la cadena tiene por lo menos un
					caracter almacenado, borramos un caracter de la cadena.*/
					if(e.text.unicode==8 && Globales::VerCadena(6).length()>0) Globales::CambiarCadena(6,Globales::VerCadena(6).erase(Globales::VerCadena(6).length()-1,1));
					/*Sino, y si la cadena no es superior en largo a 20 caracteres, ni a apretado 'Enter', almacenamos la tecla 
					apretada en la cadena.*/
					else if(Globales::VerCadena(6).length()<20 && e.text.unicode!=13 && e.text.unicode!=8) Globales::CambiarCadena(6,Globales::VerCadena(6)+(char)e.text.unicode);
					reloj.restart();
				}
			}
			if(e.type == Event::KeyPressed){ //Condicional para verificar que el evento es de tipo tecla apretada.
				//Si la tecla apretada es 'Enter', y si la cadena tiene por lo menos 1 caracter, guardamos el nombre/alias.
				if(e.key.code == Keyboard::Return && Globales::VerCadena(6).length()>0){
					Globales::CambiarEstado(0,true);  //Cambiamos el estado de la variable utilzada en la clase 'Menu'.
					Globales::CambiarEstado(6,false);  //Cambiamos el estado para que se dejen de realizar las acciones anteriores.
				}
			}
			Globales::CambiarNombreUsuario(Globales::VerCadena(6)); /*Seteamos continuamente el texto mostrado en pantalla con
																	lo introducido por teclado por el usuario.*/
		}
		if(Globales::VerEstado(3) && Globales::VerEstado(4)){ //Condicional para la ventana-clase 'Opciones'
			if(e.type == Event::TextEntered){ //Condicional para verificar que el evento es tipo entrada de texto.
				/*Si la tecla apretada por el usuario tiene valor de unicode entre los numeros escritos debajo, realizamos el
				cambio de caracter.*/
				if(e.text.unicode >= 65 && e.text.unicode <= 90 || e.text.unicode >= 97 && e.text.unicode <= 122 || e.text.unicode == 32) {
					Globales::CambiarOpcionLetra(e.text.unicode);  //Guardamos el entero que representa la letra.
					Globales::CambiarEstado(5,true);  /*Cambiamos el estado para que se realicen los cambios de teclas en 
														la ventana 'Opciones'.*/
					Globales::CambiarEstado(4,false); /*Cambiamos el estado de la variable para que se dejen de realizar 
														las acciones anteriores.*/
				}
			}
			else{
				if(e.type == Event::KeyPressed){ //Condicional para verificar que el evento es de tipo tecla apretada.
					//En cada caso verificamos que tecla apreto el usuario, para realizar el cambio.
					if(e.key.code == Keyboard::Left) {Globales::CambiarOpcionLetra(-11); Globales::CambiarEstado(5,true); Globales::CambiarEstado(4,false);}
					else if(e.key.code == Keyboard::Right) {Globales::CambiarOpcionLetra(-10); Globales::CambiarEstado(5,true); Globales::CambiarEstado(4,false);}
					else if(e.key.code == Keyboard::Up) {Globales::CambiarOpcionLetra(-9); Globales::CambiarEstado(5,true); Globales::CambiarEstado(4,false);}
					else if(e.key.code == Keyboard::Down) {Globales::CambiarOpcionLetra(-8); Globales::CambiarEstado(5,true); Globales::CambiarEstado(4,false);}
					else if(e.key.code == Keyboard::LControl) {Globales::CambiarOpcionLetra(-6); Globales::CambiarEstado(5,true); Globales::CambiarEstado(4,false);}
					else if(e.key.code == Keyboard::LShift) {Globales::CambiarOpcionLetra(-5); Globales::CambiarEstado(5,true); Globales::CambiarEstado(4,false);}
					else if(e.key.code == Keyboard::LAlt) {Globales::CambiarOpcionLetra(-4); Globales::CambiarEstado(5,true); Globales::CambiarEstado(4,false);}
					else if(e.key.code == Keyboard::RControl) {Globales::CambiarOpcionLetra(-3); Globales::CambiarEstado(5,true); Globales::CambiarEstado(4,false);}
					else if(e.key.code == Keyboard::RShift) {Globales::CambiarOpcionLetra(-2); Globales::CambiarEstado(5,true); Globales::CambiarEstado(4,false);}
					else if(e.key.code == Keyboard::RAlt) {Globales::CambiarOpcionLetra(-1); Globales::CambiarEstado(5,true); Globales::CambiarEstado(4,false);}
				}
			}
		}
	}
}

/*
Por esta funcion corre todo el programa, en la cual se actualiza la ventana de la clase correspondiente y que
acciones debe llevar esta.
*/
void Juego::Run(){
	while(ventana.isOpen()) {
		ProcesarEvento();
		Actualizar();
		Dibujar();
		if(escenaSiguiente){ /*Si se ha seteado una nueva escena, una nueva clase, se borra el puntero que apunta a la clase vieja,
							y se almacena en el puntero 'escenaActual' la nueva escena-clase, borrando, luego, lo apuntado por el 
							puntero 'escenaSiguiente'. De esta manera, el puntero 'escenaActual' realizara los metodos 'Actualizar'
							y 'Dibujar' correspondientes.*/
			delete escenaActual;
			escenaActual = escenaSiguiente;
			escenaSiguiente = nullptr;
		}
	}
}

//Mediante el puntero 'escenaActual' se corre el metodo 'Actualizar' conrrespondiente.
void Juego::Actualizar(){
	escenaActual->Actualizar(*this,ventana);
}

//Mediante el puntero 'escenaActual' se corre el metodo 'Dibujar' conrrespondiente.
void Juego::Dibujar(){
	escenaActual->Dibujar(ventana);
}


/*
Esta funcion es la encargada de setear los parametros cambiables del juego al momento de iniciarse este, debido a la lectura
de el archivo 'Controles.txt' (hecho en la clase 'Archivo').
*/
void Juego::SetearControles(){
	string control; int numTecla = 0, nLinea = 0;
	archivo.LeerControl(vectorCadenas); /*Con el objeto 'archivo' de la clase 'Archivo' llamamos al metodo 'LeerControl', con el
										objetivo de almacenar en el 'vectorCadenas' (un vector de string) las distintas cadenas de
										texto del archivo 'Controles.txt'.*/
	if(vectorCadenas.size()!=0){ //Corroboramos que se haya podido leer el archivo.
		for(size_t i=0;i<vectorCadenas.size();i++){
			size_t posIgual = vectorCadenas[i].find('='); //Buscamos la posicion del signo 'Igual' en la cadena de texto.
			if(posIgual==string::npos) cerr<<"No se pudo encontrar el singo '='"<<endl;
			else{ //Si se encuentra la posicion...
				control = vectorCadenas[i].substr(posIgual+1); //Guardamos en una variable lo que haya despues del signo 'Igual'.
				if(nLinea==0) { /*Con la primera cadena de texto buscamos ver con que dificultad se ha jugado por ultima vez al juego,
								de manera de setearla con esa misma de arranque.*/
					Globales::CambiarCadena(8,control); nLinea++;
					if(Globales::VerCadena(8)=="Facil") Globales::CambiarDificultad(0);
					else if(Globales::VerCadena(8)=="Media" || Globales::VerCadena(8)=="Medio") Globales::CambiarDificultad(1);
					else if(Globales::VerCadena(8)=="Dificil") Globales::CambiarDificultad(2);
				}
				else if(nLinea<6){ /*Con las siguientes 5 lineas buscamos ver con que teclas el usuario realizaba las acciones de
									movimiento de las formas, con el objetivo de setearlas con esas misma de arranque.*/
					int valor = RetornarValor(control);
					modificarTeclas(valor,numTecla);
					numTecla++; nLinea++;
				}else if(nLinea==6){ //Con la penultima linea verificamos que tipo de colores fueron los ultimos usados en el juego.
					Globales::CambiarCadena(7,control); nLinea++;
					if(Globales::VerCadena(7)=="Normal") Globales::CambiarEstado(7,true);
					else Globales::CambiarEstado(7,false);
				}else if(nLinea==7){ //Con la ultima linea se verifica con que tecla prefirio el usuario usar al momento de poner pausa en el juego.
					int valor = RetornarValor(control);
					modificarTeclas(valor,numTecla);
					nLinea++;
				}else{
					Globales::CambiarCadena(9,control);
					if(Globales::VerCadena(9)=="Prendido") Globales::CambiarEstado(14,true);
					else Globales::CambiarEstado(14,false);
				}
			}
		}
	}
}

//Esta funcion busca en un 'map' de <string,int> la clave dada (string) y, si la encuentra, retorna el valor anidado a este (int).
int Juego::RetornarValor(string control){
	auto it = teclado.find(control);
	return it->second;
}

/*
Esta funcion asocia las diferentes teclas permitidas para mover las distintas formas del juego con un número, 
de manera de poder establecer a cada Key-Tecla un valor especifico. Mediante la lectura de las distintas teclas (con la clase
'Archivo'), y mediante un 'map' de <string,int>, se asocia cada letra a un valor.
*/
void Juego::valoresTeclas(){
	int valor = -37;
	archivo.LeerTeclas(vectorCadenas); //Se hace uso del objeto 'archivo' de la clase 'Archivos' para llamar al metodo 'LeetTeclas'.
	if(vectorCadenas.size()!=0){
		for(size_t i=0;i<vectorCadenas.size();i++){
			teclado[vectorCadenas[i]] = valor;
			valor++;
		}
	}
}
