/*
Team 14
    Name    :   Umang Maurya
    Roll no :   055

    Name    :   Amardeep
    Roll no :   009

    Name    :   Yateesh Chandra
    Roll no :   057

    Name    :   Sachin Verma
    Roll no :   038

    */

#include<stdio.h>
#define input "Input.txt"
#define output "Output.txt"

/***************************************variable declartion**********************************/
char non_terminal[100];
int count_non_terminal=0;

char terminal[200];
int count_terminal=0;

int no_of_production=0;


/***************************************function declartion*********************************/

void readInput();
void writeOutput();
void printOutPut();
void makeCanonical();
void solveSlr();


/****************************************Main File*****************************************/
int main(){

    printf("---------------------------------------------------------------\n");
    printf("------------------------SLR Parser-----------------------------\n");
    printf("---------------------------------------------------------------\n\n");

    printf("reading input...\t\t");
    readInput();
    printf("completed\n");
    



}

/*********************************function defination*************************************/
void readInput(){ //for reading input from file
    FILE *f = fopen(input, "r");
    int c = getc(f);
    while (c != EOF) {
        putchar(c);
        c = getc(f);
    }
}

void writeOutput(){//for writing output in file

}

void printOutput(){ //for displaying output in monitor

}

void solveSlr(){

}

void makeCanonical(){

}


