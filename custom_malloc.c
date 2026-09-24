#include <stddef.h>
#include <unistd.h>

typedef struct BlockHeader {
    size_t length;
    int isfree;
    struct BlockHeader *prev;
    struct BlockHeader *next;

} BlockHeader;

#define HEADER_SIZE sizeof(BlockHeader)

static BlockHeader *start = NULL;


// Helper that looks for space and returns the space if it finds it
// Returns null otherwise

BlockHeader *findSpace(size_t length) {
    BlockHeader *current = start;

    while(current) {
        if (current->isfree == 1 && current->length >= length) {
            return current;
        } else {
            current = current->next;
        }
    
    }
    
    return NULL;
    
}

void merge(BlockHeader *tbc) {
    if (tbc->next && tbc->next->isfree == 1) {
        tbc->length += tbc->next->length + HEADER_SIZE;
        tbc->next = tbc->next->next;
        if (tbc->next) {
            tbc->next->prev = tbc; 
        }
    }

    if (tbc->prev && tbc->prev->isfree == 1) {
        tbc->prev->length += tbc->length + HEADER_SIZE;
        tbc->prev->next = tbc->next;
        if (tbc->next) {
            tbc->next->prev = tbc->prev;
        }
    }
}

void seperate(BlockHeader *tbs, size_t length) {
    if(tbs->length > length + HEADER_SIZE) {
        tbs->isfree = 0;
        BlockHeader *new_block = (BlockHeader *)((char *)(tbs + 1) + length); //this line idk
        new_block->length = tbs->length - length - HEADER_SIZE;
        new_block->isfree = 1;
        new_block->prev = tbs;
        new_block->next = tbs->next;
        if(tbs->next) {
            tbs->next->prev = new_block;
        }
        tbs->next = new_block;
        tbs->length  = length;

    }
}

BlockHeader *requestNewBlock(BlockHeader *last, size_t length) {
    BlockHeader* block = sbrk(0);
    void *request = sbrk(length + HEADER_SIZE);
    if (request == (void *)-1) {
        return NULL; // not exactly sure how ts works but we move
    }
    BlockHeader *new_block = block;
    new_block->isfree = 0;
    new_block->length = length;
    new_block->next = NULL;
    new_block->prev = last;
    if(last) {
        last->next = new_block;
    }
    return new_block;
}

void *custom_malloc(size_t length) {
    if (start == NULL) {
        start = requestNewBlock(NULL, length);
        return start + 1;
    } else {
        BlockHeader *loc = findSpace(length);
        if(loc == NULL) {
            BlockHeader *last = start;
            while (last->next) {
                last = last->next;
            }
            loc = requestNewBlock(last, length);
        } else {
            seperate(loc, length);
        }
        return (void *)(loc + 1);
    }

}

void custom_free(void *free) {
    if (!free) return;
    BlockHeader* tbf = (BlockHeader *) free  - 1;
    tbf->isfree = 1;
    merge(tbf);
}



