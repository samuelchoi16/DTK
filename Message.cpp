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

#define	SAFE_STRING(psz)				((psz) ? (psz) : "")
#define	SAFE_COPY(d, s)					strncpy((d), (s), sizeof((d))-1)
#define	IS_VALID_STRING(s)				(((s) != NULL) && ((*(s)) != 0))

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

Message::Message(void)
{
	memset(static_cast<T_DIMSE_Message*>(this), 0, sizeof(T_DIMSE_Message));
	_pcId = 0;
	_dcmDatasetPtr = NULL;
};

Message::Message(Dataset* datasetPtr)
{
	memset(static_cast<T_DIMSE_Message*>(this), 0, sizeof(T_DIMSE_Message));
	_pcId = 0;
	_dcmDatasetPtr = datasetPtr ? datasetPtr->_getDcmDataset() : NULL;
};

Message& Message::setDcmDataset(Dataset* datasetPtr)
{
	memset(static_cast<T_DIMSE_Message*>(this), 0, sizeof(T_DIMSE_Message));
	_pcId = 0;
	_dcmDatasetPtr = datasetPtr ? datasetPtr->_getDcmDataset() : NULL;

	return *this;
}

void Message::setIDs(T_ASC_Association* ascAssocPtr)
{
	String sopClassUID = _sopClassUID;
	_pcId = ASC_findAcceptedPresentationContextID(ascAssocPtr, sopClassUID.c_str());
	if (_pcId > 0)
		return;

	// check BasicGrayscalePrintManagementMetaSOPClass
	if (sopClassUID == UID_BasicFilmSessionSOPClass ||
		sopClassUID == UID_BasicFilmBoxSOPClass ||
		sopClassUID == UID_BasicGrayscaleImageBoxSOPClass ||
		sopClassUID == UID_PrinterSOPClass)
	{
		sopClassUID = UID_BasicGrayscalePrintManagementMetaSOPClass;
		_pcId = ASC_findAcceptedPresentationContextID(ascAssocPtr, sopClassUID.c_str());
		if (_pcId > 0)
			return;
	}

	// check BasicColorPrintManagementMetaSOPClass
	if ( sopClassUID == UID_BasicFilmSessionSOPClass ||
		sopClassUID == UID_BasicFilmBoxSOPClass ||
		sopClassUID == UID_BasicColorImageBoxSOPClass ||
		sopClassUID == UID_PrinterSOPClass)
	{
		sopClassUID = UID_BasicColorPrintManagementMetaSOPClass;
		_pcId = ASC_findAcceptedPresentationContextID(ascAssocPtr, sopClassUID.c_str());
		if (_pcId > 0)
			return;
	}
}

String Message::getCommandName(void) const
{
	switch(CommandField)
	{
	case DIMSE_C_ECHO_RQ :
		return "C-ECHO-RQ";
	case DIMSE_C_ECHO_RSP :
		return "C-ECHO-RSP";
	case DIMSE_C_STORE_RQ :
		return "C-STORE-RQ";
	case DIMSE_C_STORE_RSP :
		return "C-STORE-RSP";
	case DIMSE_C_GET_RQ :
		return "C-GET-RQ";
	case DIMSE_C_GET_RSP :
		return "C-GET-RSP";
	case DIMSE_C_FIND_RQ :
		return "C-FIND-RQ";
	case DIMSE_C_FIND_RSP :
		return "C-FIND-RSP";
	case DIMSE_C_MOVE_RQ :
		return "C-MOVE-RQ";
	case DIMSE_C_MOVE_RSP :
		return "C-MOVE-RSP";
	case DIMSE_C_CANCEL_RQ :
		return "C-CANCEL-RQ";
	case DIMSE_N_EVENT_REPORT_RQ :
		return "N-EVENT-REPORT-RQ";
	case DIMSE_N_EVENT_REPORT_RSP :
		return "N-EVENT-REPORT-RSP";
	case DIMSE_N_GET_RQ :
		return "N-GET-RQ";
	case DIMSE_N_GET_RSP :
		return "N-GET-RSP";
	case DIMSE_N_SET_RQ :
		return "N-SET-RQ";
	case DIMSE_N_SET_RSP :
		return "N-SET-RSP";
	case DIMSE_N_ACTION_RQ :
		return "N-ACTION-RQ";
	case DIMSE_N_ACTION_RSP :
		return "N-ACTION-RSP";
	case DIMSE_N_CREATE_RQ :
		return "N-CREATE-RQ";
	case DIMSE_N_CREATE_RSP :
		return "N-CREATE-RSP";
	case DIMSE_N_DELETE_RQ :
		return "N-DELETE-RQ";
	case DIMSE_N_DELETE_RSP :
		return "N-DELETE-RSP";
	default :
		return "UNKNOWN";
	}
}

Uint16 Message::getStatus(void) const
{
	switch(CommandField)
	{
	case DIMSE_C_ECHO_RSP :
		return msg.CEchoRSP.DimseStatus;
	case DIMSE_C_STORE_RSP :
		return msg.CStoreRSP.DimseStatus;
	case DIMSE_C_GET_RSP :
		return msg.CGetRSP.DimseStatus;
	case DIMSE_C_FIND_RSP :
		return msg.CFindRSP.DimseStatus;
	case DIMSE_C_MOVE_RSP :
		return msg.CMoveRSP.DimseStatus;
	case DIMSE_N_EVENT_REPORT_RSP :
		return msg.NEventReportRSP.DimseStatus;
	case DIMSE_N_GET_RSP :
		return msg.NGetRSP.DimseStatus;
	case DIMSE_N_SET_RSP :
		return msg.NSetRSP.DimseStatus;
	case DIMSE_N_ACTION_RSP :
		return msg.NActionRSP.DimseStatus;
	case DIMSE_N_CREATE_RSP :
		return msg.NCreateRSP.DimseStatus;
	case DIMSE_N_DELETE_RSP :
		return msg.NDeleteRSP.DimseStatus;
	default :
		return 0xFFFF;
	}
}

Message& Message::print(const String& filename)
{
	std::ofstream os;

	os.open(filename.c_str());
	DIMSE_printMessage(os, *this, _dcmDatasetPtr);
	os.close();

	return *this;
}

void Message::onProgress(Uint32 byteCount)
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

CEchoRQ::CEchoRQ(Dataset* datasetPtr)
	: Message(datasetPtr)
{
	_sopClassUID = UID_VerificationSOPClass;

	CommandField = DIMSE_C_ECHO_RQ;
	SAFE_COPY(msg.CEchoRQ.AffectedSOPClassUID, _sopClassUID.c_str());
	msg.CEchoRQ.DataSetType	= _dcmDatasetPtr ? DIMSE_DATASET_PRESENT : DIMSE_DATASET_NULL;
};

void CEchoRQ::setIDs(T_ASC_Association* ascAssocPtr)
{
	_pcId = ASC_findAcceptedPresentationContextID(ascAssocPtr, _sopClassUID.c_str());
	Message::setIDs(ascAssocPtr);
	msg.CEchoRQ.MessageID = ascAssocPtr->nextMsgID++;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

CEchoRSP::CEchoRSP(const Message& req, Uint16 status, Dataset* datasetPtr)
	: Message(datasetPtr)
{
	_pcId = req._pcId;
	assert(req.CommandField == DIMSE_C_ECHO_RQ);

	CommandField = DIMSE_C_ECHO_RSP;
	msg.CEchoRSP.opts = 0;
	msg.CEchoRSP.MessageIDBeingRespondedTo = req.msg.CEchoRQ.MessageID;
	if (IS_VALID_STRING(req.msg.CEchoRQ.AffectedSOPClassUID))
	{
		SAFE_COPY(msg.CEchoRSP.AffectedSOPClassUID, req.msg.CEchoRQ.AffectedSOPClassUID);
		msg.CEchoRSP.opts |= O_ECHO_AFFECTEDSOPCLASSUID;
	}
	msg.CEchoRSP.DataSetType = _dcmDatasetPtr ? DIMSE_DATASET_PRESENT : DIMSE_DATASET_NULL;
	msg.CEchoRSP.DimseStatus = status;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

CStoreRQ::CStoreRQ(Dataset* datasetPtr)
	: Message(datasetPtr)
{
	datasetPtr->getString(DCM_SOPClassUID, _sopClassUID);
	datasetPtr->getString(DCM_SOPInstanceUID, _sopInstanceUID);

	CommandField = DIMSE_C_STORE_RQ;
	SAFE_COPY(msg.CStoreRQ.AffectedSOPClassUID, _sopClassUID.c_str());
	SAFE_COPY(msg.CStoreRQ.AffectedSOPInstanceUID, _sopInstanceUID.c_str());
	msg.CStoreRQ.DataSetType = _dcmDatasetPtr ? DIMSE_DATASET_PRESENT : DIMSE_DATASET_NULL;
	msg.CStoreRQ.Priority = DIMSE_PRIORITY_MEDIUM;
}

void CStoreRQ::setIDs(T_ASC_Association* ascAssocPtr)
{
	DcmXfer filexfer(_dcmDatasetPtr->getOriginalXfer());
	if (filexfer.getXfer() != EXS_Unknown)
		_pcId = ASC_findAcceptedPresentationContextID(ascAssocPtr, _sopClassUID.c_str(), filexfer.getXferID());
	else
		_pcId = ASC_findAcceptedPresentationContextID(ascAssocPtr, _sopClassUID.c_str());
	msg.CStoreRQ.MessageID = ascAssocPtr->nextMsgID++;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

CStoreRSP::CStoreRSP(const Message& req, Uint16 status, Dataset* datasetPtr)
	: Message(datasetPtr)
{
	_pcId = req._pcId;
	assert(req.CommandField == DIMSE_C_STORE_RQ);

	CommandField = DIMSE_C_STORE_RSP;
	msg.CStoreRSP.opts = 0;
	msg.CStoreRSP.MessageIDBeingRespondedTo = req.msg.CStoreRQ.MessageID;
	if (IS_VALID_STRING(req.msg.CStoreRQ.AffectedSOPClassUID))
	{
		SAFE_COPY(msg.CStoreRSP.AffectedSOPClassUID, req.msg.CStoreRQ.AffectedSOPClassUID);
		msg.CStoreRSP.opts |= O_STORE_AFFECTEDSOPCLASSUID;
	}
	if (IS_VALID_STRING(req.msg.CStoreRQ.AffectedSOPInstanceUID))
	{
		SAFE_COPY(msg.CStoreRSP.AffectedSOPInstanceUID, req.msg.CStoreRQ.AffectedSOPInstanceUID);
		msg.CStoreRSP.opts |= O_STORE_AFFECTEDSOPINSTANCEUID;
	}
	msg.CStoreRSP.DataSetType = _dcmDatasetPtr ? DIMSE_DATASET_PRESENT : DIMSE_DATASET_NULL;
	msg.CStoreRSP.DimseStatus = status;
	if (req.msg.CStoreRQ.opts & O_STORE_RQ_BLANK_PADDING)
		msg.CStoreRSP.opts |= O_STORE_RSP_BLANK_PADDING;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

CFindRQ::CFindRQ(const dcm::String& sopClassUID, Dataset* datasetPtr)
	: Message(datasetPtr)
{
	_sopClassUID = SAFE_STRING(sopClassUID.c_str());

	CommandField = DIMSE_C_FIND_RQ;
	SAFE_COPY(msg.CFindRQ.AffectedSOPClassUID, _sopClassUID.c_str());
	msg.CFindRQ.DataSetType = _dcmDatasetPtr ? DIMSE_DATASET_PRESENT : DIMSE_DATASET_NULL;
	msg.CFindRQ.Priority = DIMSE_PRIORITY_MEDIUM;
}

void CFindRQ::setIDs(T_ASC_Association* ascAssocPtr)
{
	Message::setIDs(ascAssocPtr);
	msg.CFindRQ.MessageID = ascAssocPtr->nextMsgID++;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

CFindRSP::CFindRSP(const Message& req, Uint16 status, Dataset* datasetPtr)
	: Message(datasetPtr)
{
	_pcId = req._pcId;
	assert(req.CommandField == DIMSE_C_FIND_RQ);

	CommandField = DIMSE_C_FIND_RSP;
	msg.CFindRSP.opts = 0;
	msg.CFindRSP.MessageIDBeingRespondedTo = req.msg.CFindRQ.MessageID;
	if (IS_VALID_STRING(req.msg.CFindRQ.AffectedSOPClassUID))
	{
		SAFE_COPY(msg.CFindRSP.AffectedSOPClassUID, req.msg.CFindRQ.AffectedSOPClassUID);
		msg.CFindRSP.opts |= O_FIND_AFFECTEDSOPCLASSUID;
	}
	msg.CFindRSP.DataSetType = _dcmDatasetPtr ? DIMSE_DATASET_PRESENT : DIMSE_DATASET_NULL;
	msg.CFindRSP.DimseStatus = status;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

CGetRQ::CGetRQ(const String& sopClassUID, Dataset* datasetPtr)
	: Message(datasetPtr)
{
	_sopClassUID = SAFE_STRING(sopClassUID.c_str());

	CommandField = DIMSE_C_GET_RQ;
	SAFE_COPY(msg.CGetRQ.AffectedSOPClassUID, _sopClassUID.c_str());
	msg.CGetRQ.Priority = DIMSE_PRIORITY_MEDIUM;
	msg.CGetRQ.DataSetType = _dcmDatasetPtr ? DIMSE_DATASET_PRESENT : DIMSE_DATASET_NULL;
}

void CGetRQ::setIDs(T_ASC_Association* ascAssocPtr)
{
	Message::setIDs(ascAssocPtr);
	msg.CGetRQ.MessageID = ascAssocPtr->nextMsgID++;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

CGetRSP::CGetRSP(const Message& req, Uint16 status, Dataset* datasetPtr, int remaining, int completed, int failure, int warning)
	: Message(datasetPtr)
{
	_pcId = req._pcId;
	assert(req.CommandField == DIMSE_C_GET_RQ);

	CommandField = DIMSE_C_GET_RSP;
	msg.CGetRSP.opts = 0;
	msg.CGetRSP.MessageIDBeingRespondedTo = req.msg.CGetRQ.MessageID;
	if (IS_VALID_STRING(req.msg.CGetRSP.AffectedSOPClassUID))
	{
		SAFE_COPY(msg.CGetRSP.AffectedSOPClassUID, req.msg.CGetRQ.AffectedSOPClassUID);
		msg.CGetRSP.opts |= O_GET_AFFECTEDSOPCLASSUID;
	}
	msg.CGetRSP.DataSetType = _dcmDatasetPtr ? DIMSE_DATASET_PRESENT : DIMSE_DATASET_NULL;
	msg.CGetRSP.DimseStatus = status;
	if (remaining >= 0)
	{
		msg.CGetRSP.NumberOfRemainingSubOperations = remaining;
		msg.CGetRSP.opts |= O_GET_NUMBEROFREMAININGSUBOPERATIONS;
	}
	if (completed >= 0)
	{
		msg.CGetRSP.NumberOfCompletedSubOperations = completed;
		msg.CGetRSP.opts |= O_GET_NUMBEROFCOMPLETEDSUBOPERATIONS;
	}
	if (failure >= 0)
	{
		msg.CGetRSP.NumberOfFailedSubOperations = failure;
		msg.CGetRSP.opts |= O_GET_NUMBEROFFAILEDSUBOPERATIONS;
	}
	if (warning >= 0)
	{
		msg.CGetRSP.NumberOfWarningSubOperations = warning;
		msg.CGetRSP.opts |= O_GET_NUMBEROFWARNINGSUBOPERATIONS;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

CMoveRQ::CMoveRQ(const String& sopClassUID, const String& destination, Dataset* datasetPtr)
	: Message(datasetPtr)
{
	_sopClassUID = sopClassUID;

	CommandField = DIMSE_C_MOVE_RQ;
	SAFE_COPY(msg.CMoveRQ.AffectedSOPClassUID, _sopClassUID.c_str());
	SAFE_COPY(msg.CMoveRQ.MoveDestination, destination.c_str());
	msg.CMoveRQ.DataSetType = _dcmDatasetPtr ? DIMSE_DATASET_PRESENT : DIMSE_DATASET_NULL;
	msg.CMoveRQ.Priority = DIMSE_PRIORITY_MEDIUM;
}

void CMoveRQ::setIDs(T_ASC_Association* ascAssocPtr)
{
	Message::setIDs(ascAssocPtr);
	msg.CMoveRQ.MessageID = ascAssocPtr->nextMsgID++;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

CMoveRSP::CMoveRSP(const Message& req, Uint16 status, Dataset* datasetPtr, int remaining, int completed, int failure, int warning)
	: Message(datasetPtr)
{
	_pcId = req._pcId;
	assert(req.CommandField == DIMSE_C_MOVE_RQ);

	CommandField = DIMSE_C_MOVE_RSP;
	msg.CMoveRSP.opts = 0;
	msg.CMoveRSP.MessageIDBeingRespondedTo = req.msg.CMoveRQ.MessageID;
	if (IS_VALID_STRING(req.msg.CMoveRQ.AffectedSOPClassUID))
	{
		SAFE_COPY(msg.CMoveRSP.AffectedSOPClassUID, req.msg.CMoveRQ.AffectedSOPClassUID);
		msg.CMoveRSP.opts |= O_MOVE_AFFECTEDSOPCLASSUID;
	}
	msg.CMoveRSP.DataSetType = _dcmDatasetPtr ? DIMSE_DATASET_PRESENT : DIMSE_DATASET_NULL;
	msg.CMoveRSP.DimseStatus = status;

	if (remaining >= 0)
	{
		msg.CMoveRSP.NumberOfRemainingSubOperations = remaining;
		msg.CMoveRSP.opts |= O_MOVE_NUMBEROFREMAININGSUBOPERATIONS;
	}
	if (completed >= 0)
	{
		msg.CMoveRSP.NumberOfCompletedSubOperations = completed;
		msg.CMoveRSP.opts |= O_MOVE_NUMBEROFCOMPLETEDSUBOPERATIONS;
	}
	if (failure >= 0)
	{
		msg.CMoveRSP.NumberOfFailedSubOperations = failure;
		msg.CMoveRSP.opts |= O_MOVE_NUMBEROFFAILEDSUBOPERATIONS;
	}
	if (warning >= 0)
	{
		msg.CMoveRSP.NumberOfWarningSubOperations = warning;
		msg.CMoveRSP.opts |= O_MOVE_NUMBEROFWARNINGSUBOPERATIONS;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

CCancelRQ::CCancelRQ(const Message& req)
{
	_pcId = req._pcId;

	CommandField = DIMSE_C_CANCEL_RQ;

	if (req.CommandField == DIMSE_C_FIND_RQ)
		msg.CCancelRQ.MessageIDBeingRespondedTo = req.msg.CFindRQ.MessageID;
	else if (req.CommandField == DIMSE_C_MOVE_RQ)
		msg.CCancelRQ.MessageIDBeingRespondedTo = req.msg.CMoveRQ.MessageID;

	msg.CCancelRQ.DataSetType = DIMSE_DATASET_NULL;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

NEventReportRQ::NEventReportRQ(const String& sopClassUID, const String& sopInstanceUID, Uint16 eventTypeId, Dataset* datasetPtr)
	: Message(datasetPtr)
{
	_sopClassUID = sopClassUID;
	_sopInstanceUID	= sopInstanceUID;

	CommandField = DIMSE_N_EVENT_REPORT_RQ;
	SAFE_COPY(msg.NEventReportRQ.AffectedSOPClassUID, _sopClassUID.c_str());
	SAFE_COPY(msg.NEventReportRQ.AffectedSOPInstanceUID, _sopInstanceUID.c_str());
	msg.NEventReportRQ.DataSetType = _dcmDatasetPtr ? DIMSE_DATASET_PRESENT : DIMSE_DATASET_NULL;
	msg.NEventReportRQ.EventTypeID = eventTypeId;
}

void NEventReportRQ::setIDs(T_ASC_Association* ascAssocPtr)
{
	Message::setIDs(ascAssocPtr);
	msg.NEventReportRQ.MessageID = ascAssocPtr->nextMsgID++;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

NEventReportRSP::NEventReportRSP(const Message& req, Uint16 status, Dataset* datasetPtr)
	: Message(datasetPtr)
{
	_pcId = req._pcId;
	assert(req.CommandField == DIMSE_N_EVENT_REPORT_RQ);

	CommandField = DIMSE_N_EVENT_REPORT_RSP;
	msg.NEventReportRSP.opts = 0;
	msg.NEventReportRSP.MessageIDBeingRespondedTo = req.msg.NEventReportRQ.MessageID;
	if (IS_VALID_STRING(req.msg.NEventReportRQ.AffectedSOPClassUID))
	{
		SAFE_COPY(msg.NEventReportRSP.AffectedSOPClassUID, req.msg.NEventReportRQ.AffectedSOPClassUID);
		msg.NEventReportRSP.opts |= O_NEVENTREPORT_AFFECTEDSOPCLASSUID;
	}
	msg.NEventReportRSP.DimseStatus = status;
	if (IS_VALID_STRING(req.msg.NEventReportRQ.AffectedSOPInstanceUID))
	{
		SAFE_COPY(msg.NEventReportRSP.AffectedSOPInstanceUID, req.msg.NEventReportRQ.AffectedSOPInstanceUID);
		msg.NEventReportRSP.opts |= O_NEVENTREPORT_AFFECTEDSOPINSTANCEUID;
	}
	msg.NEventReportRSP.DataSetType = _dcmDatasetPtr ? DIMSE_DATASET_PRESENT : DIMSE_DATASET_NULL;
	if (req.msg.NEventReportRQ.EventTypeID > 0)
	{
		msg.NEventReportRSP.EventTypeID = req.msg.NEventReportRQ.EventTypeID;
		msg.NEventReportRSP.opts |= O_NEVENTREPORT_EVENTTYPEID;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

NGetRQ::NGetRQ(const dcm::String& sopClassUID, const dcm::String& sopInstanceUID, const DcmTagKey* dcmTagPtr, int dcmTagCount, Dataset* datasetPtr)
	: Message(datasetPtr)
{
	_sopClassUID = sopClassUID;
	_sopInstanceUID	= sopInstanceUID;

	CommandField = DIMSE_N_GET_RQ;
	SAFE_COPY(msg.NGetRQ.RequestedSOPClassUID, _sopClassUID.c_str());
	SAFE_COPY(msg.NGetRQ.RequestedSOPInstanceUID, _sopInstanceUID.c_str());
	msg.NGetRQ.DataSetType = _dcmDatasetPtr ? DIMSE_DATASET_PRESENT : DIMSE_DATASET_NULL;
	msg.NGetRQ.ListCount = dcmTagCount * 2;
	msg.NGetRQ.AttributeIdentifierList = new DIC_US[msg.NGetRQ.ListCount * 2];
	for(int nIndex = 0; nIndex < dcmTagCount; nIndex++)
	{
		msg.NGetRQ.AttributeIdentifierList[nIndex*2 + 0] = dcmTagPtr[nIndex].getGroup();
		msg.NGetRQ.AttributeIdentifierList[nIndex*2 + 1] = dcmTagPtr[nIndex].getElement();
	}
}

NGetRQ::~NGetRQ(void)
{
	delete[] msg.NGetRQ.AttributeIdentifierList;
}

void NGetRQ::setIDs(T_ASC_Association* ascAssocPtr)
{
	Message::setIDs(ascAssocPtr);
	msg.NGetRQ.MessageID = ascAssocPtr->nextMsgID++;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

NGetRSP::NGetRSP(const Message& req, Uint16 nStatus, Dataset* pDcmDatasetArg)
	: Message(pDcmDatasetArg)
{
	_pcId = req._pcId;
	assert(req.CommandField == DIMSE_N_GET_RQ);

	CommandField			= DIMSE_N_GET_RSP;
	msg.NGetRSP.opts		= 0;
	msg.NGetRSP.MessageIDBeingRespondedTo = req.msg.NGetRQ.MessageID;
	if (IS_VALID_STRING(req.msg.NGetRQ.RequestedSOPClassUID))
	{
		SAFE_COPY(msg.NGetRSP.AffectedSOPClassUID, req.msg.NGetRQ.RequestedSOPClassUID);
		msg.NGetRSP.opts |= O_NGET_AFFECTEDSOPCLASSUID;
	}
	msg.NGetRSP.DimseStatus = nStatus;
	if (IS_VALID_STRING(req.msg.NGetRQ.RequestedSOPInstanceUID))
	{
		SAFE_COPY(msg.NGetRSP.AffectedSOPInstanceUID, req.msg.NGetRQ.RequestedSOPInstanceUID);
		msg.NGetRSP.opts |= O_NGET_AFFECTEDSOPINSTANCEUID;
	}
	msg.NGetRSP.DataSetType = _dcmDatasetPtr ? DIMSE_DATASET_PRESENT : DIMSE_DATASET_NULL;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

NSetRQ::NSetRQ(const String& sopClassUID, const String& sopInstanceUID, Dataset* datasetPtr)
	: Message(datasetPtr)
{
	_sopClassUID = sopClassUID;
	_sopInstanceUID	= sopInstanceUID;

	CommandField = DIMSE_N_SET_RQ;
	SAFE_COPY(msg.NSetRQ.RequestedSOPClassUID, _sopClassUID.c_str());
	SAFE_COPY(msg.NSetRQ.RequestedSOPInstanceUID, _sopInstanceUID.c_str());
	msg.NSetRQ.DataSetType = _dcmDatasetPtr ? DIMSE_DATASET_PRESENT : DIMSE_DATASET_NULL;
}

void NSetRQ::setIDs(T_ASC_Association* ascAssocPtr)
{
	Message::setIDs(ascAssocPtr);
	msg.NSetRQ.MessageID = ascAssocPtr->nextMsgID++;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

NSetRSP::NSetRSP(const Message& req, Uint16 status, Dataset* datasetPtr)
	: Message(datasetPtr)
{
	_pcId = req._pcId;
	assert(req.CommandField == DIMSE_N_SET_RQ);

	CommandField = DIMSE_N_SET_RSP;
	msg.NSetRSP.opts = 0;
	msg.NSetRSP.MessageIDBeingRespondedTo = req.msg.NSetRQ.MessageID;
	if (IS_VALID_STRING(req.msg.NSetRQ.RequestedSOPClassUID))
	{
		SAFE_COPY(msg.NSetRSP.AffectedSOPClassUID, req.msg.NSetRQ.RequestedSOPClassUID);
		msg.NSetRSP.opts |= O_NSET_AFFECTEDSOPCLASSUID;
	}
	msg.NSetRSP.DimseStatus = status;
	if (IS_VALID_STRING(req.msg.NSetRQ.RequestedSOPInstanceUID))
	{
		SAFE_COPY(msg.NSetRSP.AffectedSOPInstanceUID, req.msg.NSetRQ.RequestedSOPInstanceUID);
		msg.NSetRSP.opts |= O_NSET_AFFECTEDSOPINSTANCEUID;
	}
	msg.NSetRSP.DataSetType = _dcmDatasetPtr ? DIMSE_DATASET_PRESENT : DIMSE_DATASET_NULL;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

NActionRQ::NActionRQ(const String& sopClassUID, const String& sopInstanceUID, Uint16 actionTypeId, Dataset* datasetPtr)
	: Message(datasetPtr)
{
	_sopClassUID = sopClassUID;
	_sopInstanceUID	= sopInstanceUID;

	CommandField			= DIMSE_N_ACTION_RQ;
	SAFE_COPY(msg.NActionRQ.RequestedSOPClassUID, _sopClassUID.c_str());
	SAFE_COPY(msg.NActionRQ.RequestedSOPInstanceUID, _sopInstanceUID.c_str());
	msg.NActionRQ.ActionTypeID = actionTypeId;
	msg.NActionRQ.DataSetType = _dcmDatasetPtr ? DIMSE_DATASET_PRESENT : DIMSE_DATASET_NULL;
}

void NActionRQ::setIDs(T_ASC_Association* ascAssocPtr)
{
	Message::setIDs(ascAssocPtr);
	msg.NActionRQ.MessageID	= ascAssocPtr->nextMsgID++;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

NActionRSP::NActionRSP(const Message& req, Uint16 status, Dataset* datasetPtr)
	: Message(datasetPtr)
{
	_pcId = req._pcId;
	assert(req.CommandField == DIMSE_N_ACTION_RQ);

	CommandField = DIMSE_N_ACTION_RSP;
	msg.NActionRSP.opts = 0;
	msg.NActionRSP.MessageIDBeingRespondedTo = req.msg.NActionRQ.MessageID;
	if (IS_VALID_STRING(req.msg.NActionRQ.RequestedSOPClassUID))
	{
		SAFE_COPY(msg.NActionRSP.AffectedSOPClassUID, req.msg.NActionRQ.RequestedSOPClassUID);
		msg.NActionRSP.opts |= O_NACTION_AFFECTEDSOPCLASSUID;
	}
	msg.NActionRSP.DimseStatus = status;
	if (IS_VALID_STRING(req.msg.NActionRQ.RequestedSOPInstanceUID))
	{
		SAFE_COPY(msg.NActionRSP.AffectedSOPInstanceUID, req.msg.NActionRQ.RequestedSOPInstanceUID);
		msg.NActionRSP.opts |= O_NACTION_AFFECTEDSOPINSTANCEUID;
	}
	if (req.msg.NActionRQ.ActionTypeID > 0)
	{
		msg.NActionRSP.ActionTypeID = req.msg.NActionRQ.ActionTypeID;
		msg.NActionRSP.opts |= O_NACTION_ACTIONTYPEID;
	}
	msg.NActionRSP.DataSetType = _dcmDatasetPtr ? DIMSE_DATASET_PRESENT : DIMSE_DATASET_NULL;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

NCreateRQ::NCreateRQ(const String& sopClassUID, const String& sopInstanceUID, Dataset* datasetPtr)
	: Message(datasetPtr)
{
	_sopClassUID = sopClassUID;
	_sopInstanceUID = sopInstanceUID;

	CommandField = DIMSE_N_CREATE_RQ;
	msg.NCreateRQ.opts = 0;
	SAFE_COPY(msg.NCreateRQ.AffectedSOPClassUID, _sopClassUID.c_str());
	if (_sopInstanceUID.size() > 0)
	{
		SAFE_COPY(msg.NCreateRQ.AffectedSOPInstanceUID, _sopInstanceUID.c_str());
		msg.NCreateRQ.opts	= O_NCREATE_AFFECTEDSOPINSTANCEUID;
	}
	msg.NCreateRQ.DataSetType = _dcmDatasetPtr ? DIMSE_DATASET_PRESENT : DIMSE_DATASET_NULL;
}

void NCreateRQ::setIDs(T_ASC_Association* ascAssocPtr)
{
	Message::setIDs(ascAssocPtr);
	msg.NCreateRQ.MessageID	= ascAssocPtr->nextMsgID++;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

NCreateRSP::NCreateRSP(const Message& req, Uint16 status, Dataset* datasetPtr)
	: Message(datasetPtr)
{
	_pcId = req._pcId;
	assert(req.CommandField == DIMSE_N_CREATE_RQ);

	CommandField = DIMSE_N_CREATE_RSP;
	msg.NCreateRSP.opts = 0;
	msg.NCreateRSP.MessageIDBeingRespondedTo = req.msg.NCreateRQ.MessageID;
	if (IS_VALID_STRING(req.msg.NCreateRQ.AffectedSOPClassUID))
	{
		SAFE_COPY(msg.NCreateRSP.AffectedSOPClassUID, req.msg.NCreateRQ.AffectedSOPClassUID);
		msg.NCreateRSP.opts |= O_NCREATE_AFFECTEDSOPCLASSUID;
	}
	msg.NCreateRSP.DimseStatus = status;
	if (IS_VALID_STRING(req.msg.NCreateRQ.AffectedSOPInstanceUID))
	{
		SAFE_COPY(msg.NCreateRSP.AffectedSOPInstanceUID, req.msg.NCreateRQ.AffectedSOPInstanceUID);
		msg.NCreateRSP.opts |= O_NCREATE_AFFECTEDSOPINSTANCEUID;
	}
	msg.NCreateRSP.DataSetType = _dcmDatasetPtr ? DIMSE_DATASET_PRESENT : DIMSE_DATASET_NULL;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

NDeleteRQ::NDeleteRQ(const String& sopClassUID, const String& sopInstanceUID, Dataset* datasetPtr)
	: Message(datasetPtr)
{
	_sopClassUID = sopClassUID;
	_sopInstanceUID	= sopInstanceUID;

	CommandField = DIMSE_N_DELETE_RQ;
	SAFE_COPY(msg.NDeleteRQ.RequestedSOPClassUID, _sopClassUID.c_str());
	SAFE_COPY(msg.NDeleteRQ.RequestedSOPInstanceUID, _sopInstanceUID.c_str());
	msg.NDeleteRQ.DataSetType = _dcmDatasetPtr ? DIMSE_DATASET_PRESENT : DIMSE_DATASET_NULL;
}

void NDeleteRQ::setIDs(T_ASC_Association* ascAssocPtr)
{
	Message::setIDs(ascAssocPtr);
	msg.NDeleteRQ.MessageID	= ascAssocPtr->nextMsgID++;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

NDeleteRSP::NDeleteRSP(const Message& req, Uint16 status, Dataset* datasetPtr)
	: Message(datasetPtr)
{
	_pcId = req._pcId;
	assert(req.CommandField == DIMSE_N_DELETE_RQ);

	CommandField = DIMSE_C_FIND_RSP;
	msg.NDeleteRSP.opts = 0;
	msg.NDeleteRSP.MessageIDBeingRespondedTo = req.msg.NDeleteRQ.MessageID;
	if (IS_VALID_STRING(req.msg.NDeleteRQ.RequestedSOPClassUID))
	{
		SAFE_COPY(msg.NDeleteRSP.AffectedSOPClassUID, req.msg.NDeleteRQ.RequestedSOPClassUID);
		msg.NDeleteRSP.opts |= O_NDELETE_AFFECTEDSOPCLASSUID;
	}
	msg.NDeleteRSP.DimseStatus = status;
	if (IS_VALID_STRING(req.msg.NDeleteRQ.RequestedSOPInstanceUID))
	{
		SAFE_COPY(msg.NDeleteRSP.AffectedSOPInstanceUID, req.msg.NDeleteRQ.RequestedSOPInstanceUID);
		msg.NDeleteRSP.opts |= O_NDELETE_AFFECTEDSOPINSTANCEUID;
	}
	msg.NDeleteRSP.DataSetType = _dcmDatasetPtr ? DIMSE_DATASET_PRESENT : DIMSE_DATASET_NULL;
}
