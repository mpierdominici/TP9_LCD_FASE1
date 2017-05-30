#pragma once
#define FTD_H
#define FTD2XX_EXPORTS

#include<ftd2xx.h>

FT_HANDLE * initLCD(int iDevice);

FT_STATUS lcdWriteIR(FT_HANDLE * deviceHandler, BYTE valor);

FT_STATUS lcdWriteDR(FT_HANDLE * deviceHandler, BYTE valor);