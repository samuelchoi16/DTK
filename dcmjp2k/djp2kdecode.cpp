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
*  Author:  Martin Willkomm
*
*  Purpose: singleton class that registers decoders for all supported JPEG 2000 processes.
*
*/

#include "dcmtk/config/osconfig.h"
#include "dcmjp2k/djp2kdecode.h"
#include "dcmtk/dcmdata/dccodec.h"  /* for DcmCodecStruct */
#include "dcmjp2k/djp2kcparam.h"
#include "dcmjp2k/djp2kcodecd.h"
#include "dcmjp2k/djp2kinternal.h"

#include "openjpeg.h"

// initialization of static members
OFBool DecoderRegistration::registered_ = OFFalse;
CodecParameter *DecoderRegistration::cp_ = NULL;
LosslessDecoder *DecoderRegistration::losslessdecoder_ = NULL;
LossyDecoder *DecoderRegistration::lossydecoder_ = NULL;

void DecoderRegistration::registerCodecs(
	UIDCreation uidcreation,
	PlanarConfiguration planarconfig,
	OFBool ignoreOffsetTable)
{
	if (!registered_)
	{
		cp_ = new CodecParameter(uidcreation, planarconfig, ignoreOffsetTable);
		if (cp_)
		{
			losslessdecoder_ = new LosslessDecoder();
			if (losslessdecoder_) DcmCodecList::registerCodec(losslessdecoder_, NULL, cp_);

			lossydecoder_ = new LossyDecoder();
			if (lossydecoder_) DcmCodecList::registerCodec(lossydecoder_, NULL, cp_);
			registered_ = OFTrue;
		}
	}
}

void DecoderRegistration::cleanup()
{
	if (registered_)
	{
		DcmCodecList::deregisterCodec(losslessdecoder_);
		DcmCodecList::deregisterCodec(lossydecoder_);
		delete losslessdecoder_;
		delete lossydecoder_;
		delete cp_;
		registered_ = OFFalse;
#ifdef DEBUG
		// not needed but useful for debugging purposes
		losslessdecoder_ = NULL;
		lossydecoder_ = NULL;
		cp_ = NULL;
#endif
	}
}

OFString DecoderRegistration::getLibraryVersionString()
{
	return opj_version();
}
