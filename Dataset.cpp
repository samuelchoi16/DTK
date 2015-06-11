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

////////////////////////////////////////////////////////////////////////////////////////////////////////////

Logger Dataset::_logger = Logger::getInstance("dcm.Dataset");

Dataset::Dataset(void)
{
	if (_created)
		delete _dcmItemPtr;

	_created = true;
	_dcmItemPtr = new DcmDataset;

	setAutoNLS(_DefaultNLS);
	setNLS(_DefaultNLS);
}

Dataset::Dataset(DcmDataset* dcmDatasetPtr)
	: Item(dcmDatasetPtr)
{
	int nls;
	if (getNLS(nls).good() && nls > 0)
		setAutoNLS(nls);
}

Dataset::Dataset(const Dataset& dataset)
{
	if (_created)
		delete _dcmItemPtr;

	_created = true;
	_dcmItemPtr = new DcmDataset(*dynamic_cast<DcmDataset*>(dataset._dcmItemPtr));

	int nls;
	if (dataset.getAutoNLS(nls).good())
		setAutoNLS(nls);
	if (dataset.getNLS(nls).good())
		setNLS(nls);
}

Dataset::~Dataset(void)
{
	if (_created) {
		delete _dcmItemPtr;
		_dcmItemPtr = NULL;
	}
}

Dataset& Dataset::operator=(const Dataset& dataset)
{
	Status dcmStat = copyFrom(&dataset);
	return *this;
}

Status Dataset::copyFrom(const Dataset* sourceDatasetPtr)
{
	if (sourceDatasetPtr == NULL)
		return EC_IllegalParameter;
	if (sourceDatasetPtr == this)
		return EC_Normal;

	clear();

	TagList tagList;
	if (sourceDatasetPtr->getTagList(tagList).good()) {
		for(TagList::iterator ti = tagList.begin(); ti != tagList.end(); ti++)
			copyValueFrom(*ti, sourceDatasetPtr);

		int nls;
		if (sourceDatasetPtr->getAutoNLS(nls).good())
			setAutoNLS(nls);
		if (sourceDatasetPtr->getNLS(nls).good())
			setNLS(nls);

		return EC_Normal;
	} else {
		return EC_TagNotFound;
	}
}

Status Dataset::load(const String& filename,
					 const E_TransferSyntax readXfer,
					 const E_GrpLenEncoding groupLength,
					 const Uint32 maxReadLength)
{
	DcmDataset* dcmDatasetPtr = dynamic_cast<DcmDataset*>(_dcmItemPtr);
	assert(dcmDatasetPtr != NULL);
	Status stat = dcmDatasetPtr->loadFile(filename, readXfer, groupLength, maxReadLength);

	int nls;
	if (getNLS(nls).good())
		setAutoNLS(nls);

	return stat;
}

Status Dataset::save(const String& filename,
					 const E_TransferSyntax writeXfer,
					 const E_EncodingType encodingType,
					 const E_GrpLenEncoding groupLength,
					 const E_PaddingEncoding padEncoding,
					 const Uint32 padLength,
					 const Uint32 subPadLength)
{
	DcmDataset* dcmDatasetPtr = dynamic_cast<DcmDataset*>(_dcmItemPtr);
	assert(dcmDatasetPtr != NULL);

	E_TransferSyntax transferSyntax = (writeXfer == EXS_Unknown) ? getTransferSyntax() : writeXfer;
	Status stat = setTransferSyntax(transferSyntax);
	if (stat.bad())
		return stat;

	return dcmDatasetPtr->saveFile(filename, transferSyntax, encodingType, groupLength, padEncoding, padLength, subPadLength);
}

Status Dataset::setTransferSyntax(E_TransferSyntax transferSyntax, DcmRepresentationParameter* dcmRepParamPtr)
{
	DcmDataset* dcmDatasetPtr = dynamic_cast<DcmDataset*>(_dcmItemPtr);
	assert(dcmDatasetPtr != NULL);
	Status stat = dcmDatasetPtr->chooseRepresentation(transferSyntax, dcmRepParamPtr);
	if (stat.good())
		dcmDatasetPtr->removeAllButCurrentRepresentations();
	return stat;
}

E_TransferSyntax Dataset::getTransferSyntax(void) const
{
	DcmDataset* dcmDatasetPtr= dynamic_cast<DcmDataset*>(_dcmItemPtr);
	assert(dcmDatasetPtr != NULL);
	return dcmDatasetPtr->getCurrentXfer();
}

Status Dataset::getPixelData(const DcmTagKey& tag, PixelDataConsumer* consumerPtr)
{
	Status stat;
	DcmDataset* dcmDatasetPtr = dynamic_cast<DcmDataset*>(_dcmItemPtr);
	assert(dcmDatasetPtr != NULL);

	Uint32 frameCount;
	stat = getValue(DCM_NumberOfFrames, frameCount);
	if (stat.bad())
		frameCount = 1;

	DcmElement* dcmElementPtr = NULL;
	stat = dcmDatasetPtr->findAndGetElement(tag, dcmElementPtr);
	if (stat.bad())
		return stat;

	DcmPixelData* dcmPixelDataPtr = dynamic_cast<DcmPixelData*>(dcmElementPtr);
	E_TransferSyntax transferSyntax = getTransferSyntax();
	if (DcmXfer(transferSyntax).isEncapsulated()) {
		DcmPixelSequence* dcmPixelSeqPtr = NULL;
		stat = dcmPixelDataPtr->getEncapsulatedRepresentation(transferSyntax, NULL, dcmPixelSeqPtr);
		if (stat.bad())
			return EC_InvalidStream;

		DcmPixelItem* dcmPixelItemPtr = NULL;
		Ulong fragmentCount = dcmPixelSeqPtr->card();
		Uint32 fragmentSize = 0;
		Uint8* fragmentDataPtr = NULL;

		// first item: basic offset table
		Uint32* basicOffsetTable = NULL;

		stat = dcmPixelSeqPtr->getItem(dcmPixelItemPtr, 0);	// to read basic offset table
		if (stat.bad())
			return EC_InvalidBasicOffsetTable;

		fragmentSize = dcmPixelItemPtr->getLength();
		if (fragmentSize == 0) {							// without basic offset table
			basicOffsetTable = NULL;
			DCMTK_LOG4CPLUS_DEBUG_FMT(_logger, "getPixelData: encapsulated: no basic offset table");
		} else if (fragmentSize == 4 * frameCount) {		// with basic offset table
			stat = dcmPixelItemPtr->getUint8Array(fragmentDataPtr);
			if (stat.bad())
				return EC_InvalidBasicOffsetTable;

			basicOffsetTable = reinterpret_cast<Uint32*>(fragmentDataPtr);
			DCMTK_LOG4CPLUS_DEBUG_FMT(_logger, "getPixelData: encapsulated: basic offset table (count=%d,size=%d,data=%08x)", frameCount, fragmentSize, fragmentDataPtr);
		} else {											// invalid basic offset table
			return EC_InvalidBasicOffsetTable;
		}

		// from second to last items: encapsulated pixel data
		Uint32 offset = 0;
		Uint32 frameIndex = 0;
		for(int fragmentIndex = 1; fragmentIndex < fragmentCount; fragmentIndex++) {
			stat = dcmPixelSeqPtr->getItem(dcmPixelItemPtr, fragmentIndex);
			if (stat.bad())
				return EC_InvalidStream;

			fragmentSize = dcmPixelItemPtr->getLength();
			stat = dcmPixelItemPtr->getUint8Array(fragmentDataPtr);
			if (stat.bad())
				return EC_InvalidStream;

			if (basicOffsetTable == NULL) {
				frameIndex++;
				assert(frameIndex == fragmentIndex);

				DCMTK_LOG4CPLUS_DEBUG_FMT(_logger, "getPixelData: encapsulated: frame(%d) (size=%d,data=%08x)", frameIndex, fragmentSize, fragmentDataPtr);
				if (consumerPtr != NULL) {
					if (!consumerPtr->onGetPixelData(true, frameIndex, fragmentSize, fragmentDataPtr, true, true))
						return EC_ItemEnd;
				}
			} else {
				bool begin = false;
				if (basicOffsetTable[frameIndex] == offset) {
					frameIndex++;
					begin = true;
				}

				offset += (8 + fragmentSize);

				bool end = false;
				if (((frameIndex < frameCount) && (basicOffsetTable[frameIndex] == offset)) ||
						((frameIndex == frameCount) && (fragmentIndex == fragmentCount - 1))) {
					end = true;
				}

				DCMTK_LOG4CPLUS_DEBUG_FMT(_logger, "getPixelData: encapsulated: frame(%d:%d) (size=%d,data=%08x)", frameIndex, fragmentIndex, fragmentSize, fragmentDataPtr);
				if (consumerPtr != NULL) {
					if (!consumerPtr->onGetPixelData(true, frameIndex, fragmentSize, fragmentDataPtr, begin, end))
						return EC_ItemEnd;
				}
			}
		}
	} else {
		Uint8* pixelDataPtr = NULL;
		stat = dcmPixelDataPtr->getUint8Array(pixelDataPtr);
		if (stat.bad())
			return EC_InvalidStream;

		Uint32 frameSize = 0;
		stat = dcmPixelDataPtr->getUncompressedFrameSize(dcmDatasetPtr, frameSize);

		Uint32 pixelDataSize = dcmPixelDataPtr->getLength();
		if (pixelDataSize != frameSize * frameCount)
			return EC_CorruptedData;

		Uint8* frameDataPtr = pixelDataPtr;
		for(int frameIndex = 0; frameIndex < frameCount; frameIndex++) {
			DCMTK_LOG4CPLUS_DEBUG_FMT(_logger, "getPixelData: uncompressed: frame(%d) (size=%d,data=%08x)", frameIndex+1, frameSize, frameDataPtr);
			if (consumerPtr != NULL) {
				if (!consumerPtr->onGetPixelData(false, frameIndex+1, frameSize, frameDataPtr, true, true))
					return EC_ItemEnd;
			}

			frameDataPtr += frameSize;
		}
	}

	return EC_Normal;
}

Status Dataset::setDefaultNLS(int nls)
{
    _DefaultNLS = DTK_MAX(nls, 0);
	return EC_Normal;
}

Status Dataset::getDefaultNLS(int& nls)
{
	nls = _DefaultNLS;
	return EC_Normal;
}

DcmDataset* Dataset::_getDcmDataset(void) const
{
	DcmDataset* dcmDatasetPtr = dynamic_cast<DcmDataset*>(_dcmItemPtr);
	return dcmDatasetPtr;
}

Dataset::operator DcmDataset*(void) const
{
	DcmDataset* dcmDatasetPtr = dynamic_cast<DcmDataset*>(_dcmItemPtr);
	return dcmDatasetPtr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

Status Dataset::prepareCompositeIODFromMWL(const std::list<dcm::Dataset *> mwlDatasetPtrList)
{
	Status stat;

	if (mwlDatasetPtrList.size() == 0)
		return EC_Normal;

	bool sameOrders = true;
	bool sameRequestedProcedures = true;

	std::list<Dataset*>::const_iterator i = mwlDatasetPtrList.cbegin();
	const Dataset* mwlDatasetPtr_0 = *i;
	const Dataset* mwlDatasetPtr;

	String mwlPatientId_0, mwlAccessionNo_0, mwlStudyInstanceUID_0;
	stat = mwlDatasetPtr_0->getString(DCM_PatientID, mwlPatientId_0);
	stat = mwlDatasetPtr_0->getString(DCM_AccessionNumber, mwlAccessionNo_0);
	stat = mwlDatasetPtr_0->getString(DCM_StudyInstanceUID, mwlStudyInstanceUID_0);

	std::list<String> mwlStudyInstanceUIDList;
	mwlStudyInstanceUIDList.push_back(mwlStudyInstanceUID_0);

	for(i++; i != mwlDatasetPtrList.cend(); i++) {
		mwlDatasetPtr = *i;

		String mwlPatientId, mwlAccessionNumber, mwlStudyInstanceUID;
		stat = mwlDatasetPtr->getString(DCM_PatientID, mwlPatientId);
		stat = mwlDatasetPtr->getString(DCM_AccessionNumber, mwlAccessionNumber);
		stat = mwlDatasetPtr->getString(DCM_StudyInstanceUID, mwlStudyInstanceUID);

		if (mwlPatientId_0 != mwlPatientId) {
			return EC_IllegalParameter;
		}
		if (mwlAccessionNo_0 != mwlAccessionNumber) {
			sameOrders = false;
		}
		if (mwlStudyInstanceUID_0 != mwlStudyInstanceUID) {
			sameRequestedProcedures = false;
		}

	//	std::list<String>::iterator iter = std::find(mwlStudyInstanceUIDList.begin(), mwlStudyInstanceUIDList.end(), mwlStudyInstanceUID);
	//	if (iter == mwlStudyInstanceUIDList.end()) {
	//		mwlStudyInstanceUIDList.push_back(mwlStudyInstanceUID);
	//	}
		mwlStudyInstanceUIDList.push_back(mwlStudyInstanceUID);
	}
	mwlStudyInstanceUIDList.sort();
	mwlStudyInstanceUIDList.unique();

	String accessionNo, studyInstanceUID;
	if (sameOrders) {
		if (sameRequestedProcedures) {
			accessionNo = mwlAccessionNo_0;
			studyInstanceUID = mwlStudyInstanceUID_0;
		} else {
			accessionNo = mwlAccessionNo_0;
			studyInstanceUID = UIDHelper::generateUID(DTK_DEFAULT_UID_ROOT);
		}
	} else {
		accessionNo = "";
		studyInstanceUID = UIDHelper::generateUID(DTK_DEFAULT_UID_ROOT);
	}

	// we use the first MWLDataset as base, generate one if required
	stat = copyValueFrom(DCM_PatientName, mwlDatasetPtr_0);
	stat = copyValueFrom(DCM_PatientID, mwlDatasetPtr_0);
	stat = copyValueFrom(DCM_PatientBirthDate, mwlDatasetPtr_0);
	stat = copyValueFrom(DCM_PatientSex, mwlDatasetPtr_0);

	stat = putString(DCM_AccessionNumber, accessionNo);
	stat = putString(DCM_StudyInstanceUID, studyInstanceUID);

	Item refStudyItem;
	for(std::list<String>::iterator i = mwlStudyInstanceUIDList.begin(); i != mwlStudyInstanceUIDList.end(); i++) {
		stat = putRefSOP(DCM_ReferencedStudySequence, refStudyItem, UID_RETIRED_DetachedStudyManagementSOPClass, *i);	// FIXME: SOP Class UID to confirm
	}

	for(i = mwlDatasetPtrList.cbegin(); i != mwlDatasetPtrList.cend(); i++) {
		mwlDatasetPtr = *i;

		String mwlStudyInstanceUID;
		stat = mwlDatasetPtr->getString(DCM_StudyInstanceUID, mwlStudyInstanceUID);

		Item sourceItem, targetItem;
		stat = mwlDatasetPtr->getItem(DCM_ScheduledProcedureStepSequence, sourceItem);
		stat = copyValueFrom(DCM_Modality, &sourceItem);

		int pos = std::distance(mwlDatasetPtrList.cbegin(), i);
		stat = putItem(DCM_RequestAttributesSequence, targetItem, pos);
		{
			stat = targetItem.copyValueFrom(DCM_RequestedProcedureID, mwlDatasetPtr);
			stat = targetItem.copyValueFrom(DCM_RequestedProcedureDescription, mwlDatasetPtr);
			stat = targetItem.copyValueFrom(DCM_RequestedProcedureCodeSequence, mwlDatasetPtr);
			stat = targetItem.copyValueFrom(DCM_ScheduledProcedureStepID, &sourceItem);
			stat = targetItem.copyValueFrom(DCM_ScheduledProcedureStepDescription, &sourceItem);
			stat = targetItem.copyValueFrom(DCM_ScheduledProtocolCodeSequence, &sourceItem);
			stat = targetItem.copyValueFrom(DCM_AccessionNumber, mwlDatasetPtr);
			stat = targetItem.putString(DCM_StudyInstanceUID, studyInstanceUID);
			stat = targetItem.putRefSOP(DCM_ReferencedStudySequence, refStudyItem, UID_RETIRED_DetachedStudyManagementSOPClass, mwlStudyInstanceUID);	// FIXME: SOP Class UID to confirm
		}

		stat = copyValueFrom(DCM_PerformedProcedureCodeSequence, &sourceItem, DCM_ScheduledProtocolCodeSequence);
	}

	return EC_Normal;
}

Status Dataset::prepareMPPSFromMWL(const std::list<Dataset*> mwlDatasetPtrList)
{
	Status stat;

	if (mwlDatasetPtrList.size() == 0)
		return EC_Normal;

	bool sameOrders = true;
	bool sameRequestedProcedures = true;
	std::list<Dataset*>::const_iterator i = mwlDatasetPtrList.cbegin();
	const Dataset* mwlDatasetPtr_0 = *i;
	const Dataset* mwlDatasetPtr;

	String mwlPatientId_0, mwlAccessionNo_0, mwlStudyInstanceUID_0;
	stat = mwlDatasetPtr_0->getString(DCM_PatientID, mwlPatientId_0);
	stat = mwlDatasetPtr_0->getString(DCM_AccessionNumber, mwlAccessionNo_0);
	stat = mwlDatasetPtr_0->getString(DCM_StudyInstanceUID, mwlStudyInstanceUID_0);

	for(i++; i != mwlDatasetPtrList.cend(); i++) {
		mwlDatasetPtr = *i;

		String mwlPatientId, mwlAccessionNumber, mwlStudyInstanceUID;
		stat = mwlDatasetPtr->getString(DCM_PatientID, mwlPatientId);
		stat = mwlDatasetPtr->getString(DCM_AccessionNumber, mwlAccessionNumber);
		stat = mwlDatasetPtr->getString(DCM_StudyInstanceUID, mwlStudyInstanceUID);

		if (mwlPatientId_0 != mwlPatientId) {
			return EC_IllegalParameter;
		}
		if (mwlAccessionNo_0 != mwlAccessionNumber) {
			sameOrders = false;
		}
		if (mwlStudyInstanceUID_0 != mwlStudyInstanceUID) {
			sameRequestedProcedures = false;
		}
	}

	String studyInstanceUID;
	if (sameOrders) {
		if (sameRequestedProcedures) {
			studyInstanceUID = mwlStudyInstanceUID_0;
		} else {
			studyInstanceUID = UIDHelper::generateUID(DTK_DEFAULT_UID_ROOT);
		}
	} else {
		studyInstanceUID = UIDHelper::generateUID(DTK_DEFAULT_UID_ROOT);
	}

	// we use the first MWLDataset as base, generate one if required
	stat = copyValueFrom(DCM_PatientName, mwlDatasetPtr_0);
	stat = copyValueFrom(DCM_PatientID, mwlDatasetPtr_0);
	stat = copyValueFrom(DCM_PatientBirthDate, mwlDatasetPtr_0);
	stat = copyValueFrom(DCM_PatientSex, mwlDatasetPtr_0);

	for(i = mwlDatasetPtrList.cbegin(); i != mwlDatasetPtrList.cend(); i++) {
		mwlDatasetPtr = *i;

		String mwlStudyInstanceUID;
		stat = mwlDatasetPtr->getString(DCM_StudyInstanceUID, mwlStudyInstanceUID);

		Item sourceItem, targetItem, refStudyItem;
		stat = mwlDatasetPtr->getItem(DCM_ScheduledProcedureStepSequence, sourceItem);
		stat = copyValueFrom(DCM_Modality, &sourceItem);

		int pos = std::distance(mwlDatasetPtrList.cbegin(), i);
		stat = putItem(DCM_ScheduledStepAttributesSequence, targetItem, pos);
		{
			stat = targetItem.copyValueFrom(DCM_AccessionNumber, mwlDatasetPtr);
			stat = targetItem.putString(DCM_StudyInstanceUID, studyInstanceUID);
			stat = targetItem.putRefSOP(DCM_ReferencedStudySequence, refStudyItem, UID_RETIRED_DetachedStudyManagementSOPClass, mwlStudyInstanceUID);	// FIXME: SOP Class UID to confirm
			stat = targetItem.copyValueFrom(DCM_RequestedProcedureID, mwlDatasetPtr);
			stat = targetItem.copyValueFrom(DCM_RequestedProcedureDescription, mwlDatasetPtr);
			stat = targetItem.copyValueFrom(DCM_RequestedProcedureCodeSequence, mwlDatasetPtr);
			stat = targetItem.copyValueFrom(DCM_ScheduledProcedureStepID, &sourceItem);
			stat = targetItem.copyValueFrom(DCM_ScheduledProcedureStepDescription, &sourceItem);
			stat = targetItem.copyValueFrom(DCM_ScheduledProtocolCodeSequence, &sourceItem);
		}
	}

	return EC_Normal;
}

Status Dataset::prepareMPPSFromCompositeIOD(const Dataset* datasetPtr)
{
	Status status;
	Item sourceItem, targetItem;

	if (datasetPtr == NULL)
		return EC_IllegalParameter;

	int nNLS;
	if (datasetPtr->getNLS(nNLS).good())
		setNLS(nNLS);

	// Performed Procedure Step Relationship
	for(Uint32 pos = 0; datasetPtr->getItem(DCM_RequestAttributesSequence, sourceItem, pos).good() ; pos++) {
		status = putItem(DCM_ScheduledStepAttributesSequence, targetItem, pos);
		{
			status = targetItem.copyValueFrom(DCM_ReferencedStudySequence, datasetPtr);
			status = targetItem.copyValueFrom(DCM_AccessionNumber, &sourceItem);
			status = targetItem.copyValueFrom(DCM_StudyInstanceUID, datasetPtr);
			status = targetItem.copyValueFrom(DCM_PlacerOrderNumberImagingServiceRequest, &sourceItem);
			status = targetItem.copyValueFrom(DCM_FillerOrderNumberImagingServiceRequest, &sourceItem);
			status = targetItem.copyValueFrom(DCM_RequestedProcedureID, &sourceItem);
			status = targetItem.copyValueFrom(DCM_RequestedProcedureDescription, &sourceItem);
			status = targetItem.copyValueFrom(DCM_RequestedProcedureCodeSequence, &sourceItem);
			status = targetItem.copyValueFrom(DCM_ScheduledProcedureStepID, &sourceItem);
			status = targetItem.copyValueFrom(DCM_ScheduledProcedureStepDescription, &sourceItem);
			status = targetItem.copyValueFrom(DCM_ScheduledProtocolCodeSequence, &sourceItem);
		}
	}
	status = copyValueFrom(DCM_PatientName, datasetPtr);
	status = copyValueFrom(DCM_PatientID, datasetPtr);
	status = copyValueFrom(DCM_IssuerOfPatientID, datasetPtr);
	status = copyValueFrom(DCM_PatientBirthDate, datasetPtr);
	status = copyValueFrom(DCM_PatientSex, datasetPtr);
	status = copyValueFrom(DCM_AdmissionID, datasetPtr);
	status = copyValueFrom(DCM_RETIRED_IssuerOfAdmissionID, datasetPtr);

	// Performed Procedure Step Information
	status = copyValueFrom(DCM_PerformedProcedureStepID, datasetPtr);
	status = copyValueFrom(DCM_PerformedProcedureStepStartDate, datasetPtr, DCM_StudyDate);
	status = copyValueFrom(DCM_PerformedProcedureStepStartTime, datasetPtr, DCM_StudyTime);
	status = copyValueFrom(DCM_PerformedProcedureStepDescription, datasetPtr, DCM_StudyDescription);
	status = putEmpty(DCM_PerformedProcedureTypeDescription);
	status = copyValueFrom(DCM_ProcedureCodeSequence, datasetPtr);

	// Image Acquisition Results
	status = copyValueFrom(DCM_Modality, datasetPtr);
	status = copyValueFrom(DCM_StudyID, datasetPtr);
	status = copyValueFrom(DCM_PerformedProtocolCodeSequence, datasetPtr);

	return EC_Normal;
}

Status Dataset::prepareMPPS2FromCompositeIOD(const Dataset* datasetPtr)
{
	Status status;

	if (datasetPtr == NULL)
		return EC_IllegalParameter;

	int nNLS;
	if (datasetPtr->getNLS(nNLS).good())
		setNLS(nNLS);

	// Image Acquisition Results
	String seriesInstanceUID;
	String sopClassUID, sopInstanceUID;
	Item seriesItem, instanceItem;
	Sint32 seriesIndex, instanceIndex;
	String str;

	status = datasetPtr->getString(DCM_SeriesInstanceUID, seriesInstanceUID);
	for(seriesIndex = 0; getItem(DCM_PerformedSeriesSequence, seriesItem, seriesIndex).good(); seriesIndex++) {
		status = seriesItem.getString(DCM_SeriesInstanceUID, str);
		if (str.compare(seriesInstanceUID) == 0)
			goto series_done;
	}
	// series not found
	status = putItem(DCM_PerformedSeriesSequence, seriesItem);
	{
		status = seriesItem.copyValueFrom(DCM_SeriesInstanceUID, datasetPtr);
		status = seriesItem.copyValueFrom(DCM_SeriesDescription, datasetPtr);
		status = seriesItem.copyValueFrom(DCM_PerformingPhysicianName, datasetPtr);
		status = seriesItem.copyValueFrom(DCM_OperatorsName, datasetPtr);
		status = seriesItem.copyValueFrom(DCM_ProtocolName, datasetPtr);
	}

series_done:
	status = datasetPtr->getString(DCM_SOPClassUID, sopClassUID);
	status = datasetPtr->getString(DCM_SOPInstanceUID, sopInstanceUID);
	if (UIDHelper::isImageSOPClassUID(sopClassUID)) {
		for(instanceIndex = 0; seriesItem.getItem(DCM_ReferencedImageSequence, instanceItem, instanceIndex).good(); instanceIndex++) {
			status = instanceItem.getString(DCM_ReferencedSOPInstanceUID, str);
			if (str.compare(sopInstanceUID) == 0)
				goto instance_done;
		}
		// instance not found
		status = seriesItem.putItem(DCM_ReferencedImageSequence, instanceItem);
		status = instanceItem.copyValueFrom(DCM_ReferencedSOPClassUID, datasetPtr, DCM_SOPClassUID);
		status = instanceItem.copyValueFrom(DCM_ReferencedSOPInstanceUID, datasetPtr, DCM_SOPInstanceUID);
	} else {
		for(instanceIndex = 0; seriesItem.getItem(DCM_ReferencedNonImageCompositeSOPInstanceSequence, instanceItem, instanceIndex).good(); instanceIndex++) {
			status = instanceItem.getString(DCM_ReferencedSOPInstanceUID, str);
			if (str.compare(sopInstanceUID) == 0)
				goto instance_done;
		}
		// instance not found
		status = seriesItem.putItem(DCM_ReferencedNonImageCompositeSOPInstanceSequence, instanceItem);
		status = instanceItem.copyValueFrom(DCM_ReferencedSOPClassUID, datasetPtr, DCM_SOPClassUID);
		status = instanceItem.copyValueFrom(DCM_ReferencedSOPInstanceUID, datasetPtr, DCM_SOPInstanceUID);
	}

instance_done:
	return EC_Normal;
}

Status Dataset::prepareStorageCommitmentFromCompositeIOD(const Dataset* datasetPtr)
{
	Status stat;

	String sopClassUID, sopInstanceUID;
	stat = datasetPtr->getString(DCM_SOPClassUID, sopClassUID);
	stat = datasetPtr->getString(DCM_SOPInstanceUID, sopInstanceUID);

	Item item;
	stat = putRefSOP(DCM_ReferencedSOPSequence, item, sopClassUID, sopInstanceUID);

	return EC_Normal;
}
