#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <stack>
#include "csvReader.h"
using namespace std;

struct Token{
    string token;
    string tokenType;
};

char** importCSV(string fname){
    
    ifstream tablecsv(fname);
    
    char* lines;
    char c;
    while(c!='\n'){
        tablecsv.get(c);
        lines += c;
    }
    tablecsv.ignore(256,'\n');
    tablecsv.ignore(10,',');
    char** table = new char*[(int)lines];
    int row = 0;
    int col = 0;
    table[row] = new char[(int)lines];
    while (!tablecsv.eof())
    {
        
        tablecsv.get(c);
        //cout<<c;
        if(c == ','){
            table[row][col] = '!';
            col++;
            continue;
        }
        if (c == '\n')
        {
            col = 0;
            row++;
            table[row] = new char[(int)lines];
            tablecsv.ignore(10,',');
            continue;
        }
        table[row][col] = c;
        tablecsv.get(c);
        if (c == '\n')
        {
            //cout<<endl;
            col = 0;
            row++;
            table[row] = new char[(int)lines];
            tablecsv.ignore(10,',');
            continue;
        }
        col++;
    }
    tablecsv.close();
    return table;
}

enum TokenTypes{
            CLASS,
            openParen, closeParen, openCurl, closeCurl, equals, semiCol, 
            VAR, comma, CONST,
            tplus, sub, star, slash,
            equalEqual, greaterEqual, lessEqual, bangEqual, tgreater, tless, bang,
            IF, WHILE, FOR,
            other, terminater, ENDIF, THEN, ELSE, DO, ENDWHILE, INPUT, PRINT
        };
map<string,TokenTypes> TokenTypeMap {
    {"class",CLASS},
    {"openCurl",openCurl},
    {"const",CONST},
    {"equal", equals},
    {"variable",other},
    {"integer",other},
    {"semiCol",semiCol},
    {"var", VAR},
    {"plus",tplus},
    {"sub",sub},
    {"star",star},
    {"slash",slash},
    {"if",IF},
    {"for",FOR},
    {"while",WHILE},
    {"closeCurl",closeCurl},
    {"termate",terminater},
    {"openParen",openParen},
    {"closeParen",closeParen},
    {"greaterEqual",greaterEqual},
    {"lessEqual",lessEqual},
    {"greater",tgreater},
    {"less",tless},
    {"bangEqual", bangEqual},
    {"equalEqual", equalEqual},
    {"endif", ENDIF},
    {"then", THEN},
    {"else",ELSE},
    {"do",DO},
    {"endwhile",ENDWHILE},
    {"comma",comma},
    {"print", PRINT},
    {"inptu",INPUT}
    };
enum opStrings{
            noOP,opClass,
            opPlus, opSub, opStar, opSlash,
            opConstEqual, opVarEqual, opEqual, opGreater, opLess, opGreaterEqual, opLessEqual,
            opEqualEqual, opIFThen, opELSE, opIFTHENELSE, opWhile, opOpenCloseParen, opPrint
        };
map<string,opStrings> opMap{
    {"noOP",noOP},
    {"+",opPlus},
    {"*",opStar},
    {"-",opSub},
    {"/",opSlash},
    {"const=",opConstEqual},
    {"var=",opVarEqual},
    {"class", opClass},
    {">", opGreater},
    {">=",opGreaterEqual},
    {"<", opLess},
    {"<=",opLessEqual},
    {"=", opEqual},
    {"ifthenendif", opIFThen},
    {"==",opEqualEqual},
    {"else",opELSE},
    {"ifthenelseendif",opIFTHENELSE},
    {"whiledoendwhile",opWhile},
    {"()",opOpenCloseParen},
    {"print",opPrint}
};

class SyntaxAnalyzer{
    public:
        SyntaxAnalyzer(){
            tokenFile.open("./token.txt");
            asmFile.open("./codeOut.asm");
            writeSybolTableToASM();
            priorTable = importCSV("./SyntaxTable.csv");
            addTokenToStack(readNextToken());
            PreviousToken = tokenStack.top();
            ActiveToken = readNextToken();
            asmFile<<"section .text"<<endl;
            bool hasError = false;
            while (!hasError)
            {
                if(getTokenType(ActiveToken.tokenType) == terminater && getTokenType(PreviousToken.tokenType) == terminater){
                    cout<<"The program has succesfully been interpreted."<<endl;
                    break;
                }
                cout<<"previous token: " + PreviousToken.tokenType + " " + to_string(TokenTypeMap[PreviousToken.tokenType]) + "  Active token: " + ActiveToken.tokenType + " " + to_string(TokenTypeMap[ActiveToken.tokenType]);
                char presedence = getPriorityChar(PreviousToken.tokenType, ActiveToken.tokenType);
                switch (presedence)
                {
                case '<':
                    cout<<"  Pres: <"<<endl;
                    addTokenToStack(ActiveToken);
                    if(TokenTypeMap[ActiveToken.tokenType] != TokenTypeMap["variable"] || TokenTypeMap[ActiveToken.tokenType] != TokenTypeMap["integer"])
                    {
                        PreviousToken = tokenStack.top();
                    }
                    ActiveToken = readNextToken();
                    break;
                case '=':
                    cout<<"  Pres: ="<<endl;
                    addTokenToStack(ActiveToken);
                    if(TokenTypeMap[ActiveToken.tokenType] != TokenTypeMap["variable"] || TokenTypeMap[ActiveToken.tokenType] != TokenTypeMap["integer"])
                    {
                        PreviousToken = tokenStack.top();
                    }
                    ActiveToken = readNextToken();
                    break;
                case '>':
                    cout<<"  Pres: >"<<endl;
                    popOpFromStack();
                    break;
                case '!':
                    cout<<"ERROR!"<<endl;
                    hasError = true;
                    break;
                default:
                    //cout<<"this is extrea through in stack"<<endl;
                    addTokenToStack(ActiveToken);
                    ActiveToken = readNextToken();
                    break;
                }
            }
            tokenFile.close();
            asmFile.close();
        }
    private:
        ifstream tokenFile;
        ofstream asmFile;
        vector<vector<char>> table;
        Token ActiveToken;
        Token PreviousToken;
        stack<Token> tokenStack;
        char** priorTable;
        stack<string> classStack;
        stack<string> fixUp;
        stack<string> endFixUp;
        stack<string> startWhile;
        int fixUpNumber = 1;
        int whileNumber = 1;
        TokenTypes getTokenType(string type){
            return TokenTypeMap[type];
        }
        char getPriorityChar(string A, string B){
            return priorTable[TokenTypeMap[A]][TokenTypeMap[B]];
        }
        Token readNextToken(){
            Token tempToken;
            char c;
            string t;
            tokenFile.get(c);
            while (c != '\n')
            {
                t = t+c;
                tokenFile.get(c);
                if (c == ',')
                {
                    tempToken.token = t;
                    t = "";
                    tokenFile.get(c);
                    continue;
                }
            }
            tempToken.tokenType = t;
            return tempToken;
        }
        void addTokenToStack(Token t){
            switch (getTokenType(t.tokenType))
            {
            case IF:
                fixUp.push("L"+to_string(fixUpNumber));
                fixUpNumber++;
                break;
            case ELSE:
                endFixUp.push("L"+to_string(fixUpNumber));
                fixUpNumber++;
                asmFile<<"JMP "+endFixUp.top()<<endl;
                asmFile<<fixUp.top()+": nop"<<endl;
                fixUp.pop();
                break;
            case WHILE:
                startWhile.push("W"+to_string(whileNumber));
                whileNumber++;
                asmFile<<startWhile.top()+": nop"<<endl;
                fixUp.push("L"+to_string(fixUpNumber));
                fixUpNumber++;
                break;
            case other:
                if(t.tokenType == "integer"){
                    t.token = "Lit"+t.token;
                }
            default:
                break;
            }
            if(t.token[0] == 'T' && isdigit(t.token[1] - '0')){
                tnumbers[t.token[1] - '0'-1] = 0;
            }
            tokenStack.push(t);
        }
        void popOpFromStack(){
            string A = "";
            string B = "";
            string op = "";
            Token pastOp = {"",""};
            while (true)
            {
                Token popToken = tokenStack.top();
                if (popToken.tokenType == "variable" || popToken.tokenType == "integer")
                {
                    if (A == ""){
                        A = popToken.token;
                    }
                    else{
                        B = popToken.token;
                    }
                }
                else{
                    if (pastOp.token == "")
                    {
                        pastOp = popToken;
                    }
                    else
                    {
                        if(priorTable[TokenTypeMap[popToken.tokenType]][TokenTypeMap[pastOp.tokenType]] == '<')
                        {
                            break;
                        }
                        else{
                            pastOp = popToken;
                        }
                    }
                    
                    op = popToken.token + op;
                }
                tokenStack.pop();
            }
            cout<<"op: " + op + " A: "+ A + " B: " + B<<endl;
            PreviousToken = tokenStack.top();
            writeCodeByOp(op,A,B);
        }
        int tnumbers[6] = {1,1,1,1,1,1};
        char getTNumber()
        {
            for (int i = 0; i < sizeof(tnumbers)-1; i++)
            {
                if(tnumbers[i] == 1){
                    tnumbers[i] = 0;
                    return i+1;
                }
            }
        }
        void checkForTvalues(string A, string B){
            if(A[0] == 'T' && isdigit(A[1] - '0')){
                
                tnumbers[A[1] - '0'-1] = 1;
            }
            if(B[0] == 'T' && isdigit((int)B[1])){
               
                tnumbers[B[1] - '0'-1] = 1;
            }
        }
        void writeCodeByOp(string op, string A, string B){
            switch (opMap[op])
            {
            case opPlus:{
                Token tvalue = {"T"+to_string(getTNumber()),"variable"};
                asmFile<<"mov ax,["<<A<<"]"<<endl;
                asmFile<<"add ax,["<<B<<"]"<<endl;
                asmFile<<"mov ["<<tvalue.token<<"],ax"<<endl;
                addTokenToStack(tvalue);
                }
                break;
            case opSub:{
                Token tvalue = {"T"+to_string(getTNumber()),"variable"};
                asmFile<<"mov ax,["<<A<<"]"<<endl;
                asmFile<<"sub ax,["<<B<<"]"<<endl;
                asmFile<<"mov ["<<tvalue.token<<"], ax"<<endl;
                addTokenToStack(tvalue);
            }
                break;
            case opStar:{
                Token tvalue = {"T"+to_string(getTNumber()),"variable"};
                asmFile<<"mov ax,["<<A<<"]"<<endl;
                asmFile<<"mul ["<<B<<"]"<<endl;
                asmFile<<"mov ["<<tvalue.token<<"], ax"<<endl;
                addTokenToStack(tvalue);
            }
                break;
            case opSlash:{
                Token tvalue = {"T"+to_string(getTNumber()),"variable"};
                asmFile<<"mov dx, 0"<<endl;
                asmFile<<"mov ax, ["<<B<<"]"<<endl;
                asmFile<<"mov bx, ["<<A<<"]"<<endl;
                asmFile<<"div bx"<<endl;
                asmFile<<"mov ["<<tvalue.token<<"], ax"<<endl;
                addTokenToStack(tvalue);
            }
                break;
            case opVarEqual:
            case opConstEqual:{
                asmFile<<"mov ax,["<<A<<"]"<<endl;
                asmFile<<"mov ["<<B<<"], ax"<<endl;
            }
                break;
            case opClass:{
                asmFile<<A+": nop"<<endl;
                classStack.push(A);
            }
                break;
            case opGreater:{
                asmFile<<"mov ax,["+A+"]"<<endl;
                asmFile<<"cmp ax,["+B+"]"<<endl;
                asmFile<<"JLE "+fixUp.top()<<endl;
            }
                break;
            case opGreaterEqual:{
                asmFile<<"mov ax,["+A+"]"<<endl;
                asmFile<<"cmp ax,["+B+"]"<<endl;
                asmFile<<"JL "+fixUp.top()<<endl;
            }
                break;
            case opLess:{
                asmFile<<"mov ax,["+A+"]"<<endl;
                asmFile<<"cmp ax,["+B+"]"<<endl;
                asmFile<<"JGE "+fixUp.top()<<endl;
            }
                break;
            case opLessEqual:{
                asmFile<<"mov ax,["+A+"]"<<endl;
                asmFile<<"cmp ax,["+B+"]"<<endl;
                asmFile<<"JG "+fixUp.top()<<endl;
            }
                break;
            case opEqual:{
                asmFile<<"mov ax,["+A+"]"<<endl;
                asmFile<<"mov ["+B+"],ax"<<endl;
            }
                break;
            case opEqualEqual:{
                asmFile<<"mov ax,["+A+"]"<<endl;
                asmFile<<"cmp ax,["+B+"]"<<endl;
                asmFile<<"JNE "+fixUp.top()<<endl;
            }
                break;
            case opIFThen:{
                asmFile<<fixUp.top()+": nop"<<endl;
                fixUp.pop();
            }
                break;
            case opIFTHENELSE:{
                asmFile<<endFixUp.top()+": nop"<<endl;
                endFixUp.pop();
            }
                break;
            case opWhile:{
                asmFile<<"jmp "+startWhile.top()<<endl;
                startWhile.pop();
                asmFile<<fixUp.top()+": nop"<<endl;
                fixUp.pop();
            }
                break;
            case opOpenCloseParen:{
                Token tvalue = {A,"variable"};
                addTokenToStack(tvalue);
            }
                break;
            case opPrint:{
                asmFile<<"mov ax, ["+A+"]"<<endl;
                asmFile<<"call ConvertIntegerToString"<<endl;
                asmFile<<"mov ax, ["+A+"]"<<endl;
                asmFile<<"mov eax, 4"<<endl;
                asmFile<<"mov ebx, 1"<<endl;
                asmFile<<"mov ecx, Result"<<endl;
                asmFile<<"mov ecx, ResultEnd"<<endl;
                asmFile<<"int 80h"<<endl;
            }
                break;
            default:
                break;
            }
            checkForTvalues(A,B);
        }
        void writeSybolTableToASM(){
            asmFile<<"section .data"<<endl;
            ifstream infile("./symbolTable.txt");
            while (true)
            {
                string name = "";
                string type = "";
                string value = "";
                string line = "";
                string dataType = "";
                for(int i=0; i<5; i++){
                    while (true)
                    {
                        char c = infile.get();
                        if(c == ',' || c=='\n' || infile.eof()) break;
                        switch (i)
                        {
                        case 0: name = name+c;
                            break;
                        case 1: type = type+c;
                            break;
                        case 2: value = value+c;
                            break;
                        case 3: line = line + c;
                            break;
                        case 4: dataType = dataType+c;
                            break;
                        }
                    }
                }
                if(infile.eof()) break;
                if(dataType == "DS"){
                    cout<<name + " " + type + " " + value <<endl;
                    asmFile<<name + " dd " + value<<endl;
                }
            }
            asmFile<<endl;
        }
    };

int main(){

    SyntaxAnalyzer();
    return 0;
};