/*
 *
 *  Copyright (C) 2015, IRM Inc.
 *  All rights reserved.  See LICENSE file for details.
 *
 *  This software and supporting documentation were developed by
 *		IRM Inc., Korea.
 *  through the contract with
 *		Seoul National University Bundang Hospital
 *  under the support of
 *		Ministry of Trade, Industry and Energy, Republic of Korea.
 *
 *  Author:  Samuel Choi (samuelchoi@irm.kr)
 *
 */

#ifndef DTKINTERNAL_H
#define DTKINTERNAL_H

#include "dcmtk/dcmdata/dcrleerg.h"
#include "dcmtk/dcmdata/dcrledrg.h"
#include "dcmtk/dcmjpeg/djencode.h"
#include "dcmtk/dcmjpeg/djdecode.h"
#include "dcmtk/dcmjpls/djencode.h"
#include "dcmtk/dcmjpls/djdecode.h"

#include "dcmtk/dcmimgle/dcmimage.h"		/* for DicomImage */
#include "dcmtk/dcmimgle/digsdfn.h"			/* for DiGSDFunction */
#include "dcmtk/dcmimgle/diciefn.h"			/* for DiCIELABFunction */
#include "dcmtk/dcmimage/diregist.h"		/* include to support color images */
#include "dcmtk/dcmimage/dipitiff.h"		/* for dcmimage TIFF plugin */
#include "dcmtk/dcmimage/dipipng.h"			/* for dcmimage PNG plugin */

#include "dcmtk/dcmjpeg/dipijpeg.h"			/* for dcmimage JPEG plugin */
#include "dcmtk/dcmjpeg/djrplol.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define	DTK_DEFAULT_UID_ROOT				("1.2.410.200055")

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

using namespace dcm;

#ifdef  WIN32
#define DTK_MAX(a, b)                       std::max(a, b)
#define	DTK_SLEEP(n)						Sleep((n))
#else
#define DTK_MAX(a, b)                       std::max(a, b)
#define	DTK_SLEEP(n)						sleep((n))
#endif

#endif // DTKINTERNAL_H
