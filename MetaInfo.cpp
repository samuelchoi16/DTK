/************************************************************************
 *
 *	Copyright (c) 2007-2012, Samuel W. Choi
 *	All Rights Reserved
 *
 *	$Workfile: DcmMetaInfo.cpp $
 *	$Author: Samuelchoi $
 *	$Date: 07-07-02 10:27a $
 *	$Revision: 4 $
 *
 ************************************************************************/

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
