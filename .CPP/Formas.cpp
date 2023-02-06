#include "Formas.h"

//El constructor carga, si encuentra, los sonidos especificados, y cambia el valor de ciertas variables de tipo 'bool'.
Formas::Formas() {
	Globales::CambiarEstado(13,false); //Colocamos como estado inicial 'false' para indicar que todavia no se movio ninguna forma.
	Globales::CambiarEstado(8,true); //Colocamos como estado inicial 'true' para indicar que los efectos de esta clase puedan reproducirse.
	debeGuardar = true; cambiarSonido = true;
	if(!musicaUno.openFromFile("Musica/CayoUno.wav")) {
		cerr<<"No se pudo encontar el archivo 'CayoUno.wav'"<<endl;
		cargoSonidoUno = false;
	}
	else {
		musicaUno.setVolume(25);
		cargoSonidoUno = true;
	}
	if(!musicaDos.openFromFile("Musica/CayoDos.wav")) {
		cerr<<"No se pudo encontrar el archivo 'CayoDos.wav'"<<endl;
		cargoSonidoDos = false;
	}
	else {
		musicaDos.setVolume(20);
		cargoSonidoDos = true;
	}
}

/*
El metodo 'Actualizar' de la clase 'Formas' verifica que la forma no se encuentra en la ultima fila (Mediante el metodo
'VerificarMovimientos', explicado mas debajo). Si se encuentra en la ultima fila, cambia el estado de la variable global para,
dentro de la clase 'Partida', crear otra forma. Si no se encuentra en la ultima fila, llama al metodo 'Mover' para, si se puede,
mover verticalmente hacia abajo un lugar cada RectangleShape de la matriz.
*/
void Formas::Actualizar(vector<vector<int>> &matrizCeldas,int valorForma,int &score){
	if(VerificarMovimientos(matrizCeldas,valorForma)){
		Globales::CambiarEstado(2,Mover(matrizCeldas,valorForma,score));
	} else Globales::CambiarEstado(2,true);
}

/*
Este metodo llama a la funcion 'UltimaColuma' para verificar que la forma no se encuentre en la ultima fila. Si es asi, llama al
metodo 'ConsolidarForma' para guardar la posicion de esa forma, devolviendo falso para crear, luego, otra forma. Si no es asi,
devuelve verdadero para seguir con el metodo 'Mover'.
*/
bool Formas::VerificarMovimientos(vector<vector<int>> &matrizCeldas,int valorForma){
	if(UltimaColuma(matrizCeldas)){
		ConsolidarForma(matrizCeldas,valorForma);
		return false;
	}
	return true;
}

/*
Este metodo corrobora que la forma no se encuentre en la ultima fila, devolviendo true en caso afirmativo, y false en caso
negativo.
*/
bool Formas::UltimaColuma(const vector<vector<int>> &matrizCeldas){
	for(int i=0;i<columnas;i++){
		if(matrizCeldas[i][19] == 1) return true;
	}
	return false;
}

/*
Este metodo corrobora que la posicion de los RectangleShape pasados como parametros no se encuentre en la ultima fila, 
devolviendo true en caso afirmativo, y false en caso negativo.
*/
bool Formas::UltimaColuma(const vector<Vector2i> posicionesUnos){
	for(int i=0;i<posicionesUnos.size();i++){
		if(posicionesUnos[i].y == 19) return true;
	}
	return false;
}

/*
Este metodo obtiene la posicion de los unos '1' en la matriz para, luego, llamar al metodo 'VerificarColision', con el objetivo
de chequear si la forma ha 'chocado' con otra. En caso afirmativo, dentro del mismo metodo 'VerificarColision' se llama a la
funcion 'ConsolidarForma' para guardar la posicion de la ultima forma creada. Caso negativo, se mueve cada RectangleShape un lugar
hacia abajo en la matriz, y se suma 1 punto en el score del usuario.
*/
bool Formas::Mover(vector<vector<int>> &matrizCeldas,int valorForma,int &score){
	m_posicionUnos.resize(0); ObtenerPosiciones(matrizCeldas); //Se obtiene la posicion de los unos '1' de la matriz.
	if(VerificarColision(matrizCeldas,valorForma)){ 
		for(size_t i=m_posicionUnos.size();i>0;i--){
			matrizCeldas[m_posicionUnos[i-1].x][m_posicionUnos[i-1].y+1] = 1; //Modificamos la numericon de la matriz.
			matrizCeldas[m_posicionUnos[i-1].x][m_posicionUnos[i-1].y] = 0;
		}
		score++; //Sumamos al puntaje del usuario, una vez se haya corroborado que se puede mover la forma un lugar hacia abajo.
		return false;
	}
	return true;
}

/*
Este metodo llama a la funcion 'ObtenerColision' para verificar si la forma ha 'chocado' o no con otra. En caso afirmativo se
llama a la funcion 'ConsolidarForma' para guardar la posicion de la ultima forma creada. En caso negativo simplemente se devuelve
false.
*/
bool Formas::VerificarColision(vector<vector<int>> &matrizCeldas,int valorForma){
	if(ObtenerColision(matrizCeldas)){
		ConsolidarForma(matrizCeldas,valorForma);
		return false;
	}
	return true;
}

//Este metodo verifica si la forma ha 'chocado' o no con otra, devolviendo true en caso afirmativo, y false en caso negativo.
bool Formas::ObtenerColision(const vector<vector<int>> &matrizCeldas){
	for(size_t i=m_posicionUnos.size();i>0;i--){
		if(matrizCeldas[m_posicionUnos[i-1].x][m_posicionUnos[i-1].y+1]>1) return true; /*Si en la posicion siguiente hacia abajo del 
																		RectangleShape de ese momento de la matriz, se encuentra que 
																tiene un numero mayor al 1, se devuelve true, indicando que la forma ha
																'chocado'. Si el modo 'Fila piedra' esta activado, tambien corrobora 
																que no haya un '-1' o '-2' en la matriz.*/
		else if(Globales::VerEstado(14)){ //Verificamos si el modo 'Fila piedra' esta activado.
			if(matrizCeldas[m_posicionUnos[i-1].x][m_posicionUnos[i-1].y+1]==-1
			|| matrizCeldas[m_posicionUnos[i-1].x][m_posicionUnos[i-1].y+1]==-2) return true;
		}
	}
	return false;
}

//Este metodo verifica si la forma ha 'chocado' o no con otra, devolviendo true en caso afirmativo, y false en caso negativo.
bool Formas::ObtenerColision(const vector<vector<int>> &matrizCeldas,const vector<Vector2i> posicionesUnos){
	for(size_t i=0;i<posicionesUnos.size();i++){
		if(matrizCeldas[posicionesUnos[i].x][posicionesUnos[i].y+1]>1) return true;
		else if(Globales::VerEstado(14)){
			if(matrizCeldas[posicionesUnos[i].x][posicionesUnos[i].y+1]==-1
			|| matrizCeldas[posicionesUnos[i].x][posicionesUnos[i].y+1]==-2) return true;
		}
	}
	return false;
}

/*
Este metodo guarda la posicion de la forma que se quiera consolidad mediante el cambio de numero dentro de la matriz, donde:
	0: No hay forma.
	1: Forma actual que se permite mover.
	2: Cuadrado.
	3: EleIzq.
	4: EleDer.
	5: Linea.
	6: EseIzq.
	7: EseDer.
	8: Te.
A su vez, tambien, si los efectos estan activos, y no se ha hecho un Hard-Drop, reproduce el efecto de sonido cargado.
*/
void Formas::ConsolidarForma(vector<vector<int>> &matrizCeldas,int valorForma){
	for(size_t i=m_posicionUnos.size();i>0;i--){
		switch(valorForma){ //Aca es donde se cambia el valor numeroico de la matriz dependiendo la forma creada.
		case 1: {matrizCeldas[m_posicionUnos[i-1].x][m_posicionUnos[i-1].y] = 2; break;}
		case 2: {matrizCeldas[m_posicionUnos[i-1].x][m_posicionUnos[i-1].y] = 3; break;}
		case 3: {matrizCeldas[m_posicionUnos[i-1].x][m_posicionUnos[i-1].y] = 4; break;}
		case 4: {matrizCeldas[m_posicionUnos[i-1].x][m_posicionUnos[i-1].y] = 5; break;}
		case 5: {matrizCeldas[m_posicionUnos[i-1].x][m_posicionUnos[i-1].y] = 6; break;}
		case 6: {matrizCeldas[m_posicionUnos[i-1].x][m_posicionUnos[i-1].y] = 7; break;}
		default: {matrizCeldas[m_posicionUnos[i-1].x][m_posicionUnos[i-1].y] = 8; break;}}
	}
	if(Globales::VerEstado(8)){ //Aca es donde se reproduce, si es posible, el efecto.
		if(Globales::VerEstado(10)){ //Verifica si los efectos estan activos.
			if(cambiarSonido) { //Verifica que archivo musical debe reproducir.
				//Verifica si el archivo ha sido cargado con normalidad.
				if(cargoSonidoUno) {musicaUno.play(); cambiarSonido = false;}
				else cambiarSonido = false;
			}
			else {
				if(cargoSonidoUno) {musicaDos.play(); cambiarSonido = true;}
				else cambiarSonido = true;
			}
		}
	}else Globales::CambiarEstado(8,true); /*Cambia el estado a 'true' para el efecto de sonido pueda reproducirse (este cambio
										no reproduce los efectos si el usuario apago estos. Este cambio en la variable se realiza
										si el usuarcio hizo un HardDrop, de manera de darle otra vez la posibilidad de reproducirse.*/
}

/*
Este metodo es el encargado de mostrar la 'sombra' de la ultima forma creada, con el objetivo de brinda ayuda visual al usuario de
donde puede 'caer' el objeto con la rotacion dada.
*/
void Formas::MuestraPieza(vector<vector<int>> &matrizCeldas,bool caidaRapida){
	m_posicionUnos.resize(0); ObtenerPosiciones(matrizCeldas); //Se obtiene la posicion de los unos '1' de la matriz.
	if(caidaRapida==false){ //Condicional que verifica si el usuario a apretado la tecla para el HardDrop.
		for(size_t i=0;i<m_posicionUnos.size();i++){ /*Sino lo ha apretado, se verifica que la ultima forma creada pueda, efectivamente,
													moverse un lugar hacia abajo, mediante la llamada de los metodos 'UltimaColuma' 
													y 'ObtenerColision'.*/
			if(UltimaColuma(matrizCeldas) || ObtenerColision(matrizCeldas)) {puedePasar = false; debeGuardar = true;  break;}
			else puedePasar = true; 
		}
	}
	else {guardarUnos.resize(0); puedePasar = false; caidaRapida = false; debeGuardar = true;} /*Si ha apretado HardDrop, se reinician
																								las variables.*/
	if(puedePasar){ //Condicional que verifica que la forma efectivamente todavia puede moverse hacia abajo.
		if(debeGuardar) {guardarUnos.resize(0); guardarPosicion.resize(0); debeGuardar = false; debeRealizar = true;} /*Condicional
			que verifica si se debe guardar la ultima posicion de ultima forma creada. Si es asi, reinicia un vector de vector2i que 
			pretende guardar la posicion de los unos '1' de la matriz, para luego bajar esa posicion hasta que 'choque' con otra forma 
			o llegue al final de la matriz. Se reinicia otro vector de vector2i que guardara, luego, que posicion efectiva de la sombra
			debe visualizarse.*/
		if(debeRealizar){
			//Se guarda la posicion de los unos '1' ya obtenidos anteriormente en otro vector de vector2i.
			for(size_t i=0;i<m_posicionUnos.size();i++) guardarUnos.push_back({m_posicionUnos[i].x,m_posicionUnos[i].y}); 
			while(UltimaColuma(guardarUnos)==false && ObtenerColision(matrizCeldas,guardarUnos)==false) { /*Mientras la posicion de
																									los unos '1' no 'choque' con otra 
																								forma o no llegue a la ultima fila de 
																							la matriz se repetira este bucle.*/
				for(size_t i=0;i<guardarUnos.size();i++) guardarUnos[i].y = guardarUnos[i].y + 1;  /*Se aumenta la posicion de
																								cada RectangleShape en uno, 
																							verticalmente (en si, bajando la forma).*/
			}
			for(size_t i=0;i<guardarUnos.size();i++){
				for(size_t j=0;j<m_posicionUnos.size();j++){
					/*Se verifica que la posicion de cada uno '1' guardado en el vector 'm_posicionUnos' no sea igual a los unos '1'
					que fueron corridos hacia abajo guardados en el vector 'guardarUnos' con el objetivo de que estos no se solapen.*/
					if(m_posicionUnos[j].x==guardarUnos[i].x && m_posicionUnos[j].y==guardarUnos[i].y) {puedePasar = false; break;}
					
					else puedePasar = true;
				}
				if(puedePasar) guardarPosicion.push_back({guardarUnos[i].x,guardarUnos[i].y}); /*Se guarda la posicion efectiva del
																							RectangleShape que tiene que ser mostrado.*/
			}
			/*Por ultimo, se modifica la matriz, colocandole un '-9' donde se haya decidido efectivamente colocar la sombra. El '-9', 
			luego, indicara que en esa posicion de la matriz ha de dibujarse la sombra.*/
			for(size_t i=0;i<guardarPosicion.size();i++) matrizCeldas[guardarPosicion[i].x][guardarPosicion[i].y] = -9;
			debeRealizar = false;
		}
	}
}

/*
Este metodo es el encargado de limpiar la sombra una vez que el usuario ha movido la forma, con el objetivo de poder dibujarla
nuevamente en otra posicion de la matriz.
*/
void Formas::LimpiarSombra(vector<vector<int>> &matrizCeldas){
	m_posicionUnos.resize(0); ObtenerPosiciones(matrizCeldas); guardarNueves.resize(0); /*Obtenemos la posicion de los unos '1' y
																					reiniamos un vector de vector2i que pretende 
																				guardar la posicion de los '-9'.*/
	for(size_t i=0;i<guardarPosicion.size();i++){
		for(size_t j=0;j<m_posicionUnos.size();j++){
			/*Corroboramos que la posicion de los unos '1', guardados en el vector 'm_posicionUnos', sea distinta de la posicion de 
			los '-9', de manera de no borrar de manera equivocada algun RectangleShape de la forma. A su vez, verificamos que la
			posicion de los unos '1' que fueron corridos hacia abajo efectivamente, guardados en el vector 'guardarPosicion', no 
			coincida en la matriz con la alguna otra forma ya consolidada, de manera de no borrar parte de ella.*/
			if(m_posicionUnos[j].x==guardarPosicion[i].x && m_posicionUnos[j].y==guardarPosicion[i].y
			|| matrizCeldas[guardarPosicion[i].x][guardarPosicion[i].y]>1) {puedePasar = false; break;}
			else puedePasar = true;
		}
		if(puedePasar) guardarNueves.push_back({guardarPosicion[i].x,guardarPosicion[i].y}); //Guardamos la posicion de los '-9'.
	}
	for(size_t i=0;i<guardarNueves.size();i++) matrizCeldas[guardarNueves[i].x][guardarNueves[i].y] = 0; /*Eliminamos los '-9' que
																								pasaron, de manera de 'limpiar' la sombra.*/
	Globales::CambiarEstado(13,false); //Cambiamos el estado para indicar que la forma se dejo de mover.
	debeGuardar = true; //Cambiamos el estado para que se realice el proceso de guardado de la posicion de la sombra nuevamente.
}
