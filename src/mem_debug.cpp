#include <cstdlib>
#include <stdio.h>
#include <new>

namespace app {

  std::size_t alloc_counter = 0;

  void* malloc(std::size_t size)
  {
    void* p = std::malloc(size);
    ++alloc_counter;
#ifdef APP_DEBUG_PRINT
    printf("malloc: %zu %p %zu\n", alloc_counter, p, size);
#endif
    return p;
  }

  void free(void* p) noexcept
  {
      --alloc_counter;
#ifdef APP_DEBUG_PRINT
    printf("free: %zu %p\n", alloc_counter, p);
#endif
    std::free(p);
    return;
  }
} //namespace app

