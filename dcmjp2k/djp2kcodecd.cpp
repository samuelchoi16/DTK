/*
*
*  Copyright (C) 2007-2011, OFFIS e.V.
*  All rights reserved.  See COPYRIGHT file for details.
*
*  This software and supporting documentation were developed by
*
*    OFFIS e.V.
*    R&D Division Health
*    Escherweg 2
*    D-26121 Oldenburg, Germany
*
*
*  Module:  dcmj2k
*
*  Author:  Martin Willkomm, Marco Eichelberg, Uli Schlachter
*
*  Purpose: codec classes for JPEG 2000 decoders.
*
*/

#include "dcmtk/config/osconfig.h"
#include "dcmjp2k/djp2kcodecd.h"

#include "dcmtk/ofstd/ofstream.h"    /* for ofstream */
#include "dcmtk/ofstd/ofcast.h"      /* for casts */
#include "dcmtk/ofstd/offile.h"      /* for class OFFile */
#include "dcmtk/ofstd/ofstd.h"       /* for class OFStandard */
#include "dcmtk/dcmdata/dcdatset.h"  /* for class DcmDataset */
#include "dcmtk/dcmdata/dcdeftag.h"  /* for tag constants */
#include "dcmtk/dcmdata/dcpixseq.h"  /* for class DcmPixelSequence */
#include "dcmtk/dcmdata/dcpxitem.h"  /* for class DcmPixelItem */
#include "dcmtk/dcmdata/dcvrpobw.h"  /* for class DcmPolymorphOBOW */
#include "dcmtk/dcmdata/dcswap.h"    /* for swapIfNecessary() */
#include "dcmtk/dcmdata/dcuid.h"     /* for dcmGenerateUniqueIdentifer()*/
#include "dcmjp2k/djp2kcparam.h"     /* for class DJP2KCodecParameter */
#include "dcmjp2k/djp2kerror.h"      /* for private class DJP2KError */

// JPEG 2000 library (OpenJPEG) includes
#include "openjpeg.h"

#include "math.h"

E_TransferSyntax DJP2KLosslessDecoder::supportedTransferSyntax() const
{
	return EXS_JPEG2000LosslessOnly;
}

E_TransferSyntax DJP2KLossyDecoder::supportedTransferSyntax() const
{
	return EXS_JPEG2000;
}

// --------------------------------------------------------------------------

DJP2KDecoderBase::DJP2KDecoderBase()
{
}


DJP2KDecoderBase::~DJP2KDecoderBase()
{
}

OFBool DJP2KDecoderBase::canChangeCoding(
	const E_TransferSyntax oldRepType,
	const E_TransferSyntax newRepType) const
{
	// this codec only handles conversion from JPEG 2000 to uncompressed.

	DcmXfer newRep(newRepType);
	if (newRep.isNotEncapsulated() &&
		((oldRepType == EXS_JPEG2000LosslessOnly) || (oldRepType == EXS_JPEG2000)))
		return OFTrue;

	return OFFalse;
}

OFCondition DJP2KDecoderBase::decode(
	const DcmRepresentationParameter * /* fromRepParam */,
	DcmPixelSequence * pixSeq,
	DcmPolymorphOBOW& uncompressedPixelData,
	const DcmCodecParameter * cp,
	const DcmStack& objStack) const
{
	// retrieve pointer to dataset from parameter stack
	DcmStack localStack(objStack);
	(void)localStack.pop();  // pop pixel data element from stack
	DcmObject *dobject = localStack.pop(); // this is the item in which the pixel data is located
	if ((!dobject) || ((dobject->ident() != EVR_dataset) && (dobject->ident() != EVR_item))) return EC_InvalidTag;
	DcmItem *dataset = OFstatic_cast(DcmItem *, dobject);
	OFBool numberOfFramesPresent = OFFalse;

	// determine properties of uncompressed dataset
	Uint16 imageSamplesPerPixel = 0;
	if (dataset->findAndGetUint16(DCM_SamplesPerPixel, imageSamplesPerPixel).bad())
		return EC_TagNotFound;
	// we only handle one or three samples per pixel
	if ((imageSamplesPerPixel != 3) && (imageSamplesPerPixel != 1))
		return EC_InvalidTag;

	Uint16 imageRows = 0;
	if (dataset->findAndGetUint16(DCM_Rows, imageRows).bad())
		return EC_TagNotFound;
	if (imageRows < 1)
		return EC_InvalidTag;

	Uint16 imageColumns = 0;
	if (dataset->findAndGetUint16(DCM_Columns, imageColumns).bad())
		return EC_TagNotFound;
	if (imageColumns < 1)
		return EC_InvalidTag;

	// number of frames is an optional attribute - we don't mind if it isn't present.
	Sint32 imageFrames = 0;
	if (dataset->findAndGetSint32(DCM_NumberOfFrames, imageFrames).good())
		numberOfFramesPresent = OFTrue;

	if (imageFrames >= OFstatic_cast(Sint32, pixSeq->card()))
		imageFrames = pixSeq->card() - 1; // limit number of frames to number of pixel items - 1
	if (imageFrames < 1)
		imageFrames = 1; // default in case the number of frames attribute is absent or contains garbage

	Uint16 imageBitsStored = 0;
	if (dataset->findAndGetUint16(DCM_BitsStored, imageBitsStored).bad())
		return EC_TagNotFound;

	Uint16 imageBitsAllocated = 0;
	if (dataset->findAndGetUint16(DCM_BitsAllocated, imageBitsAllocated).bad())
		return EC_TagNotFound;

	Uint16 imageHighBit = 0;
	if (dataset->findAndGetUint16(DCM_HighBit, imageHighBit).bad())
		return EC_TagNotFound;

	// we only support up to 16 bits per sample
	if ((imageBitsStored < 1) || (imageBitsStored > 16))
		return EC_JP2KUnsupportedBitDepth;

	// determine the number of bytes per sample (bits allocated) for the de-compressed object.
	Uint16 bytesPerSample = 1;
	if (imageBitsStored > 8)
		bytesPerSample = 2;
	else if (imageBitsAllocated > 8)
		bytesPerSample = 2;

	// compute size of uncompressed frame, in bytes
	Uint32 frameSize = bytesPerSample * imageRows * imageColumns * imageSamplesPerPixel;

	// compute size of pixel data attribute, in bytes
	Uint32 totalSize = frameSize * imageFrames;
	if (totalSize & 1)
		totalSize++; // align on 16-bit word boundary

	// assume we can cast the codec parameter to what we need
	const DJP2KCodecParameter *djcp = OFreinterpret_cast(const DJP2KCodecParameter *, cp);

	// determine planar configuration for uncompressed data
	OFString imageSopClass;
	OFString imagePhotometricInterpretation;
	dataset->findAndGetOFString(DCM_SOPClassUID, imageSopClass);
	dataset->findAndGetOFString(DCM_PhotometricInterpretation, imagePhotometricInterpretation);

	// allocate space for uncompressed pixel data element
	Uint16 *pixeldata16 = NULL;
	OFCondition result = uncompressedPixelData.createUint16Array(totalSize / sizeof(Uint16), pixeldata16);
	if (result.bad()) return result;

	Uint8 *pixeldata8 = OFreinterpret_cast(Uint8 *, pixeldata16);
	Sint32 currentFrame = 0;
	Uint32 currentItem = 1; // item 0 contains the offset table
	OFBool done = OFFalse;

	while (result.good() && !done) {
		DCMJP2K_DEBUG("JPEG 2000 decoder processes frame " << (currentFrame + 1));

		result = decodeFrame(pixSeq, djcp, dataset, currentFrame, currentItem, pixeldata8, frameSize,
			imageFrames, imageColumns, imageRows, imageSamplesPerPixel, bytesPerSample);

		if (result.good()) {
			// increment frame number, check if we're finished
			if (++currentFrame == imageFrames) done = OFTrue;
			pixeldata8 += frameSize;
		}
	}

	// Number of Frames might have changed in case the previous value was wrong
	if (result.good() && (numberOfFramesPresent || (imageFrames > 1))) {
		char numBuf[20];
		sprintf(numBuf, "%ld", OFstatic_cast(long, imageFrames));
		result = ((DcmItem *)dataset)->putAndInsertString(DCM_NumberOfFrames, numBuf);
	}

	if (result.good()) {
		// the following operations do not affect the Image Pixel Module
		// but other modules such as SOP Common.  We only perform these
		// changes if we're on the main level of the dataset,
		// which should always identify itself as dataset, not as item.
		if ((dataset->ident() == EVR_dataset) && (djcp->getUIDCreation() == EJ2KUC_always)) {
			// create new SOP instance UID
			result = DcmCodec::newInstance((DcmItem *)dataset, NULL, NULL, NULL);
		}
	}

	return result;
}

OFCondition DJP2KDecoderBase::decodeFrame(
	const DcmRepresentationParameter * /* fromParam */,
	DcmPixelSequence *fromPixSeq,
	const DcmCodecParameter *cp,
	DcmItem *dataset,
	Uint32 frameNo,
	Uint32& currentItem,
	void * buffer,
	Uint32 bufSize,
	OFString& decompressedColorModel) const
{
	OFCondition result = EC_Normal;

	// assume we can cast the codec parameter to what we need
	const DJP2KCodecParameter *djcp = OFreinterpret_cast(const DJP2KCodecParameter *, cp);

	// determine properties of uncompressed dataset
	Uint16 imageSamplesPerPixel = 0;
	if (dataset->findAndGetUint16(DCM_SamplesPerPixel, imageSamplesPerPixel).bad())
		return EC_TagNotFound;
	// we only handle one or three samples per pixel
	if ((imageSamplesPerPixel != 3) && (imageSamplesPerPixel != 1))
		return EC_InvalidTag;

	Uint16 imageRows = 0;
	if (dataset->findAndGetUint16(DCM_Rows, imageRows).bad())
		return EC_TagNotFound;
	if (imageRows < 1)
		return EC_InvalidTag;

	Uint16 imageColumns = 0;
	if (dataset->findAndGetUint16(DCM_Columns, imageColumns).bad())
		return EC_TagNotFound;
	if (imageColumns < 1)
		return EC_InvalidTag;

	Uint16 imageBitsStored = 0;
	if (dataset->findAndGetUint16(DCM_BitsStored, imageBitsStored).bad())
		return EC_TagNotFound;

	Uint16 imageBitsAllocated = 0;
	if (dataset->findAndGetUint16(DCM_BitsAllocated, imageBitsAllocated).bad())
		return EC_TagNotFound;

	//we only support up to 16 bits per sample
	if ((imageBitsStored < 1) || (imageBitsStored > 16))
		return EC_JP2KUnsupportedBitDepth;

	// determine the number of bytes per sample (bits allocated) for the de-compressed object.
	Uint16 bytesPerSample = 1;
	if (imageBitsStored > 8)
		bytesPerSample = 2;
	else if (imageBitsAllocated > 8)
		bytesPerSample = 2;

	// number of frames is an optional attribute - we don't mind if it isn't present.
	Sint32 imageFrames = 0;
	dataset->findAndGetSint32(DCM_NumberOfFrames, imageFrames).good();

	if (imageFrames >= OFstatic_cast(Sint32, fromPixSeq->card()))
		imageFrames = fromPixSeq->card() - 1; // limit number of frames to number of pixel items - 1
	if (imageFrames < 1)
		imageFrames = 1; // default in case the number of frames attribute is absent or contains garbage

	// if the user has provided this information, we trust him.
	// If the user has passed a zero, try to find out ourselves.
	if (currentItem == 0) {
		result = determineStartFragment(frameNo, imageFrames, fromPixSeq, currentItem);
	}

	if (result.good()) {
		// We got all the data we need from the dataset, let's start decoding
		DCMJP2K_DEBUG("Starting to decode frame " << frameNo << " with fragment " << currentItem);
		result = decodeFrame(fromPixSeq, djcp, dataset, frameNo, currentItem, buffer, bufSize,
			imageFrames, imageColumns, imageRows, imageSamplesPerPixel, bytesPerSample);
	}

	if (result.good()) {
		// retrieve color model from given dataset
		result = dataset->findAndGetOFString(DCM_PhotometricInterpretation, decompressedColorModel);
	}

	return result;
}

static void error_callback(const char *msg, void *client_data) {
	(void)client_data;
	fprintf(stdout, "[ERROR] %s", msg);
}

static void warning_callback(const char *msg, void *client_data) {
	(void)client_data;
	fprintf(stdout, "[WARNING] %s", msg);
}

static void info_callback(const char *msg, void *client_data) {
	(void)client_data;
	fprintf(stdout, "[INFO] %s", msg);
}

typedef struct buffer_info {
	unsigned char *sample_buf;
	unsigned char *cur_buf;
	uint64_t sample_size;
} BufferInfo;

static OPJ_SIZE_T read_from_buffer(void *p_buffer, OPJ_SIZE_T p_nb_bytes, BufferInfo *bufinfo)
{
	OPJ_SIZE_T l_nb_read;
	unsigned char *sample_end = (bufinfo->sample_buf + bufinfo->sample_size);

	if ((bufinfo->cur_buf + (ptrdiff_t)p_nb_bytes) < sample_end) {
		l_nb_read = p_nb_bytes;
	} else {
		l_nb_read = (OPJ_SIZE_T)(sample_end - bufinfo->cur_buf);
	}

	if (l_nb_read > 0) {
		memcpy(p_buffer, bufinfo->cur_buf, l_nb_read);
		bufinfo->cur_buf += l_nb_read;
	}
	return l_nb_read ? l_nb_read : (OPJ_SIZE_T)-1;
}

static OPJ_SIZE_T write_from_buffer(void *p_buffer, OPJ_SIZE_T p_nb_bytes, BufferInfo *bufinfo)
{
	memcpy(bufinfo->cur_buf,p_buffer, p_nb_bytes);
	bufinfo->cur_buf += (ptrdiff_t)p_nb_bytes;
	bufinfo->sample_size += p_nb_bytes;

	return p_nb_bytes;
}

static OPJ_OFF_T skip_from_buffer(OPJ_OFF_T p_nb_bytes, BufferInfo *bufinfo)
{
	unsigned char *sample_end = (bufinfo->sample_buf + bufinfo->sample_size);
	OPJ_OFF_T delta;

	if (sample_end > bufinfo->cur_buf) {
		delta = (sample_end - bufinfo->cur_buf);
	} else {
		delta = 0;
	}

	if (p_nb_bytes <= delta) {
		bufinfo->cur_buf += p_nb_bytes;
		return p_nb_bytes;
	}
	bufinfo->cur_buf = sample_end;

	return p_nb_bytes;
}

static OPJ_BOOL seek_from_buffer(OPJ_OFF_T p_nb_bytes, BufferInfo *bufinfo)
{
	if ((uint64_t)p_nb_bytes <= bufinfo->sample_size ) {
		bufinfo->cur_buf = (bufinfo->sample_buf + (ptrdiff_t)p_nb_bytes);
		return OPJ_TRUE;
	}

	bufinfo->cur_buf = (bufinfo->sample_buf + (ptrdiff_t)bufinfo->sample_size);

	return OPJ_FALSE;
}

static opj_stream_t* stream_create_buffer_stream(BufferInfo *bufinfo, OPJ_SIZE_T p_size, OPJ_BOOL p_is_read_stream)
{
	opj_stream_t* l_stream = opj_stream_create(p_size, p_is_read_stream);
	if (!l_stream)
		return NULL;

	opj_stream_set_user_data(l_stream, bufinfo, NULL);
	opj_stream_set_user_data_length(l_stream, bufinfo->sample_size);
	opj_stream_set_read_function(l_stream, (opj_stream_read_fn) read_from_buffer);
	opj_stream_set_write_function(l_stream, (opj_stream_write_fn) write_from_buffer);
	opj_stream_set_skip_function(l_stream, (opj_stream_skip_fn) skip_from_buffer);
	opj_stream_set_seek_function(l_stream, (opj_stream_seek_fn) seek_from_buffer);

	return l_stream;
}

OFCondition DJP2KDecoderBase::decodeFrame(
	DcmPixelSequence * fromPixSeq,
	const DJP2KCodecParameter *cp,
	DcmItem *dataset,
	Uint32 frameNo,
	Uint32& currentItem,
	void * buffer,
	Uint32 bufSize,
	Sint32 imageFrames,
	Uint16 imageColumns,
	Uint16 imageRows,
	Uint16 imageSamplesPerPixel,
	Uint16 bytesPerSample)
{
	DcmPixelItem *pixItem = NULL;
	Uint8* j2kData = NULL;
	size_t j2kSize = 0;
	Uint8* fragmentData = NULL;
	Uint32 fragmentSize = 0;
	Uint32 fragmentCount = 0;
	OFCondition result = EC_Normal;
	OFBool ignoreOffsetTable = cp->ignoreOffsetTable();

	// compute the number of JPEG 2000 fragments we need in order to decode the next frame
	fragmentCount = computeNumberOfFragments(imageFrames, frameNo, currentItem, ignoreOffsetTable, fromPixSeq);
	if (fragmentCount == 0)
		result = EC_JP2KCannotComputeNumberOfFragments;

	// determine planar configuration for uncompressed data
	OFString imageSopClass;
	OFString imagePhotometricInterpretation;
	dataset->findAndGetOFString(DCM_SOPClassUID, imageSopClass);
	dataset->findAndGetOFString(DCM_PhotometricInterpretation, imagePhotometricInterpretation);
	Uint16 imagePlanarConfiguration = 0; // 0 is color-by-pixel, 1 is color-by-plane

	if (imageSamplesPerPixel > 1) {
		switch (cp->getPlanarConfiguration()) {
		case EJ2KPC_restore:
			// get planar configuration from dataset
			imagePlanarConfiguration = 2; // invalid value
			dataset->findAndGetUint16(DCM_PlanarConfiguration, imagePlanarConfiguration);
			// determine auto default if not found or invalid
			if (imagePlanarConfiguration > 1)
				imagePlanarConfiguration = determinePlanarConfiguration(imageSopClass, imagePhotometricInterpretation);
			break;
		case EJ2KPC_auto:
			imagePlanarConfiguration = determinePlanarConfiguration(imageSopClass, imagePhotometricInterpretation);
			break;
		case EJ2KPC_colorByPixel:
			imagePlanarConfiguration = 0;
			break;
		case EJ2KPC_colorByPlane:
			imagePlanarConfiguration = 1;
			break;
		}
	}

	// get the size of all the fragments
	if (result.good()) {
		// Don't modify the original values for now
		Uint32 fragmentsForThisFrame2 = fragmentCount;
		Uint32 currentItem2 = currentItem;

		while (result.good() && fragmentsForThisFrame2--) {
			result = fromPixSeq->getItem(pixItem, currentItem2++);
			if (result.good() && pixItem) {
				fragmentSize = pixItem->getLength();
				if (result.good())
					j2kSize += fragmentSize;
			}
		} /* while */
	}

	// get the compressed data
	if (result.good()) {
		Uint32 offset = 0;
		j2kData = new Uint8[j2kSize];

		while (result.good() && fragmentCount--) {
			result = fromPixSeq->getItem(pixItem, currentItem++);
			if (result.good() && pixItem) {
				fragmentSize = pixItem->getLength();
				result = pixItem->getUint8Array(fragmentData);
				if (result.good() && fragmentData) {
					memcpy(&j2kData[offset], fragmentData, fragmentSize);
					offset += fragmentSize;
				}
			}
		} /* while */
	}

	// now, j2kData has entire JPEG 2000 code stream.
	if (result.good()) {
		if (memcmp(j2kData, J2K_CODESTREAM_MAGIC, 4) != 0) {
			result = EC_JP2KInvalidCompressedData;
		}
	}

	opj_stream_t* l_stream = NULL;
	opj_codec_t* l_codec = NULL;
	opj_image_t* l_image = NULL;

	BufferInfo bufinfo;
	memset(&bufinfo, 0, sizeof(BufferInfo));
	bufinfo.cur_buf = bufinfo.sample_buf = j2kData;
	bufinfo.sample_size = j2kSize;

	opj_dparameters_t l_param;
	opj_set_default_decoder_parameters(&l_param);
	l_param.decod_format = 0;

	if (result.good()) {
		l_stream = stream_create_buffer_stream(&bufinfo, j2kSize, OPJ_TRUE);
		if (l_stream == NULL) {
			result = EC_InternalError;
		}
	}

	if (result.good()) {
		l_codec = opj_create_decompress(OPJ_CODEC_J2K);
		if (l_codec == NULL) {
			result = EC_InternalError;
		}
	}

	opj_set_info_handler(l_codec, info_callback, 00);
	opj_set_warning_handler(l_codec, warning_callback, 00);
	opj_set_error_handler(l_codec, error_callback, 00);

	if (result.good()) {
		if (!opj_setup_decoder(l_codec, &l_param)) {
			result = EC_InternalError;
		}
	}

	if (result.good()) {
		if (!opj_read_header(l_stream, l_codec, &l_image)) {
			result = EC_InternalError;
		}
	}

	if (result.good()) {
		if (!opj_set_decode_area(l_codec, l_image, l_param.DA_x0, l_param.DA_y0, l_param.DA_x1, l_param.DA_y1)) {
			result = EC_InternalError;
		}
	}

	if (result.good()) {
		if( !opj_decode(l_codec, l_stream, l_image)) {
			result = EC_InternalError;
		}
	}

	if (result.good()) {
		if (!opj_end_decompress(l_codec, l_stream)) {
			result = EC_InternalError;
		}
	}

	if (l_codec) {
		opj_destroy_codec(l_codec);
	}

	if (result.good()) {
		if (l_image->numcomps == 3) {
			Sint32* rPos = l_image->comps[0].data;
			Sint32* gPos = l_image->comps[1].data;
			Sint32* bPos = l_image->comps[2].data;

			Uint8* outputBuffer = static_cast<Uint8*>(buffer);
			Uint8* oPos = outputBuffer;
			Uint8* oEnd = outputBuffer + bufSize;
			while(oPos < oEnd) {
				(*oPos++) = (*rPos++);
				(*oPos++) = (*gPos++);
				(*oPos++) = (*bPos++);
			}

			result = dataset->putAndInsertString(DCM_PhotometricInterpretation, "RGB");
			if (imagePlanarConfiguration == 1) {
				result = convertToPlanarConfiguration1Byte(outputBuffer, imageColumns, imageRows);
			}
		} else {
			Sint32* iPos = l_image->comps[0].data;

			Uint8* outputBuffer = static_cast<Uint8*>(buffer);
			Uint8* oPos = outputBuffer;
			Uint8* oEnd = outputBuffer + bufSize;
			while(oPos < oEnd) {
				if (bytesPerSample == 1) {
					(*oPos++) = (*iPos++);
				} else if (bytesPerSample == 2) {
					(*oPos++) = (*iPos) & 0xFF;
					(*oPos++) = ((*iPos) >> 8) & 0xFF;
					iPos++;
				}
			}
		}
	}

	if (result.good()) {
		// decompression is complete, finally adjust byte order if necessary
		/*
		if (bytesPerSample == 1) {	// we're writing bytes into words
			result = swapIfNecessary(gLocalByteOrder, EBO_LittleEndian, buffer,
					bufSize, sizeof(Uint16));
		}
		*/
	}

	if (result.bad()) {
		result = EC_JP2KInvalidCompressedData;
	}

	if (l_image) {
		opj_image_destroy(l_image);
	}

	if (j2kData) {
		delete[] j2kData;
	}

	return result;
}

OFCondition DJP2KDecoderBase::encode(
	const Uint16 * /* pixelData */,
	const Uint32 /* length */,
	const DcmRepresentationParameter * /* toRepParam */,
	DcmPixelSequence * & /* pixSeq */,
	const DcmCodecParameter * /* cp */,
	DcmStack & /* objStack */) const
{
	// we are a decoder only
	return EC_IllegalCall;
}

OFCondition DJP2KDecoderBase::encode(
	const E_TransferSyntax /* fromRepType */,
	const DcmRepresentationParameter * /* fromRepParam */,
	DcmPixelSequence * /* fromPixSeq */,
	const DcmRepresentationParameter * /* toRepParam */,
	DcmPixelSequence * & /* toPixSeq */,
	const DcmCodecParameter * /* cp */,
	DcmStack & /* objStack */) const
{
	// we don't support re-coding for now.
	return EC_IllegalCall;
}

OFCondition DJP2KDecoderBase::determineDecompressedColorModel(
	const DcmRepresentationParameter * /* fromParam */,
	DcmPixelSequence * /* fromPixSeq */,
	const DcmCodecParameter * /* cp */,
	DcmItem * dataset,
	OFString & decompressedColorModel) const
{
	OFCondition result = EC_IllegalParameter;
	if (dataset != NULL)
	{
		// retrieve color model from given dataset
		result = dataset->findAndGetOFString(DCM_PhotometricInterpretation, decompressedColorModel);
	}
	return result;
}


Uint16 DJP2KDecoderBase::determinePlanarConfiguration(
	const OFString& sopClassUID,
	const OFString& photometricInterpretation)
{
	// Hardcopy Color Image always requires color-by-plane
	if (sopClassUID == UID_RETIRED_HardcopyColorImageStorage) return 1;

	// The 1996 Ultrasound Image IODs require color-by-plane if color model is YBR_FULL.
	if (photometricInterpretation == "YBR_FULL")
	{
		if ((sopClassUID == UID_UltrasoundMultiframeImageStorage)
			|| (sopClassUID == UID_UltrasoundImageStorage)) return 1;
	}

	// default for all other cases
	return 0;
}

Uint32 DJP2KDecoderBase::computeNumberOfFragments(
	Sint32 numberOfFrames,
	Uint32 currentFrame,
	Uint32 startItem,
	OFBool ignoreOffsetTable,
	DcmPixelSequence * pixSeq)
{

	unsigned long numItems = pixSeq->card();
	DcmPixelItem *pixItem = NULL;

	// We first check the simple cases, that is, a single-frame image,
	// the last frame of a multi-frame image and the standard case where we do have
	// a single fragment per frame.
	if ((numberOfFrames <= 1) || (currentFrame + 1 == OFstatic_cast(Uint32, numberOfFrames)))
	{
		// single-frame image or last frame. All remaining fragments belong to this frame
		return (numItems - startItem);
	}
	if (OFstatic_cast(Uint32, numberOfFrames + 1) == numItems)
	{
		// multi-frame image with one fragment per frame
		return 1;
	}

	OFCondition result = EC_Normal;
	if (!ignoreOffsetTable)
	{
		// We do have a multi-frame image with multiple fragments per frame, and we are
		// not working on the last frame. Let's check the offset table if present.
		result = pixSeq->getItem(pixItem, 0);
		if (result.good() && pixItem)
		{
			Uint32 offsetTableLength = pixItem->getLength();
			if (offsetTableLength == (OFstatic_cast(Uint32, numberOfFrames) * 4))
			{
				// offset table is non-empty and contains one entry per frame
				Uint8 *offsetData = NULL;
				result = pixItem->getUint8Array(offsetData);
				if (result.good() && offsetData)
				{
					// now we can access the offset table
					Uint32 *offsetData32 = OFreinterpret_cast(Uint32 *, offsetData);

					// extract the offset for the NEXT frame. This offset is guaranteed to exist
					// because the "last frame/single frame" case is handled above.
					Uint32 offset = offsetData32[currentFrame + 1];

					// convert to local endian byte order (always little endian in file)
					swapIfNecessary(gLocalByteOrder, EBO_LittleEndian, &offset, sizeof(Uint32), sizeof(Uint32));

					// determine index of start fragment for next frame
					Uint32 byteCount = 0;
					Uint32 fragmentIndex = 1;
					while ((byteCount < offset) && (fragmentIndex < numItems))
					{
						pixItem = NULL;
						result = pixSeq->getItem(pixItem, fragmentIndex++);
						if (result.good() && pixItem)
						{
							byteCount += pixItem->getLength() + 8; // add 8 bytes for item tag and length
							if ((byteCount == offset) && (fragmentIndex > startItem))
							{
								// bingo, we have found the offset for the next frame
								return fragmentIndex - startItem;
							}
						}
						else break; /* something went wrong, break out of while loop */
					} /* while */
				}
			}
		}
	}

	// So we have a multi-frame image with multiple fragments per frame and the
	// offset table is empty or wrong. Our last chance is to peek into the JPEG 2000
	// bistream and identify the start of the next frame.
	Uint32 nextItem = startItem;
	Uint8 *fragmentData = NULL;
	while (++nextItem < numItems)
	{
		pixItem = NULL;
		result = pixSeq->getItem(pixItem, nextItem);
		if (result.good() && pixItem)
		{
			fragmentData = NULL;
			result = pixItem->getUint8Array(fragmentData);
			if (result.good() && fragmentData && (pixItem->getLength() > 3))
			{
				if (isJPEG2000StartOfImage(fragmentData))
				{
					// found a JPEG 2000 SOI marker. Assume that this is the start of the next frame.
					return (nextItem - startItem);
				}
			}
			else break; /* something went wrong, break out of while loop */
		}
		else break; /* something went wrong, break out of while loop */
	}

	// We're bust. No way to determine the number of fragments per frame.
	return 0;
}

OFBool DJP2KDecoderBase::isJPEG2000StartOfImage(Uint8 *fragmentData)
{
	// A valid JPEG 2000 bitstream will always start with an SOI marker FF4FFF51.
	if ((*fragmentData++) != 0xFF) return OFFalse;
	if ((*fragmentData++) != 0x4F) return OFFalse;
	if ((*fragmentData++) != 0xFF) return OFFalse;
	if ((*fragmentData != 0x51)) return OFFalse;

	return OFTrue;
}

