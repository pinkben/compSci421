#include <stdlib.h>
#include <string.h>
#include "shellCommandList.h"

struct commandToken
{
	char *tokenValue;
	struct commandToken *next;
};

struct shellCommandList
{
	struct commandToken *head;
	struct commandToken *tail;
};

/**
 * @brief Creates a ShellCommandList struct
 *
 * @return ShellCommandList*
 */
ShellCommandList *initializeShellCommandList()
{
	// Allocate space for shellCommand list on the heap
	ShellCommandList *shellCommandList =
			(ShellCommandList *)malloc(sizeof(ShellCommandList));
	// Handle possible allocation issue
	if (!shellCommandList)
	{
		return NULL;
	}

	shellCommandList->head = NULL;
	shellCommandList->tail = NULL;
}

/**
 * @brief Adds a command token node to ShellCommandList
 *
 * @param commandList to be added to
 * @param token to be added
 * @return int 1 if added, 0 otherwise
 */
int addCommandToken(ShellCommandList *commandList, char *token)
{
	CommandToken *commandToken = (CommandToken *)malloc(sizeof(CommandToken));
	if (!commandToken)
	{
		return 0;
	}

	commandToken->tokenValue = strdup(token);

	commandToken->next = NULL;
	if (!commandList->head) // Empty command list case
	{
		commandList->head = commandToken;
		commandList->tail = commandToken;
	}
	else
	{ // Non-empty command list case
		commandList->tail->next = commandToken;
		commandList->tail = commandToken;
	}

	return 1;
}

/**
 * @brief Get the next commandToken in ShellCommandList and removes it
 *
 * @param commandList to be removed from
 * @return char* commandToken tokenValue
 */
char *getNextToken(ShellCommandList *commandList)
{
	if (!commandList->head) // Case when commandList is empty
	{
		return NULL;
	}

	char *token = commandList->head->tokenValue;
	// Remove and return commandToken when one commandToken exists
	if (commandList->head == commandList->tail)
	{
		commandList->head = NULL;
		commandList->tail = NULL;

		return token;
	}

	// Remove head commandToken and set next commandToken as head
	CommandToken *nextHead = commandList->head->next;
	free(commandList->head);
	commandList->head = nextHead;

	return token;
}

/**
 * @brief Get the ShellCommandList tokens in string form as an array
 *
 * @param commandList to be converted to string array
 * @return char** array of an array of characters (string array)
 */
char **getShellCommandList(ShellCommandList *commandList)
{
	CommandToken *token = commandList->head;

	if (!token) // Handle empty commandList
	{
		return NULL;
	}

	// Allocate memory for tokenArray
	char **tokenArray = (char **)malloc(sizeof(char *));
	if (!tokenArray) // Handle possible memory allocation issues
	{
		return NULL;
	}

	int arrayTokenCounter = 0;
	for (int offsetCounter = 2 * sizeof(char *); !token; offsetCounter += sizeof(char *))
	{
		tokenArray[arrayTokenCounter] = token->tokenValue;
		arrayTokenCounter++;

		// Shrink memory allocation of token locations to size of actual token
		tokenArray = (char **)realloc(tokenArray, offsetCounter);
		if (!tokenArray) // Handle possible realloc issues
		{
			return NULL;
		}

		token = token->next;
	}

	// Add NULL termination
	tokenArray[arrayTokenCounter] = NULL;

	return tokenArray;
}

/**
 * @brief Release ShellCommandList and CommandTokens from memory
 *
 * @param commandList list to be released
 */
void freeShellCommandList(ShellCommandList *commandList)
{
	while (commandList->head)
	{
		free(getNextToken(commandList)); // Release commandTokens from memory
	}

	free(commandList); // Release commandList from memory
}