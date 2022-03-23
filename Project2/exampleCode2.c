/*******************************************************************************
 * Description: Updated sample code provided in class
 * Author: Demetrios Green & Ben Pink
 * Date: 03-30-2022
*******************************************************************************/

#include <stdio.h>
#include <unistd.h> // pipe/fork/dup2
#include <sys/wait.h> // waitpid
const int READ = 0;
const int WRITE = 1;
const int NUM_PIPES = 2;
void close_pipes(int pipes[][2])
{
   int i;
   for (i = 0; i < NUM_PIPES; i++)
   {
      close(pipes[i][READ]);
      close(pipes[i][WRITE]);
   }
}
int main() 
{
   int pipes[NUM_PIPES][2];
   int pid, status;
   pipe(pipes[0]);
   pipe(pipes[1]);
   pid = fork();
   if (pid == 0)
   {
      // Left child writes into WRITE end of first pipe:
      dup2(pipes[0][WRITE], STDOUT_FILENO);
      // Close each end of all pipes:
      close_pipes(pipes);
      // Read a string from stdin, print to stdout:
      char str[100];
      scanf("%s", str);
      printf("%s", str);
   }
   else
   {
      pid = fork();
      if (pid == 0)
      {
         // Middle child reads from READ end of first pipe
         dup2(pipes[0][READ], STDIN_FILENO);
         // Middle child writes to WRITE end of second pipe
         dup2(pipes[1][WRITE], STDOUT_FILENO);
         // Close each end of all pipes
         close_pipes(pipes);
         // Read a string from stdin, double the string to stdout:
         char str[100];
         scanf("%s", str);
         printf("%s%s", str, str);
      }
      else
      {
         pid = fork();
         if (pid == 0)
         {
            // Right child reads from READ end of second pipe:
            dup2(pipes[1][READ], STDIN_FILENO);
            // Close each end of all pipes 
            close_pipes(pipes);
            // Read a string from stdin, print to stdout:
            char str[100];
            scanf("%s", str);
            printf("%s", str);
         }
         else
         {
            // Parent
            // Close each end of all pipes
            close_pipes(pipes);
            // Wait until the last child terminates:
            waitpid(pid, &status, 0);  // Or, maybe don't wait?
         }
      }
   }
}