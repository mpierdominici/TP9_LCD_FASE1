#include <iostream>

#include "basicLCD.h"
#include "mySleep.h"
#define FTD_H
#define FTD2XX_EXPORTS
#include "ftd2xx.h"

using namespace std;

basicLCD::basicLCD() //: cadd(1) , constr_ok(lcdInit()) //Convendria ahorrarse el llamado al constructor de ambos??? Queda mas claro como esta a cont. desde nuestra persp.
{
	//Constructor. 
	//Inicializa el LCD y deja todo listo comenzar a utilizarlo. 
	constr_ok=lcdInit(); //Secuencia para el modo de 4 bits. 
	cadd=1; //Como el display empieza en 0, cadd debe valer 0+1.

}

basicLCD::~basicLCD()
{
	//Destructor. 
	//Libera cualquier recurso que se hubiera tomado de forma de evitar "resources leak".
	 lcdClear();
}

bool basicLCD::  lcdClear ()
{
	//Borra el display y el cursor va a HOME. 
	//Se modifica cadd.
	
	bool action_status = true;
	
	cursorPosition pos; //creamos una estructura tipo cursorPosition. 
	pos=lcdGetCursorPosition (); //Se pide a la funcion que devuelva la posicion actual del cursor. 

	if (action_status == lcdWriteIR (CLEAR_SCREEN)) //Se envia la instruccion de limpiar el screeen.
	{ 
		pos.column = 0; //Se pone el cursor en la primera fila, primera columna. 
		pos.row = 0;
		action_status &= lcdSetCursorPosition(pos); //Se actualiza el cursor del display con las nuevas coordenadas, aqui se modifica cadd.
	}

	else
	{
		std::cout<<"Cannot clear screen"<<std::endl; 
		action_status = false; //La accion no fue satisfactoria.
	}

	return action_status;
}

bool basicLCD:: lcdClearToEOL()
{
	// Borra el display desde la posici�n actual (inclusive) del cursor hasta el final de la l�nea. 
	// No modifica cadd.

	bool action_status = true;
	
	cursorPosition pos; //creamos una estructura tipo cursorPosition. 
	pos=lcdGetCursorPosition (); //Se pide a la funcion que devuelva la posicion actual del cursor. 

	for (int i = 0; i < DISPLAY_LENGTH - pos.column; i++) //Recorre las posiciones desde donde esta el cursor hasta donde termina el display. 
		action_status &= lcdWriteDR(ASCII_SPACE);
	
	action_status &= lcdSetCursorPosition(pos); ////Se actualiza el cursor del display, en este caso no se modifica cadd.

	return action_status; //Devuelve si la accion fue satisfactoria o no. 
}

basicLCD& basicLCD::operator<<(const unsigned char c)
{
	//Pone el car�cter en la posici�n actual del cursor del display y avanza el cursor a la pr�xima
	//posici�n respetando el gap (si el car�cter no es imprimible
	//lo ignora). Se toma que la proxima posicion es a la derecha.

	bool action_status = true;

	//Se analiza si el caracter es imprimible (segun la tabla del documento). 

	if((c >= 0x20 && c <= 0x7F) || (c >= 0xA0 && c <= 0xFF))
	{		
		action_status = lcdWriteDR (c); //Se envia para escribir el caracter. 
		action_status &= lcdMoveCursorRight(); //El cursor se despaza hacia la derecha.
		mySleep(DELAY); //Delay.

		if (action_status != true)
			cout << "Invalid character" << endl;
	}

	return *this;
}




bool basicLCD:: lcdMoveCursorUp ()
{
	// Pasa el cursor a la primera l�nea del display sin alterar la columna en la que estaba. 
	// Modifica cadd.

	bool action_status = true;
	
	cursorPosition pos; //creamos una estructura tipo cursorPosition. 
	pos=lcdGetCursorPosition (); //Se pide a la funcion que devuelva la posicion actual del cursor. 

	if (pos.row != 0) //Se fija si el cursor NO esta en la primera fila. 
	{	
		pos.row--; //Si ese es el caso le resta 1 al valor de las filas. 	
		action_status &= lcdSetCursorPosition(pos); ////Se actualiza el cursor del display con las nuevas coordenadas, aqui se modifica cadd.
	}
	else
	{
		std::cout<<"Cursor can not go up"<<std::endl; 
		action_status = false; //La accion no fue satisfactoria.
	}

	return action_status;
}

bool basicLCD:: lcdMoveCursorDown  ()
{
	// Pasa el cursor a la segunda l�nea del display sin alterar la columna en la que estaba. 
	// Modifica cadd.

	bool action_status = true;
	
	cursorPosition pos; //creamos una estructura tipo cursorPosition. 
	pos=lcdGetCursorPosition (); //Se pide a la funcion que devuelva la posicion del cursor. 
	
	if (pos.row < DISPLAY_HIEGHT - 1) //Se fija si el cursor NO esta en la ultima fila.
	{
		pos.row++; //Si ese es el caso le suma 1 al valor de las filas. 
		action_status &= lcdSetCursorPosition(pos); ////Se actualiza el cursor del display con las nuevas coordenadas, aqui se modifica cadd.
	}
	else
	{
		std::cout<<"Cursor can not go down"<<std::endl; 
		action_status = false; //La accion no fue satisfactoria.
	}

	return action_status;

}

bool basicLCD:: lcdMoveCursorRight  ()
{
	// Avanza el cursor una posici�n.
	// Modifica cadd.

	bool action_status = true;

	cursorPosition pos; //creamos una estructura tipo cursorPosition. 
	pos=lcdGetCursorPosition (); //Se pide a la funcion que devuelva la posicion del cursor. 

	//Se deben contemplar 3 casos.
	// 1- Si el cursor no esta en la ultima posicion de cualquier fila.
	// 2- Si el cursor esta en la ultima posicion de una fila pero no la ultima.
	// 3- Si el cursor esta en la ultima posicion de la ultima fila.

	if (pos.column < (DISPLAY_LENGTH-1)) //Se fija si no esta en la ultima columna de cualquier fila. No puede ser 15.
		pos.column++;	//Si ese es el caso, se incrementa la columna en 1.

	else if((pos.column == (DISPLAY_LENGTH-1)) && (pos.row < DISPLAY_HIEGHT - 1)) // Se fija si esta en la ultima posicion del display (ultima fila, ultima columna)
	{
		pos.column = 0; //Se pone el cursor en la primera posicion de la proxima fila..	
		pos.row++;
	}
	
	else if((pos.column == (DISPLAY_LENGTH-1)) && (pos.row == (DISPLAY_HIEGHT - 1))) // Se fija si esta en la ultima posicion del display (ultima fila, ultima columna)
	{
		pos.column = 0; //Se pone el cursor en la primera posicion del display.	
		pos.row = 0;
	}

	else
	{
		action_status = false; //Si las coordenadas estan fuera de rango, hay error. 
		cout << "Cursor can not go right" << endl;
	}

	action_status &= lcdSetCursorPosition(pos); //Se actualiza el cursor del display con las nuevas coordenadas, aqui se modifica cadd.

	return action_status;
}

bool basicLCD:: lcdMoveCursorLeft ()
{
	// Retrocede el cursor una posici�n.
	// Modifica cadd.

	bool action_status = true;

	cursorPosition pos; //creamos una estructura tipo cursorPosition. 
	pos=lcdGetCursorPosition (); //Se pide a la funcion que devuelva la posicion del cursor. 

	//Se deben contemplar 3 casos.
	// 1- Si el cursor esta no esta en la primera posicion de cualquier fila.
	// 2- Si el cursor esta en la primera posicion de una fila pero la primera.
	// 3- Si el cursor esta en la primera posicion de la primera fila.


	if (pos.column > 0) //Se fija si no esta en la primera columna de cualquier fila.
		pos.column--; //Si ese es el caso se decrementa en uno la columna.

	else if ((pos.column == 0) && (pos.row != 0)) 
	{
		pos.column = DISPLAY_LENGTH - 1; //Se pone el cursor en la ultima posicion (15).
		pos.row--; //Se decrementa en uno la fila.
	}

	else if ((pos.column == 0) && (pos.row == 0)) 
	{
		pos.column = DISPLAY_LENGTH - 1; //Se pone el cursor en la ultima posicion de la ultima fila.
		pos.row = DISPLAY_HIEGHT - 1; 
	}

	else
	{
		action_status = false; //Si las coordenadas estan fuera de rango, hay error. 
		cout << "Cursor can not go left" << endl;
	}

	action_status &= lcdSetCursorPosition(pos); //Se actualiza el cursor del display con las nuevas coordenadas, aqui se modifica cadd.

	return action_status;
}

bool basicLCD:: lcdSetCursorPosition (const cursorPosition pos)
{
	//Posiciona el cursor en la posici�n dada por row y column. row[0-1] col[0-15]. Ante un valor inv�lido 
	//de row y/o column ignora la instrucci�n (no hace nada). Modifica (cadd). 

	bool action_status = true;
	
	//A continuacion se evalua si en la estructura enviada los valores de row y colum son validos.
	//0<row<2.
	//0<colum<15.

	if((pos.row>= 0) && (pos.row< DISPLAY_HIEGHT) && (pos.column>=0) && (pos.column< DISPLAY_LENGTH))
	{
		cadd=((DISPLAY_LENGTH*pos.row)+pos.column)+1; //Actualiza el cadd siempre sumando 1 mas que la posicion actual del display.
		lcdUpdateCursor(); //Se sincroniza cadd con el cursor del Display. 
	}
	else
	{
		action_status = false; //Si las coordenadas estan fuera de rango, hay error. 
		cout << "Invalid coords for display" << endl;
	}

	return action_status;
}

cursorPosition basicLCD:: lcdGetCursorPosition ()
{
	 // Devuelve la posici�n actual del cursor del display. 
	 // No altera cadd.
	 // Recordar que la posicion del cursor es cadd-1.

	cursorPosition pos; //creamos una estructura tipo cursorPosition. 

	unsigned int row;

	if((cadd-1) < DISPLAY_LENGTH)
		row = 0;
	else
		row = 1;

	pos.column = ((cadd-1)-(row*DISPLAY_LENGTH)); //Luego de saber cual esa la fila, obtenemos la columna. 
	pos.row=row;

	//cout << "CursorPosition-> ROW:" <<pos.row<<" COLUMN:"<<pos.column<<" CADD:"<<cadd<< endl; //Print para debug

	return pos;

}

void basicLCD::lcdUpdateCursor()
{
	//Posiciona el cursor del display en la posici�n dada por (cadd)-1. 
	//cadd No se altera.
	//Tener en cuenta que cadd-1 nunca puede ser 0.

	unsigned int row, column;

	if((cadd-1) < DISPLAY_LENGTH)
		row = 0;
	else
		row = 1;
	
	
	column = ((cadd-1)-(row*DISPLAY_LENGTH)); //Luego de saber cual esa la fila, obtenemos la columna. 

	lcdWriteIR(SET_ADDRESS (column,row)); //Se actualiza el cursor del display.
	//ATENCION: en el documento de Agustin dice que esta funcion es void, por lo que no hay control si la funcion lcdWriteIR
	//funciono correctamente. 

}



/*
void basicLCD::lcdWriteString(string str)
{
	//Utilizando la sobrecarga de operador, se envia un string al display.
	for (int i=0; i<str.length(); ++i) //Se calcula el largo del string.
		*this << str[i]; //Se envia caracter por caracter al display.
}*/

bool basicLCD::lcdInitOk()
{
	return constr_ok;
}

FT_STATUS basicLCD::lcdGetError()
{
	return lowLCD::status_display;
}

