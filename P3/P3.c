/*******************************************************************************
 * Description: P3 
 * Author: Demetrios Green & Ben Pink
 * Date: 04-20-2022
*******************************************************************************/

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
#define STUDENT_SOCIALIZE_MAX 4

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

void *studentThread();
void *teacherThread();

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

        if(students_in_classroom == 10) // I think the last student has to broadcast to the teacher
        {
            // teaching_time should be of type time_t
            time_t teaching_time = (rand() % (TEACHER_TEACH_MAX - TEACHER_TEACH_MIN + 1)) + TEACHER_TEACH_MIN;

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
    /* 
       I think we should consider changin the design you started here...
       I think we should have 2 functions, and maybe three for getting random times,
       other than the main one.  We should have a teacher function and a student fucntion.
       Having the other functions gets overcomplicated and I think right now this design would lead
       us to created a new thread any time a thread changed state.
       Each thread created calls on its function, so the pthread_create(&teacher, NULL, teacher_thread, NULL);
       and the student would be pthread_create(student_threads[i], NULL, student_thread, studentId);
       We could even possible use the main thread as the teacher if we wanted, but not sure.
       The way I think of it is each of these functions is a student/teacher object.  So the fucntion body
       handles the behavior/state machine of that object.
       I am going to attempt to create the student function before I leave for dinner to try to get an 
       example of what I am thinking here.
    */

    // Init locks
    assert(pthread_mutex_init(&waiting_for_class_to_start_mutex, NULL) == 0);
    assert(pthread_mutex_init(&students_in_classroom_mutex, NULL) == 0);
    assert(pthread_mutex_init(&teacher_sleeping_mutex, NULL) == 0);
    assert(pthread_mutex_init(&class_dismissed_mutex, NULL) == 0);
    assert(pthread_mutex_init(&lightning_strikes_mutex, NULL) == 0);
    
    // Init varibles
    assert(pthread_cond_init(&teacher_sleeping_cond, NULL) == 0);
    assert(pthread_cond_init(&class_dismissed_cond, NULL) == 0);
    assert(pthread_cond_init(&waiting_for_class_to_start_cond, NULL) == 0);

    pthread_create(&teacher, NULL, teacherThread, (void *)NULL);

    for(int i = 0; i < 10; i++)
    {
        int *counter = malloc(sizeof(int*));
        *counter = i;

        //Create student threads
        pthread_create(&student_threads[i], NULL, studentThread, (void *)counter);
    }

    pthread_exit(NULL);
}

void *studentThread(void *num)
{
  time_t sometime;
  srand((unsigned) time(&sometime));
  // Get id
  int id = *((int *) num);
  while(1)
  {
    int option = (rand() % 2);
    if (option)
    {
      // Go to class
      pthread_mutex_lock(&waiting_for_class_to_start_mutex);
      pthread_mutex_lock(&students_in_classroom_mutex);
      students_in_classroom++;
      pthread_mutex_unlock(&students_in_classroom_mutex);
      printf("student_thread[%d] : in the classroom (%d)\n", id, students_in_classroom);
      pthread_cond_wait(&waiting_for_class_to_start_cond, &waiting_for_class_to_start_mutex);
      pthread_mutex_unlock(&waiting_for_class_to_start_mutex);
    }
    else
    {
      // Socalize
      printf("student_thread[%d] : socializing\n", id);
      time_t wait_time = (rand() % (STUDENT_SOCIALIZE_MAX - STUDENT_SOCIALIZE_MIN + 1)) + STUDENT_SOCIALIZE_MIN;
      sleep(wait_time);
    }
  }
  return NULL;
}

void teacherThread()
{
  time_t sometime;
  srand((unsigned) time(&sometime));
  while(1)
  {
    pthread_mutex_lock(&teacher_sleeping_mutex);
    print("teacher_thread : sleeping\n");

  }
  return NULL;
}