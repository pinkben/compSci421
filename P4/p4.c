/*******************************************************************************
 * Description: P4
 * Author: Demetrios Green & Ben Pink
 * Date: 05-11-2022
 *******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

// TODO: Not sure if we are going to need this struct considering our output
typedef struct
{
  // Tell scheduling algorithm the amount of time
  // remaining until the program finishes
  unsigned long timeRemaining;
  // Time at which the process finished executing
  unsigned long finishTime;
  // The time the program took to finish
  unsigned long turnaroundTime;
  // The time the process spent idle
  unsigned long latencyTime;
} processStats;

/* Stores proccess information */
typedef struct
{
  // Process ID number
  unsigned long pid;
  // Time at which process arrived
  unsigned long arrivalTime;
  // Process duration
  unsigned long burstTime;
  // Process scheduled time
  unsigned long scheduledTime;
  // Process statistics
  processStats stats;
} process;

typedef struct
{
  // Number of processes in the system
  unsigned long numberOfProcesses;
  // Array of processes in the system
  process *process;
  // Round robin time quantum
  unsigned long timeQuantum;
  // MLFQ S value
  unsigned long mlfqS;
} processInfo;

processInfo *parse(FILE *file);
void print_queue_helper(process *proc, unsigned long time);
void releaseQueue(int* procQueue, int *size);
void push(int queue[], int *size, int item);
void delete (int queue[], int *size, int index);
int pop(int queue[], int *size);
void run_fifo(processInfo *proc_ctl_blk);
void run_sjf(processInfo *proc_ctl_blk);
void run_rr(processInfo *proc_ctl_blk);

int main(int argc, char *argv[])
{
  // Make sure there's only one argument
  if (argc != 2)
  {
    fprintf(stderr, "Invalid number of arguments\n");
    fprintf(stderr, "Usage: p4 (process information file)\n");
    return EXIT_FAILURE;
  }

  // Parse process information file
  FILE *process_input_data = fopen(argv[1], "r");
  if (!process_input_data)
  {
    perror("Unable to open process information file");
    return EXIT_FAILURE;
  }
  fprintf(stderr, "Reading processes in %s ...", argv[1]);
  processInfo *procControlBlk = parse(process_input_data);
  fclose(process_input_data);
  fprintf(stderr, "Done\n");

  // Run the processes here using the
  // different algorithms
  // run_fifo(procControlBlk);
  run_sjf(procControlBlk);

  // Cleaning up
  free(procControlBlk->process);
  free(procControlBlk);
  return EXIT_SUCCESS;
}

// Function for parsing input file
processInfo *parse(FILE *file)
{
  // Initialize process control block and
  // fill-ins for its numerical components
  unsigned long numProcess = 0;
  unsigned long timeQuantum = 0;
  unsigned long mlfqS = 0;
  processInfo *procControlBlk = (processInfo *)malloc(sizeof(processInfo));

  // Find number of processes
  fscanf(file, "%lu\n", &numProcess);
  procControlBlk->numberOfProcesses = numProcess;

  // Allocate space for process array
  procControlBlk->process = (process *)malloc(numProcess * sizeof(process));

  // read process info and fill process array
  for (int i = 0; i < numProcess; i++)
  {
    unsigned long arrivalTime = 0;
    unsigned long burstTime = 0;
    fscanf(file, "%lu,%lu\n", &arrivalTime, &burstTime);
    procControlBlk->process[i].pid = i;
    procControlBlk->process[i].arrivalTime = arrivalTime;
    procControlBlk->process[i].burstTime = burstTime;
  }

  // Find time quantum and MLFQ S value
  fscanf(file, "%lu\n%lu", &timeQuantum, &mlfqS);
  procControlBlk->timeQuantum = timeQuantum;
  procControlBlk->mlfqS = mlfqS;

  // Return filled process control block
  return procControlBlk;
}

void push(int queue[], int *size, int item)
{
  queue[(*size)++] = item;
}

void delete (int queue[], int *size, int index)
{ // Assumes queue is not empty and
  // index is valid
  int i;
  (*size)--;
  for (i = index; i < *size; i++)
  {
    queue[i] = queue[i + 1];
  }
}

int pop(int queue[], int *size)
{ // Assumes queue is not empty
  int temp = queue[0];
  delete (queue, size, 0);
  return temp;
}

void run_fifo(processInfo *proc_ctl_blk) {
  // Print header
  printf("Simulating FIFO...");

  // Initialize overall statistic fields
  double avg_turnaround = 0;
  double avg_latency = 0;
  int size = 0;

  // Make sure there are processes to work with
  if (proc_ctl_blk && (proc_ctl_blk->numberOfProcesses > 0)) {

    // Create and populate a process queue
    // and initialize the time_remaining field
    // queue *proc_queue = queue_init();
    int *proc_queue = (int*) malloc(sizeof(int) * proc_ctl_blk->numberOfProcesses);
    int *proc_size;
    proc_size = &size;

    for (int i = 0; i < proc_ctl_blk->numberOfProcesses; i++) {
      process *p = &proc_ctl_blk->process[i];
      p->stats.timeRemaining = p->burstTime;
      // queue_enqueue(proc_queue, (void *) p);
      push(proc_queue, proc_size, p->pid);
    }
    
    // Run the processes in the process queue
    // and calculate stats
    for (int time = 0; *proc_size > 0; time++) {
      // process *p = (process *) queue_peek(proc_queue);
      process *p = &proc_ctl_blk->process[proc_queue[0]];
      // printf("Process queue length is: %d\n", *proc_size);
      if ((p->arrivalTime <= time) && (p->stats.timeRemaining)) 
      {
        if(p->scheduledTime == 0 && p->pid != 0)
        {
          p->scheduledTime = time;
        }
        p->stats.timeRemaining--;
      } else {
        // queue_dequeue(proc_queue);
        
        pop(proc_queue, proc_size);
        print_queue_helper(p, time);
        avg_turnaround += p->stats.turnaroundTime;
        avg_latency += p->stats.latencyTime;
        time--;
      }
    }
    avg_turnaround /= proc_ctl_blk->numberOfProcesses;
    avg_latency /= proc_ctl_blk->numberOfProcesses;

    // Free process queue
    releaseQueue(proc_queue, proc_size);
  // Print statistics
  printf("  Average turnaround time: %lf\n", avg_turnaround);
  printf("  Average latency:         %lf\n", avg_latency);
  printf("Done\n");
  }
  else if ((proc_ctl_blk->numberOfProcesses == 0))
  {
    printf("No jobs to do.\n");
  }
}

void run_sjf(processInfo *proc_ctl_blk)
{
    // Print header
  printf("Simulating SJF...");

  // Initialize overall statistic fields
  // double avg_turnaround = 0;
  // double avg_latency = 0;
  // int size = 0;
  //   // Make sure there are processes to work with
  // if (proc_ctl_blk && (proc_ctl_blk->numberOfProcesses > 0)) {

  //   // Create and populate a process queue
  //   // and initialize the time_remaining field
  //   int *proc_queue = (int*) malloc(sizeof(int) * proc_ctl_blk->numberOfProcesses);
  //   int *proc_size;
  //   proc_size = &size;

  //   for (int i = 0; i < proc_ctl_blk->numberOfProcesses; i++) {
  //     process *p = &proc_ctl_blk->process[i];
  //     p->stats.timeRemaining = p->burstTime;
  //     push(proc_queue, proc_size, p->pid);
  //   }
    
  //   // Run the processes in the process queue
  //   // and calculate stats
  //   for (int time = 0; *proc_size > 0; time++) {
  //     process *p = &proc_ctl_blk->process[proc_queue[0]];
  //     for(int i = 1; i < *proc_size; i++)
  //     {
  //       process *q = &proc_ctl_blk->process[proc_queue[i]];
  //       if (p->burstTime > q->burstTime)
  //       {
  //           p = q;
  //       }
  //     }
  //     if ((p->arrivalTime <= time) && (p->stats.timeRemaining)) 
  //     {
  //       if(p->scheduledTime == 0 && p->pid != 0)
  //       {
  //         p->scheduledTime = time;
  //       }
  //       p->stats.timeRemaining--;
  //     } else {
  //       // queue_dequeue(proc_queue);
        
  //       pop(proc_queue, proc_size);
  //       print_queue_helper(p, time);
  //       avg_turnaround += p->stats.turnaroundTime;
  //       avg_latency += p->stats.latencyTime;
  //       time--;
  //     }
  //   }
  //   avg_turnaround /= proc_ctl_blk->numberOfProcesses;
  //   avg_latency /= proc_ctl_blk->numberOfProcesses;


  //////////////////////////////////////////////////////////
double avg_turnaround = 0;
  double avg_latency = 0;

  // Make sure there are processes to work with
  if (proc_ctl_blk && (proc_ctl_blk->numberOfProcesses > 0)) {

    // Create and populate a process array
    process *proc_array[proc_ctl_blk->numberOfProcesses];
    for (int i = 0; i < proc_ctl_blk->numberOfProcesses; i++) {
      process *p = &proc_ctl_blk->process[i];
      p->stats.timeRemaining = p->burstTime;
      proc_array[i] = p;
    }

    // Run the processes in the process array
    // and calculate stats
    process *shortest = NULL;
    unsigned long shortest_index = 0;
    unsigned long procs_remaining = proc_ctl_blk->numberOfProcesses;
    for (int time = 0; procs_remaining; time++) {
      // Remove shortest job from the array if it's done
      if (shortest && !(shortest->stats.timeRemaining)) {
        print_queue_helper(shortest, time);
        avg_turnaround += shortest->stats.turnaroundTime;
        avg_latency += shortest->stats.latencyTime;
        proc_array[shortest_index] = NULL;
        shortest = NULL;
        shortest_index = 0;
        procs_remaining--;
      }
      // Find shortest process for given time
      for (int i = 0; i < proc_ctl_blk->numberOfProcesses; i++) {
        process *p = proc_array[i];
        if (p && (p->arrivalTime <= time)) {
          if (!shortest) {
            shortest = p;
            shortest_index = i;
          } else if ((shortest->burstTime) > (p->burstTime)) {
              shortest = p;
              shortest_index = i;
          }
        }
      }
      // Execute shortest job for one unit of time
      if (shortest && (shortest->stats.timeRemaining > 0)) {
        if(shortest->scheduledTime == 0 && time == 0)
        {
          shortest->scheduledTime = time;
        } else if(shortest->scheduledTime == 0 && time != 0)
        {
          shortest->scheduledTime = time;
        }
        shortest->stats.timeRemaining--;
      } else {
        time--;
      }
    }
    avg_turnaround /= proc_ctl_blk->numberOfProcesses;
    avg_latency /= proc_ctl_blk->numberOfProcesses;

    // Free process queue
    // releaseQueue(proc_array, proc_ctl_blk->numberOfProcesses);
  
    ////////////////////////////////////////////////////////////////

    // Print statistics
    printf("  Average turnaround time: %lf\n", avg_turnaround);
    printf("  Average latency:         %lf\n", avg_latency);
    printf("Done\n");
  }
  else if ((proc_ctl_blk->numberOfProcesses == 0))
  {
    printf("No jobs to do.\n");
  }
}

void run_rr(processInfo *proc_ctl_blk)
{
  // Print header
  printf("Simulating Round Robin...");

  // Initialize overall statistic fields
  double avg_turnaround = 0;
  double avg_latency = 0;
  int size = 0;

  // Make sure there are processes to work with
  if (proc_ctl_blk && (proc_ctl_blk->numberOfProcesses > 0)) {

    // Create and populate a process queue
    // and initialize the time_remaining field
    // queue *proc_queue = queue_init();
    int *proc_queue = (int*) malloc(sizeof(int) * proc_ctl_blk->numberOfProcesses);
    int *proc_size;
    proc_size = &size;
    int timeElasped = 0;

    for (int i = 0; i < proc_ctl_blk->numberOfProcesses; i++) {
      process *p = &proc_ctl_blk->process[i];
      p->stats.timeRemaining = p->burstTime;
      // queue_enqueue(proc_queue, (void *) p);
      push(proc_queue, proc_size, p->pid);
    }
    
    // Run the processes in the process queue
    // and calculate stats
    for (int time = 0; *proc_size > 0; time++) {
      process *p = &proc_ctl_blk->process[proc_queue[0]];
      if ((p->arrivalTime <= time) && (p->stats.timeRemaining) && (timeElasped < proc_ctl_blk->timeQuantum)) {
        if(p->scheduledTime == 0 && p->pid != 0)
        {
          p->scheduledTime = time;
        }
        p->stats.timeRemaining--;
        timeElasped++;
      } 
      else {
        // queue_dequeue(proc_queue);
        if (p->stats.timeRemaining > 0)
        {
            push(proc_queue, proc_size, p->pid);
            pop(proc_queue, proc_size);
        }
        else
        {
            pop(proc_queue, proc_size);
            print_queue_helper(p, time);
            avg_turnaround += p->stats.turnaroundTime;
            avg_latency += p->stats.latencyTime;
        }
        time--;
        timeElasped = 0;
      }
    }
    avg_turnaround /= proc_ctl_blk->numberOfProcesses;
    avg_latency /= proc_ctl_blk->numberOfProcesses;

    // Free process queue
    releaseQueue(proc_queue, proc_size);
  // Print statistics
  printf("  Average turnaround time: %lf\n", avg_turnaround);
  printf("  Average latency:         %lf\n", avg_latency);
  printf("Done\n");
  }
  else if ((proc_ctl_blk->numberOfProcesses == 0))
  {
    printf("No jobs to do.\n");
  }
}

void print_queue_helper(process *proc, unsigned long time)
{
  proc->stats.finishTime = time;
  proc->stats.turnaroundTime = time - proc->arrivalTime;
  proc->stats.latencyTime = proc->stats.turnaroundTime - proc->burstTime;

  printf("Process %lu: ", proc->pid);
  printf("arrival=%lu", proc->arrivalTime);
  printf(", burst=%lu", proc->burstTime);
  printf(", scheduled=%lu", proc->scheduledTime);
  printf(", finish=%lu", proc->stats.finishTime);
  printf(", turnaround=%lu", proc->stats.turnaroundTime);
  printf(", latency=%lu", proc->stats.latencyTime);
  printf(", response=%lu\n", proc->scheduledTime - proc->arrivalTime);
}

void releaseQueue(int* procQueue, int *size)
{
  free(procQueue);
}