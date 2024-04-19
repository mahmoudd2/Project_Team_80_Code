#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <sched.h>
#include <errno.h>
#include <unistd.h> 
#include <stdlib.h>
#include <sys/time.h>

void delay()
{
    int secs = 100000000;
    for (int i = 0; i < secs;i++)
    {
            //msh mehtagen haga hena.
    }
}

typedef struct {
    int seconds;
    int milliseconds;
    long microseconds;
} Timestamp;

Timestamp get_timestamp() {
    struct timeval current_time;
    gettimeofday(&current_time, NULL);
    long microsec = current_time.tv_usec;
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    int seconds = timeinfo->tm_sec;
    int milliseconds = microsec / 1000;
    microsec = microsec % 1000;

    // Create a Timestamp structure and populate its fields
    Timestamp timestamp;
    timestamp.seconds = seconds;
    timestamp.milliseconds = milliseconds;
    timestamp.microseconds = microsec;

    // Return the Timestamp structure
    return timestamp;
}

Timestamp timestamp_difference(Timestamp t1, Timestamp t2) {
    Timestamp diff;

    // Calculate the difference in seconds
    diff.seconds = t2.seconds - t1.seconds;

    // Calculate the difference in milliseconds
    diff.milliseconds = t2.milliseconds - t1.milliseconds;

    // Calculate the difference in microseconds
    diff.microseconds = t2.microseconds - t1.microseconds;

    // Adjust the difference if microseconds is negative
    if (diff.microseconds < 0) {
        diff.microseconds += 1000;
        diff.milliseconds--;
    }

    // Adjust the difference if milliseconds is negative
    if (diff.milliseconds < 0) {
        diff.milliseconds += 1000;
        diff.seconds--;
    }

    return diff;
}

void Tfunc1 ()
{
    printf("Inside the first thread\n");
    pthread_t id = pthread_self();

    Timestamp start = get_timestamp();
    printf("Start Time : %d seconds, %d milliseconds, %ld microseconds\n", start.seconds, start.milliseconds, start.microseconds);

    printf ("first thread id %lu\n", (unsigned long) id);
    printf("Doing some work...\n");
    delay();
    printf("Finishing First Thread Up....\n");
    
    Timestamp end = get_timestamp();
    printf("End Time: %d seconds, %d milliseconds, %ld microseconds\n", end.seconds, end.milliseconds, end.microseconds);
    
    Timestamp diff = timestamp_difference(start, end);
    printf("Excution Time: %d seconds, %d milliseconds, %ld microseconds\n\n", diff.seconds, diff.milliseconds, diff.microseconds);

    pthread_exit(NULL); 
}
void Tfunc2 ()
{
    printf("Inside the second thread\n");
    pthread_t id = pthread_self();

    Timestamp start = get_timestamp();
    printf("Start Time : %d seconds, %d milliseconds, %ld microseconds\n", start.seconds, start.milliseconds, start.microseconds);

    printf ("second thread id %lu\n", (unsigned long) id);
    printf("Doing some work...\n");
    delay();
    printf("Finishing Second Thread up...\n");
    
    Timestamp end = get_timestamp();
    printf("End Time: %d seconds, %d milliseconds, %ld microseconds\n", end.seconds, end.milliseconds, end.microseconds);
    
    Timestamp diff = timestamp_difference(start, end);
    printf("Excution Time: %d seconds, %d milliseconds, %ld microseconds\n\n", diff.seconds, diff.milliseconds, diff.microseconds);

    pthread_exit(NULL); 
}
void Tfunc3 ()
{
    printf("Inside the third thread\n");
    pthread_t id = pthread_self();

    Timestamp start = get_timestamp();
    printf("Start Time : %d seconds, %d milliseconds, %ld microseconds\n", start.seconds, start.milliseconds, start.microseconds);

    printf ("third thread id %lu\n", (unsigned long) id);
    printf("Doing some work...\n");
    delay();
    printf("Finishing Third Thread up...\n");
    
    Timestamp end = get_timestamp();
    printf("End Time: %d seconds, %d milliseconds, %ld microseconds\n", end.seconds, end.milliseconds, end.microseconds);
    
    Timestamp diff = timestamp_difference(start, end);
    printf("Excution Time: %d seconds, %d milliseconds, %ld microseconds\n\n", diff.seconds, diff.milliseconds, diff.microseconds);

    pthread_exit(NULL); 
}
void Tfunc4 ()
{
    printf("Inside the fourth thread\n");
    pthread_t id = pthread_self();

    Timestamp start = get_timestamp();
    printf("Start Time : %d seconds, %d milliseconds, %ld microseconds\n", start.seconds, start.milliseconds, start.microseconds);

    printf ("Fourth thread id %lu\n", (unsigned long) id);
    printf("Doing some work...\n");
    delay();
    printf("Finishing Fourth Thread up...\n");
    
    Timestamp end = get_timestamp();
    printf("End Time: %d seconds, %d milliseconds, %ld microseconds\n", end.seconds, end.milliseconds, end.microseconds);
    
    Timestamp diff = timestamp_difference(start, end);
    printf("Excution Time: %d seconds, %d milliseconds, %ld microseconds\n\n", diff.seconds, diff.milliseconds, diff.microseconds);

    pthread_exit(NULL); 
}


int main() {
    pthread_t threads[4];
    int threadsIDS[4];
    int i;
    pthread_attr_t attr;
    struct sched_param param;

    pthread_attr_init(&attr);
    pthread_attr_setschedpolicy(&attr, SCHED_RR);

    cpu_set_t cpuset;
    CPU_ZERO (&cpuset) ;
    CPU_SET(0, &cpuset); // Set core number here
    pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);

    param.sched_priority = sched_get_priority_max(SCHED_RR);
    pthread_attr_setschedparam(&attr, &param);


    pthread_create(&threads[1], &attr,Tfunc1,NULL);
    pthread_create(&threads[2], &attr,Tfunc2,NULL); 
    pthread_create(&threads[3], &attr,Tfunc3,NULL); 
    pthread_create(&threads[4], &attr,Tfunc4,NULL); 
    

    // Wait for all threads to finish

    pthread_join(threads[1], NULL);
    pthread_join(threads[2], NULL);
    pthread_join(threads[3], NULL);
    pthread_join(threads[4], NULL);


    printf("All threads finished.\n");
    return 0;
}

//leh fifo msh shgala sah hal 3shan el params wala 3shan el delay.
//mehtagen n3rf el start time w el end time mehtagen n show it wala el excution bs