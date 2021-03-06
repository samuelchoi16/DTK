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
#include <WinSock2.h>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

Logger AssociationRequestor::_logger = Logger::getInstance("dcm.AssociationRequestor");

AssociationRequestor::AssociationRequestor(AppEntity* appEntity)
	: Association(appEntity)
{
}

AssociationRequestor::~AssociationRequestor(void)
{
	if (_ascParams)
		close();
}

Status AssociationRequestor::connect(const String& calledAETitle, const String& hostname, const Uint16 port, const ServiceList& serviceList)
{
	QMutexLocker locker(&_assocMutex);

	char localAddress[256], remoteAddress[256];
	gethostname(localAddress, sizeof(localAddress)-1);
	sprintf(remoteAddress, "%s:%d", hostname.c_str(), port);

	Status stat;
	stat = ASC_createAssociationParameters(&_ascParams, _appEntity->_maxPDUSize);
	stat = ASC_setAPTitles(_ascParams, _appEntity->getAETitle().c_str(), calledAETitle.c_str(), NULL);
	stat = ASC_setPresentationAddresses(_ascParams, localAddress, remoteAddress);

//	OnProposePresentationContext();

	int pcId = 1;
	for(ServiceListType::const_iterator si = serviceList.cbegin(); si != serviceList.cend(); si++) {
		int count = 0;
		const char** transferSyntaxes = new const char*[si->_transferSyntaxList.size()];
		for(TransferSyntaxList::const_iterator ti = si->_transferSyntaxList.begin(); ti != si->_transferSyntaxList.end(); ti++) {
			DcmXfer dcmXfer(*ti);
			transferSyntaxes[count++] = dcmXfer.getXferID();
		}
		stat = ASC_addPresentationContext(_ascParams, pcId, si->_abstractSyntax.c_str(), transferSyntaxes, count, si->_role);
		delete []transferSyntaxes;

		pcId += 2;
	}

	T_ASC_Network* ascNetwork = reinterpret_cast<T_ASC_Network*>(_appEntity->getInternal());
	stat = ASC_requestAssociation(ascNetwork, _ascParams, &_ascAssoc);

//	int iCount = ASC_countAcceptedPresentationContexts(m_pAscParams);

	return stat;
}
/*
void CDcmAssocRequestor::OnProposePresentationContext(void)	// FIXME...
{
	OFCondition cond;
	const char* szTransferSyntaxList[3];
	int nPCid = 1;

	szTransferSyntaxList[0] = UID_LittleEndianExplicitTransferSyntax;
	szTransferSyntaxList[1] = UID_BigEndianExplicitTransferSyntax;
	szTransferSyntaxList[2] = UID_LittleEndianImplicitTransferSyntax;

	cond = ASC_addPresentationContext(m_pAscParams, nPCid, UID_VerificationSOPClass, szTransferSyntaxList, 3);
	nPCid += 2;

	for (int nIndex = 0; nIndex < numberOfDcmShortSCUStorageSOPClassUIDs; nIndex++)
	{
		cond = ASC_addPresentationContext(m_pAscParams, nPCid, dcmShortSCUStorageSOPClassUIDs[nIndex], szTransferSyntaxList, 3);
		nPCid += 2;
	}

	cond = ASC_addPresentationContext(m_pAscParams, nPCid, UID_FINDModalityWorklistInformationModel, szTransferSyntaxList, 3);
	nPCid += 2;

	cond = ASC_addPresentationContext(m_pAscParams, nPCid, UID_ModalityPerformedProcedureStepSOPClass, szTransferSyntaxList, 3);
	nPCid += 2;

	cond = ASC_addPresentationContext(m_pAscParams, nPCid, UID_StorageCommitmentPushModelSOPClass, szTransferSyntaxList, 3, ASC_SC_ROLE_SCUSCP);
	nPCid += 2;

	cond = ASC_addPresentationContext(m_pAscParams, nPCid, UID_BasicGrayscalePrintManagementMetaSOPClass, szTransferSyntaxList, 3);
	nPCid += 2;

	cond = ASC_addPresentationContext(m_pAscParams, nPCid, UID_BasicColorPrintManagementMetaSOPClass, szTransferSyntaxList, 3);
	nPCid += 2;
}
*/
