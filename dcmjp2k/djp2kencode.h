/*
 *
 *  Copyright (C) 1997-2011, OFFIS e.V.
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
 *  Purpose: singleton class that registers encoders for all supported JPEG 2000 processes.
 *
 */

#ifndef DCMJ2K_DJ2KENCODE_H
#define DCMJ2K_DJ2KENCODE_H

#include "dcmtk/config/osconfig.h"
#include "dcmtk/ofstd/oftypes.h"  /* for OFBool */
#include "djp2kutil.h"
#include "dcmtk/dcmdata/dctypes.h"  /* for Uint32 */
#include "djp2kcparam.h" /* for class DJLSCodecParameter */

namespace jp2k {

class CodecParameter;
class LosslessEncoder;
class LossyEncoder;

/** singleton class that registers encoders for all supported JPEG 2000 processes.
 */
class DCMJP2K_EXPORT EncoderRegistration
{
public:

  /** registers encoders for all supported JPEG 2000 processes.
   *  If already registered, call is ignored unless cleanup() has
   *  been performed before.
   *  @param jp2k_optionsEnabled       enable/disable use of all five JPEG 2000 parameters
   *  @param fragmentSize              maximum fragment size (in kbytes) for compression, 0 for unlimited.
   *  @param createOffsetTable         create offset table during image compression
   *  @param uidCreation               mode for SOP Instance UID creation
   *  @param convertToSC               flag indicating whether image should be converted to Secondary Capture upon compression
   */
  static void registerCodecs(
	OFBool jp2k_optionsEnabled = OFTrue,
	Uint32 fragmentSize = 0,
	OFBool createOffsetTable = OFTrue,
	UIDCreation uidCreation = EJ2KUC_default,
	OFBool convertToSC = OFFalse);

  /** deregisters encoders.
   *  Attention: Must not be called while other threads might still use
   *  the registered codecs, e.g. because they are currently encoding
   *  DICOM data sets through dcmdata.
   */
  static void cleanup();

  /** get version information of the CharLS library.
   *  Typical output format: "CharLS, Revision 55020 (modified)"
   *  @return name and version number of the CharLS library
   */
  static OFString getLibraryVersionString();

private:

  /// flag indicating whether the encoders are already registered.
  static OFBool registered_;

  /// pointer to codec parameter shared by all encoders
  static CodecParameter *cp_;

  /// pointer to encoder for lossless JPEG 2000
  static LosslessEncoder  *losslessencoder_;

  /// pointer to encoder for lossy JPEG 2000
  static LossyEncoder *lossyencoder_;

};

}

#endif
