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
			result = encodeFrame(framePointer, bitsAllocated, columns, rows,
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
	char output_file[64] = "/Users/samuelchoi/Downloads/output.j2k";	// FIXME

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

		l_current_param_ptr->prec = bitsAllocated;
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

	OPJ_COLOR_SPACE colorSpace = (samplesPerPixel > 1) ? OPJ_CLRSPC_SRGB : OPJ_CLRSPC_GRAY;

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
	}

	return EC_Normal;
}

#ifdef	NEVER2
OFCondition DJP2KEncoderBase::encodeWithOverlays(
	const Uint16 *pixelData,
	const Uint32 length,
	DcmItem *dataset,
	const DJP2KRepresentationParameter *djrp,
	DcmPixelSequence * & pixSeq,
	const DJP2KCodecParameter *djcp,
	double& compressionRatio,
	Uint16 nearLosslessDeviation) const
{
	compressionRatio = 0.0; // initialize if something goes wrong

	// determine a few image properties
	OFString photometricInterpretation;
	Uint16 bitsAllocated = 0;
	OFCondition result = dataset->findAndGetOFString(DCM_PhotometricInterpretation, photometricInterpretation);
	if (result.good()) result = dataset->findAndGetUint16(DCM_BitsAllocated, bitsAllocated);
	if (result.bad()) return result;

	// The cooked encoder only handles the following photometic interpretations
	if (photometricInterpretation != "MONOCHROME1" &&
		photometricInterpretation != "MONOCHROME2" &&
		photometricInterpretation != "RGB" &&
		photometricInterpretation != "YBR_FULL")
	{
		// a photometric interpretation that we don't handle. Fall back to raw encoder (unless in near-lossless mode)
		if (nearLosslessDeviation > 0) return EC_JP2KUnsupportedPhotometricInterpretation;
		else return encode(pixelData, length, dataset, djrp, pixSeq, djcp, compressionRatio);
	}

	Uint16 pixelRepresentation = 0;
	result = dataset->findAndGetUint16(DCM_PixelRepresentation, pixelRepresentation);
	if (result.bad()) return result;

	if (pixelRepresentation > 0)
	{
		// in near lossless mode we cannot handle signed images because JPEG 2000 assumes unsigned pixels
		if (nearLosslessDeviation > 0) return EC_JP2KUnsupportedPixelRepresentation;

		// The cooked encoder only handles unsigned color images
		if (photometricInterpretation == "RGB" || photometricInterpretation == "YBR_FULL")
		{
			// prevent a loop - only call lossless raw encoder if bitsAllocated is OK for the raw encoder
			if ((bitsAllocated == 8) || (bitsAllocated == 16))
				return encode(pixelData, length, dataset, djrp, pixSeq, djcp, compressionRatio);
			else return EC_JP2KUnsupportedPixelRepresentation;
		}
	}

	DcmPixelSequence *pixelSequence = NULL;
	DcmPixelItem *offsetTable = NULL;

	// ignore modality transformation (rescale slope/intercept or LUT) stored in the dataset
	unsigned long flags = CIF_IgnoreModalityTransformation;
	// don't convert YCbCr (Full and Full 4:2:2) color images to RGB
	flags |= CIF_KeepYCbCrColorModel;
	// Don't optimize memory usage, but keep using the same bitsAllocated.
	// Without this, the DICOM and the JPEG 2000 value for bitsAllocated could
	// differ and the decoder would error out.
	flags |= CIF_UseAbsolutePixelRange;

	DicomImage *dimage = new DicomImage(dataset, EXS_LittleEndianImplicit, flags); // read all frames
	if (dimage == NULL) return EC_MemoryExhausted;
	if (dimage->getStatus() != EIS_Normal)
	{
		delete dimage;
		return EC_IllegalCall;
	}

	// create overlay data for embedded overlays
	result = adjustOverlays(dataset, *dimage);

	// determine number of bits per sample
	int bitsPerSample = dimage->getDepth();
	if (result.good() && (bitsPerSample > 16)) result = EC_JP2KUnsupportedBitDepth;

	// create initial pixel sequence
	if (result.good())
	{
		pixelSequence = new DcmPixelSequence(DCM_PixelSequenceTag);
		if (pixelSequence == NULL) result = EC_MemoryExhausted;
		else
		{
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
	if (result.good())
	{
		unsigned long frameCount = dimage->getFrameCount();

		// compute original image size in bytes, ignoring any padding bits.
		Uint16 samplesPerPixel = 0;
		if ((dataset->findAndGetUint16(DCM_SamplesPerPixel, samplesPerPixel)).bad()) samplesPerPixel = 1;
		uncompressedSize = dimage->getWidth() * dimage->getHeight() *
			bitsPerSample * frameCount * samplesPerPixel / 8.0;

		for (unsigned long i = 0; (i<frameCount) && (result.good()); ++i)
		{
			// compress frame
			DCMJP2K_DEBUG("JPEG 2000 encoder processes frame " << (i + 1) << " of " << frameCount);
			result = encodeFrameWithOverlays(pixelSequence, dimage,
				photometricInterpretation, offsetList, compressedFrameSize, djcp, i, nearLosslessDeviation);

			compressedSize += compressedFrameSize;
		}
	}

	// store pixel sequence if everything went well.
	if (result.good()) pixSeq = pixelSequence;
	else
	{
		delete pixelSequence;
		pixSeq = NULL;
	}

	// create offset table
	if ((result.good()) && (djcp->getCreateOffsetTable()))
	{
		result = offsetTable->createOffsetTable(offsetList);
	}

	// adapt attributes in image pixel module
	if (result.good())
	{
		// adjustments needed for both color and monochrome
		if (bitsPerSample > 8)
			result = dataset->putAndInsertUint16(DCM_BitsAllocated, 16);
		else
			result = dataset->putAndInsertUint16(DCM_BitsAllocated, 8);
		if (result.good()) result = dataset->putAndInsertUint16(DCM_BitsStored, bitsPerSample);
		if (result.good()) result = dataset->putAndInsertUint16(DCM_HighBit, bitsPerSample - 1);
	}

	if (compressedSize > 0) compressionRatio = uncompressedSize / compressedSize;
	delete dimage;
	return result;
}

OFCondition DJP2KEncoderBase::adjustOverlays(
	DcmItem *dataset,
	DicomImage& image) const
{
	if (dataset == NULL) return EC_IllegalCall;

	unsigned int overlayCount = image.getOverlayCount();
	if (overlayCount > 0)
	{
		Uint16 group = 0;
		DcmStack stack;
		unsigned long bytesAllocated = 0;
		Uint8 *buffer = NULL;
		unsigned int width = 0;
		unsigned int height = 0;
		unsigned long frames = 0;
		DcmElement *elem = NULL;
		OFCondition result = EC_Normal;

		// adjust overlays (prior to grayscale compression)
		for (unsigned int i = 0; i < overlayCount; i++)
		{
			// check if current overlay is embedded in pixel data
			group = OFstatic_cast(Uint16, image.getOverlayGroupNumber(i));
			stack.clear();
			if ((dataset->search(DcmTagKey(group, 0x3000), stack, ESM_fromHere, OFFalse)).bad())
			{
				// separate Overlay Data not found. Assume overlay is embedded.
				bytesAllocated = image.create6xxx3000OverlayData(buffer, i, width, height, frames);
				if (bytesAllocated > 0)
				{
					elem = new DcmOverlayData(DcmTagKey(group, 0x3000)); // DCM_OverlayData
					if (elem)
					{
						result = elem->putUint8Array(buffer, bytesAllocated);
						delete[] buffer;
						if (result.good())
						{
							dataset->insert(elem, OFTrue /*replaceOld*/);
							// DCM_OverlayBitsAllocated
							result = dataset->putAndInsertUint16(DcmTagKey(group, 0x0100), 1);
							// DCM_OverlayBitPosition
							if (result.good()) result = dataset->putAndInsertUint16(DcmTagKey(group, 0x0102), 0);
						}
						else
						{
							delete elem;
							return result;
						}
					}
					else
					{
						delete[] buffer;
						return EC_MemoryExhausted;
					}
				}
				else return EC_IllegalCall;
			}
		}
	}
	return EC_Normal;
}

OFCondition DJP2KEncoderBase::encodeFrameWithOverlays(
	DcmPixelSequence *pixelSequence,
	DicomImage *dimage,
	const OFString& /* photometricInterpretation */,
	DcmOffsetList &offsetList,
	unsigned long &compressedSize,
	const DJP2KCodecParameter *djcp,
	Uint32 frame,
	Uint16 nearLosslessDeviation) const
{
	if (dimage == NULL) return EC_IllegalCall;

	// access essential image parameters
	int width = dimage->getWidth();
	int height = dimage->getHeight();
	int depth = dimage->getDepth();
	if ((depth < 1) || (depth > 16)) return EC_JP2KUnsupportedBitDepth;

	Uint32 fragmentSize = djcp->getFragmentSize();
	OFBool opt_use_custom_options = djcp->getUseCustomOptions();

	const DiPixel *dinter = dimage->getInterData();
	if (dinter == NULL) return EC_IllegalCall;

	// There should be no other possibilities
	int samplesPerPixel = dinter->getPlanes();
	if (samplesPerPixel != 1 && samplesPerPixel != 3) return EC_IllegalCall;

	// get pointer to internal raw representation of image data
	const void *draw = dinter->getData();
	if (draw == NULL) return EC_IllegalCall;

	OFCondition result = EC_Normal;

	const void *planes[3] = { NULL, NULL, NULL };
	if (samplesPerPixel == 3)
	{
		// for color images, dinter->getData() returns a pointer to an array
		// of pointers pointing to the real plane data
		const void * const * draw_array = OFstatic_cast(const void * const *, draw);
		planes[0] = draw_array[0];
		planes[1] = draw_array[1];
		planes[2] = draw_array[2];
	}
	else
	{
		// for monochrome images, dinter->getData() directly returns a pointer
		// to the single monochrome plane.
		planes[0] = draw;
	}

	// This is the buffer with the uncompressed pixel data
	Uint8 *buffer;
	size_t buffer_size;

	Uint32 framesize = dimage->getWidth() * dimage->getHeight();
	switch (dinter->getRepresentation())
	{
	case EPR_Uint8:
	case EPR_Sint8:
	{
		// image representation is 8 bit signed or unsigned
		if (samplesPerPixel == 1)
		{
			const Uint8 *yv = OFreinterpret_cast(const Uint8 *, planes[0]) + framesize * frame;
			buffer_size = framesize;
			buffer = new Uint8[buffer_size];
			memcpy(buffer, yv, framesize);
		}
		else
		{
			const Uint8 *rv = OFreinterpret_cast(const Uint8 *, planes[0]) + framesize * frame;
			const Uint8 *gv = OFreinterpret_cast(const Uint8 *, planes[1]) + framesize * frame;
			const Uint8 *bv = OFreinterpret_cast(const Uint8 *, planes[2]) + framesize * frame;

			buffer_size = framesize * 3;
			buffer = new Uint8[buffer_size];

			size_t i = 0;
			for (int row = height; row; --row)
			{
				for (int col = width; col; --col)
				{
					buffer[i++] = *rv;
					buffer[i++] = *gv;
					buffer[i++] = *bv;

					rv++;
					gv++;
					bv++;
				}
			}
		}
	}
	break;
	case EPR_Uint16:
	case EPR_Sint16:
	{
		// image representation is 16 bit signed or unsigned
		if (samplesPerPixel == 1)
		{
			const Uint16 *yv = OFreinterpret_cast(const Uint16 *, planes[0]) + framesize * frame;
			buffer_size = framesize*sizeof(Uint16);
			buffer = new Uint8[buffer_size];
			memcpy(buffer, yv, buffer_size);
		}
		else
		{
			const Uint16 *rv = OFreinterpret_cast(const Uint16 *, planes[0]) + framesize * frame;
			const Uint16 *gv = OFreinterpret_cast(const Uint16 *, planes[1]) + framesize * frame;
			const Uint16 *bv = OFreinterpret_cast(const Uint16 *, planes[2]) + framesize * frame;

			buffer_size = framesize * 3;
			Uint16 *buffer16 = new Uint16[buffer_size];
			buffer = OFreinterpret_cast(Uint8 *, buffer16);

			// Convert to byte count
			buffer_size *= 2;

			size_t i = 0;
			for (int row = height; row; --row)
			{
				for (int col = width; col; --col)
				{
					buffer16[i++] = *rv;
					buffer16[i++] = *gv;
					buffer16[i++] = *bv;

					rv++;
					gv++;
					bv++;
				}
			}
		}
	}
	break;
	default:
		// we don't support images with > 16 bits/sample
		return EC_JP2KUnsupportedBitDepth;
		break;
	}

/*	JlsParameters jls_params;

	// Set up the information structure for CharLS
	OFBitmanipTemplate<char>::zeroMem((char *)&jls_params, sizeof(jls_params));
	jls_params.height = height;
	jls_params.width = width;
	jls_params.allowedlossyerror = nearLosslessDeviation;
	jls_params.outputBgr = false;
	jls_params.bitspersample = depth;
	// No idea what this one does, but I don't think DICOM says anything about it
	jls_params.colorTransform = 0;

	// This was already checked for a sane value above
	jls_params.components = samplesPerPixel;
	switch (dinter->getRepresentation())
	{
	case EPR_Uint8:
	case EPR_Sint8:
		jls_params.bitspersample = 8;
		break;
	case EPR_Uint16:
	case EPR_Sint16:
		jls_params.bitspersample = 16;
		break;
	default:
		// Everything else was already handled above and can't happen here
		break;
	}

	// Unset: jls_params.jfif (thumbnail, dpi)

	if (opt_use_custom_options)
	{
		jls_params.custom.T1 = djcp->getT1();
		jls_params.custom.T2 = djcp->getT2();
		jls_params.custom.T3 = djcp->getT3();
		jls_params.custom.RESET = djcp->getReset();
	}

	switch (djcp->getJplsInterleaveMode())
	{
	case DJP2KCodecParameter::interleaveSample:
		jls_params.ilv = ILV_SAMPLE;
		break;
	case DJP2KCodecParameter::interleaveLine:
		jls_params.ilv = ILV_LINE;
		break;
	case DJP2KCodecParameter::interleaveNone:
		jls_params.ilv = ILV_NONE;
		break;
	case DJP2KCodecParameter::interleaveDefault:
	default:
		// Default for the cooked encoder is always ILV_LINE
		jls_params.ilv = ILV_LINE;
		break;
	}

	// Special case: one component images are always ILV_NONE (Standard requires this)
	if (jls_params.components == 1)
	{
		jls_params.ilv = ILV_NONE;
	}

	Uint8 *frameBuffer = NULL;
	Uint8 *framePointer = buffer;
	// Do we have to convert the image to color-by-plane now?
	if (jls_params.ilv == ILV_NONE && jls_params.components != 1)
	{
		DCMJP2K_DEBUG("Converting image from color-by-pixel to color-by-plane");

		frameBuffer = new Uint8[buffer_size];
		framePointer = frameBuffer;
		result = convertToUninterleaved(frameBuffer, buffer, samplesPerPixel, width, height, jls_params.bitspersample);
	}

	// We have no idea how big the compressed pixel data will be and we have no
	// way to find out, so we just allocate a buffer large enough for the raw data
	// plus a little more for JPEG metadata.
	// Yes, this is way too much for just a little JPEG metadata, but some
	// test-images showed that the buffer previously was too small. Plus, at some
	// places charls fails to do proper bounds checking and writes behind the end
	// of the buffer (sometimes way behind its end...).
	size_t compressed_buffer_size = buffer_size + 1024;
	Uint8 *compressed_buffer = new Uint8[compressed_buffer_size];

	JLS_ERROR err = JpegLsEncode(compressed_buffer, compressed_buffer_size,
		&compressed_buffer_size, framePointer, buffer_size, &jls_params);
	result = DJ2KError::convert(err);

	if (result.good())
	{
		// 'compressed_buffer_size' now contains the size of the compressed data in buffer
		compressedSize = compressed_buffer_size;
		result = pixelSequence->storeCompressedFrame(offsetList, compressed_buffer, compressed_buffer_size, fragmentSize);
	}

	delete[] buffer;
	delete[] compressed_buffer;
	if (frameBuffer)
		delete[] frameBuffer;
*/

	opj_cparameters_t l_param;
	opj_codec_t * l_codec;
	opj_image_t * l_image;
	opj_image_cmptparm_t l_params [NUM_COMPS_MAX];
	opj_stream_t * l_stream;
	OPJ_UINT32 l_nb_tiles;
	OPJ_UINT32 l_data_size;
	unsigned char len;

#ifdef USING_MCT
	const OPJ_FLOAT32 l_mct [] =
	{
		1 , 0 , 0 ,
		0 , 1 , 0 ,
		0 , 0 , 1
	};

	const OPJ_INT32 l_offsets [] =
	{
		128 , 128 , 128
	};
#endif

	opj_image_cmptparm_t * l_current_param_ptr;
	OPJ_UINT32 i;
	OPJ_BYTE *l_data;

	OPJ_UINT32 num_comps;
	int image_width;
	int image_height;
	int tile_width;
	int tile_height;
	int comp_prec;
	int irreversible;
	char output_file[64];

	/* should be test_tile_encoder 3 2000 2000 1000 1000 8 tte1.j2k */
#ifdef	NEVER
	if( argc == 9 )
	{
	num_comps = (OPJ_UINT32)atoi( argv[1] );
	image_width = atoi( argv[2] );
	image_height = atoi( argv[3] );
	tile_width = atoi( argv[4] );
	tile_height = atoi( argv[5] );
	comp_prec = atoi( argv[6] );
	irreversible = atoi( argv[7] );
	strcpy(output_file, argv[8] );
	}
	else
	{
	num_comps = 3;
	image_width = 2000;
	image_height = 2000;
	tile_width = 1000;
	tile_height = 1000;
	comp_prec = 8;
	irreversible = 1;
	strcpy(output_file, "test.j2k" );
	}
	if( num_comps > NUM_COMPS_MAX )
	{
	return 1;
	}
	l_nb_tiles = (OPJ_UINT32)(image_width/tile_width) * (OPJ_UINT32)(image_height/tile_height);
	l_data_size = (OPJ_UINT32)tile_width * (OPJ_UINT32)tile_height * (OPJ_UINT32)num_comps * (OPJ_UINT32)(comp_prec/8);

	l_data = (OPJ_BYTE*) malloc(l_data_size * sizeof(OPJ_BYTE));

	fprintf(stdout, "Encoding random values -> keep in mind that this is very hard to compress\n");
	for (i=0;i<l_data_size;++i)	{
		l_data[i] = (OPJ_BYTE)i; /*rand();*/
	}

	opj_set_default_encoder_parameters(&l_param);
	/** you may here add custom encoding parameters */
	/* rate specifications */
	/** number of quality layers in the stream */
	l_param.tcp_numlayers = 1;
	l_param.cp_fixed_quality = 1;
	l_param.tcp_distoratio[0] = 20;
	/* is using others way of calculation */
	/* l_param.cp_disto_alloc = 1 or l_param.cp_fixed_alloc = 1 */
	/* l_param.tcp_rates[0] = ... */


	/* tile definitions parameters */
	/* position of the tile grid aligned with the image */
	l_param.cp_tx0 = 0;
	l_param.cp_ty0 = 0;
	/* tile size, we are using tile based encoding */
	l_param.tile_size_on = OPJ_TRUE;
	l_param.cp_tdx = tile_width;
	l_param.cp_tdy = tile_height;

	/* use irreversible encoding ?*/
	l_param.irreversible = irreversible;

	/* do not bother with mct, the rsiz is set when calling opj_set_MCT*/
	/*l_param.cp_rsiz = OPJ_STD_RSIZ;*/

	/* no cinema */
	/*l_param.cp_cinema = 0;*/

	/* no not bother using SOP or EPH markers, do not use custom size precinct */
	/* number of precincts to specify */
	/* l_param.csty = 0;*/
	/* l_param.res_spec = ... */
	/* l_param.prch_init[i] = .. */
	/* l_param.prcw_init[i] = .. */


	/* do not use progression order changes */
	/*l_param.numpocs = 0;*/
	/* l_param.POC[i].... */

	/* do not restrain the size for a component.*/
	/* l_param.max_comp_size = 0; */

	/** block encoding style for each component, do not use at the moment */
	/** J2K_CCP_CBLKSTY_TERMALL, J2K_CCP_CBLKSTY_LAZY, J2K_CCP_CBLKSTY_VSC, J2K_CCP_CBLKSTY_SEGSYM, J2K_CCP_CBLKSTY_RESET */
	/* l_param.mode = 0;*/

	/** number of resolutions */
	l_param.numresolution = 6;

	/** progression order to use*/
	/** OPJ_LRCP, OPJ_RLCP, OPJ_RPCL, PCRL, CPRL */
	l_param.prog_order = OPJ_LRCP;

	/** no "region" of interest, more precisally component */
	/* l_param.roi_compno = -1; */
	/* l_param.roi_shift = 0; */

	/* we are not using multiple tile parts for a tile. */
	/* l_param.tp_on = 0; */
	/* l_param.tp_flag = 0; */

	/* if we are using mct */
#ifdef USING_MCT
	opj_set_MCT(&l_param,l_mct,l_offsets,NUM_COMPS);
#endif


	/* image definition */
	l_current_param_ptr = l_params;
	for (i=0;i<num_comps;++i) {
		/* do not bother bpp useless */
		/*l_current_param_ptr->bpp = COMP_PREC;*/
		l_current_param_ptr->dx = 1;
		l_current_param_ptr->dy = 1;

		l_current_param_ptr->h = (OPJ_UINT32)image_height;
		l_current_param_ptr->w = (OPJ_UINT32)image_width;

		l_current_param_ptr->sgnd = 0;
		l_current_param_ptr->prec = (OPJ_UINT32)comp_prec;

		l_current_param_ptr->x0 = 0;
		l_current_param_ptr->y0 = 0;

		++l_current_param_ptr;
	}

	/* should we do j2k or jp2 ?*/
	len = (unsigned char)strlen( output_file );
	if( strcmp( output_file + len - 4, ".jp2" ) == 0 )
	{
	l_codec = opj_create_compress(OPJ_CODEC_JP2);
	}
	else
	{
	l_codec = opj_create_compress(OPJ_CODEC_J2K);
	}
	if (!l_codec) {
		return 1;
	}

	/* catch events using our callbacks and give a local context */
	opj_set_info_handler(l_codec, info_callback,00);
	opj_set_warning_handler(l_codec, warning_callback,00);
	opj_set_error_handler(l_codec, error_callback,00);

	l_image = opj_image_tile_create(num_comps,l_params,OPJ_CLRSPC_SRGB);
	if (! l_image) {
		opj_destroy_codec(l_codec);
		return 1;
	}

	l_image->x0 = 0;
	l_image->y0 = 0;
	l_image->x1 = (OPJ_UINT32)image_width;
	l_image->y1 = (OPJ_UINT32)image_height;
	l_image->color_space = OPJ_CLRSPC_SRGB;

	if (! opj_setup_encoder(l_codec,&l_param,l_image)) {
		fprintf(stderr, "ERROR -> test_tile_encoder: failed to setup the codec!\n");
		opj_destroy_codec(l_codec);
		opj_image_destroy(l_image);
		return 1;
	}

	l_stream = opj_stream_create_default_file_stream(output_file, OPJ_FALSE);
	if (! l_stream) {
		fprintf(stderr, "ERROR -> test_tile_encoder: failed to create the stream from the output file %s !\n",output_file );
		opj_destroy_codec(l_codec);
		opj_image_destroy(l_image);
		return 1;
	}

	if (! opj_start_compress(l_codec,l_image,l_stream)) {
		fprintf(stderr, "ERROR -> test_tile_encoder: failed to start compress!\n");
		opj_stream_destroy(l_stream);
		opj_destroy_codec(l_codec);
		opj_image_destroy(l_image);
		return 1;
	}

	for (i=0;i<l_nb_tiles;++i) {
		if (! opj_write_tile(l_codec,i,l_data,l_data_size,l_stream)) {
			fprintf(stderr, "ERROR -> test_tile_encoder: failed to write the tile %d!\n",i);
			opj_stream_destroy(l_stream);
			opj_destroy_codec(l_codec);
			opj_image_destroy(l_image);
			return 1;
		}
	}

	if (! opj_end_compress(l_codec,l_stream)) {
		fprintf(stderr, "ERROR -> test_tile_encoder: failed to end compress!\n");
		opj_stream_destroy(l_stream);
		opj_destroy_codec(l_codec);
		opj_image_destroy(l_image);
		return 1;
	}

	opj_stream_destroy(l_stream);
	opj_destroy_codec(l_codec);
	opj_image_destroy(l_image);

	free(l_data);

	/* Print profiling*/
	/*PROFPRINT();*/

	return 0;
#endif
	return result;
}
#endif

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

	derivationDescription = "near lossless JPEG 2000 compression, factor ";
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
