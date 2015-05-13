/************************************************************************
 *
 *	Copyright (c) 2007-2012, Samuel W. Choi
 *	All Rights Reserved
 *
 *	$Workfile: DcmAssocRequestor.cpp $
 *	$Author: Pcc7510 $
 *	$Date: 08-10-08 11:03a $
 *	$Revision: 9 $
 *
 ************************************************************************/

#include "DTK.h"
#include "DTKinternal.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

AssociationRequestor::AssociationRequestor(AppEntity* dcmAppEntityPtr)
	: Association(dcmAppEntityPtr)
{
}

AssociationRequestor::~AssociationRequestor(void)
{
	if (_ascParamsPtr)
		close();
}

Status AssociationRequestor::connect(const String& calledAETitle, const String& hostname, const Uint16 port, const ServiceList& serviceList)
{
//	CSingleLock sgl(&m_mtxAssoc, TRUE);	// FIXME

	char localAddress[256], remoteAddress[256];
	gethostname(localAddress, sizeof(localAddress)-1);
	sprintf(remoteAddress, "%s:%d", hostname.c_str(), port);

	OFCondition cond;
	cond = ASC_createAssociationParameters(&_ascParamsPtr, _appEntityPtr->_maxPDUSize);
	cond = ASC_setAPTitles(_ascParamsPtr, _appEntityPtr->getAETitle().c_str(), calledAETitle.c_str(), NULL);
	cond = ASC_setPresentationAddresses(_ascParamsPtr, localAddress, remoteAddress);

//	OnProposePresentationContext();

	int pcId = 1;
	for(ServiceList::const_iterator si = serviceList.begin(); si != serviceList.end(); si++)
	{
		int count = 0;
		const char** transferSyntaxes = new const char*[si->_transferSyntaxList.size()];
		for(TransferSyntaxList::const_iterator ti = si->_transferSyntaxList.begin(); ti != si->_transferSyntaxList.end(); ti++)
		{
			DcmXfer dcmXfer(*ti);
			transferSyntaxes[count++] = dcmXfer.getXferID();
		}
		cond = ASC_addPresentationContext(_ascParamsPtr, pcId, si->_abstractSyntax.c_str(), transferSyntaxes, count, si->_role);
		delete []transferSyntaxes;

		pcId += 2;
	}

	T_ASC_Network* ascNetworkPtr = reinterpret_cast<T_ASC_Network*>(_appEntityPtr->getInternal());
	cond = ASC_requestAssociation(ascNetworkPtr, _ascParamsPtr, &_ascAssocPtr);

//	int iCount = ASC_countAcceptedPresentationContexts(m_pAscParams);

	return cond;
}
/*
Status AssociationRequestor::connect(const QString& calledAETitle, const QString& hostname, const Uint16 port, const ServiceList& serviceList)
{
//	CSingleLock sgl(&m_mtxAssoc, TRUE);	// FIXME

	char localAddress[256], remoteAddress[256];
	gethostname(localAddress, sizeof(localAddress)-1);
	sprintf(remoteAddress, "%s:%d", hostname.toLatin1().constData(), port);

	OFCondition cond;
	cond = ASC_createAssociationParameters(&_ascParamsPtr, _appEntityPtr->_maxPDUSize);
	cond = ASC_setAPTitles(_ascParamsPtr, _appEntityPtr->getAETitle1().toLatin1().constData(), calledAETitle.toLatin1().constData(), NULL);
	cond = ASC_setPresentationAddresses(_ascParamsPtr, localAddress, remoteAddress);

//	OnProposePresentationContext();

	int pcId = 1;
	for(ServiceList::const_iterator si = serviceList.begin(); si != serviceList.end(); si++)
	{
		int count = 0;
		const char** transferSyntaxes = new const char*[si->_transferSyntaxList.size()];
		for(TransferSyntaxList::const_iterator ti = si->_transferSyntaxList.begin(); ti != si->_transferSyntaxList.end(); ti++)
		{
			DcmXfer dcmXfer(*ti);
			transferSyntaxes[count++] = dcmXfer.getXferID();
		}
		cond = ASC_addPresentationContext(_ascParamsPtr, pcId, si->_abstractSyntax.c_str(), transferSyntaxes, count, si->_role);
		delete []transferSyntaxes;

		pcId += 2;
	}

	T_ASC_Network* ascNetworkPtr = reinterpret_cast<T_ASC_Network*>(_appEntityPtr->getInternal());
	cond = ASC_requestAssociation(ascNetworkPtr, _ascParamsPtr, &_ascAssocPtr);

//	int iCount = ASC_countAcceptedPresentationContexts(m_pAscParams);

	return cond;
}
*/
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
