#ifndef _RingBuffer_header
#define _RingBuffer_header

#include <stdbool.h>
#define WRITER_BUFFER_SIZE 62
#define BUFFER_SIZE 255
struct RingBuffer{
    bool buffer_used[BUFFER_SIZE];
    char buffer[BUFFER_SIZE];
    int first_in_index, next_free_index;
    bool full;
    //char read_byte(){ char ans = '0';    return ans;}
    //void write_byte(char data){    int a=0;}
};
/// void rb_free();, void rb_reset();, void rb_write(char * data, size_t data_size);
/// char * rb_read(size_t data_size);
/// typedef struct ring_buffer ring_buffer;
/// typedef ring_buffer* rb_handle;
/// rb_handle rb_init();
///

struct RingBufferHandle{

};


#endif
