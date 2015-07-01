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

AppEntity::AppEntity(void)
{
	_checkApplicationContextName = true;
	_checkImplementationClassUID = true;
	_maxPDUSize = 16384;

	_ascNetwork = NULL;
}

AppEntity::~AppEntity(void)
{
	if (_ascNetwork)
		exit();
}

Status AppEntity::init(const String& aetitle, const T_ASC_NetworkRole ascRole, const int listenerPort, const int timeout)
{
	_aetitle = aetitle;
	OFCondition cond = ASC_initializeNetwork(ascRole, listenerPort, timeout, &_ascNetwork);
	return cond;
}

Status AppEntity::exit(void)
{
	OFCondition cond = ASC_dropNetwork(&_ascNetwork);
	_ascNetwork = NULL;
	return cond;
}

String AppEntity::getAETitle(void) const
{
	return _aetitle;
}

T_ASC_Network *AppEntity::getInternal(void)
{
	return _ascNetwork;
}
