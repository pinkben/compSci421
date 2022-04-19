#include <stdlib.h>
#include <string.h>
#include "shellHistory.h"

#define SHELL_HISTORY_LIMIT 5

typedef struct shellHistoryNode
{
	char *shellCommand;
	struct shellHistoryNode *next;
	struct shellHistoryNode *previous;
} ShellHistoryNode;

typedef struct shellHistory
{
	ShellHistoryNode *head;
	ShellHistoryNode *tail;
	int size;
	int limit;
} ShellHistory;

/**
 * @brief Creates a new shellHistory on the heap
 *
 * @return ShellHistory*
 */
ShellHistory *initializeShellHistory()
{
	// Allocate space for shell history queue
	ShellHistory *currentShellHistory = (ShellHistory *)malloc(sizeof(ShellHistory));

	// Handle null reference case
	if (currentShellHistory == NULL)
	{
		return NULL;
	}

	// Initialize members
	currentShellHistory->head = NULL;
	currentShellHistory->tail = NULL;
	currentShellHistory->size = 0;
	currentShellHistory->limit = SHELL_HISTORY_LIMIT;

	return currentShellHistory;
}

/**
 * @brief Adds a new shell command to the shell history, removes first in if
 * size limit reached. Also does not add if size limit is set 0.
 *
 * @param currentShellHistory to be added to
 * @param shellCommand to be added
 * @return int 1 if added, 0 if not added
 */
int addShellHistory(ShellHistory *currentShellHistory, char *shellCommand)
{
	// Allocate space for shellHistoryNode
	ShellHistoryNode *shellHistoryNode =
			(ShellHistoryNode *)malloc(sizeof(ShellHistoryNode));
	if (!shellHistoryNode) // Handle possible allocation issues
	{
		return 0;
	}

	// Set shellHistoryNode shellCommandValue with pointer
	shellHistoryNode->shellCommand = strdup(shellCommand);
	// Handle strdup errors when assigning pointer to shellCommand member and
	// check that size limit is not set to 0
	if (!shellHistoryNode->shellCommand || currentShellHistory->limit == 0)
	{
		return 0;
	}

	// Case when shell history is empty
	if (currentShellHistory->size == 0)
	{
		shellHistoryNode->previous = NULL;
		shellHistoryNode->next = NULL;
		currentShellHistory->head = shellHistoryNode;
		currentShellHistory->tail = shellHistoryNode;
	}

	// Case when shell history is full
	else if (currentShellHistory->size == currentShellHistory->limit)
	{
		// Remove head node, set new head, reduce size
		ShellHistoryNode *newHead = currentShellHistory->head->next;
		free(currentShellHistory->head);
		newHead->previous = NULL;
		currentShellHistory->head = newHead;
		currentShellHistory->size--;
	}

	// Add to tail
	currentShellHistory->tail->next = shellHistoryNode;
	shellHistoryNode->previous = currentShellHistory->tail;
	shellHistoryNode->next = NULL;
	currentShellHistory->tail = shellHistoryNode;
	currentShellHistory->size++;
	return 1;
}

/**
 * @brief Return shellHistory in a string array
 *
 * @param currentShellHistory
 * @return char** pointer to a string array
 */
char **getShellHistory(ShellHistory *currentShellHistory)
{
	// Allocate space for the string array with NULL termination character.
	char **shellCommandArray =
			(char **)malloc(currentShellHistory->size * sizeof(char *) + 1);
	if (!shellCommandArray) // Handle possible allocation issue
	{
		return NULL;
	}

	shellCommandArray[currentShellHistory->size] = NULL; // Set last value to NULL
	ShellHistoryNode *currentNode = NULL;					// Node pointer to pull command from

	if (currentShellHistory->size == 0) // Early return
	{
		return shellCommandArray;
	}

	currentNode = currentShellHistory->head; // Start with oldest history at head
	for (int i = 0; i < currentShellHistory->size; i++)
	{
		shellCommandArray[i] = currentNode->shellCommand;
		currentNode = currentNode->next;
	}

	return shellCommandArray;
}

/**
 * @brief Deallocates memory for shellHistory and nodes within
 *
 * @param currentShellHistory
 */
void clearShellHistory(ShellHistory *currentShellHistory)
{
	ShellHistoryNode *nodeToDelete;
	while (currentShellHistory->size > 0)
	{
		nodeToDelete = currentShellHistory->head;
		currentShellHistory->head = nodeToDelete->next;
		free(nodeToDelete);
		currentShellHistory->size--;
	}

	free(currentShellHistory);
}

/**
 * @brief Get the Shell History Size object
 *
 * @param currentShellHistory
 * @return int size of shell history
 */
int getShellHistorySize(ShellHistory *currentShellHistory)
{
	return currentShellHistory->size;
}