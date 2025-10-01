#include "lib.h"
#include <time.h>
#include <unistd.h> 
#include <pthread.h>
#include <stdio.h>



void print_current_time() {
    time_t raw_time;
    struct tm *time_info;
    char buffer[80];

    time(&raw_time);
    time_info = localtime(&raw_time);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", time_info);
    printf("Current time: %s\n", buffer);
}

void* my_sleep(void* arg) {
    // Cast the void pointer back to the correct data type (int pointer).
    int sleep_duration = *((int*)arg);

    printf("Thread starting, will sleep for %d seconds...\n", sleep_duration);

    // Use the dereferenced value for sleep.
    sleep(sleep_duration);

    printf("Thread finished sleeping for %d seconds.\n", sleep_duration);

    return NULL; // A thread function should return a void pointer.
}

void test(){
    print_current_time();
    printf("Main thread started.\n");

    pthread_t thread1;
    pthread_t thread2;

    int sleep_arg1 = 2;
    int sleep_arg2 = 4;

    // Arg 3: The function name (a pointer to the function).
    // Arg 4: A pointer to the argument for that function.
    pthread_create(&thread1, NULL, my_sleep, &sleep_arg1); 
    pthread_create(&thread2, NULL, my_sleep, &sleep_arg2); 

    // 5. ADDED: Wait for the threads to finish their execution.
    // Without this, main would exit before the threads are done.
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("All threads have finished.\n");
    print_current_time();
}