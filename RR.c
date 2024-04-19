#include <stdio.h>
#include <pthread.h>
#include <sched.h>
#include <errno.h>
#include <unistd.h> 
#include <stdlib.h>
#include <sys/time.h>

void delay(char policy)
{
    if (policy == SCHED_RR || policy == SCHED_OTHER)
    {
        int secs = 1000000;
        for (int i = 0; i < secs;i++)
        {
            //msh mehtagen haga hena.
        }
    }
}

void print_timestamp(const char* message, long thread_id) {
    struct timeval current_time;
    gettimeofday(&current_time, NULL);
    long microsec = current_time.tv_usec;
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    int hours = timeinfo->tm_hour;
    int minutes = timeinfo->tm_min;
    int seconds = timeinfo->tm_sec;
    int milliseconds = microsec / 1000;
    microsec = microsec % 1000;

    printf("[%02d:%02d:%02d:%03d:%ldμs] thread_id<%ld>: %s\n", hours, minutes, seconds, milliseconds, microsec, thread_id, message);
}

typedef struct {
    int *param1;
    char param2;
} ThreadParams;

void *threadFunction(void *vargp) {
    //int *threadId = (int *)vargp;

    ThreadParams *params = (ThreadParams *)vargp;
    int *param1 = params->param1;
    char param2 = params->param2;

    pthread_t id = pthread_self();
    print_timestamp("",id); ///hena el moshkela bta3t el params.

    printf("Thread ID: %d Hello from thread!\n", param1);// w fe kol el print statments.
    printf("Thread ID: %d Doing some work...\n", param1);
    delay(param2);//param2 deh el policy el hnkhtrha.
    printf("Thread ID: %d Finishing up...\n", param1);
    printf("\n");
    pthread_exit(NULL);
    print_timestamp("",id);

    return NULL;
}

int main() {
    pthread_t threads[4];
    int threadsIDS[4];
    int i;
    //int param1 = threadsIDS[i];
    pthread_attr_t attr;
    struct sched_param param;
    struct timespec quantum ;

    #define SCHED_OTHER		0
    #define SCHED_FIFO		1
    #define SCHED_RR		2
    
    int policy;
    char param2 = policy;

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
            fprintf(stderr, "Incorrect Choice, Using The Default Policy RR");
            policy = SCHED_RR;
    }

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
        threadsIDS[i] = i;
        int param1 = threadsIDS[i];
        ThreadParams params;
        params.param1 = param1;
        params.param2 = param2;

        pthread_create(&threads[i], &attr, threadFunction,&threadsIDS[i]); // na2esna el passing bta3 el parameters be el tare2a el sah. w nfham eh el far2 lma ahot el attr aw el params
    }

    // Wait for all threads to finish
    for(i = 0; i < 4; i++) {
        pthread_join(threads[i], NULL);
    }


    printf("Main thread: All threads finished.\n");
    return 0;
}

//leh fifo msh shgala sah hal 3shan el params wala 3shan el delay.
//mehtagen n3rf el start time w el end time mehtagen n show it wala el excution bs