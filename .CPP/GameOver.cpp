#include "GameOver.h"
#include "Menu.h"

/*
Su constructor setea las dimensiones y la velocidad de fotogramos de la ventana. Carga a su vez, si encuentra, las distintas
fuentes a utilizarse en los textos. Asimismo setea distintas variables de tipo 'int' y 'bool'. Tambien, carga la musica que se
reproducira en esta ventana. Esta dependera si el usuario ha superado o no algun puntaje (como tambien cambian los textos mostrados
en pantalla). EL metodo 'CrearTexto' se explicara mas debajo.
*/
GameOver::GameOver(RenderWindow &ventana,int score) : Escenas(ventana), m_score(score){
	ventana.create(VideoMode(640,640),"Game Over"); ventana.setFramerateLimit(60);
	tamanioArchivo = 0; scoreArchivo = 0;
	posicionEscribir = archivo.LeerHayMejorPuntaje(Globales::VerDificultad(),tamanioArchivo,m_score,scoreArchivo); /*Se llama al metodo
																								'LeerHayMejorPuntaje' para buscar y
																							corroborar si el usuario ha superado 
																						algun puntaje de los almacenados.*/
	//Verifica si alguna fuente pudo ser cargada.
	if(fuente.EstadoFuenteMenu() || fuente.EstadoFuenteTetris() || fuente.EstadoFuenteOpciones() || fuente.EstadoFuenteInformacion()){
		y = 140; vectorTexto.resize(9); CrearTexto(); 
	}
	if(posicionEscribir>=0) { //Verifica si el usuario ha superado algun puntaje.
		if(Globales::VerEstado(9)){ //Verifica si la musica esta activada.
			//Verifica si el archivo musical pudo cargarse con normalidad.
			if(!musica.openFromFile("Musica/Celebration.flac")) cerr<<"No se pudo abrir el archivo musical 'Celebration.flac'"<<endl;
			else {musica.setVolume(50); musica.setLoop(true); musica.play();} //Seteamos el volumen; ponemos en modo loop; y reproducimos.
		}
	}
	else if(Globales::VerEstado(9)){
		if(!musica.openFromFile("Musica/Loginska.flac")) cerr<<"No se pudo abrir el archivo musical 'Loginska.flac'"<<endl;
		else {musica.setVolume(40); musica.setLoop(true); musica.play();}
	}
	//Corroboramos si el icono pudo ser cargado.
	if(!icono.loadFromFile("Imagenes/Icono.png")) cerr<<"No se pudo encontrar la imagen 'Icono.png'"<<endl;
	else ventana.setIcon(icono.getSize().x,icono.getSize().y,icono.getPixelsPtr()); //Seteamos el icono de la ventana.
	int eleccion = 1+rand()%3; //De manera pseudo-aleatoria obtenemos un numero que servira para obtener una combinacion RGB inicial.
	switch(eleccion){
	case 1: {rojo = 255; verde = rand()%255; azul = rand()%255; break;} 
	case 2: {verde = 255; rojo = rand()%255; azul = rand()%255; break;} 
	default: {azul = 255; rojo = rand()%255; verde = rand()%255; break;}}
}

/*
La funcion 'Actualizar' de la clase 'GameOver' modifica el color del texto de "Game Over" y "Felicitacion" con el metodo
'CrearCombinacion' (explicado mas debajo). Asimismo, si se ha superado algun puntaje, y se ha apretado 'Enter', cambia el
texto informativo para el usuario. Modifica la posicion del nombre/alis escrito por el usuario para dejarlo centrado y, por
ultimo, una vez que el usuario teclea 'Enter', llama a la funcion 'EscribirPuntaje' (explicado mas debajo).
*/
void GameOver::Actualizar(Juego &juego, RenderWindow &ventana) {
	//Verificamos que el usuario haya apretado 'Escape'. 
	if(Keyboard::isKeyPressed(Keyboard::Key::Escape)) {
		if(Globales::VerEstado(9)) musica.stop(); //Si la musica esta activada, paramos el archivo que se este reproduciendo.
		Globales::CambiarEstado(0,true);  //Cambiamos el estado utilizado en la clase 'Menu'.
		juego.SetiarEscena(new Menu(ventana)); //Seteamos la nueva escena a la clase 'Menu'.
	}
	//Creamos la combinacion de colores y seteamos el color de los textos indicados.
	CrearCombinacion(); vectorTexto[0].setFillColor(Globales::VerColor()); vectorTexto[5].setFillColor(Globales::VerColor());
	if(posicionEscribir>=0){ //Verificamos si el usuario ha superado algun puntaje.
		if(!Globales::VerEstado(6) && !dejarDeEntrar){ //Verificamos que el usuario haya apretado 'Enter' para guardar su nombre.
			AspectosGeneralesTexto(vectorTexto[8],fuente.VerFuenteInformacion(),"Precione Escape para volver al Menu",false,8,Color::White);
			vectorTexto[8].setPosition(140,590); dejarDeEntrar = true; //Cambiamos el estado para que estas acciones no se repitan mas.
		}
		//Mientras el largo de la cadena sea mayor a 1, y mientras no haya apretado 'Enter', centramos lo tecleado por el usuario.
		if(Globales::VerCadena(6).length()>1 && Globales::VerEstado(6)) Globales::VerNombreUsuario().setPosition(320-(Globales::VerCadena(6).length()*13),520);
		//Cuando el usuario aprete 'Enter' (para guardar su alias/nombre) guardamos la informacion llamando al metodo 'EscribirPuntaje'.
		if(Globales::VerEstado(0)) EscribirPuntaje(m_score,posicionEscribir);
	} 
}

//Esta funcion dibuja y muestra en la ventana los distintos textos creados.
void GameOver::Dibujar(RenderWindow &ventana) {
	ventana.clear(Color::Black);
	for(size_t i=0;i<vectorTexto.size();i++) ventana.draw(vectorTexto[i]);
	ventana.draw(Globales::VerNombreUsuario());
	ventana.display();
}

/*
Este metodo junto a la funcion 'AspectosGeneralesTexto' simplifican la creacion de los textos mostrados en pantalla, mediante
el seteo 5 metodos de la clase Text ('setFont', 'setString', 'setCharacterSize', 'setFillColor', 'setPosition').
*/
void GameOver::CrearTexto(){
	palabraScore = to_string(m_score); //Cambiamos el puntaje del usuario de tipo 'int' a 'string'.
	if(fuente.EstadoFuenteMenu() && fuente.EstadoFuenteTetris() && fuente.EstadoFuenteInformacion()){ //Corrroboramos las fuentes cargadas.
		AspectosGeneralesTexto(vectorTexto[0],fuente.VerFuenteMenu(),"Game Over",true,42,Color::White); 
		AspectosGeneralesTexto(vectorTexto[1],fuente.VerFuenteTetris(),"Mejor puntuacion:",true,28,Color::Red);
		//Corroboramos si el usuario ha superado el mejor puntaje posible.
		if(posicionEscribir==0) AspectosGeneralesTexto(vectorTexto[2],fuente.VerFuenteTetris(),palabraScore,false,28,Color::White);
		else{
			m_scoreArchivo = to_string(scoreArchivo);//Convertimos la mejor puntacion del momento de tipo 'int' a 'string'.
			AspectosGeneralesTexto(vectorTexto[2],fuente.VerFuenteTetris(),m_scoreArchivo,false,28,Color::White);
		}
		AspectosGeneralesTexto(vectorTexto[3],fuente.VerFuenteTetris(),"Tu puntuacion:",true,28,Color::Red);
		AspectosGeneralesTexto(vectorTexto[4],fuente.VerFuenteTetris(),palabraScore,false,28,Color::White);
		if(posicionEscribir>=0) { //Verificamos si el usuario ha superado algun puntaje.
			Globales::CambiarEstado(6,true); //Cambiamos el estado para que la ventana este atento a la entrada de texto del usuario.
			nJugador = to_string(posicionEscribir+1); //Convertimos la posicion superada por el usuario de tipo 'int' a 'string'.
			//Corroboramos la fuente cargada.
			if(fuente.EstadoFuenteOpciones()) AspectosGeneralesTexto(Globales::VerNombreUsuario(),fuente.VerFuenteOpciones(),"",true,32,Color::Green);
			else AspectosGeneralesTexto(Globales::VerNombreUsuario(),fuente.VerFuenteInformacion(),"",true,22,Color::Green);
			AspectosGeneralesTexto(vectorTexto[5],fuente.VerFuenteTetris(),"Felicitaciones!!!",false,36,Color::White);
			AspectosGeneralesTexto(vectorTexto[6],fuente.VerFuenteInformacion(),"Has superado al Jugador numero "+nJugador+"!",false,14,Color::White);
			AspectosGeneralesTexto(vectorTexto[7],fuente.VerFuenteInformacion(),"Por favor, teclea tu nombre:",false,15,Color::White); 
			AspectosGeneralesTexto(vectorTexto[8],fuente.VerFuenteInformacion(),"Presione Enter para guardar tu nombre",false,8,Color::White);
			Globales::VerNombreUsuario().setPosition(320,520); 
			vectorTexto[5].setPosition(180,300); 
			vectorTexto[6].setPosition(40,400);
			vectorTexto[7].setPosition(70,460);
		}
		else AspectosGeneralesTexto(vectorTexto[8],fuente.VerFuenteInformacion(),"Presione Escape para volver al Menu",false,8,Color::White);
		vectorTexto[0].setPosition(130,50);
		vectorTexto[8].setPosition(140,590);
	}
	else if(fuente.EstadoFuenteMenu() && fuente.EstadoFuenteTetris()){
		AspectosGeneralesTexto(vectorTexto[0],fuente.VerFuenteMenu(),"Game Over",true,42,Color::White);
		AspectosGeneralesTexto(vectorTexto[1],fuente.VerFuenteTetris(),"Mejor puntuacion:",true,28,Color::Red); 
		if(posicionEscribir==0) AspectosGeneralesTexto(vectorTexto[2],fuente.VerFuenteTetris(),palabraScore,false,28,Color::White);
		else{
			m_scoreArchivo = to_string(scoreArchivo);//Mejor puntuacion.
			AspectosGeneralesTexto(vectorTexto[2],fuente.VerFuenteTetris(),m_scoreArchivo,false,28,Color::White);
		}
		AspectosGeneralesTexto(vectorTexto[3],fuente.VerFuenteTetris(),"Tu puntuacion:",true,28,Color::Red);
		AspectosGeneralesTexto(vectorTexto[4],fuente.VerFuenteTetris(),palabraScore,false,28,Color::White);
		if(posicionEscribir>=0) {
			Globales::CambiarEstado(6,true);
			nJugador = to_string(posicionEscribir+1);
			if(fuente.EstadoFuenteOpciones()) AspectosGeneralesTexto(Globales::VerNombreUsuario(),fuente.VerFuenteOpciones(),"",true,32,Color::Green);
			else AspectosGeneralesTexto(Globales::VerNombreUsuario(),fuente.VerFuenteTetris(),"",true,46,Color::Green);
			AspectosGeneralesTexto(vectorTexto[5],fuente.VerFuenteTetris(),"Felicitaciones!!!",false,36,Color::White);
			AspectosGeneralesTexto(vectorTexto[6],fuente.VerFuenteTetris(),"Has superado al Jugador numero "+nJugador+"!",false,25,Color::White);
			AspectosGeneralesTexto(vectorTexto[7],fuente.VerFuenteTetris(),"Por favor, teclea tu nombre:",false,25,Color::White); 
			AspectosGeneralesTexto(vectorTexto[8],fuente.VerFuenteTetris(),"Presione Enter para guardar tu nombre",false,16,Color::White);
			Globales::VerNombreUsuario().setPosition(320,520); 
			vectorTexto[5].setPosition(180,300); 
			vectorTexto[6].setPosition(90,380);
			vectorTexto[7].setPosition(140,440);
		}
		else AspectosGeneralesTexto(vectorTexto[8],fuente.VerFuenteTetris(),"Presione Escape para volver al Menu",false,8,Color::White);
		vectorTexto[0].setPosition(130,50);
		vectorTexto[8].setPosition(140,590);
	}
	else if(fuente.EstadoFuenteMenu() && fuente.EstadoFuenteInformacion()){
		AspectosGeneralesTexto(vectorTexto[0],fuente.VerFuenteMenu(),"Game Over",true,42,Color::White);
		AspectosGeneralesTexto(vectorTexto[1],fuente.VerFuenteInformacion(),"Mejor puntuacion:",true,14,Color::Red); 
		if(posicionEscribir==0) AspectosGeneralesTexto(vectorTexto[2],fuente.VerFuenteInformacion(),palabraScore,false,14,Color::White);
		else{
			m_scoreArchivo = to_string(scoreArchivo);//Mejor puntuacion.
			AspectosGeneralesTexto(vectorTexto[2],fuente.VerFuenteInformacion(),m_scoreArchivo,false,14,Color::White);
		}
		AspectosGeneralesTexto(vectorTexto[3],fuente.VerFuenteInformacion(),"Tu puntuacion:",true,14,Color::Red);
		AspectosGeneralesTexto(vectorTexto[4],fuente.VerFuenteInformacion(),palabraScore,false,14,Color::White);
		if(posicionEscribir>=0) {
			Globales::CambiarEstado(6,true);
			nJugador = to_string(posicionEscribir+1);
			if(fuente.EstadoFuenteOpciones()) AspectosGeneralesTexto(Globales::VerNombreUsuario(),fuente.VerFuenteOpciones(),"",true,32,Color::Green);
			else AspectosGeneralesTexto(Globales::VerNombreUsuario(),fuente.VerFuenteInformacion(),"",true,22,Color::Green);
			AspectosGeneralesTexto(vectorTexto[5],fuente.VerFuenteInformacion(),"Felicitaciones!!!",false,24,Color::White);
			AspectosGeneralesTexto(vectorTexto[6],fuente.VerFuenteInformacion(),"Has superado al Jugador numero "+nJugador+"!",false,14,Color::White);
			AspectosGeneralesTexto(vectorTexto[7],fuente.VerFuenteInformacion(),"Por favor, teclea tu nombre:",false,15,Color::White); 
			AspectosGeneralesTexto(vectorTexto[8],fuente.VerFuenteInformacion(),"Presione Enter para guardar tu nombre",false,8,Color::White);
			Globales::VerNombreUsuario().setPosition(320,520); 
			vectorTexto[5].setPosition(120,300); 
			vectorTexto[6].setPosition(40,400);
			vectorTexto[7].setPosition(70,460);
		}
		else AspectosGeneralesTexto(vectorTexto[8],fuente.VerFuenteInformacion(),"Presione Escape para volver al Menu",false,8,Color::White);
		vectorTexto[0].setPosition(130,50);
		vectorTexto[8].setPosition(140,590);
	}
	else if(fuente.EstadoFuenteTetris() && fuente.EstadoFuenteInformacion()){
		AspectosGeneralesTexto(vectorTexto[0],fuente.VerFuenteTetris(),"Game Over",true,54,Color::White);
		AspectosGeneralesTexto(vectorTexto[1],fuente.VerFuenteTetris(),"Mejor puntuacion:",true,28,Color::Red); 
		if(posicionEscribir==0) AspectosGeneralesTexto(vectorTexto[2],fuente.VerFuenteTetris(),palabraScore,false,28,Color::White);
		else{
			m_scoreArchivo = to_string(scoreArchivo);//Mejor puntuacion.
			AspectosGeneralesTexto(vectorTexto[2],fuente.VerFuenteTetris(),m_scoreArchivo,false,28,Color::White);
		}
		AspectosGeneralesTexto(vectorTexto[3],fuente.VerFuenteTetris(),"Tu puntuacion:",true,28,Color::Red);
		AspectosGeneralesTexto(vectorTexto[4],fuente.VerFuenteTetris(),palabraScore,false,28,Color::White);
		if(posicionEscribir>=0) {
			Globales::CambiarEstado(6,true); 
			nJugador = to_string(posicionEscribir+1);
			if(fuente.EstadoFuenteOpciones()) AspectosGeneralesTexto(Globales::VerNombreUsuario(),fuente.VerFuenteOpciones(),"",true,32,Color::Green);
			else AspectosGeneralesTexto(Globales::VerNombreUsuario(),fuente.VerFuenteInformacion(),"",true,22,Color::Green); 
			AspectosGeneralesTexto(vectorTexto[5],fuente.VerFuenteTetris(),"Felicitaciones!!!",false,36,Color::White); 
			AspectosGeneralesTexto(vectorTexto[6],fuente.VerFuenteInformacion(),"Has superado al Jugador numero "+nJugador+"!",false,14,Color::White);
			AspectosGeneralesTexto(vectorTexto[7],fuente.VerFuenteInformacion(),"Por favor, teclea tu nombre:",false,15,Color::White); 
			AspectosGeneralesTexto(vectorTexto[8],fuente.VerFuenteInformacion(),"Presione Enter para guardar tu nombre",false,8,Color::White);
			Globales::VerNombreUsuario().setPosition(320,520); 
			vectorTexto[5].setPosition(180,300); 
			vectorTexto[6].setPosition(40,400);
			vectorTexto[7].setPosition(70,460);
		}
		else AspectosGeneralesTexto(vectorTexto[8],fuente.VerFuenteInformacion(),"Presione Escape para volver al Menu",false,8,Color::White);
		vectorTexto[0].setPosition(130,50);
		vectorTexto[8].setPosition(140,590);
	}
	else if(fuente.EstadoFuenteInformacion()){
		AspectosGeneralesTexto(vectorTexto[0],fuente.VerFuenteInformacion(),"Game Over",true,32,Color::White);
		AspectosGeneralesTexto(vectorTexto[1],fuente.VerFuenteInformacion(),"Mejor puntuacion:",true,14,Color::Red); 
		if(posicionEscribir==0) AspectosGeneralesTexto(vectorTexto[2],fuente.VerFuenteInformacion(),palabraScore,false,14,Color::White);
		else{
			m_scoreArchivo = to_string(scoreArchivo);//Mejor puntuacion.
			AspectosGeneralesTexto(vectorTexto[2],fuente.VerFuenteInformacion(),m_scoreArchivo,false,14,Color::White);
		}
		AspectosGeneralesTexto(vectorTexto[3],fuente.VerFuenteInformacion(),"Tu puntuacion:",true,14,Color::Red);
		AspectosGeneralesTexto(vectorTexto[4],fuente.VerFuenteInformacion(),palabraScore,false,14,Color::White);
		if(posicionEscribir>=0) {
			Globales::CambiarEstado(6,true); 
			nJugador = to_string(posicionEscribir+1);
			if(fuente.EstadoFuenteOpciones()) AspectosGeneralesTexto(Globales::VerNombreUsuario(),fuente.VerFuenteOpciones(),"",true,32,Color::Green);
			else AspectosGeneralesTexto(Globales::VerNombreUsuario(),fuente.VerFuenteInformacion(),"",true,22,Color::Green);
			AspectosGeneralesTexto(vectorTexto[5],fuente.VerFuenteInformacion(),"Felicitaciones!!!",false,24,Color::White);
			AspectosGeneralesTexto(vectorTexto[6],fuente.VerFuenteInformacion(),"Has superado al Jugador numero "+nJugador+"!",false,14,Color::White);
			AspectosGeneralesTexto(vectorTexto[7],fuente.VerFuenteInformacion(),"Por favor, teclea tu nombre:",false,15,Color::White); 
			AspectosGeneralesTexto(vectorTexto[8],fuente.VerFuenteInformacion(),"Presione Enter para guardar tu nombre",false,8,Color::White);
			Globales::VerNombreUsuario().setPosition(320,520); 
			vectorTexto[5].setPosition(120,300); 
			vectorTexto[6].setPosition(40,400);
			vectorTexto[7].setPosition(70,460);
		}
		else AspectosGeneralesTexto(vectorTexto[8],fuente.VerFuenteInformacion(),"Presione Escape para volver al Menu",false,8,Color::White);
		vectorTexto[0].setPosition(130,50);
		vectorTexto[8].setPosition(140,590);
	}
	else if(fuente.EstadoFuenteTetris()){
		AspectosGeneralesTexto(vectorTexto[0],fuente.VerFuenteTetris(),"Game Over",true,50,Color::White);
		AspectosGeneralesTexto(vectorTexto[1],fuente.VerFuenteTetris(),"Mejor puntuacion:",true,28,Color::Red); 
		if(posicionEscribir==0) AspectosGeneralesTexto(vectorTexto[2],fuente.VerFuenteTetris(),palabraScore,false,28,Color::White);
		else{
			m_scoreArchivo = to_string(scoreArchivo);//Mejor puntuacion.
			AspectosGeneralesTexto(vectorTexto[2],fuente.VerFuenteTetris(),m_scoreArchivo,false,28,Color::White);
		}
		AspectosGeneralesTexto(vectorTexto[3],fuente.VerFuenteTetris(),"Tu puntuacion:",true,28,Color::Red);
		AspectosGeneralesTexto(vectorTexto[4],fuente.VerFuenteTetris(),palabraScore,false,28,Color::White);
		if(posicionEscribir>=0) {
			Globales::CambiarEstado(6,true);
			nJugador = to_string(posicionEscribir+1);
			if(fuente.EstadoFuenteOpciones()) AspectosGeneralesTexto(Globales::VerNombreUsuario(),fuente.VerFuenteOpciones(),"",true,32,Color::Green);
			else AspectosGeneralesTexto(Globales::VerNombreUsuario(),fuente.VerFuenteTetris(),"",true,46,Color::Green);
			AspectosGeneralesTexto(vectorTexto[5],fuente.VerFuenteTetris(),"Felicitaciones!!!",false,36,Color::White);
			AspectosGeneralesTexto(vectorTexto[6],fuente.VerFuenteTetris(),"Has superado al Jugador numero "+nJugador+"!",false,25,Color::White);
			AspectosGeneralesTexto(vectorTexto[7],fuente.VerFuenteTetris(),"Por favor, teclea tu nombre:",false,26,Color::White); 
			AspectosGeneralesTexto(vectorTexto[8],fuente.VerFuenteTetris(),"Presione Enter para guardar tu nombre",false,18,Color::White);
			Globales::VerNombreUsuario().setPosition(320,520); 
			vectorTexto[5].setPosition(180,300); 
			vectorTexto[6].setPosition(90,380);
			vectorTexto[7].setPosition(130,440);
		}
		else AspectosGeneralesTexto(vectorTexto[8],fuente.VerFuenteTetris(),"Presione Escape para volver al Menu",false,8,Color::White); 
		vectorTexto[0].setPosition(130,50);
		vectorTexto[8].setPosition(130,590);
	}
	else{
		AspectosGeneralesTexto(vectorTexto[0],fuente.VerFuenteOpciones(),"Game Over",true,46,Color::White);
		AspectosGeneralesTexto(vectorTexto[1],fuente.VerFuenteOpciones(),"Mejor puntuacion:",true,22,Color::Red); 
		if(posicionEscribir==0) AspectosGeneralesTexto(vectorTexto[2],fuente.VerFuenteOpciones(),palabraScore,false,22,Color::White);
		else{
			m_scoreArchivo = to_string(scoreArchivo);//Mejor puntuacion.
			AspectosGeneralesTexto(vectorTexto[2],fuente.VerFuenteOpciones(),m_scoreArchivo,false,22,Color::White);
		}
		AspectosGeneralesTexto(vectorTexto[3],fuente.VerFuenteOpciones(),"Tu puntuacion:",true,22,Color::Red);
		AspectosGeneralesTexto(vectorTexto[4],fuente.VerFuenteOpciones(),palabraScore,false,22,Color::White);
		if(posicionEscribir>=0) {
			Globales::CambiarEstado(6,true);
			nJugador = to_string(posicionEscribir+1);
			AspectosGeneralesTexto(Globales::VerNombreUsuario(),fuente.VerFuenteOpciones(),"",true,32,Color::Green);
			AspectosGeneralesTexto(vectorTexto[5],fuente.VerFuenteOpciones(),"Felicitaciones!!!",false,36,Color::White);
			AspectosGeneralesTexto(vectorTexto[6],fuente.VerFuenteOpciones(),"Has superado al Jugador numero "+nJugador+"!",false,20,Color::White);
			AspectosGeneralesTexto(vectorTexto[7],fuente.VerFuenteOpciones(),"Por favor, teclea tu nombre:",false,21,Color::White); 
			AspectosGeneralesTexto(vectorTexto[8],fuente.VerFuenteOpciones(),"Presione Enter para guardar tu nombre",false,14,Color::White);
			Globales::VerNombreUsuario().setPosition(320,520); 
			vectorTexto[5].setPosition(100,300); 
			vectorTexto[6].setPosition(60,400);
			vectorTexto[7].setPosition(90,460); 
		}
		else AspectosGeneralesTexto(vectorTexto[8],fuente.VerFuenteOpciones(),"Presione Escape para volver al Menu",false,8,Color::White);
		vectorTexto[0].setPosition(130,50);
		vectorTexto[8].setPosition(90,590);
	}
}

/*
Con este metodo simplificamos el seteo de los textos, modificando:
	1) La fuente.
	2) El contenido.
	3) El tamaño.
	4) El color.
	5) La posicion.
*/
void GameOver::AspectosGeneralesTexto(Text &texto,Font &fuente,string titulo,bool esTitulo,int tamanio,Color color){
	texto.setFont(fuente); texto.setString(titulo); texto.setCharacterSize(tamanio); texto.setFillColor(color);
	if(esTitulo) texto.setPosition(110,y);
	else {texto.setPosition(450,y); y+=80;}
}

/*
Esta funcion continuamente va variando el valor de los flotantes 'rojo', 'verde',y 'azul' de manera de crear distintas
combinaciones de colores con el formato RGB.
*/
void GameOver::CrearCombinacion(){
	if(rojo!=255) rojo+=0.5;
	else rojo = rand()%255;
	if(verde!=255) verde+=0.5;
	else verde = rand()%255;
	if(azul!=255) azul+=0.5;
	else azul = rand()%255;
	Globales::CambiarColor(Color(rojo,verde,azul));
}


//Este metodo llama a la funcion 'EscribirPuntaje' de la clase 'Archivo' para escribir el nombre y el puntaje hecho por el usuario.
void GameOver::EscribirPuntaje(int m_score,int posicionEscribir){
	archivo.EscribirPuntaje(Globales::VerDificultad(),m_score,posicionEscribir,tamanioArchivo);
	Globales::CambiarEstado(0,false); // Cambiamos el estado para que esta funcion no sea llamada mas.
	//Corroboramos el estado de carga de la fuente.
	if(fuente.EstadoFuenteOpciones()) AspectosGeneralesTexto(Globales::VerNombreUsuario(),fuente.VerFuenteOpciones(),"Nombre guardado!!!",false,30,Color::Green);
	else if(fuente.EstadoFuenteInformacion()) AspectosGeneralesTexto(Globales::VerNombreUsuario(),fuente.VerFuenteInformacion(),"Nombre guardado!!!",false,22,Color::Green);
	else if(fuente.EstadoFuenteTetris()) AspectosGeneralesTexto(Globales::VerNombreUsuario(),fuente.VerFuenteTetris(),"Nombre guardado!!!",false,46,Color::Green);
	else AspectosGeneralesTexto(Globales::VerNombreUsuario(),fuente.VerFuenteMenu(),"Nombre guardado!!!",false,26,Color::Green);
	Globales::VerNombreUsuario().setPosition(105,520);
}
