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

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

//ServiceList	Service::DcmBasicSCUServiceList;
//ServiceList	Service::DcmBasicSCPServiceList;

void Service::addBasicTransferSyntaxes()
{
	_transferSyntaxList.push_back(EXS_LittleEndianExplicit);
	_transferSyntaxList.push_back(EXS_BigEndianExplicit);
	_transferSyntaxList.push_back(EXS_LittleEndianImplicit);
}
/*
void Service::addVerificationService(ServiceList& serviceList)
{
	Service dcmService;
	dcmService._abstractSyntax = UID_VerificationSOPClass;
	dcmService.addBasicTransferSyntaxes();

	serviceList.push_back(dcmService);
}

void Service::addStorageSCUServices(ServiceList& serviceList)
{
	for(int nIndex = 0; nIndex < numberOfDcmShortSCUStorageSOPClassUIDs; nIndex++)
	{
		Service dcmService;
		dcmService._abstractSyntax = dcmShortSCUStorageSOPClassUIDs[nIndex];
		dcmService.addBasicTransferSyntaxes();

		serviceList.push_back(dcmService);
	}
}

void Service::addStorageSCPServices(ServiceList& serviceList)
{
	for(int nIndex = 0; nIndex < numberOfDcmShortSCUStorageSOPClassUIDs; nIndex++)
	{
		Service dcmService;
		dcmService._abstractSyntax = dcmShortSCUStorageSOPClassUIDs[nIndex];
		dcmService.addBasicTransferSyntaxes();

		serviceList.push_back(dcmService);
	}
}

void Service::addStorageCommitmentSCUService(ServiceList& serviceList)
{
	Service dcmService;
	dcmService._role = ASC_SC_ROLE_SCU;
	dcmService._abstractSyntax = UID_StorageCommitmentPushModelSOPClass;
	dcmService.addBasicTransferSyntaxes();

	serviceList.push_back(dcmService);
}

void Service::addStorageCommitmentSCPService(ServiceList& serviceList)
{
	Service dcmService;
	dcmService._role = ASC_SC_ROLE_SCP;
	dcmService._abstractSyntax = UID_StorageCommitmentPushModelSOPClass;
	dcmService.addBasicTransferSyntaxes();

	serviceList.push_back(dcmService);
}

void Service::addQueryRetrieveService(ServiceList& serviceList)
{
	const char* dcmQRModels[6] = {
		UID_FINDPatientRootQueryRetrieveInformationModel,
		UID_MOVEPatientRootQueryRetrieveInformationModel,
		UID_GETPatientRootQueryRetrieveInformationModel,
		UID_FINDStudyRootQueryRetrieveInformationModel,
		UID_MOVEStudyRootQueryRetrieveInformationModel,
		UID_GETStudyRootQueryRetrieveInformationModel
	};

	for(int nIndex = 0; nIndex < 6; nIndex++)
	{
		Service dcmService;
		dcmService._abstractSyntax = dcmQRModels[nIndex];
		dcmService.addBasicTransferSyntaxes();

		serviceList.push_back(dcmService);
	}
}

void Service::addMWLService(ServiceList& serviceList)
{
	Service dcmService;
	dcmService._abstractSyntax = UID_FINDModalityWorklistInformationModel;
	dcmService.addBasicTransferSyntaxes();

	serviceList.push_back(dcmService);
}

void Service::addMPPSService(ServiceList& serviceList)
{
	Service dcmService;
	dcmService._abstractSyntax = UID_ModalityPerformedProcedureStepSOPClass;
	dcmService.addBasicTransferSyntaxes();

	serviceList.push_back(dcmService);
}

void Service::addGrayscalePrintService(ServiceList& serviceList)
{
	Service dcmService;
	dcmService._abstractSyntax = UID_BasicGrayscalePrintManagementMetaSOPClass;
	dcmService.addBasicTransferSyntaxes();

	serviceList.push_back(dcmService);
}

void Service::addColorPrintService(ServiceList& serviceList)
{
	Service dcmService;
	dcmService._abstractSyntax = UID_BasicColorPrintManagementMetaSOPClass;
	dcmService.addBasicTransferSyntaxes();

	serviceList.push_back(dcmService);
}
*/
