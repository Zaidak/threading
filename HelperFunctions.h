/*
 *  TODO:
 *      - Understand the working of the get_external_data
 *          [x] Do we need to Mutex the function calls? -> No: each writer is reading from a differend Dev
 *          [ ] With N
 *      - Have each writer use get_external_data function to get a piece of variable size data.
 *      - Have each writer use Mutexs to write their piece of data into the buffer
 *
 * */
#ifndef _HelperFunctions_header
#define _HelperFunctions_header

#include <assert.h>

#define COND 0                  // Inifinite loop in thread functions

// pthread functions
    // House keeping
pthread_mutex_t w_count_lock = PTHREAD_MUTEX_INITIALIZER, r_count_lock = PTHREAD_MUTEX_INITIALIZER;
    // Sol
pthread_mutex_t ring_buffer_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t not_empty = PTHREAD_COND_INITIALIZER;


// #define LOCK_PRINTF     printf("  x-   locking printf\n");            pthread_mutex_lock(&printf_lock);
// #define UNLOCK_PRINTF   printf("  o- unlocking printf\n");          pthread_mutex_unlock(&printf_lock);
#define LOCK_W_COUNT    printf("  x-   locking writers counter\n");   pthread_mutex_lock(&w_count_lock);
#define UNLOCK_W_COUNT  printf("  o- unlocking writers counter\n"); pthread_mutex_unlock(&w_count_lock);
#define LOCK_R_COUNT    printf("  x-   locking readers counter\n");   pthread_mutex_lock(&r_count_lock);
#define UNLOCK_R_COUNT  printf("  u- unlocking readers counter\n"); pthread_mutex_unlock(&r_count_lock);

// general use macros
#define MAX(A,B) A>B?A:B
#define min(A,B) (A>B?B:A)

void print_read(char* msg, size_t size){
    printf("  Printing %i chars\n%s\n",size,msg);
    for(int i=0;i<size;i++){
        printf(" ");
    }
    printf("\b^\n");

}

#endif
