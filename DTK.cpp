/*
 *
 *  Copyright (C) 2015, IRM Inc.
 *  All rights reserved.  See LICENSE file for details.
 *
 *  This software and supporting documentation were developed by
 *		IRM Inc., Korea.
 *  through the contract with
 *		Seoul National University Bundang Hospital
 *  under the support of
 *		Ministry of Trade, Industry and Energy, Republic of Korea.
 *
 *  Author:  Samuel Choi (samuelchoi@irm.kr)
 *
 */

#include "DTK.h"
#include "DTKinternal.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////

class DTK
{
public:
	DTK(void);
	virtual ~DTK(void);
};

DTK::DTK(void)
{
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// register codecs
	{
		DcmRLEEncoderRegistration::registerCodecs();
		DJEncoderRegistration::registerCodecs();
		DJLSEncoderRegistration::registerCodecs();

		DcmRLEDecoderRegistration::registerCodecs();
		DJDecoderRegistration::registerCodecs();
		DJLSDecoderRegistration::registerCodecs();
	}
}

DTK::~DTK()
{
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// unregister codecs
	{
		DcmRLEEncoderRegistration::cleanup();
		DJEncoderRegistration::cleanup();
		DJLSEncoderRegistration::cleanup();

		DcmRLEDecoderRegistration::cleanup();
		DJDecoderRegistration::cleanup();
		DJLSDecoderRegistration::cleanup();
	}
}

static DTK _dummy;
