/*
Team 14
    Name    :   Umang Maurya
    Roll no :   055

    Name    :   Amardeep Singh(Lata)
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

char first[50][50];
char follow[50][50];
char table[50][50];
char reduced[50][50];
typedef struct state{
    int count;
    char production[200][200];
    int prodCount[200];
} State;



/***************************************function declartion*********************************/

void readInput();
void writeOutput();
void printOutPut();
void makeCanonical();
void solveSlr();
void checkingTandNT();
int checkInTerminal(char);
int checkInNonTerminal(char);


/****************************************Main File*****************************************/
int main(){

    printf("---------------------------------------------------------------\n");
    printf("------------------------SLR Parser-----------------------------\n");
    printf("---------------------------------------------------------------\n\n");
    State initial,temp;

    
    readInput(&initial);
    if(initial.count==0){printf("No input found \nExitting....\n"); return 1;}
    checkingTandNT();
    
}

/*********************************function defination*************************************/
void readInput(State *x){
    FILE *file = fopen(input, "r");
    char c = '-';
    int progress=1;
    int idDetected=0;
    int arrowDetected=0;
    x->count=0;
    printf("reading input...\n");
    while (c != EOF) {
        progress=1;
        x->prodCount[x->count]=0;
        while(progress){
            c=getc(file);
            if(c==10){
                if(x->prodCount[x->count]!=0) x->count++;
                x->prodCount[x->count]=0;
            }else if(c==-1){
                if(x->prodCount[x->count]!=0)x->count++;
                progress=0;
            }else if(c==' ') continue;
            else{
                if(c=='i') idDetected=1;
                if(c=='-') arrowDetected=1;
                if(c=='d' && idDetected) x->production[x->count][--x->prodCount[x->count]]=59; //for id
                else if(c=='>' && arrowDetected) x->production[x->count][--x->prodCount[x->count]]='"';
                else{
                    x->production[x->count][x->prodCount[x->count]]=c;
                }
                if(idDetected && c!='i') idDetected=0;
                if(arrowDetected && c!='-') arrowDetected=0;
                x->prodCount[x->count]++;
            }
        }
    }
    fclose(file);
    char temp;
    int t;
    for(int i=0; i<x->count; i++){
        printf("\t\t");
        for(int j=0; j<x->prodCount[i]; j++){
            temp=x->production[i][j];
            if(temp==';') printf("id");
            else if(temp==34) printf("->");
            else printf("%c",temp);

            if(temp!='|' && temp!=34){
                t=checkInTerminal(temp);
                if(t==-1){
                    t=checkInNonTerminal(temp);
                    if(t==0) non_terminal[count_non_terminal++]=temp;
                }else if(t==0) terminal[count_terminal++]=temp;
            }
        }
        printf("\n");
    }
    printf("%d Production Found.\n",x->count);
    return ;
}
void checkingTandNT(State *x){
    printf("checking terminal...\n");
    for(int i=0; i<count_terminal; i++) printf("\t%c\n",terminal[i]);
    printf("%d terminal found\n",count_terminal);
    printf("checking non terminal...\n");
    for(int i=0; i<count_non_terminal; i++){ if(non_terminal[i]!=';') printf("\t%c\n",non_terminal[i]); else printf("\tid\n");}
    printf("%d non terminal found\n",count_non_terminal);
}



int checkInTerminal(char val){
    if(val<='Z' && val>='A'){
        for(int i=0; i<count_terminal; i++) if(terminal[i]==val) return 1;
        return 0;
    }
     return -1;
}

int checkInNonTerminal(char val){
    if(checkInTerminal(val)==-1){
        for(int i=0; i<count_non_terminal; i++) if(non_terminal[i]==val) return 1;
        return 0;
    }
    return -1;
}

void writeOutput(){//for writing output in file

}

void printOutput(){ //for displaying output in monitor

}

void solveSlr(){

}

void makeCanonical(){

}


