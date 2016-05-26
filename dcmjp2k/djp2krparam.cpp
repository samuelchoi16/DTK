/*
*
*  Copyright (C) 1997-2010, OFFIS e.V.
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
*  Author:  Martin Willkomm
*
*  Purpose: representation parameter for JPEG 2000
*
*/

#include "dcmtk/config/osconfig.h"
#include "dcmjp2k/djp2krparam.h"
#include "dcmtk/ofstd/ofstd.h"

DJP2KRepresentationParameter::DJP2KRepresentationParameter(Uint16 compressionRatio,
	OFBool losslessProcess)
	: DcmRepresentationParameter()
	, compressionRatio_(compressionRatio)
	, losslessProcess_(losslessProcess)
{
}

DJP2KRepresentationParameter::DJP2KRepresentationParameter(const DJP2KRepresentationParameter& arg)
	: DcmRepresentationParameter(arg)
	, compressionRatio_(arg.compressionRatio_)
	, losslessProcess_(arg.losslessProcess_)
{
}

DJP2KRepresentationParameter::~DJP2KRepresentationParameter()
{
}

DcmRepresentationParameter *DJP2KRepresentationParameter::clone() const
{
	return new DJP2KRepresentationParameter(*this);
}

const char *DJP2KRepresentationParameter::className() const
{
	return "DJP2KRepresentationParameter";
}

OFBool DJP2KRepresentationParameter::operator==(const DcmRepresentationParameter &arg) const
{
	const char *argname = arg.className();
	if (argname)
	{
		OFString argstring(argname);
		if (argstring == className())
		{
			const DJP2KRepresentationParameter& argll = OFreinterpret_cast(const DJP2KRepresentationParameter &, arg);
			if (losslessProcess_ && argll.losslessProcess_) return OFTrue;
			else if (losslessProcess_ != argll.losslessProcess_) return OFFalse;
			else if (compressionRatio_ != argll.compressionRatio_) return OFFalse;
			return OFTrue;
		}
	}
	return OFFalse;
}
