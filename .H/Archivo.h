/*
Esta clase es la encargada de la lectura y modificacion de los distintos archivos, ya sean binarios o de texto, utilizados
en el juego. 
*/
#ifndef ARCHIVO_H
#define ARCHIVO_H
#include "Escalas.h"

class Archivo {
public:
	Archivo();
	int LeerScore(int dificultad,vector<Usuario> &vectorUsuario);
	void LeerControl(vector<string> &vectorCadenas);
	void LeerTeclas(vector<string> &vectorCadenas);
	int LeerHayMejorPuntaje(int dificultad,int &tamanioArchivo,int m_score,int &scoreArchivo);
	void EscribirPuntaje(int dificultad,int m_score,int posicionEscribir,int tamanioArchivo);
	void VerMejorPuntaje(int dificultad,int &mejorPuntaje,string &m_mejorPuntaje);
	void ModificarArchivo();
private:
	fstream io_archivo; //Para lectura y escritura de archivos.
	ifstream i_archivo; //Para lectura de archivos.
	ofstream o_archivo; //Para escritura de archivos.
};

#endif

