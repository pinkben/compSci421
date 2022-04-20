#ifndef SHELL_COMMAND_LIST_H
#define SHELL_COMMAND_LIST_H

typedef struct commandToken CommandToken;
typedef struct shellCommandList ShellCommandList;

ShellCommandList *initializeShellCommandList();
int addCommandToken(ShellCommandList *commandList, char *token);
char *getNextToken(ShellCommandList *commandList);
char **getShellCommandList(ShellCommandList *commandList);
void freeShellCommandList(ShellCommandList *commandList);

#endif