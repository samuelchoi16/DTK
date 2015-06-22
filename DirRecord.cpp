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

DirRecord::DirRecord()
{
	if (_created)
		delete _dcmItemPtr;

	_created = true;
	_dcmItemPtr = new DcmDirectoryRecord;

	setAutoNLS(_DefaultNLS);
	setNLS(_DefaultNLS);
}
/*
DirRecord::DirRecord(const E_DirRecType recordType)
{
	if (_created)
		delete _dcmItemPtr;

	_created = true;
	_dcmItemPtr = new DcmDirectoryRecord(recordType, NULL, "", NULL);

	setAutoNLS(_DefaultNLS);
	setNLS(_DefaultNLS);
}
*/
DirRecord::DirRecord(DcmDirectoryRecord* dcmDirRecordPtr)
	: Item(dcmDirRecordPtr)
{
	int nls;
	if (getNLS(nls).good() && nls > 0)
		setAutoNLS(nls);
}

DirRecord::DirRecord(const DirRecord& dirRecord)
{
	if (_created)
		delete _dcmItemPtr;

	_created = true;
	_dcmItemPtr = new DcmDirectoryRecord(*dynamic_cast<DcmDirectoryRecord*>(dirRecord._dcmItemPtr));

	int nls;
	if (dirRecord.getAutoNLS(nls).good())
		setAutoNLS(nls);
	if (dirRecord.getNLS(nls).good())
		setNLS(nls);
}

DirRecord::~DirRecord(void)
{
	if (_created) {
		delete _dcmItemPtr;
		_dcmItemPtr = NULL;
	}
}

Status DirRecord::setFileId(const String& fileId)
{
	_fileId = fileId;
	return EC_Normal;
}

Status DirRecord::getFileId(String& fileId) const
{
	fileId = _fileId;
	return EC_Normal;
}

String DirRecord::fileId(void) const
{
	return _fileId;
}

Status DirRecord::putRecord(const E_DirRecType recordType, const String& fileId, DirRecord& subRecord)
{
	DcmDirectoryRecord* dcmDirRecordPtr = dynamic_cast<DcmDirectoryRecord*>(_dcmItemPtr);
	DcmDirectoryRecord* dcmSubRecordPtr = new DcmDirectoryRecord(recordType, NULL, "", NULL);

	Status stat = dcmDirRecordPtr->insertSub(dcmSubRecordPtr);
	subRecord.set(dcmSubRecordPtr);
	subRecord.setFileId(fileId);
	return stat;
}

Status DirRecord::getRecord(const E_DirRecType recordType, const File& file, DirRecord& subRecord)
{
	DcmDirectoryRecord* dcmDirRecordPtr = dynamic_cast<DcmDirectoryRecord*>(_dcmItemPtr);
	DcmDirectoryRecord* record = NULL;
	bool found = false;
	const Dataset* datasetPtr = file.getDataset();
	DcmTagKey recordTag, datasetTag;

	while(!found && ((record = dcmDirRecordPtr->nextSub(record)))) {
		if (record->getRecordType() == recordType) {
			String recordData, datasetData;
			subRecord.set(record);

			switch(recordType) {
			case ERT_Patient :
				recordTag = datasetTag = DCM_PatientID;
				break;
			case ERT_Study :
				recordTag = datasetTag = DCM_StudyInstanceUID;
				break;
			case ERT_Series :
				recordTag = datasetTag = DCM_SeriesInstanceUID;
				break;
			case ERT_Image :
				recordTag = DCM_ReferencedSOPInstanceUIDInFile;
				datasetTag = DCM_SOPInstanceUID;
				break;
			default :
				return EC_IllegalParameter;		// TODO
			}

			subRecord.getString(recordTag, recordData);
			datasetPtr->getString(datasetTag, datasetData);

			if (recordData.compare(datasetData.c_str()) == 0) {
				found = true;
				return EC_Normal;
			}
		}
	}

	return EC_InvalidTag;
}

Status DirRecord::copyValue(const File& file)
{
	DcmDirectoryRecord* dcmDirRecordPtr = dynamic_cast<DcmDirectoryRecord*>(_dcmItemPtr);
	Status stat;
	const MetaInfo* metainfoPtr = file.getMetaInfo();
	const Dataset* datasetPtr = file.getDataset();

	switch(dcmDirRecordPtr->getRecordType()) {
	case ERT_Patient :
		stat = copyValueFrom(DCM_PatientID, datasetPtr);
		stat = copyValueFrom(DCM_PatientName, datasetPtr);
		stat = copyValueFrom(DCM_PatientBirthDate, datasetPtr);
		stat = copyValueFrom(DCM_PatientSex, datasetPtr);
		return EC_Normal;
	case ERT_Study :
		stat = copyValueFrom(DCM_StudyInstanceUID, datasetPtr);
		stat = copyValueFrom(DCM_StudyID, datasetPtr);
		stat = copyValueFrom(DCM_StudyDate, datasetPtr);
		stat = copyValueFrom(DCM_StudyTime, datasetPtr);
		stat = copyValueFrom(DCM_StudyDescription, datasetPtr);
		stat = copyValueFrom(DCM_AccessionNumber, datasetPtr);
		return EC_Normal;
	case ERT_Series :
		stat = copyValueFrom(DCM_SeriesInstanceUID, datasetPtr);
		stat = copyValueFrom(DCM_SeriesNumber, datasetPtr);
		stat = copyValueFrom(DCM_Modality, datasetPtr);
		stat = copyValueFrom(DCM_InstitutionName, datasetPtr);
		stat = copyValueFrom(DCM_InstitutionAddress, datasetPtr);
		stat = copyValueFrom(DCM_PerformingPhysicianName, datasetPtr);
		return EC_Normal;
	case ERT_Image :
		stat = copyValueFrom(DCM_InstanceNumber, datasetPtr);
		stat = copyValueFrom(DCM_ImageType, datasetPtr);
		stat = copyValueFrom(DCM_ReferencedImageSequence, datasetPtr);
		stat = copyValueFrom(DCM_Rows, datasetPtr);
		stat = copyValueFrom(DCM_Columns, datasetPtr);
		stat = copyValueFrom(DCM_ImageType, datasetPtr);
		stat = copyValueFrom(DCM_CalibrationImage, datasetPtr);
		stat = copyValueFrom(DCM_LossyImageCompressionRatio, datasetPtr);
		stat = copyValueFrom(DCM_FrameOfReferenceUID, datasetPtr);
		stat = copyValueFrom(DCM_SynchronizationFrameOfReferenceUID, datasetPtr);
		stat = copyValueFrom(DCM_NumberOfFrames, datasetPtr);
		stat = copyValueFrom(DCM_AcquisitionTimeSynchronized, datasetPtr);
		stat = copyValueFrom(DCM_AcquisitionDateTime, datasetPtr);
		break;
	default :
		return EC_IllegalParameter;		// TODO
	}

	stat = putString(DCM_ReferencedFileID, _fileId);
	stat = copyValueFrom(DCM_ReferencedSOPClassUIDInFile, metainfoPtr, DCM_MediaStorageSOPClassUID);
	stat = copyValueFrom(DCM_ReferencedSOPInstanceUIDInFile, metainfoPtr, DCM_MediaStorageSOPInstanceUID);
	stat = copyValueFrom(DCM_ReferencedTransferSyntaxUIDInFile, metainfoPtr, DCM_TransferSyntaxUID);

	return EC_Normal;
}

E_DirRecType DirRecord::getRecordType(void)
{
	DcmDirectoryRecord* dcmDirRecordPtr = dynamic_cast<DcmDirectoryRecord*>(_dcmItemPtr);
	return  dcmDirRecordPtr->getRecordType();
}

E_DirRecType DirRecord::getRecordTypeFromSOPClass(const String& sopClassUID)
{
	E_DirRecType result = ERT_Image;

	if (sopClassUID.compare(UID_RETIRED_StandaloneOverlayStorage) == 0) {
		result = ERT_Overlay;
	} else if (sopClassUID.compare(UID_RETIRED_StandaloneModalityLUTStorage) == 0) {
		result = ERT_ModalityLut;
	} else if (sopClassUID.compare(UID_RETIRED_StandaloneVOILUTStorage) == 0) {
		result = ERT_VoiLut;
	} else if (sopClassUID.compare(UID_RETIRED_StandaloneCurveStorage) == 0 ||
			sopClassUID.compare(UID_RETIRED_StandalonePETCurveStorage) == 0) {
		result = ERT_Curve;
	} else if (sopClassUID.compare(UID_BasicTextSRStorage) == 0 ||
			sopClassUID.compare(UID_EnhancedSRStorage) == 0 ||
			sopClassUID.compare(UID_ComprehensiveSRStorage) == 0 ||
			sopClassUID.compare(UID_Comprehensive3DSRStorage) == 0 ||
			sopClassUID.compare(UID_ProcedureLogStorage) == 0 ||
			sopClassUID.compare(UID_MammographyCADSRStorage) == 0 ||
			sopClassUID.compare(UID_ChestCADSRStorage) == 0 ||
			sopClassUID.compare(UID_ColonCADSRStorage) == 0 ||
			sopClassUID.compare(UID_XRayRadiationDoseSRStorage) == 0 ||
			sopClassUID.compare(UID_RadiopharmaceuticalRadiationDoseSRStorage) == 0 ||
			sopClassUID.compare(UID_SpectaclePrescriptionReportStorage) == 0 ||
			sopClassUID.compare(UID_MacularGridThicknessAndVolumeReportStorage) == 0 ||
			sopClassUID.compare(UID_ImplantationPlanSRDocumentStorage) == 0) {
		result = ERT_SRDocument;
	} else if (sopClassUID.compare(UID_GrayscaleSoftcopyPresentationStateStorage) == 0 ||
			sopClassUID.compare(UID_ColorSoftcopyPresentationStateStorage) == 0 ||
			sopClassUID.compare(UID_PseudoColorSoftcopyPresentationStateStorage) == 0 ||
			sopClassUID.compare(UID_BlendingSoftcopyPresentationStateStorage) == 0 ||
			sopClassUID.compare(UID_XAXRFGrayscaleSoftcopyPresentationStateStorage) == 0 ||
			sopClassUID.compare(UID_BasicStructuredDisplayStorage) == 0) {
		result = ERT_Presentation;
	} else if (sopClassUID.compare(UID_TwelveLeadECGWaveformStorage) == 0 ||
			sopClassUID.compare(UID_GeneralECGWaveformStorage) == 0 ||
			sopClassUID.compare(UID_AmbulatoryECGWaveformStorage) == 0 ||
			sopClassUID.compare(UID_HemodynamicWaveformStorage) == 0 ||
			sopClassUID.compare(UID_CardiacElectrophysiologyWaveformStorage) == 0 ||
			sopClassUID.compare(UID_BasicVoiceAudioWaveformStorage) == 0 ||
			sopClassUID.compare(UID_GeneralAudioWaveformStorage) == 0 ||
			sopClassUID.compare(UID_ArterialPulseWaveformStorage) == 0 ||
			sopClassUID.compare(UID_RespiratoryWaveformStorage) == 0) {
		result = ERT_Waveform;
	} else if (sopClassUID.compare(UID_RTDoseStorage) == 0) {
		result = ERT_RTDose;
	} else if (sopClassUID.compare(UID_RTStructureSetStorage) == 0) {
		result = ERT_RTStructureSet;
	} else if (sopClassUID.compare(UID_RTPlanStorage) == 0 ||
			sopClassUID.compare(UID_RTIonPlanStorage) == 0) {
		result = ERT_RTPlan;
	} else if (sopClassUID.compare(UID_RTBeamsTreatmentRecordStorage) == 0 ||
			sopClassUID.compare(UID_RTBrachyTreatmentRecordStorage) == 0 ||
			sopClassUID.compare(UID_RTTreatmentSummaryRecordStorage) == 0 ||
			sopClassUID.compare(UID_RTIonBeamsTreatmentRecordStorage) == 0) {
		result = ERT_RTTreatRecord;
	} else if (sopClassUID.compare(UID_RETIRED_StoredPrintStorage) == 0) {
		result = ERT_StoredPrint;
	} else if (sopClassUID.compare(UID_KeyObjectSelectionDocumentStorage) == 0) {
		result = ERT_KeyObjectDoc;
	} else if (sopClassUID.compare(UID_SpatialRegistrationStorage) == 0 ||
			sopClassUID.compare(UID_DeformableSpatialRegistrationStorage) == 0) {
		result = ERT_Registration;
	} else if (sopClassUID.compare(UID_SpatialFiducialsStorage) == 0) {
		result = ERT_Fiducial;
	} else if (sopClassUID.compare(UID_RawDataStorage) == 0) {
		result = ERT_RawData;
	} else if (sopClassUID.compare(UID_MRSpectroscopyStorage) == 0) {
		result = ERT_Spectroscopy;
	} else if (sopClassUID.compare(UID_EncapsulatedPDFStorage) == 0 ||
			sopClassUID.compare(UID_EncapsulatedCDAStorage) == 0) {
		result = ERT_EncapDoc;
	} else if (sopClassUID.compare(UID_RealWorldValueMappingStorage) == 0) {
		result = ERT_ValueMap;
	} else if (sopClassUID.compare(UID_HangingProtocolStorage) == 0) {
		result = ERT_HangingProtocol;
	} else if (sopClassUID.compare(UID_StereometricRelationshipStorage) == 0) {
		result = ERT_Stereometric;
	} else if (sopClassUID.compare(UID_ColorPaletteStorage) == 0) {
		result = ERT_Palette;
	} else if (sopClassUID.compare(UID_SurfaceSegmentationStorage) == 0) {
		result = ERT_Surface;
	} else if (sopClassUID.compare(UID_LensometryMeasurementsStorage) == 0 ||
			sopClassUID.compare(UID_AutorefractionMeasurementsStorage) == 0 ||
			sopClassUID.compare(UID_KeratometryMeasurementsStorage) == 0 ||
			sopClassUID.compare(UID_SubjectiveRefractionMeasurementsStorage) == 0 ||
			sopClassUID.compare(UID_VisualAcuityMeasurementsStorage) == 0 ||
			sopClassUID.compare(UID_OphthalmicAxialMeasurementsStorage) == 0 ||
			sopClassUID.compare(UID_IntraocularLensCalculationsStorage) == 0 ||
			sopClassUID.compare(UID_OphthalmicVisualFieldStaticPerimetryMeasurementsStorage) == 0) {
		result = ERT_Measurement;
	} else if (sopClassUID.compare(UID_GenericImplantTemplateStorage) == 0) {
		result = ERT_Implant;
	} else if (sopClassUID.compare(UID_ImplantTemplateGroupStorage) == 0) {
		result = ERT_ImplantGroup;
	} else if (sopClassUID.compare(UID_ImplantAssemblyTemplateStorage) == 0) {
		result = ERT_ImplantAssy;
	} else if (sopClassUID.compare(UID_RTBeamsDeliveryInstructionStorage) == 0) {
		result = ERT_Plan;
	} else if (sopClassUID.compare(UID_SurfaceScanMeshStorage) == 0 ||
			sopClassUID.compare(UID_SurfaceScanPointCloudStorage) == 0) {
		result = ERT_SurfaceScan;
	}

	return result;
}

String DirRecord::getRecordTypeName(const E_DirRecType recordType)
{
	String recordName;

	switch(recordType)
	{
	case ERT_root:
		recordName = "Root";
		break;
	case ERT_Curve:
		recordName = "Curve";
		break;
	case ERT_FilmBox:
		recordName = "FilmBox";
		break;
	case ERT_FilmSession:
		recordName = "FilmSession";
		break;
	case ERT_Image:
		recordName = "Image";
		break;
	case ERT_ImageBox:
		recordName = "ImageBox";
		break;
	case ERT_Interpretation:
		recordName = "Interpretation";
		break;
	case ERT_ModalityLut:
		recordName = "ModalityLUT";
		break;
	case ERT_Mrdr:
		recordName = "MRDR";
		break;
	case ERT_Overlay:
		recordName = "Overlay";
		break;
	case ERT_Patient:
		recordName = "Patient";
		break;
	case ERT_PrintQueue:
		recordName = "PrintQueue";
		break;
	case ERT_Private:
		recordName = "Private";
		break;
	case ERT_Results:
		recordName = "Results";
		break;
	case ERT_Series:
		recordName = "Series";
		break;
	case ERT_Study:
		recordName = "Study";
		break;
	case ERT_StudyComponent:
		recordName = "StudyComponent";
		break;
	case ERT_Topic:
		recordName = "Topic";
		break;
	case ERT_Visit:
		recordName = "Visit";
		break;
	case ERT_VoiLut:
		recordName = "VOILUT";
		break;
	case ERT_SRDocument:
		recordName = "SRDocument";
		break;
	case ERT_Presentation:
		recordName = "Presentation";
		break;
	case ERT_Waveform:
		recordName = "Waveform";
		break;
	case ERT_RTDose:
		recordName = "RTDose";
		break;
	case ERT_RTStructureSet:
		recordName = "RTStructureSet";
		break;
	case ERT_RTPlan:
		recordName = "RTPlan";
		break;
	case ERT_RTTreatRecord:
		recordName = "RTTreatRecord";
		break;
	case ERT_StoredPrint:
		recordName = "StoredPrint";
		break;
	case ERT_KeyObjectDoc:
		recordName = "KeyObjectDoc";
		break;
	case ERT_Registration:
		recordName = "Registration";
		break;
	case ERT_Fiducial:
		recordName = "Fiducial";
		break;
	case ERT_RawData:
		recordName = "RawData";
		break;
	case ERT_Spectroscopy:
		recordName = "Spectroscopy";
		break;
	case ERT_EncapDoc:
		recordName = "EncapDoc";
		break;
	case ERT_ValueMap:
		recordName = "ValueMap";
		break;
	case ERT_HangingProtocol:
		recordName = "HangingProtocol";
		break;
	case ERT_Stereometric:
		recordName = "Stereometric";
		break;
	case ERT_HL7StrucDoc:
		recordName = "HL7StrucDoc";
		break;
	case ERT_Palette:
		recordName = "Palette";
		break;
	case ERT_Surface:
		recordName = "Surface";
		break;
	case ERT_Measurement:
		recordName = "Measurement";
		break;
	case ERT_Implant:
		recordName = "Implant";
		break;
	case ERT_ImplantGroup:
		recordName = "ImplantGroup";
		break;
	case ERT_ImplantAssy:
		recordName = "ImplantAssy";
		break;
	case ERT_Plan:
		recordName = "Plan";
		break;
	case ERT_SurfaceScan:
		recordName = "SurfaceScan";
		break;
	default:
		recordName = "(unknown-directory-record-type)";
		break;
	}

	return recordName;
}
