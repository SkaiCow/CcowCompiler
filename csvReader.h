#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using namespace std;

vector<vector<int>> getTableFromVSC(string fileName){
    ifstream file(fileName);
    char c;
    string num;
    int col = 0;
    int row = 0;
    vector<int> rowData;
    vector<vector<int>> colData;
    //skip first line
    file.get(c);
    while (c != '\n')
    {
        file.get(c);
    }
    //get table
    file.get(c);
    while(isdigit(c)){
        file.get(c);
    }
    file.get(c);
    while (!file.eof())
    {
        num = "";
        while(isdigit(c)){
            num = num + c;
            file.get(c);
        }
        col++;
        if(num == "") num = "-1";
        rowData.push_back(stoi(num));
        if(c == '\n'){
            row++;
            file.get(c);
            colData.push_back(rowData);
            rowData.clear();
            while(isdigit(c)){
                file.get(c);
            }
            col = 0;
        }
        file.get(c);
    }
    file.close();
    return colData;
    
}

