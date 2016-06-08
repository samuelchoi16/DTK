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
 *  Purpose: representation parameters for JPEG 2000
 *
 */

#ifndef DCMJP2K_DJP2KRPARAM_H
#define DCMJP2K_DJP2KRPARAM_H

#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmdata/dcpixel.h" /* for class DcmRepresentationParameter */
#include "djp2kdefine.h"

/** representation parameter for JPEG 2000
 */
class DCMJP2K_EXPORT DJP2KRepresentationParameter : public DcmRepresentationParameter
{
public:

  /** constructor
   *  @param nearlosslessDeviation used as parameter NEAR in JPEG 2000 nearlossless-encoding process
   *  @param losslessProcess true if lossless process is requested
   */
  DJP2KRepresentationParameter(
	Uint16 compressionRatio = 5,
	OFBool losslessProcess = OFFalse);

  /// copy constructor
  DJP2KRepresentationParameter(const DJP2KRepresentationParameter& arg);

  /// destructor
  virtual ~DJP2KRepresentationParameter();

  /** this methods creates a copy of type DcmRepresentationParameter *
   *  it must be overweritten in every subclass.
   *  @return copy of this object
   */
  virtual DcmRepresentationParameter *clone() const;

  /** returns the class name as string.
   *  can be used in operator== as poor man's RTTI replacement.
   */
  virtual const char *className() const;

  /** compares an object to another DcmRepresentationParameter.
   *  Implementation must make sure that classes are comparable.
   *  @param arg representation parameter to compare with
   *  @return true if equal, false otherwise.
   */
  virtual OFBool operator==(const DcmRepresentationParameter &arg) const;

  /** returns the desired compression ratio
   *  @return return desired compression ratio
   */
  Uint16 getCompressionRatio() const
  {
	return compressionRatio_;
  }

  /** returns true if lossless compression is desired
   *  @return true if lossless compression is desired
   */
  OFBool useLosslessProcess() const
  {
    return losslessProcess_;
  }

private:

  /** desired NEAR parameter
   *  Default is 0 (lossless compression).
   *  Ignored during lossless compression.
   */
  Uint16 compressionRatio_;

  /// true if lossless process should be used even in lossy transfer syntax
  OFBool losslessProcess_;

};

#endif
