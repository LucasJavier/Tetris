#include "Menu.h"
#include "Escalas.h"

Music musicaMenus;

/*
En su constructor definimos el tamaño de la ventana y la velocidad de los fotogramas. Definimos los valores de distintas variables
de tipo 'int' y 'bool', como asi tambien buscamos y, si se encuentra, iniciamos la musica y la fuente que van a utilizar
en esta clase. Las funciones 'CrearTexto', 'CargarTexturas', y 'CrearColores', iniciadas en este constructor, se explican mas debajo.
Se crea una combinacion inicial en los valores de los floatanes 'rojo', 'verde', y 'azul'. Por ultimo, configuramos un vector de 
'RectangleShape', seteando el tamaño y color.
*/
Menu::Menu(RenderWindow &ventana) : Escenas(ventana){
	ventana.create(VideoMode(640,640),"Menu"); ventana.setFramerateLimit(60);
	srand(time(0)); int eleccion = 1+rand()%3; //Obtenemos de manera pseudo-aleatoria un numero para establecer una secuencia RGB.
	y = -130; offset = 26; segundos = 5;
	if(Globales::VerEstado(0)){ //Esstado que se utiliza para realizar las siguientes acciones solo una vez.
		//Chequeamos si se ha cargado o no el archivo musical.
		if(!musicaMenus.openFromFile("Musica/Troika.flac")) cerr<<"No se puedo encontrar el archivo musical 'Troika.flac'"<<endl;
		else {musicaMenus.setVolume(70); musicaMenus.setLoop(true); musicaMenus.play();} /*Si se ha cargado correctamente, seteamos 
																					el volumen, que se reproduzca en loop, y lo 
																					reproducimos.*/
		Globales::CambiarEstado(0,false); //Cambiamos el estado para que lo anterior no se reproduzca mas.
		Globales::CambiarEstado(9,true); //Seteamos la musica para que este activada.
		Globales::CambiarEstado(10,true); //Seteamos los efectos para que esten actavados.
	}
	//Corroboramos si el icono pudo ser cargado.
	if(!icono.loadFromFile("Imagenes/Icono.png")) cerr<<"No se pudo encontrar la imagen 'Icono.png'"<<endl;
	else ventana.setIcon(icono.getSize().x,icono.getSize().y,icono.getPixelsPtr()); //Seteamos el icono de la ventana.
	Globales::CambiarCadena(6,""); //Seteamos la cadena de texto del nombre del usuario.
	switch(eleccion){ //Dependiendo que numero se haya obtenido, generado de manera pseudo-aletoria, se crea la combinacion de colores.
	case 1: {rojo = 255; verde = rand()%255; azul = rand()%255; break;} 
	case 2: {verde = 255; rojo = rand()%255; azul = rand()%255; break;} 
	default: {azul = 255; rojo = rand()%255; verde = rand()%255; break;}}
	if(fuente.EstadoFuenteMenu() || fuente.EstadoFuenteOpciones() || fuente.EstadoFuenteInformacion() || fuente.EstadoFuenteTetris()){
		CrearTexto(); //Si se ha cargado alguna fuente, llamamos al metodo 'CrearTexto'.
		sinFuente = false;
	} else sinFuente = true; //Sino se ha podido cargar ninguna fuente, cambiamos la variable 'sinFuente' a 'true'.
	CargarTexturas();
	CrearColores(Globales::VerEstado(7)); vectorFormas.resize(9); vectorFormas[0].setSize({30,30}); vectorFormas[0].setFillColor(Globales::VerColor(0));
	for(size_t i=1;i<vectorFormas.size();i++) {vectorFormas[i].setSize({24,24}); vectorFormas[i].setFillColor(Globales::VerColor(i));}
	mover = false; cambioDeLado = true; debeCrear = true;
}

/*
La funcion 'Actualizar' de 'Menu' realiza las siguientes acciones:
	1) Se crea una combinacion de colores (mediante el metodo 'CrearCombinacion') para ir cambiando el color de fondo de la ventana. 
	2) Mediante la funcion 'ChequearMouse' (explicada mas debajo), se esta pendiente de la posicion del raton, para 
	realizar variaciones en los tamaños de los textos mostrados en la ventana (si el usuario desliza el mouse por encima de estos),
	con el objetivo de brindar ayuda visual.
	3) Si el usuario realiza un click izquierdo con el mouse, se chequea donde se ha realizo este click para, de esta forma,
	hacer la accion adecuado.
	4) Se mueve verticalmente hacia abajo, mediente la funcion 'MoverForma', la forma creada por
	el metodo 'CrearFroma' (ambos explicados mas abajo). 
*/
void Menu::Actualizar(Juego &juego,RenderWindow &ventana){
	CrearCombinacion(); //Se crea la combinacion de colores.
	//Si alguna fuente se ha cargado:
	if(!sinFuente) ChequearMouse(ventana); //Chequeamos la posicion del mouse en la ventana.
	if(sinFuente){ //Sino se ha cargado ninguna fuente, se pertmite entrar a la partida con la tecla 'Enter'.
		if(Keyboard::isKeyPressed(Keyboard::Key::Return)){
			musicaMenus.stop(); juego.SetiarEscena(new Partida(ventana));
		}
	}
	else{ //Si se ha cargado una fuente:
		if(Mouse::isButtonPressed(Mouse::Left)){ //Se verifica si ha hecho un click izquierdo con el mouse.
			posicionMouse = Mouse::getPosition(ventana);
			/*Las siguientes funciones verifican si se ha hecho click en algunos de los textos/texturas mostrados y, si es asi, 
			realizan distintas acciones.*/
			if(CliqueoJugar(posicionMouse)) {musicaMenus.stop(); juego.SetiarEscena(new Partida(ventana));} /*Paramos la musica y 
																									cambiamos a la ventana de la clase
																									'Partida'*/
			if(CliqueoOpciones(posicionMouse)) juego.SetiarEscena(new Opciones(ventana)); //Cambiamos a la ventana de la clase 'Opciones'.
			if(CliqueoScore(posicionMouse)) juego.SetiarEscena(new Puntajes(ventana)); //Cambiamos a la ventana de la clase 'Puntajes'.
			if(CliqueoSalir(posicionMouse)) {musicaMenus.stop(); ventana.close();} //Paramos la musica y salimos del juego.
			if(CliqueoMusica(posicionMouse) && relojMusica.getElapsedTime().asMilliseconds()>150){
				//Si la musica esta activa, la apagamos.
				if(Globales::VerEstado(9)) {Globales::CambiarEstado(9,false); musicaMenus.stop();}
				//Si la musica esta desactivada, la prendemos.
				else {Globales::CambiarEstado(9,true); musicaMenus.play();}
				relojMusica.restart(); //Reiniciamos el reloj.
			}
			if(CliqueoEfecto(posicionMouse) && relojMusica.getElapsedTime().asMilliseconds()>150){
				//Si los efectos estan activados, los apagamos.
				if(Globales::VerEstado(10)) Globales::CambiarEstado(10,false);
				//Si los efectos estan desactivados, los prendemos.
				else Globales::CambiarEstado(10,true);
				relojMusica.restart(); //Reiniciamos el reloj.
			}
		}
	}
	if(y>640) {y = -130; mover = false; debeCrear = true; reloj.restart();} /*Si se cumple la condicion, se setean las variables
																	vistas, con el objetivo de ver otra forma 'caer'.*/
	if(reloj.getElapsedTime().asSeconds()>segundos && debeCrear){ /*Chequeamos que haya pasado el tiempo establecido y que la variable
																	de tipo bool 'debeCrear' sean los correctos. Si es asi, se crea otra
																	forma y se setea para que se empieze a mover la forma.*/
		CrearForma(); segundos = 7; mover = true; debeCrear = false;
	}
	if(mover) MoverForma(); //Si se cumple la condicion, movemos la forma.
}

//Esta funcion dibuja en pantalla los dintintos textos, texturas, y el color de fondo establecidos.
void Menu::Dibujar(RenderWindow &ventana){
	ventana.clear(Globales::VerColor());
	ventana.draw(sCargarSimbolo);
	if(Globales::VerEstado(9)) ventana.draw(sMusicaPrendida); /*Se verifica el estado de la musica en el juego, para dibujar
																una u otra textura ('MusicaPrendida.png' o 'MusicaApagada.png').*/
	else ventana.draw(sMusicaApagada); 
	if(Globales::VerEstado(10)) ventana.draw(sEfectoPrendido); /*Se verifica el estado de los efectos en el juego, para dibujar 
																una u otra textura ('EfectoPrendido.png' o 'EfectoApagado.png').*/
	else ventana.draw(sEfectoApagado);
	if(!sinFuente){ //Si se ha cargado alguna fuente, se dibujan los textos.
		ventana.draw(jugar);
		ventana.draw(opciones);
		ventana.draw(puntaje);
		ventana.draw(salir);
	}
	DibujarForma(ventana); //Con este metodo se dibujan la forma que se haya creado con el metodo 'CrearForma'.
	ventana.display();
}

/*
Estas 2 funciones que siguen ('CrearTexto' y 'AspectosGeneralesTexto') son las encargadas de crear y setear los diferentes 
aspectos de los textos mostrados.
*/
void Menu::CrearTexto(){
	if(fuente.EstadoFuenteMenu() || fuente.EstadoFuenteOpciones()){
		//Si se ha cargado la fuente 'Opciones.ttf' o 'Menu.ttf'.
		AspectosGeneralesTexto(jugar,"JUGAR",32,Color(Color::Red));
		AspectosGeneralesTexto(opciones,"OPCIONES",32,Color(Color::Red));
		AspectosGeneralesTexto(puntaje,"PUNTUACIONES",32,Color(Color::Red));
		AspectosGeneralesTexto(salir,"SALIR",32,Color(Color::Red)); 	
	}
	else if(fuente.EstadoFuenteInformacion()){ //Si se ha cargado la fuente 'Informacion.ttf'.
		AspectosGeneralesTexto(jugar,"JUGAR",26,Color(Color::Red));
		AspectosGeneralesTexto(opciones,"OPCIONES",26,Color(Color::Red));
		AspectosGeneralesTexto(puntaje,"PUNTUACIONES",26,Color(Color::Red));
		AspectosGeneralesTexto(salir,"SALIR",26,Color(Color::Red));
	}
	else { //Si se ha cargado la fuente 'Tetris.ttf'.
		AspectosGeneralesTexto(jugar,"JUGAR",42,Color(Color::Red));
		AspectosGeneralesTexto(opciones,"OPCIONES",42,Color(Color::Red));
		AspectosGeneralesTexto(puntaje,"PUNTUACIONES",42,Color(Color::Red));
		AspectosGeneralesTexto(salir,"SALIR",42,Color(Color::Red)); 
	}
	jugar.setPosition(250,320); //Setemos la ubicacion. 
	opciones.setPosition(210,400);
	puntaje.setPosition(140,480);
	salir.setPosition(250,560);
}

//Se realiza la configuracion de la fuente del texto, de su contenido, su tamaño, y su color.
void Menu::AspectosGeneralesTexto(Text &texto,string titulo,int tamanio,Color color){
	if(fuente.EstadoFuenteMenu()) texto.setFont(fuente.VerFuenteMenu());
	else if(fuente.EstadoFuenteOpciones()) texto.setFont(fuente.VerFuenteOpciones());
	else if(fuente.EstadoFuenteInformacion()) texto.setFont(fuente.VerFuenteInformacion());
	else texto.setFont(fuente.VerFuenteTetris());
	texto.setString(titulo); texto.setCharacterSize(tamanio); texto.setFillColor(color);
}

//Esta función crea de manera pseudo-aleatoria una combinacion de colores para cambiar el fondo de la ventana.
void Menu::CrearCombinacion(){
	if(rojo!=255) rojo+=0.5;
	else rojo = rand()%255;
	if(verde!=255) verde+=0.5;
	else verde = rand()%255;
	if(azul!=255) azul+=0.5;
	else azul = rand()%255;
	Globales::CambiarColor(Color(rojo,verde,azul)); 
}

/*
Las siguiente 6 funciones ('CliqueoJugar', 'CliqueoOpciones', 'CliqueoScore', 'CliqueoSalir', 'CliqueoMusica' 
'CliqueoEfecto') retornan verdadero si el usuario cliqueo con el mouse en alguna posicion dentro del tamaño del 
texto/textura mostrados.
*/
bool Menu::CliqueoJugar(Vector2i &posicionMouse){
	coordenadasTexto = jugar.getGlobalBounds(); /*Se obtiene la posicion del 'cuadrado/rectangulo' que encierra al texto jugar, 
												con el objetivo de comparar sus proporciones con la posicion del mouse, verificando asi
												si se ha hecho click o no en la posicion del texto. Misma idea se utilizan con las 
												demas funciones 'Cliqueo...'.*/
	if(posicionMouse.x>=coordenadasTexto.left && posicionMouse.x<=(coordenadasTexto.width+coordenadasTexto.left) &&
	posicionMouse.y>=coordenadasTexto.top && posicionMouse.y<=(coordenadasTexto.height+coordenadasTexto.top)) return true;
	return false;
}

bool Menu::CliqueoOpciones(Vector2i &posicionMouse){
	coordenadasTexto = opciones.getGlobalBounds();
	if(posicionMouse.x>=coordenadasTexto.left && posicionMouse.x<=(coordenadasTexto.width+coordenadasTexto.left) &&
	posicionMouse.y>=coordenadasTexto.top && posicionMouse.y<=(coordenadasTexto.height+coordenadasTexto.top)) return true;
	return false;
}

bool Menu::CliqueoScore(Vector2i &posicionMouse){
	coordenadasTexto = puntaje.getGlobalBounds();
	if(posicionMouse.x>=coordenadasTexto.left && posicionMouse.x<=(coordenadasTexto.width+coordenadasTexto.left) &&
	posicionMouse.y>=coordenadasTexto.top && posicionMouse.y<=(coordenadasTexto.height+coordenadasTexto.top)) return true;
	return false;
}

bool Menu::CliqueoSalir(Vector2i &posicionMouse){
	coordenadasTexto = salir.getGlobalBounds();
	if(posicionMouse.x>=coordenadasTexto.left && posicionMouse.x<=(coordenadasTexto.width+coordenadasTexto.left) &&
	posicionMouse.y>=coordenadasTexto.top && posicionMouse.y<=(coordenadasTexto.height+coordenadasTexto.top)) return true;
	return false;
}

bool Menu::CliqueoMusica(Vector2i &posicionMouse){
	if(Globales::VerEstado(9)) coordenadasTexto = sMusicaPrendida.getGlobalBounds();
	else coordenadasTexto = sMusicaApagada.getGlobalBounds();
	if(posicionMouse.x>=coordenadasTexto.left && posicionMouse.x<=(coordenadasTexto.width+coordenadasTexto.left) &&
	posicionMouse.y>=coordenadasTexto.top && posicionMouse.y<=(coordenadasTexto.height+coordenadasTexto.top)) return true;
	return false;
}

bool Menu::CliqueoEfecto(Vector2i &posicionMouse){
	if(Globales::VerEstado(10)) coordenadasTexto = sEfectoPrendido.getGlobalBounds();
	else coordenadasTexto = sEfectoApagado.getGlobalBounds();
	if(posicionMouse.x>=coordenadasTexto.left && posicionMouse.x<=(coordenadasTexto.width+coordenadasTexto.left) &&
	posicionMouse.y>=coordenadasTexto.top && posicionMouse.y<=(coordenadasTexto.height+coordenadasTexto.top)) return true;
	return false;
}

/*
Esta función elige de manera pseudo-aleatoria una forma y una determina rotación de esta (si es que tiene), de manera de crearla 
en alguna posición de la ventana (posicion obtenida tambien de manera pseudo-aleatoria).
*/
void Menu::CrearForma(){
	srand(time(0));	int x,n_rotacion; posicionRectangulo.resize(0); n_forma = 1+rand()%8; //Obtenemos el numero de la forma a crear.
	if(cambioDeLado) {x = 60+rand()%290; cambioDeLado = false;} /*Dependiendo de la variable tipo bool 'cambioDeLado', se crea un
																numero pseudo-aleatorio con rangos distintos, con el objetivo de que
																la forma vaya intercalando los lados de la ventana.*/
	else {x = 240+rand()%290; cambioDeLado = true;}
	switch(n_forma){
	//Cuadrado.
	case 1:{ /*En cada forma y, si tiene, dependiendo de cada rotacion, se guarda la posicion x-y de los 4 'RectangleShape', para ser 
			usados despues a la hora de dibujarlos.*/
		posicionRectangulo.push_back({x,y}); posicionRectangulo.push_back({x+offset,y});
		posicionRectangulo.push_back({x,y+offset}); posicionRectangulo.push_back({x+offset,y+offset});
		break;
	}
	case 2:{ //EleIzq.
		n_rotacion = 1+rand()%4;
			switch(n_rotacion){
			case 1:{
				posicionRectangulo.push_back({x,y}); posicionRectangulo.push_back({x,y+offset});
				posicionRectangulo.push_back({x+offset,y+offset}); posicionRectangulo.push_back({x+(offset*2),y+offset});
				break;
			}
			case 2:{
				posicionRectangulo.push_back({x,y}); posicionRectangulo.push_back({x,y+offset});
				posicionRectangulo.push_back({x,y+(offset*2)}); posicionRectangulo.push_back({x-offset,y+(offset*2)});
				break;
			}
			case 3:{
				posicionRectangulo.push_back({x,y}); posicionRectangulo.push_back({x+offset,y});
				posicionRectangulo.push_back({x+(offset*2),y}); posicionRectangulo.push_back({x+(offset*2),y+offset});
				break;
			}
			default:{
				posicionRectangulo.push_back({x,y}); posicionRectangulo.push_back({x-offset,y});
				posicionRectangulo.push_back({x-offset,y+offset}); posicionRectangulo.push_back({x-offset,y+(offset*2)});
				break;
			}
			}
		break;
	}
	case 3:{ //EleDer.
		n_rotacion = 1+rand()%4;
			switch(n_rotacion){
			case 1:{
				posicionRectangulo.push_back({x,y}); posicionRectangulo.push_back({x,y+offset});
				posicionRectangulo.push_back({x-offset,y+offset}); posicionRectangulo.push_back({x-(offset*2),y+offset});
				break;
			}
			case 2:{
				posicionRectangulo.push_back({x,y}); posicionRectangulo.push_back({x+offset,y});
				posicionRectangulo.push_back({x+offset,y+offset}); posicionRectangulo.push_back({x+offset,y+(offset*2)});
				break;
			}
			case 3:{
				posicionRectangulo.push_back({x,y}); posicionRectangulo.push_back({x-offset,y});
				posicionRectangulo.push_back({x-(offset*2),y}); posicionRectangulo.push_back({x-(offset*2),y+offset});
				break;
			}
			default:{
				posicionRectangulo.push_back({x,y}); posicionRectangulo.push_back({x,y+offset});
				posicionRectangulo.push_back({x,y+(offset*2)}); posicionRectangulo.push_back({x+offset,y+(offset*2)});
				break;
			}
			}
		break;
	}
	case 4:{ //Linea.
		n_rotacion = 1+rand()%2;
			switch(n_rotacion){
			case 1:{
				posicionRectangulo.push_back({x,y}); posicionRectangulo.push_back({x,y+offset});
				posicionRectangulo.push_back({x,y+(offset*2)}); posicionRectangulo.push_back({x,y+(offset*3)});
				break;
			}
			default:{
				posicionRectangulo.push_back({x,y}); posicionRectangulo.push_back({x+offset,y});
				posicionRectangulo.push_back({x+(offset*2),y}); posicionRectangulo.push_back({x+(offset*3),y});
				break;
			}
			}
		break;
	}
	case 5:{ //EseIzq.
		n_rotacion = 1+rand()%2;
			switch(n_rotacion){
			case 1:{
				posicionRectangulo.push_back({x,y}); posicionRectangulo.push_back({x+offset,y});
				posicionRectangulo.push_back({x+offset,y+offset}); posicionRectangulo.push_back({x+(offset*2),y+offset});
				break;
			}
			default:{
				posicionRectangulo.push_back({x,y}); posicionRectangulo.push_back({x,y+offset});
				posicionRectangulo.push_back({x-offset,y+offset}); posicionRectangulo.push_back({x-offset,y+(offset*2)});
				break;
			}
			}
		break;
	}
	case 6:{ //EseDer.
		n_rotacion = 1+rand()%2;
			switch(n_rotacion){
			case 1:{
				posicionRectangulo.push_back({x,y}); posicionRectangulo.push_back({x-offset,y});
				posicionRectangulo.push_back({x-offset,y+offset}); posicionRectangulo.push_back({x-(offset*2),y+offset});
				break;
			}
			default:{
				posicionRectangulo.push_back({x,y}); posicionRectangulo.push_back({x,y+offset});
				posicionRectangulo.push_back({x+offset,y+offset}); posicionRectangulo.push_back({x+offset,y+(offset*2)});
				break;
			}
			}
		break;
	}
	default:{ //Te.
		n_rotacion = 1+rand()%4;
			switch(n_rotacion){
			case 1:{
				posicionRectangulo.push_back({x,y}); posicionRectangulo.push_back({x+offset,y});
				posicionRectangulo.push_back({x+(offset*2),y}); posicionRectangulo.push_back({x+offset,y+offset});
				break;
			}
			case 2:{
				posicionRectangulo.push_back({x,y}); posicionRectangulo.push_back({x,y+offset});
				posicionRectangulo.push_back({x+offset,y+offset}); posicionRectangulo.push_back({x,y+(offset*2)});
				break;
			}
			case 3:{
				posicionRectangulo.push_back({x,y}); posicionRectangulo.push_back({x-offset,y+offset});
				posicionRectangulo.push_back({x,y+offset}); posicionRectangulo.push_back({x+offset,y+offset});
				break;
			}
			default:{
				posicionRectangulo.push_back({x,y}); posicionRectangulo.push_back({x-offset,y+offset});
				posicionRectangulo.push_back({x,y+offset}); posicionRectangulo.push_back({x,y+(offset*2)});
				break;
			}
			}
		break;
	}	
	}
}

//Esta función mueve la forma creada verticalmente hacia 'abajo', da manera de brindar ilusión de caída.
void Menu::MoverForma(){;
	for(size_t i=0;i<posicionRectangulo.size();i++) posicionRectangulo[i].y = posicionRectangulo[i].y + 1;
	y++; //Posicion vertical de los RectangleShape.
}

//Esta funcion posiciona y dibuja la forma creada en la ventana.
void Menu::DibujarForma(RenderWindow &ventana){
	for(size_t i=0;i<posicionRectangulo.size();i++) {
		vectorFormas[0].setPosition(posicionRectangulo[i].x,posicionRectangulo[i].y); ventana.draw(vectorFormas[0]);
		vectorFormas[n_forma].setPosition(posicionRectangulo[i].x+3,posicionRectangulo[i].y+3); ventana.draw(vectorFormas[n_forma]);
	}
}

/*
Esta funcion chequea si el usuario paso el mouse por alguna de las opciones permitidas, de manera de brindar ayuda visual
(aumentado el tamaño de los textos), y darle a entender a este que funciona.
*/
void Menu::ChequearMouse(RenderWindow &ventana){
	posicionMouse = Mouse::getPosition(ventana);
	//Se hace uso de las funciones antes creadas ('CliqueoJugar', 'Cliqueo...').
	if(CliqueoJugar(posicionMouse)) { //Si entra, aumentamos el tamaño y ajustamos la posicion.
		if(fuente.EstadoFuenteMenu() || fuente.EstadoFuenteOpciones()) jugar.setCharacterSize(38); /*Si se cargo la fuente
																									'Opciones.ttf' o 'Menu.ttf'.*/
		else if(fuente.EstadoFuenteInformacion()) jugar.setCharacterSize(30); //Si se cargo la fuente 'Informacion.ttf'.
		else jugar.setCharacterSize(46);  //Si se cargo la fuente 'Tetris.ttf'.
		jugar.setPosition(235,320);
	}
	else { //Sino entra, devolvemos a su posicion y tamaño original.
		if(fuente.EstadoFuenteMenu() || fuente.EstadoFuenteOpciones()) jugar.setCharacterSize(32);
		else if(fuente.EstadoFuenteInformacion()) jugar.setCharacterSize(26);
		else jugar.setCharacterSize(42);
		jugar.setPosition(250,320);
	}
	if(CliqueoOpciones(posicionMouse)) {
		if(fuente.EstadoFuenteMenu() || fuente.EstadoFuenteOpciones()) opciones.setCharacterSize(38); /*Si se cargo la fuente
																									'Opciones.ttf' o 'Menu.ttf'.*/
		else if(fuente.EstadoFuenteInformacion()) opciones.setCharacterSize(30); //Si se cargo la fuente 'Informacion.ttf'.
		else opciones.setCharacterSize(46);  //Si se cargo la fuente 'Tetris.ttf'.
		opciones.setPosition(190,400);
	}
	else {
		if(fuente.EstadoFuenteMenu() || fuente.EstadoFuenteOpciones()) opciones.setCharacterSize(32);
		else if(fuente.EstadoFuenteInformacion()) opciones.setCharacterSize(26);
		else opciones.setCharacterSize(42);
		opciones.setPosition(210,400);
	}
	if(CliqueoScore(posicionMouse)) {
		if(fuente.EstadoFuenteMenu() || fuente.EstadoFuenteOpciones()) puntaje.setCharacterSize(38); /*Si se cargo la fuente
																									'Opciones.ttf' o 'Menu.ttf'.*/
		else if(fuente.EstadoFuenteInformacion()) puntaje.setCharacterSize(30); //Si se cargo la fuente 'Informacion.ttf'.
		else puntaje.setCharacterSize(46);  //Si se cargo la fuente 'Tetris.ttf'. 
		puntaje.setPosition(110,480);
	}
	else {
		if(fuente.EstadoFuenteMenu() || fuente.EstadoFuenteOpciones()) puntaje.setCharacterSize(32);
		else if(fuente.EstadoFuenteInformacion()) puntaje.setCharacterSize(26);
		else puntaje.setCharacterSize(42);
		puntaje.setPosition(140,480);
	}
	if(CliqueoSalir(posicionMouse)) {
		if(fuente.EstadoFuenteMenu() || fuente.EstadoFuenteOpciones()) salir.setCharacterSize(38); /*Si se cargo la fuente
																									'Opciones.ttf' o 'Menu.ttf'.*/
		else if(fuente.EstadoFuenteInformacion()) salir.setCharacterSize(30); //Si se cargo la fuente 'Informacion.ttf'.
		else salir.setCharacterSize(46);  //Si se cargo la fuente 'Tetris.ttf'.
		salir.setPosition(235,560);
	}
	else {
		if(fuente.EstadoFuenteMenu() || fuente.EstadoFuenteOpciones()) salir.setCharacterSize(32);
		else if(fuente.EstadoFuenteInformacion()) salir.setCharacterSize(26);
		else salir.setCharacterSize(42);
		salir.setPosition(250,560);
	}
}

//Esta funcion cambia los colores de las formas, dependiendo de que opcion haya elegido el usuario
void Menu::CrearColores(bool colorNormal){
	Globales::CambiarColor(0,Color(96,96,96));//gris
	if(colorNormal){
		Globales::CambiarColor(1,Color(153,255,51));//verde
		Globales::CambiarColor(2,Color(255,153,51));//naranja
		Globales::CambiarColor(3,Color(51,255,255));//celeste 
		Globales::CambiarColor(4,Color(255,130,255));//rosado
		Globales::CambiarColor(5,Color(255,73,73));//rojo
		Globales::CambiarColor(6,Color(81,81,255));//azul;
		Globales::CambiarColor(7,Color(255,255,83));//amarillo
	}else{
		Globales::CambiarColor(1,Color(rand()%255,rand()%255,rand()%255));
		Globales::CambiarColor(2,Color(rand()%255,rand()%255,rand()%255));
		Globales::CambiarColor(3,Color(rand()%255,rand()%255,rand()%255));
		Globales::CambiarColor(4,Color(rand()%255,rand()%255,rand()%255));
		Globales::CambiarColor(5,Color(rand()%255,rand()%255,rand()%255));
		Globales::CambiarColor(6,Color(rand()%255,rand()%255,rand()%255));
		Globales::CambiarColor(7,Color(rand()%255,rand()%255,rand()%255));
	}
}

//Esta funcion es la encargada de setear, si son encontradas, las texturas buscadas.
void Menu::CargarTexturas(){
	//Se setean en todos los casos, para los distintos Sprites, la textura, su escala, y su posicion.
	if(!tTetrisSimbolo.loadFromFile("Imagenes/Simbolo Tetris.png")) cerr<<"No se pudo abrir la imagen 'Simbolo Tetris.png'"<<endl;
	else {sCargarSimbolo.setTexture(tTetrisSimbolo); sCargarSimbolo.setScale(0.5,0.5); sCargarSimbolo.setPosition(170,50);}
	if(!tMusicaPrendida.loadFromFile("Imagenes/MusicaPrendida.png")) cerr<<"No se pudo abrir la imagen 'MusicaPrendida.png'"<<endl;
	else {sMusicaPrendida.setTexture(tMusicaPrendida); sMusicaPrendida.setScale(0.15,0.15) ;sMusicaPrendida.setPosition(500,30);}
	if(!tEfectoPrendido.loadFromFile("Imagenes/EfectoPrendido.png")) cerr<<"No se pudo abrir la imagen 'EfectoPrendido.png'"<<endl;
	else {sEfectoPrendido.setTexture(tEfectoPrendido); sEfectoPrendido.setScale(0.09,0.09); sEfectoPrendido.setPosition(570,25);}
	if(!tMusicaApagada.loadFromFile("Imagenes/MusicaApagada.png")) cerr<<"No se pudo abrir la imagen 'MusicaApagada.png'"<<endl;
	else {sMusicaApagada.setTexture(tMusicaApagada); sMusicaApagada.setScale(0.09,0.09); sMusicaApagada.setPosition(510,30);}
	if(!tEfectoApagado.loadFromFile("Imagenes/EfectoApagado.png")) cerr<<"No se pudo abrir la imagen 'EfectoApagado'"<<endl;
	else {sEfectoApagado.setTexture(tEfectoApagado); sEfectoApagado.setScale(0.2,0.2); sEfectoApagado.setPosition(545,20);}
}
