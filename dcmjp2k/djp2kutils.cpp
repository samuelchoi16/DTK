/*
*
*  Copyright (C) 1997-2012, OFFIS e.V.
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
*  Author:  Martin Willkomm, Uli Schlachter
*
*  Purpose: Support code for dcmj2k
*
*/

#include "dcmtk/config/osconfig.h"
#include "dcmjp2k/djp2kutil.h"
#include "dcmtk/dcmdata/dcerror.h"

OFLogger DCM_dcmj2kLogger = OFLog::getLogger("dcmtk.dcmj2k");

#define MAKE_DCMJP2K_ERROR(number, name, description)  \
makeOFConditionConst(EC_ ## name, OFM_dcmjp2k, number, OF_error, description)

MAKE_DCMJP2K_ERROR(1, JP2KUncompressedBufferTooSmall, "Uncompressed pixel data too short for uncompressed image");
MAKE_DCMJP2K_ERROR(2, JP2KCompressedBufferTooSmall, "Allocated too small buffer for compressed image data");
MAKE_DCMJP2K_ERROR(3, JP2KCodecUnsupportedImageType, "Codec does not support this JPEG 2000 image");
MAKE_DCMJP2K_ERROR(4, JP2KCodecInvalidParameters, "Codec received invalid compression parameters");
MAKE_DCMJP2K_ERROR(5, JP2KCodecUnsupportedValue, "Codec received unsupported compression parameters");
MAKE_DCMJP2K_ERROR(6, JP2KInvalidCompressedData, "Invalid compressed image data");
MAKE_DCMJP2K_ERROR(7, JP2KUnsupportedBitDepthForTransform, "Codec does not support the image's color transformation with this bit depth");
MAKE_DCMJP2K_ERROR(8, JP2KUnsupportedColorTransform, "Codec does not support the image's color transformation");
MAKE_DCMJP2K_ERROR(9, JP2KUnsupportedBitDepth, "Unsupported bit depth in JPEG 2000 transfer syntax");
MAKE_DCMJP2K_ERROR(10, JP2KCannotComputeNumberOfFragments, "Cannot compute number of fragments for JPEG 2000 frame");
MAKE_DCMJP2K_ERROR(11, JP2KImageDataMismatch, "Image data mismatch between DICOM header and JPEG 2000 bitstream");
MAKE_DCMJP2K_ERROR(12, JP2KUnsupportedPhotometricInterpretation, "Unsupported photometric interpretation for near-lossless JPEG 2000 compression");
MAKE_DCMJP2K_ERROR(13, JP2KUnsupportedPixelRepresentation, "Unsupported pixel representation for near-lossless JPEG 2000 compression");
MAKE_DCMJP2K_ERROR(14, JP2KUnsupportedImageType, "Unsupported type of image for JPEG 2000 compression");
MAKE_DCMJP2K_ERROR(15, JP2KTooMuchCompressedData, "Too much compressed data, trailing data after image");
