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

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

UIDHelper::UIDHelper(const String& uidRoot)
{
	this->_uidRoot = uidRoot;
}

String UIDHelper::generateUID(void)
{
	return UIDHelper::generateUID(_uidRoot);
}

String UIDHelper::getSOPClassName(const String& sopClassUID)
{
	return dcmFindNameOfUID(sopClassUID.c_str());
}

String UIDHelper::getSOPClassUID(const String& sopClassName)
{
	return dcmFindUIDFromName(sopClassName.c_str());
}

bool UIDHelper::isStorageSOPClassUID(const String& sopClassUID)
{
	return dcmIsaStorageSOPClassUID(sopClassUID.c_str());
}

bool UIDHelper::isImageSOPClassUID(const String& strSOPClassUID)
{
	for(int i = 0; i < numberOfDcmImageSOPClassUIDs; i++) {
		if (strSOPClassUID.compare((dcmImageSOPClassUIDs[i])) == 0)
			return true;
	}
	return false;
}

String UIDHelper::generateUID(const String& uidRoot)
{
	char szBuffer[65];
	return dcmGenerateUniqueIdentifier(szBuffer, uidRoot.c_str());
}

String UIDHelper::getModality(const String& sopClassUID)
{
	return dcmSOPClassUIDToModality(sopClassUID.c_str());
}

Uint32 UIDHelper::getDataSize(const String& sopClassUID)
{
	return dcmGuessModalityBytes(sopClassUID.c_str());
}
