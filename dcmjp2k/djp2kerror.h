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

namespace jp2k {

/** Helper class for converting between dcmj2k and CharLS error codes
 */
class Error
{
private:
  /// private undefined constructor
  Error();

public:

};

}

#endif
