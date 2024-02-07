#pragma once

#include "GameInfo.h"

class Crypt
{
public:
	static bool EnCrypt(char* pSrc, char* pDest, int32 iLength);
	static bool DeCrypt(char* pSrc, char* pDest, int32 iLength);
};

