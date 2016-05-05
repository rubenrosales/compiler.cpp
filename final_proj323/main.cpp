//Authors: Andrew Do, Cole Cambruzzi, Ruben Rosales
//Program 4
//CPSC 323
//Purpose: read text from input file and fix syntax and remove comments

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;
string fixSymbol(string word, string symbol);
bool hasSymbol(string word, vector<string>& symbol);
bool isSymbol(char symbol);
bool isStringSymbol(char symbol, char nextSymbol);
string removeComment(string line);
bool replace(std::string& str, const std::string& from, const std::string& to);
string cleanWhiteSpaces(string line);

int main(){
    ifstream infile("finalv1.txt");
    string line, temp, sanitizedLine;
    vector<string> symbol;
    stringstream word;
    vector<string>lines;
    
    
    while(!infile.eof()){
        
        getline(infile,line);
        line.substr(0, line.find("//"));
        if (line.find("//") !=std::string::npos){
            if(line.find(";") ==std::string::npos)
                line = "";
            else{
    
                line = removeComment(line);
                line = cleanWhiteSpaces(line);
                
               
                
            }
            
        }
        
        if(line != "" && line[1] != 'n' && line[0] != '\\'){
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
bool replace(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}
string removeComment(string line){
    int count = 0;
    int initIndex = -1;
    bool comm = false;
    string comment = "";
    for(int i=0;i<line.size();i++){
//        if(line[i] == '/'){
//            count++;
//        }
        if(comm)
            comment += line[i];
        if(i < line.size()-1){
            if(line[i] == '/' && line[i+1]=='/'){
                if(comm)
                    comm = false;
                else
                    comm = true;
                comment += line[i];
            }
        }
    }
    if(comment != ""){
//        size_t index = 0;
//        index = line.find(comment, index);
//        line = line.replace(index, 3, "");
        replace(line, comment, "");
    }

  
//    if(count == 2){
//        cout << line;
//        line = line.substr(0, line.find("//"));
//        cout << endl<<line;
//    }
//    else if(count == 4){
//        index = line.find(";", index);
//    }
    return line;
}

string cleanWhiteSpaces(string line){
    string cleanLine = "";
    
    for(int i =0; i < line.size();i++){
        cleanLine += line[i];
        if(i < line.size()-1){
            if(isalpha(line[i])&& line[i+1] == '('){
                cleanLine+=" ";
            }
            else if(line[i] == ')' && line[i+1] != ' '){
                cleanLine+=" ";
            }
        }
    }
    return cleanLine;
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