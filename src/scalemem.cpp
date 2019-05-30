#include <iostream>

#include "tbb/task_scheduler_init.h"
#include "tbb/blocked_range.h"
#include "tbb/parallel_for.h"
#include "tbb/scalable_allocator.h"

// No retry loop because we assume that scalable_malloc does
// all it takes to allocate the memory, so calling it repeatedly
// will not improve the situation at all
//
// No use of std::new_handler because it cannot be done in portable
// and thread-safe way (see sidebar)
//
// We throw std::bad_alloc( ) when scalable_malloc returns NULL
//(we return NULL if it is a no-throw implementation)
void* operator new (size_t size)
{
    //std::cout << "CUSTOM NEW\n";
    if (size == 0) size = 1;
    if (void* ptr = scalable_malloc (size))
        return ptr;
    throw std::bad_alloc ( );
}

void* operator new[] (size_t size)
{   
    //std::cout << "CUSTOM NEW[]\n";
    return operator new (size);
}

void* operator new (size_t size, const std::nothrow_t&)
{
    //std::cout << "CUSTOM NEW2\n";
    if (size == 0) size = 1;
    if (void* ptr = scalable_malloc (size))
    return ptr;
    return NULL;
}

void* operator new[] (size_t size, const std::nothrow_t&) 
{
    std::cout << "CUSTOM NEW2[]\n";
    return operator new (size, std::nothrow);
}

void operator delete (void* ptr) 
{
    if (ptr != 0) scalable_free (ptr);
}

void operator delete[] (void* ptr)
{
    operator delete (ptr);
}

void operator delete (void* ptr, const std::nothrow_t&)
{
    if (ptr != 0) scalable_free (ptr);
}

void operator delete[] (void* ptr, const std::nothrow_t&)
{
    operator delete (ptr, std::nothrow);
}