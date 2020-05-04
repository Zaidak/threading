#ifndef _RingBuffer_header
#define _RingBuffer_header

#include <stdbool.h>
#include <assert.h>

// pthread synchronization
#define M  10   // writers
#define N 20   // readers

pthread_mutex_t shared_buffer_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t not_empty = PTHREAD_COND_INITIALIZER;

pthread_mutex_t r_count_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t w_count_lock = PTHREAD_MUTEX_INITIALIZER;

// Shared Data Structrue between writers and readers.
#define WRITER_BUFFER_SIZE 62
#define BUFFER_SIZE 255
///// functions to use
/// bool empty*(
/// void write (*data, len)
/// char* read ()
struct RingBuffer{
    bool buffer_used[BUFFER_SIZE];
    char buffer[BUFFER_SIZE];
    int sizes[BUFFER_SIZE];
    int reader_sizes_index; // Index of first in byte - for readers to start reading from
    int reader_data_index; // Index of first in byte - for readers to start reading from
    int writer_sizes_index; // Index of first available byte - for writers to start writing from
    int writer_data_index; // Index of first available byte - for writers to start writing from
    bool full;
    bool empty;
    //char read_byte(){ char ans = '0';    return ans;}
    //void write_byte(char data){    int a=0;}
};

void init_ring_buffer(struct RingBuffer rb){
    assert(rb.buffer && rb.sizes);
    for(int i=0;i<BUFFER_SIZE;i++){
        rb.buffer_used[i] = false;
        rb.buffer[i] = '\0';
        rb.sizes[i] =0;
    }
   rb.reader_sizes_index = 0;
    rb.reader_data_index =0;
    rb.writer_sizes_index = 0;
    rb.writer_data_index = 0;
    rb.empty = true;
    rb.full = false;                            // maybe remove
}

void add_data(struct RingBuffer rb, char* data, int size){         ///////////////////////////////////////////////////////////////////
    int a=0;
}

bool is_empty(struct RingBuffer rb){
    return rb.empty;
}

bool has_space(struct RingBuffer rb, int size){                     ///////////////////////////////////////////////////////////////////
    return !rb.full; //// to be completed
}

char * read_data(struct RingBuffer rb, int size){

}


void insert_packet(struct RingBuffer rb, char* data, int size){

}

char * read_next_packet(struct RingBuffer rb){

}
/// void rb_free();, void rb_reset();, void rb_write(char * data, size_t data_size);
/// char * rb_read(size_t data_size);
/// typedef struct ring_buffer ring_buffer;
/// typedef ring_buffer* rb_handle;
/// rb_handle rb_init();
///

struct RingBufferHandle{

};


#endif
