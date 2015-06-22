/*
 *
 *  Copyright (C) 2015, IRM Inc.
 *  All rights reserved.  See LICENSE file for details.
 *
 *  This software and supporting documentation were developed by
 *		IRM Inc., Korea.
 *  through the contract with
 *		Seoul National University Bundang Hospital
 *  under the support of
 *		Ministry of Trade, Industry and Energy, Republic of Korea.
 *
 *  Author:  Samuel Choi (samuelchoi@irm.kr)
 *
 */

#include "DTK.h"
#include "DTKinternal.h"

#include <QTextCodec>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define	LF								((Uint8)'\x0A')
#define	FF								((Uint8)'\x0C')
#define	CR								((Uint8)'\x0D')
#define	ESC								((Uint8)'\x1B')

const NLS::CharacterSet NLS::_characterSets[] =
{
	{
		// 0: Default
		"ISO_IR 6",
		"ISO 2022 IR 6",
		"\x1B\x28\x42",		// ISO 2022 IR 6: ESC ( B
	},
	{
		// 1: Latin1
		"ISO_IR 100",
		"ISO 2022 IR 100",
		"\x1B\x2D\x41",		// ISO 2022 IR 100: ESC - A
	},
	{
		// 2: Latin2
		"ISO_IR 101",
		"ISO 2022 IR 101",
		"\x1B\x2D\x42",		// ISO 2022 IR 101: ESC - B
	},
	{
		// 3: Latin3
		"ISO_IR 109",
		"ISO 2022 IR 109",
		"\x1B\x2D\x43",		// ISO 2022 IR 109: ESC - C
	},
	{
		// 4: Latin4
		"ISO_IR 110",
		"ISO 2022 IR 110",
		"\x1B\x2D\x44",		// ISO 2022 IR 110: ESC - D
	},
	{
		// 5: Latin5
		"ISO_IR 148",
		"ISO 2022 IR 148",
		"\x1B\x2D\x4D",		// ISO 2022 IR 148: ESC - M
	},
	{
		// 6: Cyrillic
		"ISO_IR 144",
		"ISO 2022 IR 144",
		"\x1B\x2D\x4C",		// ISO 2022 IR 144: ESC - L
	},
	{
		// 7: Arabic
		"ISO_IR 127",
		"ISO 2022 IR 127",
		"\x1B\x2D\x47",		// ISO 2022 IR 127: ESC - G
	},
	{
		// 8: Greek
		"ISO_IR 126",
		"ISO 2022 IR 126",
		"\x1B\x2D\x46",		// ISO 2022 IR 126: ESC - F
	},
	{
		// 9: Hebrew
		"ISO_IR 138",
		"ISO 2022 IR 138",
		"\x1B\x2D\x48",		// ISO 2022 IR 138: ESC - H
	},
	{
		// 10: Thai
		"ISO_IR 166",
		"ISO 2022 IR 166",
		"\x1B\x2D\x54",		// ISO 2022 IR 166: ESC -
	},
	{
		// 11: Korean
		"ISO_IR 149",
		"ISO 2022 IR 149",
		"\x1B\x24\x29\x43",	// ISO 2022 IR 149: ESC $ ) C
	},
	{
		// 12: Japanese
		"ISO_IR 13",
		"ISO 2022 IR 13",
		"\x1B\x29\x49",		// ISO 2022 IR 13: ESC ) I
	},
	{
		// 13: Japanese
		"ISO_IR 14",
		"ISO 2022 IR 14",
		"\x1B\x28\x4A",		// ISO 2022 IR 14: ESC ( J
	},
	{
		// 14: Japanese
		"ISO_IR 87",
		"ISO 2022 IR 87",
		"\x1B\x24\x42",		// ISO 2022 IR 87: ESC $ B
	},
	{
		// 15: Japanese
		"ISO_IR 159",
		"ISO 2022 IR 159",
		"\x1B\x24\x28\x44",	// ISO 2022 IR 159: ESC $ ( D
	},
	{
		// 16: Chinese (GB2312)
		"ISO_IR 58",
		"ISO 2022 IR 58",
		"\x1B\x24\x29\x41",	// ISO 2022 IR 58: ESC $ ) A
	},
	{
		// 17: Unicode UTF8
		"ISO_IR 192",
		"",
		"",					// None for Unicode UTF8
	},
	{
		// 18: Chinese (GB18030)
		"GB18030",
		"",
		"",					// None for GB18030
	},
};

int NLS::characterSetCount()
{
	return sizeof(_characterSets) / sizeof(CharacterSet);
}

String NLS::characterSetName1(int nls)
{
	if (nls >= 0 && nls < NLS::characterSetCount())
		return _characterSets[nls].name1;
	else
		return "";
}

String NLS::characterSetName2(int nls)
{
	if (nls >= 0 && nls < NLS::characterSetCount())
		return _characterSets[nls].name2;
	else
		return "";
}

String NLS::characterSetEscapeSequence(int nls)
{
	if (nls >= 0 && nls < NLS::characterSetCount())
		return _characterSets[nls].escapeSequence;
	else
		return "";
}

#define	U8(c)	((Uint8)(c))

////////////////////////////////////////////////////////////////////////////////////////////////////////////

String NLS::toLocal8Bit(const QString& istr, int nls)
{
	QTextCodec* codec = NULL;

	switch(nls) {
	case LATIN1 :
		codec = QTextCodec::codecForName("ISO 8859-1");
		break;
	case LATIN2 :
		codec = QTextCodec::codecForName("ISO 8859-2");
		break;
	case LATIN3 :
		codec = QTextCodec::codecForName("ISO 8859-3");
		break;
	case LATIN4 :
		codec = QTextCodec::codecForName("ISO 8859-4");
		break;
	case CYRILLIC :
		codec = QTextCodec::codecForName("ISO 8859-5");
		break;
	case ARABIC :
		codec = QTextCodec::codecForName("ISO 8859-6");
		break;
	case GREEK :
		codec = QTextCodec::codecForName("ISO 8859-7");
		break;
	case HEBREW :
		codec = QTextCodec::codecForName("ISO 8859-8");
		break;
	case LATIN5 :
		codec = QTextCodec::codecForName("ISO 8859-9");
		break;
	case THAI :
		codec = QTextCodec::codecForName("TIS-620");
		break;
	case KOREAN :
		codec = QTextCodec::codecForName("EUC-KR");
		break;
	case JAPANESE :
		codec = QTextCodec::codecForName("Shift-JIS");
		break;
	case CHINESE_GB2312 :
		codec = QTextCodec::codecForName("GB2312");
		break;
	case CHINESE_GB18030 :
		codec = QTextCodec::codecForName("GB18030");
		break;
	case UNICODE_UTF8 :
		codec = QTextCodec::codecForName("UTF-8");
		break;
	}

	if (codec != NULL)
		return codec->fromUnicode(istr).constData();
	else
		return istr.toStdString().c_str();
}

QString NLS::toUnicode(const dcm::String& istr, int nls)
{
	QTextCodec* codec = NULL;

	switch(nls) {
	case LATIN1 :
		codec = QTextCodec::codecForName("ISO 8859-1");
		break;
	case LATIN2 :
		codec = QTextCodec::codecForName("ISO 8859-2");
		break;
	case LATIN3 :
		codec = QTextCodec::codecForName("ISO 8859-3");
		break;
	case LATIN4 :
		codec = QTextCodec::codecForName("ISO 8859-4");
		break;
	case CYRILLIC :
		codec = QTextCodec::codecForName("ISO 8859-5");
		break;
	case ARABIC :
		codec = QTextCodec::codecForName("ISO 8859-6");
		break;
	case GREEK :
		codec = QTextCodec::codecForName("ISO 8859-7");
		break;
	case HEBREW :
		codec = QTextCodec::codecForName("ISO 8859-8");
		break;
	case LATIN5 :
		codec = QTextCodec::codecForName("ISO 8859-9");
		break;
	case THAI :
		codec = QTextCodec::codecForName("TIS-620");
		break;
	case KOREAN :
		codec = QTextCodec::codecForName("EUC-KR");
		break;
	case JAPANESE :
		codec = QTextCodec::codecForName("Shift-JIS");
		break;
	case CHINESE_GB2312 :
		codec = QTextCodec::codecForName("GB2312");
		break;
	case CHINESE_GB18030 :
		codec = QTextCodec::codecForName("GB18030");
		break;
	case UNICODE_UTF8 :
		codec = QTextCodec::codecForName("UTF-8");
		break;
	}

	if (codec != NULL)
		return codec->toUnicode(istr.c_str());
	else
		return istr.c_str();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

String NLS::encode(const String& istr, int nls)
{
	switch(nls) {
	///////////////////////////////////////////////////////////////////////////////////////////
	// Single Byte Character Set
	case ISO_IR_6 :
	case ISO_IR_100 :
	case ISO_IR_101 :
	case ISO_IR_109 :
	case ISO_IR_110 :
	case ISO_IR_148 :
	case ISO_IR_144 :
	case ISO_IR_127 :
	case ISO_IR_126 :
	case ISO_IR_138 :
	case ISO_IR_166 :
	case ISO_IR_13 :
	case ISO_IR_14 :
		return istr;

	///////////////////////////////////////////////////////////////////////////////////////////
	// Unicode UTF-8
	case ISO_IR_192 :
		return istr;

	///////////////////////////////////////////////////////////////////////////////////////////
	// Chinese GB18030
	case GB18030 :
		return istr;
	}

	///////////////////////////////////////////////////////////////////////////////////////////
	//	Multi Byte Character Set
	///////////////////////////////////////////////////////////////////////////////////////////

	String ostr;
	int nls1 = (nls >> 0) & 0xFF;
	int nls2 = (nls >> 8) & 0xFF;
	int nls3 = (nls >> 16) & 0xFF;
	int currentNLS = nls1;

	if (nls1 < 0 || nls1 >= NLS::characterSetCount())
		nls1 = 0;
	if (nls2 < 0 || nls2 >= NLS::characterSetCount())
		nls2 = 0;
	if (nls3 < 0 || nls3 >= NLS::characterSetCount())
		nls3 = 0;

	if (((nls1 == ISO_IR_6 || nls1 == ISO_IR_13) && nls1 != ISO_IR_87) &&
		((nls2 == ISO_IR_13 || nls2 == ISO_IR_87) && nls2 != ISO_IR_6) &&
		(nls3 == ISO_IR_13 || nls3 == ISO_IR_87 || nls3 == ISO_IR_6)) {
		// Start of Japanese
		for(String::const_iterator i = istr.begin(); i != istr.end(); ) {
			if (U8(*i) == LF || U8(*i) == FF || U8(*i) == CR || (U8(*i) >= 0x20 && U8(*i) <= 0x7F)) {
				if (nls1 == ISO_IR_6 && currentNLS != ISO_IR_6) {
					ostr += characterSetEscapeSequence(ISO_IR_6);
					currentNLS = ISO_IR_6;
				} else if (nls1 == ISO_IR_13 && currentNLS != ISO_IR_13 && currentNLS != ISO_IR_14) {
					ostr += characterSetEscapeSequence(ISO_IR_14);
					currentNLS = ISO_IR_14;
				}
				ostr += U8(*i++);
			} else if (U8(*i) == 0xA0) {
				if (nls1 == ISO_IR_13 || nls2 == ISO_IR_13 || nls3 == ISO_IR_13 ||
					nls2 == ISO_IR_87 || nls3 == ISO_IR_87) {
					ostr += (U8(*i++) & 0x7F);
				}
			} else if (U8(*i) >= 0xA1 && U8(*i) <= 0xDF) {
				if (nls1 == ISO_IR_13) {
					currentNLS = ISO_IR_13;
					ostr += U8(*i++);
				} else if (nls2 == ISO_IR_13 || nls3 == ISO_IR_13) {
					if (currentNLS != ISO_IR_13) {
						ostr += characterSetEscapeSequence(ISO_IR_13);
						currentNLS = ISO_IR_13;
					}
					ostr += U8(*i++);
				}
			} else if ((U8(*i) >= 0x81 && U8(*i) <= 0x9F) || (U8(*i) >= 0xE0 && U8(*i) <= 0xEF)) {
				if (nls2 == ISO_IR_87 || nls3 == ISO_IR_87) {
					if (currentNLS != ISO_IR_87) {
						ostr += characterSetEscapeSequence(ISO_IR_87);
						currentNLS = ISO_IR_87;
					}
					Uint8 hb = U8(*i++);
					Uint8 lb = U8(*i++);
					int aa = lb < 0x9F;
					int row = hb < 0xA0 ? 0x70 : 0xB0;
					int col = aa ? (lb > 0x7F ? 0x20 : 0x1F) : 0x7E;
					ostr += ((((hb - row) << 1) - aa + 0x80) & 0x7F);
					ostr += ((lb - col + 0x80) & 0x7F);
				}
			} else {
				// just skip it.
				i++;
			}
		}
		if (nls1 == ISO_IR_6 && currentNLS != ISO_IR_6) {
			ostr += characterSetEscapeSequence(ISO_IR_6);
			currentNLS = ISO_IR_6;
		} else if (nls1 == ISO_IR_13 && currentNLS != ISO_IR_13 && currentNLS != ISO_IR_14) {
			ostr += characterSetEscapeSequence(ISO_IR_14);
			currentNLS = ISO_IR_14;
		}
		// End of Japanese
	} else if (nls1 == ISO_IR_6 && nls2 == ISO_IR_149) {
		// Start of Korean
		currentNLS = nls1;
		for(String::const_iterator i = istr.begin(); i != istr.end(); ) {
			if (U8(*i) == LF || U8(*i) == FF || U8(*i) == CR || (U8(*i) >= 0x20 && U8(*i) <= 0x7F)) {
				currentNLS = ISO_IR_6;
				ostr += U8(*i++);
			} else if (U8(*i) >= 0xA0 && U8(*i) <= 0xFF) {
				if (currentNLS != ISO_IR_149) {
					currentNLS = ISO_IR_149;
					ostr += characterSetEscapeSequence(currentNLS);
				}
				ostr += U8(*i++);
				ostr += U8(*i++);
			} else {
				// just skip it.
				i++;
			}
		}
		// End of Korean
	} else if (nls1 == ISO_IR_6 && nls2 == ISO_IR_58) {
		// Start of Chinese GB2312
		currentNLS = nls1;
		for(String::const_iterator i = istr.begin(); i != istr.end(); ) {
			if (U8(*i) == LF || U8(*i) == FF || U8(*i) == CR || (U8(*i) >= 0x20 && U8(*i) <= 0x7F)) {
				if (currentNLS != ISO_IR_6) {
					currentNLS = ISO_IR_6;
					ostr += characterSetEscapeSequence(currentNLS);
				}
				ostr += U8(*i++);
			} else if (U8(*i) >= 0xA0 && U8(*i) <= 0xFF) {
				if (currentNLS != ISO_IR_58) {
					currentNLS = ISO_IR_58;
					ostr += characterSetEscapeSequence(currentNLS);
				}
				ostr += U8(*i++);
				ostr += U8(*i++);
			} else {
				// just skip it.
				i++;
			}
		}
		if (currentNLS != ISO_IR_6) {
			currentNLS = ISO_IR_6;
			ostr += characterSetEscapeSequence(currentNLS);
		}
		// End of Chinese GB2312
	}

	return ostr;
}

String NLS::decode(const String& istr, int nls)
{
	switch(nls) {
	///////////////////////////////////////////////////////////////////////////////////////////
	// Unicode UTF-8
	case ISO_IR_192 :
		return istr;

	///////////////////////////////////////////////////////////////////////////////////////////
	// Chinese GB18030
	case GB18030 :
		return istr;
	}

	///////////////////////////////////////////////////////////////////////////////////////////
	//	Single & Multi Byte Character Set
	///////////////////////////////////////////////////////////////////////////////////////////

	String ostr;
	Uint currentNLS = ISO_IR_6;
	char mb[3];
	Uint8 hb, lb;

	for(String::const_iterator i = istr.begin(); i != istr.end(); ) {
		if (U8(*i) == ESC) {
			for(int nIndex = 0; nIndex < NLS::characterSetCount(); nIndex++) {
				String escapeSeq = characterSetEscapeSequence(nIndex);
				if (memcmp(&(*i), escapeSeq.c_str(), escapeSeq.size()) == 0) {
					currentNLS = nIndex;
					break;
				}
			}
			while(U8(*i)) {
				if (U8(*i++) >= 0x40)
					break;
			}
		} else {
			switch(currentNLS) {
			case ISO_IR_6 :
			case ISO_IR_100 :
			case ISO_IR_101 :
			case ISO_IR_109 :
			case ISO_IR_110 :
			case ISO_IR_148 :
			case ISO_IR_144 :
			case ISO_IR_127 :
			case ISO_IR_126 :
			case ISO_IR_138 :
			case ISO_IR_166 :
			default :
				mb[0] = U8(*i++);
				mb[1] = 0;
				ostr += mb;
				break;
			case ISO_IR_149 :		// KS X 1001 at G1
			case ISO_IR_58 :		// GB2312
				if (U8(*i) < 0xA0) {
					currentNLS = ISO_IR_6;
					continue;
				}
				hb = U8(*i++);
				lb = U8(*i++);
				if (lb) {
					mb[0] = hb;
					mb[1] = lb;
					mb[2] = 0;
				} else {
					mb[0] = hb;
					mb[1] = 0;
				}
				ostr += mb;
				break;
			case ISO_IR_13 :		// JIS X 0201 at G1
			case ISO_IR_14 :		// JIS X 0201 at G0
				mb[0] = U8(*i++);
				mb[1] = 0;
				ostr += mb;
				break;
			case ISO_IR_87 :		// JIS X 0208 at G0
				hb = U8(*i++);
				lb = U8(*i++);
				if (lb) {
					int row = hb < 0x5F ? 0x70 : 0xB0;
					int col = hb % 2 ? (lb > 0x5F ? 0x20 : 0x1F) : 0x7E;
					mb[0] = ((hb + 1) >> 1) + row;
					mb[1] = (lb + col);
					mb[2] = 0;
				} else {
					mb[0] = hb;
					mb[1] = 0;
				}
				ostr += mb;
				break;
			case ISO_IR_159 :		// JIS X 0212 at G0
				//	FIXME: to be implemented
				break;
			}
		}
	}

	return ostr;
}

String NLS::encodeQ(const QString& istr, int nls)
{
	return encode(toLocal8Bit(istr, nls), nls);
}

QString NLS::decodeQ(const String& istr, int nls)
{
	return toUnicode(decode(istr, nls), nls);
}
