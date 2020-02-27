/**********************************************
 * Last Name:   Namkoong
 * First Name:  Kyoung Hwan
 * Student ID:  10125240
 * Course:      CPSC 457
 * Tutorial:    01
 * Assignment:  3
 *********************************************/
/// counts number of primes from standard input
///
/// compile with:
///   $ gcc countPrimes.c -O2 -o countPrimes -lm -lpthread
///
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

struct ThreadInfo {
    int tid;
    int64_t num;
    int64_t startIndex;
    int result;
};

int64_t count;
int stop = 0;
int nThreads;

/// primality test, if n is prime, return 1, else return 0
void * isPrime(void *trd) {
    //converts the trd argument back into the struct
    struct ThreadInfo *n = trd;
    int64_t value = n->num;

    // small numbers are not primes
    if (value <= 1) {
        return 0; 
    }

    // 2 and 3 are prime
    if (value <= 3) {
        n->result = 1;
        return 0;
    }

    // multiples of 2 and 3 are not primes
    if (value % 2 == 0 || value % 3 == 0) {
        return 0; 
    }
    //get the sqrt of the number
    int64_t max = (sqrt(value));
    int64_t i = n->startIndex;
    //while the starting index is less than the sqrt of number
    while(i <= max) {
        //condition to stop checking any further
        if(stop) {
            n->result = 0;
        }
        if (value % i == 0 || value % (i+2) == 0) {
            n->result = 0;
            return 0;
        }
        i += nThreads*6;
    }
    //else, that number is prime
    stop = 1;
    n->result = 1;
    return 0;
}

//main function that creates the threads and calls the isPrime function
int main(int argc, char **argv) {
    /// parse command line arguments
    nThreads = 1;
    //if there are not 2 arguments passed
    if (argc != 1 && argc != 2) {
        printf("Usage: countPrimes [nThreads]\n");
        exit(-1);
    }
    //set nThreads to be the number of threads passed
    if (argc == 2) {
        nThreads = atoi(argv[1]);
    }
    /// handle invalid arguments
    if (nThreads < 1 || nThreads > 256) {
        printf("Bad arguments. 1 <= nThreads <= 256!\n");
    }
    /// count the primes
    printf("Counting primes using %d thread%s.\n",
            nThreads, nThreads == 1 ? "" : "s");
    //initialize an array of threads
    pthread_t id[nThreads];
    //initialize an array of structs
    struct ThreadInfo trd[nThreads];
    //count is the number of primes
    count = 0;
    
    while (1) {
        int64_t num;
        //get each number and store it in the num variable
        //if there is no number left, break out of the loop
        if (1 != scanf("%ld", &num)) {
            break;
        }
        //restart the stop variable
        stop = 0;
        //for each thread, set the initial variables and create the thread
        for (long i = 0; i < nThreads; i++) {
            trd[i].result = 0;
            trd[i].startIndex = 5 + (i*6);
            trd[i].num = num;
            trd[i].tid = i;
            if (pthread_create(&id[i], NULL, isPrime, (void *)&trd[i]) != 0) {
                printf("Error in creating thread!! \n");
                exit(0);
            } 
        }

        //join all the threads
        for (int i = 0; i< nThreads; i++) {
            pthread_join(id[i], NULL);
        }
        //check and see if there is a 0 in any of the thread indices
        int next = 1;
        for(int i = 0; i< nThreads; i++) {
            if (trd[i].result == 0){
                next = 0;
                break;
            }
        }
        count += next;
    }

    /// report results
    printf("Found %ld primes.\n", count);
    exit(1);
    return 0;
}
