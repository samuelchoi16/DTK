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

class DTKInit
{
public:
	DTKInit(void);
	virtual ~DTKInit(void);
};

DTKInit::DTKInit(void)
{
	if (DCM_BASIC_SCU_SERVICE_LIST.size() == 0) {
		Service::addVerificationService(DCM_BASIC_SCU_SERVICE_LIST);

		Service::addStorageSCUServices(DCM_BASIC_SCU_SERVICE_LIST);
		Service::addStorageCommitmentSCUService(DCM_BASIC_SCU_SERVICE_LIST);

		Service::addQueryRetrieveService(DCM_BASIC_SCU_SERVICE_LIST);
		Service::addMWLService(DCM_BASIC_SCU_SERVICE_LIST);
		Service::addMPPSService(DCM_BASIC_SCU_SERVICE_LIST);

		Service::addGrayscalePrintService(DCM_BASIC_SCU_SERVICE_LIST);
		Service::addColorPrintService(DCM_BASIC_SCU_SERVICE_LIST);
	}

	if (DCM_BASIC_SCP_SERVICE_LIST.size() == 0) {
		Service::addVerificationService(DCM_BASIC_SCP_SERVICE_LIST);

		Service::addStorageSCPServices(DCM_BASIC_SCP_SERVICE_LIST);
		Service::addStorageCommitmentSCPService(DCM_BASIC_SCP_SERVICE_LIST);

		Service::addQueryRetrieveService(DCM_BASIC_SCP_SERVICE_LIST);
		Service::addMWLService(DCM_BASIC_SCP_SERVICE_LIST);
		Service::addMPPSService(DCM_BASIC_SCP_SERVICE_LIST);

		Service::addGrayscalePrintService(DCM_BASIC_SCP_SERVICE_LIST);
		Service::addColorPrintService(DCM_BASIC_SCP_SERVICE_LIST);
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	{
		DcmRLEEncoderRegistration::registerCodecs();
		DJEncoderRegistration::registerCodecs();
		DJLSEncoderRegistration::registerCodecs();

		DcmRLEDecoderRegistration::registerCodecs();
		DJDecoderRegistration::registerCodecs();
		DJLSDecoderRegistration::registerCodecs();
	}
}

DTKInit::~DTKInit()
{
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	{
		DcmRLEEncoderRegistration::cleanup();
		DJEncoderRegistration::cleanup();
		DJLSEncoderRegistration::cleanup();

		DcmRLEDecoderRegistration::cleanup();
		DJDecoderRegistration::cleanup();
		DJLSDecoderRegistration::cleanup();
	}
}

static DTKInit _dummy;
