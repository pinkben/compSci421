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
unsigned char registers[16]; //Registers
unsigned char memory[32]; 
unsigned short int programCounter;
unsigned short int instructionRegister;
unsigned int numberOfInstructions;
unsigned char inputBuffer[32];
char* op;
char* ri;
char* rj;
char* rk;
short* imm;
char* addr;
char *instructions[] = {"LDI", "ADD", "AND", "OR",
                        "XOR", "PRT", "RDD", "BLE"};

unsigned char mask_operand(unsigned char instructionBinary) {
    return instructionBinary >> 5;
}

unsigned char maskRi(unsigned char instructionBinary) {
    return (instructionBinary & 30) >> 1;
}

unsigned char maskRj(unsigned char instructionBinary1, unsigned char instructionBinary2) {
    if((instructionBinary1 & 1) == 1)
    {
        return (instructionBinary2 >> 5) + 8;
    }

    return instructionBinary2 >> 5;
}

unsigned char maskRk(unsigned char instructionBinary) {
    return (instructionBinary & 30) >> 1;
}

unsigned char maskAddress(unsigned short instructionBinary) {
    return instructionBinary & 31;
}

unsigned short maskImm(unsigned char instructionBinary1,
                       unsigned char instructionBinary2) {

    if((instructionBinary1 & 1) == 1){
        return (instructionBinary2 >> 1) + 128;
    }

    return instructionBinary2 >> 1;
}

unsigned int read_file(FILE *file) {

    if (!file) {
        printf("The file could not be opened.\n");
        return 1;
    }

    return fread(&inputBuffer, sizeof(char), MAXFILESIZE, file);
}

void output_instructions(unsigned char inputBufferElement1,
                         unsigned char inputBufferElement2,
                         int i,
                         FILE *outputFile) {
    // We need to grab the values from the decode fucntion and use them here.
    // So we shouldnt need to pass anything other then the outputfile and i.
    if(!outputFile) {
        printf("We were not able to access the file.\n");
        exit(1);
    }

    fprintf(outputFile, "%d: ", i);
    switch(mask_operand(inputBufferElement1)) {
        case 0:
            fprintf(outputFile, "%s R%d %d\n", instructions[0], 
                                               maskRi(inputBufferElement1),
                                               maskImm(inputBufferElement1, inputBufferElement2));
            break;
        case 1:
            fprintf(outputFile, "%s R%d R%d R%d\n", instructions[1],
                                       maskRi(inputBufferElement1),
                                       maskRj(inputBufferElement1, inputBufferElement2),
                                       maskRk(inputBufferElement2));
            break;
        case 2:
            fprintf(outputFile, "%s R%d R%d R%d\n", instructions[2],
                                       maskRi(inputBufferElement1),
                                       maskRj(inputBufferElement1, inputBufferElement2),
                                       maskRk(inputBufferElement2));
            break;
        case 3:
            fprintf(outputFile, "%s R%d R%d R%d\n", instructions[3],
                                       maskRi(inputBufferElement1),
                                       maskRj(inputBufferElement1, inputBufferElement2),
                                       maskRk(inputBufferElement2));
            break;
        case 4:
            fprintf(outputFile, "%s R%d R%d R%d\n", instructions[4],
                                       maskRi(inputBufferElement1),
                                       maskRj(inputBufferElement1, inputBufferElement2),
                                       maskRk(inputBufferElement2));
            break;
        case 5:
            fprintf(outputFile, "%s R%d\n", instructions[5],
                                            maskRi(inputBufferElement1));
            break;
        case 6:
            fprintf(outputFile, "%s R%d\n", instructions[6],
                                            maskRi(inputBufferElement1));
            break;
        case 7:
            fprintf(outputFile, "%s R%d R%d %d\n", instructions[7],
                                            maskRi(inputBufferElement1),
                                            maskRj(inputBufferElement1, inputBufferElement2),
                                            maskAddress(inputBufferElement2));
            break;
        default:
            fprintf(outputFile, "Uknown operand provided. Continuing...\n");
    }
}

// Instead of masking, just look at what values are sitting in decode and use those.
void perform_ldi() {
    *ri = *imm;
}

void perform_add() {
    *rk = *ri + *rj;
}

void perform_and() {
    *rk = *ri & *rj;
}

void perform_or() {
    *rk = *ri | *rj;
}

void perform_xor() {
    *rk = *ri ^ *rj;
}

void perform_prt() {
    printf("%d\n", *ri);
}

void perform_rdd() {
    unsigned int input;
    scanf("%d", &input);
    *ri = (unsigned char) input;
}

void perform_ble() {
    if((*addr % 2) != 0) {
        printf("The address is invalid.\n");
        exit(1);
    }

    if(*ri <= *rj) {
        programCounter = (unsigned short) maskAddress(instructionRegister) - WORDSIZE;
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

       instructionRegister = instruction[0] << 8 | instruction [1];
        op = instructions[mask_operand(instructionRegister)];
        ri = &registers[maskRi(instruction[0])];
        rj = &registers[maskRj(instruction[0], instruction[1])];
        rk = &registers[maskRk(instruction[1])];
        imm = &instructionRegister;
        addr = &instruction[1];
}

int execute_program(char program[], int number_instructions) {
    // send the instruction to the decode
    // once it has been sent to decode, read the opcode 
    // then, go along and execute the function that corresponds to the opcode
    printf("We are about to copy memory.");
    for(int i = 0; i < number_instructions; i++) {
        memory[i] = program[i];
    }
    char instruction[2];
	for(programCounter = 0; programCounter < number_instructions; programCounter++) {
        instruction[0] = memory[programCounter];
        instruction[1] = memory[programCounter + 1];
        printf("About to decode...");
        decode_instruction(instruction, op, ri, rj, rk, imm, addr);
        printf("We have decoded %d times.", programCounter);
        switch(*op) {
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
    printf("We are about to open the file.");
    FILE *inputFile = fopen(argv[1], "rb");
    numberOfInstructions = read_file(inputFile);
    fclose(inputFile);

    printf("We have read the file.");

    
    FILE *outputfile = fopen(argv[2], "w");

    printf("We are about to output instructions.");
    for(int i = 0; i < numberOfInstructions; i+=WORDSIZE) {
        output_instructions(inputBuffer[i], inputBuffer[i + 1], i, outputfile);
    }
    
    
    fclose(outputfile);
    printf("We have closed the file.");

    execute_program(inputBuffer, numberOfInstructions);

	return 0;
}