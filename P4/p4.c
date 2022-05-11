/*******************************************************************************
 * Description: P4
 * Author: Demetrios Green & Ben Pink
 * Date: 05-11-2022
 *******************************************************************************/

#include <stdio.h>
#include <stdlib.h>

//TODO: Not sure if we are going to need this struct considering our output
typedef struct {
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
typedef struct {
  // Process ID number
  unsigned long pid;
  // Time at which process arrived
  unsigned long arrivalTime;
  // Process duration
  unsigned long burstTime;
  // Process statistics
  processStats stats;
} process;

typedef struct {
  // Number of processes in the system
  unsigned long numProcs;
  // Array of processes in the system
  process *processs;
  // Round robin time quantum
  unsigned long timeQuantum;
  // MLFQ S value
  unsigned long mlfqS;
} processInfo;

processInfo *parse(FILE *file);

int main(int argc, char *argv[]) {
  // Make sure there's only one argument
  if (argc != 2) {
    fprintf(stderr, "Invalid number of arguments\n");
    fprintf(stderr, "Usage: p4 (process information file)\n");
    return EXIT_FAILURE;
  }

  // Parse process information file
  FILE *processInfo = fopen(argv[1], "r");
  if (!processInfo) {
    perror("Unable to open process information file");
    return EXIT_FAILURE;
  }
  processInfo *procControlBlk = parse(processInfo);
  fclose(processInfo);

  // Run the processes here using the
  // different algorithms


  // Cleaning up
  free(procControlBlk->process);
  free(procControlBlk);
  return EXIT_SUCCESS;
}

// Function for parsing input file
processInfo *parse(FILE *file) {
  // Initialize process control block and
  // fill-ins for its numerical components
  unsigned long numProcess = 0;
  unsigned long timeQuantum = 0;
  unsigned long mlfqS = 0;
  processInfo *procControlBlk = (processInfo *) malloc(sizeof(processInfo));

  // Find number of processes
  fscanf(file, "%lu\n", &numProcess);
  procControlBlk->numProcess = numProcess;

  // Allocate space for process array
  procControlBlk->process = (process *) malloc(numProcess * sizeof(process));

  // read process info and fill process array
  for (int i = 0; i < numProcess; i++) {
    unsigned long arrivalTime = 0;
    unsigned long burstTime = 0;
    fscanf(file, "%lu,%lu\n", &arrivalTime, &burstTime);
    procControlBlk->process[i].pid = i;
    procControlBlk->process[i].time_arrival = arrivalTime;
    procControlBlk->process[i].time_burst = burstTime;
  }

  // Find time quantum and MLFQ S value
  fscanf(file, "%lu\n%lu", &timeQuantum, &mlfqS);
  procControlBlk->timeQuantum = timeQuantum;
  procControlBlk->mlfqS = mlfqS;

  // Return filled process control block
  return procControlBlk;
}