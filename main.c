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
    // int status;             // unused variable comented out
    int val;
    char srcString[] = "0123456789abcdefghijklmnopqrstuvwxyxABCDEFGHIJKLMNOPQRSTUVWXYZ";

    val = (int)(random()%min(bufferSizeInBytes, 62));

    //  printf("*** val is: %i\n", val);   // removemem
    if (bufferSizeInBytes < val)
        return (-1);

    strncpy(buffer, srcString, val);
    /// ZAID ADDED .. maybe return or remove me    buffer[val] = '\0';         // added to enable printing with %s
//    buffer[val]= '\0'; // add null char after the data read to make the buffer printable
    return val;
}
void process_data(char *buffer, int bufferSizeInBytes)
{
    int i;

    if(buffer)
    {
        printf("thread %i - ", (long int)pthread_self());
        for(i=0; i<bufferSizeInBytes; i++)
        {
            printf("%c", buffer[i]);
        }
        printf("\n");
        memset(buffer, 0, bufferSizeInBytes);
    }
    else
        printf("error in process data - %i\n", (long int)pthread_self());

    return;
}
/*********************************************************
**********************************************************
********   PLEASE IMPLEMENT YOUR TEST CODE BELOW   *******
**********************************************************
**********************************************************/

//TODO Define global data structures to be used
/*typedef struct RingBuffer RingBuffer;
typedef struct RingBufferHandle RingBufferHandle;

RingBuffer ring_buffer;*/

typedef struct Packet{
    struct Packet *next;
    char *buff;
    int size;
}Packet;

int r_count, w_count;
int count_writers =0, count_readers =0;
/**
 * This thread is responsible for pulling data off of the shared data
 * area and processing it using the process_data() API.
 */

void *reader_thread(void *arg) {
    //TODO: Define set-up required

    pthread_mutex_lock(&r_count_lock);
    int th_id = ++ r_count;
    printf("Creating reader <%i>  thread_self # %i\n",th_id,(long int)pthread_self());// locking for %i lock %i and count %i\n", (long int)pthread_self(),ring_buffer_lock.__data.__owner,ring_buffer_lock.__data.__lock,ring_buffer_lock.__data.__count);
    pthread_mutex_unlock(&r_count_lock);
            /// Lock RB

    pthread_mutex_lock(&shared_buffer_lock);
    while(1) {

        /// Wait for RB has data and RB mutex
        pthread_cond_wait(&not_empty,&shared_buffer_lock);
        //TODO: Define data extraction (queue) and processing

        /// read from ring buffer
        printf("++READER <%i> self: %i took the cond wait!\n", th_id, (long int)pthread_self());//(long int)pthread_self());
        /// process read data
        /*       void process_data(char *buffer, int bufferSizeInBytes)
         *   Where buffer is a pointer to the data to be processed that is bufferSizeInBytes
         *   in length.
         */
        /// IF NOT towards t he end... how can I tell I am towards the end?? count R? then sleep for a bit?
        /// if(r_count < M - 2)
        ///     sleep(rand()%3+1);
        if(th_id < M-2)
            USLEEP;

        /// UNLOCK RB
        pthread_mutex_unlock(&shared_buffer_lock);
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

    pthread_mutex_lock(&w_count_lock);
    int th_id = ++w_count;
    printf("Creating writer <%i> thread_self: %i \n", w_count, (long int)pthread_self());
    pthread_mutex_unlock(&w_count_lock);
    while(1) {
        ///// LOCK RB       /////////////////////////////////////////////////////////////////////////////////////////
        pthread_mutex_lock(&shared_buffer_lock);
        //TODO: Define data extraction (device) and storage
        char buff[WRITER_BUFFER_SIZE];
        int size;

        //TODO:  read from the serialized device
        size = get_external_data(buff,BUFFER_SIZE);
        assert(size >=0);       // get_external_data error
        buff[size]='\0';        // make buff readable
        print_read(buff,size);
        //TODO:  write to the ring buffer
        /// CALL RB.write(buff,ret);
        /*if(has_space(ring_buffer, size)){
            //pthread_cond_wait              ///////////////////////////////////////////////////////////////////////////////////////////
            int a;
        }*/


/////////// Should i ?? pthread_mutex_unlock(&ring_buffer_lock); /// ZAID
        ///   Signal the cond
        pthread_cond_signal(&not_empty);
        /// UNLOCK RB       /////////////////////////////////////////////////////////////////////////////////////////
        pthread_mutex_unlock(&shared_buffer_lock);

        /// IF NOT towards t he end... how can I tell I am towards the end?? count W? then sleep for a bit?
        if(th_id < N-2)
            //usleep(rand()%USLEEP_UP_TO+1);
            USLEEP;
    }
    // return NULL;

    pthread_exit(NULL);
}


//pthread_t readers_th[N];
//pthread_t writers_th[M];

int main(int argc, char **argv) {
    int i;
    r_count =0;
    w_count =0;

    pthread_t readers_th[N];
    pthread_t writers_th[M];


//    init_ring_buffer(ring_buffer);
    //init_packet_buffer();

    int pthread_create_result;

    printf("Creating %i readers\n", N);
    for(i = 0; i < N; i++) {
        pthread_create_result = pthread_create(&readers_th[i], NULL, reader_thread, &i);
        assert(!pthread_create_result); // Exit if error creating reader pthread
    }
    printf("Creating %i writers\n", M);
    for(i = 0; i < M; i++) {
        pthread_create_result = pthread_create(&writers_th[i], NULL, writer_thread, &i);
        assert(!pthread_create_result); // Exit if error when creating writer pthread
    }

    for(i=M-1; i>=0;i--) pthread_join(writers_th[i], NULL);
    for(i=N-1; i>=0;i--) pthread_join(readers_th[i], NULL);

    printf("joined all threads ~~~~~~~~~~~~~~~~~~~~~~~\n");

    pthread_mutex_destroy(&r_count_lock);
    pthread_mutex_destroy(&w_count_lock);
    pthread_mutex_destroy(&shared_buffer_lock);
    return 0;
    pthread_exit(NULL);
}
