//Authors: Andrew Do, Cole Cambruzzi, Ruben Rosales
//Program 4
//CPSC 323
//Purpose: read text from input file and fix syntax and remove comments

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;
string fixSymbol(string word, string symbol);
bool hasSymbol(string word, vector<string>& symbol);
bool isSymbol(char symbol);
bool isStringSymbol(char symbol, char nextSymbol);

int main(){
    ifstream infile("finalv1.txt");
    string line, temp, sanitizedLine;
    vector<string> symbol;
    stringstream word;
    vector<string>lines;
    
    while(!infile.eof()){
        
        getline(infile,line);
        line = line.substr(0, line.find("//"));
        if(line != ""){
            stringstream word(line);
            while(word >> temp){
                if(hasSymbol(temp,symbol)){
                    
                    for(size_t i =0; i < symbol.size(); i++ ){
                        temp =  fixSymbol(temp,symbol.at(i));
                        
                    }
                    symbol.clear();
                }
                sanitizedLine += temp + " ";
            }
            sanitizedLine += '\n';
        }
    }
    infile.close();
    cout << sanitizedLine;
    ofstream outfile;
    outfile.open("newdata.txt");
    outfile << sanitizedLine;
    outfile.close();
    return 0;
}
string fixSymbol(string word, string symbol){
    string arg,firstname, lastname,finalWord;
    
    if(word.find(symbol) != std::string::npos && symbol != "" && word.size() > 1){
        for(int i =0;i < word.size();i++){
            
            if(isSymbol(word[i])){
                if(i == 0 && (isalpha(word[i+1]) || isdigit(word[i+1]))){
                    finalWord += word[i];
                    finalWord += " ";
                }
                else if(i == word.size() -1 && (isalpha(word[i-1]) || isdigit(word[i-1]))){
                    finalWord += " ";
                    finalWord += word[i];
                }
                else if ((isalpha(word[i-1]) || isdigit(word[i-1])) && (isalpha(word[i+1])|| isdigit(word[i+1]))){
                    finalWord += " ";
                    finalWord += word[i];
                    finalWord += " ";
                }
                else
                    finalWord += word[i];
            }
            else if(i <= word.size()-2 && isStringSymbol(word[i], word[i+1])){
                string temp;
                if(i == 0){
                    temp = word[i];
                    temp += word[i+1];
                    finalWord += temp;
                    finalWord += " ";
                }
                else if(i == word.size() -2){
                    temp = word[i];
                    temp += word[i+1];
                    finalWord += " ";
                    finalWord += temp;
                }
                else if ((isalpha(word[i-1]) || isdigit(word[i-1])) && (isalpha(word[i+1])|| isdigit(word[i+1]))){
                    temp = word[i];
                    temp += word[i+1];
                    finalWord += " ";
                    finalWord += temp;
                    finalWord += " ";
                    
                }
                else{
                    finalWord += word[i];
                    finalWord += word[i+1];
                }
                i++;
            }
            
            else
                finalWord += word[i];
        }
        return finalWord;
    }
    
    return word;
}
bool isStringSymbol(char symbol, char nextSymbol){
    if(symbol == '<' && nextSymbol == '<')
        return true;
    return false;
}

bool isSymbol(char symbol){
    if(symbol == '+' || symbol == '=' || symbol == ',' || symbol == ';')
        return true;
    return false;
}

bool hasSymbol(string word, vector<string>& symbol){
    if(word.find("+") != std::string::npos){
        symbol.push_back("+");
        
    }
    if(word.find("=") != std::string::npos){
        symbol.push_back("=");
    }
    if(word.find("<<") != std::string::npos){
        symbol.push_back("<<");
    }
    if(word.find(",") != std::string::npos){
        symbol.push_back(",");
    }
    if(word.find(";") != std::string::npos){
        symbol.push_back(";");
    }
    if(!symbol.empty() && word.size() >1)
        return true;
    return false;
}