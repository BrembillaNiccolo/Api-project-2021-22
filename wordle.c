#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX 64
#define MAXBUFF 10000

//Red and black tree data structure
typedef enum{
    RED,
    BLACK
} color;

typedef struct diz{
    char *key; //word to put in alphabetical order 
    struct diz* father;
    struct diz* left;
    struct diz* right;
    color color;
}Dictionary;

//Global Variables

int numchar [MAX];//Maintain in memory how many elements are present of any kind
//numbers, uppercase , lowercase, characters
int numcfr [MAX];//as comparison for every attempt
int Tabchar[2][MAX];//to know if the number must be exact or more
int Tempchar[2][MAX];
int possible;
char *string;//string to maintain gli input
char* selectedword;//game base word to be compared
int num_try;//number of attempt left
Dictionary * NIL=NULL;//NIL for Red Black Tree
int position;
int count;
char **Tabnec;//table for the comparisons
bool H;
int Vettemp[MAX]; //to count the number of letters
char buff[MAXBUFF];


//fast char comparisons
int fcomp( const char *p0, const char *p1, int len ){
    for(int i=0; i<len;i++){
        if(p0[i]!=p1[i])
            return p0[i]-p1[i];
    }
    return 0;
}
//Left rotation of the red and black tree
void LeftRotate(Dictionary** Root,Dictionary *x){
    Dictionary* y=x->right;
    x->right=y->left;
    if(y->left!=NIL){
        y->left->father=x;
    }
    y->father=x->father;
    if(x->father==NIL){
        (*Root)=y;
    }
    else if(x==x->father->left){
        x->father->left=y;
    }
    else{
        x->father->right=y;
    }
    y->left=x;
    x->father=y;
    return;
}
//Right rotation of the red and black tree
void RightRotate(Dictionary** Root,Dictionary *x){
    Dictionary* y=x->left;
    x->left=y->right;
    if(y->right!=NIL){
        y->right->father=x;
    }
    y->father=x->father;
    if(x->father==NIL){
        (*Root)=y;
    }
    else if(x==x->father->right){
        x->father->right=y;
    }
    else{
        x->father->left=y;
    }
    y->right=x;
    x->father=y;
    return;
}
//Fix the Tree lenght after the operations
void RBFixup(Dictionary** Root,Dictionary *z){
    Dictionary* y;
    while(z != *Root && z->father->color==RED){
        if(z->father==z->father->father->left){
            y=z->father->father->right;
            if(y->color==RED){
                z->father->color=BLACK;
                y->color=BLACK;
                z->father->father->color=RED;
                z=z->father->father;
            }
            else{
                if (z==z->father->right){
                    z=z->father;
                    LeftRotate(Root,z);
                }
                z->father->color=BLACK;
                z->father->father->color=RED;
                RightRotate(Root,z->father->father);
            }
        }
        else{
            y=z->father->father->left;
            if(y->color==RED){
                z->father->color=BLACK;
                y->color=BLACK;
                z->father->father->color=RED;
                z=z->father->father;
            }
            else {
                if (z==z->father->left){
                    z=z->father;
                    RightRotate(Root,z);
                }
                z->father->color=BLACK;
                z->father->father->color=RED;
                LeftRotate(Root,z->father->father);
            }
        }
    }
    (*Root)->color=BLACK;
    return;
}
//Return min value of the Tree Branch
Dictionary *Min(Dictionary *t){
    Dictionary* curr=t;
    while(curr->left!=NIL){
        curr=curr->left;
    }
    return curr;
}
// Return the successor
Dictionary *successore(Dictionary *t){
    Dictionary* y;
    if(t->right!=NIL) return Min(t->right);
    y=t->father;
    while(y!=NIL && y->right==t){
        t=y;
        y=y->father;
    }
    return y;
}
//Repair and delete a node of the tree
void RiparaRBcancella(Dictionary** Root,Dictionary* x){
    Dictionary *w;
    while(x!=(*Root) && x->color==BLACK){
        if(x==x->father->left){
            w=x->father->right;
            if(w->color==RED){
                w->color=BLACK;
                x->father->color=RED;
                LeftRotate(Root,x->father);
                w=x->father->right;
            }
            if(w->left->color==BLACK && w->right->color==BLACK){
                w->color=RED;
                x=x->father;
            }
            else {
                if (w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;
                    RightRotate(Root, w);
                    w = x->father->right;
                }
                w->color = x->father->color;
                x->father->color = BLACK;
                w->right->color = BLACK;
                LeftRotate(Root, x->father);
                x=(*Root);
            }
        }
        else {
            w = x->father->left;
            if (w->color == RED) {
                w->color = BLACK;
                x->father->color = RED;
                RightRotate(Root, x->father);
                w = x->father->left;
            }
            if (w->right->color == BLACK && w->left->color == BLACK) {
                w->color = RED;
                x = x->father;
            }
            else {
                if (w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    LeftRotate(Root, w);
                    w = x->father->left;
                }
                w->color = x->father->color;
                x->father->color = BLACK;
                w->left->color = BLACK;
                RightRotate(Root, x->father);
                x=(*Root);
            }
        }
    }
    x->color=BLACK;
    return;
}
//Delete a node of the tree
Dictionary* CancellaRB(Dictionary** Root,Dictionary *x){
    Dictionary * dacanc, *sotto;
    if(x->left==NIL||x->right==NIL)
        dacanc=x;
    else
        dacanc=successore(x);
    if(dacanc->left!=NIL)
        sotto=dacanc->left;
    else
        sotto=dacanc->right;
    sotto->father=dacanc->father;
    if(dacanc->father==NIL)
        (*Root)=sotto;
    else if(dacanc==dacanc->father->left)
        dacanc->father->left=sotto;
    else
        dacanc->father->right=sotto;
    if(dacanc!=x)
        x->key=dacanc->key;
    if(dacanc->color==BLACK)
        RiparaRBcancella(Root,sotto);
    return dacanc;
}//dovrebbe essere ok
//Return the position from a char in the 64 lenght of the array
int tpos(char temp){//ok
    if((temp-'0')>=0&&(temp-'0')<=9)
        return (temp-'0'+1);
    else if((temp-'A')>=0&&(temp-'A')<=25)
        return ((temp-'A')+11);
    else if((temp-'a')>=0&&(temp-'a')<=25)
        return ((temp-'a')+38);
    else if(temp=='-')
        return 0;
    else if(temp=='_')//se underscore ultimo possibile carattere
        return 37;
    return -1;
}//ok
//Return tthe position from an int in the 64 lenght of the array
int trovapos(int i){//ok
    if((string[i]-'0')>=0&&(string[i]-'0')<=9&&(numcfr[(string[i]-'0')+1]>0))
        return (string[i]-'0'+1);
    else if((string[i]-'A')>=0&&(string[i]-'A')<=25&&(numcfr[(string[i]-'A')+11]>0))
        return ((string[i]-'A')+11);
    else if((string[i]-'a')>=0&&(string[i]-'a')<=25&&(numcfr[(string[i]-'a')+38]>0))
        return ((string[i]-'a')+38);
    else if(string[i]=='-'&&(numcfr[0]>0))
        return 0;
    else if(string[i]=='_'&&(numcfr[37]>0))//se underscore ultimo possibile carattere
        return 37;
    else return -1;
}//ok
// Create the Tempchar table
void creatab(char* Ris,int lenght){
    int i;
    for(i=0;i<MAX;i++){
        Tempchar[0][i]=0;
        Tempchar[1][i]=0;
    }
    for(i=0;i<lenght;i++){
        position=tpos(string[i]);
        if(Ris[i]=='+'){
            Tempchar[0][position]++;
            Tabnec[i][position]='+';
            Tabnec[i][MAX]=string[i];
        }
        else if(Ris[i]=='|'){
            Tempchar[0][position]++;
            Tabnec[i][position]='/';
            Tempchar[1][position]=-1;
        }
        else if(Ris[i]=='/'){
            Tabnec[i][position]='/';
            if(Tempchar[1][position]==-1) Tempchar[1][position]=2;
            else if(Tempchar[1][position]==2) Tempchar[1][position]=2;
            else Tempchar[1][position]=1;
        }
    }
    for(i=0;i<MAX;i++){
        if(Tempchar[0][i]>Tabchar[0][i]){
            Tabchar[0][i]=Tempchar[0][i];
            Tabchar[1][i]=Tempchar[1][i];
        }
        else if(Tempchar[1][i]==1) Tabchar[1][i]=1;
        else if(Tempchar[0][i]==Tabchar[0][i]&&(Tempchar[1][i]>0)&&Tabchar[1][i]==0){
            Tabchar[1][i]=1;
        }
        else if(Tempchar[0][i]==Tabchar[0][i]&&(Tempchar[1][i]>0)&&Tabchar[1][i]==-1){
            Tabchar[1][i]=Tempchar[1][i];
        }
    }
}
char* confrontaP(int lenght){//cfr tra string e selectedword
    char* Riscfr=(char*)malloc(sizeof(char)*(lenght+1));
    int i=0;
    for(i=0;i<lenght;i++) Riscfr[i]=' ';
    for(i=0;i<MAX;i++) numcfr[i]=numchar[i]; //reimposto numcfr dopo ogni tentativo di parola
    for(i=0;i<lenght;i++){
        if(string[i]==selectedword[i]) {
            Riscfr[i] = '+';
            position=trovapos(i);
            numcfr[position]--;
        }
    }
    for(i=0;i<lenght;i++){
        if(Riscfr[i]!='+'){
            position=trovapos(i);
            if(position!=-1){
                Riscfr[i]='|';
                numcfr[position]--;
            }
            else
                Riscfr[i]='/';
        }
    }
    Riscfr[lenght]='\0';
    return Riscfr;
}//ok
int confronto(char* parola,int lenght){
    int i=0;
    int Poslettera[lenght];//faccio un'operazione in piu ma potrebbe servire dopo, cosi non la faccio piu volte per la stessa lettera
    for(i=0;i<lenght;i++){//1°confronto
        Poslettera[i]=tpos(parola[i]);
        //se in pos ho trovato un + e la lettera non è quella del +
        if(Tabnec[i][MAX]!=' ' && Tabnec[i][Poslettera[i]]!= '+') return (lenght+i);
        //posso anche fare && ;) parola[i]!=Tabnec[i][MAX]
        // ho un numero fisso di lettere e ci sono solo + quindi so dove devono stare
        if(Tabchar[1][Poslettera[i]]==1 && Tabnec[i][Poslettera[i]]!='+') return (2*lenght+i);
        if(Tabnec[i][Poslettera[i]]=='/') return (3*lenght+i);
        if(Tabchar[1][Poslettera[i]]==1&&Tabchar[0][Poslettera[i]]==0) return (4*lenght+i);
    }
    for(i=0;i<MAX;i++){//devo inizializzare il vettore
        Vettemp[i]=0;
    }
    for(i=0;i<lenght;i++){//devo inizializzare il vettore
        Vettemp[Poslettera[i]]++;
    }
    for(i=0;i<MAX;i++){//devo inizializzare il vettore
        if(Vettemp[i]<Tabchar[0][i]) return (5*lenght+i); //così so che ho troppi pochi elementi di quella lettera sbagliata era quella in posizione i;
        if(Vettemp[i]>Tabchar[0][i] && Tabchar[1][i]>0) return (6*lenght +i);
        //ho troppi elementi di quella lettera
    }
    return -1;
}//restituisce -1 se va bene, se no posizione sbagliata o codice predefinito
int confronta(char* parola,int* A,int lenght){
    int i=0;
    int Poslettera[lenght];//faccio un'operazione in piu ma potrebbe servire dopo, cosi non la faccio piu volte per la stessa lettera
    for(i=0;i<lenght;i++){//1°confronto
        Poslettera[i]=tpos(parola[i]);
        //se in pos ho trovato un + e la lettera non è quella del +
        if(Tabnec[i][MAX]!=' ' && Tabnec[i][Poslettera[i]]!= '+') return (lenght+i);
        //posso anche fare && ;) parola[i]!=Tabnec[i][MAX]
        // ho un numero fisso di lettere e ci sono solo + quindi so dove devono stare
        if(Tabchar[1][Poslettera[i]]==1 && Tabnec[i][Poslettera[i]]!='+') return (2*lenght+i);
        if(Tabnec[i][Poslettera[i]]=='/') return (3*lenght+i);
        //if(Tabchar[1][Poslettera[i]]==1&&Tabchar[0][Poslettera[i]]==0) return (4*lenght+i);
    }
    for(i=0;i<lenght;i++){//devo inizializzare il vettore
        Vettemp[Poslettera[i]]=0;
        Vettemp[A[i]]=0;
    }
    for(i=0;i<lenght;i++){//devo inizializzare il vettore
        Vettemp[Poslettera[i]]++;
    }

    for(i=0;i<lenght;i++){//devo inizializzare il vettore
        if(Vettemp[Poslettera[i]]<Tabchar[0][Poslettera[i]]) return (5*lenght+i); //così so che ho troppi pochi elementi di quella lettera sbagliata era quella in posizione i;
        if(Vettemp[A[i]]<Tabchar[0][A[i]]) return (5*lenght+i); //così so che ho troppi pochi elementi di quella lettera sbagliata era quella in posizione i;
        if(Vettemp[Poslettera[i]]>Tabchar[0][Poslettera[i]] && Tabchar[1][Poslettera[i]]>0) return (6*lenght +i);
        //ho troppi elementi di quella lettera
        if(Vettemp[A[i]]>Tabchar[0][A[i]] && Tabchar[1][A[i]]>0) return (6*lenght +i);
    }

    return -1;
}//restituisce -1 se va bene, se no posizione sbagliata o codice predefinito
Dictionary* CreaDiz(int lenght){
    Dictionary* Temp=(Dictionary*)malloc(sizeof(Dictionary));
    char*T=(char*) malloc(sizeof(char)*lenght+1);
    strncpy(T,string,lenght+1);
    Temp->key=T;
    Temp->father=NIL;
    Temp->color=RED;
    Temp->left=NIL;
    Temp->right=NIL;
    return Temp;
}//OK
void Inserimento(Dictionary**Head,Dictionary *x,int lenght){
    Dictionary* pre=NIL;
    Dictionary* cur=(*Head);
    while(cur!=NIL) {
        pre = cur;
        if (fcomp(x->key, cur->key,lenght) < 0)
            cur = cur->left;
        else
            cur = cur->right;
    }
    x->father=pre;
    if(pre==NIL){
        (*Head)=x;
    }
    else if(fcomp(x->key, pre->key,lenght)<0)
        pre->left=x;
    else
        pre->right=x;
    RBFixup(Head,x);
    return ;
}//OK
void CFRelementi1m(Dictionary **Headda, Dictionary **Headverso, int* A,int lenght){
//nel primo giro Headda=Head Headverso=Headtemp (guardo se Heatemp==NIL)
//dal secondo inverto i due
    Dictionary *parola=(*Headda);
    Dictionary *parolanext=NULL;
    char *keyins;
    int var;
    while(parola->left!=NIL) parola=parola->left; //vado al piu a sx
    while(parola!=NIL){
        var=confronta(parola->key,A,lenght);//confronto con tabella restituisce -1 se supera tutti i confronti, altrimenti un numero >0
        parolanext=successore(parola); //funzione presente nel programma
        //sono nell'albero principale (1° ciclo){
        // anche se non devo cancellare la parola devo andare al sccessivo
        if (var<0){ //da canc
            if(parola->left!=NIL&&parola->right!=NIL)
                parolanext=parola;//in cancella cancello il successore
            keyins=parola->key; //se cancella  il succ la struct avrà come la key del successore
            parola=CancellaRB(Headda,parola);//la parola struct punta alla struct canc
            parola->key=keyins;//necessario
            parola->father=NIL;//non so se nec
            parola->left=NIL;//non so se nec
            parola->right=NIL;//non so se nec
            parola->color=RED;//necessario
            Inserimento(Headverso,parola,lenght);
            count++;
        }
        parola=parolanext;
    }
    return;
}
void CFRelementi2m(Dictionary **Headda, Dictionary **Headverso,int *A,int lenght){
//nel primo giro Headda=Head Headverso=Headtemp (guardo se Heatemp==NIL)
//dal secondo inverto i due
    Dictionary *parola=(*Headda);
    Dictionary *parolanext=NULL;
    char *keyins;
    int var;
    while(parola->left!=NIL) parola=parola->left; //vado al piu a sx
    while(parola!=NIL){
        var=confronta(parola->key,A,lenght);//confronto con tabella restituisce -1 se supera tutti i confronti, altrimenti un numero >0
        parolanext=successore(parola); //funzione presente nel programma
        //sono nell'albero principale (1° ciclo){
        // anche se non devo cancellare la parola devo andare al sccessivo
        if (var>0){ //da canc
            if(parola->left!=NIL&&parola->right!=NIL)
                parolanext=parola;//in cancella cancello il successore
            keyins=parola->key; //se cancella  il succ la struct avrà come la key del successore
            parola=CancellaRB(Headda,parola);//la parola struct punta alla struct canc
            parola->key=keyins;//necessario
            parola->father=NIL;//non so se nec
            parola->left=NIL;//non so se nec
            parola->right=NIL;//non so se nec
            parola->color=RED;//necessario
            Inserimento(Headverso,parola,lenght);
            count--;
        }
        parola=parolanext;
    }
    return;
}
void CFRelementi1(Dictionary **Headda, Dictionary **Headverso,int lenght){
//nel primo giro Headda=Head Headverso=Headtemp (guardo se Heatemp==NIL)
//dal secondo inverto i due
    Dictionary *parola=(*Headda);
    Dictionary *parolanext=NULL;
    char *keyins;
    int var;
    while(parola->left!=NIL) parola=parola->left; //vado al piu a sx
    while(parola!=NIL){
        var=confronto(parola->key,lenght);//confronto con tabella restituisce -1 se supera tutti i confronti, altrimenti un numero >0
        parolanext=successore(parola); //funzione presente nel programma
        //sono nell'albero principale (1° ciclo){
        // anche se non devo cancellare la parola devo andare al sccessivo
        if (var<0){ //da canc
            if(parola->left!=NIL&&parola->right!=NIL)
                parolanext=parola;//in cancella cancello il successore
            keyins=parola->key; //se cancella  il succ la struct avrà come la key del successore
            parola=CancellaRB(Headda,parola);//la parola struct punta alla struct canc
            parola->key=keyins;//necessario
            parola->father=NIL;//non so se nec
            parola->left=NIL;//non so se nec
            parola->right=NIL;//non so se nec
            parola->color=RED;//necessario
            Inserimento(Headverso,parola,lenght);
            count++;
        }
        parola=parolanext;
    }
    return;
}
void CFRelementi2(Dictionary **Headda, Dictionary **Headverso,int lenght){
//nel primo giro Headda=Head Headverso=Headtemp (guardo se Heatemp==NIL)
//dal secondo inverto i due
    Dictionary *parola=(*Headda);
    Dictionary *parolanext=NULL;
    char *keyins;
    int var;
    while(parola->left!=NIL) parola=parola->left; //vado al piu a sx
    while(parola!=NIL){
        var=confronto(parola->key,lenght);//confronto con tabella restituisce -1 se supera tutti i confronti, altrimenti un numero >0
        parolanext=successore(parola); //funzione presente nel programma
        //sono nell'albero principale (1° ciclo){
        // anche se non devo cancellare la parola devo andare al sccessivo
        if (var>0){ //da canc
            if(parola->left!=NIL&&parola->right!=NIL)
                parolanext=parola;//in cancella cancello il successore
            keyins=parola->key; //se cancella  il succ la struct avrà come la key del successore
            parola=CancellaRB(Headda,parola);//la parola struct punta alla struct canc
            parola->key=keyins;//necessario
            parola->father=NIL;//non so se nec
            parola->left=NIL;//non so se nec
            parola->right=NIL;//non so se nec
            parola->color=RED;//necessario
            Inserimento(Headverso,parola,lenght);
            count--;
        }
        parola=parolanext;
    }
    return;
}
void libalbero(Dictionary **Headtemp,Dictionary * Testa,Dictionary** Head,int lenght){
    if(Testa==NIL) return;
    Dictionary *Temp=NULL;
    char* T=Testa->key;
    Temp=CancellaRB(Headtemp,Testa);
    Temp->key=T;
    Temp->father=NIL;
    Temp->color=RED;
    Temp->left=NIL;
    Temp->right=NIL;
    Inserimento(Head,Temp,lenght);
    count--;
    libalbero(Headtemp,(*Headtemp),Head,lenght);
    return;
}
void cfrParola(Dictionary** Head,Dictionary** Headtemp,int lenght){//da completare
    char*R;
    int t;
    int A[lenght];
    R=confrontaP(lenght);
    printf("%s\n",R);
    creatab(R,lenght);
    if (lenght <=20){
        for(int i=0;i<lenght;i++)
            A[i]=tpos(string[i]);
    }
    //Bisogna cambiare tutto
    if(count==0){
        if(Tabnec[0][MAX]!=' '){
            t=tpos(Tabnec[0][MAX]);
            if(lenght>20)
                CFRelementi1(&(Head[t]),&(Headtemp[t]),lenght);
            else
                CFRelementi1m(&(Head[t]),&(Headtemp[t]),A,lenght);
        }
        else{
            for(int j=0;j<MAX;j++){
                if(Tabnec[0][j]!='/'||(!(Tabchar[0][j]==0&&Tabchar[1][j]==1))){
                    if(lenght>20)
                        CFRelementi1(&(Head[j]),&(Headtemp[j]), lenght);
                    else
                        CFRelementi1m(&(Head[j]),&(Headtemp[j]),A, lenght);
                }

            }
        }
    }
    else{
        if(Tabnec[0][MAX]!=' '){
            t=tpos(Tabnec[0][MAX]);
            for(int j=0;j<MAX;j++){
                if(j!=t)
                    libalbero(&(Headtemp[j]),Headtemp[j],&(Head[j]),lenght);
            }
            if(lenght>20)
                CFRelementi2(&(Headtemp[t]),&(Head[t]), lenght);
            else{
                CFRelementi2m(&(Headtemp[t]),&(Head[t]),A, lenght);
            }

        }
        else{
            for(int j=0;j<MAX;j++){
                if(Tabnec[0][j]=='/'||(Tabchar[0][j]==0&&Tabchar[1][j]==1))
                    libalbero(&(Headtemp[j]),Headtemp[j],&(Head[j]),lenght);
                else{
                    if((lenght>20))
                        CFRelementi2(&(Headtemp[j]),&(Head[j]), lenght);
                    else{
                        CFRelementi2m(&(Headtemp[j]),&(Head[j]),A, lenght);
                    }
                }

            }
        }
    }
    printf("%d\n",count);
    free(R);
    return;
}
bool Cercaalbero(Dictionary** Testa){
    if((*Testa)==NIL) return 0;
    if (strcmp((*Testa)->key,string)==0)
        return 1;
    if (strcmp((*Testa)->key,string)>0)
        return Cercaalbero(&((*Testa)->left));
    else
        return Cercaalbero(&((*Testa)->right));
}
void stampaalbero(Dictionary **testa){
    if((*testa)==NIL) return;
    stampaalbero(&((*testa)->left));
    printf("%s\n",(*testa)->key);
    stampaalbero(&((*testa)->right));
    return;
} //stampa tutti val di Dictionary (da aggiunger cfr con bool se parola possibile)
void liberaalbero(Dictionary **testa){
    if((*testa)==NIL)return;
    liberaalbero(&((*testa)->left));
    liberaalbero(&((*testa)->right));
    if ((*testa)->key != NULL) { free((*testa)->key); }
    free((*testa));
    return;
}
void creaTNIL(){
    NIL=(Dictionary*)malloc(sizeof(Dictionary));
    NIL->key=NULL;
    NIL->father=NIL;
    NIL->color=BLACK;
    NIL->left=NIL;
    NIL->right=NIL;
    return;
}//OK
void creatabella(int lenght){
    int i=0;
    for(i=0;i<MAX;i++) {
        numchar[i]=0;
        Tabchar[0][i]=0;
        Tabchar[1][i]=0;
    }
    for(i=0;i<lenght;i++){
        if((selectedword[i]-'0')>=0&&(selectedword[i]-'0')<=9)
            numchar[(selectedword[i]-'0'+1)]++;
        else if((selectedword[i]-'A')>=0&&(selectedword[i]-'A')<=25)
            numchar[(selectedword[i]-'A')+11]++;
        else if((selectedword[i]-'a')>=0&&(selectedword[i]-'a')<=25)
            numchar[(selectedword[i]-'a')+38]++;
        else if(selectedword[i]=='-')
            numchar[0]++;
        else//se underscore ultimo possibile carattere
            numchar[37]++;
    }
    return;
}
void resettab(int lenght){
    int i,j;
    for(i=0;i<lenght;i++){
        for(j=0;j<65;j++){//per cfr
            Tabnec[i][j]=' ';
        }
    }
    return;
}
void spostaalbero(Dictionary **Headtemp,Dictionary * Testa,Dictionary** Head,int lenght){
    if(Testa==NIL) return;
    Dictionary *Temp=NULL;
    char* T=Testa->key;
    Temp=CancellaRB(Headtemp,Testa);
    Temp->key=T;
    Temp->father=NIL;
    Temp->color=RED;
    Temp->left=NIL;
    Temp->right=NIL;
    Inserimento(Head,Temp,lenght);
    spostaalbero(Headtemp,(*Headtemp),Head,lenght);
    return;
}
void prendiParola(int max){
    char c;
    int i=0;
    c=getchar();
    while(c!='\n'&&c!=EOF){
        string[i]=c;
        i++;
        c=getchar();
    }
    if(c==EOF) H=1;
    string[i]='\0';
    return ;
}
void parola_cfr(Dictionary** Headtemp,Dictionary** Head,int max,int lenght){
    resettab(lenght);
    for(int j=0;j<MAX;j++){
        spostaalbero(&(Headtemp[j]),Headtemp[j],&(Head[j]),lenght);
    }
    prendiParola(max);
    strncpy(selectedword,string,lenght+1);
    if(scanf("%d",&num_try))
        ;
    char t=getchar();
    if(t=='\n')
        ;
    creatabella(lenght);
    count=0;
    return;
}
void comparazione(Dictionary **Headtemp,Dictionary**Head,int lenght){
    if(strcmp(string,selectedword)==0){
        printf("ok\n");
        num_try=-1;
    }
    else {
        bool ris=0;
        int j=tpos(string[0]);
        ris = Cercaalbero(&(Headtemp[j])); //controllo se presente
        if (ris == 0){ //se non presente scrivo non esiste
            ris = Cercaalbero(&(Head[j]));
        }
        if (ris == 0)
            printf("not_exists\n");
        else{
            cfrParola(Head,Headtemp,lenght);
            num_try--;
        }
        if(num_try==0)
            printf("ko\n");
    }
    return;
}
void insert(Dictionary**Head,Dictionary** Headtemp, int max,int lenght) { //Inserimento valori nell'albero
    int n=1,m;
    prendiParola(max);
    while (fcomp(string, "+inserisci_fine",2) != 0) {
        Dictionary* x=CreaDiz(lenght);
        m=tpos(string[0]);
        if(count>0){
            n=confronto(x->key,lenght);//>0 se da mette re in Head altrimenti -1
        }
        if(count==0||(count>0&&n>0)) Inserimento(&(Head[m]),x,lenght);
        else {
            Inserimento(&(Headtemp[m]),x,lenght);
            count++;
        }
        prendiParola(max);
    }
    return;
}
void ins1(Dictionary**Head,Dictionary** Headtemp,int max,int lenght) { //Inserimento valori nell'albero
    int n;
    prendiParola(max);
    while (fcomp(string, "+nuova_partita",2) != 0) {
        if(fcomp(string, "+inserisci_fine",2) != 0 && fcomp(string, "+inserisci_inizio",2)!=0) {
            Dictionary *x = CreaDiz(lenght);
            n=tpos(string[0]);
            Inserimento(&(Head[n]), x,lenght);
        }
        prendiParola(max);
    }
    return;
}
void libera(char** lib,int lenght){
    int n;
    for(n=0;n<lenght;n++){
        free(lib[n]);
    }
    free(lib);
}


//Per file
int main(int argc, char *argv[]) { //o **argv
    setvbuf(stdout,buff,_IOFBF,sizeof(buff));
    Dictionary *Head[MAX];
    Dictionary *Headtemp[MAX];
    int max;
    int lenght;
    H=0;
    char**i;
    if(scanf("%d",&lenght))
        ;
    char t=getchar();
    if(t=='\n')
        ;
    if(lenght+1>=18)
        max=lenght+1;
    else max=18;
    string=(char*)malloc(sizeof(char)*max);//posso sostituire con una stringa di lunghezza MAX
    Tabnec=(char**)malloc(sizeof(char*)*(lenght));
    for(int j=0;j<lenght;j++)
        Tabnec[j]=(char*)malloc(65*sizeof(char));
    resettab(lenght);
    selectedword=(char*)malloc(sizeof(char)*(lenght+1));
    creaTNIL();
    for(int j=0;j<MAX;j++){
        Head[j]=NIL;
        Headtemp[j]=NIL;
    }

    ins1(Head,Headtemp,max,lenght);
    parola_cfr(Headtemp,Head,max,lenght);
    prendiParola(max);
    while (H==0) {
        if(fcomp(string,"+stampa_filtrate",2)==0){
            if(count>0)
                for(int j=0;j<MAX;j++){
                    stampaalbero(&(Headtemp[j]));
                }

            else
                for(int j=0;j<MAX;j++){
                    stampaalbero(&(Head[j]));
                }
        }//da cambiare con Headtemp
        else if(fcomp(string,"+inserisci_inizio",2)==0)insert(Head,Headtemp,max,lenght);
        else if(fcomp(string,"+nuova_partita",2)==0) parola_cfr(Headtemp,Head,max,lenght);
        else  comparazione(Headtemp,Head,lenght);
        prendiParola(max);
    }

    free(selectedword);
    for(int j=0;j<MAX;j++){
        liberaalbero(&(Head[j]));
        liberaalbero(&(Headtemp[j]));
    }
    free(NIL);
    free(string);
    i=Tabnec;
    libera(i,lenght);

}
