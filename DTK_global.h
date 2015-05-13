#ifndef DTK_GLOBAL_H
#define DTK_GLOBAL_H
/*
#include <QtCore/qglobal.h>

#if defined(DTK_LIBRARY)
#  define DTKSHARED_EXPORT Q_DECL_EXPORT
#else
#  define DTKSHARED_EXPORT Q_DECL_IMPORT
#endif
*/
#if defined(DTK_LIBRARY)
#  define DTKSHARED_EXPORT __declspec(dllexport)
#else
#  define DTKSHARED_EXPORT __declspec(dllimport)
#endif

#endif // DTK_GLOBAL_H
