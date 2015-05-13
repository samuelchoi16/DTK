/************************************************************************
 *
 *	Copyright (c) 2007-2012, Samuel W. Choi
 *	All Rights Reserved
 *
 *	$Workfile: DcmTag.cpp $
 *	$Author: Samuelchoi $
 *	$Date: 07-07-02 10:27a $
 *	$Revision: 2 $
 *
 ************************************************************************/

#include "DTK.h"
#include "DTKinternal.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
Tag::Tag(void)
: DcmTag()
{
}

Tag::Tag(Uint16 g, Uint16 e)
: DcmTag(g, e)
{
}

Tag::Tag(const DcmTagKey& key)
: DcmTag(key)
{
}

Tag::Tag(const Tag& dcmTag)
: DcmTag(dcmTag)
{
}

Tag::~Tag()
{
}

Tag& Tag::operator=(const Tag& dcmTag)
{
	set(dcmTag);
	return *this;
}
/*
void CDcmTag::SetVR(DcmEVR eVR)
{
	setVR(eVR);
}
*/
/*
DcmEVR Tag::GetVR() const
{
	return getEVR();
}

String Tag::getVRName() const
{
	return DcmTag::getVRName();
}
*/
/*
void CDcmTag::Set(const CDcmTag& dcmTag)
{
	set(dcmTag);
}

void CDcmTag::Set(Uint16 g, Uint16 e)
{
	set(g, e);
}

void CDcmTag::Get(Uint16* pg, Uint16* pe) const
{
	if (pg)
		*pg = getGroup();
	if (pe)
		*pe = getElement();
}

CString CDcmTag::GetTagName(void)
{
	USES_CONVERSION;
	return A2CT(getTagName());
}

void CDcmTag::setPrivateCreator(const CString& strPrivCreator)
{
	USES_CONVERSION;
	setPrivateCreator(T2CA(strPrivCreator));
}

CString CDcmTag::getPrivateCreator(void) const
{
	USES_CONVERSION;
	return A2CT(getPrivateCreator());
}

void CDcmTag::lookupVRinDictionary()
{
	lookupVRinDictionary();
}

bool CDcmTag::isSignable(void) const
{
	return isSignable();
}

bool CDcmTag::isUnknownVR(void) const
{
	return isUnknownVR();
}

int CDcmTag::operator==(const CDcmTag& key) const
{
	return DcmTagKey::operator==(key);
}

int CDcmTag::operator!=(const CDcmTag& key) const
{
	return DcmTagKey::operator!=(key);
}

int CDcmTag::operator<(const CDcmTag& key) const
{
	return DcmTagKey::operator<(key);
}

int CDcmTag::operator>(const CDcmTag& key) const
{
	return DcmTagKey::operator>(key);
}

int CDcmTag::operator<=(const CDcmTag& key) const
{
	return DcmTagKey::operator<=(key);
}

int CDcmTag::operator>=(const CDcmTag& key) const
{
	return DcmTagKey::operator>=(key);
}

String Tag::toString() const
{
	return DcmTag::toString().c_str();
}
*/
