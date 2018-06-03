#pragma once

#ifdef APP_DEBUG

#include <stdint.h>
#include <cstddef>
#include <new>

namespace app {

    extern std::size_t alloc_counter;
    void* malloc(std::size_t size);
    void free(void* p) noexcept;

} // namespace app

extern "C++" {

inline void* operator new(std::size_t size)
{
    return app::malloc(size);
}

inline void operator delete(void* p) noexcept
{
    app::free(p);
}

inline void* operator new[](std::size_t size)
{
    return app::malloc(size);
}

inline void operator delete[](void* p)
{
    app::free(p);
}

inline void* operator new(std::size_t size, const std::nothrow_t&) noexcept
{
        return app::malloc(size);
}

inline void operator delete(void* p, const std::nothrow_t&) noexcept
{
    app::free(p);
}

inline void* operator new[](std::size_t size, const std::nothrow_t&) noexcept
{
        return app::malloc(size);
}

inline void operator delete[](void* p, const std::nothrow_t&) noexcept
{
    app::free(p);
}

inline void operator delete(void* p, long unsigned int)
{
    app::free(p);
}

inline void operator delete [](void* p, long unsigned int)
{
    app::free(p);
}

} // extern "C++"

#endif
