
typedef unsigned int u32;

#define JIX_HEAP_SIZE (1024 * 1024)
#define JIX_HEAP_MAGIC 0x4A495848
#define JIX_ALIGNMENT 8

typedef struct _JIX_HEAP_BLOCK {
    u32 magic;
    u32 size;
    u32 free;

    struct _JIX_HEAP_BLOCK* next;
    struct _JIX_HEAP_BLOCK* prev;
} JIX_HEAP_BLOCK;

static unsigned char jix_heap[JIX_HEAP_SIZE]
    __attribute__((aligned(16)));

static JIX_HEAP_BLOCK* heap_first = 0;

static u32 alignSize(u32 size)
{
    return (size + (JIX_ALIGNMENT - 1))
        & ~(JIX_ALIGNMENT - 1);
}

void JixMemoryInstall()
{
    heap_first = (JIX_HEAP_BLOCK*)jix_heap;

    heap_first->magic = JIX_HEAP_MAGIC;
    heap_first->size =
        JIX_HEAP_SIZE - sizeof(JIX_HEAP_BLOCK);

    heap_first->free = 1;
    heap_first->next = 0;
    heap_first->prev = 0;
}

void* allocateMemory(unsigned int size)
{
    if (!heap_first || size == 0)
        return 0;

    if (size > JIX_HEAP_SIZE -
        sizeof(JIX_HEAP_BLOCK))
        return 0;

    size = alignSize(size);

    JIX_HEAP_BLOCK* block = heap_first;

    while (block)
    {
        if (block->magic != JIX_HEAP_MAGIC)
            return 0;

        if (block->free && block->size >= size)
        {
            // Split block if enough space remains.
            if (block->size >=
                size + sizeof(JIX_HEAP_BLOCK) + JIX_ALIGNMENT)
            {
                JIX_HEAP_BLOCK* next =
                    (JIX_HEAP_BLOCK*)((unsigned char*)
                    (block + 1) + size);

                next->magic = JIX_HEAP_MAGIC;
                next->size =
                    block->size - size -
                    sizeof(JIX_HEAP_BLOCK);

                next->free = 1;
                next->next = block->next;
                next->prev = block;

                if (next->next)
                    next->next->prev = next;

                block->next = next;
                block->size = size;
            }

            block->free = 0;

            return (void*)(block + 1);
        }

        block = block->next;
    }

    return 0;
}

void freeMemory(void* buff)
{
    if (!buff || !heap_first)
        return;

    unsigned int address = (unsigned int)buff;
    unsigned int start = (unsigned int)jix_heap;
    unsigned int end = start + JIX_HEAP_SIZE;

    if (address < start + sizeof(JIX_HEAP_BLOCK) ||
        address >= end)
        return;

    JIX_HEAP_BLOCK* block =
        ((JIX_HEAP_BLOCK*)buff) - 1;

    if ((unsigned int)block < start ||
        (unsigned int)block + sizeof(JIX_HEAP_BLOCK) > end)
        return;

    if (block->magic != JIX_HEAP_MAGIC ||
        block->free)
        return;

    block->free = 1;

    // Merge with next block.
    if (block->next && block->next->free &&
        block->next->magic == JIX_HEAP_MAGIC)
    {
        JIX_HEAP_BLOCK* next = block->next;

        block->size += sizeof(JIX_HEAP_BLOCK) + next->size;
        block->next = next->next;

        if (block->next)
            block->next->prev = block;
    }

    // Merge with previous block.
    if (block->prev && block->prev->free &&
        block->prev->magic == JIX_HEAP_MAGIC)
    {
        JIX_HEAP_BLOCK* prev = block->prev;

        prev->size += sizeof(JIX_HEAP_BLOCK) + block->size;
        prev->next = block->next;

        if (prev->next)
            prev->next->prev = prev;
    }
}
