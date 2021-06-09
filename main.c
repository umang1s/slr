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

#include<bits/stdc++.h>
#include<stdio.h>
#define fileName "Input.txt"
#define epsilon '@'
using namespace std;

/*****************************************************Variable***************************************/
char non_terminal[100], terminal[200],first[100][50],follow[100][50];
int count_non_terminal=0,count_terminal=0,count_first[100],count_follow[100];
vector<char> TERMINAL,NON_TERMINAL;
set<char> ss;
map<int,map<char,set<pair<deque<char>,deque<char>>>>> f;
map<int,vector<pair<int,char>>> g;
int num = -1;
map<char,vector<vector<char>>> mp;

struct state{
    int count;
    char production[200][200];
    int prodCount[200];
} states[100],initial;

typedef struct state State;


/******************************************Function Declaration**********************************************/
int checkInTerminal(char );
int checkInNonTerminal(char);
void addToFirst(char,int );
void addToFollow(char ,int);
void readInput();
void checkingTandNT();
void findFirst(char);
void findFollow(char);
int findPositionInNT(char);
void printChar(char x){if(x==';') printf("id");else  printf("%c",x);}
void printProduction();




/**************************************Function definition************************************************/
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
void readInput(){
    FILE *file = fopen(fileName, "r");
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
                                temp2=findPositionInNT(initial.production[i][started]);
                                if(checkInNonTerminal(initial.production[i][started])){
                                    findFirst(initial.production[i][started]);
                                    loop=0;
                                    for(int m=0; m<count_first[temp2];m++){
                                        if(first[temp2][m]==epsilon) loop=1;
                                        addToFirst(first[temp2][m],pos);
                                    }
                                    if(loop) started++;
                                }
                            }
                            // int loop=1;
                            // int started=j;
                            // while(loop){
                            //     loop=0;
                            //     findFirst(initial.production[i][started]);
                            //     temp2=findPositionInNT(initial.production[i][started]);
                            //     for(int m=0; m<count_first[temp2];m++){
                            //         if(first[temp2][m]==epsilon) loop=1;
                            //         addToFirst(first[temp2][m],pos);
                            //     }
                            //     if(loop) started++;

                            // }
                        }else addToFirst(initial.production[i][j],pos);
                    }
                }
            }
        }
    }
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
                    temp=initial.production[i][0];
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


bool search(char i, char org, char last, map<char,vector<vector<char>>> &mp){
    bool rtake = false;
    for(auto r : mp[i]){
        bool take = true;
        for(auto s : r){
            if(s == i) break;
            if(!take) break;
            if(!(s>='A'&&s<='Z')&&s!=epsilon){
                ss.insert(s);
                break;
            }
            else if(s == epsilon){
                if(org == i||i == last)
                ss.insert(s);
                rtake = true;
                break;
            }
            else{
                take = search(s,org,r[r.size()-1],mp);
                rtake |= take;
            }
        }
    }
    return rtake;
}


void depthFirstSearch(char c, char way, int last, pair<deque<char>,deque<char>> curr){
    map<char,set<pair<deque<char>,deque<char>>>> mp2;
    int rep = -2;
    if(last != -1){
        for(auto q : g[last]){
            if(q.second == way){
                rep = q.first;
                mp2 = f[q.first];
            }
        }
    }
    mp2[c].insert(curr);
    int count = 10;
    while(count--){
        for(auto q : mp2){
            for(auto r : q.second){
                if(!r.second.empty()){
                    if(r.second.front()>='A'&&r.second.front()<='Z'){
                        for(auto s : mp[r.second.front()]){
                            deque<char> st,emp;
                            for(auto t : s) st.push_back(t);
                            mp2[r.second.front()].insert({emp,st});
                        }
                    }
                }
            }
        }
    }
    for(auto q : f){
        if(q.second == mp2){
            g[last].push_back({q.first,way});
            return;
        }
    }
    if(rep == -2){
        f[++num] = mp2;
        if(last != -1)
        g[last].push_back({num,way});
    }
    else{
        f[rep] = mp2;
    }
    int cc = num;
    for(auto q : mp2){
        for(auto r : q.second){
            if(!r.second.empty()){
                r.first.push_back(r.second.front());
                r.second.pop_front();
                depthFirstSearch(q.first,r.first.back(),cc,r);
            }
        }
    }
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
/*******************************************Main Function********************************************/

int main(){
    cout<<"------------------------------------------------------------\n";
    cout<<"|-----------------------SLR PARSER-------------------------|\n";
    cout<<"------------------------------------------------------------\n";
    readInput();
    printProduction(&initial);
    if(initial.count==0){printf("No input found \nExitting....\n"); return 1;}
    checkingTandNT();
    int i,j;
    ifstream fin(fileName);
    string num;
    vector<int> fs;
    char start;
    bool flag = 0;
    while(getline(fin,num)){
        if(flag == 0) start = num[0],flag = 1;
        vector<char> temp;
        char s = num[0];
        for(i=3;i<num.size();i++){
            if(num[i] == '|'){
                mp[s].push_back(temp);
                temp.clear();
            }
            else {
                temp.push_back(num[i]);
            }
        }
        mp[s].push_back(temp);
    }
    map<char,set<char>> firstValue;
    for(auto q : mp){
        ss.clear();
        search(q.first,q.first,q.first,mp);
        for(auto g : ss) firstValue[q.first].insert(g);
    }


    

    map<char,set<char>> followValue;
    followValue[start].insert('$');
    int count = 10;
    while(count--){
        for(auto q : mp){
            for(auto r : q.second){
                for(i=0;i<r.size()-1;i++){
                    if(r[i]>='A'&&r[i]<='Z'){
                        if(!(r[i+1]>='A'&&r[i+1]<='Z')) followValue[r[i]].insert(r[i+1]);
                        else {
                            char temp = r[i+1];
                            int j = i+1;
                            while(temp>='A'&&temp<='Z'){
                                if(*firstValue[temp].begin()==epsilon){
                                    for(auto g : firstValue[temp]){
                                        if(g=='e') continue;
                                        followValue[r[i]].insert(g);
                                    }
                                    j++;
                                    if(j<r.size()){
                                        temp = r[j];
                                        if(!(temp>='A'&&temp<='Z')){
                                            followValue[r[i]].insert(temp);
                                            break;
                                        }
                                    }
                                    else{
                                        for(auto g : followValue[q.first]) followValue[r[i]].insert(g);
                                        break;
                                    }
                                }
                                else{
                                    for(auto g : firstValue[temp]){
                                        followValue[r[i]].insert(g);
                                    }
                                    break;
                                }
                            }
                        }
                    }
                }
                if(r[r.size()-1]>='A'&&r[r.size()-1]<='Z'){
                    for(auto g : followValue[q.first]) followValue[r[i]].insert(g);
                }
            }
        }
    }

    


    string temp = "";
    temp+='.';
    temp+=start;

    deque<char> emp;
    deque<char> st;
    st.push_back(start);
    depthFirstSearch('!','k',-1,{emp,st});

    cout<<"\nSimplified Form: "<<'\n';
    int cc = 1;
    set<char> action,go;
    map<pair<char,deque<char>>,int> pos;
    for(auto q : mp){
        go.insert(q.first);
        for(auto r : q.second){
            cout<<"\tr"<<cc<<": ";
            string ans = "";
            ans += q.first;
            ans+="->";
            deque<char> temp;
            for(auto s : r) ans += s,temp.push_back(s);
            pos[{q.first,temp}] = cc;
            for(auto s : r){
                if(s>='A'&&s<='Z') go.insert(s);
                else action.insert(s);
            }
            cout<<ans<<'\n';
            cc++;
        }
    }
    
    cout<<"\nGraph: \n";
    for(auto mp2 : f){
        cout<<"I";
        cout<<mp2.first<<": \n";
        for(auto q : mp2.second){
            string ans = "\t";
            ans += q.first;
            ans += "->";
            for(auto r : q.second){
                for(auto t : r.first) ans+=t;
                ans+='.';
                for(auto t : r.second) ans+=t;
                ans+='|';
            }
            ans.pop_back();  
            for(auto tt : ans){
                if(tt == '!') cout<<start<<'\'';
                else cout<<tt;
            }
            cout<<'\n';
        }
    }
    cout<<'\n';
    cout<<"goto move: "<<'\n';
    for(auto q : g){
        for(auto r : q.second){
            cout<<"\tI"<<q.first<<" -> "<<r.second<<" -> "<<"I"<<r.first<<"\n";
        }
    }
    action.insert('$');
    cout<<"\nParsing Table:"<<'\n';
    cout<<"St.\t\tAction & Goto"<<'\n';
    int tot = f.size();
    cout<<"  \t";
    for(auto q : action) cout<<q<<'\t';
    for(auto q : go) cout<<q<<'\t';
    cout<<'\n';
    for(i=0;i<tot;i++){
        cout<<"I"<<i<<'\t';
        for(auto q : action){
            if(g.count(i)){
                int flag = 0;
                for(auto r : g[i]){
                    if(r.second == q){
                        flag = 1;
                        cout<<"S"<<r.first<<"\t";
                        break;
                    }
                }
                if(!flag) cout<<"-"<<'\t';
            }
            else{
                int flag = 0;
                for(auto r : f[i]){
                    if(r.first == '!'){
                        if(q == '$'){
                        cout<<"AC\t";
                        flag = 1;
                        }
                        else cout<<"-\t";
                    }  

                }
                if(!flag){
                    for(auto r : f[i]){
                        char ccc = r.first;
                        deque<char> chk = (*r.second.begin()).first;
                        int cou = 1;
                        for(auto r : followValue[ccc]){
                            if(q == r){
                                cout<<"r"<<pos[{ccc,chk}]<<"\t";
                            }
                            cou++;
                        }
                    }
                }
            }
        }
        for(auto q : go){
            if(g.count(i)){
                int flag = 0;
                for(auto r : g[i]){
                    if(r.second == q){
                        flag = 1;
                        cout<<r.first<<"\t";
                        break;
                    }
                }
                if(!flag) cout<<"-"<<'\t';
            }
            else{
                cout<<"-"<<'\t';
            }
        }
        cout<<'\n';
    }

    return 0;
}
