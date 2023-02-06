#include "Puntajes.h"

/*
En su constructor se setean las dimenciones de la ventana y la velocidad de los fotogramas. Asimismo, si se encuentra, se setean
las fuentes y la textura a utilizarse. Mediante el metodo  'LeerScore' de la clase 'Archivo', y mediante la dificultad
elegida por el usuario, se lee el archivo 'Score...txt' que almacena los puntajes de los diferentes jugadores en un vector de Usuario. 
Usuario, en este caso, es un struct que contiene un char[20] y un int. Si se pudo leer, se llama a la funcion 'CrearTexto' 
(explicada mas debajo) para crear los textos con los nombres y puntajes almacenados.
*/
Puntajes::Puntajes(RenderWindow &ventana) : Escenas(ventana){
	ventana.create(VideoMode(680,680),"Puntuaciones"); ventana.setFramerateLimit(60);
	y = 170; vectorTexto.resize(23);
	//Corroboramos si el icono pudo ser cargado.
	if(!icono.loadFromFile("Imagenes/Icono.png")) cerr<<"No se pudo encontrar la imagen 'Icono.png'"<<endl;
	else ventana.setIcon(icono.getSize().x,icono.getSize().y,icono.getPixelsPtr()); //Seteamos el icono de la ventana.
	if(fuente.EstadoFuenteInformacion() || fuente.EstadoFuenteTetris() || fuente.EstadoFuenteOpciones()){ //Verificamos sii se ha podido cargar alguna de las 2 fuentes.
		if(fuente.EstadoFuenteInformacion()) { //Verificamos si se ha podido cargar la fuente 'Informacion.ttf'.
			vectorTexto[21].setFont(fuente.VerFuenteInformacion()); vectorTexto[21].setCharacterSize(8);
			if(fuente.EstadoFuenteOpciones()){
				vectorTexto[22].setFont(fuente.VerFuenteOpciones()); vectorTexto[22].setCharacterSize(14);
			} else vectorTexto[22].setFont(fuente.VerFuenteInformacion()); vectorTexto[22].setCharacterSize(20);
		}
		else {vectorTexto[21].setFont(fuente.VerFuenteOpciones()); vectorTexto[21].setCharacterSize(16);}
		//Verificamos si se ha podido cargar la fuente 'Tetris.ttf'.
		if(fuente.EstadoFuenteTetris()) {vectorTexto[20].setFont(fuente.VerFuenteTetris()); vectorTexto[20].setCharacterSize(42);}
		else {vectorTexto[20].setFont(fuente.VerFuenteInformacion()); vectorTexto[20].setCharacterSize(24);}
		vectorTexto[20].setFillColor(Color(255,0,255)); vectorTexto[20].setPosition(130,100);
		vectorTexto[21].setString("Haz click en la dificultad para cambiarla y visualizar los otros puntajes");
		vectorTexto[21].setFillColor(Color::Black); vectorTexto[21].setPosition(15,80);
		vectorTexto[22].setFillColor(Color::Blue); vectorTexto[22].setPosition(165,35);
		//Verificamos si el modo 'Fila piedra' esta activo.
		if(Globales::VerEstado(14)) vectorTexto[22].setString("Fila piedra' activado");
		else vectorTexto[22].setString("'Fila piedra' desactivado");
		tamanioArchivo = archivo.LeerScore(Globales::VerDificultad(),vectorUsuario); /*Llamamos al metodo 'LeerScore' para almacenar
																					en un vector de tipo 'Usuario' el contenido del 
																					archivo que haya que leer, guardando, a su vez, el 
																					tamaño de este.*/
		if(vectorUsuario.size()==0) vectorTexto[20].setString("No se encontro el archivo"); /*Corroboramos que se haya podido leer
																						leer el archivo. Si se pudo, realizamos
																						las acciones siguientes:*/
		else{
			switch(Globales::VerDificultad()){ //Dependiendo la dificultad, seteamos el texto de forma distinta.
			case 0: {vectorTexto[20].setString("Dificultad: Facil"); dificultadAux = 0; break;} /*Guardamos la variable 'dificultadAux'
																							para 'saber' a que otro archivo hay que leer 
																							si el usuario decide cambiar.*/
			case 1: {vectorTexto[20].setString("Dificultad: Media"); dificultadAux = 1; break;}
			default: {vectorTexto[20].setString("Dificultad: Dificil"); dificultadAux = 2; break;}}
			for(int i=0;i<tamanioArchivo;i++){ 
				m_usuario = vectorUsuario[i].nombre; m_puntos = to_string(vectorUsuario[i].scoreUsuario); /*Se guarda el nombre y el
																									puntaje del usuario de 2 variables,
																								para ser pasadas como parametros en la 
																								funcion 'CrearTexto'.*/
				CrearTexto(vectorTexto[i],vectorTexto[i+10], m_usuario, m_puntos);
			}
		}
	}
	//Verificamos si se ha podido cargar la textura y, en caso afirmativo, la seteamos.
	if(!tFlecha.loadFromFile("Imagenes/Flecha.png")) cerr<<"No se pudo abrir la imagen 'Flecha.png'"<<endl;
	else {sFlecha.setTexture(tFlecha); sFlecha.setScale(0.08,0.08); sFlecha.setPosition(-5,14); sFlecha.setColor(Color(0,0,0,128));}
	Globales::CambiarColor(Color(153,255,153));  //Se cambia el color de fondo de la ventana.
}


/*
El metodo 'Actualizar' de la clase 'Puntajes' chequea, sobretodo, el mouse. Si el usuario hace click en la dificultad mostrada,
SE llamara a la funcion 'CliqueoDificultad' (explicada mas debajo) para cambiar los puntajes mostrados a los otros almacenados
en el archivo correspondiente a la dificultad nueva. Si el usuario hace click en la textura con forma de flecha, o si aprieta
la tecla 'Esc', se volvera a la ventana de la clase 'Menu'. 'CambiarArchivo' y 'CliqueoFlecha' se explican mas debajo.
*/
void Puntajes::Actualizar(Juego &juego, RenderWindow &ventana) {
	if(Keyboard::isKeyPressed(Keyboard::Key::Escape)) juego.SetiarEscena(new Menu(ventana)); //Verificamos si se ha apretado 'Escape'.
	if(Mouse::isButtonPressed(Mouse::Left) && reloj.getElapsedTime().asMilliseconds()>150){ /*Verificamos que se haya hecho click
																				izquierdo y que se haya superado el tiempo establecido.*/
		posicionMouse = Mouse::getPosition(ventana); //Obtemos la posicion x-y del mouse.
		if(CliqueoDificultad(posicionMouse)){ //Verificamos que haya hecho click en el texto de dificultad.
			if(dificultadAux == 2) dificultadAux = 0; //Actualizamos la dificultad temporar del archivo visto.
			else dificultadAux = 1 + dificultadAux;
			CambiarArchivo(dificultadAux); //Llamamos al metodo 'CambiarArchivo' para cambiar los puntajes en pantalla.
		}
		if(CliqueoFlecha(posicionMouse)) juego.SetiarEscena(new Menu(ventana)); //Verificamos si hizo click en la textura de la flecha.
		reloj.restart(); //Reiniciamos el reloj.
	}
}

//Esta funcion es la encargada de mostrar en pantalla los textos y texturas seteados.
void Puntajes::Dibujar(RenderWindow &ventana) {
	ventana.clear(Globales::VerColor());
	DibujarTexto(ventana);
	ventana.draw(sFlecha);
	ventana.display();
}

/*
Este metodo simplifica la creacion y ubicacion de los textos mostrados en pantalla, seteando los textos pasados como
parametros. Se setea su fuente, su contenido, su tamaño, y su posicion.
*/
void Puntajes::CrearTexto(Text &textoNombre, Text &textoPunto, string nombre, string score){
	if(fuente.EstadoFuenteOpciones()){ //Verificamos si se ha podidoc cargar la fuente 'Opciones.ttf'.
		textoNombre.setFont(fuente.VerFuenteOpciones()); textoPunto.setFont(fuente.VerFuenteOpciones());
		textoNombre.setCharacterSize(24); textoPunto.setCharacterSize(24);
	}
	else if(fuente.EstadoFuenteTetris()) { //Verificamos si se ha podido cargar la fuente 'Tetris.ttf'.
		textoNombre.setFont(fuente.VerFuenteTetris()); textoPunto.setFont(fuente.VerFuenteTetris());
		textoNombre.setCharacterSize(24); textoPunto.setCharacterSize(24);
	}
	else { //Sino se ha podido cargar la fuente predeterminada, utilizamos la fuente 'Informacion.ttf'.
		textoNombre.setFont(fuente.VerFuenteInformacion()); textoPunto.setFont(fuente.VerFuenteInformacion());
		textoNombre.setCharacterSize(17); textoPunto.setCharacterSize(17);
	}
	textoNombre.setString(nombre); textoPunto.setString(score); 
	textoNombre.setFillColor(Color::Red); textoNombre.setPosition(100,y);
	textoPunto.setFillColor(Color::Red); textoPunto.setPosition(480,y);
	y+=50; //Aumentamos el valor de la posicion vertical de los textos.
}

//Esta funcion dibuja los textos de la ventana de clase 'Puntajes'.
void Puntajes::DibujarTexto(RenderWindow &ventana){
	for(int i=0;i<tamanioArchivo;i++) {
		ventana.draw(vectorTexto[i]);
		ventana.draw(vectorTexto[i+10]);
	}
	ventana.draw(vectorTexto[20]);
	ventana.draw(vectorTexto[21]);
	ventana.draw(vectorTexto[22]);
}

/*
Tanto la funcion 'CliqueoDificultad', como 'CliqueoFlecha', chequean si se ha hecho click en la zona correspondientes. 
Devuelven verdadero si fue asi, o falso si no.
*/
bool Puntajes::CliqueoDificultad(Vector2i posicionMouse){
	/*Se obtiene la posicion left-width-top-height del texto que informa de la dificultad para corroborar la posicion del mouse 
	con respecto a estos valores, de manera de verificar si el usuario a hecho click en el. Misma idea es para la imagen de la
	flecha.*/
	posicionOpcion = vectorTexto[20].getGlobalBounds();
	if(posicionMouse.x>=posicionOpcion.left && posicionMouse.x<=(posicionOpcion.width+posicionOpcion.left)
	&& posicionMouse.y>=posicionOpcion.top && posicionMouse.y<=(posicionOpcion.height+posicionOpcion.top)) return true;
	return false;
}

bool Puntajes::CliqueoFlecha(Vector2i posicionMouse){
	posicionOpcion = sFlecha.getGlobalBounds();
	if(posicionMouse.x>=posicionOpcion.left && posicionMouse.x<=(posicionOpcion.width+posicionOpcion.left)
	&& posicionMouse.y>=posicionOpcion.top && posicionMouse.y<=(posicionOpcion.height+posicionOpcion.top)) return true;
	return false;
}

/*
Este metodo llama nuevamente a la funcion 'LeerScore' de 'Archivo' ya que se desea mostrar los puntajes de la dificultad
siguiente.
*/
void Puntajes::CambiarArchivo(int dificultadAux){
	tamanioArchivo = archivo.LeerScore(dificultadAux,vectorUsuario);
	if(vectorUsuario.size()==0) vectorTexto[20].setString("No se encontro el archivo");
	else{
		switch(dificultadAux){
		case 0: {vectorTexto[20].setString("Dificultad: Facil"); break;}
		case 1: {vectorTexto[20].setString("Dificultad: Media"); break;}
		default: {vectorTexto[20].setString("Dificultad: Dificil"); break;}}
		for(int i=0;i<tamanioArchivo;i++){
			m_usuario = vectorUsuario[i].nombre; m_puntos = to_string(vectorUsuario[i].scoreUsuario);
			vectorTexto[i].setString(m_usuario); vectorTexto[i+10].setString(m_puntos);
		}
	}
}
