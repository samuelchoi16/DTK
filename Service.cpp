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

void Service::addBasicTransferSyntaxes()
{
	_transferSyntaxList.push_back(EXS_LittleEndianExplicit);
	_transferSyntaxList.push_back(EXS_BigEndianExplicit);
	_transferSyntaxList.push_back(EXS_LittleEndianImplicit);
}
