// build with gcc -pthread answer.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
// New headers
#include "RingBuffer.h"
#include "HelperFunctions.h"
// New Headers To here

void process_data(char *buffer, int bufferSizeInBytes);
int get_external_data(char *buffer, int bufferSizeInBytes);  // generates a string of up to 62 length

/*********************************************************
**********************************************************
***********   DO NOT MODIFY THIS SAMPLE CODE   ***********
**********************************************************
**********************************************************/
int get_external_data(char *buffer, int bufferSizeInBytes)
{
    int status;             // ZAID - what's the purpose of this int?
    int val;
    char srcString[] = "0123456789abcdefghijklmnopqrstuvwxyxABCDEFGHIJKLMNOPQRSTUVWXYZ";

    val = (int)(random()%min(bufferSizeInBytes, 62));

    //  printf("*** val is: %i\n", val);   // removemem
    if (bufferSizeInBytes < val)
        return (-1);

    strncpy(buffer, srcString, val);
/// ZAID ADDED .. maybe return or remove me    buffer[val] = '\0';         // added to enable printing with %s
    return val;
}
void process_data(char *buffer, int bufferSizeInBytes)
{
    int i;

    if(buffer)
    {
        printf("thread %i - ", (int)pthread_self());
        for(i=0; i<bufferSizeInBytes; i++)
        {
            printf("%c", buffer[i]);
        }
        printf("\n");
        memset(buffer, 0, bufferSizeInBytes);
    }
    else
        printf("error in process data - %i\n", (int)pthread_self());

    return;
}
/*********************************************************
**********************************************************
********   PLEASE IMPLEMENT YOUR TEST CODE BELOW   *******
**********************************************************
**********************************************************/

//TODO Define global data structures to be used
typedef struct RingBuffer RingBuffer;
typedef struct RingBufferHandle RingBufferHandle;

int count_writers =0, count_readers =0;
/**
 * This thread is responsible for pulling data off of the shared data
 * area and processing it using the process_data() API.
 */

void *reader_thread(void *arg) {
    //TODO: Define set-up required

    while(COND) {
        //TODO: Define data extraction (queue) and processing

        // read from ring buffer
        // process read data
        /*       void process_data(char *buffer, int bufferSizeInBytes)
         *   Where buffer is a pointer to the data to be processed that is bufferSizeInBytes
         *   in length.
         */
    }

    //  return NULL;
    pthread_exit(NULL);
}

/**
 * This thread is responsible for pulling data from a device using
 * the get_external_data() API and placing it into a shared area
 * for later processing by one of the reader threads.
 */
void *writer_thread(void *arg) {
    //TODO: Define set-up required
    LOCK_W_COUNT
    printf("Creating writer thread #%i ID# %i\n", ++count_writers,(int)pthread_self());
    UNLOCK_W_COUNT

    char buff[WRITER_BUFFER_SIZE];
    strcpy(buff,"abcabc");
    int ret;
    ret = get_external_data(buff,BUFFER_SIZE);
    printf(" writer %i read: %s with return val %i\n", buff,ret);
    //    print_read(buff,ret);
    while(COND) {
        //TODO: Define data extraction (device) and storage

        //TODO:  read from the serialized device
        /*  int get_external_data(char *buffer, int bufferSizeInBytes)
         *          Where buffer is a pointer to a storage of bufferSizeInBytes that can be filled in.
         *          The return value from this function indicates the number of bytes that have been
         *          filled in or < 0 on error.
        */
        printf(" writer TH read: % s with return val %i\n", /*buff,*/ret);
        ret = get_external_data(buff,BUFFER_SIZE);
        printf(" writer TH read: % s with return val %i\n", /*buff,*/ret);
        //TODO:  write to the ring buffer

    }
    // return NULL;

    pthread_exit(NULL);
}

#define M 1 // 10   // writers
#define N 20   // readers

pthread_t readers_th[N];
pthread_t writers_th[M];

int main(int argc, char **argv) {
    int i;
    // init Mutex
 /*   int mutex_init_result;
    mutex_init_result = pthread_mutex_init(&w_count_lock,NULL);
    assert(!mutex_init_result);
    mutex_init_result = pthread_mutex_init(&r_count_lock,NULL);
    assert(!mutex_init_result);
*/
    pthread_t readers_t[N];
    pthread_t writers_t[M];


    //// remove meee.....
   /* char buff[BUFFER_SIZE];
    for(i=0;i<100;i++){

        int retSize= get_external_data(buff,BUFFER_SIZE);
        printf("%i - %s\n",retSize,buff);
    }
    **/
    /// remove me up to here

    //    struct RingBuffer ring_buffer;

    int pthread_create_result;
    printf("Creating %i readers\n", N);
    for(i = 0; i < N; i++) {
        pthread_create_result = pthread_create(&readers_th[i], NULL, reader_thread, NULL);
        assert(!pthread_create_result); // Exit if error creating reader pthread
    }
    printf("Creating %i writers\n", M);
    for(i = 0; i < M; i++) {
        pthread_create_result = pthread_create(&writers_th[i], NULL, writer_thread, NULL);
        assert(!pthread_create_result); // Exit if error when creating writer pthread
    }


    long long x=0;
    for(i=0;i<100000000;i++)x+=18%1000;

    for(i=0; i<N;i++) pthread_join(readers_t[i], NULL);
    for(i=0; i<M;i++) pthread_join(writers_t[i], NULL);
    printf("joined all threads ~~~~~~~~~~~~~~~~~~~~~~~\n");

    pthread_mutex_destroy(&w_count_lock);
    pthread_mutex_destroy(&r_count_lock);
    return 0;
    pthread_exit(NULL);
}
