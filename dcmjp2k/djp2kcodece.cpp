/*
*
*	Copyright (C) 2007-2015, OFFIS e.V.
*	All rights reserved.	See COPYRIGHT file for details.
*
*	This software and supporting documentation were developed by
*
*		OFFIS e.V.
*		R&D Division Health
*		Escherweg 2
*		D-26121 Oldenburg, Germany
*
*
*	Module:	dcmj2k
*
*	Author:	Martin Willkomm, Marco Eichelberg, Uli Schlachter
*
*	Purpose: codec classes for JPEG 2000 encoders.
*
*/

#include "dcmtk/config/osconfig.h"
#include "dcmjp2k/djp2kcodece.h"

// ofstd includes
#include "dcmtk/ofstd/oflist.h"
#include "dcmtk/ofstd/ofstd.h"
#include "dcmtk/ofstd/ofstream.h"
#include "dcmtk/ofstd/offile.h"			/* for class OFFile */
#include "dcmtk/ofstd/ofbmanip.h"

#define INCLUDE_CMATH
#include "dcmtk/ofstd/ofstdinc.h"

// dcmdata includes
#include "dcmtk/dcmdata/dcdatset.h"	/* for class DcmDataset */
#include "dcmtk/dcmdata/dcdeftag.h"	/* for tag constants */
#include "dcmtk/dcmdata/dcovlay.h"	 /* for class DcmOverlayData */
#include "dcmtk/dcmdata/dcpixseq.h"	/* for class DcmPixelSequence */
#include "dcmtk/dcmdata/dcpxitem.h"	/* for class DcmPixelItem */
#include "dcmtk/dcmdata/dcuid.h"		 /* for dcmGenerateUniqueIdentifer()*/
#include "dcmtk/dcmdata/dcvrcs.h"		/* for class DcmCodeString */
#include "dcmtk/dcmdata/dcvrds.h"		/* for class DcmDecimalString */
#include "dcmtk/dcmdata/dcvrlt.h"		/* for class DcmLongText */
#include "dcmtk/dcmdata/dcvrst.h"		/* for class DcmShortText */
#include "dcmtk/dcmdata/dcvrus.h"		/* for class DcmUnsignedShort */
#include "dcmtk/dcmdata/dcswap.h"		/* for swapIfNecessary */

// dcmjp2k includes
#include "dcmjp2k/djp2kcparam.h"		 /* for class DJP2KCodecParameter */
#include "dcmjp2k/djp2krparam.h"		 /* for class DJP2KRepresentationParameter */
#include "dcmjp2k/djp2kerror.h"			/* for private class DJLSError */

// dcmimgle includes
#include "dcmtk/dcmimgle/dcmimage.h"	/* for class DicomImage */

// JPEG 2000 library (OpenJPEG) includes
#include "openjpeg.h"

#include <QTemporaryFile>

BEGIN_EXTERN_C
#ifdef HAVE_FCNTL_H
#include <fcntl.h>			 /* for O_RDONLY */
#endif
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>	 /* required for sys/stat.h */
#endif
#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>		/* for stat, fstat */
#endif
END_EXTERN_C

E_TransferSyntax DJP2KLosslessEncoder::supportedTransferSyntax() const
{
	return EXS_JPEG2000LosslessOnly;
}

E_TransferSyntax DJP2KLossyEncoder::supportedTransferSyntax() const
{
	return EXS_JPEG2000;
}

// --------------------------------------------------------------------------

DJP2KEncoderBase::DJP2KEncoderBase()
{
}

DJP2KEncoderBase::~DJP2KEncoderBase()
{
}

OFBool DJP2KEncoderBase::canChangeCoding(
	const E_TransferSyntax oldRepType,
	const E_TransferSyntax newRepType) const
{
	// this codec only handles conversion from uncompressed to JPEG 2000.
	DcmXfer oldRep(oldRepType);
	return (oldRep.isNotEncapsulated() && (newRepType == supportedTransferSyntax()));
}

OFCondition DJP2KEncoderBase::decode(
	const DcmRepresentationParameter * /* fromRepParam */,
	DcmPixelSequence * /* pixSeq */,
	DcmPolymorphOBOW& /* uncompressedPixelData */,
	const DcmCodecParameter * /* cp */,
	const DcmStack& /* objStack */) const
{
	// we are an encoder only
	return EC_IllegalCall;
}

OFCondition DJP2KEncoderBase::decodeFrame(
	const DcmRepresentationParameter * /* fromParam */,
	DcmPixelSequence * /* fromPixSeq */,
	const DcmCodecParameter * /* cp */,
	DcmItem * /* dataset */,
	Uint32 /* frameNo */,
	Uint32& /* startFragment */,
	void * /* buffer */,
	Uint32 /* bufSize */,
	OFString& /* decompressedColorModel */) const
{
	// we are an encoder only
	return EC_IllegalCall;
}

OFCondition DJP2KEncoderBase::encode(
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

OFCondition DJP2KEncoderBase::encode(
	const Uint16 * pixelData,
	const Uint32 length,
	const DcmRepresentationParameter * toRepParam,
	DcmPixelSequence * & pixSeq,
	const DcmCodecParameter *cp,
	DcmStack & objStack) const
{
	OFCondition result = EC_Normal;
	DJP2KRepresentationParameter defRep;

	// retrieve pointer to dataset from parameter stack
	DcmStack localStack(objStack);
	(void)localStack.pop();	// pop pixel data element from stack
	DcmObject *dobject = localStack.pop(); // this is the item in which the pixel data is located
	if ((!dobject) || ((dobject->ident() != EVR_dataset) && (dobject->ident() != EVR_item))) return EC_InvalidTag;
	DcmItem *dataset = OFstatic_cast(DcmItem *, dobject);

	// assume we can cast the codec and representation parameters to what we need
	const DJP2KCodecParameter *djcp = OFreinterpret_cast(const DJP2KCodecParameter *, cp);
	const DJP2KRepresentationParameter *djrp = OFreinterpret_cast(const DJP2KRepresentationParameter *, toRepParam);
	double compressionRatio = 0.0;

	if (!djrp)
		djrp = &defRep;
	/*
	if (supportedTransferSyntax() == EXS_JPEGLSLossless || djrp->useLosslessProcess())
	{
		if (djcp->cookedEncodingPreferred())
			result = encodeWithOverlays(pixelData, length, dataset, djrp, pixSeq, djcp, compressionRatio, 0);
		else result = encode(pixelData, length, dataset, djrp, pixSeq, djcp, compressionRatio);
	}
	else
	{
		// near-lossless mode always uses the "cooked" encoder since this one is guaranteed not to "mix"
		// overlays and pixel data in one cell subjected to lossy compression.
		result = encodeWithOverlays(pixelData, length, dataset, djrp, pixSeq, djcp, compressionRatio, djrp->getnearlosslessDeviation());
	}
	*/
	result = encode(pixelData, length, dataset, djrp, pixSeq, djcp, compressionRatio);

	// the following operations do not affect the Image Pixel Module
	// but other modules such as SOP Common.	We only perform these
	// changes if we're on the main level of the dataset,
	// which should always identify itself as dataset, not as item.
	if (result.good() && dataset->ident() == EVR_dataset)
	{
		Uint16 samplesPerPixel;
		if (result.good()) result = dataset->findAndGetUint16(DCM_SamplesPerPixel, samplesPerPixel);

		if (supportedTransferSyntax() == EXS_JPEG2000LosslessOnly || djrp->useLosslessProcess()) {
			// lossless process - create new UID if mode is EUC_always or if we're converting to Secondary Capture
			if (djcp->getConvertToSC() || (djcp->getUIDCreation() == EJ2KUC_always))
				result = DcmCodec::newInstance(dataset, "DCM", "121320", "Uncompressed predecessor");

			// set phtometric interpretation to YBR_RCT if samplesPerPixel == 3
			if (result.good() && samplesPerPixel == 3) result = dataset->putAndInsertString(DCM_PhotometricInterpretation, "YBR_RCT");
		} else {
			// lossy process - create new UID unless mode is EUC_never and we're not converting to Secondary Capture
			if (djcp->getConvertToSC() || (djcp->getUIDCreation() != EJ2KUC_never))
				result = DcmCodec::newInstance(dataset, "DCM", "121320", "Uncompressed predecessor");

			// update image type
			if (result.good()) result = DcmCodec::updateImageType(dataset);

			// update derivation description
			if (result.good()) result = updateDerivationDescription(dataset, djrp, compressionRatio);

			// update lossy compression ratio
			if (result.good()) result = updateLossyCompressionRatio(dataset, compressionRatio);

			// set phtometric interpretation to YBR_ICT if samplesPerPixel == 3
			if (result.good() && samplesPerPixel == 3) result = dataset->putAndInsertString(DCM_PhotometricInterpretation, "YBR_ICT");
		}

		// convert to Secondary Capture if requested by user.
		// This method creates a new SOP class UID, so it should be executed
		// after the call to newInstance() which creates a Source Image Sequence.
		if (result.good() && djcp->getConvertToSC()) result = DcmCodec::convertToSecondaryCapture(dataset);
	}

	return result;
}

OFCondition DJP2KEncoderBase::determineDecompressedColorModel(
	const DcmRepresentationParameter * /* fromParam */,
	DcmPixelSequence * /* fromPixSeq */,
	const DcmCodecParameter * /* cp */,
	DcmItem * /* dataset */,
	OFString & /* decompressedColorModel */) const
{
	return EC_IllegalCall;
}

OFCondition DJP2KEncoderBase::encode(
	const Uint16 *pixelData,
	const Uint32 length,
	DcmItem *dataset,
	const DJP2KRepresentationParameter *djrp,
	DcmPixelSequence * & pixSeq,
	const DJP2KCodecParameter *djcp,
	double& compressionRatio) const
{
	compressionRatio = 0.0; // initialize if something goes wrong

	// determine image properties
	Uint16 bitsAllocated = 0;
	Uint16 bitsStored = 0;
	Uint16 bytesAllocated = 0;
	Uint16 samplesPerPixel = 0;
	Uint16 pixelRepresentation = 0;
	Uint16 planarConfiguration = 0;
	Uint16 columns = 0;
	Uint16 rows = 0;
	Sint32 numberOfFrames = 1;
	OFBool byteSwapped = OFFalse;	// true if we have byte-swapped the original pixel data
	OFString photometricInterpretation;

	OFCondition result = dataset->findAndGetUint16(DCM_BitsAllocated, bitsAllocated);
	if (result.good()) result = dataset->findAndGetUint16(DCM_BitsStored, bitsStored);
	if (result.good()) result = dataset->findAndGetUint16(DCM_SamplesPerPixel, samplesPerPixel);
	if (result.good()) result = dataset->findAndGetUint16(DCM_Columns, columns);
	if (result.good()) result = dataset->findAndGetUint16(DCM_Rows, rows);
	if (result.good()) result = dataset->findAndGetOFString(DCM_PhotometricInterpretation, photometricInterpretation);
	if (result.good()) result = dataset->findAndGetUint16(DCM_PixelRepresentation, pixelRepresentation);
	if (result.good()) {
		result = dataset->findAndGetSint32(DCM_NumberOfFrames, numberOfFrames);
		if (result.bad() || numberOfFrames < 1) numberOfFrames = 1;
		result = EC_Normal;
	}
	if (result.good() && (samplesPerPixel > 1)) {
		result = dataset->findAndGetUint16(DCM_PlanarConfiguration, planarConfiguration);
	}

	if (result.good()) {
		// check if bitsAllocated is 8 or 16 - we don't handle anything else
		if (bitsAllocated == 8) {
			bytesAllocated = 1;
		} else if (bitsAllocated == 16) {
			bytesAllocated = 2;
		} else {
			/*
			if (photometricInterpretation == "MONOCHROME1" ||
				photometricInterpretation == "MONOCHROME2" ||
				photometricInterpretation == "RGB" ||
				photometricInterpretation == "YBR_FULL")
			{
				// A bitsAllocated value that we don't handle, but a color model that indicates
				// that the cooked encoder could handle this case. Fall back to cooked encoder.
				return encodeWithOverlays(pixelData, length, dataset, djrp, pixSeq, djcp, compressionRatio, 0);
			}
			*/
			// an image that is not supported by either the raw or the cooked encoder.
			result = EC_JP2KUnsupportedImageType;
		}

		// make sure that all the descriptive attributes have sensible values
		if ((columns < 1) || (rows < 1) || (samplesPerPixel < 1)) result = EC_JP2KUnsupportedImageType;

		// make sure that we have at least as many bytes of pixel data as we expect
		if (bytesAllocated * samplesPerPixel * columns * rows *
			OFstatic_cast(unsigned long, numberOfFrames) > length)
			result = EC_JP2KUncompressedBufferTooSmall;
	}

	DcmPixelSequence *pixelSequence = NULL;
	DcmPixelItem *offsetTable = NULL;

	// create initial pixel sequence
	if (result.good()) {
		pixelSequence = new DcmPixelSequence(DCM_PixelSequenceTag);
		if (pixelSequence == NULL) {
			result = EC_MemoryExhausted;
		} else {
			// create empty offset table
			offsetTable = new DcmPixelItem(DCM_PixelItemTag);
			if (offsetTable == NULL) result = EC_MemoryExhausted;
			else pixelSequence->insert(offsetTable);
		}
	}

	DcmOffsetList offsetList;
	unsigned long compressedSize = 0;
	unsigned long compressedFrameSize = 0;
	double uncompressedSize = 0.0;

	// render and compress each frame
	if (result.good()) {

		// byte swap pixel data to little endian if bits allocate is 8
		if ((gLocalByteOrder == EBO_BigEndian) && (bitsAllocated == 8)) {
			swapIfNecessary(EBO_LittleEndian, gLocalByteOrder, OFstatic_cast(void *, OFconst_cast(Uint16 *, pixelData)), length, sizeof(Uint16));
			byteSwapped = OFTrue;
		}

		unsigned long frameCount = OFstatic_cast(unsigned long, numberOfFrames);
		unsigned long frameSize = columns * rows * samplesPerPixel * bytesAllocated;
		const Uint8 *framePointer = OFreinterpret_cast(const Uint8 *, pixelData);

		// compute original image size in bytes, ignoring any padding bits.
		uncompressedSize = columns * rows * samplesPerPixel * bitsStored * frameCount / 8.0;

		for (unsigned long i = 0; (i<frameCount) && (result.good()); ++i) {
			// compress frame
			DCMJP2K_DEBUG("JPEG 2000 encoder processes frame " << (i + 1) << " of " << frameCount);
			result = encodeFrame(framePointer, bitsAllocated, bitsStored, columns, rows,
				samplesPerPixel, pixelRepresentation, planarConfiguration, photometricInterpretation,
				djrp, pixelSequence, offsetList, compressedFrameSize, djcp);

			compressedSize += compressedFrameSize;
			framePointer += frameSize;
		}
	}

	// store pixel sequence if everything went well.
	if (result.good()) {
		pixSeq = pixelSequence;
	} else {
		delete pixelSequence;
		pixSeq = NULL;
	}

	// create offset table
	if ((result.good()) && (djcp->getCreateOffsetTable())) {
		result = offsetTable->createOffsetTable(offsetList);
	}

	if (compressedSize > 0) {
		compressionRatio = uncompressedSize / compressedSize;
	}

	// byte swap pixel data back to local endian if necessary
	if (byteSwapped) {
		swapIfNecessary(gLocalByteOrder, EBO_LittleEndian, OFstatic_cast(void *, OFconst_cast(Uint16 *, pixelData)), length, sizeof(Uint16));
	}

	return result;
}

#define NUM_COMPS_MAX 4

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

OFCondition DJP2KEncoderBase::encodeFrame(
	const Uint8 *framePointer,
	Uint16 bitsAllocated,
	Uint16 bitsStored,
	Uint16 width,
	Uint16 height,
	Uint16 samplesPerPixel,
	Uint16 pixelRepresentation,
	Uint16 planarConfiguration,
	const OFString& photometricInterpretation,
	const DJP2KRepresentationParameter *djrp,
	DcmPixelSequence *pixelSequence,
	DcmOffsetList &offsetList,
	unsigned long &compressedSize,
	const DJP2KCodecParameter *djcp) const
{
	OFCondition result = EC_Normal;
	Uint16 bytesAllocated = bitsAllocated / 8;
	Uint32 frameSize = width * height * bytesAllocated * samplesPerPixel;
	Uint32 fragmentSize = djcp->getFragmentSize();

	opj_cparameters_t l_param;
	opj_codec_t * l_codec = NULL;
	opj_image_t * l_image = NULL;
	opj_image_cmptparm_t l_params [NUM_COMPS_MAX];
	opj_stream_t * l_stream = NULL;

	QTemporaryFile tempFile;
	if (!tempFile.open()) {
		return EC_CouldNotCreateTemporaryFile;
	}
	QByteArray tempFilename = tempFile.fileName().toLocal8Bit();
	const char* output_file = tempFilename.constData();
	tempFile.close();

	opj_image_cmptparm_t * l_current_param_ptr;
	OPJ_UINT32 i;

	OPJ_UINT32 num_comps = samplesPerPixel;
	int tile_width = width;
	int tile_height = height;

	if (num_comps > NUM_COMPS_MAX) {
		return EC_IllegalParameter;
	}
	OPJ_UINT32 l_nb_tiles = (OPJ_UINT32)(width/tile_width) * (OPJ_UINT32)(height/tile_height);

	opj_set_default_encoder_parameters(&l_param);

	/* tile definitions parameters */
	/* position of the tile grid aligned with the image */
	l_param.cp_tx0 = 0;
	l_param.cp_ty0 = 0;
	/* tile size, we are using tile based encoding */
	l_param.tile_size_on = OPJ_TRUE;
	l_param.cp_tdx = tile_width;
	l_param.cp_tdy = tile_height;

	/* use irreversible encoding ?*/
	l_param.irreversible = (supportedTransferSyntax() != EXS_JPEG2000LosslessOnly) ? OPJ_TRUE : OPJ_FALSE;

	/* rate specifications */
	if (l_param.irreversible) {	// lossy
		l_param.tcp_numlayers = 5;
		l_param.tcp_rates[0] = djrp->getCompressionRatio() * 20;
		l_param.tcp_rates[1] = djrp->getCompressionRatio() * 10;
		l_param.tcp_rates[2] = djrp->getCompressionRatio() * 5;
		l_param.tcp_rates[3] = djrp->getCompressionRatio() * 3;
		l_param.tcp_rates[4] = djrp->getCompressionRatio();
		l_param.cp_disto_alloc = 1;
	} else {					// lossless
		l_param.tcp_numlayers = 5;
		l_param.tcp_rates[0] = 20;
		l_param.tcp_rates[1] = 10;
		l_param.tcp_rates[2] = 5;
		l_param.tcp_rates[3] = 3;
		l_param.tcp_rates[4] = 1;
		l_param.cp_disto_alloc = 1;
	}

	/* use SOP or EPH markers */
	l_param.csty |= 0x02;
	l_param.csty |= 0x04;

	/** block encoding style for each component, do not use at the moment */
	/** J2K_CCP_CBLKSTY_TERMALL, J2K_CCP_CBLKSTY_LAZY, J2K_CCP_CBLKSTY_VSC, J2K_CCP_CBLKSTY_SEGSYM, J2K_CCP_CBLKSTY_RESET */
	/* l_param.mode = 0;*/

	/** number of resolutions */
	l_param.numresolution = 6;

	/** progression order to use*/
	/** OPJ_LRCP, OPJ_RLCP, OPJ_RPCL, PCRL, CPRL */
	l_param.prog_order = OPJ_LRCP;

	/* image definition */
	l_current_param_ptr = l_params;
	for(i = 0; i < num_comps; ++i) {
		l_current_param_ptr->dx = 1;
		l_current_param_ptr->dy = 1;

		l_current_param_ptr->h = height;
		l_current_param_ptr->w = width;

		l_current_param_ptr->x0 = 0;
		l_current_param_ptr->y0 = 0;

		l_current_param_ptr->prec = bitsStored;
		l_current_param_ptr->bpp = bitsAllocated;
		l_current_param_ptr->sgnd = pixelRepresentation;

		++l_current_param_ptr;
	}

	OPJ_BYTE* frameData = NULL;
	frameData = new OPJ_BYTE[frameSize];
	if (frameData == NULL) {
		result = EC_MemoryExhausted;
	}

	if (result.good()) {
		if (samplesPerPixel > 1 && planarConfiguration == 0) {
			result = convertToPlanarConfiguration1(frameData, framePointer, samplesPerPixel, width, height, bitsAllocated);
		} else {
			memcpy(frameData, framePointer, frameSize);
		}
	}

	if (result.good()) {
		l_codec = opj_create_compress(OPJ_CODEC_J2K);
		if (l_codec == NULL) {
			result = EC_InternalError;
		}
	}

	/* catch events using our callbacks and give a local context */
	opj_set_info_handler(l_codec, info_callback, 00);
	opj_set_warning_handler(l_codec, warning_callback, 00);
	opj_set_error_handler(l_codec, error_callback, 00);

	OPJ_COLOR_SPACE colorSpace;
	if (samplesPerPixel == 1) {
		colorSpace = OPJ_CLRSPC_GRAY;
	} else if (samplesPerPixel == 3) {
		if (photometricInterpretation == "RGB") {
			colorSpace = OPJ_CLRSPC_SRGB;
		} else if (photometricInterpretation == "YBR_FULL") {
			colorSpace = OPJ_CLRSPC_SYCC;
		} else {
			result = EC_InvalidValue;
		}
	} else {
		result = EC_InvalidValue;
	}

	if (result.good()) {
		l_image = opj_image_tile_create(num_comps, l_params, colorSpace);
		if (l_image == NULL) {
			result = EC_InternalError;
		}
	}

	if (result.good()) {
		l_image->x0 = 0;
		l_image->y0 = 0;
		l_image->x1 = (OPJ_UINT32)width;
		l_image->y1 = (OPJ_UINT32)height;
		l_image->color_space = colorSpace;

		if (!opj_setup_encoder(l_codec, &l_param, l_image)) {
			result = EC_InternalError;
		}
	}

	if (result.good()) {
		l_stream = opj_stream_create_default_file_stream(output_file, OPJ_FALSE);
		if (!l_stream) {
			result = EC_InternalError;
		}
	}

	if (result.good()) {
		if (!opj_start_compress(l_codec, l_image, l_stream)) {
			result = EC_InternalError;
		}
	}

	if (result.good()) {
		for(i = 0; i < l_nb_tiles; ++i) {
			if (!opj_write_tile(l_codec, i, frameData, frameSize, l_stream)) {
				result = EC_InternalError;
			}
		}
	}

	if (result.good()) {
		if (!opj_end_compress(l_codec, l_stream)) {
			result = EC_InternalError;
		}
	}

	if (l_stream) {
		opj_stream_destroy(l_stream);
	}
	if (l_image) {
		opj_image_destroy(l_image);
	}
	if (l_codec) {
		opj_destroy_codec(l_codec);
	}
	if (frameData) {
		delete[] frameData;
	}

	if (result.good()) {
		FILE* fp = fopen(output_file, "rb");
		fseek(fp, 0, SEEK_END);
		compressedSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		Uint8* j2kData = new Uint8[compressedSize];
		fread(j2kData, compressedSize, 1, fp);
		fclose(fp);

		result = pixelSequence->storeCompressedFrame(offsetList, j2kData, compressedSize, fragmentSize);

		delete[] j2kData;
		remove(output_file);
	}

	return EC_Normal;
}

OFCondition DJP2KEncoderBase::updateLossyCompressionRatio(
	DcmItem *dataset,
	double ratio) const
{
	if (dataset == NULL) return EC_IllegalCall;

	// set Lossy Image Compression to "01" (see DICOM part 3, C.7.6.1.1.5)
	OFCondition result = dataset->putAndInsertString(DCM_LossyImageCompression, "01");
	if (result.bad()) return result;

	// set Lossy Image Compression Ratio
	OFString s;
	const char *oldRatio = NULL;
	if ((dataset->findAndGetString(DCM_LossyImageCompressionRatio, oldRatio)).good() && oldRatio)
	{
		s = oldRatio;
		s += "\\";
	}

	// append lossy compression ratio
	char buf[64];
	OFStandard::ftoa(buf, sizeof(buf), ratio, OFStandard::ftoa_uppercase, 0, 5);
	s += buf;

	result = dataset->putAndInsertString(DCM_LossyImageCompressionRatio, s.c_str());
	if (result.bad()) return result;

	// count VM of lossy image compression ratio
	size_t i;
	size_t s_vm = 0;
	size_t s_sz = s.size();
	for (i = 0; i < s_sz; ++i)
		if (s[i] == '\\') ++s_vm;

	// set Lossy Image Compression Method
	const char *oldMethod = NULL;
	OFString m;
	if ((dataset->findAndGetString(DCM_LossyImageCompressionMethod, oldMethod)).good() && oldMethod)
	{
		m = oldMethod;
		m += "\\";
	}

	// count VM of lossy image compression method
	size_t m_vm = 0;
	size_t m_sz = m.size();
	for (i = 0; i < m_sz; ++i)
		if (m[i] == '\\') ++m_vm;

	// make sure that VM of Compression Method is not smaller than	VM of Compression Ratio
	while (m_vm++ < s_vm) m += "\\";

	m += "ISO_14495_1";
	return dataset->putAndInsertString(DCM_LossyImageCompressionMethod, m.c_str());
}

OFCondition DJP2KEncoderBase::updateDerivationDescription(
	DcmItem *dataset,
	const DJP2KRepresentationParameter *djrp,
	double ratio) const
{
	OFString derivationDescription;
	char buf[64];

	derivationDescription = "lossy JPEG 2000 compression, factor ";
	OFStandard::ftoa(buf, sizeof(buf), ratio, OFStandard::ftoa_uppercase, 0, 5);
	derivationDescription += buf;
	sprintf(buf, " (NEAR=%lu)", OFstatic_cast(unsigned long, djrp->getCompressionRatio()));
	derivationDescription += buf;

	// append old Derivation Description, if any
	const char *oldDerivation = NULL;
	if ((dataset->findAndGetString(DCM_DerivationDescription, oldDerivation)).good() && oldDerivation)
	{
		derivationDescription += " [";
		derivationDescription += oldDerivation;
		derivationDescription += "]";
		if (derivationDescription.length() > 1024)
		{
			// ST is limited to 1024 characters, cut off tail
			derivationDescription.erase(1020);
			derivationDescription += "...]";
		}
	}

	OFCondition result = dataset->putAndInsertString(DCM_DerivationDescription, derivationDescription.c_str());
	if (result.good()) result = DcmCodec::insertCodeSequence(dataset, DCM_DerivationCodeSequence, "DCM", "113040", "Lossy Compression");
	return result;
}
