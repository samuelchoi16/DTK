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
 *  Author:  Uli Schlachter
 *
 *  Purpose: Helper function than converts between CharLS and dcmjpgls errors
 *
 */

#ifndef DJ2KERROR_H
#define DJ2KERROR_H

#include "dcmtk/config/osconfig.h"
#include "djp2kutil.h" /* For the OFCondition codes */

/** Helper class for converting between dcmj2k and CharLS error codes
 */
class DJP2KError
{
private:
  /// private undefined constructor
  DJP2KError();

public:

  /** This method converts a CharLS error code into a dcmj2k OFCondition
   *  @param error The CharLS error code
   *  @return The OFCondition
   */
  /*
  static const OFConditionConst& convert(JLS_ERROR error)
  {
    switch (error)
    {
      case OK:
        return EC_Normal;
      case UncompressedBufferTooSmall:
        return EC_J2KUncompressedBufferTooSmall;
      case CompressedBufferTooSmall:
        return EC_J2KCompressedBufferTooSmall;
      case ImageTypeNotSupported:
        return EC_J2KCodecUnsupportedImageType;
      case InvalidJlsParameters:
        return EC_J2KCodecInvalidParameters;
      case ParameterValueNotSupported:
        return EC_J2KCodecUnsupportedValue;
      case InvalidCompressedData:
        return EC_J2KInvalidCompressedData;
      case UnsupportedBitDepthForTransform:
        return EC_J2KUnsupportedBitDepthForTransform;
      case UnsupportedColorTransform:
        return EC_J2KUnsupportedColorTransform;
      case TooMuchCompressedData:
        return EC_J2KTooMuchCompressedData;
      default:
        return EC_IllegalParameter;
    }
  }
  */
};

#endif
