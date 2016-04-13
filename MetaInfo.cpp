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
		delete _dcmItem;
	_dcmItem = new DcmMetaInfo;
}

MetaInfo::MetaInfo(DcmMetaInfo* dcmMetaInfo)
	: Item(dcmMetaInfo)
{
}

MetaInfo::MetaInfo(const MetaInfo& metaInfo)
{
	if (_created)
		delete _dcmItem;
	_dcmItem = new DcmMetaInfo(*dynamic_cast<DcmMetaInfo*>(metaInfo._dcmItem));
}

MetaInfo::~MetaInfo(void)
{
	if (_created) {
		delete _dcmItem;
		_dcmItem = NULL;
	}
}

MetaInfo& MetaInfo::operator=(const MetaInfo& metaInfo)
{
	if (this == &metaInfo)
		return *this;

	if (_created)
		delete _dcmItem;
	_dcmItem = new DcmMetaInfo(*dynamic_cast<DcmMetaInfo*>(metaInfo._dcmItem));

	return *this;
}

bool MetaInfo::isValidTag(const DcmTagKey& tag)
{
	return (tag >= Tag(0x0002, 0x0000) && tag < Tag(0x0008, 0x0000)) ? true : false;
}
