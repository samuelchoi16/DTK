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

#include "openjpeg.h"

// initialization of static members
OFBool DJP2KDecoderRegistration::registered_ = OFFalse;
DJP2KCodecParameter *DJP2KDecoderRegistration::cp_ = NULL;
DJP2KLosslessDecoder *DJP2KDecoderRegistration::losslessdecoder_ = NULL;
DJP2KLossyDecoder *DJP2KDecoderRegistration::lossydecoder_ = NULL;

void DJP2KDecoderRegistration::registerCodecs(
	JP2K_UIDCreation uidcreation,
	JP2K_PlanarConfiguration planarconfig,
	OFBool ignoreOffsetTable)
{
	if (!registered_)
	{
		cp_ = new DJP2KCodecParameter(uidcreation, planarconfig, ignoreOffsetTable);
		if (cp_)
		{
			losslessdecoder_ = new DJP2KLosslessDecoder();
			if (losslessdecoder_) DcmCodecList::registerCodec(losslessdecoder_, NULL, cp_);

			lossydecoder_ = new DJP2KLossyDecoder();
			if (lossydecoder_) DcmCodecList::registerCodec(lossydecoder_, NULL, cp_);
			registered_ = OFTrue;
		}
	}
}

void DJP2KDecoderRegistration::cleanup()
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

OFString DJP2KDecoderRegistration::getLibraryVersionString()
{
	return opj_version();
}
