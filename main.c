/*
Team 14
    Name    :   Umang Maurya
    Roll no :   055

    Name    :   Amardeep Singh
    Roll no :   009

    Name    :   Yateesh Chandra
    Roll no :   057

    Name    :   Sachin Verma
    Roll no :   038


    */

#include<stdio.h>
#include<string.h>
#define input "Input.txt"
#define epsilon '@'

/***************************************variable declartion**********************************/
char non_terminal[100], terminal[200],first[100][50],follow[100][50],reduce_table[50][50];
int count_non_terminal=0,count_terminal=0,count_first[100],count_follow[100],count_state=1,edges[400][3],count_edge=0;


struct state{
    int count;
    char production[200][200];
    int prodCount[200];
    int completed;
} states[100],initial;

typedef struct state State;



/***************************************function declartion*********************************/

void readInput();
void checkingTandNT();
int checkInTerminal(char);
int checkInNonTerminal(char);
void printProduction();
void findFirst(char );
void findFollow(char );
void addToFirst(char,int);
void addToFollow(char,int);
int findPositionInNT(char);
void pushInEdge(int ,char,int);
int compareState(int);
int printChar(char x){if(x==';') printf("id");else  printf("%c",x);}
void copyProduction(int ,int ,int );
void seprateStates(int );
void moveDot(int ,int ,char );

// char nextCharAfterDotArr(char arr[]);
// char  moveDot(char arr[],int len);



/****************************************Main File*****************************************/


int main(){

    printf("---------------------------------------------------------------\n");
    printf("------------------------SLR Parser-----------------------------\n");
    printf("---------------------------------------------------------------\n");
    readInput();
    printProduction(&initial);
    if(initial.count==0){printf("No input found \nExitting....\n"); return 1;}
    checkingTandNT(&initial);
    char starter='A';
    while(checkInTerminal(starter)==1) starter++; //finding initial
    states[0].count=1;
    states[0].production[0][2]=initial.production[0][0];
    states[0].production[0][0]=starter;
    states[0].production[0][1]='.';
    states[0].prodCount[0]=3;
    for(int i=0; i<initial.count; i++){
        for(int j=0; j<initial.prodCount[i]; j++){
            if(initial.production[i][j]=='|'){
                states[0].count++;
                states[0].prodCount[states[0].count]=1;
                states[0].production[states[0].count][0]=initial.production[i][0];
            }else{
                states[0].production[states[0].count][states[0].prodCount[states[0].count]++]=initial.production[i][j];
            }
        }
        states[0].count++;
    }


    for(int i=1;i<states[0].count; i++){
        for (int j=states[0].prodCount[i]; j>1; j--)  states[0].production[i][j] = states[0].production[i][j-1];
        states[0].production[i][1]='.';
        states[0].prodCount[i]++;
    }
    terminal[count_terminal++]='$';
    seprateStates(0);
    for(int i=0; i<count_state+1; i++){
        printf("I%d->\n",i);
        printProduction(&states[i]);
    }

    printf("Edges...\n");
    for(int i=0; i<count_edge; i++){
        printf("\tI%d -> ",edges[i][0]);
        printChar(edges[i][1]);
        printf(" -> I%d\n",edges[i][2]);
    }
        
    
    // printf("Parsing table ...\nState\t\taction\t\tgo_to\n\t");
}

/*********************************function defination*************************************/





void moveDot(int x,int s,char val){

    int flag=0;
    char temp;
    int where=0;
    int arr[states[s].count];
    //swap all data
    for(int i=0; i<states[s].count; i++){
        where=0;
        for(int r=1; r<states[s].prodCount[i]; r++)if(states[s].production[i][r]=='.'){ where=r+1; break;}
        temp=states[s].production[i][where];
        states[s].production[i][where]=states[s].production[i][where-1];
        states[s].production[i][where-1]=temp;
        arr[i]=where;
    }

    for(int i=0; i<states[s].count; i++){
         where=arr[i]+1;
        if(where<states[s].prodCount[i]){
            flag=1;
            copyProduction(count_state,s,i);
            temp=states[s].production[i][where];
            if(checkInNonTerminal(temp)==1){
                int fill=1;
                char temp2[20];
                temp2[0]=temp;

                for(int n=0; n<fill; n++){
                    for(int m=0; m<states[0].count; m++){
                        if(temp2[n]==states[0].production[m][0]){
                            copyProduction(count_state,0,m);
                            if(checkInNonTerminal(states[0].production[m][2])){
                                int matched=0;
                                for(int y=0; y<fill; y++) if(temp2[y]==states[0].production[m][2]) matched=1;
                                if(!matched) temp2[fill++]=states[0].production[m][2];
                            }
                        }
                    }
                }
            }
        }
    }
    
    if(flag==1){
        int xx=compareState(count_state);
        if(xx==-1){
            count_state++;
            seprateStates(count_state-1);
            pushInEdge(x,val,count_state);
        }else{
            pushInEdge(x,val,xx);
        }
    }else pushInEdge(x,val,s);
}



////////////////////////////done methods/////////////////////////////////////////////////
int checkInTerminal(char val){
    if(checkInNonTerminal(val)==-1){
        for(int i=0; i<count_terminal; i++) if(terminal[i]==val) return 1;
        return 0;
    }
     return -1;
}
int checkInNonTerminal(char val){
    if(val<='Z' && val>='A'){
        for(int i=0; i<count_non_terminal; i++) if(non_terminal[i]==val) return 1;
        return 0;
    }
    return -1;
}
void addToFirst(char val,int pos){
    for(int i=0; i<count_first[pos]; i++) if(first[pos][i]==val) return;
    first[pos][count_first[pos]++]=val;
}
void addToFollow(char val,int pos){
    for(int i=0; i<count_follow[pos]; i++) if(follow[pos][i]==val) return;
    follow[pos][count_follow[pos]++]=val;
}
void printProduction(State *x){
    char temp;
    for(int i=0; i<x->count; i++){
        printf("\t\t");
        for(int j=0; j<x->prodCount[i]; j++){
            temp=x->production[i][j];
            printChar(temp);
            if(j==0) printf("->");
        }
        printf("\n");
    }
    printf("\n");
}
int findPositionInNT(char val){
    for(int ret=0; ret<count_non_terminal; ret++) if(non_terminal[ret]==val)return ret;
    return 0;
}
void findFollow(char val){
    int flag=0;
    int pos=findPositionInNT(val);
    char temp;
    if(count_follow[pos]!=-1) return;
    count_follow[pos]=0;
    if(val==initial.production[0][0])    addToFollow('$',pos);
    for(int i=0; i<initial.count; i++){
        for(int j=1; j<initial.prodCount[i]; j++){
            if(val==initial.production[i][j]){
                temp=initial.production[i][j+1];
                if(temp=='|' || temp=='\0' || temp==epsilon){
                    if(temp!=val){
                        findFollow(temp);
                        int xxx=findPositionInNT(temp);
                        for(int p=0; p<count_follow[xxx]; p++) addToFollow(follow[xxx][p],pos);
                    }
                }else if(checkInNonTerminal(temp)==1){
                    int loop=1;
                    int nex=j+1;
                    while(loop){
                        temp=initial.production[i][nex++];
                        if(temp=='|' || temp=='\0'){
                            if(temp!=val){
                                findFollow(temp);
                                int xxx=findPositionInNT(temp);
                                for(int p=0; p<count_follow[xxx]; p++) addToFollow(follow[xxx][p],pos);
                            }
                            loop=0;
                        }else{
                            int ox=findPositionInNT(temp);
                            loop=0;
                            for(int k=0; k<count_first[ox]; k++){
                                if(first[ox][k]==epsilon) loop=1;
                                else addToFollow(first[ox][k],pos);
                            }
                        }
                    }
                }else addToFollow(temp,pos);
            }
        }
    }
}
void findFirst(char val){
    int flag=0;
    int pos=findPositionInNT(val);
    if(count_first[pos]!=-1) return;
    int temp,temp2;
    count_first[pos]=0;
    for(int i=0; i<initial.count; i++){
        if(initial.production[i][0]==val){
            flag=1;
            for(int j=1; j<initial.prodCount[i]; j++){
                if(initial.production[i][j]=='|') flag=1;
                else{
                    if(flag==1){
                        flag=0;
                        temp=checkInNonTerminal(initial.production[i][j]);
                        if(temp==1){
                            int loop=1;
                            int started=j;
                            while(loop){
                                loop=0;
                                findFirst(initial.production[i][started]);
                                temp2=findPositionInNT(initial.production[i][started]);
                                for(int m=0; m<count_first[temp2];m++){
                                    if(first[temp2][m]==epsilon) loop=1;
                                    addToFirst(first[temp2][m],pos);
                                }
                                if(loop) started++;

                            }
                        }else addToFirst(initial.production[i][j],pos);
                    }
                }
            }
        }
    }
}
void checkingTandNT(){
    //finding first and follow
    for(int i=0; i<count_non_terminal; i++){
        count_first[i]=-1;
        count_follow[i]=-1;
    }
    for(int i=0; i<count_non_terminal; i++) findFirst(non_terminal[i]);
    for(int i=0; i<count_non_terminal; i++) findFollow(non_terminal[i]);
    printf("Checking non terminals...\nNon Terminals\t\tFirst\t\tFollow\n");
    for(int i=0; i<count_non_terminal; i++){
        printf("\t%c\t\t",non_terminal[i]);
        for(int j=0;j<count_first[i]; j++)  printChar(first[i][j]);
        printf("\t\t");
        for(int j=0;j<count_follow[i]; j++) printChar(follow[i][j]);
        printf("\n");
    }
    printf("\nchecking  terminals...\n");
    for(int i=0; i<count_terminal; i++){printf("\t"); printChar(terminal[i]); printf("\n"); }
}
void readInput(){
    FILE *file = fopen(input, "r");
    char c = '-';
    int progress=1;
    int idDetected=0;
    int arrowDetected=0;
    initial.count=0;
    printf("reading input...\n");
    while (c != EOF) {
        progress=1;
        initial.prodCount[initial.count]=0;
        while(progress){
            c=getc(file);
            if(c==10){
                if(initial.prodCount[initial.count]!=0) initial.count++;
                initial.prodCount[initial.count]=0;
            }else if(c==-1){
                if(initial.prodCount[initial.count]!=0)initial.count++;
                progress=0;
            }else if(c==' ') continue;
            else{
                if(c=='i') idDetected=1;
                if(c=='-') arrowDetected=1;
                if(c=='d' && idDetected) initial.production[initial.count][--initial.prodCount[initial.count]]=59; //for id
                else if(c=='>' && arrowDetected) initial.prodCount[initial.count]-=2;
                else{
                    initial.production[initial.count][initial.prodCount[initial.count]]=c;
                }
                if(idDetected && c!='i') idDetected=0;
                if(arrowDetected && c!='-') arrowDetected=0;
                initial.prodCount[initial.count]++;
            }
        }
    }
    fclose(file);
    char temp;
    int t;
    for(int i=0; i<initial.count; i++){
        for(int j=0; j<initial.prodCount[i]; j++){
            temp=initial.production[i][j];
            if(temp!='|'){
                t=checkInNonTerminal(temp);
                if(t==-1){
                    t=checkInTerminal(temp);
                    if(t==0) terminal[count_terminal++]=temp;
                }else if(t==0) non_terminal[count_non_terminal++]=temp;
            }
        }
    }
    return ;  
}
int compareState(int current){
    for(int i=1; i<count_state; i++){
        if(i!=current && states[current].count==states[i].count){
            int matched=1;
            for(int j=0; j<states[current].count; j++){
                for(int k=0; k<states[current].prodCount[j]; k++){
                    if(states[current].production[j][k]!=states[i].production[j][k]){ matched=0; k=100; j=100;}
                }
            }
            if(matched) return i;
        }
    }
    return -1;
}
void pushInEdge(int i,char val,int t){
    edges[count_edge][0]=i;edges[count_edge][1]=val; edges[count_edge][2]=t;
    count_edge++;
}
void copyProduction(int into,int from,int num){
    int isPresent=0;
    for(int i=0; i<states[into].count; i++){
        if(states[into].production[i][0]==states[from].production[num][0]){
            isPresent=1;
            for(int j=1; j<states[into].prodCount[i]; j++){
                if(states[into].production[i][j]!=states[from].production[num][j]) isPresent=0;
            }
            if(isPresent) return;
        }
    }

    states[into].prodCount[states[into].count]=states[from].prodCount[num];
    for(int i=0; i<states[from].prodCount[num]; i++) 
        states[into].production[states[into].count][i]=states[from].production[num][i];
    states[into].count++;
}
void seprateStates(int s){
    char mov,temp,arr[50];
    int filled=1,curr=s;
    for(int i=0; i<states[s].prodCount[0]; i++) if(states[s].production[0][i]=='.') 
    {mov=states[s].production[0][i+1]; break;}
    arr[0]=mov;
    if(states[s].count==1){
        moveDot(s,s,arr[0]);
        return ;
    }

    for(int i=0; i<states[s].count; i++){//only seprate the state
        for(int j=1; j<states[s].prodCount[i]; j++){
            if(states[s].production[i][j]=='.'){
                temp=states[s].production[i][j+1];
                int found=filled;
                for(int m=0; m<filled; m++) if(arr[m]==temp){found=m; break;}
                copyProduction(count_state+found,s,i);
                if(found==filled) arr[filled++]=temp;
            }
        }
    }
    count_state+=filled;
    for(int i=0; i<filled; i++) moveDot(s,s+i+1,arr[i]);
}


