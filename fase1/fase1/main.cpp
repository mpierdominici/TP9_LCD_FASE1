






//#define FTD_H
//#define FTD2XX_EXPORTS
//#include<ftd2xx.h>
//#include <thread>
//#include <chrono>
//DWORD byteSent = 0;
//int main()
//{
//	char tst = 0;
//	FT_HANDLE hand;
//	if (FT_Open(0, &hand) == FT_OK)
//	{
//		unsigned char Mask = 0xFF;
//		unsigned char Mode = 1;
//		if (FT_SetBitMode(hand, Mask, Mode) != FT_OK)
//		{
//			return false;
//		}
//		tst = 0x30;//eneable 0
//		FT_Write(hand, &tst, 1, &byteSent);
//		std::this_thread::sleep_for(std::chrono::milliseconds(1));
//		tst = 0x31;
//		FT_Write(hand, &tst, 1, &byteSent);
//		std::this_thread::sleep_for(std::chrono::milliseconds(1));
//		tst = 0x30;
//		FT_Write(hand, &tst, 1, &byteSent);
//		std::this_thread::sleep_for(std::chrono::milliseconds(5));
//
//		tst = 0x30;//eneable 0
//		FT_Write(hand, &tst, 1, &byteSent);
//		std::this_thread::sleep_for(std::chrono::milliseconds(1));
//		tst = 0x31;
//		FT_Write(hand, &tst, 1, &byteSent);
//		std::this_thread::sleep_for(std::chrono::milliseconds(1));
//		tst = 0x30;
//		FT_Write(hand, &tst, 1, &byteSent);
//		std::this_thread::sleep_for(std::chrono::milliseconds(1));
//
//		tst = 0x30;//eneable 0
//		FT_Write(hand, &tst, 1, &byteSent);
//		std::this_thread::sleep_for(std::chrono::milliseconds(1));
//		tst = 0x31;
//		FT_Write(hand, &tst, 1, &byteSent);
//		std::this_thread::sleep_for(std::chrono::milliseconds(1));
//		tst = 0x30;
//		FT_Write(hand, &tst, 1, &byteSent);
//		std::this_thread::sleep_for(std::chrono::milliseconds(1));
//
//
//		tst = 0x20;//eneable 0
//		FT_Write(hand, &tst, 1, &byteSent);
//		std::this_thread::sleep_for(std::chrono::milliseconds(1));
//		tst = 0x21;
//		FT_Write(hand, &tst, 1, &byteSent);
//		std::this_thread::sleep_for(std::chrono::milliseconds(1));
//		tst = 0x20;
//		FT_Write(hand, &tst, 1, &byteSent);
//		std::this_thread::sleep_for(std::chrono::milliseconds(1));
//
//		tst = 0x20;//eneable 0
//		FT_Write(hand, &tst, 1, &byteSent);
//		std::this_thread::sleep_for(std::chrono::milliseconds(1));
//		tst = 0x21;
//		FT_Write(hand, &tst, 1, &byteSent);
//		std::this_thread::sleep_for(std::chrono::milliseconds(1));
//		tst = 0x20;
//		FT_Write(hand, &tst, 1, &byteSent);
//		std::this_thread::sleep_for(std::chrono::milliseconds(1));
//
//		tst = 0x80;//eneable 0
//		FT_Write(hand, &tst, 1, &byteSent);
//		std::this_thread::sleep_for(std::chrono::milliseconds(1));
//		tst = 0x81;
//		FT_Write(hand, &tst, 1, &byteSent);
//		std::this_thread::sleep_for(std::chrono::milliseconds(1));
//		tst = 0x80;
//		FT_Write(hand, &tst, 1, &byteSent);
//		std::this_thread::sleep_for(std::chrono::milliseconds(1));
//	}
//
//
//}


















//
//
//#include"lcd.h"
//#include <iostream>
//int main(){
//	FT_HANDLE  * temp=initLCD();
//	if (temp != NULL)
//	{
//		if (lcdWriteByte(temp, 0x01, IR))
//		{
//			std::cout << "puedo escribir en el display" << std::endl;
//		}
//		if (lcdWriteByte(temp, 'a', DR))
//		{
//			std::cout << "puedo escribir en el display" << std::endl;
//		}
//	}
//	
//	return 0;
//}

#include <iostream>
#include <string>
#include "basicLCD.h"
#include "lowLCD.h"
#include "mySleep.h"


using namespace std;

int main(void)
{
	//Se continua a realizar pruebas sobre las funciones de la clase basicLCD.

	basicLCD display; //creamos objeto.

					  //string mens ("HELLO WORLD!!"); //Como se explica en el .h, esta funcion es para customizar y debe estar en private pero asi no se puede acceder desde aqui.
					  //display.lcdWriteString(mens);
	if (display.lcdInitOk() == false)
	{
		std::cout << "The display could not be initialized" << std::endl;
		display.lcdClear();
	}
	else // No se realizo control de error en cada una de las funciones por un tema de hacer mas amena la lectura de main pero se podria hacer con los returns values y lcdGetError
	{

		cout << "Lets send a message with the overload operator" << endl;
		display << 'I' << ' ' << 'L' << ' O' << 'V' << 'E' << ' ' << 'E' << 'D' << 'A';
		mySleep(1000);

		cout << "Lets clean the screen" << endl;
		display.lcdClear();
		mySleep(1000);

		cout << "Lets send a new message" << endl;
		display << 'L' << 'C' << 'D';
		mySleep(1000);

		cout << "Lets move the cursor down" << endl;
		display.lcdMoveCursorDown();
		mySleep(1000);

		cout << "And complete the message" << endl;
		display << 'D' << 'I' << 'S' << 'P' << 'L' << 'A' << 'Y';
		mySleep(1000);

		cout << "What happens if we want to go more below" << endl;
		display.lcdMoveCursorDown();
		mySleep(1000);

		cout << "Or up" << endl;
		display.lcdMoveCursorUp();
		display.lcdMoveCursorUp();
		mySleep(1000);

		cout << "Lets write all the display with 4's but going from right to left" << endl;
		for (int i = 0; i<DISPLAY_LENGTH * DISPLAY_HIEGHT; i++)
		{
			display.lcdWriteDR('4');
			display.lcdMoveCursorLeft();
			mySleep(DELAY);
		}
		mySleep(1000);

		cout << "Lets send the final message with the overload operator" << endl;
		display << 'B' << 'Y' << 'E' << ' ' << 'B' << 'Y' << 'E';
		mySleep(1000);

		cout << "But clean de line" << endl;
		display.lcdClearToEOL();
		mySleep(1000);

		cout << "Now the destructor will be called and will clean the screen" << endl;
		display.lcdClear();
		display << 'N' << 'O' << ' ' << 'M' << 'E' << ' ' << 'M' << 'A' << 'T' << 'E' << 'S' << ' ' << 'T' << 'E' << 'N' << 'G' << 'O' << ' ' << 'F' << 'A' << 'M' << 'I' << 'L' << 'I' << 'A' << ' ' << 'D' << ':';
		mySleep(1000);
		display.lcdClear();
		display << 'A' << 'H' << 'H' << 'H' << 'H' << 'H' << 'H' << 'H' << 'H' << 'H' << 'H' << 'H' << 'H' << 'H' << 'H' << 'H';


	}

	return 0;
}


//#include<cstdio>
//#include<windows.h>
//
//#define FTD2XX_EXPORTS
//#include "ftd2xx.h"
//
//int main()
//{
//
//	FT_HANDLE lcdHandle;
//	unsigned char info = 0x00;
//	DWORD sizeSent = 0;
//	bool found = false;
//
//	for (int i = 0; (i < 10) && !found; i++)
//	{
//		if (FT_Open(i, &lcdHandle) == FT_OK)	//Examples in FTDI guid use 0. But practice shows 1 is usually the case.
//		{
//			found = true;
//			UCHAR Mask = 0xFF;	//Selects all FTDI pins.
//			UCHAR Mode = 1; 	// Set asynchronous bit-bang mode
//			if (FT_SetBitMode(lcdHandle, Mask, Mode) == FT_OK)	// Sets LCD as asynch bit mode. Otherwise it doesn't work.
//			{
//
//				//Examplo to write 0xf0 to the display 
//
//				//(E=0, RS=0, D4-D7=f)...
//				info = 0xf0;
//
//				//Finally executes the action "write to LCD"...
//				if (FT_Write(lcdHandle, &info, 1, &sizeSent) == FT_OK)
//				{
//					printf("hola");	//If success continue with the program (...)
//
//				}
//				else
//					printf("Error writing to the LCD\n");
//			}
//			else
//				printf("Couldn't configure LCD\n");
//
//			FT_Close(lcdHandle);
//		}
//		else
//			printf("Couldn't open USB %d\n", i);
//	}
//
//	return 0;
//}
