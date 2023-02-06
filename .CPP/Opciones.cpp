#include "Opciones.h"
#include "Menu.h"

/*
Su constructor modificar ciertas variables de tipo 'bool' e 'int', como asi tambien crea las dimensiones de la ventana y 
establece la valocidad de los fotogramas. Configura, si encuentra, la textura a utilizar y el color de fondo de la ventana,
dependiendo la dificultad establecida. A su vez, si encuentra una fuente, setea un vector de Text para ser utilizado en la
funcion 'CrearTexto' (explicada mas debajo).
*/
Opciones::Opciones(RenderWindow &ventana) : Escenas(ventana){
	Globales::CambiarEstado(3,true); //Cambiamos el estado para 'saber' que estamos en la ventana de la clase 'Opciones'.
	guardarArchivo = false; modificarDificultad = false; modificarColor = false;
	ventana.create(VideoMode(800,700),"Opciones"); ventana.setFramerateLimit(60);
	//Si se pudo cargar alguna fuente, creamos los textos.
	if(fuente.EstadoFuenteOpciones() || fuente.EstadoFuenteTetris() || fuente.EstadoFuenteInformacion()) {
		y = 80; vectorTexto.resize(19); CrearTexto();
	}
	//Cargamos, si encuentra, la textura y la configuramos.
	if(!tFlecha.loadFromFile("Imagenes/Flecha.png")) cerr<<"No se pudo abrir la imagen 'Flecha.png'"<<endl; 
	else {sFlecha.setTexture(tFlecha); sFlecha.setScale(0.08,0.08); sFlecha.setPosition(-5,14); sFlecha.setColor(Color(0,0,0,128));}
	//Cambiamos el color de fondo de la ventana dependiendo de la dificultad actual del juego.
	if(Globales::VerCadena(8)=="Facil")  Globales::CambiarColor(Color(255,51,153));
	else if(Globales::VerCadena(8)=="Media" || Globales::VerCadena(8)=="Medio")  Globales::CambiarColor(Color(255,128,0));
	else if(Globales::VerCadena(8)=="Dificil")  Globales::CambiarColor(Color(255,15,0));
	//Corroboramos si el icono pudo ser cargado.
	if(!icono.loadFromFile("Imagenes/Icono.png")) cerr<<"No se pudo encontrar la imagen 'Icono.png'"<<endl;
	else ventana.setIcon(icono.getSize().x,icono.getSize().y,icono.getPixelsPtr()); //Seteamos el icono de la ventana.
}

/*
El 'Actualizar' de la clase 'Opciones' se encarga:
	1) De chequear si el usuario a cliqueado alguno de los parametros intercambiables para permitirle elegir:
		i) La tecla con la cual se realiza cierta accion en el juego (mover forma, poner pausa, etc).
		ii) Elegir la dificultad del juego.
		iii) Cambiar de color las formas.
		iv) Activar o desactivar el modo 'Fila prendida'.
	2) Chequea tambien si el usuario aprieta la tecla 'Esc' o cliquea en la imagen de tipo flecha para volver a la ventana de la 
	clase 'Menu'. 
	Las funciones 'ModificarArchivo', 'ChequearMouse', 'ObtenerOpcionElegida', 'CambiarTexto', y 'RealizarCambio' se explican mas debajo.
*/
void Opciones::Actualizar(Juego &juego, RenderWindow &ventana) {
	if(Keyboard::isKeyPressed(Keyboard::Key::Escape)) {//Condicional que verifica si se ha apretado la tecla 'Escape'.
		if(guardarArchivo) archivo.ModificarArchivo(); /*Si se ha hecho alguna modificacion, se llama al metodo 'ModificarArchivo'
														de la clase 'Archivo'.*/
		juego.SetiarEscena(new Menu(ventana)); //Se cambia a la clase 'Menu', modificando la ventana.
	}
	ChequearMouse(ventana); //Se chequea constantemente la posicion del mouse.
	if(relojInformacion.getElapsedTime().asSeconds()>12){ /*Si no se ha hecho ninguna accion en el tiempo determinado, se muestra
															un texto informativo diferente.*/
		vectorTexto[18].setString("Haz click en cualquier parametro para cambiarlo"); vectorTexto[18].setPosition(150,30);
	}
	if(Mouse::isButtonPressed(Mouse::Left) && reloj.getElapsedTime().asMilliseconds()>150){ /*Condicional que verifica si se ha 
																						hecho click izquierdo con el mousese y si ha 
																						pasado el suficiente tiempo.*/
		posicionMouse = Mouse::getPosition(ventana); //Se obtiene la posicion x-y del mouse.
		opcionSeleccionada = ObtenerOpcionElegida(posicionMouse); /*Obtenemos un numero indicativo de lo seleccionado con el usuario 
																	mediante el metodo 'ObtenerOpcionElegida'.*/
		//En los siguientes 5 condicionales se verifica si se ha cliqueado en alguna opcion con accion.
		if(opcionSeleccionada==1){ //Dentro de este condicional se cambia la dificultad del juego, como tambien el color de la ventana.
			switch(Globales::VerDificultad()){
			case 0:{ Globales::CambiarCadena(8,"Media"); Globales::CambiarDificultad(1); Globales::CambiarColor(Color(255,128,0)); modificarDificultad = true; break;}
			case 1:{ Globales::CambiarCadena(8,"Dificil"); Globales::CambiarDificultad(2); Globales::CambiarColor(Color(255,15,0)); modificarDificultad = true; break;}
			default:{ Globales::CambiarCadena(8,"Facil"); Globales::CambiarDificultad(0); Globales::CambiarColor(Color(255,51,153)); modificarDificultad = true; break;}}
			relojInformacion.restart();
		}
		if(opcionSeleccionada>1 && opcionSeleccionada<7 || opcionSeleccionada==8) { /*Dentro de este condicional se esta a la espera
																				de que el usuario teclee, de manera de cambiar la tecla
																			con la que se realiza la accion elegida.*/
			vectorTexto[18].setString("Teclea cualquier letra para configurarla"); vectorTexto[18].setPosition(200,30);
			Globales::CambiarEstado(4,true); relojInformacion.restart();
		}
		if(opcionSeleccionada==7){ //Dentro de este condicional se modifica los colores de las formas en el juego.
			if(Globales::VerEstado(7)) {Globales::CambiarCadena(7,"Random"); Globales::CambiarEstado(7,false); modificarColor = true;}
			else {Globales::CambiarCadena(7,"Normal"); Globales::CambiarEstado(7,true); modificarColor = true;}
		}
		if(opcionSeleccionada==9){ //Dentro de este condicional se modifica el estado del modo 'Fila piedra'.
			if(Globales::VerEstado(14)) {Globales::CambiarCadena(9,"Apagado"); Globales::CambiarEstado(14,false); modificarPrendido = true;}
			else {Globales::CambiarCadena(9,"Prendido"); Globales::CambiarEstado(14,true); modificarPrendido = true;}
		}
		if(opcionSeleccionada==-1) { /*Dentro de este condicional se verifica si se ha hecho algun cambio, para guardarlo, y se
									vuelve a la clase 'Menu'.*/
			if(guardarArchivo) archivo.ModificarArchivo();
			juego.SetiarEscena(new Menu(ventana));
		}
		reloj.restart();
	}
	if(Globales::VerEstado(5)){ //Condicional para verificar si se ha hecho el cambio de alguna tecla.
		RealizarCambio(opcionSeleccionada); //Se cambia la tecla con la que se realiza la accion elegida.
		CambiarTexto(opcionSeleccionada); //Se cambia el texto, mostrando la nueva tecla con la que se realiza la accion.
		vectorTexto[18].setString("Tecla cambiada"); vectorTexto[18].setPosition(300,30);
		Globales::CambiarEstado(5,false); //Cambiamos el estado para que estas acciones no se repitan.
		guardarArchivo = true; opcionSeleccionada = 0;  //Reseteamos las variables.
		relojInformacion.restart(); //Reiniciamos el reloj.
	}
	if(modificarDificultad){ //Condicional para verificar si se ha cambiado la dificultad.
		CambiarTexto(opcionSeleccionada); //Se cambia el texto de la dificultad, para dar entender al usuario que se ha hecho el cambio.
		vectorTexto[18].setString("Dificultad cambiada"); vectorTexto[18].setPosition(300,30);
		modificarDificultad = false; guardarArchivo = true; opcionSeleccionada = 0;
		relojInformacion.restart();
	}
	if(modificarColor){ //Condicional para verificar si se ha cambiado los colores.
		CambiarTexto(opcionSeleccionada); //Se cambia el texto de los colores, para dar entender al usuario que se ha hecho el cambio.
		vectorTexto[18].setString("Colores cambiados"); vectorTexto[18].setPosition(300,30);
		modificarColor = false; guardarArchivo = true; opcionSeleccionada = 0;
		relojInformacion.restart();
	}
	if(modificarPrendido){ //Condicional para verificar si se ha activado/desactivado el modo 'Fila piedra'.
		CambiarTexto(opcionSeleccionada); //Se cambia el texto indicativo si el modo 'Fila piedra' esta activo o no.
		if(Globales::VerEstado(14)) vectorTexto[18].setString("Filas piedras activadas");  
		else vectorTexto[18].setString("Filas piedras desactivadas"); 
		vectorTexto[18].setPosition(280,30);
		modificarPrendido = false; guardarArchivo = true; opcionSeleccionada = 0;
		relojInformacion.restart();
	}
}

//Esta funcion es la encargada de mostrar en pantalla los parametros de texto, textura y color establecidos.
void Opciones::Dibujar (RenderWindow & ventana) {
	ventana.clear(Globales::VerColor());
	for(size_t i=0;i<vectorTexto.size();i++) ventana.draw(vectorTexto[i]);
	ventana.draw(sFlecha);
	ventana.display();
}

/*
Esta función, como tambien la siguiente 'OpcionesGenerales', son las encargadas de crear y setear los distintos parametros de
los textos mostrados.
*/
void Opciones::CrearTexto(){
	if(fuente.EstadoFuenteOpciones() || fuente.EstadoFuenteTetris()){ //Verificamos si la fuente 'Opciones.ttf' y/o 'Tetris' han sido cargadas.
		if(fuente.EstadoFuenteOpciones() && fuente.EstadoFuenteTetris()){  //Verificamos si se han cargado las 2 fuentes.
			OpcionesGenerales(vectorTexto[0],fuente.VerFuenteTetris(),true,"Dificultad:",28,Color(Color::Yellow));
			OpcionesGenerales(vectorTexto[1],fuente.VerFuenteOpciones(),false,Globales::VerCadena(8),28,Color(Color::Green));
			OpcionesGenerales(vectorTexto[2],fuente.VerFuenteTetris(),true,"Mover bloque hacia la izquierda:",28,Color(Color::Yellow));
			OpcionesGenerales(vectorTexto[3],fuente.VerFuenteOpciones(),false,Globales::VerCadena(0),28,Color(Color::Green));
			OpcionesGenerales(vectorTexto[4],fuente.VerFuenteTetris(),true,"Mover bloque hacia la derecha:",28,Color(Color::Yellow)); 
			OpcionesGenerales(vectorTexto[5],fuente.VerFuenteOpciones(),false,Globales::VerCadena(1),28,Color(Color::Green));
			OpcionesGenerales(vectorTexto[6],fuente.VerFuenteTetris(),true,"Mover bloque hacia abajo:",28,Color(Color::Yellow));
			OpcionesGenerales(vectorTexto[7],fuente.VerFuenteOpciones(),false,Globales::VerCadena(2),28,Color(Color::Green));
			OpcionesGenerales(vectorTexto[8],fuente.VerFuenteTetris(),true,"Rotar pieza:",28,Color(Color::Yellow));
			OpcionesGenerales(vectorTexto[9],fuente.VerFuenteOpciones(),false,Globales::VerCadena(3),28,Color(Color::Green));
			OpcionesGenerales(vectorTexto[10],fuente.VerFuenteTetris(),true,"Hard-Drop",28,Color(Color::Yellow));
			OpcionesGenerales(vectorTexto[11],fuente.VerFuenteOpciones(),false,Globales::VerCadena(4),28,Color(Color::Green));
			OpcionesGenerales(vectorTexto[12],fuente.VerFuenteTetris(),true,"Colores formas:",28,Color(Color::Yellow));
			OpcionesGenerales(vectorTexto[13],fuente.VerFuenteOpciones(),false,Globales::VerCadena(7),28,Color(Color::Green));
			OpcionesGenerales(vectorTexto[14],fuente.VerFuenteTetris(),true,"Pausa:",28,Color(Color::Yellow));
			OpcionesGenerales(vectorTexto[15],fuente.VerFuenteOpciones(),false,Globales::VerCadena(5),28,Color(Color::Green));
			OpcionesGenerales(vectorTexto[16],fuente.VerFuenteTetris(),true,"Filas piedra:",28,Color(Color::Yellow));
			OpcionesGenerales(vectorTexto[17],fuente.VerFuenteOpciones(),false,Globales::VerCadena(9),28,Color(Color::Green));
			if(!fuente.EstadoFuenteInformacion()) {	 //Verificamos si la fuente 'Informacion.ttf' se ha cargado o no.
				OpcionesGenerales(vectorTexto[18],fuente.VerFuenteOpciones(),false,"Haz click en la dificultad o en la letra para cambiarla",15,Color(Color::Black)); 
				vectorTexto[18].setPosition(120,30);
			}
			else {
				OpcionesGenerales(vectorTexto[18],fuente.VerFuenteInformacion(),false,"Haz click en la dificultad o en la letra para cambiarla",10,Color(Color::Black)); 
				vectorTexto[18].setPosition(120,30);
			}
		}
		else if(fuente.EstadoFuenteOpciones()){ //Verificamos si se ha cargado la fuente 'Opciones.ttf'
			OpcionesGenerales(vectorTexto[0],fuente.VerFuenteOpciones(),true,"Dificultad:",28,Color(Color::Yellow));
			OpcionesGenerales(vectorTexto[1],fuente.VerFuenteOpciones(),false,Globales::VerCadena(8),28,Color(Color::Green));
			OpcionesGenerales(vectorTexto[2],fuente.VerFuenteOpciones(),true,"Mover bloque hacia la izquierda:",20,Color(Color::Yellow));
			OpcionesGenerales(vectorTexto[3],fuente.VerFuenteOpciones(),false,Globales::VerCadena(0),28,Color(Color::Green));
			OpcionesGenerales(vectorTexto[4],fuente.VerFuenteOpciones(),true,"Mover bloque hacia la derecha:",20,Color(Color::Yellow)); 
			OpcionesGenerales(vectorTexto[5],fuente.VerFuenteOpciones(),false,Globales::VerCadena(1),28,Color(Color::Green));
			OpcionesGenerales(vectorTexto[6],fuente.VerFuenteOpciones(),true,"Mover bloque hacia abajo:",24,Color(Color::Yellow));
			OpcionesGenerales(vectorTexto[7],fuente.VerFuenteOpciones(),false,Globales::VerCadena(2),28,Color(Color::Green));
			OpcionesGenerales(vectorTexto[8],fuente.VerFuenteOpciones(),true,"Rotar pieza:",28,Color(Color::Yellow));
			OpcionesGenerales(vectorTexto[9],fuente.VerFuenteOpciones(),false,Globales::VerCadena(3),28,Color(Color::Green));
			OpcionesGenerales(vectorTexto[10],fuente.VerFuenteOpciones(),true,"Hard-Drop",28,Color(Color::Yellow));
			OpcionesGenerales(vectorTexto[11],fuente.VerFuenteOpciones(),false,Globales::VerCadena(4),28,Color(Color::Green));
			OpcionesGenerales(vectorTexto[12],fuente.VerFuenteOpciones(),true,"Colores formas:",28,Color(Color::Yellow));
			OpcionesGenerales(vectorTexto[13],fuente.VerFuenteOpciones(),false,Globales::VerCadena(7),28,Color(Color::Green));
			OpcionesGenerales(vectorTexto[14],fuente.VerFuenteOpciones(),true,"Pausa:",28,Color(Color::Yellow));
			OpcionesGenerales(vectorTexto[15],fuente.VerFuenteOpciones(),false,Globales::VerCadena(5),28,Color(Color::Green));
			OpcionesGenerales(vectorTexto[16],fuente.VerFuenteOpciones(),true,"Filas piedra:",28,Color(Color::Yellow));
			OpcionesGenerales(vectorTexto[17],fuente.VerFuenteOpciones(),false,Globales::VerCadena(9),28,Color(Color::Green));
			if(!fuente.EstadoFuenteInformacion()){ 
				OpcionesGenerales(vectorTexto[18],fuente.VerFuenteOpciones(),false,"Haz click en la dificultad o en la letra para cambiarla",15,Color(Color::Black)); 
				vectorTexto[18].setPosition(120,30);
			}
			else {
				OpcionesGenerales(vectorTexto[18],fuente.VerFuenteInformacion(),false,"Haz click en la dificultad o en la letra para cambiarla",10,Color(Color::Black)); 
				vectorTexto[18].setPosition(120,30);
			}
		}
		else{
			OpcionesGenerales(vectorTexto[0],fuente.VerFuenteTetris(),true,"Dificultad:",28,Color(Color::Yellow));
			OpcionesGenerales(vectorTexto[1],fuente.VerFuenteTetris(),false,Globales::VerCadena(8),28,Color(Color::Green));
			OpcionesGenerales(vectorTexto[2],fuente.VerFuenteTetris(),true,"Mover bloque hacia la izquierda:",28,Color(Color::Yellow));
			OpcionesGenerales(vectorTexto[3],fuente.VerFuenteTetris(),false,Globales::VerCadena(0),28,Color(Color::Green));
			OpcionesGenerales(vectorTexto[4],fuente.VerFuenteTetris(),true,"Mover bloque hacia la derecha:",28,Color(Color::Yellow)); 
			OpcionesGenerales(vectorTexto[5],fuente.VerFuenteTetris(),false,Globales::VerCadena(1),28,Color(Color::Green));
			OpcionesGenerales(vectorTexto[6],fuente.VerFuenteTetris(),true,"Mover bloque hacia abajo:",28,Color(Color::Yellow));
			OpcionesGenerales(vectorTexto[7],fuente.VerFuenteTetris(),false,Globales::VerCadena(2),28,Color(Color::Green));
			OpcionesGenerales(vectorTexto[8],fuente.VerFuenteTetris(),true,"Rotar pieza:",28,Color(Color::Yellow));
			OpcionesGenerales(vectorTexto[9],fuente.VerFuenteTetris(),false,Globales::VerCadena(3),28,Color(Color::Green));
			OpcionesGenerales(vectorTexto[10],fuente.VerFuenteTetris(),true,"Hard-Drop",28,Color(Color::Yellow));
			OpcionesGenerales(vectorTexto[11],fuente.VerFuenteTetris(),false,Globales::VerCadena(4),28,Color(Color::Green));
			OpcionesGenerales(vectorTexto[12],fuente.VerFuenteTetris(),true,"Colores formas:",28,Color(Color::Yellow));
			OpcionesGenerales(vectorTexto[13],fuente.VerFuenteTetris(),false,Globales::VerCadena(7),28,Color(Color::Green));
			OpcionesGenerales(vectorTexto[14],fuente.VerFuenteTetris(),true,"Pausa:",28,Color(Color::Yellow));
			OpcionesGenerales(vectorTexto[15],fuente.VerFuenteTetris(),false,Globales::VerCadena(5),28,Color(Color::Green));
			OpcionesGenerales(vectorTexto[16],fuente.VerFuenteTetris(),true,"Filas piedra:",28,Color(Color::Yellow));
			OpcionesGenerales(vectorTexto[17],fuente.VerFuenteTetris(),false,Globales::VerCadena(9),28,Color(Color::Green));
			if(!fuente.EstadoFuenteInformacion()) {
				OpcionesGenerales(vectorTexto[18],fuente.VerFuenteTetris(),false,"Haz click en la dificultad o en la letra para cambiarla",20,Color(Color::Black)); 
				vectorTexto[18].setPosition(120,30);
			}
			else {
				OpcionesGenerales(vectorTexto[18],fuente.VerFuenteInformacion(),false,"Haz click en la dificultad o en la letra para cambiarla",10,Color(Color::Black)); 
				vectorTexto[18].setPosition(120,30);
			}
		}
	}
	else{ //Sino se ha cargado la fuente 'Opciones.tff' ni 'Tetris.ttf', utilizamos la fuente 'Informacion.ttf'.
		OpcionesGenerales(vectorTexto[0],fuente.VerFuenteInformacion(),true,"Dificultad:",22,Color(Color::Yellow));
		OpcionesGenerales(vectorTexto[1],fuente.VerFuenteInformacion(),false,Globales::VerCadena(8),22,Color(Color::Green));
		OpcionesGenerales(vectorTexto[2],fuente.VerFuenteInformacion(),true,"Mover bloque hacia la izquierda:",13,Color(Color::Yellow));
		OpcionesGenerales(vectorTexto[3],fuente.VerFuenteInformacion(),false,Globales::VerCadena(0),22,Color(Color::Green));
		OpcionesGenerales(vectorTexto[4],fuente.VerFuenteInformacion(),true,"Mover bloque hacia la derecha:",13,Color(Color::Yellow)); 
		OpcionesGenerales(vectorTexto[5],fuente.VerFuenteInformacion(),false,Globales::VerCadena(1),22,Color(Color::Green));
		OpcionesGenerales(vectorTexto[6],fuente.VerFuenteInformacion(),true,"Mover bloque hacia abajo:",16,Color(Color::Yellow));
		OpcionesGenerales(vectorTexto[7],fuente.VerFuenteInformacion(),false,Globales::VerCadena(2),22,Color(Color::Green));
		OpcionesGenerales(vectorTexto[8],fuente.VerFuenteInformacion(),true,"Rotar pieza:",22,Color(Color::Yellow));
		OpcionesGenerales(vectorTexto[9],fuente.VerFuenteInformacion(),false,Globales::VerCadena(3),22,Color(Color::Green));
		OpcionesGenerales(vectorTexto[10],fuente.VerFuenteInformacion(),true,"Hard-Drop",22,Color(Color::Yellow));
		OpcionesGenerales(vectorTexto[11],fuente.VerFuenteInformacion(),false,Globales::VerCadena(4),22,Color(Color::Green));
		OpcionesGenerales(vectorTexto[12],fuente.VerFuenteInformacion(),true,"Colores formas:",22,Color(Color::Yellow));
		OpcionesGenerales(vectorTexto[13],fuente.VerFuenteInformacion(),false,Globales::VerCadena(7),22,Color(Color::Green));
		OpcionesGenerales(vectorTexto[14],fuente.VerFuenteInformacion(),true,"Pausa:",22,Color(Color::Yellow));
		OpcionesGenerales(vectorTexto[15],fuente.VerFuenteInformacion(),false,Globales::VerCadena(5),22,Color(Color::Green));
		OpcionesGenerales(vectorTexto[16],fuente.VerFuenteInformacion(),true,"Filas piedra:",22,Color(Color::Yellow));
		OpcionesGenerales(vectorTexto[17],fuente.VerFuenteInformacion(),false,Globales::VerCadena(9),22,Color(Color::Green));
		OpcionesGenerales(vectorTexto[18],fuente.VerFuenteInformacion(),false,"Haz click en la dificultad o en la letra para cambiarla",10,Color(Color::Black)); 
		vectorTexto[18].setPosition(120,30);
	}
}

//Se establece la fuente a usar, el contenido del texto, su tamaño, su color, y su posicion.
void Opciones::OpcionesGenerales(Text &texto,Font &fuente,bool estatico,string titulo,int tamanio,Color color){
	texto.setFont(fuente); texto.setString(titulo); texto.setFillColor(color); texto.setCharacterSize(tamanio); 
	if(estatico) texto.setPosition(50,y);
	else {texto.setPosition(570,y); y+=70;}
}

/*
Esta función devuelve, si el usuario hace click con el mouse, un numero, dependiendo donde se haya realizado la accion.
El numero de vuelto determina luego que accion hacer.
*/
int Opciones::ObtenerOpcionElegida(Vector2i posicionMouse){
	/*Se obtiene la posicion left-width-top-height de los distintos textos para corroborar la posicion del mouse con respecto
	a estos valores, de manera de verificar si el usuario a hecho click en cualquier opcion.*/
	posicionOpcion = vectorTexto[1].getGlobalBounds();
	if(posicionMouse.x>=posicionOpcion.left && posicionMouse.x<=(posicionOpcion.width+posicionOpcion.left)
	&& posicionMouse.y>=posicionOpcion.top && posicionMouse.y<=(posicionOpcion.height+posicionOpcion.top)) return 1;
	posicionOpcion = vectorTexto[3].getGlobalBounds();
	if(posicionMouse.x>=posicionOpcion.left && posicionMouse.x<=(posicionOpcion.width+posicionOpcion.left)
	&& posicionMouse.y>=posicionOpcion.top && posicionMouse.y<=(posicionOpcion.height+posicionOpcion.top)) return 2;
	posicionOpcion = vectorTexto[5].getGlobalBounds();
	if(posicionMouse.x>=posicionOpcion.left && posicionMouse.x<=(posicionOpcion.width+posicionOpcion.left)
	&& posicionMouse.y>=posicionOpcion.top && posicionMouse.y<=(posicionOpcion.height+posicionOpcion.top)) return 3;
	posicionOpcion = vectorTexto[7].getGlobalBounds();
	if(posicionMouse.x>=posicionOpcion.left && posicionMouse.x<=(posicionOpcion.width+posicionOpcion.left)
	&& posicionMouse.y>=posicionOpcion.top && posicionMouse.y<=(posicionOpcion.height+posicionOpcion.top)) return 4;
	posicionOpcion = vectorTexto[9].getGlobalBounds();
	if(posicionMouse.x>=posicionOpcion.left && posicionMouse.x<=(posicionOpcion.width+posicionOpcion.left)
	&& posicionMouse.y>=posicionOpcion.top && posicionMouse.y<=(posicionOpcion.height+posicionOpcion.top)) return 5;
	posicionOpcion = vectorTexto[11].getGlobalBounds();
	if(posicionMouse.x>=posicionOpcion.left && posicionMouse.x<=(posicionOpcion.width+posicionOpcion.left)
	&& posicionMouse.y>=posicionOpcion.top && posicionMouse.y<=(posicionOpcion.height+posicionOpcion.top)) return 6;
	posicionOpcion = vectorTexto[13].getGlobalBounds();
	if(posicionMouse.x>=posicionOpcion.left && posicionMouse.x<=(posicionOpcion.width+posicionOpcion.left)
	&& posicionMouse.y>=posicionOpcion.top && posicionMouse.y<=(posicionOpcion.height+posicionOpcion.top)) return 7;
	posicionOpcion = sFlecha.getGlobalBounds();
	posicionOpcion = vectorTexto[15].getGlobalBounds();
	if(posicionMouse.x>=posicionOpcion.left && posicionMouse.x<=(posicionOpcion.width+posicionOpcion.left)
	&& posicionMouse.y>=posicionOpcion.top && posicionMouse.y<=(posicionOpcion.height+posicionOpcion.top)) return 8;
	posicionOpcion = vectorTexto[17].getGlobalBounds();
	if(posicionMouse.x>=posicionOpcion.left && posicionMouse.x<=(posicionOpcion.width+posicionOpcion.left)
	&& posicionMouse.y>=posicionOpcion.top && posicionMouse.y<=(posicionOpcion.height+posicionOpcion.top)) return 9;
	posicionOpcion = sFlecha.getGlobalBounds();
	if(posicionMouse.x>=posicionOpcion.left && posicionMouse.x<=(posicionOpcion.width+posicionOpcion.left)
	&& posicionMouse.y>=posicionOpcion.top && posicionMouse.y<=(posicionOpcion.height+posicionOpcion.top)) return -1;
	return 0;
}

/*
Esta funcion llama al metodo 'modificarTeclas', pasandole distintos parametros dependiendo que haya seleccionado el usuario, con
el objetivo de cambiar la tecla con la que se realiza cierta accion.
*/
void Opciones::RealizarCambio(int opcionSeleccionada){
	switch(opcionSeleccionada){
	case 2: {modificarTeclas(Globales::VerOpcionLetra(),0); break;}
	case 3: {modificarTeclas(Globales::VerOpcionLetra(),1); break;}
	case 4: {modificarTeclas(Globales::VerOpcionLetra(),2); break;}
	case 5: {modificarTeclas(Globales::VerOpcionLetra(),3); break;}
	case 6: {modificarTeclas(Globales::VerOpcionLetra(),4); break;}
	case 8: {modificarTeclas(Globales::VerOpcionLetra(),5); break;}}
}

/*
Esta funcion se encarga de setear el texto mostrado, de manera de brindar mas ayuda visual al usuario, con el objetivo
de hacerle entender que se ha realizado la modificacion.
*/
void Opciones::CambiarTexto(int opcionSeleccionada){
	switch(opcionSeleccionada){
	case 1: {vectorTexto[1].setString(Globales::VerCadena(8)); break;}
	case 2: {vectorTexto[3].setString(Globales::VerCadena(0)); break;}
	case 3: {vectorTexto[5].setString(Globales::VerCadena(1)); break;}
	case 4: {vectorTexto[7].setString(Globales::VerCadena(2)); break;}
	case 5: {vectorTexto[9].setString(Globales::VerCadena(3)); break;}
	case 6: {vectorTexto[11].setString(Globales::VerCadena(4)); break;}
	case 7: {vectorTexto[13].setString(Globales::VerCadena(7)); break;}
	case 8: {vectorTexto[15].setString(Globales::VerCadena(5)); break;}
	default: {vectorTexto[17].setString(Globales::VerCadena(9)); break;}}
}

/*
Esta funcion es la encargada de chequear la posicion del mouse, de manera de cambiar el color de los parametros cambiables
por el usuario si este ha pasado el raton por encima de alguno de estos, con el objetivo de brindar mas ayuda visual.
*/
void Opciones::ChequearMouse(RenderWindow &ventana){
	posicionMouse = Mouse::getPosition(ventana); int n;
	opcionSeleccionada = ObtenerOpcionElegida(posicionMouse);
	switch(opcionSeleccionada){
	case 1: {vectorTexto[1].setFillColor(Color::Black); n = 1; break;} /*Guardamos en la variable 'n' que texto esta siendo actualmente
																	'tocado' por el mouse del usuario, de manera de cambiarle el color.*/
	case 2: {vectorTexto[3].setFillColor(Color::Black); n = 3; break;}
	case 3: {vectorTexto[5].setFillColor(Color::Black); n = 5; break;}
	case 4: {vectorTexto[7].setFillColor(Color::Black); n = 7; break;}
	case 5: {vectorTexto[9].setFillColor(Color::Black); n = 9; break;}
	case 6: {vectorTexto[11].setFillColor(Color::Black); n = 11; break;}
	case 7: {vectorTexto[13].setFillColor(Color::Black); n = 13; break;}
	case 8: {vectorTexto[15].setFillColor(Color::Black); n = 15; break;}
	case 9: {vectorTexto[17].setFillColor(Color::Black); n = 17; break;}}
	for(size_t i=1;i<vectorTexto.size();i+=2){
		if(i==n) continue; 
		vectorTexto[i].setFillColor(Color::Green); //A las opciones no seleccionadas por el usuario la dejamos en el color habitual.
	}
}
