#ifndef PROTOCOLS_GLOBAL_H
#define PROTOCOLS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PROTOCOLS_LIBRARY)
#  define PROTOCOLSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define PROTOCOLSSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // PROTOCOLS_GLOBAL_H
