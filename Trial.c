#define _GNU_SOURCE
#include <sched.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

#define NUM_THREADS 4
int t;

void *thread_function(void *thread_id) {
    

    long tid = (long)thread_id; // Cast the thread_id to long

    printf("creating thread %ld\n", tid);

    printf("Hello from thread %ld\n", tid);

    for (int i = 0; i < 1000000000; i++) {
        t=0;
    }
    
    pthread_exit(NULL); // Exit the thread
}


int main() {
    
    // Create threads with default attributes
    pthread_t threads[NUM_THREADS]; // Array to store thread IDs
    int i;

    // Create threads
    for (i = 0; i < NUM_THREADS; i++) {
        int status = pthread_create(&threads[i], NULL, thread_function, (void *)(intptr_t)i);
        if (status != 0) {
            fprintf(stderr, "Error creating thread %d\n", i);
            exit(EXIT_FAILURE);
        }
    }

    // Wait for all threads to finish
    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(0, &cpuset);
   
    if (sched_setaffinity(0, sizeof(cpu_set_t), &cpuset) != 0) {
        perror("sched_setaffinity");
        exit(EXIT_FAILURE);
    }

    #define SCHED_OTHER		0
    #define SCHED_FIFO		1
    #define SCHED_RR		2

    pthread_attr_t attr;
    struct sched_param param;
    
    int policy;

    printf("Enter the desired scheduling policy:\n");
    printf("1. SCHED_FIFO\n");
    printf("2. SCHED_RR\n");
    printf("3. SCHED_OTHER\n");
    printf("Enter your choice (1-3): ");
    scanf("%d", &policy);

    pthread_attr_init(&attr);

    pthread_attr_setschedpolicy(&attr, policy);

    switch (policy) {
        case 1:
            param.sched_priority=99;
            policy = SCHED_FIFO;
            break;
        case 2:
            param.sched_priority=99;
            policy = SCHED_RR;
            break;
        case 3:
            param.sched_priority = 0;
            policy = SCHED_OTHER;
            break;
        default:
            fprintf(stderr, "Invalid choice. Using default policy (SCHED_FIFO).\n");
            policy = SCHED_FIFO;
    }

   
}