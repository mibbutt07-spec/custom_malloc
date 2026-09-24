#include <stdio.h>
#include <string.h>
#include <assert.h>
void *custom_malloc(size_t length);
void custom_free(void *ptr);

int main(void) {

    // Normal allocation Test
    printf("Basic allocation: ");
    int *numbers = custom_malloc(5 * sizeof(int));
    assert(numbers != NULL);

    for (int i = 0; i < 5; i++) {
        numbers[i] = i * 10;
    }

    for (int i = 0; i < 5; i++) {
        assert(numbers[i] == i * 10);
    }

    printf("PASS\n");


    // Multiple allocations test
    printf("Multiple allocations: ");

    int *a = custom_malloc(sizeof(int));
    int *b = custom_malloc(sizeof(int));

    assert(a != NULL);
    assert(b != NULL);
    assert(a != b);

    *a = 10;
    *b = 20;

    assert(*a == 10);
    assert(*b == 20);

    printf("PASS\n");


    // Free and reuse memory test
    printf("Reusing freed memory: ");

    custom_free(a);

    int *c = custom_malloc(sizeof(int));

    assert(c != NULL);

    /*
     * Since a's block was freed and is large enough,
     * the allocator should reuse it.
     */
    assert(c == a);

    printf("PASS\n");


    // Splitting a free block test
    printf("Splitting free block: ");

    custom_free(c);

    /*
     * The block previously occupied by c should now
     * be available. Requesting a smaller amount should
     * cause the allocator to split the block.
     */
    char *small = custom_malloc(10);
    char *another = custom_malloc(10);

    assert(small != NULL);
    assert(another != NULL);
    assert(small != another);

    printf("PASS\n");


    // Free adjacent blocks test
    printf("Freeing adjacent blocks: ");

    custom_free(small);
    custom_free(another);

    
    // The allocator should merge adjacent free blocks.
    
    char *large = custom_malloc(20);

    assert(large != NULL);

    printf("PASS\n");


    // Writing larger amounts of data test 
    printf("Writing to allocated memory: ");

    char *message = custom_malloc(100);

    assert(message != NULL);

    strcpy(message, "wassup");

    assert(strcmp(message, "wassup") == 0);

    printf("PASS\n");


    custom_free(numbers);
    custom_free(b);
    custom_free(large);
    custom_free(message);

    printf("\nAll tests passed!\n");

    return 0;
}