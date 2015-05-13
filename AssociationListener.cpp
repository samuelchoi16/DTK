/************************************************************************
 *
 *	Copyright (c) 2007-2012, Samuel W. Choi
 *	All Rights Reserved
 *
 *	$Workfile: DcmAssocListener.cpp $
 *	$Author: Pcc7510 $
 *	$Date: 08-10-08 11:03a $
 *	$Revision: 11 $
 *
 ************************************************************************/

#include "DTK.h"
#include "DTKinternal.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

AssociationListener::AssociationListener(AppEntity* appEntityPtr)
	: Association(appEntityPtr)
{
}

AssociationListener::~AssociationListener(void)
{
}

Status AssociationListener::listen(int timeout)
{
//	CSingleLock sgl(&m_mtxAssoc, TRUE);	// FIXME
	OFCondition cond, cond2;
	T_ASC_Network* ascNetworkPtr = reinterpret_cast<T_ASC_Network*>(_appEntityPtr->getInternal());

	if (!ASC_associationWaiting(ascNetworkPtr, timeout))
		return DUL_NOASSOCIATIONREQUEST;

	if (timeout == -1)
		cond = ASC_receiveAssociation(ascNetworkPtr, &_ascAssocPtr, _maxPDUSize, NULL, NULL, 0, DUL_BLOCK);
	else
		cond = ASC_receiveAssociation(ascNetworkPtr, &_ascAssocPtr, _maxPDUSize, NULL, NULL, 0, DUL_NOBLOCK, timeout);

	if (cond.bad() && cond == DUL_NOASSOCIATIONREQUEST)
	{
		cond = ASC_dropSCPAssociation(_ascAssocPtr);
		cond = ASC_destroyAssociation(&_ascAssocPtr);
		_ascParamsPtr = NULL;
		_ascAssocPtr  = NULL;

		return DUL_NOASSOCIATIONREQUEST;
	}

	ASC_setAPTitles(_ascAssocPtr->params, NULL, NULL, _appEntityPtr->getAETitle().c_str());

	if (_appEntityPtr->_checkApplicationContextName)
	{
		char buffer[BUFSIZ];
		cond = ASC_getApplicationContextName(_ascAssocPtr->params, buffer);
		if (cond.bad() || strcmp(buffer, UID_StandardApplicationContext) != 0)
		{
			T_ASC_RejectParameters ascRejectParams = {
				ASC_RESULT_REJECTEDPERMANENT,
				ASC_SOURCE_SERVICEUSER,
				ASC_REASON_SU_APPCONTEXTNAMENOTSUPPORTED
			};

			cond = ASC_rejectAssociation(_ascAssocPtr, &ascRejectParams);
			close();

			return DUL_ASSOCIATIONREJECTED;
		}
	}

	if (_appEntityPtr->_checkImplementationClassUID)
	{
		if (strlen(_ascAssocPtr->params->theirImplementationClassUID) == 0)
		{
			T_ASC_RejectParameters ascRejectParams = {
				ASC_RESULT_REJECTEDPERMANENT,
				ASC_SOURCE_SERVICEUSER,
				ASC_REASON_SU_NOREASON
			};

			cond = ASC_rejectAssociation(_ascAssocPtr, &ascRejectParams);
			close();

			return DUL_ASSOCIATIONREJECTED;
		}
	}

	return EC_Normal;
}

Status AssociationListener::accept(const ServiceList& dcmServiceList)
{
//	CSingleLock sgl(&m_mtxAssoc, TRUE);
	OFCondition cond;

//	OnAcceptPresentationContext();

	for(ServiceList::const_iterator si = dcmServiceList.begin(); si != dcmServiceList.end(); si++)
	{
		const char* abstractSyntaxUID = si->_abstractSyntax.c_str();
		int count = 0;
		const char** transferSyntaxes = new const char*[si->_transferSyntaxList.size()];

		for(TransferSyntaxList::const_iterator ti = si->_transferSyntaxList.begin(); ti != si->_transferSyntaxList.end(); ti++)
		{
			DcmXfer dcmXfer(*ti);
			transferSyntaxes[count++] = dcmXfer.getXferID();
		}
		cond = ASC_acceptContextsWithPreferredTransferSyntaxes(_ascAssocPtr->params, &abstractSyntaxUID, 1, transferSyntaxes, count, si->_role);
		delete[] transferSyntaxes;
	}

	cond = ASC_acknowledgeAssociation(_ascAssocPtr);

	return cond;
}

Status AssociationListener::reject(void)
{
//	CSingleLock sgl(&m_mtxAssoc, TRUE);	// FIXME

	T_ASC_RejectParameters ascRejectParams = {
		ASC_RESULT_REJECTEDPERMANENT,
		ASC_SOURCE_SERVICEUSER,
		ASC_REASON_SU_NOREASON
	};

	OFCondition cond = ASC_rejectAssociation(_ascAssocPtr, &ascRejectParams);
	close();

	return cond;
}

Status AssociationListener::abort(void)
{
//	CSingleLock sgl(&m_mtxAssoc, TRUE);	// FIXME

	OFCondition cond = ASC_abortAssociation(_ascAssocPtr);
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
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

AssociationListenerMaster::AssociationListenerMaster(AppEntity* appEntityPtr)
	: AssociationListener(appEntityPtr)
{
	_started = false;
	_stopped = true;
}

AssociationListenerMaster::~AssociationListenerMaster(void)
{
}

Status AssociationListenerMaster::startlisten(void)
{
//	LOG_MESSAGE(4, LOG_DEBUG, _T("AssocMaster.startListen: starting."));

	start();

	_started = TRUE;
	_stopped = FALSE;
	_counter = 0;

	return EC_Normal;
}

Status AssociationListenerMaster::stoplisten(void)
{
	_started = FALSE;

	while(!_stopped)
	{
//		LOG_MESSAGE(4, LOG_DEBUG, _T("AssocMaster.stopListen: waiting for master listener to die..."));
#ifdef  WIN32
        Sleep(1000);
#else
        sleep(1000);
#endif
	}

//	LOG_MESSAGE(4, LOG_DEBUG, _T("AssocMaster.stopListen: stopped."));

	return EC_Normal;
}

bool AssociationListenerMaster::isListening(void)
{
	return _started && !_stopped;
}

void AssociationListenerMaster::run(void)
{
//	LOG_MESSAGE(4, LOG_DEBUG, _T("AssocMaster.run: started."));

	while(_started)
	{
		AssociationListenerWorker* workerPtr;
		Status dcmStat = listen(1);
		if (dcmStat == DUL_NOASSOCIATIONREQUEST)
			continue;
		if (dcmStat != EC_Normal)
		{
//			LOG_MESSAGE(4, LOG_DEBUG, _T("AssocMaster.run: listen() returned 0x%04X. (%s)"), dcmStat.getCode(), dcmStat.getText());
			close();
			break;
		}
		workerPtr = clone();
		workerPtr->startlisten();
	}

	while(_counter > 0)
	{
//		LOG_MESSAGE(4, LOG_DEBUG, _T("AssocMaster.run: waiting for slave listeners(%d) to die..."), m_nCounter);
#ifdef  WIN32
        Sleep(1000);
#else
		sleep(1000);
#endif
	}

	_stopped = TRUE;

//	LOG_MESSAGE(4, LOG_DEBUG, _T("AssocMaster.run: ended."));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

AssociationListenerWorker::AssociationListenerWorker(AssociationListenerMaster* masterPtr)
	: AssociationListener(masterPtr->_appEntityPtr)
	, _masterPtr(masterPtr)
{
	_appEntityPtr = masterPtr->_appEntityPtr;
	_ascParamsPtr = masterPtr->_ascParamsPtr;
	_ascAssocPtr = masterPtr->_ascAssocPtr;
	_maxPDUSize = masterPtr->_maxPDUSize;
}

AssociationListenerWorker::~AssociationListenerWorker(void)
{
}

Status AssociationListenerWorker::startlisten(void)
{
//	LOG_MESSAGE(4, LOG_DEBUG, _T("AssocWorker.startListen: starting."));

	start();
	return EC_Normal;
}

bool AssociationListenerWorker::isListening(void)
{
	return _masterPtr->isListening();
}

void AssociationListenerWorker::run(void)
{
//	LOG_MESSAGE(4, LOG_DEBUG, _T("AssocWorker.run: started."));

//	_masterPtr->m_mtx.Lock();
	_masterPtr->_counter++;
//	_masterPtr->m_mtx.Unlock();

	onlisten();

//	_masterPtr->m_mtx.Lock();
	_masterPtr->_counter--;
//	_masterPtr->m_mtx.Unlock();

	delete this;

//	LOG_MESSAGE(4, LOG_DEBUG, _T("AssocWorker.run: ended."));
}
