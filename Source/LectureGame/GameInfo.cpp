
#include "GameInfo.h"
#include <codecvt>
#include <string>
#include <vector>
#include <locale>

DEFINE_LOG_CATEGORY(LDH);

void PrintViewport(float fTime, const FColor& color, const FString& strText)
{
	GEngine->AddOnScreenDebugMessage(-1, fTime, color, strText);
}

void ConvertString(char* pResult, const FString& str)
{
	std::wstring	strConvert = *str;
	typedef std::codecvt<wchar_t, char, std::mbstate_t>	codecvt_t;

	std::locale	loc = std::locale("Korean");	// Window용
	codecvt_t const& cvt = std::use_facet<codecvt_t>(loc);
	std::mbstate_t	state = std::mbstate_t();

	std::vector<char>	buf((strConvert.size() + 1) * cvt.max_length());

	wchar_t const* in_next = strConvert.c_str();

	char* out_next = &buf[0];

	std::codecvt_base::result	r = cvt.out(state, strConvert.c_str(), strConvert.c_str() + strConvert.size(),
	in_next, &buf[0], &buf[0] + buf.size(), out_next);

	if (r == std::codecvt_base::error)
	{
		PrintViewport(1.f, FColor::Red, TEXT("String Convert Error"));
		return;
	}

	std::string	strResult(&buf[0]);

	if (strResult.length() > 0)
		memcpy(pResult, strResult.c_str(), strResult.length());
		//strcpy_s(pResult, strResult.length(), strResult.c_str());
}

void ConvertString(FString& strResult, const char* pStr)
{
	strResult = UTF8_TO_TCHAR(pStr);
}
