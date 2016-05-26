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

#include "openjpeg.h"

// initialization of static members
OFBool DJP2KEncoderRegistration::registered_ = OFFalse;
DJP2KCodecParameter *DJP2KEncoderRegistration::cp_ = NULL;
DJP2KLosslessEncoder *DJP2KEncoderRegistration::losslessencoder_ = NULL;
DJP2KLossyEncoder *DJP2KEncoderRegistration::lossyencoder_ = NULL;

void DJP2KEncoderRegistration::registerCodecs(
	/*
	OFBool jpls_optionsEnabled,
	Uint16 jpls_t1,
	Uint16 jpls_t2,
	Uint16 jpls_t3,
	Uint16 jpls_reset,
	Uint16 jpls_limit,
	OFBool preferCookedEncoding,
	*/
	Uint32 fragmentSize,
	OFBool createOffsetTable,
	JP2K_UIDCreation uidCreation,
	OFBool convertToSC//,
/*	DJP2KCodecParameter::interleaveMode jplsInterleaveMode*/)
{
	if (!registered_)
	{
		cp_ = new DJP2KCodecParameter(/*jpls_optionsEnabled, jpls_t1, jpls_t2, jpls_t3, jpls_reset,
			jpls_limit, preferCookedEncoding,*/ fragmentSize, createOffsetTable, uidCreation,
			convertToSC, EJ2KPC_restore, OFFalse/*, jplsInterleaveMode*/);

		if (cp_)
		{
			losslessencoder_ = new DJP2KLosslessEncoder();
			if (losslessencoder_) DcmCodecList::registerCodec(losslessencoder_, NULL, cp_);
			lossyencoder_ = new DJP2KLossyEncoder();
			if (lossyencoder_) DcmCodecList::registerCodec(lossyencoder_, NULL, cp_);
			registered_ = OFTrue;
		}
	}
}

void DJP2KEncoderRegistration::cleanup()
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

OFString DJP2KEncoderRegistration::getLibraryVersionString()
{
	return opj_version();
}
