/************************************************************************
 *
 *	Copyright (c) 2007-2012, Samuel W. Choi
 *	All Rights Reserved
 *
 *	$Workfile: DcmService.cpp $
 *	$Author: Pcc7510 $
 *	$Date: 08-10-08 11:28a $
 *	$Revision: 6 $
 *
 ************************************************************************/

#include "DTK.h"
#include "DTKinternal.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

ServiceList	Service::DcmBasicSCUServiceList;
ServiceList	Service::DcmBasicSCPServiceList;

void Service::addBasicTransferSyntaxList()
{
	_transferSyntaxList.push_back(EXS_LittleEndianExplicit);
	_transferSyntaxList.push_back(EXS_BigEndianExplicit);
	_transferSyntaxList.push_back(EXS_LittleEndianImplicit);
}

void Service::addVerificationService(ServiceList& serviceList)
{
	Service dcmService;
	dcmService._abstractSyntax = UID_VerificationSOPClass;
	dcmService.addBasicTransferSyntaxList();

	serviceList.push_back(dcmService);
}

void Service::addStorageSCUServices(ServiceList& serviceList)
{
	for(int nIndex = 0; nIndex < numberOfDcmShortSCUStorageSOPClassUIDs; nIndex++)
	{
		Service dcmService;
		dcmService._abstractSyntax = dcmShortSCUStorageSOPClassUIDs[nIndex];
		dcmService.addBasicTransferSyntaxList();

		serviceList.push_back(dcmService);
	}
}

void Service::addStorageSCPServices(ServiceList& serviceList)
{
	for(int nIndex = 0; nIndex < numberOfDcmShortSCUStorageSOPClassUIDs; nIndex++)
	{
		Service dcmService;
		dcmService._abstractSyntax = dcmShortSCUStorageSOPClassUIDs[nIndex];
		dcmService.addBasicTransferSyntaxList();

		serviceList.push_back(dcmService);
	}
}

void Service::addStorageCommitmentSCUService(ServiceList& serviceList)
{
	Service dcmService;
	dcmService._role = ASC_SC_ROLE_SCU;
	dcmService._abstractSyntax = UID_StorageCommitmentPushModelSOPClass;
	dcmService.addBasicTransferSyntaxList();

	serviceList.push_back(dcmService);
}

void Service::addStorageCommitmentSCPService(ServiceList& serviceList)
{
	Service dcmService;
	dcmService._role = ASC_SC_ROLE_SCP;
	dcmService._abstractSyntax = UID_StorageCommitmentPushModelSOPClass;
	dcmService.addBasicTransferSyntaxList();

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
		dcmService.addBasicTransferSyntaxList();

		serviceList.push_back(dcmService);
	}
}

void Service::addMWLService(ServiceList& serviceList)
{
	Service dcmService;
	dcmService._abstractSyntax = UID_FINDModalityWorklistInformationModel;
	dcmService.addBasicTransferSyntaxList();

	serviceList.push_back(dcmService);
}

void Service::addMPPSService(ServiceList& serviceList)
{
	Service dcmService;
	dcmService._abstractSyntax = UID_ModalityPerformedProcedureStepSOPClass;
	dcmService.addBasicTransferSyntaxList();

	serviceList.push_back(dcmService);
}

void Service::addGrayscalePrintService(ServiceList& serviceList)
{
	Service dcmService;
	dcmService._abstractSyntax = UID_BasicGrayscalePrintManagementMetaSOPClass;
	dcmService.addBasicTransferSyntaxList();

	serviceList.push_back(dcmService);
}

void Service::addColorPrintService(ServiceList& serviceList)
{
	Service dcmService;
	dcmService._abstractSyntax = UID_BasicColorPrintManagementMetaSOPClass;
	dcmService.addBasicTransferSyntaxList();

	serviceList.push_back(dcmService);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

class ServiceInit
{
public:
	ServiceInit(void);
};

ServiceInit::ServiceInit(void)
{
	if (DCM_BASIC_SCU_SERVICE_LIST.size() == 0)
	{
		Service::addVerificationService(DCM_BASIC_SCU_SERVICE_LIST);

		Service::addStorageSCUServices(DCM_BASIC_SCU_SERVICE_LIST);
		Service::addStorageCommitmentSCUService(DCM_BASIC_SCU_SERVICE_LIST);

		Service::addQueryRetrieveService(DCM_BASIC_SCU_SERVICE_LIST);
		Service::addMWLService(DCM_BASIC_SCU_SERVICE_LIST);
		Service::addMPPSService(DCM_BASIC_SCU_SERVICE_LIST);

		Service::addGrayscalePrintService(DCM_BASIC_SCU_SERVICE_LIST);
		Service::addColorPrintService(DCM_BASIC_SCU_SERVICE_LIST);
	}

	if (DCM_BASIC_SCP_SERVICE_LIST.size() == 0)
	{
		Service::addVerificationService(DCM_BASIC_SCU_SERVICE_LIST);

		Service::addStorageSCPServices(DCM_BASIC_SCU_SERVICE_LIST);
		Service::addStorageCommitmentSCPService(DCM_BASIC_SCU_SERVICE_LIST);

		Service::addQueryRetrieveService(DCM_BASIC_SCU_SERVICE_LIST);
		Service::addMWLService(DCM_BASIC_SCU_SERVICE_LIST);
		Service::addMPPSService(DCM_BASIC_SCU_SERVICE_LIST);

		Service::addGrayscalePrintService(DCM_BASIC_SCU_SERVICE_LIST);
		Service::addColorPrintService(DCM_BASIC_SCU_SERVICE_LIST);
	}
}

static ServiceInit _dummy;
