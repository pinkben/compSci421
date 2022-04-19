#include <stdio.h>
#include <pthread.h>
#include <stdlib.h> // rand, etc
#include <unistd.h> // sleep

#define TEACHER_TEACH_MIN 2
#define TEACHER_TEACH_MAX 5
#define TEACHER_RESEARCH_MIN 5
#define TEACHER_RESEARCH_MAX 8
#define INSPIRE_MIN 7
#define INSPIRE_MAX 10
#define STUDENT_SOCIALIZE_MIN 1
#define STUDENT_SOCIALE_MAX 4

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


void *teacher_sleep()
{



    while(1)
    {
        while(teacher_sleeping_cond)
        {
            printf("Teacher is waiting for class to start(sleeping)");
            pthread_cond_wait(&teacher_sleeping_cond, &teacher_sleeping_mutex);
            teacher_sleeping = 0;
        }

        if(students_in_classroom == 10)
        {
            int teaching_time = (rand() % (TEACHER_TEACH_MAX - TEACHER_TEACH_MIN + 1)) + TEACHER_TEACH_MIN;

            pthread_cond_wait(&waiting_for_class_to_start_cond, &waiting_for_class_to_start_mutex);
            
            waiting_for_class_to_start = 0;
            
            printf("teaching");
        }



    }




}

void teach_class()
{

}

void do_research()
{

}

void *student_decision()
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
        int *counter = malloc(sizeof(int*));
        *counter = i;

        //Create student threads
        pthread_create(&student_threads[i], NULL, student_decision, (void *)counter);
    }

    pthread_exit(NULL);
}