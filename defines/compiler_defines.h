/**
 * @file    compiler_defines.h
 * @author  Pascal-Emmanuel Lachance
 * @date    2019/08/06  -  16:39
 *
 * @brief   Collection of defines to allow cross-compiler compatibility.
 */
#pragma once

/*****************************************************************************/
/* Defines ----------------------------------------------------------------- */

#if defined(__GNUC__)
#    define DO_PRAGMA(X)         _Pragma(#    X)
#    define DISABLE_WARNING_PUSH _Pragma("GCC diagnostic push")
#    define DISABLE_WARNING_POP  _Pragma("GCC diagnostic pop")
#    define DISABLE_WARNING(warningName)                                                           \
        DISABLE_WARNING_PUSH;                                                                      \
        DO_PRAGMA(GCC diagnostic ignored warningName)
#elif defined(__clang__)
#    define DO_PRAGMA(X)         _Pragma(#    X)
#    define DISABLE_WARNING_PUSH _Pragma("clang diagnostic push")
#    define DISABLE_WARNING_POP  _Pragma("clang diagnostic pop")
#    define DISABLE_WARNING(warningName)                                                           \
        DISABLE_WARNING_PUSH                                                                       \
        DO_PRAGMA(clang diagnostic ignored #warningName)
#else
#    error Unsuported Compiler!
#endif

#define REGION(name)                                                                               \
    if constexpr (1)                                                                               \
    {
#define END_REGION }

#if defined(__XC16__)
#    define _PACKED_      __attribute__((__packed__))
#    define _INTERRUPT_   __attribute__((interrupt, no_auto_psv))
#    define _NOINLINE_    __attribute__((noinline))
#    define ALWAYS_INLINE __attribute__((always_inline))
#    define _xmemory_     space(xmemory)
#    define _EDS_         __eds__
#    define _ASM_         __asm__ volatile
#elif defined(__GNUC__) || defined(__clang__)
#    define _PACKED_      __attribute__((packed))
#    define _INTERRUPT_   __attribute__((interrupt))
#    define _NOINLINE_    __attribute__((noinline))
#    define ALWAYS_INLINE __attribute__((always_inline))
#    define _xmemory_
#    define _EDS_
#    define _ASM_ __asm__ volatile
#elif defined(__MSVC__)
#    define _PACKED_
#    define _INTERRUPT_
#    define _NOINLINE_    declspec(noinline)
#    define ALWAYS_INLINE __forceinline
#    define _xmemory_
#    define _EDS_
#    define _ASM_
#    define ALLOW_ASM 0 /* Disable inline assembly */
#else
#    error Unsuported Compiler!
#endif

// Define the function signature macro for the compiler being used.
#if defined(_MSC_VER)
// Visual Studio
#    define FUNCSIG __FUNCSIG__
#elif defined(__GNUC__)
// GCC
#    define FUNCSIG __PRETTY_FUNCTION__
#elif defined(__clang__)
// clang
#    define FUNCSIG __PRETTY_FUNCTION__
#elif defined(__MINGW32__)
// MinGW 32/MinGW-w64 32 bits
#    define FUNCSIG __PRETTY_FUNCTION__
#elif defined(__MINGW64__)
// MinGW-w64 64 bits
#    define FUNCSIG __PRETTY_FUNCTION__
#else
#    error Unsupported Compiler
#endif

/*************************************************************************************************/

/* ----- END OF FILE ----- */
