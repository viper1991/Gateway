#ifndef SHUIWEI_GLOBAL_H
#define SHUIWEI_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SHUIWEI_LIBRARY)
#  define SHUIWEISHARED_EXPORT Q_DECL_EXPORT
#else
#  define SHUIWEISHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // SHUIWEI_GLOBAL_H
