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

static void ProgressCallback(void *callbackContextPtr, Ulong byteCount);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

Logger Association::_logger = Logger::getInstance("dcm.Association");

Association::Association(AppEntity* appEntityPtr)
{
	_appEntityPtr = appEntityPtr;
	_ascParamsPtr = NULL;
	_ascAssocPtr = NULL;
	_maxPDUSize = appEntityPtr->_maxPDUSize;
}

Association::~Association(void)
{
}

Status Association::close()
{
	QMutexLocker locker(&_assocMutex);

	OFCondition cond;
	if (_ascAssocPtr) {
		cond = ASC_releaseAssociation(_ascAssocPtr);
		cond = ASC_destroyAssociation(&_ascAssocPtr);
		_ascParamsPtr = NULL;
		_ascAssocPtr = NULL;
	}
	return cond;
}

Status Association::sendMessage(dcm::Message& req)
{
	QMutexLocker locker(&_assocMutex);

	OFCondition cond;
	T_ASC_PresentationContextID& pcId = req._pcId;
	T_DIMSE_Message* dimsePtr = &req;
	DcmDataset* statusDatasetPtr = NULL;

	switch(dimsePtr->CommandField) {
	case DIMSE_C_ECHO_RQ :
	case DIMSE_C_STORE_RQ :
	case DIMSE_C_GET_RQ :
	case DIMSE_C_FIND_RQ :
	case DIMSE_C_MOVE_RQ :
	case DIMSE_N_EVENT_REPORT_RQ :
	case DIMSE_N_GET_RQ :
	case DIMSE_N_SET_RQ :
	case DIMSE_N_ACTION_RQ :
	case DIMSE_N_CREATE_RQ :
	case DIMSE_N_DELETE_RQ :
		req.setIDs(_ascAssocPtr);
		break;
	default :
		break;
	}

	cond = DIMSE_sendMessageUsingMemoryData(_ascAssocPtr, pcId, dimsePtr, statusDatasetPtr, req._dcmDatasetPtr, ProgressCallback, (void*)&req);

	DCMTK_LOG4CPLUS_DEBUG_FMT(_logger, "sendMessage: cmd=%04XH:%s,cond=%s", dimsePtr->CommandField, req.getCommandName().c_str(), cond.text());

	if (statusDatasetPtr != NULL)
		delete statusDatasetPtr;

	return cond;
}

Status Association::receiveMessage(Message& rsp, int timeout)
{
	QMutexLocker locker(&_assocMutex);

	OFCondition cond;
	T_ASC_PresentationContextID& pcId = rsp._pcId;
	T_ASC_PresentationContextID pcId2 = 0;
	T_DIMSE_Message	&dimse = rsp;
	DcmDataset* statusDatasetPtr = NULL;
	bool isDatasetPresent = false;

	if (rsp._dcmDatasetPtr)
		cond = rsp._dcmDatasetPtr->clear();

//	if (!ASC_dataWaiting(m_pAscAssoc, nTimeout))	// FIXME
//		return DIMSE_NODATAAVAILABLE;

	if (timeout == -1)
		cond = DIMSE_receiveCommand(this->_ascAssocPtr, DIMSE_BLOCKING, 0, &pcId, &dimse, &statusDatasetPtr);
	else
		cond = DIMSE_receiveCommand(this->_ascAssocPtr, DIMSE_NONBLOCKING, timeout, &pcId, &dimse, &statusDatasetPtr);

	if (statusDatasetPtr != NULL)
		delete statusDatasetPtr;

	if (cond == DIMSE_NODATAAVAILABLE) {
		return DIMSE_NODATAAVAILABLE;
	} else if (cond == DUL_PEERREQUESTEDRELEASE) {
		cond = ASC_acknowledgeRelease(this->_ascAssocPtr);
		return DUL_PEERREQUESTEDRELEASE;
	}
	else if (cond != EC_Normal)
		return cond;

	DCMTK_LOG4CPLUS_DEBUG_FMT(_logger, "receiveMessage: cmd=%04XH:%s,cond=%s", dimse.CommandField, rsp.getCommandName().c_str(), cond.text());

	switch(dimse.CommandField) {
	case DIMSE_NOTHING :
	default :
		return DIMSE_BADCOMMANDTYPE;
	case DIMSE_C_ECHO_RQ :
		if (dimse.msg.CEchoRQ.DataSetType == DIMSE_DATASET_PRESENT)
			isDatasetPresent = true;
		break;
	case DIMSE_C_ECHO_RSP :
		if (dimse.msg.CEchoRSP.DataSetType == DIMSE_DATASET_PRESENT)
			isDatasetPresent = true;
		break;
	case DIMSE_C_STORE_RQ :
		if (dimse.msg.CStoreRQ.DataSetType == DIMSE_DATASET_PRESENT)
			isDatasetPresent = true;
		break;
	case DIMSE_C_STORE_RSP :
		if (dimse.msg.CStoreRSP.DataSetType == DIMSE_DATASET_PRESENT)
			isDatasetPresent = true;
		break;
	case DIMSE_C_GET_RQ :
		if (dimse.msg.CGetRQ.DataSetType == DIMSE_DATASET_PRESENT)
			isDatasetPresent = true;
		break;
	case DIMSE_C_GET_RSP :
		if (dimse.msg.CGetRSP.DataSetType == DIMSE_DATASET_PRESENT)
			isDatasetPresent = true;
		break;
	case DIMSE_C_FIND_RQ :
		if (dimse.msg.CFindRQ.DataSetType == DIMSE_DATASET_PRESENT)
			isDatasetPresent = true;
		break;
	case DIMSE_C_FIND_RSP :
		if (dimse.msg.CFindRSP.DataSetType == DIMSE_DATASET_PRESENT)
			isDatasetPresent = true;
		break;
	case DIMSE_C_MOVE_RQ :
		if (dimse.msg.CMoveRQ.DataSetType == DIMSE_DATASET_PRESENT)
			isDatasetPresent = true;
		break;
	case DIMSE_C_MOVE_RSP :
		if (dimse.msg.CMoveRSP.DataSetType == DIMSE_DATASET_PRESENT)
			isDatasetPresent = true;
		break;
	case DIMSE_C_CANCEL_RQ :
		if (dimse.msg.CCancelRQ.DataSetType == DIMSE_DATASET_PRESENT)
			isDatasetPresent = true;
		break;
	case DIMSE_N_EVENT_REPORT_RQ :
		if (dimse.msg.NEventReportRQ.DataSetType == DIMSE_DATASET_PRESENT)
			isDatasetPresent = true;
		break;
	case DIMSE_N_EVENT_REPORT_RSP :
		if (dimse.msg.NEventReportRSP.DataSetType == DIMSE_DATASET_PRESENT)
			isDatasetPresent = true;
		break;
	case DIMSE_N_GET_RQ :
		if (dimse.msg.NGetRQ.DataSetType == DIMSE_DATASET_PRESENT)
			isDatasetPresent = true;
		break;
	case DIMSE_N_GET_RSP :
		if (dimse.msg.NGetRSP.DataSetType == DIMSE_DATASET_PRESENT)
			isDatasetPresent = true;
		break;
	case DIMSE_N_SET_RQ :
		if (dimse.msg.NSetRQ.DataSetType == DIMSE_DATASET_PRESENT)
			isDatasetPresent = true;
		break;
	case DIMSE_N_SET_RSP :
		if (dimse.msg.NSetRSP.DataSetType == DIMSE_DATASET_PRESENT)
			isDatasetPresent = true;
		break;
	case DIMSE_N_ACTION_RQ :
		if (dimse.msg.NActionRQ.DataSetType == DIMSE_DATASET_PRESENT)
			isDatasetPresent = true;
		break;
	case DIMSE_N_ACTION_RSP :
		if (dimse.msg.NActionRSP.DataSetType == DIMSE_DATASET_PRESENT)
			isDatasetPresent = true;
		break;
	case DIMSE_N_CREATE_RQ :
		if (dimse.msg.NCreateRQ.DataSetType == DIMSE_DATASET_PRESENT)
			isDatasetPresent = true;
		break;
	case DIMSE_N_CREATE_RSP :
		if (dimse.msg.NCreateRSP.DataSetType == DIMSE_DATASET_PRESENT)
			isDatasetPresent = true;
		break;
	case DIMSE_N_DELETE_RQ :
		if (dimse.msg.NDeleteRQ.DataSetType == DIMSE_DATASET_PRESENT)
			isDatasetPresent = true;
		break;
	case DIMSE_N_DELETE_RSP :
		if (dimse.msg.NDeleteRSP.DataSetType == DIMSE_DATASET_PRESENT)
			isDatasetPresent = true;
		break;
	}

	if (isDatasetPresent) {
		if (timeout == -1)
			cond = DIMSE_receiveDataSetInMemory(_ascAssocPtr, DIMSE_BLOCKING, 0, &pcId2, &rsp._dcmDatasetPtr,
															ProgressCallback, (void*)&rsp);
		else
			cond = DIMSE_receiveDataSetInMemory(_ascAssocPtr, DIMSE_NONBLOCKING, timeout, &pcId2, &rsp._dcmDatasetPtr,
															ProgressCallback, (void*)&rsp);
	}

	return cond;
}

Status Association::getAssocInfo(AssociationInfo& assocInfo)
{
	if (_ascAssocPtr == NULL)
		return EC_Normal;

	assocInfo.applicationContextName = _ascAssocPtr->params->DULparams.applicationContextName;

	assocInfo.callingAETitle = _ascAssocPtr->params->DULparams.callingAPTitle;
	assocInfo.callingPresentationAddress = _ascAssocPtr->params->DULparams.callingPresentationAddress;
	assocInfo.callingImplementationClassUID = _ascAssocPtr->params->DULparams.callingImplementationClassUID;
	assocInfo.callingImplementationVersion = _ascAssocPtr->params->DULparams.callingImplementationVersionName;

	assocInfo.calledAETitle = _ascAssocPtr->params->DULparams.calledAPTitle;
	assocInfo.calledPresentationAddress = _ascAssocPtr->params->DULparams.calledPresentationAddress;
	assocInfo.calledImplementationClassUID = _ascAssocPtr->params->DULparams.calledImplementationClassUID;
	assocInfo.calledImplementationVersion = _ascAssocPtr->params->DULparams.calledImplementationVersionName;

	assocInfo.respondingAETitle = _ascAssocPtr->params->DULparams.respondingAPTitle;

	assocInfo.maxPDUSizeOfMine = _ascAssocPtr->params->DULparams.maxPDU;
	assocInfo.maxPDUSizeOfPeer = _ascAssocPtr->params->DULparams.peerMaxPDU;

	return EC_Normal;
}

Status Association::verify(const String& localAETitle, const String& aetitle, const String& hostname, const Uint16 port, const Uint16 timeout)
{
	if (localAETitle.empty() || aetitle.empty() || hostname.empty())
		return EC_IllegalParameter;

	bool verified = false;
	Status dcmStat;
	AppEntity dcmAppEntity;
	ServiceList verificationServiceList;
	AssociationRequestor dcmRequestor(&dcmAppEntity);
	CEchoRQ req;
	Message rsp;

	dcmStat = dcmAppEntity.init(localAETitle, NET_REQUESTOR, 0, timeout);
	if (!dcmStat.good())
		goto on_exit;

	dcmStat = dcmRequestor.connect(aetitle, hostname, port, verificationServiceList);
	if (!dcmStat.good())
		goto on_exit;

	dcmStat = dcmRequestor.sendMessage(req);
	if (!dcmStat.good())
		goto on_exit;

	dcmStat = dcmRequestor.receiveMessage(rsp, 10);
	if (!dcmStat.good())
		goto on_exit;

	verified = true;

on_exit:
	if (verified) {
		DCMTK_LOG4CPLUS_INFO_FMT(_logger, "%s@%s:%d verified", aetitle.c_str(), hostname.c_str(), port);
	} else {
		DCMTK_LOG4CPLUS_INFO_FMT(_logger, "%s@%s:%d NOT verified", aetitle.c_str(), hostname.c_str(), port);
	}

	dcmRequestor.close();
	dcmAppEntity.exit();

	return dcmStat;
}

Status Association::verify(const QString& localAETitle, const QString& aetitle, const QString& hostname, const Uint16 port, const Uint16 timeout)
{
	return verify(QSTR_TO_DSTR(localAETitle), QSTR_TO_DSTR(aetitle), QSTR_TO_DSTR(hostname), port, timeout);
}

static void ProgressCallback(void *callbackContextPtr, Ulong byteCount)
{
	Message* messagePtr = reinterpret_cast<Message*>(callbackContextPtr);
	messagePtr->onProgress(byteCount);
}