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

ServiceList ServiceList::_basicSCUServiceList;
ServiceList ServiceList::_basicSCPServiceList;

ServiceList::ServiceList(bool includeEcho)
{
	if (includeEcho)
		addVerificationService();
}

bool ServiceList::hasService(const String& abstractSyntax, TransferSyntax transferSyntax)
{
	for(ServiceListType::const_iterator i = _serviceList.cbegin(); i != _serviceList.cend(); i++) {
		const Service* service = &(*i);
		if (service->_abstractSyntax.compare(abstractSyntax) != 0)
			continue;

		if (service->_transferSyntaxList.size() == 0)
			return false;

		const TransferSyntax ts = *(service->_transferSyntaxList.cbegin());
		if (ts == transferSyntax)
			return true;
	}

	return false;
}

bool ServiceList::hasService(const String& abstractSyntax, const String& transferSyntaxUID)
{
	return hasService(abstractSyntax, DcmXfer(transferSyntaxUID.c_str()).getXfer());
}

void ServiceList::addService(const String& abstractSyntax, TransferSyntax transferSyntax)
{
	Service service;
	service._abstractSyntax = abstractSyntax;
	service._transferSyntaxList.push_back(transferSyntax);

	_serviceList.push_back(service);
}

void ServiceList::addService(const String& abstractSyntax, const String& transferSyntaxUID)
{
	Service service;
	service._abstractSyntax = abstractSyntax;
	service._transferSyntaxList.push_back(DcmXfer(transferSyntaxUID.c_str()).getXfer());

	_serviceList.push_back(service);
}

void ServiceList::addService(const String& abstractSyntax, const TransferSyntaxList& transferSyntaxList)
{
	Service service;
	service._abstractSyntax = abstractSyntax;
	service._transferSyntaxList.insert(service._transferSyntaxList.end(), transferSyntaxList.begin(), transferSyntaxList.end());

	_serviceList.push_back(service);
}

void ServiceList::addService(const String& abstractSyntax, const StringList& transferSyntaxUIDList)
{
	Service service;
	service._abstractSyntax = abstractSyntax;
	for(StringList::const_iterator i = transferSyntaxUIDList.cbegin(); i != transferSyntaxUIDList.cend(); i++) {
		service._transferSyntaxList.push_back(DcmXfer(i->c_str()).getXfer());
	}

	_serviceList.push_back(service);
}

void ServiceList::addVerificationService()
{
	Service service;
	service._abstractSyntax = UID_VerificationSOPClass;
	service.addBasicTransferSyntaxes();

	_serviceList.push_back(service);
}

void ServiceList::addAllStorageSCUServices()
{
	for(int nIndex = 0; nIndex < numberOfDcmShortSCUStorageSOPClassUIDs; nIndex++)
	{
		Service service;
		service._abstractSyntax = dcmShortSCUStorageSOPClassUIDs[nIndex];
		service.addBasicTransferSyntaxes();

		_serviceList.push_back(service);
	}
}

void ServiceList::addAllStorageSCPServices()
{
	for(int nIndex = 0; nIndex < numberOfDcmShortSCUStorageSOPClassUIDs; nIndex++)
	{
		Service service;
		service._abstractSyntax = dcmShortSCUStorageSOPClassUIDs[nIndex];
		service.addBasicTransferSyntaxes();

		_serviceList.push_back(service);
	}
}

void ServiceList::addAllQueryRetrieveServices()
{
	const char* qrModels[6] = {
		UID_FINDPatientRootQueryRetrieveInformationModel,
		UID_MOVEPatientRootQueryRetrieveInformationModel,
		UID_GETPatientRootQueryRetrieveInformationModel,
		UID_FINDStudyRootQueryRetrieveInformationModel,
		UID_MOVEStudyRootQueryRetrieveInformationModel,
		UID_GETStudyRootQueryRetrieveInformationModel
	};

	for(int nIndex = 0; nIndex < 6; nIndex++)
	{
		Service service;
		service._abstractSyntax = qrModels[nIndex];
		service.addBasicTransferSyntaxes();

		_serviceList.push_back(service);
	}
}

void ServiceList::addMWLService()
{
	Service service;
	service._abstractSyntax = UID_FINDModalityWorklistInformationModel;
	service.addBasicTransferSyntaxes();

	_serviceList.push_back(service);
}

void ServiceList::addMPPSService()
{
	Service service;
	service._abstractSyntax = UID_ModalityPerformedProcedureStepSOPClass;
	service.addBasicTransferSyntaxes();

	_serviceList.push_back(service);
}

void ServiceList::addStorageCommitmentSCUService()
{
	Service service;
	service._role = ASC_SC_ROLE_SCU;
	service._abstractSyntax = UID_StorageCommitmentPushModelSOPClass;
	service.addBasicTransferSyntaxes();

	_serviceList.push_back(service);
}

void ServiceList::addStorageCommitmentSCPService()
{
	Service service;
	service._role = ASC_SC_ROLE_SCP;
	service._abstractSyntax = UID_StorageCommitmentPushModelSOPClass;
	service.addBasicTransferSyntaxes();

	_serviceList.push_back(service);
}

void ServiceList::addGrayscalePrintService()
{
	Service service;
	service._abstractSyntax = UID_BasicGrayscalePrintManagementMetaSOPClass;
	service.addBasicTransferSyntaxes();

	_serviceList.push_back(service);
}

void ServiceList::addColorPrintService()
{
	Service service;
	service._abstractSyntax = UID_BasicColorPrintManagementMetaSOPClass;
	service.addBasicTransferSyntaxes();

	_serviceList.push_back(service);
}

const ServiceList& ServiceList::getBasicSCUServiceList()
{
	if (_basicSCUServiceList.size() == 0) {
		_basicSCUServiceList.addVerificationService();
	}
	if (_basicSCUServiceList.size() == 1) {
		_basicSCUServiceList.addAllStorageSCUServices();
		_basicSCUServiceList.addAllQueryRetrieveServices();

		_basicSCUServiceList.addMWLService();
		_basicSCUServiceList.addMPPSService();
		_basicSCUServiceList.addStorageCommitmentSCUService();

		_basicSCUServiceList.addGrayscalePrintService();
		_basicSCUServiceList.addColorPrintService();
	}

	return _basicSCUServiceList;
}

const ServiceList& ServiceList::getBasicSCPServiceList()
{
	if (_basicSCPServiceList.size() == 0) {
		_basicSCPServiceList.addVerificationService();
	}
	if (_basicSCPServiceList.size() == 1) {
		_basicSCPServiceList.addAllStorageSCPServices();
		_basicSCPServiceList.addAllQueryRetrieveServices();

		_basicSCPServiceList.addMWLService();
		_basicSCPServiceList.addMPPSService();
		_basicSCPServiceList.addStorageCommitmentSCPService();

		_basicSCPServiceList.addGrayscalePrintService();
		_basicSCPServiceList.addColorPrintService();
	}

	return _basicSCPServiceList;
}

ServiceListType::size_type ServiceList::size() const
{
	return _serviceList.size();
}

ServiceListType::const_iterator ServiceList::cbegin() const
{
	return _serviceList.cbegin();
}

ServiceListType::const_iterator ServiceList::cend() const
{
	return _serviceList.cend();
}

