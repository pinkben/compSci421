/*******************************************************************************
 * Description: Program for reading in a binary program file as input, 
 * translating the input code to assembly code and outputting the resulting to 
 * a text file. 
 * Author: Demetrios Green & Ben Pink
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
unsigned short inputBuffer[MAXFILESIZE]; // Memory buffer
unsigned char reg[16]; // Registers
unsigned short int programCounter;
unsigned short int instructionRegister;
char *instructions[] = {"LDI", "ADD", "AND", "OR",
                        "XOR", "PRT", "RDD", "BLE"};
int numOfInstructions;

unsigned short mask_operand(unsigned short instructionBinary) {
    unsigned short operand = instructionBinary  >> 13;
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

unsigned int read_file(FILE *file) {

    if (!file) {
        printf("The file could not be opened.\n");
        return 1;
    }

    int numOfInstructions = fread(&inputBuffer, 
                                  sizeof(short), MAXFILESIZE, file);

    return numOfInstructions;
}

void output_instructions(unsigned short inputBufferElement, int i, FILE *outputFile) {
    
    if(!outputFile) {
        printf("We were not able to access the file.\n");
        exit(1);
    }

    short operand = mask_operand(inputBufferElement);

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

void perform_ldi(unsigned short instructionBinary) {
    reg[maskRi(instructionBinary)] = maskImm(instructionBinary);
}

void perform_add(unsigned short instructionBinary) {
    reg[maskRk(instructionBinary)] = reg[maskRi(instructionBinary)] + reg[maskRj(instructionBinary)];
}

void perform_and(unsigned short instructionBinary) {
    reg[maskRk(instructionBinary)] = reg[maskRi(instructionBinary)] & reg[maskRj(instructionBinary)];
}

void perform_or(unsigned short instructionBinary) {
    reg[maskRk(instructionBinary)] = reg[maskRi(instructionBinary)] | reg[maskRj(instructionBinary)];
}

void perform_xor(unsigned short instructionBinary) {
    reg[maskRk(instructionBinary)] = reg[maskRi(instructionBinary)] ^ reg[maskRj(instructionBinary)];
}

void perform_prt(unsigned short instructionBinary) {
    printf("%d\n", reg[maskRi(instructionBinary)]);
}

void perform_rdd(unsigned short int instructionBinary) {
    unsigned int input;
    scanf("%d", &input);
    reg[maskRi(instructionBinary)] = (unsigned char) input;
}

void perform_ble(unsigned short instructionBinary) {
    if((maskAddress(instructionBinary) % 2) != 0) {
        printf("The address is invalid.\n");
        exit(1);
    }

    if(reg[maskRi(instructionBinary)] <= reg[maskRj(instructionBinary)]) {
        programCounter = (unsigned short) maskAddress(instructionBinary) / 2;
        printf("Program counter new value:%d\n", programCounter);
    }

    if(programCounter > numOfInstructions) {
        printf("Memory bounds has been violated.\n");
        exit(1);
    }
    
}

void decode_instruction(char instructions[],
                        char* op,
                        char* ri,
                        char* rj,
                        char* rk,
                        short* imm,
                        char* addr) {

}

int execute_program(unsigned short program[], int number_instructions) {
	for(programCounter = 0; programCounter < number_instructions; programCounter++) {
        instructionRegister = ntohs(inputBuffer[programCounter]);
        unsigned short currentInstruction = mask_operand(instructionRegister);
        switch(currentInstruction) {
            case 0:
                perform_ldi(instructionRegister);
                break;
            case 1:
                perform_add(instructionRegister);
                break;
            case 2:
                perform_and(instructionRegister);
                break;
            case 3:
                perform_or(instructionRegister);
                break;
            case 4:
                perform_xor(instructionRegister);
                break;
            case 5:
                perform_prt(instructionRegister);
                break;
            case 6:
                perform_rdd(instructionRegister);
                break;
            case 7:
                perform_ble(instructionRegister);
                break;
            default:
                printf("Invalid instruction encountered within memory.\n");
                exit(1);
        }
    }
}

int main(int argc, char* argv[])
{
    /* Argument count check */
    if(argc != 3) {
        printf("There was an invalid number of arguments. Try again.\n");
        exit(1);
    }

    memset(inputBuffer, 0 , sizeof(inputBuffer));
    //remove me after makefile is complete

    FILE *inputFile = fopen(argv[1], "rb");
    numOfInstructions = read_file(inputFile);
    printf("Number of instructions: %d\n", numOfInstructions);
    fclose(inputFile);

    FILE *outputfile = fopen(argv[2], "w");
    for(int i = 0; i < numOfInstructions; i++){
        output_instructions(ntohs(inputBuffer[i]), i, outputfile);
    }
    fclose(outputfile);

    execute_program(inputBuffer, numOfInstructions);

	return 0;
}


