/*

Group 14 :   CONSTRUCTION OF THE **SLR PARSER** 
 
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
char non_terminal[100],terminal[200],first[100][50],follow[100][50];
int count_non_terminal=0,count_terminal=0,count_first[100],count_follow[100];

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
void augmenting();
void solveSlr();
void checkingTandNT();
int checkInTerminal(char);
int checkInNonTerminal(char);
void printProduction();
void findFirst(State*,char );

/****************************************Main File*****************************************/
int main(){

    printf("---------------------------------------------------------------\n");
    printf("------------------------SLR Parser-----------------------------\n");
    printf("---------------------------------------------------------------\n");
    State initial,temp,var;
    readInput(&initial);
    printProduction(&initial);
    printf("%d Production Found.\n",initial.count);
    temp=initial;
    var=initial;
    if(initial.count==0){printf("No input found \nExitting....\n"); return 1;}
    checkingTandNT(&initial);
    for(int i=0;i<initial.count; i++){
        for (int j=initial.prodCount[i]; j>1; j--)  initial.production[i][j] = initial.production[i][j-1];
        initial.production[i][1]='.';
        initial.prodCount[i]++;
    }
    char starter='A';
    while(checkInTerminal(starter)==1) starter++; //finding initial
    //shifting
    for(int i=initial.count-1; i>0; i--){
        for(int j=0; j<initial.prodCount[i]; j++) initial.production[i+1][j]=initial.production[i][j];
        initial.prodCount[i+1]=initial.prodCount[i];
    }
    initial.production[0][2]=initial.production[0][0];
    initial.production[0][0]=starter;
    initial.production[0][1]='.';
    initial.prodCount[0]=3;
    printf("I0-> \n");
    //printProduction(&initial);


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
                else if(c=='>' && arrowDetected) x->prodCount[x->count]-=2;
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
        for(int j=0; j<x->prodCount[i]; j++){
            temp=x->production[i][j];
            if(temp!='|'){
                t=checkInTerminal(temp);
                if(t==-1){
                    t=checkInNonTerminal(temp);
                    if(t==0) non_terminal[count_non_terminal++]=temp;
                }else if(t==0) terminal[count_terminal++]=temp;
            }
        }
    }
    return ;
}
void checkingTandNT(State *x){
    //finding first and follow
    for(int i=0; i<count_terminal; i++){
        count_first[i]=-1;
        count_follow[i]=-1;
    }

    for(int i=0; i<count_terminal; i++){
        findFirst(x,terminal[i]);
        //printf("%d--\n",count_first[i]);
       // count_follow[i]=0;
    }


    printf("checking terminal...\tFirst\t\tFollow\n");
    for(int i=0; i<count_terminal; i++){
        printf("\t%c\t\t",terminal[i]);
        for(int j=0;j<count_first[i]; j++) printf("%c",first[i][j]);
        printf("\t\t");
        for(int j=0;j<count_follow[i]; j++) printf("%c",follow[j]);
        printf("\n");
    }
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
void printProduction(State *x){
    char temp;
    for(int i=0; i<x->count; i++){
        printf("\t\t");
        for(int j=0; j<x->prodCount[i]; j++){
            temp=x->production[i][j];
            if(temp==';') printf("id");
            else printf("%c",temp);
            if(j==0) printf("->");
        }
        printf("\n");
    }
}



//unsolved
void augmenting(){

}
void writeOutput(){//for writing output in file

}

void printOutput(){ //for displaying output in monitor

}

void solveSlr(){

}

void findFollow(State *x,char val){
    
}

void findFirst(State *x,char val){
    int flag=0;
    int pos=0;
    for(pos=0; pos<count_terminal; pos++) if(terminal[pos]==val) break;
    if(count_first[pos]!=-1) return;
    int temp,temp2;
    count_first[pos]=0;
    for(int i=0; i<x->count; i++){
        if(x->production[i][0]==val){
            flag=1;
            for(int j=1; j<x->prodCount[i]; j++){
                if(x->production[i][j]=='|') flag=1;
                else{
                    if(flag==1){
                        flag=0;
                        temp=checkInTerminal(x->production[i][j]);
                        if(temp==1){
                            findFirst(x,x->production[i][j]);
                            for(int m=0; m<count_terminal; m++) if(terminal[m]==x->production[i][j]){temp2=m; break;}
                            for(int m=0; m<count_first[temp2]; m++){
                                first[pos][count_first[pos]++]=first[temp2][m];
                            }
                        }else{
                            first[pos][count_first[pos]++]=x->production[i][j];
                        }
                    }
                }
            }
        }
    }
}


