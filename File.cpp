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

File::File(void)
{
	_dcmFileFormatPtr = new DcmFileFormat;

	Status stat = _metaInfo.set(_dcmFileFormatPtr->getMetaInfo());

	int nls;
	stat = Dataset::getDefaultNLS(nls);
	stat = _dataset.set(_dcmFileFormatPtr->getDataset(), nls);
	stat = _dataset.setNLS(nls);
}

File::File(const File& file)
{
	_dcmFileFormatPtr = new DcmFileFormat(*file._dcmFileFormatPtr);

	Status stat = _metaInfo.set(_dcmFileFormatPtr->getMetaInfo());

	int nls;
	stat = file._dataset.getNLS(nls);
	stat = _dataset.set(_dcmFileFormatPtr->getDataset(), nls);
	stat = _dataset.setNLS(nls);
}

File::~File(void)
{
	delete _dcmFileFormatPtr;
}

File& File::operator=(const File& file)
{
	if (this == &file)
		return *this;

	delete _dcmFileFormatPtr;

	_dcmFileFormatPtr = new DcmFileFormat(*file._dcmFileFormatPtr);

	Status stat = _metaInfo.set(_dcmFileFormatPtr->getMetaInfo());

	int nls;
	if (file._dataset.getNLS(nls).good()) {
		stat = _dataset.set(_dcmFileFormatPtr->getDataset(), nls);
		stat = _dataset.setNLS(nls);
	}

	return *this;
}

Status File::load(const String& filename, const E_TransferSyntax readXfer, const E_GrpLenEncoding groupLength, const Uint32 maxReadLength, const E_FileReadMode readMode)
{
	Status stat = _dcmFileFormatPtr->loadFile(filename, readXfer, groupLength, maxReadLength, readMode);

	int nls;
	if (_dataset.getNLS(nls).good())
		_dataset.setAutoNLS(nls);

	return stat;
}

Status File::save(const String& filename, const E_TransferSyntax writeXfer, const E_EncodingType encodingType, const E_GrpLenEncoding groupLength, const E_PaddingEncoding padEncoding, const Uint32 padLength, const Uint32 subPadLength, const E_FileWriteMode fileWriteMode)
{
	Dataset* datasetPtr = getDataset();

	E_TransferSyntax transferSyntax = (writeXfer == EXS_Unknown) ? datasetPtr->getTransferSyntax() : writeXfer;
	Status stat = datasetPtr->setTransferSyntax(transferSyntax);
	if (stat.bad())
		return stat;

	return _dcmFileFormatPtr->saveFile(filename, transferSyntax, encodingType, groupLength, padEncoding, padLength, subPadLength, fileWriteMode);
}

Status File::clear(void)
{
	Status stat = _dcmFileFormatPtr->clear();

	int nls;
	if (Dataset::getDefaultNLS(nls).good()) {
		_dataset.set(_dcmFileFormatPtr->getDataset(), nls);
		_dataset.setNLS(nls);
	}

	return stat;
}

Status File::print(const String& filename)
{
	std::ofstream os;
	
	os.open(filename.c_str());
	_dcmFileFormatPtr->print(os);
	os.close();

	return EC_Normal;
}

Status File::printXML(const String& filename)
{
	std::ofstream os;

	os.open(filename.c_str());
	Status stat = _dcmFileFormatPtr->writeXML(os);
	os.close();

	return stat;
}

MetaInfo& File::metaInfo(void)
{
	return _metaInfo;
}

Dataset& File::dataset(void)
{
	return _dataset;
}

MetaInfo* File::getFileMetaInfo(void)
{
	return &_metaInfo;
}

Dataset* File::getDataset(void)
{
	return &_dataset;
}

const MetaInfo* File::getFileMetaInfo(void) const
{
	return &_metaInfo;
}

const Dataset* File::getDataset(void) const
{
	return &_dataset;
}

DcmFileFormat* File::getDcmFileFormat(void) const
{
	return _dcmFileFormatPtr;
}

File::operator DcmFileFormat*(void) const
{
	return _dcmFileFormatPtr;
}
