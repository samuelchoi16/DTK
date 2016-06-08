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
 *  Author:  Martin Willkomm
 *
 *  Purpose: singleton class that registers the decoder for all supported JPEG 2000 processes.
 *
 */

#ifndef DCMJ2K_DJ2KDECODE_H
#define DCMJ2K_DJ2KDECODE_H

#include "dcmtk/config/osconfig.h"
#include "dcmtk/ofstd/oftypes.h"      /* for OFBool */
#include "djp2kutil.h"                 /* for enums */

class DJP2KCodecParameter;
class DJP2KLosslessDecoder;
class DJP2KLossyDecoder;

/** singleton class that registers decoders for all supported JPEG 2000 processes.
 */
class DCMJP2K_EXPORT DJP2KDecoderRegistration
{
public:

  /** registers decoder for all supported JPEG 2000 processes.
   *  If already registered, call is ignored unless cleanup() has
   *  been performed before.
   *  @param uidcreation flag indicating whether or not
   *    a new SOP Instance UID should be assigned upon decompression.
   *  @param planarconfig flag indicating how planar configuration
   *    of color images should be encoded upon decompression.
   *  @param ignoreOffsetTable flag indicating whether to ignore the offset table when decompressing multiframe images
   */
  static void registerCodecs(
    JP2K_UIDCreation uidcreation = EJ2KUC_default,
    JP2K_PlanarConfiguration planarconfig = EJ2KPC_restore,
    OFBool ignoreOffsetTable = OFFalse);

  /** deregisters decoders.
   *  Attention: Must not be called while other threads might still use
   *  the registered codecs, e.g. because they are currently decoding
   *  DICOM data sets through dcmdata.
   */
  static void cleanup();

  /** get version information of the JPEG 2000 library.
   *  Typical output format: "CharLS, Revision 55020 (modified)"
   *  @return name and version number of the CharLS library
   */
  static OFString getLibraryVersionString();

private:

  /// flag indicating whether the decoders are already registered.
  static OFBool registered_;

  /// pointer to codec parameter shared by all decoders
  static DJP2KCodecParameter *cp_;

  /// pointer to decoder for lossless JPEG 2000
  static DJP2KLosslessDecoder *losslessdecoder_;

  /// pointer to decoder for lossy JPEG 2000
  static DJP2KLossyDecoder *lossydecoder_;

};

#endif
