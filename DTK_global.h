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

#ifndef DTK_GLOBAL_H
#define DTK_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(DTK_LIBRARY)
#  define DTKSHARED_EXPORT Q_DECL_EXPORT
#else
#  define DTKSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // DTK_GLOBAL_H
