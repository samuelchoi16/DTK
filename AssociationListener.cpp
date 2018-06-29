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

Logger AssociationListener::_logger = Logger::getInstance("dcm.AssociationListener");

AssociationListener::AssociationListener(AppEntity* appEntity)
	: Association(appEntity)
{
}

AssociationListener::~AssociationListener(void)
{
}

Status AssociationListener::listen(int timeout)
{
	QMutexLocker locker(&_assocMutex);
	OFCondition cond;
	T_ASC_Network* ascNetworkPtr = _appEntity->getInternal();

	if (!ASC_associationWaiting(ascNetworkPtr, timeout))
		return DUL_NOASSOCIATIONREQUEST;

	if (timeout == -1)
		cond = ASC_receiveAssociation(ascNetworkPtr, &_ascAssoc, _maxPDUSize, NULL, NULL, 0, DUL_BLOCK);
	else
		cond = ASC_receiveAssociation(ascNetworkPtr, &_ascAssoc, _maxPDUSize, NULL, NULL, 0, DUL_NOBLOCK, timeout);

	if (cond.bad() /*&& cond == DUL_NOASSOCIATIONREQUEST*/) {
		cond = ASC_dropSCPAssociation(_ascAssoc);
		cond = ASC_destroyAssociation(&_ascAssoc);
		_ascParams = NULL;
		_ascAssoc  = NULL;

		return DUL_NOASSOCIATIONREQUEST;
	}

	ASC_setAPTitles(_ascAssoc->params, NULL, NULL, _appEntity->getAETitle().c_str());

	if (_appEntity->_checkApplicationContextName) {
		char buffer[BUFSIZ];
		cond = ASC_getApplicationContextName(_ascAssoc->params, buffer);
		if (cond.bad() || strcmp(buffer, UID_StandardApplicationContext) != 0) {
			T_ASC_RejectParameters ascRejectParams = {
				ASC_RESULT_REJECTEDPERMANENT,
				ASC_SOURCE_SERVICEUSER,
				ASC_REASON_SU_APPCONTEXTNAMENOTSUPPORTED
			};

			cond = ASC_rejectAssociation(_ascAssoc, &ascRejectParams);
			close();

			return DUL_ASSOCIATIONREJECTED;
		}
	}

	if (_appEntity->_checkImplementationClassUID) {
		if (strlen(_ascAssoc->params->theirImplementationClassUID) == 0) {
			T_ASC_RejectParameters ascRejectParams = {
				ASC_RESULT_REJECTEDPERMANENT,
				ASC_SOURCE_SERVICEUSER,
				ASC_REASON_SU_NOREASON
			};

			cond = ASC_rejectAssociation(_ascAssoc, &ascRejectParams);
			close();

			return DUL_ASSOCIATIONREJECTED;
		}
	}

	return EC_Normal;
}

Status AssociationListener::accept(const ServiceList& serviceList)
{
	QMutexLocker locker(&_assocMutex);
	OFCondition cond;

//	OnAcceptPresentationContext();

	for(ServiceListType::const_iterator si = serviceList.cbegin(); si != serviceList.cend(); si++) {
		const char* abstractSyntaxUID = si->_abstractSyntax.c_str();
		int count = 0;
		const char** transferSyntaxes = new const char*[si->_transferSyntaxList.size()];

		for(TransferSyntaxList::const_iterator ti = si->_transferSyntaxList.begin(); ti != si->_transferSyntaxList.end(); ti++) {
			DcmXfer dcmXfer(*ti);
			transferSyntaxes[count++] = dcmXfer.getXferID();
		}
		cond = ASC_acceptContextsWithPreferredTransferSyntaxes(_ascAssoc->params, &abstractSyntaxUID, 1, transferSyntaxes, count, si->_role);
		delete[] transferSyntaxes;
	}

	cond = ASC_acknowledgeAssociation(_ascAssoc);

	return cond;
}

Status AssociationListener::reject(void)
{
	QMutexLocker locker(&_assocMutex);

	T_ASC_RejectParameters ascRejectParams = {
		ASC_RESULT_REJECTEDPERMANENT,
		ASC_SOURCE_SERVICEUSER,
		ASC_REASON_SU_NOREASON
	};

	OFCondition cond = ASC_rejectAssociation(_ascAssoc, &ascRejectParams);
	close();

	return cond;
}

Status AssociationListener::abort(void)
{
	QMutexLocker locker(&_assocMutex);

	OFCondition cond = ASC_abortAssociation(_ascAssoc);
	close();

	return cond;
}
/*
void CDcmAssocListener::OnAcceptPresentationContext(void)	// FIXME...
{
	OFCondition cond;
	const char* szTransferSyntaxList[3];

	szTransferSyntaxList[0] = UID_LittleEndianExplicitTransferSyntax;
	szTransferSyntaxList[1] = UID_BigEndianExplicitTransferSyntax;
	szTransferSyntaxList[2] = UID_LittleEndianImplicitTransferSyntax;

	const char* dcmAdditionalSOPClassUIDs[] =
	{
		UID_VerificationSOPClass,
		UID_FINDModalityWorklistInformationModel,
		UID_ModalityPerformedProcedureStepSOPClass,
		UID_BasicGrayscalePrintManagementMetaSOPClass,
		UID_BasicColorPrintManagementMetaSOPClass,
	};
	const char* dcmStorageCommitSOPClassUID[] =
	{
		UID_StorageCommitmentPushModelSOPClass,
	};

	cond = ASC_acceptContextsWithPreferredTransferSyntaxes(m_pAscAssoc->params, dcmAllStorageSOPClassUIDs, numberOfAllDcmStorageSOPClassUIDs, szTransferSyntaxList, 3);
	cond = ASC_acceptContextsWithPreferredTransferSyntaxes(m_pAscAssoc->params, dcmAdditionalSOPClassUIDs, DIM_OF(dcmAdditionalSOPClassUIDs), szTransferSyntaxList, 3);
	cond = ASC_acceptContextsWithPreferredTransferSyntaxes(m_pAscAssoc->params, dcmStorageCommitSOPClassUID, DIM_OF(dcmStorageCommitSOPClassUID), szTransferSyntaxList, 3, ASC_SC_ROLE_SCUSCP);
}
*/
