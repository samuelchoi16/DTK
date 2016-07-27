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
*  Purpose: singleton class that registers encoders for all supported JPEG processes.
*
*/

#include "dcmtk/config/osconfig.h"
#include "dcmjp2k/djp2kencode.h"
#include "dcmtk/dcmdata/dccodec.h"  /* for DcmCodecStruct */
#include "dcmjp2k/djp2kcparam.h"
#include "dcmjp2k/djp2kcodece.h"
#include "dcmjp2k/djp2kinternal.h"

#include "openjpeg.h"

// initialization of static members
OFBool EncoderRegistration::registered_ = OFFalse;
CodecParameter *EncoderRegistration::cp_ = NULL;
LosslessEncoder *EncoderRegistration::losslessencoder_ = NULL;
LossyEncoder *EncoderRegistration::lossyencoder_ = NULL;

void EncoderRegistration::registerCodecs(
	OFBool jp2k_optionsEnabled,
	Uint32 fragmentSize,
	OFBool createOffsetTable,
	UIDCreation uidCreation,
	OFBool convertToSC)
{
	if (!registered_)
	{
		cp_ = new CodecParameter(jp2k_optionsEnabled, fragmentSize, createOffsetTable, uidCreation, convertToSC, EJ2KPC_restore, OFFalse);

		if (cp_)
		{
			losslessencoder_ = new LosslessEncoder();
			if (losslessencoder_) DcmCodecList::registerCodec(losslessencoder_, NULL, cp_);
			lossyencoder_ = new LossyEncoder();
			if (lossyencoder_) DcmCodecList::registerCodec(lossyencoder_, NULL, cp_);
			registered_ = OFTrue;
		}
	}
}

void EncoderRegistration::cleanup()
{
	if (registered_)
	{
		DcmCodecList::deregisterCodec(losslessencoder_);
		DcmCodecList::deregisterCodec(lossyencoder_);
		delete losslessencoder_;
		delete lossyencoder_;
		delete cp_;
		registered_ = OFFalse;
#ifdef DEBUG
		// not needed but useful for debugging purposes
		losslessencoder_ = NULL;
		lossyencoder_ = NULL;
		cp_ = NULL;
#endif
	}
}

OFString EncoderRegistration::getLibraryVersionString()
{
	return opj_version();
}
