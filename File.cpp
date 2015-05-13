/************************************************************************
 *
 *	Copyright (c) 2007-2012, Samuel W. Choi
 *	All Rights Reserved
 *
 *	$Workfile: DcmFile.cpp $
 *	$Author: Samuelchoi $
 *	$Date: 07-07-02 10:27a $
 *	$Revision: 7 $
 *
 ************************************************************************/

#include "DTK.h"
#include "DTKinternal.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////

File::File(void)
{
	_dcmFileFormatPtr = new DcmFileFormat;

	int nNLS;
	Status dcmStat;
	dcmStat = Dataset::getDefaultNLS(nNLS);
	dcmStat = _dataset.set(_dcmFileFormatPtr->getDataset(), nNLS);
	dcmStat = _dataset.setNLS(nNLS);
}

File::File(const File& file)
{
	_dcmFileFormatPtr = new DcmFileFormat(*file._dcmFileFormatPtr);

	int nNLS;
	Status dcmStat;
	dcmStat = file._dataset.getNLS(nNLS);
	dcmStat = _dataset.set(_dcmFileFormatPtr->getDataset(), nNLS);
	dcmStat = _dataset.setNLS(nNLS);
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

	int nls;
	if (file._dataset.getNLS(nls).good()) {
		_dataset.set(_dcmFileFormatPtr->getDataset(), nls);
		_dataset.setNLS(nls);
	}

	return *this;
}

Status File::load(const String& filename, const E_TransferSyntax readXfer, const E_GrpLenEncoding groupLength, const Uint32 maxReadLength, const E_FileReadMode readMode)
{
	OFCondition cond = _dcmFileFormatPtr->loadFile(filename, readXfer, groupLength, maxReadLength, readMode);

	int nls;
	if (_dataset.getNLS(nls).good())
		_dataset.setAutoNLS(nls);

	return cond;
}

Status File::save(const String& filename, const E_TransferSyntax writeXfer, const E_EncodingType encodingType, const E_GrpLenEncoding groupLength, const E_PaddingEncoding padEncoding, const Uint32 padLength, const Uint32 subPadLength, const E_FileWriteMode fileWriteMode)
{
	return _dcmFileFormatPtr->saveFile(filename, writeXfer, encodingType, groupLength, padEncoding, padLength, subPadLength, fileWriteMode);
}

Status File::clear(void)
{
	OFCondition cond = _dcmFileFormatPtr->clear();

	int nls;
	if (Dataset::getDefaultNLS(nls).good()) {
		_dataset.set(_dcmFileFormatPtr->getDataset(), nls);
		_dataset.setNLS(nls);
	}

	return cond;
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
	OFCondition cond = _dcmFileFormatPtr->writeXML(os);
	os.close();

	return cond;
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

DcmFileFormat* File::getDcmFileFormat(void) const
{
	return _dcmFileFormatPtr;
}

File::operator DcmFileFormat*(void) const
{
	return _dcmFileFormatPtr;
}
