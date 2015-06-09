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

////////////////////////////////////////////////////////////////////////////////////////////////////////////

int Item::_DefaultNLS = NLS::DEFAULT;

Item::Item(void)
{
	_created = true;
	_dcmItemPtr = new DcmItem;

	Status dcmStat;
	dcmStat = setAutoNLS(_DefaultNLS);
}

Item::Item(DcmItem* dcmItemPtr)
{
	_created = false;
	_dcmItemPtr = dcmItemPtr;

	int nNLS;
	Status dcmStat;
	dcmStat = getNLS(nNLS);
	if (nNLS > 0)
		dcmStat = setAutoNLS(nNLS);		// FIXME?
}

Item::Item(const Item& dcmItem)
{
	_created = true;
	_dcmItemPtr = new DcmItem(*dcmItem._dcmItemPtr);

	int nNLS;
	Status dcmStat;
	dcmStat = dcmItem.getAutoNLS(nNLS);
	dcmStat = setAutoNLS(nNLS);
}

Item::~Item(void)
{
	if (_created)
	{
		delete _dcmItemPtr;
		_dcmItemPtr = NULL;
	}
}

Item& Item::operator=(const Item& item)
{
	copyFrom(&item);
	return *this;
}

Status Item::copyFrom(const Item* sourceItemPtr)
{
	if (sourceItemPtr == NULL)
		return EC_IllegalParameter;
	if (sourceItemPtr == this)
		return EC_Normal;

	clear();

	TagList tagList;
	if (sourceItemPtr->getTagList(tagList).good()) {
		for(TagList::iterator ti = tagList.begin(); ti != tagList.end(); ti++)
			copyValueFrom(*ti, sourceItemPtr);

		int nls;
		if (sourceItemPtr->getAutoNLS(nls).good())
			setAutoNLS(nls);
		if (sourceItemPtr->getNLS(nls).good())
			setNLS(nls);

		return EC_Normal;
	} else {
		return EC_TagNotFound;
	}
}

Status Item::set(DcmItem* dcmItemPtr, int parentNLS)
{
	if (_created)
		delete _dcmItemPtr;
	_created = false;
	_dcmItemPtr = dcmItemPtr;

	int nNLS;
	Status status;
	status = getNLS(nNLS);
	if (nNLS > 0)
		status = setAutoNLS(nNLS);
	else
		status = setAutoNLS(parentNLS);
	return status;
}

Status Item::clear(void)
{
	OFCondition cond = _dcmItemPtr->clear();
	_autoNLS = NLS::DEFAULT;
	return cond;
}

Status Item::print(const char* filename) const
{
	std::ofstream os;

	os.open(filename);
	_dcmItemPtr->print(os);
	os.close();

	return EC_Normal;
}

Status Item::print(const String& filename) const
{
	std::ofstream os;

	os.open(filename.c_str());
	_dcmItemPtr->print(os);
	os.close();

	return EC_Normal;
}

Status Item::print(const QString& filename) const
{
	std::ofstream os;

	os.open(filename.toStdString().c_str());
	_dcmItemPtr->print(os);
	os.close();

	return EC_Normal;
}

Status Item::printXML(const char* filename) const
{
	std::ofstream os;

	os.open(filename);
	OFCondition cond = _dcmItemPtr->writeXML(os);
	os.close();

	return cond;
}

Status Item::printXML(const String& filename) const
{
	std::ofstream os;

	os.open(filename.c_str());
	OFCondition cond = _dcmItemPtr->writeXML(os);
	os.close();

	return cond;
}

Status Item::printXML(const QString& filename) const
{
	std::ofstream os;

	os.open(filename.toStdString().c_str());
	OFCondition cond = _dcmItemPtr->writeXML(os);
	os.close();

	return cond;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

Status Item::putString(const DcmTagKey& tag, const char* value, int nls)
{
	// AE, AS, AT, CS, DA, DS, DT, FL, FD, IS, LO, LT, OB, OF, OW, PN, SH, SL, SS, ST, TM, UI, UL, US, UT
	String str;

	if (value == NULL)
		value = "";

	switch(DcmTag(tag).getEVR())
	{
	case EVR_SH :
	case EVR_LO :
	case EVR_ST :
	case EVR_LT :
	case EVR_UT :
	case EVR_PN :
		str = NLS::encode(value, (nls == NLS::AUTO) ? _autoNLS : nls);
		break;
	default :
		str = value;
		break;
	}

	return _dcmItemPtr->putAndInsertString(tag, str.c_str());
}

Status Item::putString(const DcmTagKey& tag, const String& value, int nls)
{
	return putString(tag, value.c_str(), nls);
}

Status Item::putString(const DcmTagKey& tag, const QString& value, int nls)
{
	nls = (nls == NLS::AUTO) ? _autoNLS : nls;
	String str = NLS::toLocal8Bit(value, nls);
	return putString(tag, str, nls);
}

#define	PUT_VALUE(tag, value, pos) \
{ \
	switch(DcmTag((tag)).getEVR()) \
	{ \
	case EVR_US : \
		return _dcmItemPtr->putAndInsertUint16((tag), (Uint16)(value), (pos)); \
	case EVR_SS : \
		return _dcmItemPtr->putAndInsertSint16((tag), (Sint16)(value), (pos)); \
	case EVR_UL : \
		return _dcmItemPtr->putAndInsertUint32((tag), (Uint32)(value), (pos)); \
	case EVR_SL : \
		return _dcmItemPtr->putAndInsertSint32((tag), (Sint32)(value), (pos)); \
	case EVR_FL : \
		return _dcmItemPtr->putAndInsertFloat32((tag), (Float32)(value), (pos)); \
	case EVR_FD : \
		return _dcmItemPtr->putAndInsertFloat64((tag), (Float64)(value), (pos)); \
	default : \
		String str, str2; \
		char buffer[4000]; \
		if ((pos) > 0 && getString((tag), str2).good()) { \
			sprintf(buffer, "%s\\%d", str2.c_str(), (value)); \
			str = buffer; \
		} else { \
			sprintf(buffer, "%d", (value)); \
			str = buffer; \
		} \
		return putString((tag), str); \
	} \
}

Status Item::putValue(const DcmTagKey& tag, Uint value, Uint32 pos)
{
	PUT_VALUE(tag, value, pos);
}

Status Item::putValue(const DcmTagKey& tag, Sint value, Uint32 pos)
{
	PUT_VALUE(tag, value, pos);
}

Status Item::putValue(const DcmTagKey& tag, Uint16 value, Uint32 pos)
{
	PUT_VALUE(tag, value, pos);
}

Status Item::putValue(const DcmTagKey& tag, Sint16 value, Uint32 pos)
{
	PUT_VALUE(tag, value, pos);
}

Status Item::putValue(const DcmTagKey& tag, Uint32 value, Uint32 pos)
{
	PUT_VALUE(tag, value, pos);
}

Status Item::putValue(const DcmTagKey& tag, Sint32 value, Uint32 pos)
{
	PUT_VALUE(tag, value, pos);
}

Status Item::putValue(const DcmTagKey& tag, Float32 value, Uint32 pos)
{
	PUT_VALUE(tag, value, pos);
}

Status Item::putValue(const DcmTagKey& tag, Float64 value, Uint32 pos)
{
	PUT_VALUE(tag, value, pos);
}

Status Item::putValue(const DcmTagKey& tag, const Uint8* valuePtr, Uint32 length)
{
	return _dcmItemPtr->putAndInsertUint8Array(tag, valuePtr, length);	// AT, OW, US/SS, OB/OW
}

Status Item::putValue(const DcmTagKey& tag, const Uint16* valuePtr, Uint32 length)
{
	return _dcmItemPtr->putAndInsertUint16Array(tag, valuePtr, length);	// AT, OW, US/SS, OB/OW
}

Status Item::putValue(const DcmTagKey& tag, const Sint16* valuePtr, Uint32 length)
{
	return _dcmItemPtr->putAndInsertSint16Array(tag, valuePtr, length);	// SS, US/SS
}

Status Item::putValue(const DcmTagKey& tag, const Float32* valuePtr, Uint32 length)
{
	return _dcmItemPtr->putAndInsertFloat32Array(tag, valuePtr, length);	// FL
}

Status Item::putValue(const DcmTagKey& tag, const Float64* valuePtr, Uint32 length)
{
	return _dcmItemPtr->putAndInsertFloat64Array(tag, valuePtr, length);	// FD
}

Status Item::putEmpty(const DcmTagKey& tag)
{
	return _dcmItemPtr->insertEmptyElement(tag);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

Status Item::getString(const DcmTagKey& tag, String& value, Sint32 pos, int nls) const
{
	OFCondition cond;
	String ostr;

	if (pos == -1) {
		const char* valuePtr;
		cond = _dcmItemPtr->findAndGetString(tag, valuePtr);
		ostr = (valuePtr != NULL) ? valuePtr : "";
	} else {
		cond = _dcmItemPtr->findAndGetOFString(tag, ostr, pos);
	}

	nls = (nls == NLS::AUTO) ? _autoNLS : nls;
	if (nls > 0)
		value = NLS::decode(ostr, nls);
	else
		value = ostr;

	return cond;
}

Status Item::getString(const DcmTagKey& tag, QString& value, Sint32 pos, int nls) const
{
	OFCondition cond;
	String str;
	nls = (nls == NLS::AUTO) ? _autoNLS : nls;

	cond = getString(tag, str, pos, nls);
	value = NLS::toUnicode(str, nls);
	return cond;
}

#define	GET_VALUE(tag, value, pos) \
{ \
	OFCondition cond; \
	switch(getVR((tag))) \
	{ \
	case EVR_US : \
		Uint16 u16; \
		cond = _dcmItemPtr->findAndGetUint16((tag), u16, (pos)); \
		(value) = u16; \
		break; \
	case EVR_SS : \
		Sint16 s16; \
		cond = _dcmItemPtr->findAndGetSint16((tag), s16, (pos)); \
		(value) = s16; \
		break; \
	case EVR_UL : \
		Uint32 u32; \
		cond = _dcmItemPtr->findAndGetUint32((tag), u32, (pos)); \
		(value) = u32; \
		break; \
	case EVR_SL : \
		Sint32 s32; \
		cond = _dcmItemPtr->findAndGetSint32((tag), s32, (pos)); \
		(value) = s32; \
		break; \
	case EVR_FL : \
		Float32 f32; \
		cond = _dcmItemPtr->findAndGetFloat32((tag), f32, (pos)); \
		(value) = f32; \
		break; \
	case EVR_FD : \
		Float64 f64; \
		cond = _dcmItemPtr->findAndGetFloat64((tag), f64, (pos)); \
		(value) = f64; \
		break; \
	default : \
		String str; \
		cond = getString((tag), str, (pos)); \
		(value) = atoi(str.c_str()); \
		break; \
	} \
	return cond; \
}

Status Item::getValue(const DcmTagKey& tag, Uint& value, Uint32 pos) const
{
	GET_VALUE(tag, value, pos);
}

Status Item::getValue(const DcmTagKey& tag, Sint& value, Uint32 pos) const
{
	GET_VALUE(tag, value, pos);
}

Status Item::getValue(const DcmTagKey& tag, Uint16& value, Uint32 pos) const
{
	GET_VALUE(tag, value, pos);
}

Status Item::getValue(const DcmTagKey& tag, Uint32& value, Uint32 pos) const
{
	GET_VALUE(tag, value, pos);
}

Status Item::getValue(const DcmTagKey& tag, Sint16& value, Uint32 pos) const
{
	GET_VALUE(tag, value, pos);
}

Status Item::getValue(const DcmTagKey& tag, Sint32& value, Uint32 pos) const
{
	GET_VALUE(tag, value, pos);
}

Status Item::getValue(const DcmTagKey& tag, Float32& value, Uint32 pos) const
{
	GET_VALUE(tag, value, pos);
}

Status Item::getValue(const DcmTagKey& tag, Float64& value, Uint32 pos) const
{
	GET_VALUE(tag, value, pos);
}

Status Item::getValue(const DcmTagKey& tag, Uint8& value, Uint32 pos) const
{
	return _dcmItemPtr->findAndGetUint8(tag, value, pos);
}

Status Item::getValue(const DcmTagKey& tag, const Uint8*& valuePtr, Ulong* countPtr) const
{
	return _dcmItemPtr->findAndGetUint8Array(tag, valuePtr, countPtr);
}

Status Item::getValue(const DcmTagKey& tag, const Uint16*& valuePtr, Ulong* countPtr) const
{
	return _dcmItemPtr->findAndGetUint16Array(tag, valuePtr, countPtr);
}

Status Item::getValue(const DcmTagKey& tag, const Uint32*& valuePtr, Ulong* countPtr) const
{
	return _dcmItemPtr->findAndGetUint32Array(tag, valuePtr, countPtr);
}

Status Item::getValue(const DcmTagKey& tag, const Sint16*& valuePtr, Ulong* countPtr) const
{
	return _dcmItemPtr->findAndGetSint16Array(tag, valuePtr, countPtr);
}

Status Item::getValue(const DcmTagKey& tag, const Sint32*& valuePtr, Ulong* countPtr) const
{
	return _dcmItemPtr->findAndGetSint32Array(tag, valuePtr, countPtr);
}

Status Item::getValue(const DcmTagKey& tag, const Float32*& valuePtr, Ulong* countPtr) const
{
	return _dcmItemPtr->findAndGetFloat32Array(tag, valuePtr, countPtr);
}

Status Item::getValue(const DcmTagKey& tag, const Float64*& valuePtr, Ulong* countPtr) const
{
	return _dcmItemPtr->findAndGetFloat64Array(tag, valuePtr, countPtr);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

Status Item::putItem(const DcmTagKey& tag, Item& item, Sint32 pos)
{
	DcmItem* dcmItemPtr = NULL;
	OFCondition cond = _dcmItemPtr->findOrCreateSequenceItem(tag, dcmItemPtr, pos);
	if (cond.good())
		item.set(dcmItemPtr, _autoNLS);
	return cond;
}

Status Item::getItem(const DcmTagKey& tag, Item& item, Sint32 pos) const
{
	DcmItem* dcmItemPtr = NULL;
	OFCondition cond = _dcmItemPtr->findAndGetSequenceItem(tag, dcmItemPtr, pos);
	if (cond.good())
		item.set(dcmItemPtr, _autoNLS);
	return cond;
}

Sint32 Item::getItemCount(const DcmTagKey& tag) const
{
	DcmSequenceOfItems* dcmSequencePtr = NULL;
	OFCondition cond = _dcmItemPtr->findAndGetSequence(tag, dcmSequencePtr);
	return (cond.good() && dcmSequencePtr) ? dcmSequencePtr->card() : -1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
CDcmStatus CDcmItem::putPixelItem(const DcmTagKey& tag, const CDcmPixelSequence& dcmPixelSequence)
{
	return EC_IllegalParameter;
}
*/
Status Item::getPixelItem(const DcmTagKey& tag, Uint32 pos, Uint8*& dataPtr, Uint32* lengthPtr) const
{
	DcmPixelSequence* dcmPixelSequencePtr = getPixelSequence(tag);
	if (dcmPixelSequencePtr == NULL)
		return EC_IllegalParameter;

	DcmPixelItem* dcmPixelItemPtr = NULL;
	OFCondition cond = dcmPixelSequencePtr->getItem(dcmPixelItemPtr, pos);
	if (cond.good())
	{
		cond = dcmPixelItemPtr->getUint8Array(dataPtr);
		*lengthPtr = dcmPixelItemPtr->getLength();
	}
	return cond;
}

Sint32 Item::getPixelItemCount(const DcmTagKey& tag) const
{
	DcmPixelSequence* dcmPixelSequencePtr = getPixelSequence(tag);
	return dcmPixelSequencePtr ? dcmPixelSequencePtr->card() : -1;
}

DcmPixelSequence* Item::getPixelSequence(const DcmTagKey& tag) const
{
	OFCondition cond;
	DcmElement* dcmElementPtr = NULL;
	cond = _dcmItemPtr->findAndGetElement(tag, dcmElementPtr);
	if (cond.bad())
		return NULL;
	DcmPixelData* dcmPixelDataPtr = dynamic_cast<DcmPixelData*>(dcmElementPtr);
	if (dcmPixelDataPtr == NULL)
		return NULL;

	E_TransferSyntax ts;
	const DcmRepresentationParameter* dcmRepParamPtr = NULL;
	dcmPixelDataPtr->getOriginalRepresentationKey(ts, dcmRepParamPtr);
	if (cond.bad())
		return NULL;

	DcmPixelSequence* dcmPixelSequencePtr = NULL;
	cond = dcmPixelDataPtr->getEncapsulatedRepresentation(ts, dcmRepParamPtr, dcmPixelSequencePtr);
	if (cond.bad())
		return NULL;
	if (dcmPixelSequencePtr == NULL)
		return NULL;

	return dcmPixelSequencePtr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

Status Item::putDate(const DcmTagKey& tag, const DateTime& dt)
{
	// DA, DT
	switch(DcmTag(tag).getEVR()) {
	case EVR_DA :
		return putString(tag, dt.format("%Y%m%d"));
		break;
	case EVR_DT :
		return putString(tag, dt.format("%Y%m%d%H%M%S"));
		break;
	default :
		return EC_InvalidTag;
	}
}

Status Item::putDate(const DcmTagKey& tag, const QDateTime& qdt)
{
	// DA, DT
	switch(DcmTag(tag).getEVR()) {
	case EVR_DA :
		return putString(tag, qdt.toString("yyyyMMdd"));
		break;
	case EVR_DT :
		return putString(tag, qdt.toString("HHmmss"));
		break;
	default :
		return EC_InvalidTag;
	}
}

Status Item::getDate(const DcmTagKey& tag, DateTime& dt) const
{
	Status stat;
	String str;

	// DA, TM, DT
	switch(DcmTag(tag).getEVR()) {
	case EVR_DA :
		stat = getString(tag, str);
		if (str.length() < 8)
			return EC_InvalidStream;
		dt.tm_year = atoi(str.substr(0, 4).c_str());
		dt.tm_mon = atoi(str.substr(4, 2).c_str()) - 1;
		dt.tm_mday = atoi(str.substr(6, 2).c_str());
		break;
	case EVR_TM :
		stat = getString(tag, str);
		if (str.length() < 6)
			return EC_InvalidStream;
		dt.tm_hour = atoi(str.substr(0, 2).c_str());
		dt.tm_min = atoi(str.substr(2, 2).c_str());
		dt.tm_sec = atoi(str.substr(4, 2).c_str());
		break;
	case EVR_DT :
		stat = getString(tag, str);
		if (str.length() < 14)
			return EC_InvalidStream;
		dt.tm_year = atoi(str.substr(0, 4).c_str());
		dt.tm_mon = atoi(str.substr(4, 2).c_str()) - 1;
		dt.tm_mday = atoi(str.substr(6, 2).c_str());
		dt.tm_hour = atoi(str.substr(8, 2).c_str());
		dt.tm_min = atoi(str.substr(10, 2).c_str());
		dt.tm_sec = atoi(str.substr(12, 2).c_str());
		break;
	default :
		return EC_InvalidTag;
	}

	return stat;
}

Status Item::getDate(const DcmTagKey& tag, QDateTime& qdt) const
{
	Status stat;
	String str;

	// DA, TM, DT
	switch(DcmTag(tag).getEVR()) {
	case EVR_DA :
		stat = getString(tag, str);
		if (str.length() < 8)
			return EC_InvalidStream;
		qdt.setDate(QDate(atoi(str.substr(0, 4).c_str()), atoi(str.substr(4, 2).c_str()), atoi(str.substr(6, 2).c_str())));
		break;
	case EVR_TM :
		stat = getString(tag, str);
		if (str.length() < 6)
			return EC_InvalidStream;
		qdt.setTime(QTime(atoi(str.substr(0, 2).c_str()), atoi(str.substr(2, 2).c_str()), atoi(str.substr(4, 2).c_str())));
		break;
	case EVR_DT :
		stat = getString(tag, str);
		if (str.length() < 14)
			return EC_InvalidStream;
		qdt.setDate(QDate(atoi(str.substr(0, 4).c_str()), atoi(str.substr(4, 2).c_str()), atoi(str.substr(6, 2).c_str())));
		qdt.setTime(QTime(atoi(str.substr(8, 2).c_str()), atoi(str.substr(10, 2).c_str()), atoi(str.substr(12, 2).c_str())));
		break;
	default :
		return EC_InvalidTag;
	}

	return stat;
}

Status Item::putDateTime(const DcmTagKey& dtag, const DcmTagKey& ttag, const DateTime& dt)
{
	// DA
	if (DcmTag(dtag).getEVR() == EVR_DA)
		return putString(dtag, dt.format("%Y%m%d"));
	else
		return EC_InvalidTag;

	// TM
	if (DcmTag(ttag).getEVR() == EVR_TM)
		return putString(ttag, dt.format("%H%M%S"));
	else
		return EC_InvalidTag;
}

Status Item::putDateTime(const DcmTagKey& dtag, const DcmTagKey& ttag, const QDateTime& qdt)
{
	// DA
	if (DcmTag(dtag).getEVR() == EVR_DA)
		return putString(dtag, qdt.toString("yyyyMMdd"));
	else
		return EC_InvalidTag;

	// TM
	if (DcmTag(ttag).getEVR() == EVR_TM)
		return putString(ttag, qdt.toString("HHmmss"));
	else
		return EC_InvalidTag;
}

Status Item::getDateTime(const DcmTagKey& dtag, const DcmTagKey& ttag, DateTime& dt) const
{
	Status status;
	String str;
	bool ok = false;

	// DA, TM
	if (DcmTag(dtag).getEVR() == EVR_DA) {
		status = getString(dtag, str);
		if (str.length() < 8)
			return EC_InvalidStream;
		dt.tm_year = atoi(str.substr(0, 4).c_str());
		dt.tm_mon = atoi(str.substr(4, 2).c_str()) - 1;
		dt.tm_mday = atoi(str.substr(6, 2).c_str());
		ok = true;
	}
	if (DcmTag(ttag).getEVR() == EVR_TM) {
		getString(ttag, str);
		if (str.length() < 6)
			return EC_InvalidStream;
		dt.tm_hour = atoi(str.substr(0, 2).c_str());
		dt.tm_min = atoi(str.substr(2, 2).c_str());
		dt.tm_sec = atoi(str.substr(4, 2).c_str());
		ok = true;
	}
	if (!ok)
		return EC_InvalidTag;

	return status;
}

Status Item::getDateTime(const DcmTagKey& dtag, const DcmTagKey& ttag, QDateTime& qdt) const
{
	Status status;
	String str;
	bool ok = false;

	// DA, TM
	if (DcmTag(dtag).getEVR() == EVR_DA) {
		status = getString(dtag, str);
		if (str.length() < 8)
			return EC_InvalidStream;
		qdt.setDate(QDate(atoi(str.substr(0, 4).c_str()), atoi(str.substr(4, 2).c_str()), atoi(str.substr(6, 2).c_str())));
		ok = true;
	}
	if (DcmTag(ttag).getEVR() == EVR_TM) {
		getString(ttag, str);
		if (str.length() < 6)
			return EC_InvalidStream;
		qdt.setTime(QTime(atoi(str.substr(0, 2).c_str()), atoi(str.substr(2, 2).c_str()), atoi(str.substr(4, 2).c_str())));
		ok = true;
	}
	if (!ok)
		return EC_InvalidTag;

	return status;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

Status Item::putCode(const String& codeValue, const String& codingSchemeDesignator, const String& codingSchemeVersion, const String& codeMeaning)
{
	Status status;
	status = putString(DCM_CodeValue, codeValue);
	status = putString(DCM_CodingSchemeDesignator, codingSchemeDesignator);
	status = putString(DCM_CodingSchemeVersion, codingSchemeVersion);
	status = putString(DCM_CodeMeaning, codeMeaning);
	return status;
}

Status Item::putCode(const QString& codeValue, const QString& codingSchemeDesignator, const QString& codingSchemeVersion, const QString& codeMeaning)
{
	Status status;
	status = putString(DCM_CodeValue, codeValue);
	status = putString(DCM_CodingSchemeDesignator, codingSchemeDesignator);
	status = putString(DCM_CodingSchemeVersion, codingSchemeVersion);
	status = putString(DCM_CodeMeaning, codeMeaning);
	return status;
}

Status Item::getCode(String& codeValue, String& codingSchemeDesignator, String& codingSchemeVersion, String& codeMeaning) const
{
	Status status;
	status = getString(DCM_CodeValue, codeValue);
	status = getString(DCM_CodingSchemeDesignator, codingSchemeDesignator);
	status = getString(DCM_CodingSchemeVersion, codingSchemeVersion);
	status = getString(DCM_CodeMeaning, codeMeaning);
	return status;
}

Status Item::getCode(QString& codeValue, QString& codingSchemeDesignator, QString& codingSchemeVersion, QString& codeMeaning) const
{
	Status status;
	status = getString(DCM_CodeValue, codeValue);
	status = getString(DCM_CodingSchemeDesignator, codingSchemeDesignator);
	status = getString(DCM_CodingSchemeVersion, codingSchemeVersion);
	status = getString(DCM_CodeMeaning, codeMeaning);
	return status;
}

Status Item::putCode(const DcmTagKey& tag, Item& item,
					 const String& codeValue, const String& codingSchemeDesignator, const String& codingSchemeVersion, const String& codeMeaning,
					 Sint32 pos)
{
	Status status = putItem(tag, item, pos);
	if (status.good())
		status = item.putCode(codeValue, codingSchemeDesignator, codingSchemeVersion, codeMeaning);
	return status;
}

Status Item::putCode(const DcmTagKey& tag, Item& item,
					 const QString& codeValue, const QString& codingSchemeDesignator, const QString& codingSchemeVersion, const QString& codeMeaning,
					 Sint32 pos)
{
	Status status = putItem(tag, item, pos);
	if (status.good())
		status = item.putCode(codeValue, codingSchemeDesignator, codingSchemeVersion, codeMeaning);
	return status;
}

Status Item::getCode(const DcmTagKey& tag, Item& item,
					 String& codeValue, String& codingSchemeDesignator, String& codingSchemeVersion, String& codeMeaning,
					 Sint32 pos) const
{
	Status status = getItem(tag, item, pos);
	if (status.good())
		status = item.getCode(codeValue, codingSchemeDesignator, codingSchemeVersion, codeMeaning);
	return status;
}

Status Item::getCode(const DcmTagKey& tag, Item& item,
					 QString& codeValue, QString& codingSchemeDesignator, QString& codingSchemeVersion, QString& codeMeaning,
					 Sint32 pos) const
{
	Status status = getItem(tag, item, pos);
	if (status.good())
		status = item.getCode(codeValue, codingSchemeDesignator, codingSchemeVersion, codeMeaning);
	return status;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

Status Item::putRefSOP(const String& sopClassUID, const String& sopInstanceUID)
{
	Status status;
	status = putString(DCM_ReferencedSOPClassUID, sopClassUID);
	status = putString(DCM_ReferencedSOPInstanceUID, sopInstanceUID);
	return status;
}

Status Item::putRefSOP(const QString& sopClassUID, const QString& sopInstanceUID)
{
	Status status;
	status = putString(DCM_ReferencedSOPClassUID, sopClassUID);
	status = putString(DCM_ReferencedSOPInstanceUID, sopInstanceUID);
	return status;
}

Status Item::getRefSOP(String& sopClassUID, String& sopInstanceUID) const
{
	Status status;
	status = getString(DCM_ReferencedSOPClassUID, sopClassUID);
	status = getString(DCM_ReferencedSOPInstanceUID, sopInstanceUID);
	return status;
}

Status Item::getRefSOP(QString& sopClassUID, QString& sopInstanceUID) const
{
	Status status;
	status = getString(DCM_ReferencedSOPClassUID, sopClassUID);
	status = getString(DCM_ReferencedSOPInstanceUID, sopInstanceUID);
	return status;
}

Status Item::putRefSOP(const DcmTagKey& tag, Item& item, const String& sopClassUID, const String& sopInstanceUID, Sint32 pos)
{
	Status status = putItem(tag, item, pos);
	if (status.good())
		status = item.putRefSOP(sopClassUID, sopInstanceUID);
	return status;
}

Status Item::putRefSOP(const DcmTagKey& tag, Item& item, const QString& sopClassUID, const QString& sopInstanceUID, Sint32 pos)
{
	Status status = putItem(tag, item, pos);
	if (status.good())
		status = item.putRefSOP(sopClassUID, sopInstanceUID);
	return status;
}

Status Item::getRefSOP(const DcmTagKey& tag, Item& item, String& sopClassUID, String& sopInstanceUID, Sint32 pos) const
{
	Status status = getItem(tag, item, pos);
	if (status.good())
		status = item.getRefSOP(sopClassUID, sopInstanceUID);
	return status;
}

Status Item::getRefSOP(const DcmTagKey& tag, Item& item, QString& sopClassUID, QString& sopInstanceUID, Sint32 pos) const
{
	Status status = getItem(tag, item, pos);
	if (status.good())
		status = item.getRefSOP(sopClassUID, sopInstanceUID);
	return status;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

Status Item::getVR(const DcmTagKey& tag, DcmEVR& vr) const
{
	DcmElement* dcmElementPtr;
	OFCondition cond = _dcmItemPtr->findAndGetElement(tag, dcmElementPtr);
	if (cond.bad())
		return cond;
	vr = dcmElementPtr->getTag().getEVR();
	return cond;
}

Status Item::getVM(const DcmTagKey& tag, Uint32& vm) const
{
	DcmElement* dcmElementPtr;
	OFCondition cond = _dcmItemPtr->findAndGetElement(tag, dcmElementPtr);
	if (cond.bad())
		return cond;
	vm = dcmElementPtr->getVM();
	return cond;
}

Status Item::getVL(const DcmTagKey& tag, Uint32& vl) const
{
	DcmElement* dcmElementPtr;
	OFCondition cond = _dcmItemPtr->findAndGetElement(tag, dcmElementPtr);
	if (cond.bad())
		return cond;
	vl = dcmElementPtr->getLength();
	return cond;
}

DcmEVR Item::getVR(const DcmTagKey& tag) const
{
	DcmEVR vr;
	if (getVR(tag, vr).good())
		return vr;
	else
		return EVR_na;
}

Sint32 Item::getVM(const DcmTagKey& tag) const
{
	Uint32 vm;
	if (getVM(tag, vm).good())
		return vm;
	else
		return -1;
}

Sint32 Item::getVL(const DcmTagKey& tag) const
{
	Uint32 nVL;
	if (getVL(tag, nVL).good())
		return nVL;
	else
		return -1;
}

bool Item::hasTag(const DcmTagKey& tag) const
{
	return _dcmItemPtr->tagExists(tag);
}

bool Item::hasTagValue(const DcmTagKey& tag) const
{
	return _dcmItemPtr->tagExistsWithValue(tag);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

Status Item::getTagList(TagList& tagList) const
{
	tagList.clear();

	DcmObject* dcmObjectPtr = NULL;
	while((dcmObjectPtr = _dcmItemPtr->nextInContainer(dcmObjectPtr)) != NULL) {
		Tag tag = dcmObjectPtr->getTag();
		tagList.push_back(tag);
	}

	return EC_Normal;
}

Status Item::copyValueFrom(const DcmTagKey& tag, const Item* sourceItemPtr, DcmTagKey sourceTag)
{
	Status stat;
	String str;
	Item subItemSource, subItemTarget;
	const Sint16* s16Ptr = NULL;
	const Uint16* u16Ptr = NULL;
	const Sint32* s32Ptr = NULL;
	const Uint32* u32Ptr = NULL;
	const Uint8* obPtr = NULL;
	const Uint16* owPtr = NULL;
	const Float32* ofPtr = NULL;
	Ulong length = 0;

	if (sourceTag == DcmTagKey(0xffff, 0xffff))
		sourceTag = tag;
	if (!sourceItemPtr->hasTag(sourceTag))
		return EC_TagNotFound;

	switch(sourceItemPtr->getVR(sourceTag))
	{
	case EVR_AE :
	case EVR_AS :
	case EVR_AT :
	case EVR_CS :
	case EVR_DA :
	case EVR_DS :
	case EVR_DT :
	case EVR_FL :
	case EVR_FD :
	case EVR_IS :
	case EVR_LO :
	case EVR_LT :
	case EVR_PN :
	case EVR_SH :
	case EVR_ST :
	case EVR_TM :
	case EVR_UI :
	case EVR_UT :
		stat = sourceItemPtr->getString(sourceTag, str);
		stat = putString(tag, str);
		break;
	case EVR_SL :
		sourceItemPtr->getValue(sourceTag, s32Ptr, &length);
		for(int i = 0; i < length; i++) {
			stat = putValue(tag, s32Ptr[i], i);
		}
		break;
	case EVR_SS :
		sourceItemPtr->getValue(sourceTag, s16Ptr, &length);
		stat = putValue(tag, s16Ptr, length);
		break;
	case EVR_UL :
		sourceItemPtr->getValue(sourceTag, u32Ptr, &length);
		for(int i = 0; i < length; i++) {
			stat = putValue(tag, u32Ptr[i], i);
		}
		break;
	case EVR_US :
		sourceItemPtr->getValue(sourceTag, u16Ptr, &length);
		stat = putValue(tag, u16Ptr, length);
		break;
	case EVR_SQ :
		for(Sint32 pos = 0; (stat = sourceItemPtr->getItem(sourceTag, subItemSource, pos)).good(); pos++) {
			stat = putItem(tag, subItemTarget, pos);
			TagList tagList;
			stat = subItemSource.getTagList(tagList);
			for(TagList::iterator ti = tagList.begin(); ti != tagList.end(); ti++)
				stat = subItemTarget.copyValueFrom(*ti, &subItemSource);
		}
		break;
	case EVR_OB :
		stat = sourceItemPtr->getValue(sourceTag, obPtr, &length);
		stat = putValue(tag, obPtr, length);
		break;
	case EVR_OW :
		stat = sourceItemPtr->getValue(sourceTag, owPtr, &length);
		stat = putValue(tag, owPtr, length);
		break;
	case EVR_OF :
		stat = sourceItemPtr->getValue(sourceTag, ofPtr, &length);
		stat = putValue(tag, ofPtr, length);
		break;
	default :
		stat = EC_IllegalParameter;
		break;
	}
	return stat;
}

Status Item::removeValue(const DcmTagKey& tag)
{
	DcmElement* dcmElementPtr = _dcmItemPtr->remove(tag);
	if (dcmElementPtr) {
		delete dcmElementPtr;
		return EC_Normal;
	} else {
		return EC_TagNotFound;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

Status Item::setNLS(int nls)
{
	Uint nls1 = (nls >> 0) & 0xFF;
	Uint nls2 = (nls >> 8) & 0xFF;
	Uint nls3 = (nls >> 16) & 0xFF;
	String str;

	if (nls1 >= NLS::characterSetCount() || nls2 >= NLS::characterSetCount() || nls3 >= NLS::characterSetCount())
		return EC_IllegalParameter;

	if ((nls2 == 0) && (nls3 == 0)) {
		if (nls1)
			str = NLS::characterSetName1(nls1);
	} else {
		if (nls1)
			str = NLS::characterSetName2(nls1);
		if (nls2)
			str = str + "\\" + NLS::characterSetName2(nls2);
		if (nls3)
			str = str + "\\" + NLS::characterSetName2(nls3);
	}

	if (str.size() > 0) {
		return _dcmItemPtr->putAndInsertString(DCM_SpecificCharacterSet, str.c_str());
	} else {
		return EC_Normal;
	}
}

Status Item::getNLS(int& nls) const
{
	OFCondition cond1, cond2, cond3;
	String nls1, nls2, nls3;
	nls = 0;

	// NLS component 1
	cond1 = _dcmItemPtr->findAndGetOFString(DCM_SpecificCharacterSet, nls1, 0);
	if (cond1.good() && nls1.size() > 0) {
		for(int i = 0; i < NLS::characterSetCount(); i++) {
			if (nls1.compare(NLS::characterSetName1(i)) == 0) {
				nls = i;
				break;
			}
		}
		for(int i = 0; i < NLS::characterSetCount(); i++) {
			if (nls1.compare(NLS::characterSetName2(i)) == 0) {
				nls = i;
				break;
			}
		}
	}

	// NLS component 2
	cond2 = _dcmItemPtr->findAndGetOFString(DCM_SpecificCharacterSet, nls2, 1);
	if (cond2.good() && nls2.size() > 0) {
		for(int i = 0; i < NLS::characterSetCount(); i++) {
			if (nls2.compare(NLS::characterSetName2(i)) == 0) {
				nls |= (i << 8);
				break;
			}
		}
	}

	// NLS component 3
	cond3 = _dcmItemPtr->findAndGetOFString(DCM_SpecificCharacterSet, nls3, 2);
	if (cond3.good() && nls3.size() > 0) {
		for(int i = 0; i < NLS::characterSetCount(); i++) {
			if (nls3.compare(NLS::characterSetName2(i)) == 0) {
				nls |= (i << 16);
				break;
			}
		}
	}

	return cond1;
}

Status Item::setAutoNLS(int nls)
{
    _autoNLS = DTK_MAX(nls, 0);
	return EC_Normal;
}

Status Item::getAutoNLS(int& nls) const
{
	nls = _autoNLS;
	return EC_Normal;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

DcmItem* Item::_getDcmItem(void) const
{
	return _dcmItemPtr;
}

Item::operator DcmItem*(void) const
{
	return _dcmItemPtr;
}
