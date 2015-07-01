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
#include <QFile>
#include <QFileInfo>
#include <QDir>

///////////////////////////////////////////////////////////////////////////////

#define	DICOM_FILE_PATH					"DICOM"

Logger Dir::_logger = Logger::getInstance("dcm.Dir");

Dir::Dir()
{
	_dcmDir = new DcmDicomDir;
	_baseDir = ".";
}

Dir::Dir(const String& dicomdir)
{
	_dcmDir = new DcmDicomDir(dicomdir);
	_baseDir = QSTR_TO_DSTR(QFileInfo(_dcmDir->getDirFileName().getCharPointer()).absolutePath());
}

Dir::~Dir()
{
	delete _dcmDir;
}

Status Dir::getRootRecord(DirRecord& rootRecord)
{
	DcmDirectoryRecord* rootRecordPtr = &(_dcmDir->getRootRecord());
	rootRecord.set(rootRecordPtr);
	return EC_Normal;
}

#define	LOG_ADD_FILE_DEBUG(file, tag, msg, type) \
{ \
	String str; \
	(file).getDataset()->getString((tag), str); \
	String name = DirRecord::getRecordTypeName((type)); \
	DCMTK_LOG4CPLUS_DEBUG_FMT(_logger, (msg), name.c_str(), str.c_str()); \
}

Status Dir::addFile(const String& filename)
{
	File dcmFile;
	Status stat = dcmFile.load(filename);

	DirRecord rootRecord;
	stat = getRootRecord(rootRecord);

	DirRecord patientRecord;
	stat = rootRecord.getRecord(ERT_Patient, dcmFile, patientRecord);
	if (stat.good()) {
		LOG_ADD_FILE_DEBUG(dcmFile, DCM_PatientID, "addFile: %s ALREADY added (pid=%s)", ERT_Patient);
	} else {
		stat = rootRecord.putRecord(ERT_Patient, "", patientRecord);
		stat = patientRecord.copyValue(dcmFile);
		LOG_ADD_FILE_DEBUG(dcmFile, DCM_PatientID, "addFile: %s added (pid=%s)", ERT_Patient);
	}

	DirRecord studyRecord;
	stat = patientRecord.getRecord(ERT_Study, dcmFile, studyRecord);
	if (stat.good()) {
		LOG_ADD_FILE_DEBUG(dcmFile, DCM_StudyInstanceUID, "addFile: %s ALREADY added (uid=%s)", ERT_Study);
	} else {
		stat = patientRecord.putRecord(ERT_Study, "", studyRecord);
		stat = studyRecord.copyValue(dcmFile);
		LOG_ADD_FILE_DEBUG(dcmFile, DCM_StudyInstanceUID, "addFile: %s added (uid=%s)", ERT_Study);
	}

	DirRecord seriesRecord;
	stat = studyRecord.getRecord(ERT_Series, dcmFile, seriesRecord);
	if (stat.good()) {
		LOG_ADD_FILE_DEBUG(dcmFile, DCM_SeriesInstanceUID, "addFile: %s ALREADY added (uid=%s)", ERT_Series);
	} else {
		stat = studyRecord.putRecord(ERT_Series, "", seriesRecord);
		stat = seriesRecord.copyValue(dcmFile);
		LOG_ADD_FILE_DEBUG(dcmFile, DCM_SeriesInstanceUID, "addFile: %s added (uid=%s)", ERT_Series);
	}

	String sopClassUID;
	dcmFile.getMetaInfo()->getString(DCM_MediaStorageSOPClassUID, sopClassUID);
	E_DirRecType recordType = DirRecord::getRecordTypeFromSOPClass(sopClassUID);

	DirRecord instanceRecord;
	stat = seriesRecord.getRecord(recordType, dcmFile, instanceRecord);
	if (stat.good()) {
		LOG_ADD_FILE_DEBUG(dcmFile, DCM_SOPInstanceUID, "addFile: %s ALREADY added (uid=%s)", recordType);
	} else {
		String fileId = generateFileId();
		stat = seriesRecord.putRecord(recordType, fileId, instanceRecord);
		stat = instanceRecord.copyValue(dcmFile);
		LOG_ADD_FILE_DEBUG(dcmFile, DCM_SOPInstanceUID, "addFile: %s added (uid=%s)", recordType);

		_sourceFileList.push_back(filename);
		_targetFileList.push_back(fileId);
	}

	return stat;
}

Status Dir::save(const E_EncodingType encodingType, const E_GrpLenEncoding groupLength)
{
	QDir baseDir(DSTR_TO_QSTR(_baseDir));
	baseDir.mkpath(".");
	Status stat = _dcmDir->write(DICOMDIR_DEFAULT_TRANSFERSYNTAX, encodingType, groupLength);

	assert(_sourceFileList.size() == _targetFileList.size());
	StringList::iterator si = _sourceFileList.begin();
	StringList::iterator ti = _targetFileList.begin();
	for( ; si != _sourceFileList.end() && ti != _targetFileList.end(); si++, ti++) {
		String sourceFile = *si;
		String targetFile = _baseDir.empty() ? *ti : (_baseDir + "/" + *ti);

		baseDir.mkpath(DICOM_FILE_PATH);
		bool ret = QFile::copy(DSTR_TO_CSTR(sourceFile), DSTR_TO_CSTR(targetFile));
		if (ret) {
			DCMTK_LOG4CPLUS_DEBUG_FMT(_logger, "save: %s ==> %s copied", sourceFile.c_str(), targetFile.c_str());
		} else {
			DCMTK_LOG4CPLUS_ERROR_FMT(_logger, "save: %s ==> %s NOT copied", sourceFile.c_str(), targetFile.c_str());
		}
	}
	return stat;
}

Status Dir::print(const String& filename)
{
	std::ofstream os;

	os.open(filename.c_str());
	_dcmDir->print(os);
	os.close();

	return EC_Normal;
}

static int seq = 0;

String Dir::generateFileId(void)
{
	char fileId[1000];
	while(1) {
		sprintf(fileId, "%s\\%08d", DICOM_FILE_PATH, ++seq);
		String fullpath = _baseDir.empty() ? fileId : (_baseDir + "\\" + fileId);
		if (access(fullpath.c_str(), 0) != 0)
			break;
	}
	return fileId;
}
