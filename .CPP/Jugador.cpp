#include "Jugador.h"

void ObtenerPosiciones(const vector<vector<int>> &matrizCeldas); //Declaracion de la funcion global 'ObtenerPosiciones'.
vector<Vector2i> m_posicionUnos; //Declaracion del vector de vector2i global 'm_posicionUnos'.

/*
En su constructor se cargan, si se encuentran, los efectos de sonido a utilizarse. Por ultimo se modifican las variables de tipo
'int' y 'bool'.
*/
Jugador::Jugador() {
	if(!musicaUno.openFromFile("Musica/NoMove.wav")) {
		cerr<<"No se pudo encontrar el archivo 'NoMove.wav'"<<endl;
		cargoSonidoUno = false;
	}
	else {
		musicaUno.setVolume(30);
		cargoSonidoUno = true;
	}
	if(!musicaDos.openFromFile("Musica/Move.wav")) {
		cerr<<"No se pudo encontrar el archivo 'Move.wav'"<<endl;
		cargoSonidoDos = false;
	}
	else {
		musicaDos.setVolume(45);
		cargoSonidoDos = true;
	}
	numRotaciones = 0; cambioDeLado = false; primeraVezPausa = false;
}

/*
Lo que realiza el metodo 'Actualizar' de la clase 'Jugador' es, principalmente, chequear si el usuario apreto alguna tecla relacionada
al movimiento de la forma. Si es asi, verifica cual y, dependiendo cual sea, llama a una u otra funcion. Los metodos 'MoverIzq', 
'MoverDer', 'MoverAbajo', 'RotarForma', y 'BajarForma' se explican mas abajo.
*/
bool Jugador::Actualizar(vector<vector<int>> &matrizCeldas,int valorForma,int &score){
	puedeMoverse = 0;
	if(Globales::VerEstado(2)) numRotaciones = 0;
	m_posicionUnos.resize(0); ObtenerPosiciones(matrizCeldas); //Se obtiene la posicion de los unos '1' de la matriz.
	if(Keyboard::isKeyPressed(Globales::VerTecla(0))){ //Verifica si se apreto la tecla relacionada con mover la forma hacia la izquierda.
		if(reloj.getElapsedTime().asMilliseconds()>100) {MoverIzq(matrizCeldas); Globales::CambiarEstado(13,true); return false;} 
		//Si entra, se llama al metodo indicado y se cambia el estado de la variable para indicar si se movio (true) o no (false).
	}		
	if(Keyboard::isKeyPressed(Globales::VerTecla(1))){ //Verifica si se apreto la tecla relacionada con mover la forma hacia la derecha.
		if(reloj.getElapsedTime().asMilliseconds()>100) {MoverDer(matrizCeldas); Globales::CambiarEstado(13,true); return false;}
	}
	if(Keyboard::isKeyPressed(Globales::VerTecla(2))){ //Verifica si se apreto la tecla relacionada con mover la forma hacia la abajo.
		if(reloj.getElapsedTime().asMilliseconds()>150) {MoverAbajo(matrizCeldas,score); Globales::CambiarEstado(13,false); return false;}
	}
	if(Keyboard::isKeyPressed(Globales::VerTecla(3))){ //Verifica si se apreto la tecla relacionada con rotar la forma.
		debeRotar = 0; //Se reinicia la variable.
		if(reloj.getElapsedTime().asMilliseconds()>150){
			for(size_t i=0;i<m_posicionUnos.size();i++) {
				if(m_posicionUnos[i].y!=19) debeRotar++; /*Verificamos que ningun RectangleShape se encuentra en la ultima fila. De
														ser asi, no se permite hacer la rotacion.*/
				else debeRotar = 0;
			}
		if(debeRotar==4) {RotarForma(matrizCeldas,valorForma); Globales::CambiarEstado(13,true); return false;}
		}
	}
	if(Keyboard::isKeyPressed(Globales::VerTecla(4))){ //Verifica si se apreto la tecla relacionada con realizar un HardDrop.
		if(reloj.getElapsedTime().asMilliseconds()>150) {BajarForma(matrizCeldas,valorForma,score); Globales::CambiarEstado(13,false); return true;}
	}
	return false;
}

//Este metodo se encarga de 'mover' la forma un lugar hacia la izquierda.
void Jugador::MoverIzq(vector<vector<int>> &matrizCeldas){
	for(size_t i=0;i<m_posicionUnos.size();i++){
		if(CorroborarMover(matrizCeldas,m_posicionUnos[i].x,m_posicionUnos[i].y,1)) puedeMoverse++; /*Se llama a la funcion
																								'CorroborarMover' para chequear si, 
																							efectivamente, se mueve mover la forma. 
																						Caso negativo, no se permite realizar la accion.*/
		else {
			puedeMoverse = 0;  //Se reinicia la variable.
			if(Globales::VerEstado(10)){ //Verifica si los efectos estan activos.
				//Verifica si el sonido pudo cargarse con normalidad.
				if(cargoSonidoUno) musicaUno.play(); 
			}
			break;
		}
	}
	if(puedeMoverse==4){
		for(size_t i=0;i<m_posicionUnos.size();i++){
			/*Se toma a cada RectangleShape y, en su posicion, se le resta una columa, colocando en esa nueva posicion un uno '1' y, 
			en la antigua, un cero '0', borrando de esta manera el antiguo RectangleShape.*/
			matrizCeldas[m_posicionUnos[i].x-1][m_posicionUnos[i].y] = 1;
			matrizCeldas[m_posicionUnos[i].x][m_posicionUnos[i].y] = 0;
		}
	}
	reloj.restart(); //Reiniciamos el reloj.
}

//Este metodo se encarga de 'mover' la forma un lugar hacia la derecha.
void Jugador::MoverDer(vector<vector<int>> &matrizCeldas){
	for(size_t i=m_posicionUnos.size();i>0;i--){
		if(CorroborarMover(matrizCeldas,m_posicionUnos[i-1].x,m_posicionUnos[i-1].y,2)) puedeMoverse++; /*Se llama a la funcion
																								'CorroborarMover' para chequear si, 
																							efectivamente, se mueve mover la forma. 
																						Caso negativo, no se permite realizar la accion.*/
		else {
			puedeMoverse = 0;	
			if(Globales::VerEstado(10)) {
				if(cargoSonidoUno) musicaUno.play();
			}
			break;
		}
	}	
	if(puedeMoverse==4){
		for(size_t i=m_posicionUnos.size();i>0;i--){
			/*Se toma a cada RectangleShape y, en su posicion, se le suma una columa, colocando en esa nueva posicion un uno '1' y, 
			en la antigua, un cero '0', borrando de esta manera el antiguo RectangleShape.*/
			matrizCeldas[m_posicionUnos[i-1].x+1][m_posicionUnos[i-1].y] = 1;
			matrizCeldas[m_posicionUnos[i-1].x][m_posicionUnos[i-1].y] = 0;
		}
	}
	reloj.restart();
}
	
void Jugador::MoverAbajo(vector<vector<int>> &matrizCeldas,int &score){
	for(size_t i=m_posicionUnos.size();i>0;i--){
		if(CorroborarMover(matrizCeldas,m_posicionUnos[i-1].x,m_posicionUnos[i-1].y,3)) puedeMoverse++; /*Se llama a la funcion
																									'CorroborarMover' para chequear si, 
																								efectivamente, se mueve mover la forma. 
																						Caso negativo, no se permite realizar la accion.*/
		else {puedeMoverse = 0; break;}
	}
	if(puedeMoverse==4){
		for(size_t i=m_posicionUnos.size();i>0;i--){
			/*Se toma a cada RectangleShape y, en su posicion, se le suma una fila, colocando en esa nueva posicion un uno '1' y, 
			en la antigua, un cero '0', borrando de esta manera el antiguo RectangleShape.*/
			matrizCeldas[m_posicionUnos[i-1].x][m_posicionUnos[i-1].y+1] = 1;
			matrizCeldas[m_posicionUnos[i-1].x][m_posicionUnos[i-1].y] = 0;
		}
		if(Globales::VerEstado(10)) {
			if(cargoSonidoDos) musicaDos.play();
		}
		score++; //Se suma 1 punto al score del usuario.
	}
	reloj.restart();
}

//Este metodo se encarga de, si se permite, rotar la forma.
void Jugador::RotarForma(vector<vector<int>> &matrizCeldas,int valorForma){
	if(CorroborarRotar(matrizCeldas,valorForma)){ /*Se llama a la funcion 'CorroborarRotar' para verificar si, efectivamente, se
												puede rotar la pieza. Caso negativo, no se permite la accion.*/
		switch(valorForma){ //Dependiendo que objeto sea, cambia la forma en que se realiza la accion.
		/*
			En cada 'case' se tiene, a su vez, otro switch que verifica el numero de la rotacion, entiendo a este como que tipo de
			rotacion que ha de hacerse. Ciertas formas contienen mas rotaciones que otras y, el cuadrado, no tiene rotacion. Segun 
			la forma y el numero de rotacion, la matriz cambiara su composicion de numeros, de manera que cuando se dibuje nuevamente,
			se dara la ilusion que la forma ha rotado.
		*/
		case 2:{ //EleIzq
			switch(numRotaciones){
			case 0:{
				if(m_posicionUnos[2].x==m_posicionUnos[3].x){
					matrizCeldas[m_posicionUnos[0].x+1][m_posicionUnos[0].y] = 1;
					matrizCeldas[m_posicionUnos[0].x][m_posicionUnos[0].y] = 0;
					matrizCeldas[m_posicionUnos[1].x][m_posicionUnos[1].y+1] = 1;
					matrizCeldas[m_posicionUnos[1].x][m_posicionUnos[1].y] = 0;
					matrizCeldas[m_posicionUnos[3].x][m_posicionUnos[3].y+1] = 1;
					matrizCeldas[m_posicionUnos[3].x+1][m_posicionUnos[3].y] = 0;
				}else{
					matrizCeldas[m_posicionUnos[0].x-1][m_posicionUnos[0].y+1] = 1;
					matrizCeldas[m_posicionUnos[2].x][m_posicionUnos[2].y] = 0;
					matrizCeldas[m_posicionUnos[3].x-2][m_posicionUnos[3].y-2] = 1;
					matrizCeldas[m_posicionUnos[3].x][m_posicionUnos[3].y] = 0;
				}
				numRotaciones++;
				break;}
			case 1:{
				matrizCeldas[m_posicionUnos[0].x+1][m_posicionUnos[0].y+1] = 1;
				matrizCeldas[m_posicionUnos[1].x-2][m_posicionUnos[1].y+2] = 1;
				matrizCeldas[m_posicionUnos[1].x][m_posicionUnos[1].y] = 0;
				matrizCeldas[m_posicionUnos[2].x][m_posicionUnos[2].y] = 0;
				numRotaciones++;
				break;}
			case 2:{
				if(m_posicionUnos[0].x==m_posicionUnos[1].x) {
					matrizCeldas[m_posicionUnos[0].x][m_posicionUnos[0].y+2] = 1;
					matrizCeldas[m_posicionUnos[0].x-1][m_posicionUnos[0].y] = 0;
					matrizCeldas[m_posicionUnos[3].x-1][m_posicionUnos[3].y] = 1;
					matrizCeldas[m_posicionUnos[3].x][m_posicionUnos[3].y] = 0;
				}
				else {
					matrizCeldas[m_posicionUnos[0].x+2][m_posicionUnos[0].y+2] = 1;
					matrizCeldas[m_posicionUnos[0].x][m_posicionUnos[0].y] = 0;
					matrizCeldas[m_posicionUnos[1].x][m_posicionUnos[1].y] = 0;
					matrizCeldas[m_posicionUnos[3].x+1][m_posicionUnos[3].y-1] = 1;
				}
				numRotaciones++;
				break;}
			default:{
				matrizCeldas[m_posicionUnos[1].x][m_posicionUnos[1].y] = 0;
				matrizCeldas[m_posicionUnos[2].x+2][m_posicionUnos[2].y-2] = 1;
				matrizCeldas[m_posicionUnos[2].x][m_posicionUnos[2].y] = 0;
				matrizCeldas[m_posicionUnos[3].x-1][m_posicionUnos[3].y-1] = 1;
				numRotaciones = 0;
				break;}
			}
			break;
		}
		case 3:{ //EleDer
			switch(numRotaciones){
			case 0:{
				matrizCeldas[m_posicionUnos[0].x+2][m_posicionUnos[0].y+2] = 1;
				matrizCeldas[m_posicionUnos[0].x][m_posicionUnos[0].y] = 0;
				matrizCeldas[m_posicionUnos[1].x+1][m_posicionUnos[1].y+1] = 1;
				matrizCeldas[m_posicionUnos[2].x][m_posicionUnos[2].y] = 0;
				numRotaciones++;
				break;}
			case 1:{
				matrizCeldas[m_posicionUnos[0].x][m_posicionUnos[0].y] = 0;
				matrizCeldas[m_posicionUnos[2].x+1][m_posicionUnos[2].y-1] = 1;
				matrizCeldas[m_posicionUnos[3].x+2][m_posicionUnos[3].y-2] = 1;
				matrizCeldas[m_posicionUnos[3].x][m_posicionUnos[3].y] = 0;
				numRotaciones++;
			   break;}
			case 2:{
				matrizCeldas[m_posicionUnos[2].x][m_posicionUnos[2].y] = 0;
				matrizCeldas[m_posicionUnos[2].x-1][m_posicionUnos[2].y-1] = 1;
				matrizCeldas[m_posicionUnos[3].x][m_posicionUnos[3].y] = 0;
				matrizCeldas[m_posicionUnos[3].x-1][m_posicionUnos[3].y+1] = 1;
				numRotaciones++;
				break;}
			default:{
				matrizCeldas[m_posicionUnos[0].x-2][m_posicionUnos[0].y+2] = 1;
				matrizCeldas[m_posicionUnos[0].x][m_posicionUnos[0].y] = 0;
				matrizCeldas[m_posicionUnos[1].x-1][m_posicionUnos[1].y+1] = 1;
				matrizCeldas[m_posicionUnos[3].x][m_posicionUnos[3].y] = 0;
				numRotaciones = 0;
				break;}
			}
			break;
		}
		case 4:{ //Linea
			switch(numRotaciones){
			case 0:{
				if(cambioDeLado){
					matrizCeldas[m_posicionUnos[0].x-2][m_posicionUnos[0].y+1] = 1;
					matrizCeldas[m_posicionUnos[0].x][m_posicionUnos[0].y] = 0;
					matrizCeldas[m_posicionUnos[2].x-1][m_posicionUnos[2].y-1] = 1;
					matrizCeldas[m_posicionUnos[2].x][m_posicionUnos[2].y] = 0;
					matrizCeldas[m_posicionUnos[3].x+1][m_posicionUnos[3].y-2] = 1;
					matrizCeldas[m_posicionUnos[3].x][m_posicionUnos[3].y] = 0;
					cambioDeLado = false;
				}else{
					matrizCeldas[m_posicionUnos[0].x-1][m_posicionUnos[0].y+1] = 1;
					matrizCeldas[m_posicionUnos[0].x][m_posicionUnos[0].y] = 0;
					matrizCeldas[m_posicionUnos[2].x+1][m_posicionUnos[2].y-1] = 1;
					matrizCeldas[m_posicionUnos[2].x][m_posicionUnos[2].y] = 0;
					matrizCeldas[m_posicionUnos[3].x+2][m_posicionUnos[3].y-2] = 1;
					matrizCeldas[m_posicionUnos[3].x][m_posicionUnos[3].y] = 0;
				}
				numRotaciones++;
				break;}
			default:{
				if(m_posicionUnos[2].x==m_posicionUnos[3].x){
					matrizCeldas[m_posicionUnos[0].x+2][m_posicionUnos[0].y-1] = 1;
					matrizCeldas[m_posicionUnos[0].x][m_posicionUnos[0].y] = 0;
					matrizCeldas[m_posicionUnos[1].x+1][m_posicionUnos[1].y+1] = 1;
					matrizCeldas[m_posicionUnos[1].x][m_posicionUnos[1].y] = 0;
					matrizCeldas[m_posicionUnos[3].x][m_posicionUnos[3].y+2] = 1;
					matrizCeldas[m_posicionUnos[3].x+1][m_posicionUnos[3].y] = 0;
				}else{
					matrizCeldas[m_posicionUnos[0].x+1][m_posicionUnos[0].y-1] = 1;
					matrizCeldas[m_posicionUnos[0].x][m_posicionUnos[0].y] = 0;
					matrizCeldas[m_posicionUnos[2].x-1][m_posicionUnos[2].y+1] = 1;
					matrizCeldas[m_posicionUnos[2].x][m_posicionUnos[2].y] = 0;
					matrizCeldas[m_posicionUnos[3].x-2][m_posicionUnos[3].y+2] = 1;
					matrizCeldas[m_posicionUnos[3].x][m_posicionUnos[3].y] = 0;
				}
				numRotaciones = 0;
				break;}
			}
			break;
		}
		case 5:{ //EseIzq
			switch(numRotaciones){
			case 0:{
				matrizCeldas[m_posicionUnos[0].x][m_posicionUnos[0].y] = 0;
				matrizCeldas[m_posicionUnos[2].x+1][m_posicionUnos[2].y-1] = 1;
				matrizCeldas[m_posicionUnos[3].x][m_posicionUnos[3].y-2] = 1;
				matrizCeldas[m_posicionUnos[3].x][m_posicionUnos[3].y] = 0;
				numRotaciones++;
				break;}
			default:{
				matrizCeldas[m_posicionUnos[1].x-1][m_posicionUnos[1].y-1] = 1;
				matrizCeldas[m_posicionUnos[2].x][m_posicionUnos[2].y+2] = 1;
				matrizCeldas[m_posicionUnos[2].x][m_posicionUnos[2].y] = 0;
				matrizCeldas[m_posicionUnos[3].x][m_posicionUnos[3].y] = 0;
				numRotaciones = 0;
				break;}
			}	
			break;
		}
		case 6:{ //EseDer
			switch(numRotaciones){
			case 0:{
				matrizCeldas[m_posicionUnos[0].x+2][m_posicionUnos[0].y] = 1;
				matrizCeldas[m_posicionUnos[0].x][m_posicionUnos[0].y] = 0;
				matrizCeldas[m_posicionUnos[2].x][m_posicionUnos[2].y] = 0;
				matrizCeldas[m_posicionUnos[3].x-1][m_posicionUnos[3].y-1] = 1;
				numRotaciones++;
			   break;}
			default:{
				matrizCeldas[m_posicionUnos[0].x][m_posicionUnos[0].y] = 0;
				matrizCeldas[m_posicionUnos[2].x-1][m_posicionUnos[2].y+1] = 1;
				matrizCeldas[m_posicionUnos[3].x-2][m_posicionUnos[3].y] = 1;
				matrizCeldas[m_posicionUnos[3].x][m_posicionUnos[3].y] = 0;
				numRotaciones = 0;
				break;}
			}
			break;
		}
		case 7:{ //Te
			switch(numRotaciones){
			case 0:{
				matrizCeldas[m_posicionUnos[0].x][m_posicionUnos[0].y] = 0;
				matrizCeldas[m_posicionUnos[3].x-1][m_posicionUnos[3].y-1] = 1;
				numRotaciones++;
				break;}
			case 1:{
				matrizCeldas[m_posicionUnos[0].x-1][m_posicionUnos[0].y+1] = 1;
				matrizCeldas[m_posicionUnos[2].x][m_posicionUnos[2].y] = 0;
				numRotaciones++;
				break;}
			case 2:{
				matrizCeldas[m_posicionUnos[0].x+1][m_posicionUnos[0].y+1] = 1;
				matrizCeldas[m_posicionUnos[3].x][m_posicionUnos[3].y] = 0;
				numRotaciones++;
				break;}
			default:{
				matrizCeldas[m_posicionUnos[1].x+1][m_posicionUnos[1].y+1] = 1;
				matrizCeldas[m_posicionUnos[1].x][m_posicionUnos[1].y] = 0;
				numRotaciones = 0;
				break;}
			}
			break;
		}
		}	
	}
	reloj.restart();
}

/*
Este metodo, junto a 'CorroborarObjeto', son los encargados de verificar si la forma efectivamente puede realizar la rotacion.
Dependiendo la forma y el numero de rotacion, los condicionales cambian, verificando que en donde se vaya a hacer la variacion
numerica no exista ya un objeto consolidado, o si el objeto a rotar no se salga de la matriz (al realizar la rotacion). En caso
afirmativo, se permite la rotacion. Caso negativo, se la deniega.
En algunos casos particulares se cambio, momentaneamente, el 'centro' de donde se realizaba la rotacion, ya que en determinadas
ocasiones, como por ejemplo la forma 'Linea', no se te permitia modificar la pieza, aun cuando deberia ser posible. En la 'Linea', 
como ejemplo, si se tenia otro objeto hacia la derecha a 2 lugares de distancia, y se tenia mas de 2 lugares vacios a la izquierda,
ls rotacion deberia ser posible. No obstante, no se rotaba la pieza, al tener solo un lugar posible hacia la derecha. En casos como
estos, el 'centro' de rotacion se modifico. En la 'Linea', generalmente, su 'centro' de rotacion era desde el segundo RectangleShape
mas cerca hacia la izquierda o hacia arriba. No obstante, en los casos en lo que debio ser cambiado, el 'centro' se movio hacia el
segundo RectangleShape mas cerca hacia la derecha o hacia abajo.
*/
bool Jugador::CorroborarRotar(vector<vector<int>> &matrizCeldas,int valorForma){
	switch(valorForma){
	case 2:{ //EleIzq.
		switch(numRotaciones){
		case 0:{
			if(m_posicionUnos[0].x==0 || m_posicionUnos[0].y==0 || m_posicionUnos[3].x==9){ /*Si la pieza al rotar se saldria de
																						la matriz, se realizan las siguientes acciones.*/
				if(m_posicionUnos[0].x==0){
					if(matrizCeldas[m_posicionUnos[0].x+1][m_posicionUnos[0].y]>1) return false;
					if(matrizCeldas[m_posicionUnos[3].x+1][m_posicionUnos[3].y]>1) return false;
					for(size_t i=0;i<m_posicionUnos.size();i++) m_posicionUnos[i].x = m_posicionUnos[i].x + 1;
					if(matrizCeldas[m_posicionUnos[0].x-1][m_posicionUnos[0].y+1]>1) return false;
					if(matrizCeldas[m_posicionUnos[3].x-2][m_posicionUnos[3].y-2]>1) return false;
					for(size_t i=0;i<m_posicionUnos.size();i++){
						if(i<2) matrizCeldas[m_posicionUnos[i].x-1][m_posicionUnos[i].y] = 0;
						if(i<1 || i>2) matrizCeldas[m_posicionUnos[i].x][m_posicionUnos[i].y] = 1;
					}
				}
				else if(m_posicionUnos[3].x==9){
					m_posicionUnos[3].x = m_posicionUnos[3].x - 1;
					if(matrizCeldas[m_posicionUnos[0].x+1][m_posicionUnos[0].y]>1) return false;
					if(matrizCeldas[m_posicionUnos[1].x][m_posicionUnos[1].y+1]>1) return false;
					if(matrizCeldas[m_posicionUnos[3].x][m_posicionUnos[3].y+1]>1) return false;
					return true; break;
				}
				if(m_posicionUnos[0].y==0){
					if(matrizCeldas[m_posicionUnos[1].x][m_posicionUnos[1].y+1]>1) return false;
					if(matrizCeldas[m_posicionUnos[2].x][m_posicionUnos[2].y+1]>1) return false;
					if(matrizCeldas[m_posicionUnos[3].x][m_posicionUnos[3].y+1]>1) return false;
					for(size_t i=0;i<m_posicionUnos.size();i++) m_posicionUnos[i].y = m_posicionUnos[i].y + 1;
					if(matrizCeldas[m_posicionUnos[0].x-1][m_posicionUnos[0].y+1]>1) return false;
					if(matrizCeldas[m_posicionUnos[3].x-2][m_posicionUnos[3].y-2]>1) return false;
					for(size_t i=0;i<m_posicionUnos.size();i++){
						if(i!=1) matrizCeldas[m_posicionUnos[i].x][m_posicionUnos[i].y-1] = 0;
						if(i!=0) matrizCeldas[m_posicionUnos[i].x][m_posicionUnos[i].y] = 1;
					}
				}
				return true; break;
			}
			//Sino, se corrobora que no modifique/choque ningun otro objeto consolidado.
			else if(CorroborarObjeto(matrizCeldas,valorForma)) {return true; break;}
			else return false; break;
		}	
		case 1:{
			if(m_posicionUnos[0].x==0){
				if(matrizCeldas[m_posicionUnos[1].x+1][m_posicionUnos[1].y]>1) return false;
				if(matrizCeldas[m_posicionUnos[2].x+1][m_posicionUnos[2].y]>1) return false;
				if(matrizCeldas[m_posicionUnos[3].x+1][m_posicionUnos[3].y]>1) return false;
				for(size_t i=0;i<m_posicionUnos.size();i++) m_posicionUnos[i].x = m_posicionUnos[i].x + 1;
				if(matrizCeldas[m_posicionUnos[0].x+1][m_posicionUnos[0].y+1]>1) return false;
				if(matrizCeldas[m_posicionUnos[1].x-2][m_posicionUnos[1].y+2]>1) return false;
				for(size_t i=0;i<m_posicionUnos.size();i++){
					if(i!=3) matrizCeldas[m_posicionUnos[i].x-1][m_posicionUnos[i].y] = 0;
					if(i!=0) matrizCeldas[m_posicionUnos[i].x][m_posicionUnos[i].y] = 1;
				}
				return true; break;
			}
			else if(CorroborarObjeto(matrizCeldas,valorForma)) {return true; break;}
			else return false; break;
		}
		case 2:{
			if(m_posicionUnos[2].x==9){
				if(matrizCeldas[m_posicionUnos[0].x-1][m_posicionUnos[0].y]>1) return false;
				if(matrizCeldas[m_posicionUnos[3].x-1][m_posicionUnos[3].y]>1) return false;
				for(size_t i=0;i<m_posicionUnos.size();i++) m_posicionUnos[i].x = m_posicionUnos[i].x - 1;
				if(matrizCeldas[m_posicionUnos[0].x+2][m_posicionUnos[0].y+2]>1) return false;
				if(matrizCeldas[m_posicionUnos[3].x+1][m_posicionUnos[3].y-1]>1) return false;
				for(size_t i=0;i<m_posicionUnos.size();i++){
					if(i>1) matrizCeldas[m_posicionUnos[i].x+1][m_posicionUnos[i].y] = 0;
					if(i<1 || i>2) matrizCeldas[m_posicionUnos[i].x][m_posicionUnos[i].y] = 1;
				}
				return true; break;
			}
			if(m_posicionUnos[0].x==0) {
				m_posicionUnos[0].x = m_posicionUnos[0].x + 1;
				if(matrizCeldas[m_posicionUnos[0].x][m_posicionUnos[0].y+2]>1) return false;
				if(matrizCeldas[m_posicionUnos[3].x-1][m_posicionUnos[3].y]>1) return false;
				return true; break;
			}
			if(CorroborarObjeto(matrizCeldas,valorForma)) {return true; break;}
			else return false; break;
		}
		default:{
			if(m_posicionUnos[3].x==9 || m_posicionUnos[0].y==0){
				if(m_posicionUnos[3].x==9){
					if(matrizCeldas[m_posicionUnos[0].x-1][m_posicionUnos[0].y]>1) return false;
					if(matrizCeldas[m_posicionUnos[1].x-1][m_posicionUnos[1].y]>1) return false;
					if(matrizCeldas[m_posicionUnos[2].x-1][m_posicionUnos[2].y]>1) return false;
					for(size_t i=0;i<m_posicionUnos.size();i++) m_posicionUnos[i].x = m_posicionUnos[i].x - 1;
					if(matrizCeldas[m_posicionUnos[2].x+2][m_posicionUnos[2].y-2]>1) return false;
					if(matrizCeldas[m_posicionUnos[3].x-1][m_posicionUnos[3].y-1]>1) return false;
					for(size_t i=0;i<m_posicionUnos.size();i++){
						if(i!=0) matrizCeldas[m_posicionUnos[i].x+1][m_posicionUnos[i].y] = 0;
						if(i!=3) matrizCeldas[m_posicionUnos[i].x][m_posicionUnos[i].y] = 1;
					}
					return true; break;
				}
				if(m_posicionUnos[0].y==0){
					if(matrizCeldas[m_posicionUnos[2].x][m_posicionUnos[2].y+1]>1) return false;
					if(matrizCeldas[m_posicionUnos[3].x][m_posicionUnos[3].y+1]>1) return false;
					for(size_t i=0;i<m_posicionUnos.size();i++) m_posicionUnos[i].y = m_posicionUnos[i].y + 1;
					if(matrizCeldas[m_posicionUnos[2].x+2][m_posicionUnos[2].y-2]>1) return false;
					if(matrizCeldas[m_posicionUnos[3].x-1][m_posicionUnos[3].y-1]>1) return false;
					for(size_t i=0;i<m_posicionUnos.size();i++){
						if(i<2) matrizCeldas[m_posicionUnos[i].x][m_posicionUnos[i].y-1] = 0;
						if(i>1) matrizCeldas[m_posicionUnos[i].x][m_posicionUnos[i].y] = 1;
					}
					return true; break;
				}
				return true; break;
			}
			else if(CorroborarObjeto(matrizCeldas,valorForma)) {return true; break;}
			else return false; break;
		}
		}
		break;
	}
	case 3:{ //EleDer.
		switch(numRotaciones){
		case 0:{
			if(CorroborarObjeto(matrizCeldas,valorForma)) {return true; break;}
			else return false; break;
		}
		case 1:{
			if(m_posicionUnos[1].x==8 || m_posicionUnos[1].x==9){
				if(m_posicionUnos[1].x==8){
					if(matrizCeldas[m_posicionUnos[0].x-1][m_posicionUnos[0].y]>1) return false;
					if(matrizCeldas[m_posicionUnos[2].x-1][m_posicionUnos[2].y]>1) return false;
					if(matrizCeldas[m_posicionUnos[3].x-1][m_posicionUnos[3].y]>1) return false;
					for(size_t i=0;i<m_posicionUnos.size();i++) m_posicionUnos[i].x = m_posicionUnos[i].x - 1;
					if(matrizCeldas[m_posicionUnos[2].x+1][m_posicionUnos[2].y-1]>1) return false; 
					if(matrizCeldas[m_posicionUnos[3].x+2][m_posicionUnos[3].y-2]>1) return false;
					for(size_t i=0;i<m_posicionUnos.size();i++){
						if(i!=0) matrizCeldas[m_posicionUnos[i].x+1][m_posicionUnos[i].y] = 0;
						if(i!=1) matrizCeldas[m_posicionUnos[i].x][m_posicionUnos[i].y] = 1;
					}
				}else{
					for(size_t i=0;i<m_posicionUnos.size();i++){
						if(matrizCeldas[m_posicionUnos[i].x-2][m_posicionUnos[i].y]>1) return false;
					}
					for(size_t i=0;i<m_posicionUnos.size();i++) m_posicionUnos[i].x = m_posicionUnos[i].x - 2;
					if(matrizCeldas[m_posicionUnos[2].x+1][m_posicionUnos[2].y-1]>1) return false; 
					if(matrizCeldas[m_posicionUnos[3].x+2][m_posicionUnos[3].y-2]>1) return false;
					for(size_t i=0;i<m_posicionUnos.size();i++){
						matrizCeldas[m_posicionUnos[i].x+2][m_posicionUnos[i].y] = 0;
						matrizCeldas[m_posicionUnos[i].x][m_posicionUnos[i].y] = 1;
					}
				}
				return true; break;
			}
			else if(CorroborarObjeto(matrizCeldas,valorForma)) {return true; break;}
			else return false; break;
		}
		case 2:{
			if(m_posicionUnos[0].y==0){
				if(matrizCeldas[m_posicionUnos[1].x][m_posicionUnos[1].y+1]>1) return false;
				if(matrizCeldas[m_posicionUnos[2].x][m_posicionUnos[2].y+1]>1) return false;
				if(matrizCeldas[m_posicionUnos[3].x][m_posicionUnos[3].y+1]>1) return false;
				for(size_t i=0;i<m_posicionUnos.size();i++) m_posicionUnos[i].y = m_posicionUnos[i].y + 1;
				if(matrizCeldas[m_posicionUnos[2].x-1][m_posicionUnos[2].y-1]>1) return false;
				if(matrizCeldas[m_posicionUnos[3].x-1][m_posicionUnos[3].y+1]>1) return false;
				for(size_t i=0;i<m_posicionUnos.size();i++){
					if(i!=1) matrizCeldas[m_posicionUnos[i].x][m_posicionUnos[i].y-1] = 0;
					if(i!=0) matrizCeldas[m_posicionUnos[i].x][m_posicionUnos[i].y] = 1;
				}
				return true; break;
			}else if(CorroborarObjeto(matrizCeldas,valorForma)) {return true; break;}
			else return false; break;
		}
		default:{
			if(m_posicionUnos[0].x==0 || m_posicionUnos[0].x==1){
				if(m_posicionUnos[0].x==0){
					for(size_t i=0;i<m_posicionUnos.size();i++) {
						if(matrizCeldas[m_posicionUnos[i].x+2][m_posicionUnos[i].y]>1) return false;
					}
					for(size_t i=0;i<m_posicionUnos.size();i++) m_posicionUnos[i].x = m_posicionUnos[i].x + 2;
					if(matrizCeldas[m_posicionUnos[0].x-2][m_posicionUnos[0].y+2]>1) return false;
					if(matrizCeldas[m_posicionUnos[1].x-1][m_posicionUnos[1].y+1]>1) return false;
					for(size_t i=0;i<m_posicionUnos.size();i++){
						matrizCeldas[m_posicionUnos[i].x-2][m_posicionUnos[i].y] = 0;
						matrizCeldas[m_posicionUnos[i].x][m_posicionUnos[i].y] = 1;
					}
				}
				else{
					if(matrizCeldas[m_posicionUnos[0].x+1][m_posicionUnos[0].y]>1) return false;
					if(matrizCeldas[m_posicionUnos[1].x+1][m_posicionUnos[1].y]>1) return false;
					if(matrizCeldas[m_posicionUnos[3].x+1][m_posicionUnos[3].y]>1) return false;
					for(size_t i=0;i<m_posicionUnos.size();i++) m_posicionUnos[i].x = m_posicionUnos[i].x + 1;
					if(matrizCeldas[m_posicionUnos[0].x-2][m_posicionUnos[0].y+2]>1) return false;
					if(matrizCeldas[m_posicionUnos[1].x-1][m_posicionUnos[1].y+1]>1) return false;
					for(size_t i=0;i<m_posicionUnos.size();i++){
						if(i!=3) matrizCeldas[m_posicionUnos[i].x-1][m_posicionUnos[i].y] = 0;
						if(i!=2) matrizCeldas[m_posicionUnos[i].x][m_posicionUnos[i].y] = 1;
					}
				}
				return true; break;
			}else if(CorroborarObjeto(matrizCeldas,valorForma)) {return true; break;}
			else return false; break;
		}
		}
		break;
	}
	case 4:{ //Linea.
		switch(numRotaciones){
		case 0:{
			if(m_posicionUnos[0].x==0 || m_posicionUnos[0].x==9){
				if(m_posicionUnos[0].x==0){
					for(size_t i=0;i<m_posicionUnos.size();i++){
						if(matrizCeldas[m_posicionUnos[i].x+1][m_posicionUnos[i].y]>1) return false;
					}
					for(size_t i=0;i<m_posicionUnos.size();i++) m_posicionUnos[i].x = m_posicionUnos[i].x + 1;
					if(matrizCeldas[m_posicionUnos[0].x-1][m_posicionUnos[0].y+1]>1) return false;
					if(matrizCeldas[m_posicionUnos[2].x+1][m_posicionUnos[2].y-1]>1) return false;
					if(matrizCeldas[m_posicionUnos[3].x+2][m_posicionUnos[3].y-2]>1) return false;
					for(size_t i=0;i<m_posicionUnos.size();i++){
						matrizCeldas[m_posicionUnos[i].x-1][m_posicionUnos[i].y] = 0;
						matrizCeldas[m_posicionUnos[i].x][m_posicionUnos[i].y] = 1;
					}
				}
				else {
					for(size_t i=0;i<m_posicionUnos.size();i++){
						if(matrizCeldas[m_posicionUnos[i].x-1][m_posicionUnos[i].y]>1) return false;
					}
					for(size_t i=0;i<m_posicionUnos.size();i++) m_posicionUnos[i].x = m_posicionUnos[i].x - 1;
					if(matrizCeldas[m_posicionUnos[0].x-2][m_posicionUnos[0].y+1]>1) return false;
					if(matrizCeldas[m_posicionUnos[2].x-1][m_posicionUnos[2].y-1]>1) return false;
					if(matrizCeldas[m_posicionUnos[3].x+1][m_posicionUnos[2].y-2]>1) return false;
					for(size_t i=0;i<m_posicionUnos.size();i++){
						matrizCeldas[m_posicionUnos[i].x+1][m_posicionUnos[i].y] = 0;
						matrizCeldas[m_posicionUnos[i].x][m_posicionUnos[i].y] = 1;
					}
					cambioDeLado = true;
				}
				return true; break;
			}else if(CorroborarObjeto(matrizCeldas,valorForma)) {return true; break;}
			else return false; break;
		}
		default:{
			if(m_posicionUnos[2].x==8){
				m_posicionUnos[3].x = m_posicionUnos[3].x - 1;
				if(matrizCeldas[m_posicionUnos[0].x+2][m_posicionUnos[0].y-1]>1) return false;
				if(matrizCeldas[m_posicionUnos[1].x+1][m_posicionUnos[1].y+1]>1) return false;
				if(matrizCeldas[m_posicionUnos[3].x][m_posicionUnos[3].y+2]>1) return false;
				return true; break;
			}
			else if(CorroborarObjeto(matrizCeldas,valorForma)) {return true; break;}
			else return false; break;
		}
		}
		break;
	}
	case 5:{ //EseIzq.
		switch(numRotaciones){
		case 0:{
			if(m_posicionUnos[0].y==0){
				if(matrizCeldas[m_posicionUnos[0].x][m_posicionUnos[0].y+1]>1) return false;
				if(matrizCeldas[m_posicionUnos[2].x][m_posicionUnos[2].y+1]>1) return false;
				if(matrizCeldas[m_posicionUnos[3].x][m_posicionUnos[3].y+1]>1) return false;
				for(size_t i=0;i<m_posicionUnos.size();i++) m_posicionUnos[i].y = m_posicionUnos[i].y + 1;
				if(matrizCeldas[m_posicionUnos[2].x+1][m_posicionUnos[2].y-1]>1) return false;
				if(matrizCeldas[m_posicionUnos[3].x][m_posicionUnos[3].y-2]>1) return false;
				for(size_t i=0;i<m_posicionUnos.size();i++){
					if(i!=2) matrizCeldas[m_posicionUnos[i].x][m_posicionUnos[i].y-1] = 0;
					if(i!=1) matrizCeldas[m_posicionUnos[i].x][m_posicionUnos[i].y] = 1;
				}
				return true; break;
			}else if(CorroborarObjeto(matrizCeldas,valorForma)) {return true; break;}
			else return false; break;
		}
		default:{
			if(m_posicionUnos[0].x==0){
				if(matrizCeldas[m_posicionUnos[1].x+1][m_posicionUnos[1].y]>1) return false;
				if(matrizCeldas[m_posicionUnos[2].x+1][m_posicionUnos[2].y]>1) return false;
				if(matrizCeldas[m_posicionUnos[3].x+1][m_posicionUnos[3].y]>1) return false;
				for(size_t i=0;i<m_posicionUnos.size();i++) m_posicionUnos[i].x = m_posicionUnos[i].x + 1;
				if(matrizCeldas[m_posicionUnos[1].x-1][m_posicionUnos[1].y-1]>1) return false;
				if(matrizCeldas[m_posicionUnos[2].x][m_posicionUnos[2].y+2]>1) return false;
				for(size_t i=0;i<m_posicionUnos.size();i++){
					if(i!=3) matrizCeldas[m_posicionUnos[i].x-1][m_posicionUnos[i].y] = 0;
					if(i!=0) matrizCeldas[m_posicionUnos[i].x][m_posicionUnos[i].y] = 1;
				}
				return true; break;
			}else if(CorroborarObjeto(matrizCeldas,valorForma)) {return true; break;}
			else return false; break;
		}
		}
		break;
	}
	case 6:{ //EseDer.
		switch(numRotaciones){
		case 0:{
			if(m_posicionUnos[1].y==0){
				if(matrizCeldas[m_posicionUnos[0].x][m_posicionUnos[0].y+1]>1) return false;
				if(matrizCeldas[m_posicionUnos[2].x][m_posicionUnos[2].y+1]>1) return false;
				if(matrizCeldas[m_posicionUnos[3].x][m_posicionUnos[3].y+1]>1) return false;
				for(size_t i=0;i<m_posicionUnos.size();i++) m_posicionUnos[i].y = m_posicionUnos[i].y + 1;
				if(matrizCeldas[m_posicionUnos[0].x+2][m_posicionUnos[0].y]>1) return false;
				if(matrizCeldas[m_posicionUnos[3].x-1][m_posicionUnos[3].y-1]>1) return false;
				for(size_t i=0;i<m_posicionUnos.size();i++){
					if(i!=2) matrizCeldas[m_posicionUnos[i].x][m_posicionUnos[i].y-1] = 0;
					if(i!=1) matrizCeldas[m_posicionUnos[i].x][m_posicionUnos[i].y] = 1;
				}
				return true; break;
			}else if(CorroborarObjeto(matrizCeldas,valorForma)) {return true; break;}
			else return false; break;
		}
		default:{
			if(m_posicionUnos[0].x==0){
				if(matrizCeldas[m_posicionUnos[0].x+1][m_posicionUnos[0].y]>1) return false;
				if(matrizCeldas[m_posicionUnos[2].x+1][m_posicionUnos[2].y]>1) return false;
				if(matrizCeldas[m_posicionUnos[3].x+1][m_posicionUnos[3].y]>1) return false;
				for(size_t i=0;i<m_posicionUnos.size();i++) m_posicionUnos[i].x = m_posicionUnos[i].x + 1;
				if(matrizCeldas[m_posicionUnos[2].x-1][m_posicionUnos[2].y+1]>1) return false;
				if(matrizCeldas[m_posicionUnos[3].x-2][m_posicionUnos[3].y]>1) return false;
				for(size_t i=0;i<m_posicionUnos.size();i++){
					if(i!=2) matrizCeldas[m_posicionUnos[i].x-1][m_posicionUnos[i].y] = 0;
					if(i!=1) matrizCeldas[m_posicionUnos[i].x][m_posicionUnos[i].y] = 1;
				}
				return true; break;
			}else if(CorroborarObjeto(matrizCeldas,valorForma)) {return true; break;}
			else return false; break;
		}
		}
		break;
	}
	case 7:{ //Te.
		switch(numRotaciones){
		case 0:{
			if(m_posicionUnos[0].y==0){
				if(matrizCeldas[m_posicionUnos[0].x][m_posicionUnos[0].y+1]>1) return false;
				if(matrizCeldas[m_posicionUnos[2].x][m_posicionUnos[2].y+1]>1) return false;
				if(matrizCeldas[m_posicionUnos[3].x][m_posicionUnos[3].y+1]>1) return false;
				for(size_t i=0;i<m_posicionUnos.size();i++) m_posicionUnos[i].y = m_posicionUnos[i].y + 1;
				if(matrizCeldas[m_posicionUnos[3].x-1][m_posicionUnos[3].y-1]>1) return false;
				for(size_t i=0;i<m_posicionUnos.size();i++){
					if(i!=2) matrizCeldas[m_posicionUnos[i].x][m_posicionUnos[i].y-1] = 0;
					if(i!=1) matrizCeldas[m_posicionUnos[i].x][m_posicionUnos[i].y] = 1;
				}
				return true; break;
			}else if(CorroborarObjeto(matrizCeldas,valorForma)) {return true; break;}
			return false; break;
		}
		case 1:{
			if(m_posicionUnos[0].x==0){
				if(matrizCeldas[m_posicionUnos[0].x+1][m_posicionUnos[0].y]>1) return false;
				if(matrizCeldas[m_posicionUnos[2].x+1][m_posicionUnos[2].y]>1) return false;
				if(matrizCeldas[m_posicionUnos[3].x+1][m_posicionUnos[3].y]>1) return false;
				for(size_t i=0;i<m_posicionUnos.size();i++) m_posicionUnos[i].x = m_posicionUnos[i].x + 1;
				if(matrizCeldas[m_posicionUnos[0].x-1][m_posicionUnos[0].y+1]>1) return false;
				for(size_t i=0;i<m_posicionUnos.size();i++){
					if(i!=3) matrizCeldas[m_posicionUnos[i].x-1][m_posicionUnos[i].y] = 0;
					if(i!=1) matrizCeldas[m_posicionUnos[i].x][m_posicionUnos[i].y] = 1;
				}
				return true; break;
			}else if(CorroborarObjeto(matrizCeldas,valorForma)) {return true; break;}
			else return false; break;
		}
		case 2:{
			if(CorroborarObjeto(matrizCeldas,valorForma)) {return true; break;}
			else return false; break;
		}
		default:{
			if(m_posicionUnos[1].x==9){
				if(matrizCeldas[m_posicionUnos[0].x-1][m_posicionUnos[0].y]>1) return false;
				if(matrizCeldas[m_posicionUnos[1].x-1][m_posicionUnos[1].y]>1) return false;
				if(matrizCeldas[m_posicionUnos[3].x-1][m_posicionUnos[3].y]>1) return false;
				for(size_t i=0;i<m_posicionUnos.size();i++) m_posicionUnos[i].x = m_posicionUnos[i].x - 1;
				if(matrizCeldas[m_posicionUnos[1].x+1][m_posicionUnos[1].y+1]>1) return false;
				for(size_t i=0;i<m_posicionUnos.size();i++){
					if(i>0) matrizCeldas[m_posicionUnos[i].x+1][m_posicionUnos[i].y] = 0;
					if(i!=2) matrizCeldas[m_posicionUnos[i].x][m_posicionUnos[i].y] = 1;
				}
				return true; break;
			}else if(CorroborarObjeto(matrizCeldas,valorForma)) {return true; break;}
			else return false; break;
		}
		}
		break;
	}
	}
}

/*
Este metodo corrobora que, donde se vaya a hacer la modificacion numerica, no haya otro objeto ya consolidado, de manera de
no borrar parte de este. Dependiendo la forma, y el numero de rotacion, los condicionales cambian.
*/
bool Jugador::CorroborarObjeto(vector<vector<int>> &matrizCeldas,int valorForma){
	switch(valorForma){
	case 2:{ //EleIzq.
		switch(numRotaciones){
		case 0:{
			if(matrizCeldas[m_posicionUnos[0].x-1][m_posicionUnos[0].y+1]>1) return false;
			if(matrizCeldas[m_posicionUnos[3].x-2][m_posicionUnos[3].y-2]>1) return false;
			break;
		}
		case 1:{
			if(matrizCeldas[m_posicionUnos[0].x+1][m_posicionUnos[0].y+1]>1) return false;
			if(matrizCeldas[m_posicionUnos[1].x-2][m_posicionUnos[1].y+2]>1) return false;
			break;
		}
		case 2:{
			if(matrizCeldas[m_posicionUnos[0].x+2][m_posicionUnos[0].y+2]>1) return false;
			if(matrizCeldas[m_posicionUnos[3].x+1][m_posicionUnos[3].y-1]>1) return false;
			break;
		}
		default:{
			if(matrizCeldas[m_posicionUnos[2].x+2][m_posicionUnos[2].y-2]>1) return false;
			if(matrizCeldas[m_posicionUnos[3].x-1][m_posicionUnos[3].y-1]>1) return false;
			break;
		}
		}
		break;
	}
	case 3:{ //EleDer.
		switch(numRotaciones){
		case 0:{
			if(matrizCeldas[m_posicionUnos[0].x+2][m_posicionUnos[0].y+2]>1) return false;
			if(matrizCeldas[m_posicionUnos[1].x+1][m_posicionUnos[1].y+1]>1) return false;
			break;
		}
		case 1:{
			if(matrizCeldas[m_posicionUnos[2].x+1][m_posicionUnos[2].y-1]>1) return false;
			if(matrizCeldas[m_posicionUnos[3].x+2][m_posicionUnos[3].y-2]>1) return false;
			break;
		}
		case 2:{
			if(matrizCeldas[m_posicionUnos[2].x-1][m_posicionUnos[2].y-1]>1) return false;
			if(matrizCeldas[m_posicionUnos[3].x-1][m_posicionUnos[3].y+1]>1) return false;
			break;
		}
		default:{
			if(matrizCeldas[m_posicionUnos[0].x-2][m_posicionUnos[0].y+2]>1) return false;
			if(matrizCeldas[m_posicionUnos[1].x-1][m_posicionUnos[1].y+1]>1) return false;
			break;
		}
		}
		break;
	}
	case 4:{ //Linea.
		switch(numRotaciones){
		case 0:{
			if(m_posicionUnos[3].x+2>9){
				if(matrizCeldas[m_posicionUnos[3].x+1][m_posicionUnos[3].y-2]>1) return false;
				if(matrizCeldas[m_posicionUnos[0].x-2][m_posicionUnos[0].y+1]>1) return false;
				if(matrizCeldas[m_posicionUnos[2].x-1][m_posicionUnos[2].y-1]>1) return false;
				cambioDeLado = true;
			}
			else{
				if(matrizCeldas[m_posicionUnos[3].x+2][m_posicionUnos[3].y-2]>1) {
					if(matrizCeldas[m_posicionUnos[3].x+1][m_posicionUnos[3].y-2]<=1){
						if(matrizCeldas[m_posicionUnos[0].x-2][m_posicionUnos[0].y+1]>1) return false;
						if(matrizCeldas[m_posicionUnos[2].x-1][m_posicionUnos[2].y-1]>1) return false;
						cambioDeLado = true;
					}else return false;
				}else{
					if(matrizCeldas[m_posicionUnos[0].x-1][m_posicionUnos[0].y+1]>1) return false;
					if(matrizCeldas[m_posicionUnos[2].x+1][m_posicionUnos[2].y-1]>1) return false;
				}
				break;
			}
		}
		default:{
			if(matrizCeldas[m_posicionUnos[0].x+1][m_posicionUnos[0].y-1]>1) return false;
			if(matrizCeldas[m_posicionUnos[2].x-1][m_posicionUnos[2].y+1]>1) return false;
			if(matrizCeldas[m_posicionUnos[3].x-2][m_posicionUnos[3].y+2]>1) return false;
		}
		}
		break;
	}
	case 5:{ //EseIzq.
		switch(numRotaciones){
		case 0:{
			if(matrizCeldas[m_posicionUnos[2].x+1][m_posicionUnos[2].y-1]>1) return false;
			if(matrizCeldas[m_posicionUnos[3].x][m_posicionUnos[3].y-2]>1) return false;
		}
		default:{
			if(matrizCeldas[m_posicionUnos[1].x-1][m_posicionUnos[1].y-1]>1) return false;
			if(matrizCeldas[m_posicionUnos[2].x][m_posicionUnos[2].y+2]>1) return false;
		}	
		}
		break;
	}
	case 6:{ //EseDer.
		switch(numRotaciones){
		case 0:{
			if(matrizCeldas[m_posicionUnos[0].x+2][m_posicionUnos[0].y]>1) return false;
			if(matrizCeldas[m_posicionUnos[3].x-1][m_posicionUnos[3].y-1]>1) return false;
		}
		default:{
			if(matrizCeldas[m_posicionUnos[2].x-1][m_posicionUnos[2].y+1]>1) return false;;
			if(matrizCeldas[m_posicionUnos[3].x-2][m_posicionUnos[3].y]>1) return false;
		}
		}
		break;
	}
	case 7:{ //Te.
		switch(numRotaciones){
		case 0:{
			if(matrizCeldas[m_posicionUnos[3].x-1][m_posicionUnos[3].y-1]>1) return false;
			break;
		}
		case 1:{
			if(matrizCeldas[m_posicionUnos[0].x-1][m_posicionUnos[0].y+1]>1) return false;
			break;
		}
		case 2:{
			if(matrizCeldas[m_posicionUnos[0].x+1][m_posicionUnos[0].y+1]>1) return false;
			break;
		}
		default:{
			if(matrizCeldas[m_posicionUnos[1].x+1][m_posicionUnos[1].y+1]>1) return false;
			break;
		}
		}
		break;
	}
	}
	return true;
}

/*
Este metodo es el encargado de corroborar, para las funciones 'MoverIzq', 'MoverDer', y 'MoverAbajo', que la accion pueda
realizarse, al verificar que, al mover la forma, no se salga de la matriz.
*/
bool Jugador::CorroborarMover(vector<vector<int>> &matrizCeldas,int i,int j,int lado){
	switch(lado){
	case 1: {if(i-1<0 || matrizCeldas[i-1][j]>1) return false; break;} /*Si al RectangleShape al restarle una columna en posicion se 
																	sale de la matriz, el metodo devuelve falso y se deniega la accion.*/
	case 2: {if(i+1>9 || matrizCeldas[i+1][j]>1) return false; break;} /*Si al RectangleShape al sumarle una columna en posicion se 
																	sale de la matriz, el metodo devuelve falso y se deniega la accion.*/
	default: {if(j+1>19 || matrizCeldas[i][j+1]>1) return false; break;}}/*Si al RectangleShape al sumarle una fila en posicion se 
																	sale de la matriz, el metodo devuelve falso y se deniega la accion.*/
	return true;
}

//Este metodo es el encargado de bajar la forma hasta que llegue al final de la matriz o 'choque' con otro objeto.
void Jugador::BajarForma(vector<vector<int>> &matrizCeldas,int valorForma,int &score){
	Globales::CambiarEstado(8,false);
	/*Se realiza un bucle del cual no se sale hasta que la forma llegue hasta el final de la matriz o 'choque' con otro objeto,
	verificado gracias a los metodos de la clase 'Formas', 'VerificarMovimientos' y 'VerificarColision'.*/
	while(formas.VerificarMovimientos(matrizCeldas,valorForma)==true && formas.VerificarColision(matrizCeldas,valorForma)==true){
		puedeMoverse = 0;
		for(size_t i=0;i<m_posicionUnos.size();i++) {
			/*Se corrobora que a las distintas posiciones de la forma se le pueda sumar una fila sin que estas se salgan de la
			matriz.*/
			if(CorroborarMover(matrizCeldas,m_posicionUnos[i].x,m_posicionUnos[i].y,3)) puedeMoverse++;
			else puedeMoverse = 0;
		}
		if(puedeMoverse==4){
			for(size_t i=m_posicionUnos.size();i>0;i--){
				/*Si es posible realizar la modificacion, se suma a las posiciones de cada RectangleShape una fila; luego se
				modifica la matriz, colocando un uno '1' en la nueva posicion y un cero '0' en la antigua, modificando, asi, la
				posicion de la forma en la matriz.*/
				m_posicionUnos[i-1].y = m_posicionUnos[i-1].y + 1;
				matrizCeldas[m_posicionUnos[i-1].x][m_posicionUnos[i-1].y] = 1;
				matrizCeldas[m_posicionUnos[i-1].x][m_posicionUnos[i-1].y-1] = 0;
			}
			score++; //Se suma 1 punto al score del usuario.
		}
	}
	Globales::CambiarEstado(2,true); numRotaciones = 0; reloj.restart();
}

//Este metodo verifica si el usuario apreto la tecla de Pausa para, justamente, pausar el juego.
bool Jugador::TecleoPausa(){
	primeraVezPausa = false; /*Si no se ha apretado la tecla pausa, cambiamos el estado a 'false', de modo que ciertas acciones
							en la clase 'Partida' se realicen una unica vez.*/
	if(Keyboard::isKeyPressed(Globales::VerTecla(5))){ //Condicional que verifica si se apreto la tecla relacionada con pausar el juego.
		if(reloj.getElapsedTime().asMilliseconds()>150){ /*Si lo apreto, y si el tiempo es mayor al numero especificado, se
														se cambia el estado:
															1) Si estaba en Pausa, se cambia de true a false, reiniciando el juego.
															Ademas, se cambia el estado de otra variable para indicar que el juego
															se acaba de despausar para realizar ciertas acciones en la clase 'Partida'.
															2) Si estaba corriendo el juego, se cambia de false a true, pausando
															el juego.*/
			if(Globales::VerEstado(11)) {Globales::CambiarEstado(11,false); Globales::CambiarEstado(12,true);}
			else {Globales::CambiarEstado(11,true); primeraVezPausa = true;}
			reloj.restart(); //Reiniciamos el reloj.
		}
	}
	return primeraVezPausa;
}
