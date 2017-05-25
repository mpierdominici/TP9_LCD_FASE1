
#define FTD2XX_STATIC 
//falta el define para compilacion statica
#include <iostream>
extern "C"
{
	#include<ftd2xx.h>
}

#include <thread>
#include <chrono>
#define SIZE_BUFFER 1
#define IR 0X00
#define DR 0X02
#define ENABLE_BIT 0X01
#define DEVICE_NUMBER 1
bool lcdWriteNibble(FT_HANDLE * h, BYTE value, BYTE rs);
FT_HANDLE * initLCD()
{
	FT_HANDLE * temp = NULL;
	temp = new FT_HANDLE;
	if (FT_Open(DEVICE_NUMBER, temp) == FT_OK)
	{

	}

	return NULL;
}

bool lcdWriteNibble(FT_HANDLE * h, BYTE value,BYTE rs)
{
	LPDWORD byteSent = 0;
	char buffer[SIZE_BUFFER];
	
	buffer[0] = ((value < 4) & 0xF0);//guardo en el buffer en su parte alta el nibble enviado a traves de value, y borro el resto del nible

	//si rs no es un valor aceptado devuelvo error
	if (rs != IR || rs != DR)
	{
		return false;
	}

	buffer[0] |= rs;//pongo en modo correspondiente el bit rs;


	if (FT_Write(h, buffer, SIZE_BUFFER, byteSent) == FT_OK)//enable en 0
	{
		
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		buffer[0] |= ENABLE_BIT;//enable en 1
		if (FT_Write(h, buffer, SIZE_BUFFER, byteSent) == FT_OK)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			buffer[0] &= (~ENABLE_BIT);//enable en 0
			if (FT_Write(h, buffer, SIZE_BUFFER, byteSent) == FT_OK)
			{
				if (FT_Write(h, buffer, SIZE_BUFFER, byteSent) == FT_OK)
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(1));
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}

	}
	else
	{
		return false;
	}

}