#include <iostream>
#include <unistd.h>
#include <cstdint>


int main()
{
    // sbrk is a system call to ask the os for more memory for our heap
    // it increases the heap size by the specified amount of memory
    // and returns a pointer to the previous program break
    // a program break is the last address space in the heap
    // this means that since sbrk returns the program break to the previous heap space (technically, a pointer to the new address space)
    // doing the integer arithmetic that we do down below would only be allowed after we acquire another chunk of memory

    printf("location of prg. code: %p\n", main);
    void* ptr1 = malloc(50);
    printf("location of heap: %p\n", ptr1);
    // freeing the mem. space immediately after to ensure consistency
    free(ptr1);
    int x = 3;
    printf("location of stack: %p\n", &x);

    printf("\nassigning 1024 additional bytes to heap...");
    void *p1 = sbrk(1024);
    std::uintptr_t prg_brk1 = reinterpret_cast<std::uintptr_t>(p1);

    printf("\nassigning another 2048 additional bytes to heap...");
    void *p2 = sbrk(2048);
    std::uintptr_t prg_brk2 = reinterpret_cast<std::uintptr_t>(p2);

    printf("\nheap increased by %ld bytes (in the previous acquisition), location of new heap space: %p\n", prg_brk2-prg_brk1, p1);

    // this is a dummy mem. acquisition 
    void *p3 = sbrk(1024);
    std::uintptr_t prg_brk3 = reinterpret_cast<std::uintptr_t>(p3);

    printf("\nheap increased by %ld bytes, location of new heap space: %p\n", prg_brk3-prg_brk2, p3);

    // freeing acquired memory space: unnecessary but a good habit
    // honestly speaking the os reclaims the memory at the end of the program itself
    // but wouldn't hurt to do it
    sbrk(-1024); // first  acquisition
    sbrk(-2048); // second acquisition
    sbrk(-1024); // third  acquisition


    // we should probably get the same answer as in line [17]
    // since we don't have any other (unfreed) heap allocations in between 
    void* ptr2 = malloc(50);
    printf("\nlocation of heap: %p", ptr2);
    free(ptr2);
    return 0;
}

