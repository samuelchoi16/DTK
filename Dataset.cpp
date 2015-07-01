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

#define	D								(0x0001)
#define	Z								(0x0002)
#define	X								(0x0004)
#define	K								(0x0008)
#define	C								(0x0010)
#define	U								(0x0020)

#define	Z_D								(Z | D)
#define	X_Z								(X | Z)
#define	X_D								(X | D)
#define	X_Z_D							(X | Z | D)
#define	X_Z_U							(X | Z | U)

struct AttributeConfidentiality {
	DcmTagKey tag;
	int basicProfileAction;
};

static AttributeConfidentiality attributeConfidentialityList[] = {
	{	DCM_AccessionNumber,									Z	},
	{	DCM_RETIRED_AcquisitionComments,						X	},
	{	DCM_AcquisitionContextSequence,							X	},
	{	DCM_AcquisitionDate,									X_Z	},
	{	DCM_AcquisitionDateTime,								X_D	},
	{	DCM_AcquisitionDeviceProcessingDescription,				X_D	},
	{	DCM_AcquisitionProtocolDescription,						X	},
	{	DCM_AcquisitionTime,									X_Z	},
	{	DCM_ActualHumanPerformersSequence,						X	},
	{	DCM_AdditionalPatientHistory,							X	},
	{	DCM_RETIRED_AddressTrial,								X	},
	{	DCM_AdmissionID,										X	},
	{	DCM_AdmittingDate,										X	},
	{	DCM_AdmittingDiagnosesCodeSequence,						X	},
	{	DCM_AdmittingDiagnosesDescription,						X	},
	{	DCM_AdmittingTime,										X	},
	{	DCM_AffectedSOPInstanceUID,								X	},
	{	DCM_Allergies,											X	},
	{	DCM_RETIRED_Arbitrary,									X	},
	{	DCM_AuthorObserverSequence,								X	},
	{	DCM_BranchOfService,									X	},
	{	DCM_CassetteID,											X	},
	{	DCM_CommentsOnThePerformedProcedureStep,				X	},
	{	DCM_ConcatenationUID,									U	},
	{	DCM_ConfidentialityConstraintOnPatientDataDescription,	X	},
	{	DCM_ContentCreatorName,									Z	},
	{	DCM_ContentCreatorIdentificationCodeSequence,			X	},
	{	DCM_ContentDate,										Z_D	},
	{	DCM_ContentSequence,									X	},
	{	DCM_ContentTime,										Z_D	},
	{	DCM_ContextGroupExtensionCreatorUID,					U	},
	{	DCM_ContrastBolusAgent,									Z_D	},
	{	DCM_ContributionDescription,							X	},
	{	DCM_CountryOfResidence,									X	},
	{	DCM_CreatorVersionUID,									U	},
	{	DCM_RETIRED_CurrentObserverTrial,						X	},
	{	DCM_CurrentPatientLocation,								X	},
	{	DCM_RETIRED_CurveData,									X	},
	{	DCM_RETIRED_CurveDate,									X	},
	{	DCM_RETIRED_CurveTime,									X	},
	{	DCM_CustodialOrganizationSequence,						X	},
	{	DCM_DataSetTrailingPadding,								X	},
	{	DCM_DerivationDescription,								X	},
	{	DCM_DetectorID,											X_D	},
	{	DCM_DeviceSerialNumber,									X_Z_D	},
	{	DCM_DeviceUID,											U	},
	{	DCM_DigitalSignatureUID,								X	},
	{	DCM_DigitalSignaturesSequence,							X	},
	{	DCM_DimensionOrganizationUID,							U	},
	{	DCM_RETIRED_DischargeDiagnosisDescription,				X	},
	{	DCM_RETIRED_DistributionAddress,						X	},
	{	DCM_RETIRED_DistributionName,							X	},
	{	DCM_DoseReferenceUID,									U	},
	{	DCM_EndAcquisitionDateTime,								X_D	},
	{	DCM_EthnicGroup,										X	},
	{	DCM_ExpectedCompletionDateTime,							X	},
	{	DCM_FailedSOPInstanceUIDList,							U	},
	{	DCM_FiducialUID,										U	},
	{	DCM_RETIRED_FillerOrderNumberImagingServiceRequestRetired,	Z	},
	{	DCM_FrameComments,										X	},
	{	DCM_FrameOfReferenceUID,								U	},
	{	DCM_GantryID,											X	},
	{	DCM_GeneratorID,										X	},
	{	DCM_GraphicAnnotationSequence,							D	},
	{	DCM_HumanPerformerName,									X	},
	{	DCM_HumanPerformerOrganization,							X	},
	{	DCM_IconImageSequence,									X	},
	{	DCM_RETIRED_IdentifyingComments,						X	},
	{	DCM_ImageComments,										X	},
	{	DCM_RETIRED_ImagePresentationComments,					X	},
	{	DCM_ImagingServiceRequestComments,						X	},
	{	DCM_RETIRED_Impressions,								X	},
	{	DCM_InstanceCoercionDateTime,							X	},
	{	DCM_InstanceCreatorUID,									U	},
	{	DCM_InstitutionAddress,									X	},
	{	DCM_InstitutionCodeSequence,							X_Z_D	},
	{	DCM_InstitutionName,									X_Z_D	},
	{	DCM_InstitutionalDepartmentName,						X	},
	{	DCM_RETIRED_InsurancePlanIdentification,				X	},
	{	DCM_IntendedRecipientsOfResultsIdentificationSequence,	X	},
	{	DCM_RETIRED_InterpretationApproverSequence,				X	},
	{	DCM_RETIRED_InterpretationAuthor,						X	},
	{	DCM_RETIRED_InterpretationDiagnosisDescription,			X	},
	{	DCM_RETIRED_InterpretationIDIssuer,						X	},
	{	DCM_RETIRED_InterpretationRecorder,						X	},
	{	DCM_RETIRED_InterpretationText,							X	},
	{	DCM_RETIRED_InterpretationTranscriber,					X	},
	{	DCM_IrradiationEventUID,								U	},
	{	DCM_RETIRED_IssuerOfAdmissionID,						X	},
	{	DCM_IssuerOfPatientID,									X	},
	{	DCM_RETIRED_IssuerOfServiceEpisodeID,					X	},
	{	DCM_RETIRED_LargePaletteColorLookupTableUID,			U	},
	{	DCM_LastMenstrualDate,									X	},
	{	DCM_MAC,												X	},
	{	DCM_MediaStorageSOPInstanceUID,							U	},
	{	DCM_MedicalAlerts,										X	},
	{	DCM_MedicalRecordLocator,								X	},
	{	DCM_MilitaryRank,										X	},
	{	DCM_ModifiedAttributesSequence,							X	},
	{	DCM_RETIRED_ModifiedImageDescription,					X	},
	{	DCM_RETIRED_ModifyingDeviceID,							X	},
	{	DCM_RETIRED_ModifyingDeviceManufacturer,				X	},
	{	DCM_NameOfPhysiciansReadingStudy,						X	},
	{	DCM_NamesOfIntendedRecipientsOfResults,					X	},
	{	DCM_RETIRED_ObservationDateTrial,						X	},
	{	DCM_RETIRED_ObservationSubjectUIDTrial,					U	},
	{	DCM_RETIRED_ObservationTimeTrial,						X	},
	{	DCM_ObservationUID,										U	},
	{	DCM_Occupation,											X	},
	{	DCM_OperatorIdentificationSequence,						X_D	},
	{	DCM_OperatorsName,										X_Z_D	},
	{	DCM_OriginalAttributesSequence,							X	},
	{	DCM_OrderCallbackPhoneNumber,							X	},
	{	DCM_OrderEnteredBy,										X	},
	{	DCM_OrderEntererLocation,								X	},
	{	DCM_OtherPatientIDs,									X	},
	{	DCM_OtherPatientIDsSequence,							X	},
	{	DCM_OtherPatientNames,									X	},
	{	DCM_RETIRED_OverlayComments,							X	},
	{	DCM_OverlayData,										X	},
	{	DCM_RETIRED_OverlayDate,								X	},
	{	DCM_RETIRED_OverlayTime,								X	},
	{	DCM_PaletteColorLookupTableUID,							U	},
	{	DCM_ParticipantSequence,								X	},
	{	DCM_PatientAddress,										X	},
	{	DCM_PatientComments,									X	},
	{	DCM_PatientID,											Z	},
	{	DCM_PatientSexNeutered,									X_Z	},
	{	DCM_PatientState,										X	},
	{	DCM_PatientTransportArrangements,						X	},
	{	DCM_PatientAge,											X	},
	{	DCM_PatientBirthDate,									Z	},
	{	DCM_PatientBirthName,									X	},
	{	DCM_PatientBirthTime,									X	},
	{	DCM_PatientInstitutionResidence,						X	},
	{	DCM_PatientInsurancePlanCodeSequence,					X	},
	{	DCM_PatientMotherBirthName,								X	},
	{	DCM_PatientName,										Z	},
	{	DCM_PatientPrimaryLanguageCodeSequence,					X	},
	{	DCM_PatientPrimaryLanguageModifierCodeSequence,			X	},
	{	DCM_PatientReligiousPreference,							X	},
	{	DCM_PatientSex,											Z	},
	{	DCM_PatientSize,										X	},
	{	DCM_PatientTelephoneNumbers,							X	},
	{	DCM_PatientWeight,										X	},
	{	DCM_PerformedLocation,									X	},
	{	DCM_PerformedProcedureStepDescription,					X	},
	{	DCM_PerformedProcedureStepEndDate,						X	},
	{	DCM_PerformedProcedureStepEndDateTime,					X	},
	{	DCM_PerformedProcedureStepEndTime,						X	},
	{	DCM_PerformedProcedureStepID,							X	},
	{	DCM_PerformedProcedureStepStartDate,					X	},
	{	DCM_PerformedProcedureStepStartDateTime,				X	},
	{	DCM_PerformedProcedureStepStartTime,					X	},
	{	DCM_PerformedStationAETitle,							X	},
	{	DCM_PerformedStationGeographicLocationCodeSequence,		X	},
	{	DCM_PerformedStationName,								X	},
	{	DCM_PerformedStationNameCodeSequence,					X	},
	{	DCM_PerformingPhysicianIdentificationSequence,			X	},
	{	DCM_PerformingPhysicianName,							X	},
	{	DCM_PersonAddress,										X	},
	{	DCM_PersonIdentificationCodeSequence,					D	},
	{	DCM_PersonName,											D	},
	{	DCM_PersonTelephoneNumbers,								X	},
	{	DCM_RETIRED_PhysicianApprovingInterpretation,			X	},
	{	DCM_PhysiciansReadingStudyIdentificationSequence,		X	},
	{	DCM_PhysiciansOfRecord,									X	},
	{	DCM_PhysiciansOfRecordIdentificationSequence,			X	},
	{	DCM_RETIRED_PlacerOrderNumberImagingServiceRequestRetired,	Z	},
	{	DCM_PlateID,											X	},
	{	DCM_PreMedication,										X	},
	{	DCM_PregnancyStatus,									X	},
	{	DCM_ProcedureStepCancellationDateTime,					X	},
//	{	DCM_PrivateAttributes,									X	},
	{	DCM_ProtocolName,										X_D	},
	{	DCM_RETIRED_ReasonForTheImagingServiceRequest,			X	},
	{	DCM_RETIRED_ReasonForStudy,								X	},
	{	DCM_ReferencedDigitalSignatureSequence,					X	},
	{	DCM_ReferencedFrameOfReferenceUID,						U	},
	{	DCM_RETIRED_ReferencedGeneralPurposeScheduledProcedureStepSequence,	U	},
	{	DCM_ReferencedImageSequence,							X_Z_U	},
	{	DCM_RETIRED_ReferencedObservationUIDTrial,				U	},
	{	DCM_ReferencedPatientAliasSequence,						X	},
	{	DCM_ReferencedPatientPhotoSequence,						X	},
	{	DCM_ReferencedPatientSequence,							X	},
	{	DCM_ReferencedPerformedProcedureStepSequence,			X_Z_D	},
	{	DCM_ReferencedSOPInstanceMACSequence,					X	},
	{	DCM_ReferencedSOPInstanceUID,							U	},
	{	DCM_ReferencedSOPInstanceUIDInFile,						U	},
	{	DCM_ReferencedStudySequence,							X_Z	},
	{	DCM_ReferringPhysicianAddress,							X	},
	{	DCM_ReferringPhysicianIdentificationSequence,			X	},
	{	DCM_ReferringPhysicianName,								Z	},
	{	DCM_ReferringPhysicianTelephoneNumbers,					X	},
	{	DCM_RegionOfResidence,									X	},
	{	DCM_RETIRED_RelatedFrameOfReferenceUID,					U	},
	{	DCM_RequestAttributesSequence,							X	},
	{	DCM_RequestedContrastAgent,								X	},
	{	DCM_RequestedProcedureComments,							X	},
	{	DCM_RequestedProcedureDescription,						X_Z	},
	{	DCM_RequestedProcedureID,								X	},
	{	DCM_RequestedProcedureLocation,							X	},
	{	DCM_RequestedSOPInstanceUID,							U	},
	{	DCM_RequestingPhysician,								X	},
	{	DCM_RequestingService,									X	},
	{	DCM_ResponsibleOrganization,							X	},
	{	DCM_ResponsiblePerson,									X	},
	{	DCM_RETIRED_ResultsComments,							X	},
	{	DCM_RETIRED_ResultsDistributionListSequence,			X	},
	{	DCM_RETIRED_ResultsIDIssuer,							X	},
	{	DCM_ReviewerName,										X_Z	},
	{	DCM_ScheduledHumanPerformersSequence,					X	},
	{	DCM_RETIRED_ScheduledPatientInstitutionResidence,		X	},
	{	DCM_ScheduledPerformingPhysicianIdentificationSequence,	X	},
	{	DCM_ScheduledPerformingPhysicianName,					X	},
	{	DCM_ScheduledProcedureStepEndDate,						X	},
	{	DCM_ScheduledProcedureStepEndTime,						X	},
	{	DCM_ScheduledProcedureStepDescription,					X	},
	{	DCM_ScheduledProcedureStepLocation,						X	},
	{	DCM_ScheduledProcedureStepModificationDateTime,			X	},
	{	DCM_ScheduledProcedureStepStartDate,					X	},
	{	DCM_ScheduledProcedureStepStartDateTime,				X	},
	{	DCM_ScheduledProcedureStepStartTime,					X	},
	{	DCM_ScheduledStationAETitle,							X	},
	{	DCM_ScheduledStationGeographicLocationCodeSequence,		X	},
	{	DCM_ScheduledStationName,								X	},
	{	DCM_ScheduledStationNameCodeSequence,					X	},
	{	DCM_RETIRED_ScheduledStudyLocation,						X	},
	{	DCM_RETIRED_ScheduledStudyLocationAETitle,				X	},
	{	DCM_SeriesDate,											X_D	},
	{	DCM_SeriesDescription,									X	},
	{	DCM_SeriesInstanceUID,									U	},
	{	DCM_SeriesTime,											X_D	},
	{	DCM_ServiceEpisodeDescription,							X	},
	{	DCM_ServiceEpisodeID,									X	},
	{	DCM_SmokingStatus,										X	},
	{	DCM_SOPInstanceUID,										U	},
	{	DCM_SourceImageSequence,								X_Z_U	},
	{	DCM_SourceSerialNumber,									X	},
	{	DCM_SpecialNeeds,										X	},
	{	DCM_StartAcquisitionDateTime,							X_D	},
	{	DCM_StationName,										X_Z_D	},
	{	DCM_StorageMediaFileSetUID,								U	},
	{	DCM_RETIRED_StudyComments,								X	},
	{	DCM_StudyDate,											Z	},
	{	DCM_StudyDescription,									X	},
	{	DCM_StudyID,											Z	},
	{	DCM_RETIRED_StudyIDIssuer,								X	},
	{	DCM_StudyInstanceUID,									U	},
	{	DCM_StudyTime,											Z	},
	{	DCM_SynchronizationFrameOfReferenceUID,					U	},
	{	DCM_TargetUID,											U	},
	{	DCM_RETIRED_TelephoneNumberTrial,						X	},
	{	DCM_RETIRED_TemplateExtensionCreatorUID,				U	},
	{	DCM_RETIRED_TemplateExtensionOrganizationUID,			U	},
	{	DCM_RETIRED_TextComments,								X	},
	{	DCM_TextString,											X	},
	{	DCM_TimezoneOffsetFromUTC,								X	},
	{	DCM_RETIRED_TopicAuthor,								X	},
	{	DCM_RETIRED_TopicKeywords,								X	},
	{	DCM_RETIRED_TopicSubject,								X	},
	{	DCM_RETIRED_TopicTitle,									X	},
	{	DCM_TransactionUID,										U	},
	{	DCM_UID,												U	},
	{	DCM_RETIRED_VerbalSourceTrial,							X	},
	{	DCM_RETIRED_VerbalSourceIdentifierCodeSequenceTrial,	X	},
	{	DCM_VerifyingObserverIdentificationCodeSequence,		Z	},
	{	DCM_VerifyingObserverName,								D	},
	{	DCM_VerifyingObserverSequence,							D	},
	{	DCM_VerifyingOrganization,								X	},
	{	DCM_VisitComments,										X	},
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

Logger Dataset::_logger = Logger::getInstance("dcm.Dataset");

Dataset::Dataset(void)
{
	if (_created)
		delete _dcmItem;

	_created = true;
	_dcmItem = new DcmDataset;

	setAutoNLS(_DefaultNLS);
	setNLS(_DefaultNLS);
}

Dataset::Dataset(DcmDataset* dcmDataset)
	: Item(dcmDataset)
{
	int nls;
	if (getNLS(nls).good() && nls > 0)
		setAutoNLS(nls);
}

Dataset::Dataset(const Dataset& dataset)
{
	if (_created)
		delete _dcmItem;

	_created = true;
	_dcmItem = new DcmDataset(*dynamic_cast<DcmDataset*>(dataset._dcmItem));

	int nls;
	if (dataset.getAutoNLS(nls).good())
		setAutoNLS(nls);
	if (dataset.getNLS(nls).good())
		setNLS(nls);
}

Dataset::~Dataset(void)
{
	if (_created) {
		delete _dcmItem;
		_dcmItem = NULL;
	}
}

Dataset& Dataset::operator=(const Dataset& dataset)
{
	Status dcmStat = copyFrom(&dataset);
	return *this;
}

Status Dataset::copyFrom(const Dataset* sourceDataset)
{
	if (sourceDataset == NULL)
		return EC_IllegalParameter;
	if (sourceDataset == this)
		return EC_Normal;

	clear();

	TagList tagList;
	if (sourceDataset->getTagList(tagList).good()) {
		for(TagList::iterator ti = tagList.begin(); ti != tagList.end(); ti++)
			copyValueFrom(*ti, sourceDataset);

		int nls;
		if (sourceDataset->getAutoNLS(nls).good())
			setAutoNLS(nls);
		if (sourceDataset->getNLS(nls).good())
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
	DcmDataset* dcmDatasetPtr = dynamic_cast<DcmDataset*>(_dcmItem);
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
	DcmDataset* dcmDatasetPtr = dynamic_cast<DcmDataset*>(_dcmItem);
	assert(dcmDatasetPtr != NULL);

	E_TransferSyntax transferSyntax = (writeXfer == EXS_Unknown) ? getTransferSyntax() : writeXfer;
	Status stat = setTransferSyntax(transferSyntax);
	if (stat.bad())
		return stat;

	return dcmDatasetPtr->saveFile(filename, transferSyntax, encodingType, groupLength, padEncoding, padLength, subPadLength);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Dataset::registerCodecs()
{
	DcmRLEEncoderRegistration::registerCodecs();
	DJEncoderRegistration::registerCodecs();
	DJLSEncoderRegistration::registerCodecs();

	DcmRLEDecoderRegistration::registerCodecs();
	DJDecoderRegistration::registerCodecs();
	DJLSDecoderRegistration::registerCodecs();
}

void Dataset::unregisterCodecs()
{
	DcmRLEEncoderRegistration::cleanup();
	DJEncoderRegistration::cleanup();
	DJLSEncoderRegistration::cleanup();

	DcmRLEDecoderRegistration::cleanup();
	DJDecoderRegistration::cleanup();
	DJLSDecoderRegistration::cleanup();
}

Status Dataset::setTransferSyntax(E_TransferSyntax transferSyntax, DcmRepresentationParameter* dcmRepParam)
{
	DcmDataset* dcmDatasetPtr = dynamic_cast<DcmDataset*>(_dcmItem);
	assert(dcmDatasetPtr != NULL);
	Status stat = dcmDatasetPtr->chooseRepresentation(transferSyntax, dcmRepParam);
	if (stat.good())
		dcmDatasetPtr->removeAllButCurrentRepresentations();
	return stat;
}

E_TransferSyntax Dataset::getTransferSyntax(void) const
{
	DcmDataset* dcmDatasetPtr= dynamic_cast<DcmDataset*>(_dcmItem);
	assert(dcmDatasetPtr != NULL);
	return dcmDatasetPtr->getCurrentXfer();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

Status Dataset::getPixelData(const DcmTagKey& tag, PixelDataConsumer* consumer)
{
	Status stat;
	DcmDataset* dcmDatasetPtr = dynamic_cast<DcmDataset*>(_dcmItem);
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
		for(Uint fragmentIndex = 1; fragmentIndex < fragmentCount; fragmentIndex++) {
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
				if (consumer != NULL) {
					if (!consumer->onGetPixelData(true, frameIndex, fragmentSize, fragmentDataPtr, true, true))
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
				if (consumer != NULL) {
					if (!consumer->onGetPixelData(true, frameIndex, fragmentSize, fragmentDataPtr, begin, end))
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
		for(Uint frameIndex = 0; frameIndex < frameCount; frameIndex++) {
			DCMTK_LOG4CPLUS_DEBUG_FMT(_logger, "getPixelData: uncompressed: frame(%d) (size=%d,data=%08x)", frameIndex+1, frameSize, frameDataPtr);
			if (consumer != NULL) {
				if (!consumer->onGetPixelData(false, frameIndex+1, frameSize, frameDataPtr, true, true))
					return EC_ItemEnd;
			}

			frameDataPtr += frameSize;
		}
	}

	return EC_Normal;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

Status Dataset::deidentify(void)
{
	Status stat;

	int count = sizeof(attributeConfidentialityList) / sizeof(AttributeConfidentiality);
	for(int i = 0; i < count; i++) {
		DcmTagKey tag = attributeConfidentialityList[i].tag;
		int action = attributeConfidentialityList[i].basicProfileAction;

		if (action & D) {
			DcmEVR vr = DcmTag(tag).getEVR();
			switch(vr) {
			case EVR_AS :
			case EVR_DA :
			case EVR_DT :
			case EVR_TM :
				stat = putEmpty(tag);
				break;
			default :
				stat = putString(tag, "-");
				break;
			}
		} else if (action & Z) {
			stat = putEmpty(tag);
		} else if (action == X) {
			stat = removeValue(tag);
		}
	}

	TagList tagList;
	stat = getTagList(tagList);
	if (stat.good()) {
		for(TagList::iterator i = tagList.begin(); i != tagList.end(); i++) {
			DcmTagKey tag = *i;
			if (tag.getGroup() % 2 != 0)
				stat = removeValue(tag);
		}
	}

	return EC_Normal;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
	DcmDataset* dcmDatasetPtr = dynamic_cast<DcmDataset*>(_dcmItem);
	return dcmDatasetPtr;
}

Dataset::operator DcmDataset*(void) const
{
	DcmDataset* dcmDatasetPtr = dynamic_cast<DcmDataset*>(_dcmItem);
	return dcmDatasetPtr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

Status Dataset::prepareCompositeIODFromMWL(const std::list<dcm::Dataset *> mwlDatasetList)
{
	Status stat;

	if (mwlDatasetList.size() == 0)
		return EC_Normal;

	bool sameOrders = true;
	bool sameRequestedProcedures = true;

	std::list<Dataset*>::const_iterator i = mwlDatasetList.cbegin();
	const Dataset* mwlDatasetPtr_0 = *i;
	const Dataset* mwlDatasetPtr;

	String mwlPatientId_0, mwlAccessionNo_0, mwlStudyInstanceUID_0;
	stat = mwlDatasetPtr_0->getString(DCM_PatientID, mwlPatientId_0);
	stat = mwlDatasetPtr_0->getString(DCM_AccessionNumber, mwlAccessionNo_0);
	stat = mwlDatasetPtr_0->getString(DCM_StudyInstanceUID, mwlStudyInstanceUID_0);

	std::list<String> mwlStudyInstanceUIDList;
	mwlStudyInstanceUIDList.push_back(mwlStudyInstanceUID_0);

	for(i++; i != mwlDatasetList.cend(); i++) {
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

	for(i = mwlDatasetList.cbegin(); i != mwlDatasetList.cend(); i++) {
		mwlDatasetPtr = *i;

		String mwlStudyInstanceUID;
		stat = mwlDatasetPtr->getString(DCM_StudyInstanceUID, mwlStudyInstanceUID);

		Item sourceItem, targetItem;
		stat = mwlDatasetPtr->getItem(DCM_ScheduledProcedureStepSequence, sourceItem);
		stat = copyValueFrom(DCM_Modality, &sourceItem);

		int pos = std::distance(mwlDatasetList.cbegin(), i);
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

Status Dataset::prepareMPPSFromMWL(const std::list<Dataset*> mwlDatasetList)
{
	Status stat;

	if (mwlDatasetList.size() == 0)
		return EC_Normal;

	bool sameOrders = true;
	bool sameRequestedProcedures = true;
	std::list<Dataset*>::const_iterator i = mwlDatasetList.cbegin();
	const Dataset* mwlDatasetPtr_0 = *i;
	const Dataset* mwlDatasetPtr;

	String mwlPatientId_0, mwlAccessionNo_0, mwlStudyInstanceUID_0;
	stat = mwlDatasetPtr_0->getString(DCM_PatientID, mwlPatientId_0);
	stat = mwlDatasetPtr_0->getString(DCM_AccessionNumber, mwlAccessionNo_0);
	stat = mwlDatasetPtr_0->getString(DCM_StudyInstanceUID, mwlStudyInstanceUID_0);

	for(i++; i != mwlDatasetList.cend(); i++) {
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

	for(i = mwlDatasetList.cbegin(); i != mwlDatasetList.cend(); i++) {
		mwlDatasetPtr = *i;

		String mwlStudyInstanceUID;
		stat = mwlDatasetPtr->getString(DCM_StudyInstanceUID, mwlStudyInstanceUID);

		Item sourceItem, targetItem, refStudyItem;
		stat = mwlDatasetPtr->getItem(DCM_ScheduledProcedureStepSequence, sourceItem);
		stat = copyValueFrom(DCM_Modality, &sourceItem);

		int pos = std::distance(mwlDatasetList.cbegin(), i);
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

Status Dataset::prepareMPPSFromCompositeIOD(const Dataset* dataset)
{
	Status status;
	Item sourceItem, targetItem;

	if (dataset == NULL)
		return EC_IllegalParameter;

	int nNLS;
	if (dataset->getNLS(nNLS).good())
		setNLS(nNLS);

	// Performed Procedure Step Relationship
	for(Uint32 pos = 0; dataset->getItem(DCM_RequestAttributesSequence, sourceItem, pos).good() ; pos++) {
		status = putItem(DCM_ScheduledStepAttributesSequence, targetItem, pos);
		{
			status = targetItem.copyValueFrom(DCM_ReferencedStudySequence, dataset);
			status = targetItem.copyValueFrom(DCM_AccessionNumber, &sourceItem);
			status = targetItem.copyValueFrom(DCM_StudyInstanceUID, dataset);
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
	status = copyValueFrom(DCM_PatientName, dataset);
	status = copyValueFrom(DCM_PatientID, dataset);
	status = copyValueFrom(DCM_IssuerOfPatientID, dataset);
	status = copyValueFrom(DCM_PatientBirthDate, dataset);
	status = copyValueFrom(DCM_PatientSex, dataset);
	status = copyValueFrom(DCM_AdmissionID, dataset);
	status = copyValueFrom(DCM_RETIRED_IssuerOfAdmissionID, dataset);

	// Performed Procedure Step Information
	status = copyValueFrom(DCM_PerformedProcedureStepID, dataset);
	status = copyValueFrom(DCM_PerformedProcedureStepStartDate, dataset, DCM_StudyDate);
	status = copyValueFrom(DCM_PerformedProcedureStepStartTime, dataset, DCM_StudyTime);
	status = copyValueFrom(DCM_PerformedProcedureStepDescription, dataset, DCM_StudyDescription);
	status = putEmpty(DCM_PerformedProcedureTypeDescription);
	status = copyValueFrom(DCM_ProcedureCodeSequence, dataset);

	// Image Acquisition Results
	status = copyValueFrom(DCM_Modality, dataset);
	status = copyValueFrom(DCM_StudyID, dataset);
	status = copyValueFrom(DCM_PerformedProtocolCodeSequence, dataset);

	return EC_Normal;
}

Status Dataset::prepareMPPS2FromCompositeIOD(const Dataset* dataset)
{
	Status status;

	if (dataset == NULL)
		return EC_IllegalParameter;

	int nNLS;
	if (dataset->getNLS(nNLS).good())
		setNLS(nNLS);

	// Image Acquisition Results
	String seriesInstanceUID;
	String sopClassUID, sopInstanceUID;
	Item seriesItem, instanceItem;
	Sint32 seriesIndex, instanceIndex;
	String str;

	status = dataset->getString(DCM_SeriesInstanceUID, seriesInstanceUID);
	for(seriesIndex = 0; getItem(DCM_PerformedSeriesSequence, seriesItem, seriesIndex).good(); seriesIndex++) {
		status = seriesItem.getString(DCM_SeriesInstanceUID, str);
		if (str.compare(seriesInstanceUID) == 0)
			goto series_done;
	}
	// series not found
	status = putItem(DCM_PerformedSeriesSequence, seriesItem);
	{
		status = seriesItem.copyValueFrom(DCM_SeriesInstanceUID, dataset);
		status = seriesItem.copyValueFrom(DCM_SeriesDescription, dataset);
		status = seriesItem.copyValueFrom(DCM_PerformingPhysicianName, dataset);
		status = seriesItem.copyValueFrom(DCM_OperatorsName, dataset);
		status = seriesItem.copyValueFrom(DCM_ProtocolName, dataset);
	}

series_done:
	status = dataset->getString(DCM_SOPClassUID, sopClassUID);
	status = dataset->getString(DCM_SOPInstanceUID, sopInstanceUID);
	if (UIDHelper::isImageSOPClassUID(sopClassUID)) {
		for(instanceIndex = 0; seriesItem.getItem(DCM_ReferencedImageSequence, instanceItem, instanceIndex).good(); instanceIndex++) {
			status = instanceItem.getString(DCM_ReferencedSOPInstanceUID, str);
			if (str.compare(sopInstanceUID) == 0)
				goto instance_done;
		}
		// instance not found
		status = seriesItem.putItem(DCM_ReferencedImageSequence, instanceItem);
		status = instanceItem.copyValueFrom(DCM_ReferencedSOPClassUID, dataset, DCM_SOPClassUID);
		status = instanceItem.copyValueFrom(DCM_ReferencedSOPInstanceUID, dataset, DCM_SOPInstanceUID);
	} else {
		for(instanceIndex = 0; seriesItem.getItem(DCM_ReferencedNonImageCompositeSOPInstanceSequence, instanceItem, instanceIndex).good(); instanceIndex++) {
			status = instanceItem.getString(DCM_ReferencedSOPInstanceUID, str);
			if (str.compare(sopInstanceUID) == 0)
				goto instance_done;
		}
		// instance not found
		status = seriesItem.putItem(DCM_ReferencedNonImageCompositeSOPInstanceSequence, instanceItem);
		status = instanceItem.copyValueFrom(DCM_ReferencedSOPClassUID, dataset, DCM_SOPClassUID);
		status = instanceItem.copyValueFrom(DCM_ReferencedSOPInstanceUID, dataset, DCM_SOPInstanceUID);
	}

instance_done:
	return EC_Normal;
}

Status Dataset::prepareStorageCommitmentFromCompositeIOD(const Dataset* dataset)
{
	Status stat;

	String sopClassUID, sopInstanceUID;
	stat = dataset->getString(DCM_SOPClassUID, sopClassUID);
	stat = dataset->getString(DCM_SOPInstanceUID, sopInstanceUID);

	Item item;
	stat = putRefSOP(DCM_ReferencedSOPSequence, item, sopClassUID, sopInstanceUID);

	return EC_Normal;
}
