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
 *  Author:  Martin Willkomm, Uli Schlachter
 *
 *  Purpose: codec classes for JPEG 2000 encoders.
 *
 */

#ifndef DCMJ2K_DJ2KCODEC_H
#define DCMJ2K_DJ2KCODEC_H

#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmdata/dccodec.h"  /* for class DcmCodec */
#include "dcmtk/dcmdata/dcofsetl.h" /* for struct DcmOffsetList */
#include "dcmtk/ofstd/ofstring.h"   /* for class OFString */
#include "djp2kdefine.h"
#include "djp2kcodecb.h"

namespace jp2k {

class RepresentationParameter;
class CodecParameter;
class DicomImage;

/** abstract codec class for JPEG 2000 encoders.
 *  This abstract class contains most of the application logic
 *  needed for a dcmdata codec object that implements a JPEG 2000 encoder
 *  This class only supports compression, it neither implements
 *  decoding nor transcoding.
 */
class DCMJP2K_EXPORT EncoderBase : public CodecBase
{
public:

  /// default constructor
  EncoderBase();

  /// destructor
  virtual ~EncoderBase();

  /** decompresses the given pixel sequence and
   *  stores the result in the given uncompressedPixelData element.
   *  @param fromRepParam current representation parameter of compressed data, may be NULL
   *  @param pixSeq compressed pixel sequence
   *  @param uncompressedPixelData uncompressed pixel data stored in this element
   *  @param cp codec parameters for this codec
   *  @param objStack stack pointing to the location of the pixel data
   *    element in the current dataset.
   *  @return EC_Normal if successful, an error code otherwise.
   */
  virtual OFCondition decode(
    const DcmRepresentationParameter * fromRepParam,
    DcmPixelSequence * pixSeq,
    DcmPolymorphOBOW& uncompressedPixelData,
    const DcmCodecParameter * cp,
    const DcmStack& objStack) const;

  /** decompresses a single frame from the given pixel sequence and
   *  stores the result in the given buffer.
   *  @param fromParam representation parameter of current compressed
   *    representation, may be NULL.
   *  @param fromPixSeq compressed pixel sequence
   *  @param cp codec parameters for this codec
   *  @param dataset pointer to dataset in which pixel data element is contained
   *  @param frameNo number of frame, starting with 0 for the first frame
   *  @param startFragment index of the compressed fragment that contains
   *    all or the first part of the compressed bitstream for the given frameNo.
   *    Upon successful return this parameter is updated to contain the index
   *    of the first compressed fragment of the next frame.
   *    When unknown, zero should be passed. In this case the decompression
   *    algorithm will try to determine the index by itself, which will always
   *    work if frames are decompressed in increasing order from first to last,
   *    but may fail if frames are decompressed in random order, multiple fragments
   *    per frame and multiple frames are present in the dataset, and the offset
   *    table is empty.
   *  @param buffer pointer to buffer where frame is to be stored
   *  @param bufSize size of buffer in bytes
   *  @param decompressedColorModel upon successful return, the color model
   *    of the decompressed image (which may be different from the one used
   *    in the compressed images) is returned in this parameter.
   *  @return EC_Normal if successful, an error code otherwise.
   */
  virtual OFCondition decodeFrame(
    const DcmRepresentationParameter * fromParam,
    DcmPixelSequence * fromPixSeq,
    const DcmCodecParameter * cp,
    DcmItem *dataset,
    Uint32 frameNo,
    Uint32& startFragment,
    void *buffer,
    Uint32 bufSize,
    OFString& decompressedColorModel) const;

  /** compresses the given uncompressed DICOM image and stores
   *  the result in the given pixSeq element.
   *  @param pixelData pointer to the uncompressed image data in OW format
   *    and local byte order
   *  @param length of the pixel data field in bytes
   *  @param toRepParam representation parameter describing the desired
   *    compressed representation (e.g. JPEG quality)
   *  @param pixSeq compressed pixel sequence (pointer to new DcmPixelSequence object
   *    allocated on heap) returned in this parameter upon success.
   *  @param cp codec parameters for this codec
   *  @param objStack stack pointing to the location of the pixel data
   *    element in the current dataset.
   *  @return EC_Normal if successful, an error code otherwise.
   */
  virtual OFCondition encode(
    const Uint16 * pixelData,
    const Uint32 length,
    const DcmRepresentationParameter * toRepParam,
    DcmPixelSequence * & pixSeq,
    const DcmCodecParameter *cp,
    DcmStack & objStack) const;

  /** transcodes (re-compresses) the given compressed DICOM image and stores
   *  the result in the given toPixSeq element.
   *  @param fromRepType current transfer syntax of the compressed image
   *  @param fromRepParam current representation parameter of compressed data, may be NULL
   *  @param fromPixSeq compressed pixel sequence
   *  @param toRepParam representation parameter describing the desired
   *    new compressed representation (e.g. JPEG quality)
   *  @param toPixSeq compressed pixel sequence (pointer to new DcmPixelSequence object
   *    allocated on heap) returned in this parameter upon success.
   *  @param cp codec parameters for this codec
   *  @param objStack stack pointing to the location of the pixel data
   *    element in the current dataset.
   *  @return EC_Normal if successful, an error code otherwise.
   */
  virtual OFCondition encode(
    const E_TransferSyntax fromRepType,
    const DcmRepresentationParameter * fromRepParam,
    DcmPixelSequence * fromPixSeq,
    const DcmRepresentationParameter * toRepParam,
    DcmPixelSequence * & toPixSeq,
    const DcmCodecParameter * cp,
    DcmStack & objStack) const;

  /** checks if this codec is able to convert from the
   *  given current transfer syntax to the given new
   *  transfer syntax
   *  @param oldRepType current transfer syntax
   *  @param newRepType desired new transfer syntax
   *  @return true if transformation is supported by this codec, false otherwise.
   */
  virtual OFBool canChangeCoding(
    const E_TransferSyntax oldRepType,
    const E_TransferSyntax newRepType) const;

  /** determine color model of the decompressed image
   *  @param fromParam representation parameter of current compressed
   *    representation, may be NULL
   *  @param fromPixSeq compressed pixel sequence
   *  @param cp codec parameters for this codec
   *  @param dataset pointer to dataset in which pixel data element is contained
   *  @param dataset pointer to DICOM dataset in which this pixel data object
   *    is located. Used to access photometric interpretation.
   *  @param decompressedColorModel upon successful return, the color model
   *    of the decompressed image (which may be different from the one used
   *    in the compressed images) is returned in this parameter
   *  @return EC_Normal if successful, an error code otherwise
   */
  virtual OFCondition determineDecompressedColorModel(
    const DcmRepresentationParameter *fromParam,
    DcmPixelSequence *fromPixSeq,
    const DcmCodecParameter *cp,
    DcmItem *dataset,
    OFString &decompressedColorModel) const;

private:

  /** returns the transfer syntax that this particular codec
   *  is able to encode
   *  @return supported transfer syntax
   */
  virtual E_TransferSyntax supportedTransferSyntax() const = 0;

  /** encoder that compresses the complete pixel cell
   *  (very much like the RLE encoder in module dcmdata).
   *  @param pixelData pointer to the uncompressed image data in OW format
   *    and local byte order
   *  @param length of the pixel data field in bytes
   *  @param dataset pointer to dataset containing image pixel module
   *  @param djrp representation parameter
   *  @param pixSeq pixel sequence to write to
   *  @param djcp codec parameter
   *  @param compressionRatio compression ratio returned upon success
   *  @return EC_Normal if successful, an error code otherwise.
   */
  OFCondition encode(
    const Uint16 *pixelData,
    const Uint32 length,
    DcmItem *dataset,
	const RepresentationParameter *djrp,
    DcmPixelSequence * & pixSeq,
    const CodecParameter *djcp,
    double& compressionRatio) const;

  /**
   * @brief encodeFrame
   * @param framePointer pointer to start of frame
   * @param bitsAllocated number of bits allocated per pixel
   * @param bitsStored
   * @param columns frame width
   * @param rows frame height
   * @param samplesPerPixel image samples per pixel
   * @param pixelRepresentation
   * @param planarConfiguration image planar configuration
   * @param photometricInterpretation photometric interpretation of the DICOM dataset
   * @param djrp
   * @param pixelSequence object in which the compressed frame is stored
   * @param offsetList list of frame offsets updated in this parameter
   * @param compressedSize size of compressed frame returned in this parameter
   * @param djcp parameters for the codec
   * @return EC_Normal if successful, an error code otherwise
   */
  OFCondition encodeFrame(const Uint8 *framePointer,
	Uint16 bitsAllocated, Uint16 bitsStored,
	Uint16 columns,
	Uint16 rows,
	Uint16 samplesPerPixel,
	Uint16 pixelRepresentation,
	Uint16 planarConfiguration,
	const OFString& photometricInterpretation,
	const RepresentationParameter* djrp,
	DcmPixelSequence *pixelSequence,
	DcmOffsetList &offsetList,
	unsigned long &compressedSize,
	const CodecParameter *djcp) const;

  /** create Lossy Image Compression and Lossy Image Compression Ratio.
   *  @param dataset dataset to be modified
   *  @param ratio image compression ratio > 1. This is the real effective ratio
   *    between compressed and uncompressed image,
   *    i. e. 30 means a 30:1 lossy compression.
   *  @return EC_Normal if successful, an error code otherwise
   */
  OFCondition updateLossyCompressionRatio(
	DcmItem *dataset,
	double ratio) const;

  /** create Derivation Description.
   *  @param dataset dataset to be modified
   *  @param djrp representation parameter passed to encode()
   *  @param ratio image compression ratio > 1. This is the real effective ratio
   *    between compressed and uncompressed image,
   *    i. e. 30 means a 30:1 lossy compression.
   *  @return EC_Normal if successful, an error code otherwise
   */
  OFCondition updateDerivationDescription(
	DcmItem *dataset,
	const RepresentationParameter *djrp,
	double ratio) const;
};

/** codec class for JPEG 2000 lossless only TS encoding
 */
class DCMJP2K_EXPORT LosslessEncoder : public EncoderBase
{
  /** returns the transfer syntax that this particular codec
   *  is able to encode
   *  @return supported transfer syntax
   */
  virtual E_TransferSyntax supportedTransferSyntax() const;
};

/** codec class for JPEG 2000 lossy and lossless TS encoding
 */
class DCMJP2K_EXPORT LossyEncoder : public EncoderBase
{
  /** returns the transfer syntax that this particular codec
   *  is able to encode
   *  @return supported transfer syntax
   */
  virtual E_TransferSyntax supportedTransferSyntax() const;
};

}

#endif
