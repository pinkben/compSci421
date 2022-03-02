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

#define MAXFILESIZE 32
#define WORDSIZE 2

/* Global Variables */
unsigned char inputBuffer[MAXFILESIZE]; // Memory buffer <- can probably remove this
unsigned char registers[16]; //Registers
unsigned char memory[32]; 
unsigned short int programCounter;
unsigned short int instructionRegister;
char* op;
char* ri;
char* rj;
char* rk;
short* imm;
char* addr;
char *instructions[] = {"LDI", "ADD", "AND", "OR",
                        "XOR", "PRT", "RDD", "BLE"};

unsigned char mask_operand(unsigned char instructionBinary) {
    unsigned char operand = instructionBinary  >> 4;
    return operand;
}

unsigned char maskRi(unsigned char instructionBinary){
    unsigned char registerNum = (instructionBinary & 60)  >> 2;
    return registerNum;
}

unsigned char maskRj(unsigned char instructionBinary1, unsigned char instructionBinary2){
    if((instructionBinary1 & 1) == 1)
    {
        unsigned char registerNum = (instructionBinary2 & >> 5) + 8;
    }
    else
    {
        unsigned char registerNum = instructionBinary2 >> 5;
    }
    return registerNum;
}

unsigned char maskRk(unsigned char instructionBinary){
    unsigned char registerNum = (instructionBinary & 30) >> 1;
    return registerNum;
}

unsigned char maskAddress(unsigned short instructionBinary){
    unsigned short registerNum = instructionBinary & 31;
    return registerNum;
}

unsigned char maskImm(unsigned char instructionBinary) {
    unsigned char immediateValue = (instructionBinary & 510) >> 1;
    return immediateValue;
}

unsigned int read_file(FILE *file) {

    if (!file) {
        printf("The file could not be opened.\n");
        return 1;
    }

    int numOfInstructions = fread(&memory, 
                                  sizeof(char), MAXFILESIZE, file);

    return numOfInstructions;
}

void output_instructions(unsigned char inputBufferElement, int i, FILE *outputFile) {
    // We need to grab the values from the decode fucntion and use them here.
    // So we shouldnt need to pass anything other then the outputfile and i.
    if(!outputFile) {
        printf("We were not able to access the file.\n");
        exit(1);
    }

    char operand = mask_operand(inputBufferElement);

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

// Instead of masking, just look at what values are sitting in decode and use those.
void perform_ldi(unsigned char instructionBinary) {
    reg[maskRi(instructionBinary)] = maskImm(instructionBinary);
}

void perform_add(unsigned char instructionBinary) {
    reg[maskRk(instructionBinary)] = reg[maskRi(instructionBinary)] + reg[maskRj(instructionBinary)];
}

void perform_and(unsigned char instructionBinary) {
    reg[maskRk(instructionBinary)] = reg[maskRj(instructionBinary)] & reg[maskRj(instructionBinary)];
}

void perform_or(unsigned char instructionBinary) {
    reg[maskRk(instructionBinary)] = reg[maskRi(instructionBinary)] | reg[maskRj(instructionBinary)];
}

void perform_xor(unsigned char instructionBinary) {
    reg[maskRk(instructionBinary)] = reg[maskRi(instructionBinary)] ^ reg[maskRj(instructionBinary)];
}

void perform_prt(unsigned char instructionBinary) {
    printf("%d\n", reg[maskRi(instructionBinary)]);
}

void perform_rdd(unsigned char instructionBinary) {
    unsigned int input;
    scanf("%d", &input);
    reg[maskRi(instructionBinary)] = (unsigned char) input;
}

void perform_ble(unsigned short instructionBinary) {
    if((maskImm(instructionBinary) % 2) != 0) {
        printf("The address is invalid.\n");
        exit(1);
    }

    if(reg[maskRi(instructionBinary)] < reg[maskRj(instructionBinary)]) {
        programCounter = (unsigned short) maskAddress(instructionBinary) - WORDSIZE;
    }
}

void decode_instruction(char instruction[],
                        char* op,
                        char* ri,
                        char* rj,
                        char* rk,
                        short* imm,
                        char* addr) {
    // We are going to fill all of these will whatever value is in the instruction.  
    // Then when we print/execute, we will only grab the varibles we need
    op = mask_operand(instruction[0]);
    ri = maskRi(instruction[0]);
    rj = maskRj(instruction[0], instruction[1]);
    rk = maskRk(instruction[1]);
    imm = maskImm(instruction[1]);
    addr = maskAddress(instruction[1]);
}

int execute_program(char program[], int number_instructions) {
    // send the instruction to the decode
    // once it has been sent to decode, read the opcode 
    // then, go along and execute the function that corresponds to the opcode
	for(programCounter = 0; programCounter < number_instructions; programCounter++) {
        instructionRegister = inputBuffer[programCounter];
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

    memset(memory, 0 , sizeof(memory));
    //remove me after makefile is complete

    FILE *inputFile = fopen(argv[1], "rb");
    int numOfInstructions = read_file(inputFile);
    fclose(inputFile);

    FILE *outputfile = fopen(argv[2], "w");
    for(int i = 0; i < numOfInstructions && i < 32; i+=WORDSIZE){
        // Scott said no more then 16 instructions
        char firstHalf = inputBuffer[i];
        char secondHalf = inputBuffer[i + 1];
        char instruction[] = {firstHalf,secondHalf};
        decode_instruction(instruction, op, ri, rj, rk, imm, addr);
        //output_instructions(inputBuffer[i], i, outputfile);
    
    fclose(outputfile);

    execute_program(inputBuffer, numOfInstructions);

	return 0;
}


