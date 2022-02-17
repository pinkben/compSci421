/*******************************************************************************
 * Description: Program for reading in a binary program file as input, 
 * translating the input code to assembly code and outputting the resulting to 
 * a text file. 
 * Author: Demetrios Green
 * Date: 02-08-2022
*******************************************************************************/

#include <stdio.h>
#include <err.h>


int main(int argc, char* argv[])
{
	FILE* inputFile;
    FILE* outputfile;
    const int maxFileSize = 16;
    unsigned char inputBuffer[maxFileSize];

    inputFile = fopen(argv[1], "rb");

    if (!inputFile) {
        printf("The file could not be opened.\n");
        return 1;
    }

    printf("Opened the file!\n");

    fread(&inputBuffer, sizeof(short), maxFileSize, inputFile);

    for(int i = 0; i < maxFileSize; i ++)
    {
        printf("%d\n", inputBuffer[i]);
    }

    fclose(inputFile);

	return 0;
}

void decode_instruction(char instruction[],
                        char* op,
                        char* ri,
                        char* rj,
                        char* rk,
                        short* imm,
                        char* addr)
{

}

int execute_program(char program[], int number_instructions)
{
	
}