#include <stdio.h>
#include <pthread.h>
#include <stdlib.h> // rand, etc
#include <unistd.h> // sleep

pthread_mutex_t students_in_classroom_mutex;
int students_in_classroom = 0; // counter

pthread_mutex_t lightning_strikes_mutex;
int lightning_strikes = 0; // indicator;

pthread_mutex_t waiting_for_class_to_start_mutex;
pthread_cond_t waiting_for_class_to_start_cond;
int waiting_for_class_to_start = 1; // indicator

pthread_mutex_t class_dismissed_mutex;
pthread_cond_t class_dismissed_cond;
int class_dismissed = 1; // indicator

pthread_mutex_t teacher_sleeping_mutex;
pthread_cond_t teacher_sleeping_cond;
int teacher_sleeping = 1; // indicator

//Create teacher thread
pthread_t teacher;

//Create array for student threads
pthread_t student_threads[10];

void teacher_sleep()
{
    while(!lightning_strikes && students_in_classroom < 10)
    {
        printf("Teacher is waiting for class to start(sleeping)");
        //wait();
    }


}

void teach_class()
{

}

void do_research()
{

}

void student_decision()
{

}

void student_socialize()
{

}

void student_attend_class()
{

}



int main()
{
    pthread_create(&teacher, NULL, teacher_sleep, (void *)NULL);

    for(int i = 0; i < 10; i++)
    {
        //Create student threads
        pthread_create(&student_threads[i], NULL, student_decision, (void *)i);
    }

    pthread_exit(NULL);
}