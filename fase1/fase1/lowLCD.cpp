#include <iostream>
#include "lowLCD.h"
#include "mySleep.h"


using namespace std;

lowLCD::lowLCD()
{
	//Constructor.
	//Seguimos los pasos como en el archivo que nos envio 
	status_display=FT_OK;
	if ( (status_display=FT_Open(0, &display_handle)) == FT_OK)
	{
		unsigned char Mask = 0xFF;
		unsigned char Mode = 1; 
		if (FT_SetBitMode (display_handle, Mask, Mode) != FT_OK)
			cout << "Couldn't configure LCD" << endl;
	}
	else 
		cout << "Couldn't open LCD" << endl;
}


lowLCD::~lowLCD()
{
	FT_Close (display_handle);
}

//No hay control del tipo de error ya que directamente devuelve true or false y el InitOk devuelve ese mismo bool.
bool lowLCD:: lcdInit()
{
	//Secuencia que inicia el display en modo de 4 bits como dice en el documento.
	bool progr_status = true; //FT_Ok es ==0 entonces si se suma deberia quedarse igual

	progr_status &= lcdWriteNibble (MSN(FUNCTION_SET) | MODE_8BIT); //Envia el nibble alto de �function set� con el modo en 8 bits. 
	mySleep(5); //Sleep de 4ms 
	progr_status &= lcdWriteNibble (MSN(FUNCTION_SET) | MODE_8BIT); //Envia el nibble alto de �function set� con el modo en 8 bits. 
	mySleep(1);//Sleep de 1ms (deberia ser 0,1ms pero no genera diferencia)
	progr_status &= lcdWriteNibble (MSN(FUNCTION_SET) | MODE_8BIT); //Envia el nibble alto de �function set� con el modo en 8 bits. 
	progr_status &= lcdWriteNibble (MSN(FUNCTION_SET) | MODE_4BIT); //Envia el nibble alto de �function set� con el modo en 8 bits. 
	progr_status &= lcdWriteIR (FUNCTION_SET | MODE_4BIT | DSP_LINES_TWO | FONT_5X8 ); //Envia la instruccion �function set� con el modo en 4 bits.
	progr_status &= lcdWriteIR (DISPLAY_ON_OFF_CONTROL ); //Envia la instrucci�n  �display on/off control� con el modo en 4 bits.
	progr_status &= lcdWriteIR (CLEAR_SCREEN ); //Envia la instrucci�n  �clear screen� con el modo en 4 bits.
	progr_status &= lcdWriteIR (ENTRY_MODE_SET); //Envia la instrucci�n  �mode set entry� con el modo en 4 bits.

	return progr_status;
}



bool lowLCD::lcdWriteByte (unsigned char value,unsigned char rs)
{
	//Devuelve un 0 si se produjo un error en alguna de los dos por eso el and bitwise ambas devuelven 1 si estuvo todo ok 
	// ademas corta la ejecucion si el primero genero error por el lazy del && (cosa que no se produce bitwise)
	return ( lcdWriteNibble (MSN (value) | rs) && lcdWriteNibble (LSN (value) | rs) ); 
}


bool lowLCD::lcdWriteIR (unsigned char value)
{
	//Habilita el envio de instrucciones, apaga el RS.
	return lcdWriteByte (value, LCD_RS_OFF);//Si todo esta ok, devuelve un 1
}


//Version actualizada con control de error
bool lowLCD:: lcdWriteNibble (unsigned char value) 
{
	//Envia un nibble al puerto.
	bool prog_status = false;//true si estuvo todo 10 puntitos
	DWORD sizeSent = 0;
	unsigned char info = LCD_RS;
	
	if( (status_display = FT_Write (display_handle, &info, 1, &sizeSent))==FT_OK)
	{
		info = value;
		if( (status_display = FT_Write (display_handle, &info, 1, &sizeSent))==FT_OK)
		{
			mySleep (1);
			info = value | LCD_E;
			if( (status_display = FT_Write (display_handle, &info, 1, &sizeSent))==FT_OK)
			{
				mySleep (1);
				info = value;
				if( (status_display = FT_Write (display_handle, &info, 1, &sizeSent))==FT_OK)
				{
					mySleep (1);
					prog_status=true;
				}
			}
		}
	}

	return prog_status;
}


bool lowLCD:: lcdWriteDR (unsigned char value)
{
	//Habilita el envio de caracteres, prende el RS. 
	return lcdWriteByte (value, LCD_RS_ON); //Pone el RS en 1. Si todo esta ok, devuelve un 1.
}