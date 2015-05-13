/************************************************************************
 *
 *	Copyright (c) 2007-2012, Samuel W. Choi
 *	All Rights Reserved
 *
 *	$Workfile: m_dcmUIDHelper.cpp $
 *	$Author: Samuelchoi $
 *	$Date: 07-07-02 10:27a $
 *	$Revision: 3 $
 *
 ************************************************************************/

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
			return TRUE;
	}
	return FALSE;
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
