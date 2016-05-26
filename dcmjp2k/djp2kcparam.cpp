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
	/*
	OFBool jpls_optionsEnabled,
	Uint16 jpls_t1, // these are the defaults for 8bpp in lossless mode
	Uint16 jpls_t2,
	Uint16 jpls_t3,
	Uint16 jpls_reset,
	Uint16 jpls_limit,
	OFBool preferCookedEncoding,
	*/
	Uint32 fragmentSize,
	OFBool createOffsetTable,
	JP2K_UIDCreation uidCreation,
	OFBool convertToSC,
	JP2K_PlanarConfiguration planarConfiguration,
	OFBool ignoreOffsetTble//,
/*	interleaveMode jplsInterleaveMode*/)
	: DcmCodecParameter()
	/*
	, jpls_optionsEnabled_(jpls_optionsEnabled)
	, jpls_t1_(jpls_t1)
	, jpls_t2_(jpls_t2)
	, jpls_t3_(jpls_t3)
	, jpls_reset_(jpls_reset)
	, jpls_limit_(jpls_limit)
	*/
	, fragmentSize_(fragmentSize)
	, createOffsetTable_(createOffsetTable)
//	, preferCookedEncoding_(preferCookedEncoding)
	, uidCreation_(uidCreation)
	, convertToSC_(convertToSC)
//	, jplsInterleaveMode_(jplsInterleaveMode)
	, planarConfiguration_(planarConfiguration)
	, ignoreOffsetTable_(ignoreOffsetTble)
{
}

DJP2KCodecParameter::DJP2KCodecParameter(
	JP2K_UIDCreation uidCreation,
	JP2K_PlanarConfiguration planarConfiguration,
	OFBool ignoreOffsetTble)
	: DcmCodecParameter()
	/*
	, jpls_optionsEnabled_(OFFalse)
	, jpls_t1_(3)
	, jpls_t2_(7)
	, jpls_t3_(21)
	, jpls_reset_(64)
	, jpls_limit_(0)
	*/
	, fragmentSize_(0)
	, createOffsetTable_(OFTrue)
//	, preferCookedEncoding_(OFTrue)
	, uidCreation_(uidCreation)
	, convertToSC_(OFFalse)
//	, jplsInterleaveMode_(interleaveDefault)
	, planarConfiguration_(planarConfiguration)
	, ignoreOffsetTable_(ignoreOffsetTble)
{
}

DJP2KCodecParameter::DJP2KCodecParameter(const DJP2KCodecParameter& arg)
	: DcmCodecParameter(arg)
	/*
	, jpls_optionsEnabled_(arg.jpls_optionsEnabled_)
	, jpls_t1_(arg.jpls_t1_)
	, jpls_t2_(arg.jpls_t2_)
	, jpls_t3_(arg.jpls_t3_)
	, jpls_reset_(arg.jpls_reset_)
	, jpls_limit_(arg.jpls_limit_)
	*/
	, fragmentSize_(arg.fragmentSize_)
	, createOffsetTable_(arg.createOffsetTable_)
//	, preferCookedEncoding_(arg.preferCookedEncoding_)
	, uidCreation_(arg.uidCreation_)
	, convertToSC_(arg.convertToSC_)
//	, jplsInterleaveMode_(arg.jplsInterleaveMode_)
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
