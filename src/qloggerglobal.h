#ifndef QLOGGER_GLOBAL_H
#define QLOGGER_GLOBAL_H

#include "config.h"
#include <QtGlobal>

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
 *********************************/
#define QLOGGER_DEPREC              [[deprecated]]          /**< Use to mark a method as deprecated. \n\code{.cpp}QLOGGER_DEPREC void myOldFct(); \endcode */
#define QLOGGER_DEPREC_X(reason)    [[deprecated(reason)]] 	/**< Use to mark a method as deprecated and specify a reason. \n\code{.cpp}QLOGGER_DEPREC_X("Use myNewFct() instead") void myOldFunc(); \endcode */

/**********************************
 * Custom macros used to detect custom
 * built-in functions
 * Sources:
 * - MSVC: No equivalent
 * - GCC: https://gcc.gnu.org/onlinedocs/gcc-13.2.0/cpp/_005f_005fhas_005fbuiltin.html
 * - Clang: https://clang.llvm.org/docs/LanguageExtensions.html#has-builtin
 *********************************/
#if defined(__GNUC__) || defined(__clang__)
#define QLOGGER_BUILTIN(x)  __has_builtin(x)
#else
#define QLOGGER_BUILTIN(x)  0
#endif

/**********************************
 * Custom macros in order to
 * not trigger warning on expected
 * behaviour
 *********************************/
#define QLOGGER_FALLTHROUGH  [[fallthrough]]    /**< Indicates that the fall through from the previous case label is intentional and should not be diagnosed by a compiler that warns on fallthrough */

/**********************************
 * Context informations
 *********************************/
#define QLOGGER_FILE            __FILE__
#define QLOGGER_LINE            __LINE__
#define QLOGGER_FCTNAME         __func__

#define QLOGGER_FCTSIG          Q_FUNC_INFO

/**********************************
 * Classes behaviours
 *********************************/
#define QLOGGER_DISABLE_COPY(Class) \
    Q_DISABLE_COPY(Class)

#define QLOGGER_DISABLE_MOVE(Class) \
    Q_DISABLE_MOVE(Class)

#define QLOGGER_DISABLE_COPY_MOVE(Class) \
    Q_DISABLE_COPY_MOVE(Class)

#endif // QLOGGER_GLOBAL_H
