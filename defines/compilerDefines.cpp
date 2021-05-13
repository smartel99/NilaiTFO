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
#include <new>
#include <malloc.h>

void* operator new(std::size_t size)
{
    return malloc(size);
}

void* operator new[](std::size_t size)
{
    return malloc(size);
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


extern "C" void __cxa_pure_virtual()
{
    cep::Application::AssertFailed();
}
