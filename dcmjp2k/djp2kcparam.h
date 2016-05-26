/*
 *
 *  Copyright (C) 1997-2011, OFFIS e.V. - IRM Inc.
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
 *  Purpose: codec parameter class JPEG 2000 codecs
 *
 */

#ifndef DCMJ2K_DJ2KCPARAM_H
#define DCMJ2K_DJ2KCPARAM_H

#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmdata/dccodec.h" /* for DcmCodecParameter */
#include "djp2kutil.h" /* for enums */

/** codec parameter for JPEG 2000 codecs
 */
class DCMJP2K_EXPORT DJP2KCodecParameter : public DcmCodecParameter
{
public:

  /** Constants describing the interleave mode which the encoder will use
   */
  enum interleaveMode
  {
    /// Use same interleave mode as the DICOM header says in planarConfiguration,
    /// if possible. The cooked encoder will force interleaveLine.
    interleaveDefault,
    /// Sample-interleaved (color-by-pixel)
    interleaveSample,
    /// Line-interleaved (color-by-line)
    interleaveLine,
    /// Uninterleaved (color-by-plane)
    interleaveNone
  };

  /** constructor, for use with encoders.
   *  @param jpls_optionsEnabled       enable/disable use of all five JPEG 2000 parameters
   *  @param jpls_t1                   JPEG 2000 parameter "Threshold 1" (used for quantization)
   *  @param jpls_t2                   JPEG 2000 parameter "Threshold 2"
   *  @param jpls_t3                   JPEG 2000 parameter "Threshold 3"
   *  @param jpls_reset                JPEG 2000 parameter "RESET", i.e., value at which the counters A, B, and N are halved.
   *  @param jpls_limit                JPEG 2000 parameter "LIMIT"
   *  @param preferCookedEncoding      true if the "cooked" lossless encoder should be preferred over the "raw" one
   *  @param fragmentSize              maximum fragment size (in kbytes) for compression, 0 for unlimited.
   *  @param createOffsetTable         create offset table during image compression
   *  @param uidCreation               mode for SOP Instance UID creation
   *  @param convertToSC               flag indicating whether image should be converted to Secondary Capture upon compression
   *  @param planarConfiguration       flag describing how planar configuration of decompressed color images should be handled
   *  @param ignoreOffsetTable         flag indicating whether to ignore the offset table when decompressing multiframe images
   *  @param jplsInterleaveMode        flag describing which interleave the JPEG 2000 datastream should use
   */
   DJP2KCodecParameter(
	/*
     OFBool jpls_optionsEnabled,
     Uint16 jpls_t1 = 3, // these are the defaults for 8bpp in lossless mode
     Uint16 jpls_t2 = 7,
     Uint16 jpls_t3 = 21,
     Uint16 jpls_reset = 64,
     Uint16 jpls_limit = 0,
	 OFBool preferCookedEncoding = OFFalse,
	*/
     Uint32 fragmentSize = 0,
     OFBool createOffsetTable = OFTrue,
     JP2K_UIDCreation uidCreation = EJ2KUC_default,
     OFBool convertToSC = OFFalse,
	 JP2K_PlanarConfiguration planarConfiguration = EJ2KPC_restore,
	 OFBool ignoreOffsetTable = OFFalse//,
/*     interleaveMode jplsInterleaveMode = interleaveLine*/);

  /** constructor, for use with decoders. Initializes all encoder options to defaults.
   *  @param uidCreation               mode for SOP Instance UID creation (used both for encoding and decoding)
   *  @param planarConfiguration       flag describing how planar configuration of decompressed color images should be handled
   *  @param ignoreOffsetTable         flag indicating whether to ignore the offset table when decompressing multiframe images
   */
  DJP2KCodecParameter(
    JP2K_UIDCreation uidCreation = EJ2KUC_default,
	JP2K_PlanarConfiguration planarConfiguration = EJ2KPC_restore,
    OFBool ignoreOffsetTable = OFFalse);

  /// copy constructor
  DJP2KCodecParameter(const DJP2KCodecParameter& arg);

  /// destructor
  virtual ~DJP2KCodecParameter();

  /** this methods creates a copy of type DcmCodecParameter *
   *  it must be overweritten in every subclass.
   *  @return copy of this object
   */
  virtual DcmCodecParameter *clone() const;

  /** returns the class name as string.
   *  can be used as poor man's RTTI replacement.
   */
  virtual const char *className() const;

  /** returns secondary capture conversion flag
   *  @return secondary capture conversion flag
   */
  OFBool getConvertToSC() const
  {
    return convertToSC_;
  }

  /** returns create offset table flag
   *  @return create offset table flag
   */
  OFBool getCreateOffsetTable() const
  {
   return createOffsetTable_;
  }

  /** returns mode for SOP Instance UID creation
   *  @return mode for SOP Instance UID creation
   */
  JP2K_UIDCreation getUIDCreation() const
  {
    return uidCreation_;
  }

  /** returns mode for handling planar configuration
   *  @return mode for handling planar configuration
   */
  JP2K_PlanarConfiguration getPlanarConfiguration() const
  {
    return planarConfiguration_;
  }

  /** returns flag indicating whether or not the "cooked" lossless encoder
   *  should be preferred over the "raw" one
   *  @return raw/cooked lossless encoding flag
   */
  /*
  OFBool cookedEncodingPreferred() const
  {
    return preferCookedEncoding_;
  }
  */

  /** returns maximum fragment size (in kbytes) for compression, 0 for unlimited.
   *  @return maximum fragment size for compression
   */
  Uint32 getFragmentSize() const
  {
    return fragmentSize_;
  }

  /** returns JPEG 2000 parameter T1
   *  @return JPEG 2000 parameter T1
   */
  /*
  Uint16 getT1() const
  {
    return jpls_t1_;
  }
  */

  /** returns JPEG 2000 parameter T2
   *  @return JPEG 2000 parameter T2
   */
  /*
  Uint16 getT2() const
  {
    return jpls_t2_;
  }
  */

  /** returns JPEG 2000 parameter T3
   *  @return JPEG 2000 parameter T3
   */
  /*
  Uint16 getT3() const
  {
    return jpls_t3_;
  }
  */

  /** returns JPEG 2000 parameter RESET
   *  @return JPEG 2000 parameter RESET
   */
  /*
  Uint16 getReset() const
  {
    return jpls_reset_;
  }
  */

  /** returns JPEG 2000 parameter LIMIT
   *  @return JPEG 2000 parameter LIMIT
   */
  /*
  Uint16 getLimit() const
  {
    return jpls_t1_;
  }
  */
  
  /** returns true if JPEG 2000 parameters T1-T3, RESET and LIMIT are enabled, false otherwise
   *  @return true if JPEG 2000 parameters T1-T3, RESET and LIMIT are enabled, false otherwise
   */
  /*
  OFBool getUseCustomOptions() const
  {
    return jpls_optionsEnabled_;
  }
  */

  /** returns true if the offset table should be ignored when decompressing multiframe images
   *  @return true if the offset table should be ignored when decompressing multiframe images
   */
  OFBool ignoreOffsetTable() const
  {
    return ignoreOffsetTable_;
  }

  /** returns the interleave mode which the encoder should use
   *  @return the interleave mode which the encoder should use
   */
  /*
  interleaveMode getJplsInterleaveMode() const
  {
    return jplsInterleaveMode_;
  }
  */

private:

  /// private undefined copy assignment operator
  DJP2KCodecParameter& operator=(const DJP2KCodecParameter&);

  // ****************************************************
  // **** Parameters describing the encoding process ****
/*
  /// enable/disable use of all five JPEG 2000 parameters
  OFBool jpls_optionsEnabled_;

  /// JPEG 2000 parameter "Threshold 1" (used for quantization)
  Uint16 jpls_t1_;

  /// JPEG 2000 parameter "Threshold 2"
  Uint16 jpls_t2_;

  /// JPEG 2000 parameter "Threshold 3"
  Uint16 jpls_t3_;

  /// JPEG 2000 parameter "RESET", i.e., value at which the counters A, B, and N are halved.
  Uint16 jpls_reset_;

  /// JPEG 2000 parameter "LIMIT"
  Uint16 jpls_limit_;
*/
  /// maximum fragment size (in kbytes) for compression, 0 for unlimited.
  Uint32 fragmentSize_;

  /// create offset table during image compression
  OFBool createOffsetTable_;
/*
  /// Flag indicating if the "cooked" lossless encoder should be preferred over the "raw" one
  OFBool preferCookedEncoding_;
*/
  /// mode for SOP Instance UID creation (used both for encoding and decoding)
  JP2K_UIDCreation uidCreation_;

  /// flag indicating whether image should be converted to Secondary Capture upon compression
  OFBool convertToSC_;
/*
  /// Flag describing the interleave mode which the encoder will use
  interleaveMode jplsInterleaveMode_;
*/
  // ****************************************************
  // **** Parameters describing the decoding process ****

  /// flag describing how planar configuration of decompressed color images should be handled
  JP2K_PlanarConfiguration planarConfiguration_;

  /// flag indicating if temporary files should be kept, false if they should be deleted after use
  OFBool ignoreOffsetTable_;

};


#endif
