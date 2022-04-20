/*******************************************************************************
 * Description: P3
 * Author: Demetrios Green & Ben Pink
 * Date: 04-20-2022
 *******************************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h> // rand, etc
#include <unistd.h> // sleep
#include <assert.h>

#define STUDENT_SIZE 10
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

pthread_t teacher; // Create teacher thread
pthread_t inspiration; // Create inspiration thread
pthread_t student_threads[STUDENT_SIZE]; // Create array for student threads

// Prototypes
void *student_thread(void *num);
void *teacher_thread();
void *inspire_thread();
void do_research();
void teach_class();

int main()
{
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

    time_t sometime;
    srand((unsigned)time(&sometime));

    pthread_create(&teacher, NULL, teacher_thread, (void *)NULL);

    pthread_create(&inspiration, NULL, inspire_thread, (void *)NULL);

    for (int i = 0; i < STUDENT_SIZE; i++)
    {
        int *counter = malloc(sizeof(int *));
        *counter = i;

        // Create student threads
        pthread_create(&student_threads[i], NULL, student_thread, (void *)counter);
    }

    // Wait for all threads to terminate (rare case) by joining
    pthread_join(teacher, NULL);
    pthread_join(inspiration, NULL);
    for(int i = 0; i < STUDENT_SIZE; i++)
    {
        pthread_join(student_threads[i], NULL);
    }
    
    // Exit process killing all threads
    exit(3);
}

void *student_thread(void *num)
{
    // Get id
    int id = *((int *)num);
    while (1)
    {
        int option = (rand() % 2);
        if (option)
        {
            // Go to class
            pthread_mutex_lock(&students_in_classroom_mutex);
            students_in_classroom++;
            pthread_mutex_unlock(&students_in_classroom_mutex);
            printf("student_thread[%d] : in the classroom (%d)\n", id, students_in_classroom);
            
            // Check if class is full to inform teacher
            if (students_in_classroom == STUDENT_SIZE)
            {
                pthread_mutex_lock(&waiting_for_class_to_start_mutex);
                waiting_for_class_to_start = 0;
                pthread_mutex_unlock(&waiting_for_class_to_start_mutex);

                pthread_cond_broadcast(&teacher_sleeping_cond);
            }

            // Wait for class to start
            pthread_cond_wait(&waiting_for_class_to_start_cond, &waiting_for_class_to_start_mutex);

            // Begin learning
            printf("student_thread[%d] : learning\n", id);
            pthread_mutex_unlock(&waiting_for_class_to_start_mutex);

            // Wait for class to be dismissed
            pthread_cond_wait(&class_dismissed_cond, &class_dismissed_mutex);
            pthread_mutex_unlock(&class_dismissed_mutex);
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

void *teacher_thread()
{
    while (1)
    {
        printf("teacher_thread : sleeping\n");

        // Wait to be woken up
        pthread_cond_wait(&teacher_sleeping_cond, &teacher_sleeping_mutex);
        teacher_sleeping = 0;

        // Action when woken by students
        if (!pthread_mutex_trylock(&waiting_for_class_to_start_mutex))
        {
            if (waiting_for_class_to_start == 0)
            {
                teach_class();
            }

            pthread_mutex_unlock(&waiting_for_class_to_start_mutex);
        }

        // Action when woken by inspiration
        if (!pthread_mutex_trylock(&lightning_strikes_mutex))
        {
            if (lightning_strikes == 1)
            {
                do_research();
            }

            pthread_mutex_unlock(&lightning_strikes_mutex);
        }
    }

    return NULL;
}

void *inspire_thread()
{
    while (1)
    {
        time_t wait_time = (rand() % (INSPIRE_MAX - INSPIRE_MIN + 1)) + INSPIRE_MIN;
        sleep(wait_time);

        // Check if teacher sleeping
        if (!pthread_mutex_trylock(&teacher_sleeping_mutex))
        {
            // Confirm teacher is sleeping
            if (teacher_sleeping == 1)
            {
                // Release lock so teacher can wakeup
                pthread_mutex_unlock(&teacher_sleeping_mutex);

                printf("inspiration_thread : lightning strikes!\n");
                pthread_mutex_lock(&lightning_strikes_mutex);
                lightning_strikes = 1;
                pthread_mutex_unlock(&lightning_strikes_mutex);

                pthread_cond_signal(&teacher_sleeping_cond);
            }
            else
            {
                // Release lock due to teacher already being awake, thus
                // lightning being unable to strike
                pthread_mutex_unlock(&teacher_sleeping_mutex);
            }
        }
    }

    return NULL;
}

void teach_class()
{
    // Teaching started
    printf("teacher_thread : teaching\n");
    waiting_for_class_to_start = 1;
    pthread_mutex_unlock(&waiting_for_class_to_start_mutex);

    // Class in session
    pthread_mutex_lock(&class_dismissed_mutex);
    class_dismissed = 0;
    pthread_mutex_unlock(&class_dismissed_mutex);

    //Inform students they are about to learn
    pthread_cond_broadcast(&waiting_for_class_to_start_cond);
    time_t wait_time = (rand() % (TEACHER_TEACH_MAX - TEACHER_TEACH_MIN + 1)) + TEACHER_TEACH_MIN;
    sleep(wait_time);

    // Reset student count in class
    pthread_mutex_lock(&students_in_classroom_mutex);
    students_in_classroom = 0;
    pthread_mutex_unlock(&students_in_classroom_mutex);

    // Prepared to sleep
    teacher_sleeping = 1;
    pthread_mutex_unlock(&teacher_sleeping_mutex);

    // Dismiss class
    printf("teacher_thread : class dismissed\n");
    pthread_mutex_lock(&class_dismissed_mutex);
    class_dismissed = 1;
    pthread_cond_broadcast(&class_dismissed_cond);
    pthread_mutex_unlock(&class_dismissed_mutex);
}

void do_research()
{
    printf("teacher_thread : doing research\n");
    time_t wait_time = (rand() % (TEACHER_RESEARCH_MAX - TEACHER_RESEARCH_MIN + 1)) + TEACHER_RESEARCH_MIN;
    sleep(wait_time);

    // Reset lightning indicator
    lightning_strikes = 0;

    // Check if lightning struck, woke teacher, then class became full
    // prior to class starting
    if (!pthread_mutex_trylock(&waiting_for_class_to_start_mutex))
    {
        if (waiting_for_class_to_start == 0)
        {
            teach_class();
        }

        pthread_mutex_unlock(&waiting_for_class_to_start_mutex);
    }
}