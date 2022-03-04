#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(BROWSERLIB_LIB)
#  define BROWSERLIB_EXPORT Q_DECL_EXPORT
# else
#  define BROWSERLIB_EXPORT Q_DECL_IMPORT
# endif
#else
# define BROWSERLIB_EXPORT
#endif
