/* 
 * @addtogroup  defines
 * @{
 * @addtogroup  compilerDefines
 * @{
 * 
 * @file    compilerDefines.h
 * @author  Pascal-Emmanuel Lachance
 * @p       https://github.com/Raesangur/
 * @date    2019/08/06  -  16:39
 * 
 * @brief   Collection of defines to allow cross-compiler compatilibity.
 */
#pragma once

/*****************************************************************************/
/* Defines ----------------------------------------------------------------- */
#if defined(__XC16__)
    #define _PACKED_        __attribute__((__packed__))
    #define _INTERRUPT_     __attribute__ ((interrupt, no_auto_psv))
    #define _NOINLINE_      __attribute__((noinline))
    #define ALWAYS_INLINE   __attribute__((always_inline))
    #define _xmemory_       space(xmemory)
    #define _EDS_           __eds__
    #define _ASM_           __asm__ volatile
#elif defined (__GCC__) || defined(__clang__)
    #define _PACKED_        __attribute__((packed))
    #define _INTERRUPT_     __attribute__((interrupt))
    #define _NOINLINE_      __attribute__((noinline))
    #define ALWAYS_INLINE   __attribute__((always_inline))
    #define _xmemory_       
    #define _EDS_           
    #define _ASM_           __asm__ volatile
#elif defined (__MSVC__)
    #define _PACKED_  
    #define _INTERRUPT_ 
    #define _NOINLINE_      declspec(noinline)
    #define ALWAYS_INLINE   __forceinline
    #define _xmemory_
    #define _EDS_
    #define _ASM_
    #define ALLOW_ASM   0               /* Disable inline assembly */
#else
    #define _PACKED_
    #define _NOINLINE_
    #define _INTERRUPT_
    #define ALWAYS_INLINE
    #define _xmemory_
    #define _EDS_
    #define _ASM_
#endif


/*
 * This is a very rough fix to a bug cause by ClangIntellisense not recognizing the `-std=c++17`
 * compiler flag passed to it by VisualGDB.
 * This causes the `__cplusplus` macro to be defined to another standard, not allowing the Clang
 * Intellisense to recognize features of the C++17 standard, such as `std::string_view`.
 * This fix is **not** to be kept if the toolchain changes from gcc to llvm, as it could have
 * catastrophic effects.
 * Currently, since we are not compiling with clang, only using tools from llvm, this does not have
 * effects on the code itself.
 * - Pascal 2020/07/02
 */
#ifdef __clang__
#    ifdef __cplusplus
#        undef __cplusplus
#    endif
#    define __cplusplus 201703UL
#endif


/*************************************************************************************************/
/**
 * @}
 * @}
 */
/* ----- END OF FILE ----- */
