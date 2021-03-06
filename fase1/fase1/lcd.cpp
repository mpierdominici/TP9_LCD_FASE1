

//falta el define para compilacion statica
#include <iostream>
#include "lcd.h"


#include <thread>
#include <chrono>
#define SIZE_BUFFER 1

#define ENABLE_BIT 0X01
#define DEVICE_NUMBER 0

#define IR 0X00
#define DR 0X02

FT_STATUS lcdWriteNibble(FT_HANDLE * h, BYTE value, BYTE rs);
FT_STATUS lcdWriteByte(FT_HANDLE * h, BYTE value, BYTE rs);


FT_HANDLE * initLCD(int iDevice)
{
	FT_HANDLE * temp = NULL;
	temp = new FT_HANDLE;
	
	if (FT_Open(DEVICE_NUMBER, temp) == FT_OK)
	{
		unsigned char Mask = 0xFF;
		unsigned char Mode = 1;
		if (FT_SetBitMode(*temp, Mask, Mode) != FT_OK)
		{
			return false;
		}
		if (lcdWriteNibble(temp, 0x03, IR)==FT_OK)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(4));
			
			if (lcdWriteNibble(temp, 0x03, IR)==FT_OK)
			{
				std::this_thread::sleep_for(std::chrono::microseconds(110));
				if (lcdWriteNibble(temp, 0x03, IR) == FT_OK)
				{
					if (lcdWriteNibble(temp, 0x02, IR) == FT_OK)//modo 4 bits;
					{
						if (lcdWriteByte(temp, 0x28, IR) == FT_OK)
						{
							if (lcdWriteByte(temp, 0x08, IR) == FT_OK)
							{
								if (lcdWriteByte(temp, 0x01, IR) == FT_OK)
								{
									if (lcdWriteByte(temp, 0x06, IR) == FT_OK)
									{
										return temp;
									}

								}


							}

						}

					}

				}
			}
		}

	}
	
		delete temp;
		temp = NULL;
	
	return temp;
}

FT_STATUS lcdWriteNibble(FT_HANDLE * h, BYTE value,BYTE rs)
{
	FT_STATUS temp = FT_OK;
	DWORD byteSent = 0;
	char buffer[SIZE_BUFFER];
	
	buffer[0] = ((value << 4) & 0xF0);//guardo en el buffer en su parte alta el nibble enviado a traves de value, y borro el resto del nible

	//si rs no es un valor aceptado devuelvo error
	if (rs != IR && rs != DR)
	{
		return false;
	}

	buffer[0] |= rs;//pongo en modo correspondiente el bit rs;


	if ((temp = FT_Write(*h, buffer, SIZE_BUFFER, &byteSent)) == FT_OK)//enable en 0
	{

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		buffer[0] |= ENABLE_BIT;//enable en 1
		if ((temp = FT_Write(*h, buffer, SIZE_BUFFER, &byteSent)) == FT_OK)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			buffer[0] &= (~ENABLE_BIT);//enable en 0
			if ((temp = FT_Write(*h, buffer, SIZE_BUFFER, &byteSent)) == FT_OK)
			{

				std::this_thread::sleep_for(std::chrono::milliseconds(1));


			}
		}
	}
	return temp;
		

}

FT_STATUS lcdWriteByte(FT_HANDLE * h, BYTE value, BYTE rs)
{
	FT_STATUS temp = FT_OK;
	temp = lcdWriteNibble(h, value >> 4, rs);
	if (temp != FT_OK)
	{
		temp = lcdWriteNibble(h, value, rs);
	}

	return temp;
}

FT_STATUS lcdWriteIR(FT_HANDLE * deviceHandler, BYTE valor)
{
	return lcdWriteByte(deviceHandler, valor, IR);
}

FT_STATUS lcdWriteDR(FT_HANDLE * deviceHandler, BYTE valor)
{
	return lcdWriteByte(deviceHandler, valor, DR);
}

