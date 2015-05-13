/************************************************************************
 *
 *	Copyright (c) 2007-2012, Samuel W. Choi
 *	All Rights Reserved
 *
 *	$Workfile: DcmString.cpp $
 *	$Author: Samuelchoi $
 *	$Date: 08-05-19 4:28p $
 *	$Revision: 2 $
 *
 ************************************************************************/

#include "DTK.h"
#include "DTKinternal.h"

#include <stdarg.h>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define	LF								((Uint8)'\x0A')
#define	FF								((Uint8)'\x0C')
#define	CR								((Uint8)'\x0D')
#define	ESC								((Uint8)'\x1B')

const char* _ISO_IR[DTK_NLS_COUNT] =
{
	"ISO_IR 6",			// 0: Default
	"ISO_IR 100",		// 1: Latin1
	"ISO_IR 101",		// 2: Latin2
	"ISO_IR 109",		// 3: Latin3
	"ISO_IR 110",		// 4: Latin4
	"ISO_IR 148",		// 5: Latin5
	"ISO_IR 144",		// 6: Cyrillic
	"ISO_IR 127",		// 7: Arabic
	"ISO_IR 126",		// 8: Greek
	"ISO_IR 138",		// 9: Hebrew
	"ISO_IR 166",		// 10: Thai
	"ISO_IR 149",		// 11: Korean
	"ISO_IR 13",		// 12: Japanese
	"ISO_IR 14",		// 13: Japanese
	"ISO_IR 87",		// 14: Japanese
	"ISO_IR 159",		// 15: Japanese
	"ISO_IR 192",		// 16: Unicode UTF8
};

const char* _ISO_2022[DTK_NLS_COUNT] =
{
	"ISO 2022 IR 6",	// 0: Default
	"ISO 2022 IR 100",	// 1: Latin1
	"ISO 2022 IR 101",	// 2: Latin2
	"ISO 2022 IR 109",	// 3: Latin3
	"ISO 2022 IR 110",	// 4: Latin4
	"ISO 2022 IR 148",	// 5: Latin5
	"ISO 2022 IR 144",	// 6: Cyrillic
	"ISO 2022 IR 127",	// 7: Arabic
	"ISO 2022 IR 126",	// 8: Greek
	"ISO 2022 IR 138",	// 9: Hebrew
	"ISO 2022 IR 166",	// 10: Thai
	"ISO 2022 IR 149",	// 11: Korean
	"ISO 2022 IR 13",	// 12: Japanese
	"ISO 2022 IR 14",	// 13: Japanese
	"ISO 2022 IR 87",	// 14: Japanese
	"ISO 2022 IR 159",	// 15: Japanese
	"ISO 2022 IR 192",	// 16: Unicode UTF8
};

const char* _ISO_2022_ESC[DTK_NLS_COUNT] =
{
	"\x1B\x28\x42",		//  0: ISO 2022 IR 6: ESC ( B
	"\x1B\x2D\x41",		//  1: ISO 2022 IR 100: ESC - A
	"\x1B\x2D\x42",		//  2: ISO 2022 IR 101: ESC - B
	"\x1B\x2D\x43",		//  3: ISO 2022 IR 109: ESC - C
	"\x1B\x2D\x44",		//  4: ISO 2022 IR 110: ESC - D
	"\x1B\x2D\x4D",		//  5: ISO 2022 IR 148: ESC - M
	"\x1B\x2D\x4C",		//  6: ISO 2022 IR 144: ESC - L
	"\x1B\x2D\x47",		//  7: ISO 2022 IR 127: ESC - G
	"\x1B\x2D\x46",		//  8: ISO 2022 IR 126: ESC - F
	"\x1B\x2D\x48",		//  9: ISO 2022 IR 138: ESC - H
	"\x1B\x2D\x54",		// 10: ISO 2022 IR 166: ESC -
	"\x1B\x24\x29\x43",	// 11: ISO 2022 IR 149: ESC $ ) C
	"\x1B\x29\x49",		// 12: ISO 2022 IR 13: ESC ) I
	"\x1B\x28\x4A",		// 13: ISO 2022 IR 14: ESC ( J
	"\x1B\x24\x42",		// 14: ISO 2022 IR 87: ESC $ B
	"\x1B\x24\x28\x44",	// 15: ISO 2022 IR 159: ESC $ ( D
	"",					// 16: None for Unicode UTF8
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef	XXX
String::String(void)
{
}

String::String(const char ch)
{
	_s = ch;
//	*this = ch;
}

String::String(const char* psz)
//	: std::string(psz)
{
	_s = psz;
}

String::String(const std::string& str)
//	: std::string(str.c_str())
{
	_s = str;
}

String::String(const OFString& ostr)
//	: std::string(ostr.c_str())
{
	_s = ostr.c_str();
}

#ifdef	WIN32xx
CDcmString::CDcmString(const CString& cstr)
{
	USES_CONVERSION;
	*this = CT2A(cstr);
}
#endif

const String& String::operator=(char ch)
{
	_s.assign(1, ch);
	return *this;
}

const String& String::operator=(const char* psz)
{
	_s.assign(psz);
	return *this;
}

const String& String::operator=(const std::string& str)
{
	_s.assign(str.c_str());
	return *this;
}

const String& String::operator=(const OFString& ostr)
{
	_s.assign(ostr.c_str());
	return *this;
}

#ifdef	WIN32xx
const CDcmString& CDcmString::operator=(const CString& cstr)
{
	USES_CONVERSION;
	assign(CT2A(cstr));
	return *this;
}
#endif

String::operator const char*() const
{
	return _s.c_str();
}

String::operator const OFString() const
{
	return _s.c_str();
}

const char* String::toConstData() const
{
	return _s.c_str();
}

String String::format(const String format, ...)
{
	// The following code were borrowed from
	//	http://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf

	int final_n, n = ((int)format.size()) * 2; /* Reserve two times as much as the length of the fmt_str */
	std::unique_ptr<char[]> formatted;
	va_list ap;
	while(1) {
		formatted.reset(new char[n]); /* Wrap the plain char array into the unique_ptr */
		strcpy(&formatted[0], format);
		va_start(ap, format);
		final_n = vsnprintf(&formatted[0], n, format, ap);
		va_end(ap);
		if (final_n < 0 || final_n >= n)
			n += abs(final_n - n + 1);
		else
			break;
	}
	return std::string(formatted.get());
}

size_t String::size() const
{
	return _s.size();
}

bool String::isEmpty() const
{
	return _s.empty();
}

int String::operator==(const String& s) const
{
	return _s.compare(s) == 0;
}

int String::operator!=(const String& s) const
{
	return _s.compare(s) != 0;
}

int String::compare(const String& s) const
{
	return _s.compare(s);
}

String String::substr(size_t a, size_t b) const
{
	return _s.substr(a, b);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

String StringEncoder::encode(int nls)
{
	return StringEncoder::encode(*this, nls);
}

String StringEncoder::decode(int nls)
{
	return StringEncoder::decode(*this, nls);
}
#endif
String NLS::encode(const String& str, int nls)
{
	String ostr = str;
#ifdef	XXX
	///////////////////////////////////////////////////////////////////////////////////////////
	//	Unicode UTF-8
	///////////////////////////////////////////////////////////////////////////////////////////

	if (nls == DTK_NLS_ISO_IR_192) {
		/*	FIXME
		int nMBlen = str.length();
		const char* pMBstr = str;
		wchar_t* pWCstr = new wchar_t[nMBlen+1];
		char* pUTF8str = new char[nMBlen*4+1];
		int nWClen = MultiByteToWideChar(CP_OEMCP, 0, pMBstr, nMBlen, pWCstr, nMBlen);
		int nUTF8len = WideCharToMultiByte(CP_UTF8, 0, pWCstr, nWClen, pUTF8str, nWClen*4, NULL, NULL);
		pUTF8str[nUTF8len] = 0;
		ostr = pUTF8str;
		delete pUTF8str;
		delete pWCstr;
		*/
		return ostr;
	}

	///////////////////////////////////////////////////////////////////////////////////////////
	//	Single Byte Character Set
	///////////////////////////////////////////////////////////////////////////////////////////

	switch(nls) {
	case DTK_NLS_ISO_IR_6 :
	case DTK_NLS_ISO_IR_100 :
	case DTK_NLS_ISO_IR_101 :
	case DTK_NLS_ISO_IR_109 :
	case DTK_NLS_ISO_IR_110 :
	case DTK_NLS_ISO_IR_148 :
	case DTK_NLS_ISO_IR_144 :
	case DTK_NLS_ISO_IR_127 :
	case DTK_NLS_ISO_IR_126 :
	case DTK_NLS_ISO_IR_138 :
	case DTK_NLS_ISO_IR_166 :
	case DTK_NLS_ISO_IR_13 :
	case DTK_NLS_ISO_IR_14 :
		ostr = str;
		return ostr;
	}

	///////////////////////////////////////////////////////////////////////////////////////////
	//	Multi Byte Character Set
	///////////////////////////////////////////////////////////////////////////////////////////

	u_int nls1 = (nls >> 0) & 0xFF;
	u_int nls2 = (nls >> 8) & 0xFF;
	u_int nls3 = (nls >> 16) & 0xFF;
	u_int currentNLS = nls1;

	if (((nls1 == DTK_NLS_ISO_IR_6 || nls1 == DTK_NLS_ISO_IR_13) && nls1 != DTK_NLS_ISO_IR_87) &&
		((nls2 == DTK_NLS_ISO_IR_13 || nls2 == DTK_NLS_ISO_IR_87) && nls2 != DTK_NLS_ISO_IR_6) &&
		(nls3 == DTK_NLS_ISO_IR_13 || nls3 == DTK_NLS_ISO_IR_87 || nls3 == DTK_NLS_ISO_IR_6)) {
		//	Start of Japanese
		const Uint8* inPtr = (Uint8*)(const char*)str;
		while(*inPtr) {
			if (*inPtr == LF || *inPtr == FF || *inPtr == CR || (*inPtr >= 0x20 && *inPtr <= 0x7F)) {
				if (nls1 == DTK_NLS_ISO_IR_6 && currentNLS != DTK_NLS_ISO_IR_6) {
					ostr += _ISO_2022_ESC[DTK_NLS_ISO_IR_6];
					currentNLS = DTK_NLS_ISO_IR_6;
				} else if (nls1 == DTK_NLS_ISO_IR_13 && currentNLS != DTK_NLS_ISO_IR_13 && currentNLS != DTK_NLS_ISO_IR_14) {
					ostr += _ISO_2022_ESC[DTK_NLS_ISO_IR_14];
					currentNLS = DTK_NLS_ISO_IR_14;
				}
				ostr += (*inPtr++);
				continue;
			}
			if (*inPtr == 0xA0) {
				if (nls1 == DTK_NLS_ISO_IR_13 || nls2 == DTK_NLS_ISO_IR_13 || nls3 == DTK_NLS_ISO_IR_13 ||
					nls2 == DTK_NLS_ISO_IR_87 || nls3 == DTK_NLS_ISO_IR_87) {
					ostr += (*inPtr++ & 0x7F);
					continue;
				}
			}
			if (*inPtr >= 0xA1 && *inPtr <= 0xDF) {
				if (nls1 == DTK_NLS_ISO_IR_13) {
					currentNLS = DTK_NLS_ISO_IR_13;
					ostr += *inPtr++;
					continue;
				} else if (nls2 == DTK_NLS_ISO_IR_13 || nls3 == DTK_NLS_ISO_IR_13) {
					if (currentNLS != DTK_NLS_ISO_IR_13) {
						ostr += _ISO_2022_ESC[DTK_NLS_ISO_IR_13];
						currentNLS = DTK_NLS_ISO_IR_13;
					}
					ostr += *inPtr++;
					continue;
				}
			}
			if ((*inPtr >= 0x81 && *inPtr <= 0x9F) || (*inPtr >= 0xE0 && *inPtr <= 0xEF)) {
				if (nls2 == DTK_NLS_ISO_IR_87 || nls3 == DTK_NLS_ISO_IR_87) {
					if (currentNLS != DTK_NLS_ISO_IR_87) {
						ostr += _ISO_2022_ESC[DTK_NLS_ISO_IR_87];
						currentNLS = DTK_NLS_ISO_IR_87;
					}
					Uint8 cHB = *inPtr++;
					Uint8 cLB = *inPtr++;
					int nA = cLB < 0x9F;
					int nRow = cHB < 0xA0 ? 0x70 : 0xB0;
					int nCol = nA ? (cLB > 0x7F ? 0x20 : 0x1F) : 0x7E;
					ostr += ((((cHB - nRow) << 1) - nA + 0x80) & 0x7F);
					ostr += ((cLB - nCol + 0x80) & 0x7F);
					continue;
				}
			} else {
				inPtr++;
			}
		}
		if (nls1 == DTK_NLS_ISO_IR_6 && currentNLS != DTK_NLS_ISO_IR_6) {
			ostr += _ISO_2022_ESC[DTK_NLS_ISO_IR_6];
			currentNLS = DTK_NLS_ISO_IR_6;
		} else if (nls1 == DTK_NLS_ISO_IR_13 && currentNLS != DTK_NLS_ISO_IR_13 && currentNLS != DTK_NLS_ISO_IR_14) {
			ostr += _ISO_2022_ESC[DTK_NLS_ISO_IR_14];
			currentNLS = DTK_NLS_ISO_IR_14;
		}
		//	End of Japanese
	} else if (nls1 == DTK_NLS_ISO_IR_6 && nls2 == DTK_NLS_ISO_IR_149) {
		//	Start of Korean
		const Uint8* inPtr = (Uint8*)(const char*)str;
		while(*inPtr) {
			if (*inPtr == LF || *inPtr == FF || *inPtr == CR || (*inPtr >= 0x20 && *inPtr <= 0x7F)) {
				currentNLS = DTK_NLS_ISO_IR_6;
				ostr += (*inPtr++);
				continue;
			}
			if (*inPtr >= 0xA0 && *inPtr <= 0xFF) {
				if (currentNLS != DTK_NLS_ISO_IR_149) {
					ostr += _ISO_2022_ESC[DTK_NLS_ISO_IR_149];
					currentNLS = DTK_NLS_ISO_IR_149;
				}
				ostr += (*inPtr++);
				ostr += (*inPtr++);
				continue;
			}
			else
			{
				inPtr++;
			}
		}
		//	End of Korean
	}
#endif
	return ostr;
}

String NLS::decode(const String& str, int nls)
{
	String ostr = str;
#ifdef	XXX
	///////////////////////////////////////////////////////////////////////////////////////////
	//	Unicode UTF-8
	///////////////////////////////////////////////////////////////////////////////////////////

	if (nls == DTK_NLS_ISO_IR_192)
	{
		/*	FIXME
		const char* pUTF8str = str;
		int nUTF8len = (int)str.length();
		wchar_t* pWCstr = new wchar_t[nUTF8len+1];
		char* pMBstr = new char[nUTF8len*2+1];
		int nWClen = MultiByteToWideChar(CP_UTF8, 0, pUTF8str, nUTF8len, pWCstr, nUTF8len);
		int nMBlen = WideCharToMultiByte(CP_OEMCP, 0, pWCstr, nWClen, pMBstr, nWClen*2, NULL, NULL);
		pMBstr[nMBlen] = 0;
		ostr = pMBstr;
		delete[] pMBstr;
		delete[] pWCstr;
		*/
		return ostr;
	}

	///////////////////////////////////////////////////////////////////////////////////////////
	//	Single & Multi Byte Character Set
	///////////////////////////////////////////////////////////////////////////////////////////

	const Uint8* inPtr = (const Uint8*)(const char*)str;
	const Uint8* inEndPtr = inPtr + str.length();
	u_int currentNLS = DTK_NLS_ISO_IR_6;
	char mb[3];
	char* mbPtr = mb;
	Uint8 hb, lb;

	while(*inPtr && inPtr < inEndPtr) {
		if (*inPtr == ESC) {
			for(int nIndex = 0; nIndex < DTK_NLS_COUNT; nIndex++) {
				if (memcmp(inPtr, _ISO_2022_ESC[nIndex], strlen(_ISO_2022_ESC[nIndex])) == 0) {
					currentNLS = nIndex;
					break;
				}
			}
			while(*inPtr) {
				if (*inPtr++ >= 0x40)
					break;
			}
		} else {
			switch(currentNLS) {
			case DTK_NLS_ISO_IR_6 :
			case DTK_NLS_ISO_IR_100 :
			case DTK_NLS_ISO_IR_101 :
			case DTK_NLS_ISO_IR_109 :
			case DTK_NLS_ISO_IR_110 :
			case DTK_NLS_ISO_IR_148 :
			case DTK_NLS_ISO_IR_144 :
			case DTK_NLS_ISO_IR_127 :
			case DTK_NLS_ISO_IR_126 :
			case DTK_NLS_ISO_IR_138 :
			case DTK_NLS_ISO_IR_166 :
			default :
				mbPtr[0] = (*inPtr++);
				mbPtr[1] = 0;
				ostr += mb;
				break;
			case DTK_NLS_ISO_IR_149 :		// KS X 1001 at G1
				if (*inPtr < 0xA0) {
					currentNLS = DTK_NLS_ISO_IR_6;
					continue;
				}
				hb = (*inPtr++);
				lb = (*inPtr++);
				if (lb) {
					mbPtr[0] = hb;
					mbPtr[1] = lb;
					mbPtr[2] = 0;
				} else {
					mbPtr[0] = hb;
					mbPtr[1] = 0;
				}
				ostr += mb;
				break;
			case DTK_NLS_ISO_IR_13 :		// JIS X 0201 at G1
			case DTK_NLS_ISO_IR_14 :		// JIS X 0201 at G0
				mbPtr[0] = (*inPtr++);
				mbPtr[1] = 0;
				ostr += mb;
				break;
			case DTK_NLS_ISO_IR_87 :		// JIS X 0208 at G0
				hb = *inPtr++;
				lb = *inPtr++;
				if (lb) {
					int nRow = hb < 0x5F ? 0x70 : 0xB0;
					int nCol = hb % 2 ? (lb > 0x5F ? 0x20 : 0x1F) : 0x7E;
					mbPtr[0] = ((hb + 1) >> 1) + nRow;
					mbPtr[1] = (lb + nCol);
					mbPtr[2] = 0;
				} else {
					mbPtr[0] = hb;
					mbPtr[1] = 0;
				}
				ostr += mb;
				break;
			case DTK_NLS_ISO_IR_159 :		// JIS X 0212 at G0
				//	FIXME: to be implemented
				break;
			}
		}
	}
#endif
	return ostr;
}
