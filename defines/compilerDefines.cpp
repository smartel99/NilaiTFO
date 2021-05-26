/**
 * @addtogroup compilerDefines.cpp
 * @{
 *******************************************************************************
 * @file	compilerDefines.cpp
 * @author	Samuel Martel
 * @brief   This file's content is really meant to reduce program size.
 * Created on: May 12, 2021
 *******************************************************************************
 */

#include "../processes/application.hpp"

#include "defines/macros.hpp"

#include <new>
#include <malloc.h>

void* operator new(std::size_t size)
{
    void* ptr = malloc(size);
    CEP_ASSERT(ptr != nullptr, "Failed to allocate memory!");
    return ptr;
}

void* operator new[](std::size_t size)
{
    void* ptr = malloc(size);
    CEP_ASSERT(ptr != nullptr, "Failed to allocate memory!");
    return ptr;
}

void operator delete(void* ptr)
{
    free(ptr);
}

void operator delete[](void* ptr)
{
    free(ptr);
}

void operator delete(void* ptr, size_t)
{
    free(ptr);
}

void operator delete[](void* ptr, size_t)
{
    free(ptr);
}


/**
 * @brief   This function is called whenever a pure virtual function
 *          is somehow managed to be called through extremely heretical
 *          practices.
 * 
 */
extern "C" void __cxa_pure_virtual()
{
    cep::Application::AssertFailed();
}
