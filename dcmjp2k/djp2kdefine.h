/*
 *
 *  Copyright (C) 2011, OFFIS e.V. - IRM Inc.
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
 *  Purpose: Contains preprocessor definitions
 *
 */

#ifndef D2KDEFINE_H
#define D2KDEFINE_H

#include "dcmtk/config/osconfig.h"

#include "dcmtk/ofstd/ofdefine.h"

#ifdef DCMJP2K_EXPORTS
#define DCMJP2K_EXPORT DCMTK_DECL_EXPORT
#else
#define DCMJP2K_EXPORT DCMTK_DECL_IMPORT
#endif

#endif
