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
#include <QThread>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

Logger AssociationListener::_logger = Logger::getInstance("dcm.AssociationListener");

AssociationListener::AssociationListener(AppEntity* appEntityPtr)
	: Association(appEntityPtr)
{
}

AssociationListener::~AssociationListener(void)
{
}

Status AssociationListener::listen(int timeout)
{
	QMutexLocker locker(&_assocMutex);
	OFCondition cond;
	T_ASC_Network* ascNetworkPtr = _appEntityPtr->getInternal();

	if (!ASC_associationWaiting(ascNetworkPtr, timeout))
		return DUL_NOASSOCIATIONREQUEST;

	DCMTK_LOG4CPLUS_DEBUG(_logger, "1b");
	if (timeout == -1)
		cond = ASC_receiveAssociation(ascNetworkPtr, &_ascAssocPtr, _maxPDUSize, NULL, NULL, 0, DUL_BLOCK);
	else
		cond = ASC_receiveAssociation(ascNetworkPtr, &_ascAssocPtr, _maxPDUSize, NULL, NULL, 0, DUL_NOBLOCK, timeout);

	DCMTK_LOG4CPLUS_DEBUG(_logger, "1c");
	if (cond.bad() /*&& cond == DUL_NOASSOCIATIONREQUEST*/) {
		cond = ASC_dropSCPAssociation(_ascAssocPtr);
		cond = ASC_destroyAssociation(&_ascAssocPtr);
		_ascParamsPtr = NULL;
		_ascAssocPtr  = NULL;

		return DUL_NOASSOCIATIONREQUEST;
	}

	ASC_setAPTitles(_ascAssocPtr->params, NULL, NULL, _appEntityPtr->getAETitle().c_str());

	if (_appEntityPtr->_checkApplicationContextName) {
		char buffer[BUFSIZ];
		cond = ASC_getApplicationContextName(_ascAssocPtr->params, buffer);
		if (cond.bad() || strcmp(buffer, UID_StandardApplicationContext) != 0) {
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

	if (_appEntityPtr->_checkImplementationClassUID) {
		if (strlen(_ascAssocPtr->params->theirImplementationClassUID) == 0) {
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
		cond = ASC_acceptContextsWithPreferredTransferSyntaxes(_ascAssocPtr->params, &abstractSyntaxUID, 1, transferSyntaxes, count, si->_role);
		delete[] transferSyntaxes;
	}

	cond = ASC_acknowledgeAssociation(_ascAssocPtr);

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

	OFCondition cond = ASC_rejectAssociation(_ascAssocPtr, &ascRejectParams);
	close();

	return cond;
}

Status AssociationListener::abort(void)
{
	QMutexLocker locker(&_assocMutex);

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

Logger AssociationListenerMaster::_logger = Logger::getInstance("dcm.AssociationListenerMaster");

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
	DCMTK_LOG4CPLUS_DEBUG(_logger, "startListen: starting.");

	start();

	_started = true;
	_stopped = false;
	_counter = 0;

	return EC_Normal;
}

Status AssociationListenerMaster::stoplisten(void)
{
	_started = false;

	while(!_stopped)
	{
		DCMTK_LOG4CPLUS_DEBUG(_logger, "stopListen: waiting for master listener to die...");
		QThread::sleep(1);
	}

	DCMTK_LOG4CPLUS_DEBUG(_logger, "stopListen: stopped.");

	return EC_Normal;
}

bool AssociationListenerMaster::isListening(void)
{
	return _started && !_stopped;
}

void AssociationListenerMaster::run(void)
{
	DCMTK_LOG4CPLUS_DEBUG(_logger, "run: started.");

	while(_started) {
		AssociationListenerWorker* workerPtr;
		Status stat = listen(1);
		if (stat == DUL_NOASSOCIATIONREQUEST) {
			DCMTK_LOG4CPLUS_TRACE_FMT(_logger, "run: listen() timeout");
			continue;
		}
		if (stat != EC_Normal) {
			DCMTK_LOG4CPLUS_DEBUG_FMT(_logger, "run: listen() returned 0x%04X. (%s)", stat.code(), stat.text());
			close();
			break;
		}
		workerPtr = clone();
		workerPtr->startlisten();
	}

	while(_counter > 0) {
		DCMTK_LOG4CPLUS_DEBUG_FMT(_logger, "run: waiting for slave listeners(%d) to die...", _counter);
		QThread::sleep(1);
	}

	_stopped = true;

	DCMTK_LOG4CPLUS_DEBUG(_logger, "run: ended.");
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

Logger AssociationListenerWorker::_logger = Logger::getInstance("dcm.AssociationListenerWorker");

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
	DCMTK_LOG4CPLUS_DEBUG(_logger, "startListen: starting.");

	start();
	return EC_Normal;
}

bool AssociationListenerWorker::isListening(void)
{
	return _masterPtr->isListening();
}

void AssociationListenerWorker::run(void)
{
	DCMTK_LOG4CPLUS_DEBUG(_logger, "run: started.");

	_masterPtr->_masterMutex.lock();
	_masterPtr->_counter++;
	_masterPtr->_masterMutex.unlock();

	onlisten();

	_masterPtr->_masterMutex.lock();
	_masterPtr->_counter--;
	_masterPtr->_masterMutex.unlock();

	delete this;

	DCMTK_LOG4CPLUS_DEBUG(_logger, "run: ended.");
}