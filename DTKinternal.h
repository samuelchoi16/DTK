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

#include "zlib.h"							/* for zlibVersion() */

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define	DTK_DEFAULT_UID_ROOT				("1.2.410.200055")

extern const char* _ISO_IR[];
extern const char* _ISO_2022[];
extern const char* _ISO_2022_ESC[];

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

using namespace dcm;

#ifdef  WIN32
#define DTK_MAX(a, b)                       max(a, b)
#else
#define DTK_MAX(a, b)                       std::max(a, b)
#endif

#endif // DTKINTERNAL_H
