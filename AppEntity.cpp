/************************************************************************
 *
 *	Copyright (c) 2007-2012, Samuel W. Choi
 *	All Rights Reserved
 *
 *	$Workfile: dcmAppEntity.cpp $
 *	$Author: Pcc7510 $
 *	$Date: 08-10-08 11:28a $
 *	$Revision: 3 $
 *
 ************************************************************************/

#include "DTK.h"
#include "DTKinternal.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

AppEntity::AppEntity(void)
{
	_checkApplicationContextName = true;
	_checkImplementationClassUID = true;
	_maxPDUSize = 16384;

	_ascNetworkPtr = NULL;
}

AppEntity::~AppEntity(void)
{
	if (_ascNetworkPtr)
		exit();
}

Status AppEntity::init(const String& aetitle, const T_ASC_NetworkRole ascRole, const int listenerPort, const int timeout)
{
	_aetitle = aetitle;
	OFCondition cond = ASC_initializeNetwork(ascRole, listenerPort, timeout, &_ascNetworkPtr);
	return cond;
}

Status AppEntity::exit(void)
{
	OFCondition cond = ASC_dropNetwork(&_ascNetworkPtr);
	_ascNetworkPtr = NULL;
	return cond;
}

String AppEntity::getAETitle(void) const
{
	return _aetitle;
}

T_ASC_Network *AppEntity::getInternal(void)
{
	return _ascNetworkPtr;
}
