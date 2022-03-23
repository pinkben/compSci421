/*******************************************************************************
 * Description: Original sample code provided with lab
 * Author: Demetrios Green & Ben Pink
 * Date: 03-30-2022
 *******************************************************************************/

#include <string.h>
#include <unistd.h>		// pipe/fork/dup2
#include <sys/wait.h> // waitpid

const int READ = 0;
const int WRITE = 1;
const int NUM_PIPES = 2;
int pipes[NUM_PIPES][2];
int pid, i, status;
pipe(pipes[0]);
pipe(pipes[1]);
pid = fork();
if (pid == 0)
{
	// Left child writes into WRITE end of first pipe:
	dup2(pipes[0][WRITE], STDOUT_FILENO);
	for (i = 0; i < NUM_PIPES; i++) // Close each end of all pipes
	{
		close(pipes[i][READ]);
		close(pipes[i][WRITE]);
	}
	// call exec* ??
}
else
{
	pid = fork();
	if (pid == 0)
	{
		// Middle child reads from READ end of first pipe
		// Middle child writes to WRITE end of second pipe
		// Close each end of all pipes
		// call exec*
	}
	else
	{
		pid = fork();
		if (pid == 0)
		{
			// Right child reads from READ end of second pipe:
			dup2(pipes[1][READ], STDIN_FILENO);
			// Maybe duplicate STDOUT as an arbitrary file?
			// dup2(??, STDOUT_FILENO);
			// Close each end of all pipes
			// call exec*
		}
		else
		{
			// Parent
			// Close each end of all pipes
			// Wait until the last child terminates:
			waitpid(pid, &status, 0); // Or, maybe don’t wait?
		}
	}
}