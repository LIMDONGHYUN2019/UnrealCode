#include "Crypt.h"

const int32	g_iC1 = 52845;
const int32	g_iC2 = 22719;
const int32	g_iKey = 72957;

/*
0001 0001 1100 1111 1101
0000 0000 0000 1000 0000
0001 0001 1100 0111 1101
0001 0001 1100
*/

bool Crypt::EnCrypt(char* pSrc, char* pDest, int32 iLength)
{
	if (!pSrc || !pDest || iLength <= 0)
		return false;

	int32		iKey = g_iKey;

	for (int32 i = 0; i < iLength; ++i)
	{
		pDest[i] = pSrc[i] ^ iKey >> 8;
		iKey = (pDest[i] + iKey) * g_iC1 + g_iC2;
	}

	return true;
}

bool Crypt::DeCrypt(char* pSrc, char* pDest, int32 iLength)
{
	if (!pSrc || !pDest || iLength <= 0)
		return false;

	int32	iKey = g_iKey;
	char	cPrevBlock;

	for (int32 i = 0; i < iLength; ++i)
	{
		cPrevBlock = pSrc[i];
		pDest[i] = pSrc[i] ^ iKey >> 8;
		iKey = (cPrevBlock + iKey) * g_iC1 + g_iC2;
	}

	return true;
}
