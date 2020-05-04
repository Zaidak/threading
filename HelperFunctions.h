/*
 *  TODO:
 *      - Understand the working of the get_external_data
 *          [x] Do we need to Mutex the function calls? -> No: each writer is reading from a differend Dev
 *          [x] With N
 *      - Have each writer use get_external_data function to get a piece of variable size data.
 *      - Have each writer use Mutexs to write their piece of data into the buffer
 *
 * */
#ifndef _HelperFunctions_header
#define _HelperFunctions_header

// #define SECOND_IN_USECOND 1000000
#define USLEEP_FROM  50000// 50000 // 500000 // (0.5 s)
#define USLEEP_UP_TO 100000// 10000  // 1000000// (1 s)

#define USLEEP usleep((rand()%(USLEEP_UP_TO-USLEEP_FROM+1)+USLEEP_FROM))

// general use macros
#define min(A,B) (A>B?B:A)

void print_read(char* msg, size_t size){
    printf("  Printing %i chars\n%s\n",size,msg);
    for(int i=0;i<size;i++){
        printf(" ");
    }
    printf("\b^\n");

}

#endif
