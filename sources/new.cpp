#include <new>

void operator delete(void *) {
}

void* operator new(std::size_t size, void* ptr) noexcept {
    return ptr;
}
