#include "Archivo.h"

Archivo::Archivo() {
	
}

/*
Esta funcion se encarga de abrir el archivo correspondiente (segunda la dificultad seteada) 
para su lectura y almacenamiento en un vector de tipo Usuario (Usuario en este caso es un struct que contiene un variable 
tipo char[20] y una variable del tipo int). Asimismo, se  guardar el tamaño del archivo leeido para ser devuelto de donde 
haya sido llamada este metodo.
*/
int Archivo::LeerScore(int dificultad,vector<Usuario> &vectorUsuario){
	int tamanioArchivo = 0; //Reiniciamos la variable.
	if(Globales::VerEstado(14)){ //Verificamos si el modo 'Fila piedra' esta activado.
		switch(dificultad){ //Segun la dificultad ajustada, se abre uno u otro archivo.
		case 0: {
			io_archivo.open("Puntajes/ScoreFacilPiedra.txt",ios::binary|ios::in|ios::ate); /*Se abre el archivo en lectura binaria,
																				permitiendo entrada de datos y al final de este 
																			(con el objetido  de calcular el tamaño del archivo).*/
			//Verificamos si el archivo pudo abrirse con normalidad.
			if(!io_archivo.is_open()) {cerr<<"No se pudo abrir el archivo 'ScoreFacilPiedra.txt'"<<endl; break;}
			else break;
		}
		case 1: {
			io_archivo.open("Puntajes/ScoreMedioPiedra.txt",ios::binary|ios::in|ios::ate); 
			if(!io_archivo.is_open()) {cerr<<"No se pudo abrir el archivo 'ScoreMedioPiedra.txt'"<<endl; break;}
			else break;
		}
		default: {
			io_archivo.open("Puntajes/ScoreDificilPiedra.txt",ios::binary|ios::in|ios::ate); 
			if(!io_archivo.is_open()) {cerr<<"No se pudo abrir el archivo 'ScoreDificilPiedra.txt'"<<endl; break;}
			else break;
		}
		}
	}
	else{ //Si el modo 'Fila piedra' no esta activado, se abren los siguientes archivos (dependiendo de la dificultad seteada).
		switch(dificultad){
		case 0: {
			io_archivo.open("Puntajes/ScoreFacil.txt",ios::binary|ios::in|ios::ate); /*Se abre el archivo en lectura binaria, permitiendo
																				entrada de datos y al final de este (con el objetido 
																				de calcular el tamaño del archivo).*/
			if(!io_archivo.is_open()) {cerr<<"No se pudo abrir el archivo 'ScoreFacil.txt'"<<endl; break;}
			else break;
		}
		case 1: {
			io_archivo.open("Puntajes/ScoreMedio.txt",ios::binary|ios::in|ios::ate); 
			if(!io_archivo.is_open()) {cerr<<"No se pudo abrir el archivo 'ScoreMedio.txt'"<<endl; break;}
			else break;
		}
		default: {
			io_archivo.open("Puntajes/ScoreDificil.txt",ios::binary|ios::in|ios::ate); 
			if(!io_archivo.is_open()) {cerr<<"No se pudo abrir el archivo 'ScoreDificil.txt'"<<endl; break;}
			else break;
		}
		}
	}
	if(io_archivo.is_open()){
		vectorUsuario.resize(0); 
		tamanioArchivo = io_archivo.tellg()/sizeof(Usuario); io_archivo.seekg(0); /*Calculamos el tamaño del archivo y, luego, volvemos
																					al inicio de este.*/
		for(int i=0;i<tamanioArchivo;i++){
			Usuario user; io_archivo.read(reinterpret_cast<char*>(&user),sizeof(Usuario)); //Leemos el contenido del archivo.
			vectorUsuario.push_back(user); //Guardamos cada par de datos (char[20]-int) en un vector.
		}
		io_archivo.close(); //Cerramos el archivo.
	}
	return tamanioArchivo;
}

//Este metodo se encarga de guardar en un vector de tipo string el contenido de el archivo de texto 'Controles.txt'.
void Archivo::LeerControl(vector<string> &vectorCadenas){
	vectorCadenas.resize(0);
	i_archivo.open("Control-Teclas/Controles.txt"); //Abrimos el archivo.
	if(!i_archivo.is_open()) cerr<<"No se pudo abrir el archivo 'Controles.txt'"<<endl; //Verificamos si el archivo pudo encontrarse.
	else{
		string linea; //Reiniciamos la variable.
		while(i_archivo >> linea) vectorCadenas.push_back(linea); /*Mientras haya contenido para leer, se lo almacena en la variable
																	linea.*/
	}
	i_archivo.close(); //Cerramos el archivo.
}

//Este metodo se encarga de guardar en un vector de tipo string el contenido de el archivo de texto 'Teclas.txt'.
void Archivo::LeerTeclas(vector<string> &vectorCadenas){
	vectorCadenas.resize(0);
	i_archivo.open("Control-Teclas/Teclas.txt"); //Abrimos el archivo.
	if(!i_archivo.is_open()) cerr<<"No se pudo abrir el archivo 'Teclas'"<<endl; //Verificamos si el archivo pudo encontrarse.
	else{
		string tecla;
		while(i_archivo >> tecla) vectorCadenas.push_back(tecla); /*Mientras haya contenido para leer, se lo almacena en la variable
																linea.*/
	}
	i_archivo.close(); //Cerramos el archivo.
}

/*
Esta funcion se encarga de abrir el archivo correspondiente (segun la dificultad seteada) para verificar si el puntaje 
hecho por el usuario supera o no a algunas de las almacenadas. Si supera, pero no al mejor, se guarda el puntaje de este ultimo
para ser mostrado en pantalla. Al superar algun puntaje, se devuelve la posicion en donde debe ser insertado el nuevo puntaje 
hecho por el jugador.
*/
int Archivo::LeerHayMejorPuntaje(int dificultad,int &tamanioArchivo,int m_score,int &scoreArchivo){
	int contadorPosicion = 0; //Reiniciamos la variable.
	if(Globales::VerEstado(14)){ //Verificamos si el modo 'Fila piedra' esta activada.
		switch(dificultad){ //Segun la dificulta ajustada, se abrira uno u otro archivo.
		case 0:{
			io_archivo.open("Puntajes/ScoreFacilPiedra.txt",ios::binary|ios::in|ios::ate); /*Se abre el archivo en lectura binaria,
																						permitiendo la entrada de datos y al final de
																					este (para calcular el tamaño del archivo).*/
			//Verificamos si el archivo pudo abrise.
			if(!io_archivo.is_open()) {cerr<<"No se pudo abrir el archivo 'ScoreFacilPiedra.txt"<<endl; return -1; break;}
			else break;
		}
		case 1:{
			io_archivo.open("Puntajes/ScoreMedioPiedra.txt",ios::binary|ios::in|ios::ate);
			if(!io_archivo.is_open()) {cerr<<"No se pudo abrir el archivo 'ScoreMedioPiedra.txt"<<endl; return -1; break;}
			else break;
		}
		default:{
			io_archivo.open("Puntajes/ScoreDificilPiedra.txt",ios::binary|ios::in|ios::ate);
			if(!io_archivo.is_open()) {cerr<<"No se pudo abrir el archivo 'ScoreDificilPiedra.txt"<<endl; return -1; break;}
			else break;
		}	
		}
	}
	else{ //Si el modo 'Fila piedra' no esta activado, se abren los siguientes archivos (dependiendo de la dificultad seteada).
		switch(dificultad){
		case 0:{
			io_archivo.open("Puntajes/ScoreFacil.txt",ios::binary|ios::in|ios::ate); /*Se abre el archivo en lectura binaria, permitiendo
																				la entrada de datos, y al final de este (para calcular
																				el tamaño del archivo).*/
			if(!io_archivo.is_open()) {cerr<<"No se pudo abrir el archivo 'ScoreFacil.txt"<<endl; return -1; break;}
			else break;
		}
		case 1:{
			io_archivo.open("Puntajes/ScoreMedio.txt",ios::binary|ios::in|ios::ate);
			if(!io_archivo.is_open()) {cerr<<"No se pudo abrir el archivo 'ScoreMedio.txt"<<endl; return -1; break;}
			else break;
		}
		default:{
			io_archivo.open("Puntajes/ScoreDificil.txt",ios::binary|ios::in|ios::ate);
			if(!io_archivo.is_open()) {cerr<<"No se pudo abrir el archivo 'ScoreDificil.txt"<<endl; return -1; break;}
			else break;
		}	
		}
	}
	if(io_archivo.is_open()){ //Verificamos que el archivo se haya abierto.
		tamanioArchivo = io_archivo.tellg()/sizeof(Usuario); io_archivo.seekg(0); /*Se calcula el tamaño del archivo y se vuelve
																					al inicio de este.*/
		for(int i=0;i<tamanioArchivo;i++){
			Usuario user; io_archivo.read(reinterpret_cast<char*>(&user),sizeof(Usuario)); /*Se lee y almacena en el struct 'user'
																						de 'Usuaro' la informacion de la primera
																						linea del archivo, y asi sucesivamente 
																						hasta llegar al final.*/
			if(m_score>user.scoreUsuario) {io_archivo.close(); return contadorPosicion; break;} /*Si verificamos que el puntaje
																							hecho por el usuario supera al puntaje 
																						guardado en ese momento en el struct 'user',
																						cerramos el archivo y devolvemos la posicion.*/
			else { //Sino supera al puntaje del momento, aumentamos la posicion.
				contadorPosicion++; 
				if(contadorPosicion==1) scoreArchivo = user.scoreUsuario; /*Si no se supera a la primera posicion, se guarda el
																			puntaje de este, para ser mostrado en pantalla.*/
			}
		}
		io_archivo.close(); //Cerramos el archivo.
		return -1; /*Sino supero a ningun puntaje, se devuelve un valor significativo para que no se realice ninguna accion
					especial.*/
	}
}

/*
Esta funcion es la encargada de abrir el archivo especifico (segun la dificultad seteada) con el objetivo de
leer y escribir los puntajes almacenadas, mas el hecho por el usuario.
*/
void Archivo::EscribirPuntaje(int dificultad,int m_score,int posicionEscribir,int tamanioArchivo){
	if(Globales::VerEstado(14)){ //Verificamos si el modo 'Fila piedra' esta activado o no.
		switch(dificultad){ //Segun la dificultad ajusta, se abrira uno u otro archivo.
		case 0:{
			io_archivo.open("Puntajes/ScoreFacilPiedra.txt",ios::binary|ios::out|ios::in); /*Se abre el archivo en lectura binario, 
																						permitiendo la salida y entrada de datos.*/
			//Verificamos si el archivo pudo abrirse.
			if(!io_archivo.is_open()) {cerr<<"No se pudo abrir el archivo 'ScoreFacilPiedra.txt'"<<endl; break;}
			else break;
		}
		case 1:{
			io_archivo.open("Puntajes/ScoreMedioPiedra.txt",ios::binary|ios::out|ios::in);
			if(!io_archivo.is_open()) {cerr<<"No se pudo abrir el archivo 'ScoreMedioPiedra.txt'"<<endl; break;}
			else break;
		}
		default:{
			io_archivo.open("Puntajes/ScoreDificilPiedra.txt",ios::binary|ios::out|ios::in);
			if(!io_archivo.is_open()) {cerr<<"No se pudo abrir el archivo 'ScoreDificilPiedra.txt'"<<endl; break;}
			else break;
		}
		}
	}
	else{ //Si el modo 'Fila piedra' no esta activado, se abiran los siguientes archivos (dependiendo de la dificultad ajustada).
		switch(dificultad){
		case 0:{
			io_archivo.open("Puntajes/ScoreFacil.txt",ios::binary|ios::out|ios::in); /*Se abre el archivo en lectura binario, permitiendo
																						la salida y entrada de datos.*/
			if(!io_archivo.is_open()) {cerr<<"No se pudo abrir el archivo 'ScoreFacil.txt'"<<endl; break;}
			else break;
		}
		case 1:{
			io_archivo.open("Puntajes/ScoreMedio.txt",ios::binary|ios::out|ios::in);
			if(!io_archivo.is_open()) {cerr<<"No se pudo abrir el archivo 'ScoreMedio.txt'"<<endl; break;}
			else break;
		}
		default:{
			io_archivo.open("Puntajes/ScoreDificil.txt",ios::binary|ios::out|ios::in);
			if(!io_archivo.is_open()) {cerr<<"No se pudo abrir el archivo 'ScoreDificil.txt'"<<endl; break;}
			else break;
		}
		}
	}
	if(io_archivo.is_open()){ //Verificamos si el archivo esta abierto.
		if(posicionEscribir<9){ /*Condicional que verifica si la posicion en al cual se debe escribir el puntaje del usuario es
								menor a la decima posicion.*/
			for(int i=tamanioArchivo-2;i>=posicionEscribir;i--){ /*Se empieza con un int 'i' igual al tamaño del archivo - 2, ya que:
														1) Ya se ha verificado que el puntaje a escribir no debe ser insertado en la
														decima posicion, por lo que, al momento de haber escrito todos los puntajes,
														el usuario con el puntaje que haya estado en la decima posicion desaparecera.
														2) La posicion empieza desde el 0, y no desde el 1.*/
				io_archivo.seekp(i*sizeof(Usuario)); Usuario user; //Vamos a la posicion del archivo calculado.
				if(i==posicionEscribir){ /*Condicional para verificar que, donde estamos parados en el archivo, es donde debe ser
										escrito el puntaje del usuario.*/
					/*Si es asi, se leer el puntaje que haya en esa posicion y se lo escribe en la posicion siguiente.
					Luego se vuelve al mismo punto donde se empezo y se escribe el puntaje del usuario y su nombre.*/
					io_archivo.read(reinterpret_cast<char*>(&user),sizeof(Usuario));
					io_archivo.write(reinterpret_cast<char*>(&user),sizeof(Usuario));
					io_archivo.seekp(i*sizeof(Usuario));
					user.scoreUsuario = m_score; strcpy(user.nombre,Globales::VerCadena(6).c_str());
					io_archivo.write(reinterpret_cast<char*>(&user),sizeof(Usuario));
				}else{ //Se lee y se escribe un lugar "mas adelante" en la clasificacion el puntaje y nombre del almacenado.
					io_archivo.read(reinterpret_cast<char*>(&user),sizeof(Usuario));
					io_archivo.write(reinterpret_cast<char*>(&user),sizeof(Usuario));
				}
			}
		}else{ /*Si la posicion donde debe ser escrito el puntaje y nombre del usuario es la decima, simplemente se para en esa
				posicion en el archivo y se escribe.*/
			io_archivo.seekp(posicionEscribir*sizeof(Usuario)); Usuario user;
			user.scoreUsuario = m_score; strcpy(user.nombre,Globales::VerCadena(6).c_str());
			io_archivo.write(reinterpret_cast<char*>(&user),sizeof(Usuario));
		}
		io_archivo.close(); //Cerrramos el archivo.
	}
}

/*
Esta funcion se encarga de setear la variable de tipo int 'mejorPuntaje' y de tipo string 'm_mejorPuntaje' para ser
utilizadas como informacion en el juego.
*/
void Archivo::VerMejorPuntaje(int dificultad,int &mejorPuntaje,string &m_mejorPuntaje){
	if(Globales::VerEstado(14)){ //Verificamos si el modo 'Fila piedra' esta activado.
		switch(dificultad){ //Dependiendo de la dificultad se abira uno u otro archivo.
		case 0:{
			io_archivo.open("Puntajes/ScoreFacilPiedra.txt",ios::binary|ios::in); /*Se abre el archivo en lectura binaria, permitiendo la
																					entrada de datos.*/
			//Verificamos que el archivo este abierto.
			if(!io_archivo.is_open()) {cerr<<"No se pudo abrir el archivo 'ScoreFacilPiedra'"<<endl; break;}
			else break;
		}
		case 1:{
			io_archivo.open("Puntajes/ScoreMedioPiedra.txt",ios::binary|ios::in);
			if(!io_archivo.is_open()) {cerr<<"No se pudo abrir el archivo 'ScoreMedioPiedra'"<<endl; break;}
			else break;
		}
		default:{
			io_archivo.open("Puntajes/ScoreDificilPiedra.txt",ios::binary|ios::in);
			if(!io_archivo.is_open()) {cerr<<"No se pudo abrir el archivo 'ScoreDificilPiedra'"<<endl; break;}
			else break;
		}
		}
	}
	else{ //Si el modo 'Fila piedra' no esta activada, se abriran los siguientes archivos (dependiendo de la dificultad seteada).
		switch(dificultad){
		case 0:{
			io_archivo.open("Puntajes/ScoreFacil.txt",ios::binary|ios::in); /*Se abre el archivo en lectura binaria, permitiendo la
																			entrada de datos.*/
			if(!io_archivo.is_open()) {cerr<<"No se pudo abrir el archivo 'ScoreFacil'"<<endl; break;}
			else break;
		}
		case 1:{
			io_archivo.open("Puntajes/ScoreMedio.txt",ios::binary|ios::in);
			if(!io_archivo.is_open()) {cerr<<"No se pudo abrir el archivo 'ScoreMedio'"<<endl; break;}
			else break;
		}
		default:{
			io_archivo.open("Puntajes/ScoreDificil.txt",ios::binary|ios::in);
			if(!io_archivo.is_open()) {cerr<<"No se pudo abrir el archivo 'ScoreDificil'"<<endl; break;}
			else break;
		}
		}
	}
	if(io_archivo.is_open()){ //Verificamos que el archivo este abierto.
		//Se lee y, luego, se almacena el contenido del struct 'Usuario'.
		Usuario user; io_archivo.read(reinterpret_cast<char*>(&user),sizeof(Usuario)); 
		io_archivo.close(); //Cerramos el archivo.
		mejorPuntaje = user.scoreUsuario; m_mejorPuntaje = to_string(user.scoreUsuario);
	}
}

/*
Este metodo es el encargado de borrar el contenido anterior del archivo de texto 'Controles.txt' para almacenar las nuevas
variables.
*/
void Archivo::ModificarArchivo(){
	o_archivo.open("Control-Teclas/Controles.txt",ios::trunc); /*Se abre el archivo de texto 'Controles.txt', destruyendo todo su
																contenido anterior.*/
	o_archivo << "Dificultad=" + Globales::VerCadena(8) << endl;
	o_archivo << "Izquierda=" +  Globales::VerCadena(0) << endl;
	o_archivo << "Derecha=" +  Globales::VerCadena(1) << endl;
	o_archivo << "Abajo=" +  Globales::VerCadena(2) << endl;
	o_archivo << "Rotar=" +  Globales::VerCadena(3) << endl;
	o_archivo << "HardDrop=" +  Globales::VerCadena(4) << endl;
	o_archivo << "Color=" +  Globales::VerCadena(7) << endl;
	o_archivo << "Pausa=" + Globales::VerCadena(5) << endl;
	o_archivo << "FilasPiedra=" + Globales::VerCadena(9) << endl;
}
	
