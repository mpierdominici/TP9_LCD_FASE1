#ifndef BASICLCD_H
#define BASICLCD_H

#include "lowLCD.h"
#define FTD_H
#define FTD2XX_EXPORTS

#include "ftd2xx.h"

//Este archivo junto con su respectivo .cpp, corresponden a la FASE II. Aqui se encuentran todas las funciones de la capa media. 

using namespace std;

typedef struct 
{
	int row;
	int column;	

} cursorPosition ;


class basicLCD : public lowLCD
{
public:
	basicLCD();
	~basicLCD();

	bool lcdInitOk(); //HECHA
	FT_STATUS lcdGetError(); //Hecha pero falta cambiar las demas funciones para que actualizen status_display 
	
	bool lcdClear (); // Borra el display y el cursor va a HOME.
	bool lcdClearToEOL(); //Borra el display desde la posici�n actual del cursor hasta el final de la l�nea. 
	basicLCD& operator<< (const unsigned char c);  //Envia un caracter al display.

	bool lcdMoveCursorUp (); //Pasa el cursor a la primera l�nea del display sin alterar la columna en la que estaba. 
	bool lcdMoveCursorDown  (); // Pasa el cursor a la segunda l�nea del display sin alterar la columna en la que estaba.
	bool lcdMoveCursorRight  (); // Avanza el cursor una posici�n. 
	bool lcdMoveCursorLeft (); //  Retrocede el cursor una posici�n 
	
	bool lcdSetCursorPosition(const cursorPosition pos); // Posiciona el cursor en la posici�n dada por row y column.
	cursorPosition lcdGetCursorPosition();  //Devuelve la posici�n actual del cursor. 
	
	//void lcdWriteString(string str); //ATENCION: Esta funcion es para customizar, segun Agustin debe ir en private. ES SOLO PARA PROBAR! (al final la comentamos)

private: 
	int cadd; //La direcci�n actual del cursor en el display + 1.
	bool constr_ok;
	void lcdUpdateCursor(); //Posiciona el cursor del display en la posici�ndada por (cadd)-1.
	basicLCD(basicLCD &o);//no queremos que puedan copiar el display 
	basicLCD& operator= (const basicLCD& other); //Tampoco el operador =
};

#define DISPLAY_LENGTH 16
#define DISPLAY_HIEGHT 2
#define ASCII_SPACE 0x20 //Se utiliza en la funcion lcdClearToEOL().
#define DELAY 40 //Delay entre la escritura de cada caracter.

#endif //BASICLCD_H