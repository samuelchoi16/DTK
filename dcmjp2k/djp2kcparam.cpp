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
#include "dcmjp2k/djp2kinternal.h"

CodecParameter::CodecParameter(
	OFBool jp2k_optionsEnabled,
	Uint32 fragmentSize,
	OFBool createOffsetTable,
	UIDCreation uidCreation,
	OFBool convertToSC,
	PlanarConfiguration planarConfiguration,
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

CodecParameter::CodecParameter(
	UIDCreation uidCreation,
	PlanarConfiguration planarConfiguration,
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

CodecParameter::CodecParameter(const CodecParameter& arg)
	: DcmCodecParameter(arg)
	, fragmentSize_(arg.fragmentSize_)
	, createOffsetTable_(arg.createOffsetTable_)
	, uidCreation_(arg.uidCreation_)
	, convertToSC_(arg.convertToSC_)
	, planarConfiguration_(arg.planarConfiguration_)
	, ignoreOffsetTable_(arg.ignoreOffsetTable_)
{
}

CodecParameter::~CodecParameter()
{
}

DcmCodecParameter *CodecParameter::clone() const
{
	return new CodecParameter(*this);
}

const char *CodecParameter::className() const
{
	return "DJP2KCodecParameter";
}
