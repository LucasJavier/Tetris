#include "Partida.h"
#include "Menu.h"

/*
El constructor de esta clase setea las dimensiones de la ventana, cambia la seccion de esta que es visualizada, y establece 
la velocidad de los fotogramas. A su vez, carga, si encuentra, fuentes e imagenes que seran usadas. Ajusta diferentes vectores
que seran modificados a lo largo de la partida y, en algunos, los rellena con ceros '0'. Setea el tamaño, el color, y otros detalles
de los RectangleShape que contiene el vector 'celdasFormas' y setea distintas variables de tipo 'int' y 'bool'. Los metodos
'CrearTexto', 'CrearRectanguulos', 'ModificarDificultad', 'MostrarMejorPuntaje', 'CargarMusica', y 'MostrarVelocidad' son 
explicados mas abajo. Por ultimo, inicia el reloj que llevara la cuenta del tiempo transcurrido desde que se inicio la partida.
*/
Partida::Partida(RenderWindow &ventana) : Escenas(ventana){
	ventana.create(VideoMode(escala*tamanioCuadrado*columnas*3,escala*tamanioCuadrado*filas),"Tetris"); //Creamos la ventana.
	ventana.setView(View(FloatRect(0,0,tamanioCuadrado*columnas*3,tamanioCuadrado*filas))); //Establecemos que zona visualizar
	if(fuente.EstadoFuenteOpciones()) {vectorTexto.resize(19); CrearTexto();} //Creamos los textos a utilizar en la partida.
	if(!tPlay.loadFromFile("Imagenes/Play.png")) cerr<<"No se pudo abrir la imagen 'Play.png'"<<endl; /*Chequeamos si la textura 
																										pudo cargarse.*/
	else {sPlay.setTexture(tPlay); sPlay.setScale(0.15,0.15); sPlay.setPosition(tamanioCuadrado*2+5,tamanioCuadrado*2-5);}
	if(!tPausa.loadFromFile("Imagenes/Pausa.png")) cerr<<"No se pudo abrir la imagen 'Pausa.png'"<<endl; //Cargamos y seteamos texturas.
	else {sPausa.setTexture(tPausa); sPausa.setScale(0.3,0.3); sPausa.setPosition(tamanioCuadrado*2+5,tamanioCuadrado*2-5);}
	//Corroboramos si el icono pudo ser cargado.
	if(!icono.loadFromFile("Imagenes/Icono.png")) cerr<<"No se pudo encontrar la imagen 'Icono.png'"<<endl;
	else ventana.setIcon(icono.getSize().x,icono.getSize().y,icono.getPixelsPtr()); //Seteamos el icono de la ventana.
	CrearRectangulos(); ModificarDificultad(); MostrarMejorPuntaje(); CargarMusica(); MostrarVelocidad();
	Globales::CambiarColor(Color(Color::Black)); /*Cambiamos el color de fondo de la ventana*/ celdasFormas.resize(11);
	for(size_t i=0;i<celdasFormas.size();i++){ //Seteamos los RectangleShape contenidos en el vector 'celdasFormas'.
		celdasFormas[i].setSize({tamanioCuadrado-1,tamanioCuadrado-1});
		if(i<9) celdasFormas[i].setFillColor(Globales::VerColor(i));
		else if(i==9) celdasFormas[9].setFillColor(Color(192,192,192));
		else celdasFormas[10].setFillColor(Color(192,192,192,50));
		if(i==0) continue;
		if(Globales::VerEstado(7)) {celdasFormas[i].setOutlineThickness(0.3); celdasFormas[i].setOutlineColor(Color(204,255,55));}
		else {celdasFormas[i].setOutlineThickness(-0.5); celdasFormas[i].setOutlineColor(Color(Color::Cyan));}
	}
	matrizCeldas.resize(columnas); 
	for(int i=0;i<columnas;i++) matrizCeldas[i].resize(filas,0); //Llenamos cada vector dentro de la matriz con ceros '0'.
	matrizFormaSiguiente.resize(6);
	for(int i=0;i<6;i++) matrizFormaSiguiente[i].resize(6,0); //Llenamos cada vector dentro de la matriz con ceros '0'.
	//Seteamos las distintas variables a utilizar durante la partida.
	Globales::CambiarEstado(2,true); caidaRapida = false; cambioMusica = true; borrarLinea = false; esGameOver = false;
	puedeFestejar = true; esperar = false; m_si = false; m_no = false; cambioTiempo = false; primeraVez = true; 
	contadorLineas = 0; score = 0; repetir = 1; hora = 0; min = 0; seg = 0; mili = 0;numFilas = 0; m_puntaje = "0";
	srand(time(0)); n_forma = 1+rand()%8; //De manera pseudo-aleatoria, se elige que primer forma crear.
	relojTiempo.restart(); relojEscape.restart(); //Empezamos los relojes.
}

//En su destructor se elimina todos los punteros que contiene la clase (punteros a la clase 'Music' y a la clase 'Clock').
Partida::~Partida(){
	for(int i=0;i<cantMusica;i++) delete punteroMusica; 
	for(size_t i=0;i<vectorFilas.size();i++) delete relojFilas;
}

/*
El metodo 'Actualizar' de la clase 'Partida' realiza diferentes acciones:
	1) Verifica si el juego debe de terminar mediante (principalmente) la funcion 'ChequearGameOver' (explicada mas debajo). Si
	es cierto, se realizan las siguientes acciones:
		i) Se genera, por ultima vez, una forma, para rellenar mas el espacio y mostrar porque no se podia continua el juego.
		ii) Se cambia el color de todas las formas que se encuentran en la matriz mediante el metodo 'CambiarColorFormas' (explicado
		mas debajo).
		iii) Se paran todas las musicas y/o efectos cargados (si estan activos).
		iv) Se cambia una parte del texto para indicarle al usuario que debe apretar para salir de esa ventana.
		v) Si la musica se dejo activada, se ejecuta la musica de Game Over.
		vi) Se cambia, cada cierto periodo de tiempo, el color del texto cambiado, para brindar mas ayuda visual al usuario.
	2) Verifica si el usuario a apretado la tecla 'Escape'. Si es cierto, sucede lo siguiente:
		i) Se para la musica sonando (si esta está activa)
		ii) Se toma el tiempo de cada fila hasta el momento (si esta activa el modo 'Fila piedra')
		iii) Se muestra en pantalla un dialogo que pregunta al usuario si realmente
		quiere salir de la partida y volver al menu. Si es cierto se cambia a la clase 'Menu', acabando asi la partida. 
		Sino, se continua el juego.
	3) Verifica si el usuario ha hecho click en la textura de 'Play'/'Pausa' o a apretado la tecla de Pausa. De ser cierto, para
	toda la musica sonando (si esta activa), captura el tiempo de cada fila (si esta activo el modo 'Fila piedra'), y:
		i) Si se encontraba andando el juego, este se para y se indica que se encuentra en estado pausa, cambiando la imagen de pausa
		a play.
		ii) Si se encuentra en pausa, se despausa el juego y este continua andando, renaudando la musica (si se encuentra activa), el
		tiempo de cada fila (si esta activo), y cambiando la imagen de play a pausa.
	4) Si el juego corre, suceden las siguientes cosas:
		i) Se obtiene el tiempo que ha pasado desde que se inicio el juego con el metedo 'ObtenerTiempo' (explicado mas debajo).
		ii) Se verifica si se ha añadido alguna forma en alguna fila.
		iii) Se verifica si se ha completado alguna o varias filas de la matriz. Si sucede, se saltea todo lo explicado a continuacion
		para pasar a la etapa de dibujado. Sino:
			a) Si esta activo el modo 'Fila piedra', se chequea el tiempo de cada una de estas.
			b) Si debe, genera una nueva forma en la matriz.
			c) Si ha pasado los tiempos establecidos, llama al metodo 'Actualizar' de la clase 'Formas' para, si puede, mover el objeto
			hacia abajo.
			d) Llama al metodo 'Actualizar' de la clase 'Jugador' para, si se ha hecho alguna accion, realizarla.
			e) Verifica si la variable de tipo 'bool' 'caidaRapida' es verdadera y, si lo es, y si los efectos estan activas, ejecuta
			un archivo de audio.
			f) Verifica si la forma se a movido. Si lo ha hecho, y si los efectos estan activados, se reproduce un archivo de audio.
			Ademas, se llama al metodo 'LimpiarSombra' de la clase 'Formas' para eliminar la sombra del objeto.
			g) Llama al metodo 'MuestraPieza' de la clase 'Formas' para crear la sombra del objeto.
			h) Llama al metodo 'ModificarVariable' (explicado mas debajo) para actualizar el puntaje.
			i) Si la musica esta activada, y si se cargaron los archivos, verifica si es tiempo de cambiar los archivos musicales.
*/
void Partida::Actualizar(Juego &juego,RenderWindow &ventana) {
	if(esGameOver || ChequearGameOver(n_forma)){ //Verificacion de Game Over del juego.
		borrarLinea = false; repetir = 1; /*cambiamos la variable 'borrarLinea' a 'false' ya que no se eliminara ninguna linea mas
											en el juego. Cambiamos a uno '1' la variable repetir para que el proceso de dibujado se
											realice una sola vez*/
		if(!Globales::VerEstado(0)){ //Condicional usado para realizar las acciones siguientes una sola vez.
			if(Globales::VerEstado(2)) Generar(matrizCeldas); //Generamos un a forma.
			CambiarColorFormas(); //Cambiamos los colores de todas las formas.
			if(Globales::VerEstado(9) || Globales::VerEstado(10)){ //Si la musica o los efectos estan activados, realizamos lo siguiente:
				for(int i=0;i<cantMusica;i++) punteroMusica[i].stop(); //Paramos los archivos de sonido que haya.
			}
			//Si la fuente fue cargada, modificamos el texto de 'Siguiente' a 'Pulsa Enter para salir' para indicarle al usuario 
			//como abandonar la ventana.
			if(fuente.EstadoFuenteOpciones()) {AspectosTextoGenerales(vectorTexto[0],fuente.VerFuenteOpciones(),"Pulse Enter para salir",false,8); vectorTexto[0].setPosition(tamanioCuadrado*(columnas+0.20)*2,filas*0.2);}
			if(Globales::VerEstado(9)){ //Si la musica esta activa, reproducimos la musica de Game Over.
				if(!punteroMusica[2].openFromFile("Musica/Game Over.flac")) { //Chequeamos si el archivo musical pudo cargarse.
					cerr<<"No se pudo encontrar el archivo musical 'Game Over.flac'"<<endl;
					vectorEstadoSonido[2] = false; //Si no se ha cargado el archvio de audio, seteamos el estado a 'false'.
				}
				else {
					punteroMusica[2].setVolume(70); punteroMusica[2].play(); //Seteamos el volumen y lo reproducimos.
					vectorEstadoSonido[2] = true;  //Como se pudo cargar el archivo musical, seteamos su estado a 'true'.
				}
			}
			Globales::CambiarEstado(0,true); //Cambiamos el estado a true para que las acciones anteriores no se realicen mas.
		}
		if(Keyboard::isKeyPressed(Keyboard::Key::Return)) { //Condicional que verifica si el usuario ha apreta la tecla 'Enter'.
			if(Globales::VerEstado(9)) { //Si la musica esta activa:
				if(vectorEstadoSonido[2]) punteroMusica[2].stop();  //Paramos el sonido de Game Over.
			}
			Globales::CambiarEstado(0,false); //Cambiamos el estado de una variable que es utilizada en la clase 'GameOver'.
			juego.SetiarEscena(new GameOver(ventana,score)); //Seteamos la escena a la clase 'GameOver'.
		}
		if(fuente.EstadoFuenteOpciones()){ //Si la fuente fue cargada.
			if(reloj.getElapsedTime().asMilliseconds()>300) { //Si se ha superado el tiempo establecido:
				vectorTexto[0].setFillColor(Color(rand()%256,rand()%256,rand()%256)); /*Modificamos el color del texto 'Pulsa Enter para
																					salir', de manera de brindar ayuda visual al usuario.*/
				reloj.restart(); //Reiniciamos el reloj.
			}	
		}
	}
	else {
		//Si la tecla 'Escape' fue apretada, y si se ha superado el tiempo establecido:
		if(Keyboard::isKeyPressed(Keyboard::Key::Escape) && relojEscape.getElapsedTime().asMilliseconds()>300) {
			esperar = true; //Cambiamos la variable 'esperar' a 'true' para que el juego 'pare'.
			primeraVezPausa = true; //Cambiamos la variable 'primeraVezPausa' a 'true' para realizar ciertas acciones una sola vez.
			relojEscape.restart(); //Reiniciamos el reloj.
		}
		if(esperar){ //Si el juego tiene que 'parar'.
			if(primeraVezPausa){ //Condicional para realizar las siguientes acciones una unica vez.
				if(Globales::VerEstado(9)){ //Si la musica esta activa, ponemos en pausa la debida.
					if(cambioMusica) { //Condicional que verifica a que archivo musical ha de ponerlo en pausa.
						//Chequeamos si la musica se ha cargado con normalidad.
						if(vectorEstadoSonido[0]) punteroMusica[0].pause();
					}
					else  {
						if(vectorEstadoSonido[1]) punteroMusica[1].pause();
					}
				}
				if(Globales::VerEstado(14)){ //Si el modo 'Fila piedra' esta activado.
					if(vectorTiempo.size()!=0){ //Verifica si hay objetos ya consolidados.
						//Guardamos el tiempo hasta este momento de cada fila que posea un objeto.
						for(int i=0;i<numFilas;i++) vectorTiempo[i] = relojFilas[i].getElapsedTime() + vectorTiempo[i];
					}
				}
				primeraVezPausa = false; //Cambiamos el estado de la variable para que lo anterior no se realice mas.
			}
			if(Mouse::isButtonPressed(Mouse::Left)){ //Verificamos el usuario haya hecho click izquierdo con el mouse
				RespuestaUsuario(ventana); //Cambiamos, si ha de hacerse, las variables de tipo 'bool' 'm_si' y 'm_no'.
				if(m_si){ //Si 'm_si' es verdadera se para, si la musica esta activa, esta, y se setea la escena a la clase 'Menu'.
					if(Globales::VerEstado(9) || Globales::VerEstado(10)) for(int i=0;i<cantMusica;i++) punteroMusica[i].stop();
					Globales::CambiarEstado(0,true); //Cambiamos el estado de una variable que se utiliza en la clase 'Menu'.
					juego.SetiarEscena(new Menu(ventana)); //Cambiamos de ventana, al cambiar de clase.
				}else if(m_no) { /*Si 'm_no' es verdadera:
									i) Si la musica esta activa, se reanuda esta.
									ii) Si el modo 'Fila piedra' esta activa, se resetean los tiempos
									iii) Se continua el juego.*/
									
					esperar = false; /*Cambiamos el estado de la variable, para que estas acciones no se repitan mas y las siguientes
										continuen.*/
					if(Globales::VerEstado(9)){ //Si la musica esta activada.
						if(cambioMusica) { //Verifica que archivo musical debe reproducirse.
							//Si el archivo musical pudo cargarse con normalidad.
							if(vectorEstadoSonido[0]) punteroMusica[0].play();
						}
						else {
							if(vectorEstadoSonido[1]) punteroMusica[1].play();
						}
					}
					if(Globales::VerEstado(14)){ //Si el modo 'Fila piedra' esta activada.
						if(vectorTiempo.size()!=0){ //Si hay filas con objetos consolidados.
							for(int i=0;i<numFilas;i++) relojFilas[i].restart(); //Reiniciamos los relojes de las filas.
						}
					}
				}
			}
		}else{ //Si el juego continua.
			if(Mouse::isButtonPressed(Mouse::Left)) CliqueoPausa(ventana); /*Si el usuario ha hecho click izquierdo con el mouse,
																			chequeamos si lo ha hecho en el signo Play/Pausa
																			para realizar las acciones correspondientes.*/
			else primeraVezPausa = jugador.TecleoPausa(); //Sino ha hecho click, verificamos si ha apretado la tecla de Pausa.
			if(!Globales::VerEstado(11)){ //Si el juego no se encuentra en estado de pausa. 
				if(Globales::VerEstado(12)){ //Si el juego estuvo pausado.
					if(Globales::VerEstado(9)){ //Si la musica esta activa.
					//se reanuda esta.
						if(cambioMusica) { //Que archivo musical hay que reproducir
							if(vectorEstadoSonido[0]) punteroMusica[0].play();
						}
						else {
							if(vectorEstadoSonido[1]) punteroMusica[1].play();
						}
					}
					if(Globales::VerEstado(14)){ //Si el modo 'Fila piedra' esta activa.
						for(int i=0;i<numFilas;i++) relojFilas[i].restart(); //Reiniciamos todos los relojes.
					}
					Globales::CambiarEstado(12,false); //Cambiamos el estado para que las acciones anteriores no se realicen mas.
				}
				if(fuente.EstadoFuenteOpciones()){ //Si la fuente pudo ser cargada.
					ObtenerTiempo(); vectorTexto[10].setString(tiempoCadena); /*Obtenemos el tiempo transcurrido desde que se inicio el
																				juego y lo seteamos en un texto.*/
				}
				VerificarLinea(matrizCeldas); /*Verificamos si el usuario a completado algunas o varias filas de la matriz, o si
												hay formas consolidadas en filas*/
				if(!borrarLinea){ //Sino no a completado ninguna fila, se realizan lo siguiente:
					if(Globales::VerEstado(14)) ChequearTiempoFilas(); /*Si el modo 'Fila piedra' esta activo, llamamos a la funcion
																		'ChequearTiempoFilas' (explicado mas debajo), para verificar
																		el tiempo en cada una de estas.*/
					repetir = 1; //Cambiamos la variable para que el proceso de dibujado se realice una sola vez.
					if(Globales::VerEstado(2)) {Generar(matrizCeldas); relojEsperar.restart();} /*Si debe, se genera una nueva forma
																					con el metodo 'Generar' (explicado mas debajo).*/
					if(reloj.getElapsedTime().asMilliseconds()>tiempoDeCaida && relojEsperar.getElapsedTime().asMilliseconds()>1000){
						/*Si ha pasado el tiempo establecido, se llama al metodo 'Actualizar' de la clase 'Forma' para, si se puede, 
						mover el objeto hacia abajo un '1' lugar.*/
						forma.Actualizar(matrizCeldas,valorForma,score);
						reloj.restart(); //Reiniciamos el reloj.
					}
					caidaRapida = jugador.Actualizar(matrizCeldas,valorForma,score); /*Se llama al metodo 'Actualizar' de la clase
																		'Jugador' para realizar las acciones que haga el usuario.*/
					if(caidaRapida) { /*Si el usuario a apretado para hacer un HardDrop, y si los efectos estan activados,
										se reproduce un archivo musical.*/
						if(Globales::VerEstado(9) && Globales::VerEstado(10)){ //Si la musica y los efectos estan activados.
							//Si el efecto se cargo correctamente.
							if(vectorEstadoSonido[7]) punteroMusica[7].play();
						}
						else if(Globales::VerEstado(10)) { //Si los efectos estan activados.
							if(vectorEstadoSonido[3]) punteroMusica[3].play();
						}
					}
					if(Globales::VerEstado(13)) { /*Si se movio la forma, y si los efectos estan activados, se reproduce un
													archivo musica. Ademas, se llama al metodo 'LimpiarSombra' de la clase 'Formas' 
													para borrar la sombra del objeto.*/
						if(Globales::VerEstado(9) && Globales::VerEstado(10)) { //Si la musica y los efectos estan activados.
							//Si el efecto se cargo correctamente.
							if(vectorEstadoSonido[4]) punteroMusica[4].play();
						}
						else if(Globales::VerEstado(10)) { //Si los efectos estan activados.
							if(vectorEstadoSonido[0]) punteroMusica[0].play(); 
						}
						forma.LimpiarSombra(matrizCeldas);
					}
					forma.MuestraPieza(matrizCeldas,caidaRapida); /*Se llama al metodo 'MuestraPieza' de la clase 'Formas' para
																	dibujar la sombra del objeto.*/
					ModificarVariables(2); //Se actualiza el score del usuario.
					if(Globales::VerEstado(9)){ /*Si la musica esta activa, y si se cargaron los archivos musicales, se verifica que
												haya pasado el tiempo establecido para realizar el cambio de archivos.*/
						if(cambioMusica && vectorEstadoSonido[0]){
							if(relojCambioMusica.getElapsedTime().asSeconds()>punteroMusica[0].getDuration().asSeconds()*2){
								punteroMusica[0].stop(); punteroMusica[1].play(); cambioMusica = false; relojCambioMusica.restart();
							}
						}else if(vectorEstadoSonido[1]){
							if(relojCambioMusica.getElapsedTime().asSeconds()>punteroMusica[1].getDuration().asSeconds()-5){
								punteroMusica[1].stop(); punteroMusica[0].play(); punteroMusica[0].setLoop(true); cambioMusica = true; relojCambioMusica.restart();
							}
						}
					}
				}else {repetir = 380; CambiarNumeros();} /*Si ha completado una o varias filas, se cambia la cantidad de veces que debe
														de dibujarse la ventana. Asimismo, se llama al metodo 'CambiarNumeros' 
														(explicado mas debajo) para modificar los valores de las lineas completadas a 
														nueve '9'.*/
			}else{ //Si pone en pausa el juego el usuario.
				if(primeraVezPausa){ //Condicional para que las siguientes acciones se realizen una sola vez.
					if(Globales::VerEstado(14)){ //Si el modo 'Fila piedra' esta activado.
						//Guardamos el tiempo de cada fila.
						for(int i=0;i<numFilas;i++) vectorTiempo[i] = relojFilas[i].getElapsedTime() + vectorTiempo[i];
					}
					if(Globales::VerEstado(9)){ //Si la musica esta activada.
						if(cambioMusica) { //Verifica que archivo musical debe reproducir.
							if(vectorEstadoSonido[0]) punteroMusica[0].pause();
						}
						else if(vectorEstadoSonido[1]) punteroMusica[1].pause();
					}
					primeraVezPausa = false; //Cambiamos el estado para que las acciones anteriores no se realicen mas.
				}
			}
		}
	}
}

/*
Este es el metodo encargado de dibujar:
	1) Los RectangleShape contenido en el vector 'celdasFormas' en la matriz 'matrizCeldas', tantas veces como grande sea la matriz, 
	y dependiendo del numero tenga adentro:
		'-9': Corresponde a la sombra de la ultima forma creada.
		'0': Celda vacia, sin ninguna forma.
		'1': Posicion de la ultima forma creada.
		'2': Celdas que contienen a la forma 'Cuadrado'.
		'3': Celdas que contienen a la forma 'EleIzq'.
		'4': Celdas que contienen a la forma 'EleDer'.
		'5': Celdas que contienen a la forma 'Linea'.
		'6': Celdas que contienen a la forma 'EseIzq'.
		'7': Celdas que contienen a la forma 'EseDer'.
		'8': Celdas que contienen a la forma 'Te'.
		'9': Celdas que contienen a RectangleShape que han de ser eliminados, debido a que se ha completado la fila.
	2) Los RectangleShape contenido en el vector 'celdasFormas' en la matriz 'matrizFormaSiguiente', dependiendo del numero tenga 
	adentro, de manera de mostrar que forma viene a continuacion:
		'0': Celda vacia, sin ninguna forma.
		'2': Celdas que contienen a la forma 'Cuadrado'.
		'3': Celdas que contienen a la forma 'EleIzq'.
		'4': Celdas que contienen a la forma 'EleDer'.
		'5': Celdas que contienen a la forma 'Linea'.
		'6': Celdas que contienen a la forma 'EseIzq'.
		'7': Celdas que contienen a la forma 'EseDer'.
		'8': Celdas que contienen a la forma 'Te'.
	3) El fondo (RectangleShape) de las otras 2 ventanas anidadas al de la partida, una hacia la izquierda de esta y otra hacia la
	derecha.
	4) Dentro del fondo (RectangleShape) creado a la derecha de la partida, se dibujar otro RectangleShape en el cual, dentro, se
	dibuja la forma siguiente.
	5) En este mismo fondo (RectangleShape) se dibujar los distintos textos informativos para el usuario (puntaje, dificultad, lineas
	despejadas, etc.)
	6) En el fondo (RectangleShape) creado a la izquierda de la partida, se dibuja la imagen de Pausa o Play, y los demas textos
	(Tiempo de partida, velocidad de caida, etc.)
	7) Si se pone en Pausa el juego se crea otro RectangleShape en el cual, dentro, se dibuja un texto informativo que la partida se
	encuentra en estado de pausa.
	8) Si se aprieta 'Escape' se dibuja otro RectangleShape con texto informativo dentro, para que el usuario decida si salir o no
	del juego.
	9) Si se debe borrar una o varias filas:
		i) El proceso de dibujado se repite 30 veces. Caso contrario, una unica vez.
		ii) Si los efectos estan activos, se reproduce 2 archivos musicales.
		iii) Se llama a los metodos 'LimpiarLinea' y 'ModificarVariable' (explicados mas debajos) para, justamente, despejar
		la fila llena y modificar las lineas y el puntaje del usuario, como asi tambien, si es debido, el tiempo de caida de la forma.
*/
void Partida::Dibujar(RenderWindow &ventana) {
	if(borrarLinea) { //Si se debe eliminar una fila.
		if(Globales::VerEstado(9) && Globales::VerEstado(10)) { /*Si la musica y los efectos estan activados, se reproduce 
																un archivo musical.*/
			if(vectorEstadoSonido[6]) punteroMusica[6].play();
		}
		else if(Globales::VerEstado(10)) { //Si los efectos estan activos.
			if(vectorEstadoSonido[2]) punteroMusica[2].play();
		}
	}
	for(int i=0;i<repetir;i++){ /*Dependiendo de lo que ocurra en la partida, la cantidad de veces que se realice este 'for' va a
								depender de la variable repetir.*/
		ventana.clear(Globales::VerColor()); //'Limpiamos' todo lo que contiene la ventana.
		for(int i=0;i<columnas;i++){ 
			for(int j=0;j<filas;j++){
				switch(matrizCeldas[i][j]){ /*Se setea la posicion y se dibuja cada RectangleShape, tantas veces como grande sea 
											la matriz 'matrizCeldas'. Los colores de los RectangleShape dependeran del numero que
											tenga adentro la matriz.*/
				case -9:{Sombra.setPosition(tamanioCuadrado*i+(columnas*escala*2),tamanioCuadrado*j); ventana.draw(Sombra); break;}
				case -1:{celdasFormas[9].setPosition(tamanioCuadrado*i+(columnas*escala*2),tamanioCuadrado*j); ventana.draw(celdasFormas[9]); break;}
				case -2:{celdasFormas[10].setPosition(tamanioCuadrado*i+(columnas*escala*2),tamanioCuadrado*j); ventana.draw(celdasFormas[10]); break;}
				case 0:{celdasFormas[0].setPosition(tamanioCuadrado*i+(columnas*escala*2),tamanioCuadrado*j); ventana.draw(celdasFormas[0]); break;}
				case 1:{celdasFormas[valorForma].setPosition(tamanioCuadrado*i+(columnas*escala*2),tamanioCuadrado*j); ventana.draw(celdasFormas[valorForma]); break;}
				case 2:{celdasFormas[1].setPosition(tamanioCuadrado*i+(columnas*escala*2),tamanioCuadrado*j); ventana.draw(celdasFormas[1]); break;}
				case 3:{celdasFormas[2].setPosition(tamanioCuadrado*i+(columnas*escala*2),tamanioCuadrado*j); ventana.draw(celdasFormas[2]); break;}
				case 4:{celdasFormas[3].setPosition(tamanioCuadrado*i+(columnas*escala*2),tamanioCuadrado*j); ventana.draw(celdasFormas[3]); break;}
				case 5:{celdasFormas[4].setPosition(tamanioCuadrado*i+(columnas*escala*2),tamanioCuadrado*j); ventana.draw(celdasFormas[4]); break;}
				case 6:{celdasFormas[5].setPosition(tamanioCuadrado*i+(columnas*escala*2),tamanioCuadrado*j); ventana.draw(celdasFormas[5]); break;}
				case 7:{celdasFormas[6].setPosition(tamanioCuadrado*i+(columnas*escala*2),tamanioCuadrado*j); ventana.draw(celdasFormas[6]); break;}
				case 8:{celdasFormas[7].setPosition(tamanioCuadrado*i+(columnas*escala*2),tamanioCuadrado*j); ventana.draw(celdasFormas[7]); break;}
				default:{EfectoLinea(matrizCeldas); celdasFormas[8].setPosition(tamanioCuadrado*i+(columnas*escala*2),tamanioCuadrado*j); ventana.draw(celdasFormas[8]); break;}
				}
			}
		}
		for(size_t i=0;i<matrizFormaSiguiente.size();i++){
			for(size_t j=0;j<matrizFormaSiguiente[i].size();j++){
				switch(matrizFormaSiguiente[i][j]){ /*Se setea la posicion y se dibuja cada RectangleShape, tantas veces como sea
													grande la matriz 'matrizFormaSiguiente'. Los colores de los RectangleShape dependeran
													del numero que tenga dentro la matriz.*/
				case 0:{celdasFormas[0].setPosition(tamanioCuadrado*i+((columnas+1)*escala*escala),tamanioCuadrado*(j+2)); ventana.draw(celdasFormas[0]); break;}
				case 2:{celdasFormas[1].setPosition(tamanioCuadrado*i+((columnas+1)*escala*escala),tamanioCuadrado*(j+2)); ventana.draw(celdasFormas[1]); break;}
				case 3:{celdasFormas[2].setPosition(tamanioCuadrado*i+((columnas+1)*escala*escala),tamanioCuadrado*(j+2)); ventana.draw(celdasFormas[2]); break;}
				case 4:{celdasFormas[3].setPosition(tamanioCuadrado*i+((columnas+1)*escala*escala),tamanioCuadrado*(j+2)); ventana.draw(celdasFormas[3]); break;}
				case 5:{celdasFormas[4].setPosition(tamanioCuadrado*i+((columnas+1)*escala*escala),tamanioCuadrado*(j+2)); ventana.draw(celdasFormas[4]); break;}
				case 6:{celdasFormas[5].setPosition(tamanioCuadrado*i+((columnas+1)*escala*escala),tamanioCuadrado*(j+2)); ventana.draw(celdasFormas[5]); break;}
				case 7:{celdasFormas[6].setPosition(tamanioCuadrado*i+((columnas+1)*escala*escala),tamanioCuadrado*(j+2)); ventana.draw(celdasFormas[6]); break;}
				default:{celdasFormas[7].setPosition(tamanioCuadrado*i+((columnas+1)*escala*escala),tamanioCuadrado*(j+2)); ventana.draw(celdasFormas[7]); break;}}
			}
		}
		ventana.draw(FondoSegundaVentana); /*RectangleShape donde se situaran las formas que se crearan a continuacion de la 
											ultima creada, y diversos textos informativos.*/
		ventana.draw(ContenedorSiguienteForma); /*RectangleShape donde, dentro, se dibujara la forma que se creara a continuacion de
												la ultima.*/
		if(fuente.EstadoFuenteOpciones()){ //Si la fuente se ha cargado con normalidad.
			for(size_t i=0;i<vectorTexto.size()-4;i++) ventana.draw(vectorTexto[i]); //Dibujamos los textos.
		}
		ventana.draw(FondoTercerVentana); //RectangleShape donde se dibujara la imagen de Play/Pausa y diversos textos informativos.
		if(Globales::VerEstado(11)){ //Si nos encontramos en estado de Play o Pausa.
			ventana.draw(sPlay); ventana.draw(FondoPausa); 	/*Si se encuentra en estado de pausa, se dibujara la imagen de Play, 
															un RectangleShape casi transparente del tamaño de la ventana y, dentro, 
															un texto informativo.*/
			//Si la fuente ha sido cargado con normalidad.
			if(fuente.EstadoFuenteOpciones()) ventana.draw(vectorTexto[15]);
		}
		else ventana.draw(sPausa); //Se dibuja la imagen de Pausa.
		if(esperar) { //Si el usuario ha apretado 'Escape'.
			ventana.draw(FondoPreguntar); /*RectangleShape donde se encontraran los textos para que el usuario decida si salir al
											'Menu' o continua la partida.*/
			if(fuente.EstadoFuenteOpciones()){ //Si la fuente ha sido cargada con normalidad.
				ventana.draw(vectorTexto[16]); ventana.draw(vectorTexto[17]); ventana.draw(vectorTexto[18]);
			}
		}
		ventana.display(); //Mostramos lo dibujado en la ventana.
	}
	if(borrarLinea) { //Si ha de eliminarse una o varias filas.
		if(Globales::VerEstado(9) && Globales::VerEstado(10)) { //Si la musica y los efectos estan activos.
			//Si el efecto se ha cargado con normalidad.
			if(vectorEstadoSonido[6]) punteroMusica[6].stop(); 
			if(vectorEstadoSonido[5]) punteroMusica[5].play();
		}
		else if(Globales::VerEstado(10)) { //Si los efectos estan activos.
			//Si el efecto se ha cargado con normalidad.
			if(vectorEstadoSonido[2]) punteroMusica[2].stop();
			if(vectorEstadoSonido[1]) punteroMusica[1].play();
		}
		LimpiarLinea(matrizCeldas); ModificarVariables(1); /*Se llama a los metodos 'LimpiarLinea' y 'ModificarVariables', para 
															eliminar la/s fila/s completa/s y actualizar los textos.*/
	}
}	

/*
Este metodo es el encargado de:
	1) Reiniciar la 'matrizFormaSiguiente', donde se mostrara la forma que se creara a continuacion del ultimo objeto generado.
	2) Crear la forma que se mostrara en la 'matrizCeldas', la matriz que conforma el juego. Dependiendo del numero que se haya
	generado estos pueden ser:
		1: Se genera la forma tipo 'Cuadrado'.
		2: Se genera la forma tipo 'EleIze'.
		3: Se genera la forma tipo 'EleDer'.
		4: Se genera la forma tipo 'Linea'.
		5: Se genera la forma tipo 'EseIzq'.
		6: Se genera la forma tipo 'EseDer'.
		7: Se genera la forma tipo 'Te'.
	3) Llamar a la funcion 'MostrarSiguiente' (explicada mas debajo), para crear la forma que vendria a continuacion de la ultima.
*/
void Partida::Generar(vector<vector<int>> &matrizCeldas){
	for(size_t i=0;i<matrizFormaSiguiente.size();i++){
		for(size_t j=0;j<matrizFormaSiguiente[i].size();j++) matrizFormaSiguiente[i][j] = 0; /*Limpiamos la matriz llenando todas sus
																				celdas de ceros '0', representacion de celda vacia.*/
	}
	switch(n_forma){ //Segun el numero generado, creamos una de las siguientes 7 formas.
	case 1:{Cuadrado cuadrado(matrizCeldas,true); valorForma = 1; break;} /*Guardamos la variable 'valorForma' para saber que tipo 
																			de objeto ha sido creado.*/
	case 2:{EleIzq eleIzq(matrizCeldas,true); valorForma = 2; break;}
	case 3:{EleDer eleDer(matrizCeldas,true); valorForma = 3; break;}
	case 4:{Linea linea(matrizCeldas,true); valorForma = 4; break;}
	case 5:{EseIzq eseIzq(matrizCeldas,true); valorForma = 5; break;}
	case 6:{EseDer eseDer(matrizCeldas,true); valorForma = 6; break;}
	default:{Te te(matrizCeldas,true); valorForma = 7; break;}}
	srand(time(0)); n_forma = 1+rand()%8; //Generamos otro numero para mostrar la forma que vendra a continuacion.
	MostrarSiguiente(n_forma); //Hacemos uso de la funcion 'MostrarSiguiente' para enseñarle al usuario que forma continua en la secuencia.
	Globales::CambiarEstado(2,false); //Cambiamos el estado de la variable para que no se cree otra forma.
}

/*
Este metodo es el encargado de verificar si una o varias fila estan completas. Ademas, si esta activo el modo 'Fila piedra', se
rellena un vector de tipo 'int' para almacenar que filas contienen objetos consolidados. Si este vector contiene numeros almacenados,
se llama al metodo 'GuardarTiempos' (explicado mas debajo) para almacenar el tiempo de cada fila.
*/
void Partida::VerificarLinea(vector<vector<int>> &matrizCeldas){
	vectorJotas.resize(0); //En este vector de int guardaremos todas las filas que se encuenten completas en la matriz.
	for(int j=0;j<filas;j++){
		lineaLlena = 0; //Resetamos el contador de filas con objetos consolidados.
		for(int i=0;i<columnas;i++){
			if(matrizCeldas[i][j]>1) lineaLlena++; /*Contamos cada columna de cada fila, chequeando si esta, adentro, contiene un
													numero mayor al uno '1', lo que significaria que ahi hay una forma consolidada.*/
		}
		if(lineaLlena==10) {vectorJotas.push_back(j); lineaLlena=0;} /*Si el contado llego a 10, significa que la fila esta completa,
																	y, por lo tanto, guardamos de que fila se trata.*/
		else if(lineaLlena>0){ //Verificamos que, aunque la fila no este completa, pueda tener alguna forma consolidada.
			if(Globales::VerEstado(14)){ //Verificamos si el modo 'Fila piedra' este activado.
				for(size_t i=0;i<vectorFilas.size();i++){
					if(vectorFilas[i]==j) {puedeEntrar = false; break;} /*Chequeamos que la fila a guardar no se haya almacenado 
																		con anterioridad.*/
				}
				if(puedeEntrar) guardarFila.push_back(j); //Si no se ha almecenado anteriormente, la guardamos (al numero de la fila).
				else puedeEntrar = true;
			}
		}
	}
	if(vectorJotas.size()>0) borrarLinea = true; /*Si al finalizar la evaluacion, corroboramos que por lo menos hay una fila llena,
												cambiamos el estado de la variable de tipo bool 'borrarLinea', para que corran
												los metodos adecuados.*/
	else { //Sino hay ninguna fila completa:
		borrarLinea = false; //Cambiamos el estado de 'borrarLinea' para que no se realicen acciones especiales.
		if(Globales::VerEstado(14)){ //Verificamos el modo 'Fila piedra' este activado.
			for(size_t i=guardarFila.size();i>0;i--) vectorFilas.push_back(guardarFila[i-1]); /*Guardamos en el vector tipo 'int',
																							'vectorFilas', el numero de la/s fila/s
																							con formas consolidadas.*/
			if(vectorFilas.size()>0){ /*Si el vector de filas con formas consolidadas no esta vacio, empezamos y guardamos el tiempo
									de cada fila con el metodo 'GuardarTiempos'.*/
				GuardarTiempos();
				guardarFila.resize(0);
			}
		}
	}
}

/*
Este metodo es el encargado de 'limpiar' la o las filas completas en la matriz. A su vez, llama al metodo 'BajarCeldas' 
(explicado mas debajo) para cambiar el estado de la matriz, dando ilusion de caida de las formas. Si el modo 'Fila piedra'
esta activado, llama al metodo 'EliminarTiempos' (explicado mas debajo) para borrar el numero y los tiempos de las filas completadas.
*/
void Partida::LimpiarLinea(vector<vector<int>> &matrizCeldas){
	for(size_t j=0;j<vectorJotas.size();j++){
		for(int i=0;i<columnas;i++) matrizCeldas[i][vectorJotas[j]] = 0; /*Se coloca un cero '0' en cada columna de una fila para
																		limpiar la matriz.*/
		//Si el modo 'Fila piedra' esta activado'.
		if(Globales::VerEstado(14)) EliminarTiempos(vectorJotas[j],-1,true);
	}
	BajarCeldas(matrizCeldas);
}

/*
Este metodo 'baja' todas las formas consolidadas por encima de la o las filas limpiadas, con el objetivo de dar iusion de caida
de las celdas. Esto es posible mediante la modificacion numerica de la matriz.
*/
void Partida::BajarCeldas(vector<vector<int>> &matrizCeldas){
	for(size_t cantLineas = 0;cantLineas<vectorJotas.size();cantLineas++){ //Cantidad de filas completadas.
		for(int j=vectorJotas[cantLineas]-1;j>0;j--){ /*Desde la fila que se encuentra arriba de la primera fila limpiada, hasta el 
														final superior de la matriz, se realiza este for.*/
			for(int i=0;i<columnas;i++){ 
				if(matrizCeldas[i][j]>1){ /*Si corroboramos que nos encontramos con una celda que contenga un numero mayor a uno '1',
											significando que hay una forma consolidada, realizamos la siguientes acciones:
												1) Guardamos el valor que contenga la matriz en esa  posicion en la misma columna 
												pero en una fila superior.
												2) Borramos el valor contenido viejo, colocando un cero '0' en la matriz.*/
					matrizCeldas[i][j+1] = matrizCeldas[i][j];
					matrizCeldas[i][j] = 0;
				}
			}
		}
	}
}

//Este metodo es el encargado de crear la visualizacion de la forma que vendra a continuacion, con el objetivo de ayudar al usuario.
void Partida::MostrarSiguiente(int n_forma){
	switch(n_forma){ //Segun el numero creado, se elige la forma a crear.
	case 1:{Cuadrado cuadradoDos(matrizFormaSiguiente,false); break;} //Forma tipo 'Cuadrado'.
	case 2:{EleIzq eleIzqDos(matrizFormaSiguiente,false); break;} //Forma tipo 'EleIzq'.
	case 3:{EleDer eleDerDos(matrizFormaSiguiente,false); break;} //Forma tipo 'EleDer'.
	case 4:{Linea lineaDos(matrizFormaSiguiente,false); break;} //Forma tipo 'Linea'.
	case 5:{EseIzq eseIzqDos(matrizFormaSiguiente,false); break;} //Forma tipo 'EseIzq'.
	case 6:{EseDer eseDerDos(matrizFormaSiguiente,false); break;} //Forma tipo 'EseDer'.
	default:{Te teDos(matrizFormaSiguiente,false); break;}} //Forma tipo 'Te'.
}

/*Este metodo es el encargado de crear los distintos RectangleShape que no corresponden a una matriz. En esta funcion se setean 
diferentes parametros, como:
	1) Su tamaño.
	2) Su color.
	3) En algunos casos el borde.
	4) Su posicion.
'ContenedorSiguienteForma': es el RectangleShape donde se dibujara y mostrara la forma que viene a continuacion de la ultima creada.
'FondoSegundaVentana': es el RectangleShape que se encuentra a la derecha del RectangleShape principal por donde corre el juego.
						En el se dibuja distintos textos informativos y el RectangleShape 'ContenedorSiguienteForma'.
'FondoTercerVentana': es el RectangleShape que se encuentra a la izquierda del RectangleShape prinicipal por donde corre el juego.
						En el se dibuja distintos textos informativos y la imagen de Play/Pausa.
'FondoPausa': es el RectangleShape que se dibuja cuando el usuario pone en Pausa el juego. En el se dibuja texto informativo.
'FondoPreguntar': es el RectangleShape que aparece cuando el usuario teclea 'Escape'. En el se dibuja distintos textos informativos.
'Sombra': es el RectangleShape que mustra en que lugar se consolidaria la forma una vez llegue al final de la matriz o 'choque' con otra.
*/
void Partida::CrearRectangulos(){
	ContenedorSiguienteForma.setSize({tamanioCuadrado*6,tamanioCuadrado*6});
	ContenedorSiguienteForma.setFillColor(Color(180,180,180,50));
	ContenedorSiguienteForma.setOutlineThickness(2);
	ContenedorSiguienteForma.setPosition(tamanioCuadrado*columnas*2.2,tamanioCuadrado*(((filas/2)/2)/2));
	FondoSegundaVentana.setSize({tamanioCuadrado*columnas,tamanioCuadrado*filas});
	FondoSegundaVentana.setPosition(tamanioCuadrado*columnas*2,0);
	FondoSegundaVentana.setFillColor(Color(102,178,255,50));
	Sombra.setSize({tamanioCuadrado-1,tamanioCuadrado-1});
	if(Globales::VerEstado(7)) Sombra.setFillColor(Color(204,255,255));
	else Sombra.setFillColor(Color(204,255,255,50));
	FondoTercerVentana.setSize({tamanioCuadrado*columnas-1,tamanioCuadrado*filas});
	FondoTercerVentana.setPosition(0,0);
	FondoTercerVentana.setFillColor(Color(102,255,102,50));
	FondoPausa.setSize({tamanioCuadrado*columnas*3,tamanioCuadrado*filas});
	FondoPausa.setFillColor(Color(192,192,192,50));
	FondoPausa.setPosition(0,0);
	FondoPreguntar.setSize({tamanioCuadrado*columnas,tamanioCuadrado*(filas/2/2)});
	FondoPreguntar.setFillColor(Color(204,153,255));
	FondoPreguntar.setPosition(tamanioCuadrado*columnas,tamanioCuadrado*(filas/2/2/2));
	FondoPreguntar.setOutlineThickness(-1);
}

//Este metodo y el siguiene, 'AspectosTextoGenerales', son los encargados se crear y setear los distintos textos utilziados en la partida.
void Partida::CrearTexto(){ //En algunos casos se setea la posicion. En los que no, estos son seteados en otras funciones.
	AspectosTextoGenerales(vectorTexto[0],fuente.VerFuenteOpciones(),"Siguiente",true,18); vectorTexto[0].setPosition(tamanioCuadrado*(columnas+1.2)*1.88,0.70);
	AspectosTextoGenerales(vectorTexto[1],fuente.VerFuenteOpciones(),"Mejor Puntaje",true,13); vectorTexto[1].setPosition(tamanioCuadrado*(columnas+0.5)*1.95,filas*3.5);
	AspectosTextoGenerales(vectorTexto[2],fuente.VerFuenteOpciones(),"",false,18);
	AspectosTextoGenerales(vectorTexto[3],fuente.VerFuenteOpciones(),"Score",true,18); vectorTexto[3].setPosition(tamanioCuadrado*(columnas+2.8)*1.77,filas*4.8);
	AspectosTextoGenerales(vectorTexto[4],fuente.VerFuenteOpciones(),m_puntaje,false,18);
	AspectosTextoGenerales(vectorTexto[5],fuente.VerFuenteOpciones(),"Lineas Despejadas",true,11.5); vectorTexto[5].setPosition(tamanioCuadrado*(columnas)*2+2,filas*6);
	AspectosTextoGenerales(vectorTexto[6],fuente.VerFuenteOpciones(),"0",false,18); vectorTexto[6].setPosition(tamanioCuadrado*(columnas+4.55)*1.68,filas*6.4);
	AspectosTextoGenerales(vectorTexto[7],fuente.VerFuenteOpciones(),"Dificultad",true,14); vectorTexto[7].setPosition(tamanioCuadrado*(columnas+1.75)*1.83,filas*7);
	AspectosTextoGenerales(vectorTexto[8],fuente.VerFuenteOpciones(),"",false,18);
	AspectosTextoGenerales(vectorTexto[9],fuente.VerFuenteOpciones(),"Tiempo jugado",true,14); vectorTexto[9].setPosition(2,tamanioCuadrado*(filas/2)-25);
	AspectosTextoGenerales(vectorTexto[10],fuente.VerFuenteOpciones(),"00:00:00:00",false,15); vectorTexto[10].setPosition(tamanioCuadrado-2,tamanioCuadrado*(filas/2)-5);
	AspectosTextoGenerales(vectorTexto[11],fuente.VerFuenteOpciones(),"Velocidad de caida",true,11.5); vectorTexto[11].setPosition(1,tamanioCuadrado*(filas/2)+15);
	AspectosTextoGenerales(vectorTexto[12],fuente.VerFuenteOpciones(),"",false,13);
	AspectosTextoGenerales(vectorTexto[13],fuente.VerFuenteOpciones(),"Velocidad fila piedra",true,9.5); vectorTexto[13].setPosition(2,tamanioCuadrado*(filas/2)+50);
	AspectosTextoGenerales(vectorTexto[14],fuente.VerFuenteOpciones(),"",false,16); vectorTexto[14].setPosition(4.5,tamanioCuadrado*(filas/2)+63);
	AspectosTextoGenerales(vectorTexto[15],fuente.VerFuenteOpciones(),"PAUSA",false,31); vectorTexto[15].setPosition(tamanioCuadrado*columnas+(columnas/2),tamanioCuadrado*(filas/2));
	AspectosTextoGenerales(vectorTexto[16],fuente.VerFuenteOpciones(),"Desea salir?",true,14); vectorTexto[16].setPosition(tamanioCuadrado*columnas+5,tamanioCuadrado*(filas/2/2/2)+5);
	AspectosTextoGenerales(vectorTexto[17],fuente.VerFuenteOpciones(),"SI",false,14); vectorTexto[17].setPosition(tamanioCuadrado*columnas+10,tamanioCuadrado*(filas/2/2/2)+25); vectorTexto[15].setFillColor(Color::Red);
	AspectosTextoGenerales(vectorTexto[18],fuente.VerFuenteOpciones(),"NO",false,14); vectorTexto[18].setPosition(tamanioCuadrado*columnas+55,tamanioCuadrado*(filas/2/2/2)+25); vectorTexto[16].setFillColor(Color::Red);
}

/*Con esta funcion se setean aspectos del texto como:
	1) Su fuente.
	2) Su contenido.
	3) Su tamaño.
	4) Su escala.
	5) Su color.
*/
void Partida::AspectosTextoGenerales(Text &texto,Font &fuente,string titulo, bool esTitulo,int tamanio){
	texto.setFont(fuente); texto.setString(titulo); texto.setCharacterSize(tamanio); texto.setScale(0.5,0.5);
	if(esTitulo) texto.setFillColor(Color::Yellow);
	else texto.setFillColor(Color::White);
}

/*
Este metodo es el encargado de cambiar el contenido de los distintos textos mostrados en pantalla, con el objetivo de ir informando
al usuario de su progreso. Tambien, si es debido, disminuye el tiempo que tarda la forma en bajar una celda en la matriz. 
En esta funcion se realizan los cambios de:
	1) El tiempo que tarda la forma en bajar una celda.
	2) El texto que muestra el tiempo que tarda la forma en bajar una celda.
	3) El puntaje del usuario.
	4) Las cantidad de filas que el usuario va despejando.
	5) Verificar si el puntaje hecho por el usuario ha superado al mas alto en el momento.
En todos los casos se corrobora el tamaño de la cadena de texto, con el objetivo de modificar la posicion de estos, de manera de 
dejar lo mas centrado posible la informacion.
*/
void Partida::ModificarVariables(int n){
	if(n==1){ //Corrobora si, ademas del puntaje, hay que modificar otros textos.
		for(size_t i=0;i<vectorJotas.size();i++) {
			score = score + (100*(i+1)) + ((i+1)*5); /*Realizamos operaciones aritmeticas para calcular cuanto se le suma al
													puntaje del usuario por haber limpiado una o varias filas.*/
			contadorLineas++; //Contamos las filas despejadas por el usuario.
		}
		m_puntaje = to_string(score); //Convertimos el puntaje del usuario de int a string.
		m_lineas = to_string(contadorLineas); //Convertimos la cantidad de filas despejadas por el usuario de int a string.
		switch(Globales::VerDificultad()){ /*Corroboramos en que dificultad se encuentra el juego y, dependiendo de la variable
											'tiempoDeCaida' y la cantidad de lineas despejadas, reducimos el tiempo que tarda la forma 
											en bajar una celda en la matriz.*/
		case 0:{if(tiempoDeCaida>100 && contadorLineas%2==0) tiempoDeCaida-=15; cambioTiempo = true; break;}
		case 1:{if(tiempoDeCaida>100 && contadorLineas%3==0) tiempoDeCaida-=28; cambioTiempo = true; break;}
		default:{if(tiempoDeCaida>100 && contadorLineas%4==0) tiempoDeCaida-=36; cambioTiempo = true; break;}}
		if(cambioTiempo){ /*Si el tiempo de caida ha sido modificado, le mostramos al usuario el cambio, modificando el texto y 
							seteando la posicion, de manera de dejar lo mas centrado posible la informacion.*/
			if(fuente.EstadoFuenteOpciones()){ //Verificamos si la fuente ha sido cargada con normalidad.
				if(tiempoDeCaida>=1100) {
					vectorTexto[12].setString("1."+to_string(tiempoDeCaida%1000)+" segundos");
					vectorTexto[12].setPosition(2,tamanioCuadrado*(filas/2)+30);
				}
				else if(tiempoDeCaida>1000 && tiempoDeCaida<1100){
					vectorTexto[12].setString("1.0"+to_string(tiempoDeCaida%100)+" segundos");
					vectorTexto[12].setPosition(2,tamanioCuadrado*(filas/2)+30);
				}
				else if(tiempoDeCaida==1000) {
					vectorTexto[12].setString("1 segundo"); 
					vectorTexto[12].setPosition(tamanioCuadrado*2,tamanioCuadrado*(filas/2)+30);
				}
				else if(tiempoDeCaida>100 && tiempoDeCaida<1000){
					vectorTexto[12].setString("0."+to_string(tiempoDeCaida)+" segundos"); 
					vectorTexto[12].setPosition(1,tamanioCuadrado*(filas/2)+30);
				}
				else{
					vectorTexto[12].setString("0.0"+to_string(tiempoDeCaida)+" segundos"); 
					vectorTexto[12].setPosition(0,tamanioCuadrado*(filas/2)+30);
				}
			}
			cambioTiempo = false; //Cambiamos el estado de la variable para que las acciones anteriores no se realicen mas.
		}
		if(fuente.EstadoFuenteOpciones()){ //Verificamos si la fuente ha sido cargada con normalidad.
			vectorTexto[4].setString(m_puntaje); //Cambiamos el contenido del texto que muestra el puntaje del usuario.
			switch(m_puntaje.length()){ /*Dependiendo del largo de la cadena de texto del puntaje del usuario, seteamos su posicion en
										una zona distinta, de manera de dejar lo mas centrado posible la informacion.*/
			case 1: {vectorTexto[4].setPosition(tamanioCuadrado*(columnas+4.5)*1.69,filas*5.4); break;} 
			case 2: {vectorTexto[4].setPosition(tamanioCuadrado*(columnas+4.36)*1.67,filas*5.4); break;}
			case 3:	{vectorTexto[4].setPosition(tamanioCuadrado*(columnas+4.16)*1.67,filas*5.4); break;}
			case 4:	{vectorTexto[4].setPosition(tamanioCuadrado*(columnas+4.01)*1.66,filas*5.4); break;}
			case 5:	{vectorTexto[4].setPosition(tamanioCuadrado*(columnas+3.86)*1.65,filas*5.4); break;}
			case 6:	{vectorTexto[4].setPosition(tamanioCuadrado*(columnas+3.71)*1.64,filas*5.4); break;}
			case 7:	{vectorTexto[4].setPosition(tamanioCuadrado*(columnas+3.56)*1.63,filas*5.4); break;}}
			vectorTexto[6].setString(m_lineas); //Cambiamos el contenido del texto que muestra las lineas despejadas por el usuario.
			switch(m_lineas.length()){ /*Dependiendo del largo de la cadena de texto de las lineas despejadas por el usuario, 
								seteamos su posicion en una zona distinta, de manera de dejar lo mas centrado posible la informacion.*/
			case 1: {vectorTexto[6].setPosition(tamanioCuadrado*(columnas+4.80)*1.67,filas*6.4); break;} 
			case 2:	{vectorTexto[6].setPosition(tamanioCuadrado*(columnas+4.55)*1.66,filas*6.4); break;}
			case 3:	{vectorTexto[6].setPosition(tamanioCuadrado*(columnas+4.35)*1.65,filas*6.4); break;}
			case 4:	{vectorTexto[6].setPosition(tamanioCuadrado*(columnas+4.10)*1.65,filas*6.4); break;}
			case 5:	{vectorTexto[6].setPosition(tamanioCuadrado*(columnas+3.90)*1.65,filas*6.4); break;}}
		}
	}
	else{ //Solo se debe modificar el puntaje.
		m_puntaje = to_string(score); //Convertimos el puntaje del usuario de int a string.
		if(fuente.EstadoFuenteOpciones()){ //Verificamos que la fuente haya sido cargada con normalidad.
			vectorTexto[4].setString(m_puntaje); //Cambiamos el contenido del texto que muestra el puntaje del usuario.
			switch(m_puntaje.length()){ /*Dependiendo del largo de la cadena de texto de las lineas despejadas por el usuario, 
								seteamos su posicion en una zona distinta, de manera de dejar lo mas centrado posible la informacion.*/
			case 1: {vectorTexto[4].setPosition(tamanioCuadrado*(columnas+4.5)*1.68,filas*5.4); break;}
			case 2: {vectorTexto[4].setPosition(tamanioCuadrado*(columnas+4.36)*1.66,filas*5.4); break;}
			case 3:	{vectorTexto[4].setPosition(tamanioCuadrado*(columnas+4.16)*1.66,filas*5.4); break;}
			case 4:	{vectorTexto[4].setPosition(tamanioCuadrado*(columnas+4.01)*1.66,filas*5.4); break;}
			case 5:	{vectorTexto[4].setPosition(tamanioCuadrado*(columnas+3.86)*1.65,filas*5.4); break;}
			case 6:	{vectorTexto[4].setPosition(tamanioCuadrado*(columnas+3.71)*1.64,filas*5.4); break;}
			case 7:	{vectorTexto[4].setPosition(tamanioCuadrado*(columnas+3.56)*1.63,filas*5.4); break;}}
		}
	}
	if(score>mejorPuntaje && puedeFestejar && Globales::VerEstado(9)) {  /*Condicional que verifica si el usuario ha superado al
																		puntaje mas alto en el momento de jugar. Si lo ha hecho,
																		verifica si la musica esta activada y, si es la primera vez 
																		que supera al puntaje, se reproduce un archivo musical.*/
		//Verificamos que el archivo musical se haya cargado con normalidad.
		if(vectorEstadoSonido[3]) punteroMusica[3].play();
		puedeFestejar = false; //Cambiamos el estado de la variable para que lo anterior no se reproduzca mas.
	}
}

/*
Este metodo es el encargado de verificar si el juego debe o no terminar. La verificacion la hace de la siguiente manera:
Se chequea que pieza vendra a continuacion. Dependiendo de cual sea, se verifica si, en la posicion indicada de la matriz, hay
o no una forma ya consolidada. Si no la hay, el juego continua. Si la hay, significa que en el momento de crearse la forma
siguiente, esta chocaria/destruiria el objeto ya consolidado. Por lo tanto, el juego termina.
*/
bool Partida::ChequearGameOver(int n_forma){
	switch(n_forma){
	case 1:{
		if(matrizCeldas[(columnas/2)-1][0]>1 || matrizCeldas[columnas/2][0]>1 /*Guardamos la variable 'esGameOver' para, una vez
																			chequeado el Game Over, no tener que realizarlo mas.*/
		|| matrizCeldas[(columnas/2)-1][1]>1 || matrizCeldas[columnas/2][1]>1) {esGameOver = true; return true;}
		break;}
	case 2:{
		if(matrizCeldas[columnas/2-1][1]>1 || matrizCeldas[columnas/2-1][0]>1
		|| matrizCeldas[columnas/2][1]>1 || matrizCeldas[columnas/2+1][1]>1) {esGameOver = true; return true;}
		break;}
	case 3:{
		if(matrizCeldas[columnas/2+1][1]>1 || matrizCeldas[columnas/2+1][0]>1
		|| matrizCeldas[columnas/2][1]>1 || matrizCeldas[columnas/2-1][1]>1) {esGameOver = true; return true;}
		break;}
	case 4:{
		if(matrizCeldas[columnas/2][1]>1 || matrizCeldas[columnas/2][0]>1
		|| matrizCeldas[columnas/2][2]>1 || matrizCeldas[columnas/2][3]>1) {esGameOver = true; return true;}
		break;}
	case 5:{
		if(matrizCeldas[columnas/2][0]>1 || matrizCeldas[columnas/2-1][0]>1
		|| matrizCeldas[columnas/2][1]>1 || matrizCeldas[columnas/2+1][1]>1) {esGameOver = true; return true;}
		break;}
	case 6:{
		if(matrizCeldas[columnas/2][0]>1 || matrizCeldas[columnas/2+1][0]>1
		|| matrizCeldas[columnas/2][1]>1 || matrizCeldas[columnas/2-1][1]>1) {esGameOver = true; return true;}
		break;}
	default:{
		if(matrizCeldas[columnas/2][0]>1 || matrizCeldas[columnas/2+1][0]>1
		|| matrizCeldas[columnas/2-1][0]>1 || matrizCeldas[columnas/2][1]>1) {esGameOver = true; return true;}
		break;}
	}
	return false;
}

/*
Este metodo es el encargado de setear el tiempo de caida inicial y el texto y la posicion que muestra la dificultad actual del juego,
dependiendo como lo haya seteado el usuario. Ademas, dependiendo si el modo 'Fila piedra' este activado o no, mostrara un texto
u otro.
*/
void Partida::ModificarDificultad(){
	switch(Globales::VerDificultad()){ //switch dependiendo la dificultad elegida.
	case 0: {
		tiempoDeCaida = 1500;  //Se setea el tiempo inicial de caida de las formas.
		if(Globales::VerEstado(14)) { //Si el modo 'Fila piedra' esta activado.
			tiempoPiedra = 90000;  //Se setea el tiempo del modo 'Fila piedra'.
			//Si la fuente se ha cargado con normalidad.
			if(fuente.EstadoFuenteOpciones()) vectorTexto[14].setString("90 segundos");
		}
		else {
			if(fuente.EstadoFuenteOpciones()) 
				vectorTexto[14].setString("Desactivado");{
				vectorTexto[14].setPosition(tamanioCuadrado-1,tamanioCuadrado*(filas/2)+63);
			}
		}
		if(fuente.EstadoFuenteOpciones()) vectorTexto[8].setPosition(tamanioCuadrado*(columnas+3.1)*1.75,filas*7.4); 
		break;} 
	case 1: {
		tiempoDeCaida = 1000; 
		if(Globales::VerEstado(14)) {
			tiempoPiedra = 60000; 
			if(fuente.EstadoFuenteOpciones()) vectorTexto[14].setString("60 segundos");
		}
		else {
			if(fuente.EstadoFuenteOpciones()) {
				vectorTexto[14].setString("Desactivado");
				vectorTexto[14].setPosition(tamanioCuadrado-1,tamanioCuadrado*(filas/2)+63);
			}
		}
		if(fuente.EstadoFuenteOpciones()) vectorTexto[8].setPosition(tamanioCuadrado*(columnas+2.8)*1.78,filas*7.4); 
		break;}
	default: {
		tiempoDeCaida = 600; 
		if(Globales::VerEstado(14)) {
			tiempoPiedra = 45000;
			if(fuente.EstadoFuenteOpciones()) vectorTexto[14].setString("45 segundos");
		}
		else {
			if(fuente.EstadoFuenteOpciones()) {
				vectorTexto[14].setPosition(tamanioCuadrado-1,tamanioCuadrado*(filas/2)+63);
				vectorTexto[14].setString("Desactivado");
			}
		} 
		if(fuente.EstadoFuenteOpciones()) vectorTexto[8].setPosition(tamanioCuadrado*(columnas+2.6)*1.78,filas*7.4); 
		break;}
	}
	if(fuente.EstadoFuenteOpciones()) vectorTexto[8].setString(Globales::VerCadena(8));
}

/*
Este metodo es el encargado de setear el texto y la posicion del mejor puntaje hasta el momento hecho, haciendo uso del metodo
'VerMejorPuntaje' de la clase 'Archivo'.
*/
void Partida::MostrarMejorPuntaje(){
	archivo.VerMejorPuntaje(Globales::VerDificultad(),mejorPuntaje,m_mejorPuntaje);
	if(mejorPuntaje>=0){
		if(fuente.EstadoFuenteOpciones()){ //Verifica si la fuente ha sido cargado con normalidad.
			vectorTexto[2].setString(m_mejorPuntaje); //Seteamos el contenido del texto que muestra el mejor puntaje.
			switch(m_mejorPuntaje.length()){  /*Dependiendo del largo de la cadena de texto del mejor puntaje hecho, seteamos su
											posicion en una zona distinta, de manera de dejar lo mas centrado posible la informacion.*/
			case 1: {vectorTexto[2].setPosition(columnas*(columnas+1.35)*1.71,filas*4.1); break;}
			case 2: {vectorTexto[2].setPosition(columnas*(columnas+1.15)*1.71,filas*4.1); break;}
			case 3: {vectorTexto[2].setPosition(columnas*(columnas+1.05)*1.70,filas*4.1); break;}
			case 4: {vectorTexto[2].setPosition(columnas*(columnas+0.95)*1.69,filas*4.1); break;}
			case 5: {vectorTexto[2].setPosition(columnas*(columnas+0.85)*1.68,filas*4.1); break;}
			case 6: {vectorTexto[2].setPosition(columnas*(columnas+0.75)*1.68,filas*4.1); break;}
			case 7: {vectorTexto[2].setPosition(columnas*(columnas+0.65)*1.68,filas*4.1); break;}}
		}
	}
}

/*
Este metodo es el encargado de cambiar el color de los RectangleShape dentro del vector 'celdasFormas', con el objetivo de modificar,
justamente, el color de los objetos dentro de la matriz, de manera de brindar mas ayuda visual al usuario para darle a entender que
la partida termino.
*/
void Partida::CambiarColorFormas(){
	for(int i=1;i<celdasFormas.size();i++) celdasFormas[i].setFillColor(Color(192,192,192));
}

//Este metodo es el encargado de variar los colores de las celdas que van a ser 'limpiadas' (debido a que la fila esta completa).
void Partida::EfectoLinea(vector<vector<int>> &matrizCeldas){
	celdasFormas[8].setFillColor(Color(rand()%255,rand()%255,rand()%255));
}

/*
Este metodo es el encargado de cambiar el contenido numerico de la matriz a nueve '9'. El cambio se produce en las filas que han sido 
completadas.
*/
void Partida::CambiarNumeros(){
	for(size_t j=0;j<vectorJotas.size();j++){
		for(int i=0;i<columnas;i++) matrizCeldas[i][vectorJotas[j]] = 9;
	}
}

/*
Este metodo se utiliza si la funcion 'Fila piedra' esta activada. Este es el encargado de cambiar el contenido numerico de la 
matriz a menos uno '-1' o menos dos '-2' dependiendo el numero que contenga cada celda. El cambio se produce en filas con 
formas consolidadas que no han sido completadas a tiempo.
*/
void Partida::CambiarNumeros(int j){
	for(size_t i=0;i<columnas;i++) {
		if(matrizCeldas[i][j]>1) matrizCeldas[i][j] = -1;
		else matrizCeldas[i][j] = -2;
	}
}

//Este metodo es el encargado de cargar los archivos musicales a utilizar en la partida.
void Partida::CargarMusica(){
	if(Globales::VerEstado(9) || Globales::VerEstado(10)) { //Verifica si la musica y/o los efectos estan activados.
		if(Globales::VerEstado(9) && Globales::VerEstado(10)) { //Verifica si la musica y los efectos estan activados.
			cantMusica = 9; //Cambiamos el valor de 'cantMusica' para indicar cuantos archivos musicales han de crearse.
			punteroMusica = new Music[cantMusica];  //Creamos un vector de objetos tipo 'Music' en el puntero 'punteroMusica'.
			vectorEstadoSonido.resize(9);
			if(!punteroMusica[0].openFromFile("Musica/Original Tetris theme.wav")) { //Chequeamos si el archivo pudo o no cargarse.
				cerr<<"No se pudo encontrar el archivo musical 'Original Tetris theme.wav'"<<endl;
				vectorEstadoSonido[0] = false; //Guardamos el estado del archivo, dependiendo si este pudo o no cargarse.
			}
			else {
				punteroMusica[0].setVolume(15); punteroMusica[0].setLoop(true); punteroMusica[0].play(); 
				vectorEstadoSonido[0] = true;
			}
			if(!punteroMusica[1].openFromFile("Musica/Bradinsky.flac")) {
				cerr<<"No se pudo encontrar el archivo musical 'Bradinsky.flac'"<<endl;
				vectorEstadoSonido[1] = false;
			}
			else {
				punteroMusica[1].setVolume(55);
				vectorEstadoSonido[1] = true;
			}
			if(!punteroMusica[3].openFromFile("Musica/SpeedUp.wav")) {
				cerr<<"No se pudo encontar el archivo 'SpeedUp.wav'"<<endl;
				vectorEstadoSonido[3] = false;
			}
			else {
				punteroMusica[3].setVolume(35);
				vectorEstadoSonido[3] = true;
			}
			if(!punteroMusica[4].openFromFile("Musica/Move.wav")){
				cerr<<"No se pudo encontrar el archivo 'Move.wav'"<<endl;
				vectorEstadoSonido[4] = false;
			}
			else{
				punteroMusica[4].setVolume(45); 
				vectorEstadoSonido[4] = true;
			}
			if(!punteroMusica[5].openFromFile("Musica/Clear.wav")) {
				cerr<<"No se pudo encontrar el archivo 'Clear.wav'"<<endl;
				vectorEstadoSonido[5] = false;
			}
			else {
				punteroMusica[5].setVolume(70);
				vectorEstadoSonido[5] = true;
			}
			if(!punteroMusica[6].openFromFile("Musica/Shhhs.wav")) {
				cerr<<"No se pudo encontrar el archivo 'Shhh.wav'"<<endl;
				vectorEstadoSonido[6] = false;
			}
			else {
				punteroMusica[6].setVolume(100); punteroMusica[6].setLoop(true);
				vectorEstadoSonido[6] = true;
			}
			if(!punteroMusica[7].openFromFile("Musica/HardDrop.wav")) {
				cerr<<"No se pudo encontar el archivo 'HardDrop.wav'"<<endl;
				vectorEstadoSonido[7] = false;
			}
			else {
				punteroMusica[7].setVolume(25);
				vectorEstadoSonido[7] = true;
			}
			if(!punteroMusica[8].openFromFile("Musica/Stone.wav")){
				cerr<<"No se pudo encontrar el archivo 'Stone.waw'"<<endl;
				vectorEstadoSonido[8] = false;
			}
			else {
				punteroMusica[8].setVolume(30);
				vectorEstadoSonido[8] = true;
			}
		}
		else if(Globales::VerEstado(9)) {
			cantMusica = 4;
			punteroMusica = new Music[cantMusica]; 
			vectorEstadoSonido.resize(4);
			if(!punteroMusica[0].openFromFile("Musica/Original Tetris theme.wav")) {
				cerr<<"No se pudo encontrar el archivo musical 'Original Tetris theme.wav'"<<endl;
				vectorEstadoSonido[0] = false;
			}
			else {
				punteroMusica[0].setVolume(15); punteroMusica[0].setLoop(true); punteroMusica[0].play();
				vectorEstadoSonido[0] = true;
			}
			if(!punteroMusica[1].openFromFile("Musica/Bradinsky.flac")) {
				cerr<<"No se pudo encontrar el archivo musical 'Bradinsky.flac'"<<endl;
				vectorEstadoSonido[1] = false;
			}
			else {
				punteroMusica[1].setVolume(55); 
				vectorEstadoSonido[1] = true;
			}
			if(!punteroMusica[3].openFromFile("Musica/SpeedUp.wav")) {
				cerr<<"No se pudo encontar el archivo 'SpeedUp.wav'"<<endl;
				vectorEstadoSonido[3] = false;
			}
			else {
				punteroMusica[3].setVolume(35);
				vectorEstadoSonido[3] = true;
			}
		}
		else {
			cantMusica = 5;
			punteroMusica = new Music[cantMusica]; 
			vectorEstadoSonido.resize(5);
			if(!punteroMusica[0].openFromFile("Musica/Move.wav")) {
				cerr<<"No se pudo encontrar el archivo 'Move.wav'"<<endl;
				vectorEstadoSonido[0] = false;
			}
			else {
				punteroMusica[0].setVolume(45); 
				vectorEstadoSonido[0] = true;
			}
			if(!punteroMusica[1].openFromFile("Musica/Clear.wav")) {
				cerr<<"No se pudo encontrar el archivo 'Clear.wav'"<<endl;
				vectorEstadoSonido[1] = false;
			}
			else {
				punteroMusica[1].setVolume(70);
				vectorEstadoSonido[1] = true;
			}
			if(!punteroMusica[2].openFromFile("Musica/Shhhs.wav")) {
				cerr<<"No se pudo encontrar el archivo 'Shhh.wav'"<<endl;
				vectorEstadoSonido[2] = false;
			}
			else {
				punteroMusica[2].setVolume(100); punteroMusica[2].setLoop(true);
				vectorEstadoSonido[2] = true;
			}
			if(!punteroMusica[3].openFromFile("Musica/HardDrop.wav")) {
				cerr<<"No se pudo encontar el archivo 'HardDrop.wav'"<<endl;
				vectorEstadoSonido[3] = false;
			}
			else {
				punteroMusica[3].setVolume(25);
				vectorEstadoSonido[3] = true;
			}
			if(!punteroMusica[4].openFromFile("Musica/Stone.wav")){
				cerr<<"No se pudo encontrar el archivo 'Stone.wav'"<<endl;
				vectorEstadoSonido[4] = false;
			}
			else {
				punteroMusica[4].setVolume(30);
				vectorEstadoSonido[4] = true;
			}
		}
	}
}

//Este metodo es el encargado de corroborar si el usuario ha hecho click en la imagen de Play/Pausa.
bool Partida::CliqueoPausa(RenderWindow &ventana){
	if(relojMouse.getElapsedTime().asMilliseconds()>150){ //Si el tiempo supera al establecido.
		posicionMouse = Mouse::getPosition(ventana); //Obtenemos la posicion x-y del mouse.
		if(posicionMouse.x>=80 && posicionMouse.x<=240 && posicionMouse.y>=40 && posicionMouse.y<=200){ /*Si la posicion del mouse se 
																									encuentra dentro de los marges de la 
																									imagen, devolvemos true, sino false.*/
			if(Globales::VerEstado(11)){ //Si el juego se encuentra en estado de Pausa, realizamos esta accion.
				Globales::CambiarEstado(11,false); //Cambiamos el estado que verifica si el juego se encuentra en pausa o no.
				Globales::CambiarEstado(12,true); //Cambiamos el estado que verifica si el juego estuvo en pausa.
				relojMouse.restart(); /*Reiniciamos el reloj.*/ return true;
			}
			else{ //Si el juego se encuentra en estado de Play, realizamos esta accion.
				Globales::CambiarEstado(11,true); primeraVezPausa = true;
				relojMouse.restart(); return true;
			}
		} 
		else {relojMouse.restart(); return false;}
	}
}

/*
Este metodo es el encargado de verificar el tiempo transcurrido y convertir las distintas variables de tipo int en una cadena de
texto, mediante el objeto 'ss' de la clase 'stringstream', para, luego, cargarlo en una variable de tipo string.
*/
void Partida::ObtenerTiempo(){
	ss.str(""); ss.clear(); int recorteMili; //Limpiamos el contenido anterior del objeto 'ss'.
	tiempo = relojTiempo.getElapsedTime(); //Obtenemos el tiempo transcurrido hasta este momento.
	mili = tiempo.asMilliseconds(); //Convertimos el tiempo obtenido en milisegundos.
	//Si los milisegundos transcurridos son menores a 10, devolvemos la siguiente cadena de texto.
	if(mili<10) {ss << "0" << hora << ":" << "0" << min << ":" << "0" << seg << ":" << "0" << mili; ss >> tiempoCadena;}
	else{
		if(mili>=1000) {seg++; mili = 0; relojTiempo.restart();} /*Si los milisegunos transcurridos son iguales a 1000, sumamos
																1 segundo y reiniciamos los milisegundos y el reloj.*/
		if(mili>=100) recorteMili = mili/10; /*Si los milisegundos transcurridos son igual o mayores a 100, recortamos una de sus
											ultimas cifras.*/
		else recorteMili = mili;
		//Si los segundos transcurridos son menores a 10, devolvemos la siguiente cadena de texto.
		if(seg<10) {ss << "0" << hora << ":" << "0" << min << ":" << "0" << seg << ":" << recorteMili; ss >> tiempoCadena;}
		else{
			if(seg>=60) {min++; seg = 0;} /*Si los segundos transcurridos son iguales a 60, sumamos 1 minuto y reiniciamos los
											segundos.*/
			//Si los minutos transcurridos son menores a 10, devolvemos la siguiente cadena de texto.
			if(min<10) {ss << "0" << hora << ":" << "0" << min << ":" << seg << ":" << recorteMili; ss >> tiempoCadena;}
			else{
				if(min>=60) {hora++; min = 0;} /*Si los minutos transcurridos son iguales a 60, sumamos 1 hora y reiniciamos los
												minutos.*/
				//Si las horas transcurridas son menores a 10, devolvemos la siguiente cadena de texto.
				if(hora<10) {ss << "0" << hora << ":" << min << ":" << seg << ":" << recorteMili; ss >> tiempoCadena;}
				else {ss << hora << ":" << min << ":" << seg << ":" << recorteMili; ss >> tiempoCadena;}
			}
		}
	}
}

/*
Este metodo es el encargado de setear el contenido del texto que muestra la velocidad de caida de la pieza y su posicion al
momento de iniciar el juego, dependiendo de la dificultad seteada por el usuario.
*/
void Partida::MostrarVelocidad(){
	if(tiempoDeCaida==1500) {
		if(fuente.EstadoFuenteOpciones()) { //Si la fuente ha sido cargada con normalidad.
			vectorTexto[12].setString("1.500 segundos"); 
			vectorTexto[12].setPosition(2,tamanioCuadrado*(filas/2)+30);
		}
	} 
	else if(tiempoDeCaida==1000) {
		if(fuente.EstadoFuenteOpciones()){
			vectorTexto[12].setString("1 segundo"); 
			vectorTexto[12].setPosition(tamanioCuadrado*2,tamanioCuadrado*(filas/2)+30);
		}
	}
	else {
		if(fuente.EstadoFuenteOpciones()) {
			vectorTexto[12].setString("0.600 segundos");
			vectorTexto[12].setPosition(1,tamanioCuadrado*(filas/2)+30);
		}
	}
}

/*
Este metodo es el encargado de verificar si el usuario ha cliqueado alguna de las 2 opciones disponibles, una vez haya apretado
la tecla 'Escape', de manera de hacer la acciones correspondientes.
*/
void Partida::RespuestaUsuario(RenderWindow &ventana){
	posicionMouse = Mouse::getPosition(ventana); //Obtenemos la posicion x-y del mouse.
	if(posicionMouse.x>=360 && posicionMouse.x<=400 && posicionMouse.y>=165 && posicionMouse.y<=192) {m_si = true; m_no = false;}
	else if(posicionMouse.x>=540 && posicionMouse.x<=590 && posicionMouse.y>=165 && posicionMouse.y<=192){m_si = false; m_no = true;}
}

/*
Este metodo es utilizado si el modo 'Fila piedra' esta activada. Es el encargado de ir actualizando y almacenando el tiempo de 
cada fila que contenga una forma consolidada.
*/
void Partida::GuardarTiempos(){
	if(numFilas<vectorFilas.size()){ /*Condicional que verifica si el numero de filas que anteriormente tenian formas consolidadas 
									es menor a la cantidad de filas con objetos consolidados actualmente.*/
		int auxiliar = numFilas; //Guardamos en una variable auxiliar el numero viejo de filas con objetos consolidados.
		numFilas = vectorFilas.size(); //Actualizamos el numero de filas con formas consolidadas.
		if(primeraVez){ /*Verificamos si es la primera vez que se llama la funcion, o si el numero de filas con objetos
						consolidados llego a ser de cero '0'.*/
			relojFilas = new Clock[numFilas]; //Creamos tantos objetos de tipo 'Clock' como filas con objetos consolidados haya.
			vectorTiempo.resize(numFilas); primeraVez = false; //Cambiamos el estado para que estas acciones no se repitan mas.
			for(int i=0;i<numFilas;i++) relojFilas[i].restart(); //Empezamos el tiempo de cada fila.
		}
		else{
			for(int i=0;i<auxiliar;i++) vectorTiempo[i] = relojFilas[i].getElapsedTime() + vectorTiempo[i]; /*Guardamos el tiempo de
																										cada reloj anteriormente creado,
																									de manera de no perder el tiempo una
																									vez reiniciado los relojes.*/
			relojFilas = new Clock[numFilas]; //Creamos tantos objetos tipo 'Clock' como filas con objetos consolidados haya.
			for(int i=0;i<numFilas;i++) {
				if(i+1>auxiliar) vectorTiempo.push_back(Time()); /*Para los nuevos relojes, debido a las nuevas filas, agrandamos
																el vector de tipo 'Time' 'vectorTiempo', que es el encargado de 
																guardar el tiempo de cada reloj antes de reiniciar estos.*/
				relojFilas[i].restart(); //Reiniciamos los relojes.
			}
		}
	}
}

/*
Este metodo es el encargado de llamar a las funciones 'CambiarNumeros' y 'EliminarTiempos' (explicado este ultimo mas debajo) si el 
tiempo transcurrido de una fila supera al establecido (dependiendo la dificultad del juego). Si los efectos estan activados, y si
se paso del tiempo establecido, se reproduce un archivo musical.
*/
void Partida::ChequearTiempoFilas(){
	for(int i=numFilas;i>0;i--){
		//Verificamos si la suma del tiempo actual del reloj mas el guardado antes de su reinicio supera al establecido..
		if(relojFilas[i-1].getElapsedTime().asMilliseconds() + vectorTiempo[i-1].asMilliseconds()>=tiempoPiedra) {
			CambiarNumeros(vectorFilas[i-1]);
			EliminarTiempos(vectorFilas[i-1],i-1,false);
			//Corrobora que los efectos esten activados.
			forma.LimpiarSombra(matrizCeldas); /*Llamamos al metodo 'LimpiarSombra' de la clase 'Formas' para eliminar la sombra actual,
												ya que una fila se ha modificado.*/
			forma.MuestraPieza(matrizCeldas,caidaRapida); /*Llamos al metodo 'MuestraPieza' de la clase 'Formas' para dibujar nuevamente
															la sombra.*/ 
			if(Globales::VerEstado(9) && Globales::VerEstado(10)) punteroMusica[8].play();
			else if(Globales::VerEstado(10)) punteroMusica[4].play();
			Globales::CambiarEstado(13,true);
		}
	}
}

/*
Este metodo es el encargado de actualizar los vectores 'vectorFilas' y 'vectorTiempo, guardando (o no) el tiempo
de los demas relojes, y reiniciando (o no) otros. Esto dependera de si se ha completado o no una fila, y si la fila que se
ha cambiar tiene o no filas arriba de ella.
*/
void Partida::EliminarTiempos(int fila,int posicion,bool seEliminoFila){
	if(numFilas>1){ //Verificamos si el numero de filas con objetos consolidados es mayor a uno '1'.
		if(posicion==-1){ //Sino sabemos la posicion de la fila que debe de ser cambiada, la buscamos con el siguiente 'for'.
			for(int i=0;i<numFilas;i++){
				if(vectorFilas[i]==fila) posicion = i; //Guardamos la fila que debe ser cambiada en la variable 'posicion'.
			}
		}
		//Verificamos si la posicion de la fila que debe ser cambiada no es ni la primera ni la ultima del vector 'vectorFilas'.
		if(vectorFilas[posicion]!=vectorFilas[vectorFilas.size()-1] && vectorFilas[posicion]!=vectorFilas[0]){
			for(int i=0;i<posicion;i++) vectorTiempo[i] = relojFilas[i].getElapsedTime() + vectorTiempo[i]; /*Guardamos los tiempos de
																							de las filas que se encuentran por debajo
																						de la fila que ha ser cambiada.*/
			if(!seEliminoFila){ //Verificamos si no se ha eliminado ninguna fila.
				for(int i=posicion;i<numFilas-1;i++){
					vectorTiempo[i] = relojFilas[i+1].getElapsedTime() + vectorTiempo[i+1]; /*Guardamos el tiempo de las filas que se
																				encuentran por encima de la fila que ha ser cambiada.*/
					vectorFilas[i] = vectorFilas[i+1]; //Modificamos el lugar que guarda el numero de la fila con objetos consolidados.
				}
			}else{ //Si se ha eliminado una fila:
				for(int i=posicion;i<numFilas-1;i++) {
					vectorFilas[i] = vectorFilas[i+1]+1; //Modificamos el lugar que guarda el numero de la fila con objetos consolidados.
					vectorTiempo[i] = Time(); /*Creamos un objeto tipo 'Time' en el lugar de la fila que queremos que se reinicie su
												tiempo.*/
				}
			}
		}
		else if(vectorFilas[posicion]==vectorFilas[0]){ //Verificamos si la fila que ha de ser cambiada es la primera.
			for(int i=0;i<numFilas-1;i++) {
				if(!seEliminoFila) { //Verificamos que no se haya eliminado una fila.
					vectorFilas[i] = vectorFilas[i+1]; //Modificamos el lugar que guarda el numero de la fila con objetos consolidados.
					vectorTiempo[i] = relojFilas[i+1].getElapsedTime() + vectorTiempo[i+1]; /*Guardamos el tiempo de las filas que se
																				encuentran por encima de la fila que ha ser cambiada.*/
				}
				else { //Si se ha eliminado una fila:
					vectorFilas[i] = vectorFilas[i+1]+1; //Modificamos el lugar que guarda el numero de la fila con objetos consolidados.
					for(int i=0;i<numFilas-1;i++) vectorTiempo[i] = Time(); /*Creamos un objeto tipo 'Time' en el lugar de la fila que
																			queremos que se reinicie su tiempo.*/
				}
			}
		}
		else{
			for(int i=0;i<numFilas-1;i++) vectorTiempo[i] = relojFilas[i].getElapsedTime() + vectorTiempo[i]; /*Guardamos el tiempo de 
																									las filas que se encuentran por 
																									debajo de la fila eliminada.*/
		}
		vectorFilas.resize(vectorFilas.size()-1); /*Reducimos la capacidad del vector 'vectorFilas', eliminando el lugar 
													de la fila cambiada.*/
		vectorTiempo.resize(vectorTiempo.size()-1); /*Reducimos la capacidad del vector 'vectorTiempo', eliminando e lugar 
													de la fila cambiada.*/
		relojFilas = new Clock[vectorFilas.size()]; //Cambiamos la cantidad de relojes activos.
		for(int i=0;i<numFilas-1;i++) relojFilas[i].restart(); //Reiniciamos todos los relojes.
		numFilas = vectorFilas.size(); //Actualizamos la cantidad de filasc on objetos consolidados.
	}else{ //Si la cantidad de filas con objetos consolidados es igual a uno '1'.
		vectorFilas.resize(0); //Vaciamos el vector 'vectorFilas'.
		vectorTiempo.resize(0); //Vaciamos el vector 'vectorTiempo'.
	}
	if(vectorFilas.size()==0){ //Si el tamaño del vector 'vectorFilas' es de cero '0':
		primeraVez = true; //Cambiamos el estado 'primeraVez' para que realice las acciones correspondientes.
		numFilas = 0; //Actualizamos la cantidad de filas con objetos consolidados.
	}
}
