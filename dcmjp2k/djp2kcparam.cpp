/*
*
*  Copyright (C) 1997-2011, OFFIS e.V.
*  All rights reserved.  See COPYRIGHT file for details.
*
*  This software and supporting documentation were developed by
*
*    OFFIS e.V.
*    R&D Division Health
*    Escherweg 2
*    D-26121 Oldenburg, Germany
*
*
*  Module:  dcmj2k
*
*  Author:  Martin Willkomm, Uli Schlachter
*
*  Purpose: codec parameter class for JPEG 2000 codecs
*
*/

#include "dcmtk/config/osconfig.h"
#include "dcmjp2k/djp2kcparam.h"
#include "dcmtk/ofstd/ofstd.h"

DJP2KCodecParameter::DJP2KCodecParameter(
	Uint32 fragmentSize,
	OFBool createOffsetTable,
	JP2K_UIDCreation uidCreation,
	OFBool convertToSC,
	JP2K_PlanarConfiguration planarConfiguration,
	OFBool ignoreOffsetTble)
	: DcmCodecParameter()
	, fragmentSize_(fragmentSize)
	, createOffsetTable_(createOffsetTable)
	, uidCreation_(uidCreation)
	, convertToSC_(convertToSC)
	, planarConfiguration_(planarConfiguration)
	, ignoreOffsetTable_(ignoreOffsetTble)
{
}

DJP2KCodecParameter::DJP2KCodecParameter(
	JP2K_UIDCreation uidCreation,
	JP2K_PlanarConfiguration planarConfiguration,
	OFBool ignoreOffsetTble)
	: DcmCodecParameter()
	, fragmentSize_(0)
	, createOffsetTable_(OFTrue)
	, uidCreation_(uidCreation)
	, convertToSC_(OFFalse)
	, planarConfiguration_(planarConfiguration)
	, ignoreOffsetTable_(ignoreOffsetTble)
{
}

DJP2KCodecParameter::DJP2KCodecParameter(const DJP2KCodecParameter& arg)
	: DcmCodecParameter(arg)
	, fragmentSize_(arg.fragmentSize_)
	, createOffsetTable_(arg.createOffsetTable_)
	, uidCreation_(arg.uidCreation_)
	, convertToSC_(arg.convertToSC_)
	, planarConfiguration_(arg.planarConfiguration_)
	, ignoreOffsetTable_(arg.ignoreOffsetTable_)
{
}

DJP2KCodecParameter::~DJP2KCodecParameter()
{
}

DcmCodecParameter *DJP2KCodecParameter::clone() const
{
	return new DJP2KCodecParameter(*this);
}

const char *DJP2KCodecParameter::className() const
{
	return "DJP2KCodecParameter";
}
