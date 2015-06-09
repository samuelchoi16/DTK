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

MetaInfo::MetaInfo(void)
{
	if (_created)
		delete _dcmItemPtr;
	_dcmItemPtr = new DcmMetaInfo;
}

MetaInfo::MetaInfo(DcmMetaInfo* dcmMetaInfoPtr)
	: Item(dcmMetaInfoPtr)
{
}

MetaInfo::MetaInfo(const MetaInfo& metaInfo)
{
	if (_created)
		delete _dcmItemPtr;
	_dcmItemPtr = new DcmMetaInfo(*dynamic_cast<DcmMetaInfo*>(metaInfo._dcmItemPtr));
}

MetaInfo::~MetaInfo(void)
{
	if (_created) {
		delete _dcmItemPtr;
		_dcmItemPtr = NULL;
	}
}

MetaInfo& MetaInfo::operator=(const MetaInfo& metaInfo)
{
	if (this == &metaInfo)
		return *this;

	if (_created)
		delete _dcmItemPtr;
	_dcmItemPtr = new DcmMetaInfo(*dynamic_cast<DcmMetaInfo*>(metaInfo._dcmItemPtr));

	return *this;
}
