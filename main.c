//#define _PRINT                // define to print thread execution tracing messages,   remove definition for optimal execution time
#define _RUN_FOR_10_SECONDS     // define to run process for 10 seconds before exiting, remove definition to keep running

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include <unistd.h>
#include <assert.h>
#include <semaphore.h>

#define M 10   // writers
#define N 20   // readers

#define BUFFER_SIZE 62
#define min(A,B) (A>B?B:A)

// synchronization objects
pthread_mutex_t lock_buffer_shared = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock_count_readers = PTHREAD_MUTEX_INITIALIZER;     // to lock access to a counter of reader threads
pthread_mutex_t lock_count_writers = PTHREAD_MUTEX_INITIALIZER;     // to lock access to a counter of writer threads
pthread_mutex_t lock_printf = PTHREAD_MUTEX_INITIALIZER;            // to lock access to stdout
sem_t sem_count_packets;                                            // to count packets created by writers and signal readers to consume and process data

void process_data(char *buffer, int bufferSizeInBytes);
int get_external_data(char *buffer, int bufferSizeInBytes);  // generates a string of up to 62 length

/*********************************************************
**********************************************************
***********   DO NOT MODIFY THIS SAMPLE CODE   ***********
**********************************************************
**********************************************************/
int get_external_data(char *buffer, int bufferSizeInBytes)
{
    // int status;             // unused variable commented out
    int val;
    char srcString[] = "0123456789abcdefghijklmnopqrstuvwxyxABCDEFGHIJKLMNOPQRSTUVWXYZ";

    val = (int)(random()%min(bufferSizeInBytes, 62));

    if (bufferSizeInBytes < val)
        return (-1);

    strncpy(buffer, srcString, val);

    return val;
}
void process_data(char *buffer, int bufferSizeInBytes)
{
    //int i;                // unused variable commented out

    if(buffer)
    {
#ifdef _PRINT
        pthread_mutex_lock(&lock_printf);
        printf("thread %i - ", (long int)pthread_self());
        for(i=0; i<bufferSizeInBytes; i++)
        {
            printf("%c", buffer[i]);
        }
        printf("\n");
        pthread_mutex_unlock(&lock_printf);
#endif
        memset(buffer, 0, bufferSizeInBytes);
    }
    else{
        pthread_mutex_lock(&lock_printf);
        printf("error in process data - %li\n", (long int)pthread_self());
        pthread_mutex_unlock(&lock_printf);
    }

    return;
}
/*********************************************************
**********************************************************
********   PLEASE IMPLEMENT YOUR TEST CODE BELOW   *******
**********************************************************
**********************************************************/

// Assuming no memory constraints, linked list implementation of the shared memory buffer between readers and writers a simple and allows efficient read/write operations.
typedef struct Packet{
    char buff[BUFFER_SIZE];
    int size;
    struct Packet *next;
}Packet;

Packet *head, *tail;
int count_writers =0, count_readers =0;


/**
 * This thread is responsible for pulling data off of the shared data
 * area and processing it using the process_data() API.
 */

void *reader_thread(void *arg) {

    pthread_mutex_lock(&lock_count_readers);
    int th_id = ++ count_readers;
    pthread_mutex_unlock(&lock_count_readers);
    pthread_mutex_lock(&lock_printf);
    printf("Creating reader <%i>  thread_self # %li\n",th_id,(long int)pthread_self());// locking for %i lock %i and count %i\n", (long int)pthread_self(),ring_buffer_lock.__data.__owner,ring_buffer_lock.__data.__lock,ring_buffer_lock.__data.__count);
    pthread_mutex_unlock(&lock_printf);


    Packet * take_out_packet;
    while(1) {
        sem_wait(&sem_count_packets);                               //  wait until there's packets data in the buffer
#ifdef _PRINT
        pthread_mutex_lock(&lock_printf);
        printf(" +REDER <%i> self: %i found non empty LL, records count %i!\n",th_id,(long int)pthread_self(),count_packets);
        pthread_mutex_unlock(&lock_printf);
#endif
        // data available, race for the head.
        pthread_mutex_lock(&lock_buffer_shared);                    // LOCK

#ifdef _PRINT
        pthread_mutex_lock(&lock_printf);
        printf("++READER <[%i]> self: %i took the buffer lock!\n", th_id, (long int)pthread_self());
        pthread_mutex_unlock(&lock_printf);
#endif
        if(head ==NULL) printf("zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz\n");
        assert(head!=NULL);     // exit if reader is reading empty buffer.. semaphore wait would have failed then..
        // take out first in packet
        take_out_packet = head;
        head = head->next;      // update head pointer of LL.
        pthread_mutex_unlock(&lock_buffer_shared);                  // UN LOCK


        process_data(take_out_packet->buff,take_out_packet->size);
#ifdef _PRINT
        pthread_mutex_lock(&lock_printf);
        printf("~~Reader <%i> is clearing mem~~\n",th_id);
        pthread_mutex_unlock(&lock_printf);
#endif
        free(take_out_packet);                                     // cleanout processed packet
    }
    pthread_exit(NULL);
}

/**
 * This thread is responsible for pulling data from a device using
 * the get_external_data() API and placing it into a shared area
 * for later processing by one of the reader threads.
 */

void *writer_thread(void *arg) {

    pthread_mutex_lock(&lock_count_writers);
    int th_id = ++count_writers;
    pthread_mutex_unlock(&lock_count_writers);
    pthread_mutex_lock(&lock_printf);
    printf("Creating writer <%i> thread_self: %li \n", th_id, (long int)pthread_self());
    pthread_mutex_unlock(&lock_printf);
    Packet *new_packet;// pointer to the packet we will allocate when reading and add to LL

    while(1) {
        new_packet = (Packet*) malloc(sizeof(Packet));
        new_packet->size = get_external_data(new_packet->buff,BUFFER_SIZE);
        new_packet->next = NULL;
        //size = get_external_data(b,BUFFER_SIZE);
        if(new_packet->size <0) printf("HEYYYYY... what's data being written here? %s\n", new_packet->buff);
        assert(new_packet->size >=0);       // exit if get_external_data error
  //      if(size<0) printf("HEYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYyou \m");
  //      assert(size>=0);
/*        new_packet->buff = b;
        new_packet->size = size;
        */
     //   new_packet->buff[new_packet->size]='\0';        // make buff readable

#ifdef _PRINT
        pthread_mutex_lock(&lock_printf);
        printf("writer <%i> ",th_id);
        pthread_mutex_unlock(&lock_printf);
#endif

        pthread_mutex_lock(&lock_buffer_shared);                        // LOCK
        if(head==NULL){
#ifdef _PRINT
            pthread_mutex_lock(&lock_printf);
            printf("Writer  <%i> found empty LL buffer!. Setting LL only packet [%s]\n",th_id,new_packet->buff);
            pthread_mutex_unlock(&lock_printf);
#endif
            head = tail = new_packet;                                   // only packet will be head and tail
        }
        else{
#ifdef _PRINT
            pthread_mutex_lock(&lock_printf);
            printf("writer <%i> added [%s]\n",th_id,new_packet->buff);
            pthread_mutex_unlock(&lock_printf);
#endif
            tail->next = new_packet;                                    // append new packet to the tail of the LL
            tail = new_packet;                                          // new tail
        }
        pthread_mutex_unlock(&lock_buffer_shared);                      // UN LOCK

        sem_post(&sem_count_packets);                                   // post packet_counter_sem
    //    new_packet = (Packet*) malloc(sizeof(Packet));
    }
    pthread_exit(NULL);
}

int main(int argc, char **argv) {
    int i;
    count_readers =0;
    count_writers =0;

    pthread_t readers_th[N];
    pthread_t writers_th[M];

    sem_init(&sem_count_packets,0,0);

    int pthread_create_result;
    pthread_mutex_lock(&lock_printf);
    printf("Creating %i readers\n", N);
    pthread_mutex_unlock(&lock_printf);
    for(i = 0; i < N; i++) {
        pthread_create_result = pthread_create(&readers_th[i], NULL, reader_thread, &i);
        assert(!pthread_create_result); // Exit if error creating reader pthread
    }
    pthread_mutex_lock(&lock_printf);
    printf("Creating %i writers\n", M);
    pthread_mutex_unlock(&lock_printf);
    for(i = 0; i < M; i++) {
        pthread_create_result = pthread_create(&writers_th[i], NULL, writer_thread, &i);
        assert(!pthread_create_result); // Exit if error when creating writer pthread
    }
#ifdef _RUN_FOR_10_SECONDS
    sleep(2);
    return 0;
#else
    pthread_exit(NULL);
#endif
}
