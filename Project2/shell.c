/*******************************************************************************
 * Description: Shell code
 * Author: Demetrios Green & Ben Pink
 * Date: 03-30-2022
 *******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>		// pipe/fork/dup2
#include <sys/wait.h> // waitpid
#include <fcntl.h>
#include "shellHistory.h"
#include "shellCommandList.h"

int MAX_INPUT_SIZE = 100;
int tokenize(char line[], char *tokens[], char delim[]);
int validateToken(char token[]);
int executeCommands(char *commands[], ShellHistory *shellHistory);


int main(int argc, char *argv[])
{
	char input[MAX_INPUT_SIZE + 1];
	int done = 0;
	int numberOfTokens;
	char *arguments[10];
	ShellHistory *history = initializeShellHistory();

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
			arguments[10 - 1] = NULL;
			done = executeCommands(arguments, history);
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
            return 0;
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

// Function to handle execution of commands
// I think this is where that example code from class will live
int executeCommands(char *commands[], ShellHistory *shellHistory)
{
    if (strcmp(commands[0], "quit") == 0)
	{
        return 1;
    } else if (strcmp(commands[0], "hist") == 0)
	{
		char **historyList = getShellHistory(shellHistory);
		int historySize = getShellHistorySize(shellHistory);
		if(historyList)
		{
			for(int i = 0; i < historySize; i++)
			{
				printf("%d %s\n", historySize - i, historyList[i]);
			}
		}
	} else if (strcmp(commands[0], "r") == 0)
		{
			char **historyList = getShellHistory(shellHistory);
			int histSize = getShellHistorySize(shellHistory);
			int histSelect = atoi(commands[1]);
			char *args[] = { historyList[histSize - histSelect], NULL};

			if(fork() == 0) {
				execvp(historyList[histSize - histSelect], args);
				exit(1);
			} else {
				wait(NULL);
			}
		} else { 
		addShellHistory(shellHistory, commands[0]);

		// for(int i = 0; i < 3; i++)
		// {
		// 	printf("Command at %d: %s\n", i, commands[i]);
		// }
		//////////////////////////////////////////////////////////////
		ShellCommandList *commandList = initializeShellCommandList();
		int i = 0;
		while (strcmp(commands[i], "\0") != 0)
		{
			printf("About to add token.\n");
			addCommandToken(commandList, commands[i]);
			i++;
		}
			printf("Midway.\n");
		char *currToken = getNextToken(commandList);
		while (currToken != NULL)
		{
			printf("The commandList value %s\n", currToken);
			currToken = getNextToken(commandList);
		}
		/////////////////////////////////////////////////////////////////
		if(fork() == 0)
		{ 
			// printf("We are about to execute.\n");
			// int status_code = 
			execvp(commands[0], commands);
			// if (status_code == -1)
			// {
			// 	printf("We did not terminate correctly.\n");
			// 	return 0;
			// }
			exit(1);
		} else
		{
			wait(NULL);
		}
	}
    return 0;
}