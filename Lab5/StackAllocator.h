#pragma once
class StackAllocator {
public:
    // This is the pointer to the first memory location of the
    // stack
    void* m_StartPtr = nullptr;
    // The total size of the memory to be allocated size
    // (ITEMTYPE) * n_items
    size_t m_TotalSize;
    // Offset from the initial address, it should be increase
    // with every allocation
    size_t m_Offset;

    /*
    Here you should allocate enough memory (malloc) for the whole stack, which will then be
    managed internally, returning free addresses each time we ask for a new allocation
    */
    void init(size_t total_size);
    /*
    This should be a fast function that just computes and
    returns the next free address, depending on the
    ones that have already been allocated and the size of what
    we're allocating.
    */
    void* allocate(size_t size);
    /*
    This function should just reset the offset to zero, so that
    we can "clear" the stack while keep using
    the memory that we have allocated in the init function.
    */
    void reset();
    /*
    This should reset the stack and also free up the memory
    that was allocated in the init function
    */
    ~StackAllocator();
};
