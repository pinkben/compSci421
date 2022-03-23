#include <stdlib.h>
#include <string.h>
#include "shellHistory.h"

#define SHELL_HISTORY_LIMIT 5

struct shellHistoryNode
{
    char *shellCommand;
    struct historyNode *next;
    struct historyNode *previous;
};

struct shellHistory
{
    ShellHistoryNode *head;
    ShellHistoryNode *tail;
    int size;
    int limit;
};

/**
 * @brief Creates a new shellHistory on the heap
 * 
 * @return ShellHistory* 
 */
ShellHistory *initializeShellHistory()
{
    //Allocate space for shell history queue
    ShellHistory *shellHistory = (ShellHistory *) malloc(sizeof(ShellHistory));

    //Handle null reference case
    if (shellHistory == NULL)
    {
        return NULL;
    }

    //Initialize members
    shellHistory->head = NULL;
    shellHistory->tail = NULL;
    shellHistory->size = 0;
    shellHistory->limit = SHELL_HISTORY_LIMIT;

    return shellHistory;
}

/**
 * @brief Adds a new shell command to the shell history, removes first in if
 * size limit reached. Also does not add if size limit is set 0.
 * 
 * @param shellHistory to be added to
 * @param shellCommand to be added
 * @return int 1 if added, 0 if not added
 */
int addShellHistory(ShellHistory *shellHistory, char *shellCommand)
{
    //Allocate space for shellHistoryNode
    ShellHistoryNode *shellHistoryNode = 
        (ShellHistoryNode *) malloc (sizeof(ShellHistoryNode));
    if (!shellHistoryNode) //Handle possible allocation issues
    {
        return 0;
    }

    //Set shellHistoryNode shellCommandValue with pointer
    shellHistoryNode->shellCommand = strdup(shellCommand);
    //Handle strdup errors when assigning pointer to shellCommand member and
    //check that size limit is not set to 0
    if (!shellHistoryNode->shellCommand || shellHistory->limit == 0) {
        return 0;
    }

    //Case when shell history is empty
    if (shellHistory->size == 0)
    {
        shellHistoryNode->previous = NULL;
        shellHistoryNode->next = NULL;
        shellHistory->head = shellHistoryNode;
        shellHistory->tail = shellHistoryNode;
    }

    //Case when shell history is full
    else if (shellHistory->size == shellHistory->limit)
    {
        //Remove head node, set new head, reduce size
        ShellHistoryNode *newHead = shellHistory->head->next;
        free(shellHistory->head);
        newHead->previous = NULL;
        shellHistory->head = newHead;
        shellHistory->size--;
    }

    //Add to tail
    shellHistory->tail->next = shellHistoryNode;
    shellHistoryNode->previous = shellHistory->tail;
    shellHistoryNode->next = NULL;
    shellHistory->tail = shellHistoryNode;
    shellHistory->size++;
    return 1;
}

/**
 * @brief Return shellHistory in a string array
 * 
 * @param shellHistory 
 * @return char** pointer to a string array
 */
char **getShellHistory(ShellHistory *shellHistory)
{
    //Allocate space for the string array with NULL termination character.
    char **shellCommandArray = (char **) malloc(shellHistory->size * sizeof(char *) + 1);
    if(!shellCommandArray) //Handle possible allocation issue
    {
        return NULL;
    }

    shellCommandArray[shellHistory->size] = NULL; //Set last value to NULL
    ShellHistoryNode *currentNode = NULL; //Node pointer to pull command from

    if (shellHistory->size == 0) //Early return
    {
        return shellCommandArray;
    }

    currentNode = shellHistory->head; //Start with oldest history at head
    for(int i = 0; i < shellHistory->size; i++)
    {
        shellCommandArray[i] = currentNode->shellCommand;
        currentNode = currentNode->next;
    }

    return shellCommandArray;
}

/**
 * @brief Deallocates memory for shellHistory and nodes within
 * 
 * @param shellHistory 
 */
void clearShellHistory(ShellHistory *shellHistory)
{
    ShellHistoryNode *nodeToDelete;
    while(shellHistory->size > 0)
    {
        nodeToDelete = shellHistory->head;
        shellHistory->head = nodeToDelete->next;
        free(nodeToDelete);
        shellHistory->size--;
    }

    free(shellHistory);
}

/**
 * @brief Get the Shell History Size object
 * 
 * @param shellHistory 
 * @return int size of shell history
 */
int getShellHistorySize(ShellHistory *shellHistory)
{
    return shellHistory->size;
}