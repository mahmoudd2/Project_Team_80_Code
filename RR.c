#include <stdio.h>
#include <pthread.h>
#include <sched.h>
#include <errno.h>
#include <unistd.h> // For usleep function

void *threadFunction(void *vargp) {
  int *threadId = (int *)vargp;
 
  printf("Inside the %d thread\n",*threadId);
  pthread_t id = pthread_self();
  printf("Thread ID: %lu Hello from thread!\n", (unsigned long) id);
// usleep(1000000); // Simulate some work (sleep for 1 second)
  printf("Thread ID: %lu Doing some work...\n", (unsigned long) id);
  usleep(1000000); // Simulate some work (sleep for 1 second)
  printf("Thread ID: %lu Finishing up...\n", (unsigned long) id);
   
  printf("\n");
  return NULL;
}

int main() {
    pthread_t threads[4];
    int i;
    pthread_attr_t attr;
    struct sched_param param;
    struct timespec quantum ;

    // Initialize thread attributes
    pthread_attr_init(&attr);
    
    // Set scheduling policy to SCHED_RR
    pthread_attr_setschedpolicy(&attr, SCHED_RR);
    
    // Set priority for threads
    param.sched_priority = 10; // Set priority to 10
    pthread_attr_setschedparam(&attr, &param);
  // Get the time quantum for Round Robin scheduling
    int result = sched_rr_get_interval(0, &quantum);
    if (result == -1) {
        perror("sched_rr_get_interval");
        return errno;
    }

    printf("Time quantum for Round Robin scheduling: %ld ns\n", quantum.tv_nsec);



    for(i = 0; i < 4; i++) {
        pthread_create(&threads[i], &attr, threadFunction,&i);
    }

    // Wait for all threads to finish
    for(i = 0; i < 4; i++) {
        pthread_join(threads[i], NULL);
        
    }
    printf("Main thread: All threads finished.\n");
    return 0;
}