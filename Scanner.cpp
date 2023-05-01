#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <stack>
#include "csvReader.h"
using namespace std;

struct Symbol{
    string token;
    string classification;
    string value;
    int address;
    string segment;
};

class Scanner{
    public:
        Scanner(char* filename){
            table = getTableFromVSC("./ScannerDSFA.csv");
            codeFile.open(filename);
            tokenFile.open("./token.txt");
            tokenFile << "T,termate\n";
            peek(); //peek for the first char
            // TABLE TIME with an DNF
            int nextStage = 0;
            while (!codeFile.eof())
            {   
                previousToken = token;
                previousType = type;
                getNextToken();
                switch (nextStage)
                {
                case 1:
                    //cout << token + "   name of program   " + to_string(type) + "\n";
                    addSymbol(token, "Program Name", "?", 0, "CS");
                    break;
                case 8:{
                    //cout << token + "   name of var   " + to_string(type) + "\n";
                    addSymbol(token,"Variable","0",DSAddress,"DS");
                    break;
                }
                case 21:
                case 7:
                    //cout << previousToken + "   litera;   " + to_string(previousType) + "\n";
                    addSymbol("Lit"+previousToken,"Literal",previousToken,DSAddress,"DS");
                    break;
                default:
                    //cout << "this is default response\n";
                    break;
                }
                tokenFile << token + "," + typeString + "\n";
                cout<< token + "," + typeString + "\n";
                //check if code is done
                if(codeFile.eof()) break;
                //get the next stage to go to
                nextStage = table[nextStage][type];
            }
            addSymbol("T1","Variable","0",DSAddress,"DS");
            addSymbol("T2","Variable","0",DSAddress,"DS");
            addSymbol("T3","Variable","0",DSAddress,"DS");
            addSymbol("T4","Variable","0",DSAddress,"DS");
            addSymbol("T5","Variable","0",DSAddress,"DS");
            addSymbol("T6","Variable","0",DSAddress,"DS");
            exportSymbols();
            tokenFile << "T,termate\n";
            tokenFile.close();
        }
    private:
        stack<Symbol> symbolList;
        int DSAddress = 0;
        ifstream codeFile;
        ofstream tokenFile;
        char sHead;
        enum TokenTypes{
            CLASS, 
            variable,
            openParen, closeParen, openCurl, closeCurl, equals, semiCol,
            integer, VAR, comma, CONST,
            plus, sub, star, slash,
            equalEqual, greaterEqual, lessEqual, bangEqual, greater, less, bang,
            IF, WHILE, FOR, ENDIF, THEN, ELSE, DO, ENDWHILE,INPUT,PRINT,
            other
        };
        string token = "";
        string previousToken = "";
        TokenTypes type;
        TokenTypes previousType;
        string typeString = "";
        vector<vector<int>> table;
        
        map<string, TokenTypes> keywords {
            {"class",CLASS},{"var",VAR},{"const",CONST},{"if",IF},{"for",FOR},{"while",WHILE},
            {"then",THEN},{"endif",ENDIF},{"else",ELSE},{"do",DO},{"endwhile",ENDWHILE},{"input",INPUT},{"print",PRINT}
        };
        TokenTypes getNextToken(){
            token = sHead;
            switch (sHead)
            {
            case 'A'...'z':
                //get spring token
                getString();
                if(keywords.find(token) != keywords.end())
                {
                    type = keywords[token];
                    typeString = token;
                    break;
                } 
                type = variable;
                typeString = "variable";
                break;
            case '0'...'9':
                getInteger();
                type = integer;
                typeString = "integer";
            break;
            case '(':
                peek();
                type = openParen;
                typeString = "openParen";
            break;
            case ')':
                peek();
                type = closeParen;
                typeString = "closeParen";
            break;
            case '{':
                peek();
                type = openCurl;
                typeString = "openCurl";
            break;
            case '}':
                peek();
                type = closeCurl;
                typeString = "closeCurl";
            break;
            case '=':
                if(checkNext('=')){
                    type = equalEqual;
                    typeString = "equalEqual";
                }
                else {
                    type = equals;
                    typeString = "equal";
                }
                peek();
            break;
            case ';':
                peek();
                type = semiCol;
                typeString = "semiCol";
            break;
            case ',':
                peek();
                type = comma;
                typeString = "comma";
            break;
            case '+':
                peek();
                type = plus;
                typeString = "plus";
            break;
            case '-':
                peek();
                type = sub;
                typeString = "sub";
            break;
            case '*':
                peek();
                type = star;
                typeString = "star";
            break;
            case '/':
                peek();
                type = slash;
                typeString = "slash";
            break;
            case '>':
                if(checkNext('=')){
                    type = greaterEqual;
                    typeString = "greaterEqual";
                }
                else {
                    type = greater;
                    typeString = "greater";
                }
                peek();
            break;
            case '<':
                if(checkNext('=')){
                    type = lessEqual;
                    typeString = "lessEqual";
                }
                else {
                    type = less;
                    typeString = "less";
                }
                peek();
            break;
            case ' ':
            case '\n':
                while (sHead == ' '||sHead == '\n')
                {
                    peek();
                }
                getNextToken();
            break;
            default:
                peek();
                type = other;
            break;
            }
        }
        
        void peek(){
            codeFile.get(sHead);
        }
        bool checkNext(char want){
            bool check = false;
            if(codeFile.peek() == want){
                peek();
                token = token + sHead;
                check = true;
            }
            return check;
        }
        void getString(){
            while (true)
            {
                peek();
                if(iswalnum(sHead) == 0) return;
                token = token + sHead;
            }
        }
        void getInteger(){
            while (true)
            {
                peek();
                if(!isdigit(sHead)) return;
                token = token + sHead;
            }
            
        }
        void addSymbol(string sym, string classif, string value, int adrs, string seg){
            Symbol tempSymbol = {
                sym,
                classif,
                value,
                adrs,
                seg
            };
            symbolList.push(tempSymbol);
            DSAddress += 2;
        }
        void exportSymbols(){
            string output = "";
            int total = symbolList.size();
            for (size_t i = 0; i < total; i++)
            {
                Symbol top = symbolList.top();
                output = top.token + "," + top.classification + "," + top.value + "," + to_string(top.address) + "," + top.segment + "\n" + output;
                symbolList.pop();
            }
            cout << output;
            ofstream symbolFile("symbolTable.txt");
            symbolFile << output;
            symbolFile.close();
        }
};


int main(int argc, char* argv[]){
    Scanner scan(argv[1]);

    return 0;
}