#ifndef SHELL_HISTORY_H
#define SHELL_HISTORY_H

// typedef struct shellHistoryNode ShellHistoryNode;
typedef struct shellHistory ShellHistory;

ShellHistory *initializeShellHistory();
int addShellHistory(ShellHistory *shellHistory, char *shellCommand);
char **getShellHistory(ShellHistory *shellHistory);
void clearShellHistory(ShellHistory *shellHistory);
int getShellHistorySize(ShellHistory *shellHistory);

#endif