#ifndef QLOGGER_GLOBAL_H
#define QLOGGER_GLOBAL_H

#include "config.h"
#include <QtCore/qglobal.h>

/**********************************
 * Library management
 *********************************/
#if defined(QLOGGER_LIB)
#  define QLOGGER_EXPORT Q_DECL_EXPORT
#else
#  define QLOGGER_EXPORT Q_DECL_IMPORT
#endif

/**********************************
 * Deprecations warnings
 * Sources:
 * - MSVC: https://learn.microsoft.com/en-us/cpp/cpp/deprecated-cpp?redirectedfrom=MSDN&view=msvc-170
 * - GCC: https://gcc.gnu.org/onlinedocs/gcc-4.7.1/gcc/Type-Attributes.html#Type-Attributes
 * - Clang: https://clang.llvm.org/docs/LanguageExtensions.html#messages-on-deprecated-and-unavailable-attributes
 *********************************/
#if defined(__GNUC__) || defined(__clang__)
#define QLOGGER_DEPRECATED __attribute__((deprecated))
#define QLOGGER_DEPRECATED_INFOS(message) __attribute__((deprecated(message)))

#elif defined(_MSC_VER)
#define QLOGGER_DEPRECATED __declspec(deprecated)
#define QLOGGER_DEPRECATED_INFOS(message) __declspec(deprecated(message))

#else
#warning "QLOGGER_DEPRECATED is not implemented for this compiler"
#warning "QLOGGER_DEPRECATED_INFOS(message) is not implemented for this compiler"
#define QLOGGER_DEPRECATED
#define QLOGGER_DEPRECATED_INFOS
#endif

#endif // QLOGGER_GLOBAL_H
