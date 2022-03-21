/*******************************************************************************
 * Description: Shell code
 * Author: Demetrios Green & Ben Pink
 * Date: 03-30-2022
*******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <unistd.h> // pipe/fork/dup2
#include <sys/wait.h> // waitpid
#include <fcntl.h>

int MAX_INPUT_SIZE = 100;
int tokenize(char line[], char* tokens[], char delim[]);

int main(int argc, char* argv[])
{
    char input[MAX_INPUT_SIZE + 1];
    int done = 0;
    int numberOfTokens;
    char* arguments[10];
    printf("> ");
    // Read the input
    while (!done) {
        // Read input
        if (!fgets(input, MAX_INPUT_SIZE + 1, stdin)) {
            printf("Could not read command.\n");
            return 2;
        }
        // Remove newline
        input[MAX_INPUT_SIZE] = '\0';
        if (memchr(input, '\n', MAX_INPUT_SIZE + 1)) {
          *((char *) memchr(input, '\n', MAX_INPUT_SIZE + 1)) = '\0';
        }
        // Parse input
        numberOfTokens = tokenize(input, arguments, " ");
        if (numberOfTokens > 0)
        {
            done = 1;
        }

        printf("> ");
    }
    return 0;
}

int tokenize(char line[], char* tokens[], char delim[])
{
    return sizeof(tokens);
}