/*******************************************************************************
 * Description: Shell code
 * Author: Demetrios Green & Ben Pink
 * Date: 03-30-2022
 *******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <unistd.h>		// pipe/fork/dup2
#include <sys/wait.h> // waitpid
#include <fcntl.h>
#include "shellHistory.h"
#include "shellCommandList.h"

int MAX_INPUT_SIZE = 100;
int tokenize(char line[], char *tokens[], char delim[]);
int validateToken(char token[]);

int main(int argc, char *argv[])
{
	char input[MAX_INPUT_SIZE + 1];
	int done = 0;
	int numberOfTokens;
	char *arguments[10];
	printf("> ");
	// Read the input
	while (!done)
	{
		// Read input
		if (!fgets(input, MAX_INPUT_SIZE + 1, stdin))
		{
			printf("Could not read command.\n");
			return 2;
		}
		// Remove newline
		input[MAX_INPUT_SIZE] = '\0';
		if (memchr(input, '\n', MAX_INPUT_SIZE + 1))
		{
			*((char *)memchr(input, '\n', MAX_INPUT_SIZE + 1)) = '\0';
		}
		// Parse input
		numberOfTokens = tokenize(input, arguments, " \t\r\n");
		if (numberOfTokens > 0)
		{
			done = 1;
		}

		printf("> ");
	}
	return 0;
}

int tokenize(char line[], char *tokens[], char delim[])
{
    int i = 0;
    // Get the first token
    char *token = strtok(line, delim);
    while(token != NULL ) {
        // Add the token to the list of tokens and continue
        // until we hit the end of the list
        if(validateToken(token)){
            tokens[i] = token;
            token = strtok(NULL, delim);
            i++;
        }
        else
        {
            // What should we do if we hit an invalid token? freeCommandList?
            return NULL;
        }
    }
    return i;
}

// Not sure what we want to do here, I think the idea is if there is more than
// one "token" in a command, make sure the middle token is a < | & 
int validateToken(char token[])
{
    return 1;
}