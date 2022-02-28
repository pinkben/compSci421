/*******************************************************************************
 * Description: Program for reading in a binary program file as input, 
 * translating the input code to assembly code and outputting the resulting to 
 * a text file. 
 * Author: Demetrios Green
 * Date: 02-08-2022
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>
#include <arpa/inet.h>

#define MAXFILESIZE 16
#define WORDSIZE 2

/* Global Variables */
unsigned short inputBuffer[MAXFILESIZE]; //Memory buffer
unsigned char reg[16]; // Registers
unsigned short int programCounter;
unsigned short int instructionRegister;
char *instructions[] = {"LDI", "ADD", "AND", "OR",
                        "XOR", "PRT", "RDD", "BLE"};

unsigned short maskOperand(unsigned short instructionBinary) {
    short operand = (instructionBinary & 57344) >> 13;
    return operand;
}

unsigned short maskRi(unsigned short instructionBinary){
    unsigned short registerNum = (instructionBinary & 7680)  >> 9;
    return registerNum;
}

unsigned short maskRj(unsigned short instructionBinary){
    unsigned short registerNum = (instructionBinary & 480) >> 5;
    return registerNum;
}

unsigned short maskRk(unsigned short instructionBinary){
    unsigned short registerNum = (instructionBinary & 30)  >> 1;
    return registerNum;
}

unsigned short maskAddress(unsigned short instructionBinary){
    unsigned short registerNum = instructionBinary & 31;
    return registerNum;
}

unsigned short maskImm(unsigned short instructionBinary) {
    unsigned short immediateValue = (instructionBinary & 510) >> 1;
    return immediateValue;
}

unsigned int readFile(FILE *file) {

    if (!file) {
        printf("The file could not be opened.\n");
        return 1;
    }

    int numOfInstructions = fread(&inputBuffer, 
                                  sizeof(short), MAXFILESIZE, file);

    return numOfInstructions;
}

void outputInstructions(unsigned short inputBufferElement, int i, FILE *outputFile) {
    
    if(!outputFile) {
        printf("We were not able to access the file.\n");
        exit(1);
    }

    short operand = maskOperand(inputBufferElement);

    fprintf(outputFile, "%d: ", i * WORDSIZE);
    switch(operand) {
        case 0:
            fprintf(outputFile, "%s R%d %d\n", instructions[0], 
                                               maskRi(inputBufferElement),
                                               maskImm(inputBufferElement));
            break;
        case 1:
            fprintf(outputFile, "%s R%d R%d R%d\n", instructions[1],
                                       maskRi(inputBufferElement),
                                       maskRj(inputBufferElement),
                                       maskRk(inputBufferElement));
            break;
        case 2:
            fprintf(outputFile, "%s R%d R%d R%d\n", instructions[2],
                                       maskRi(inputBufferElement),
                                       maskRj(inputBufferElement),
                                       maskRk(inputBufferElement));
            break;
        case 3:
            fprintf(outputFile, "%s R%d R%d R%d\n", instructions[3],
                                       maskRi(inputBufferElement),
                                       maskRj(inputBufferElement),
                                       maskRk(inputBufferElement));
            break;
        case 4:
            fprintf(outputFile, "%s R%d R%d R%d\n", instructions[4],
                                       maskRi(inputBufferElement),
                                       maskRj(inputBufferElement),
                                       maskRk(inputBufferElement));
            break;
        case 5:
            fprintf(outputFile, "%s R%d\n", instructions[5],
                                            maskRi(inputBufferElement));
            break;
        case 6:
            fprintf(outputFile, "%s R%d\n", instructions[6],
                                            maskRi(inputBufferElement));
            break;
        case 7:
            fprintf(outputFile, "%s R%d R%d %d\n", instructions[7],
                                            maskRi(inputBufferElement),
                                            maskRj(inputBufferElement),
                                            maskAddress(inputBufferElement));
            break;
        default:
            fprintf(outputFile, "Uknown operand provided. Continuing...\n");
    }
}

void decode_instruction(char instruction[],
                        char* op,
                        char* ri,
                        char* rj,
                        char* rk,
                        short* imm,
                        char* addr) {

}

int execute_program(char program[], int number_instructions) {
	
}

int main(int argc, char* argv[])
{
    if(argc != 3) {
        printf("There was an invalid number of arguments. Try again.\n");
        exit(1);
    }

    memset(inputBuffer, 0 , sizeof(inputBuffer));
    //remove me after makefile is complete

    FILE *inputFile = fopen(argv[1], "rb");
    int numOfInstructions = readFile(inputFile);
    fclose(inputFile);

    FILE *outputfile = fopen(argv[2], "w");
    for(int i = 0; i < numOfInstructions; i++){
        outputInstructions(ntohs(inputBuffer[i]), i, outputfile);
    }
    fclose(outputfile);

	return 0;
}


