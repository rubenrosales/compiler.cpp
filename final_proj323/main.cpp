//Authors: Andrew Do, Cole Cambruzzi, Ruben Rosales
//Final
//CPSC 323
//Purpose:

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <stack>
#include <iterator> 
#include <sstream>

using namespace std;

string fixSymbol(string word, string symbol);
bool hasSymbol(string word, vector<string>& symbol);
bool isSymbol(char symbol);
bool isStringSymbol(char symbol, char nextSymbol);
string removeComment(string line);
bool replace(std::string& str, const std::string& from, const std::string& to);
string cleanWhiteSpaces(string line);
void checkGrammar();
bool existsIn(vector<string> ar, string value);
bool areEqual(string parse[],string ar[]);
void displayStack( stack<string> st);
void translate();
string varType(vector<string> line);
string outputStatement(vector<string> line);
string santizeInput();
void toFile(string output);
void findError(string token,string read,stack<string> grammar);

int TABLE[32][50] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 0},
    {0, 0, 5, 0, 0, 0, 5, 5, 0, 0, 0, 5, 5, 5, 5, 5, 5, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 6, 6, 6, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7, 7, 7, 7, 0},
    {0, 0, 0, 0, 0, 0, 9, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11, 11, 11, 11, 11, 0},
    {0, 0, 0, 0, 0, 13, 0, 0, 0, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 12, 12, 12, 12, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 14, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 15, 15, 15, 15, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 17, 17, 17, 17, 17, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18, 0, 0, 18, 18, 0, 0, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 0},
    {0, 0, 21, 0, 0, 0, 0, 0, 0, 0, 0, 21, 0, 19, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 22, 0, 0, 22, 22, 0, 0, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 0},
    {0, 0, 25, 0, 0, 0, 0, 0, 0, 0, 0, 25, 0, 25, 25, 23, 24, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 28, 0, 0, 27, 27, 0, 0, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 26, 26, 26, 26, 26, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 29, 29, 0, 0, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 0, 0, 0, 0, 0, 0},
    {0, 0, 31, 0, 0, 0, 0, 0, 0, 0, 0, 31, 0, 31, 31, 31, 31, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 32, 33, 0, 0, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 45, 46, 47, 48, 49, 0}
};
vector<string> COLUMNS = {"null","program", ";", "var", "begin", "end.", ":", ",", "integer", "write", "(", ")", "=", "+", "-", "*", "/","0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "a", "b", "c", "$"};
vector<string> ROWS = { "null","P", "identifier", "identifiertail", "dec-list", "dec", "dectail", "type", "stat-list",
    "stat-listtail", "stat", "W", "assign", "expr", "exprtail", "term", "termtail", "factor", "number",
    "numbertail", "sign", "digit", "id"};
vector<string> DIGITS = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
vector<string> LETTERS = {"a", "b", "c"};
vector<string> SYMBOLS = {",", ";", ":", "(", ")", "-", "+", "-", "*", "/", "=", "."};
vector<string> RESERVED_WORDS = {"program", "var", "begin", "end.", "integer", "write"};
string PREDICTIVE_SET[50][40] = {{"null"}, {"program", "identifier", ";", "var", "dec-list", "begin", "stat-list", "end."},{"id", "identifiertail"}, {"id", "identifiertail"}, {"digit", "identifiertail"}, {"%"},
    {"dec", ":", "type", ";"}, {"identifier", "dectail"}, {",", "dec"}, {"%"}, {"integer"},
    {"stat", "stat-listtail"}, {"stat-list"}, {"%"}, {"W"}, {"assign"},
    {"write", "(", "identifier", ")", ";"}, {"identifier", "=", "expr", ";"}, {"term", "exprtail"},
    {"+", "expr"}, {"-", "expr"}, {"%"}, {"factor", "termtail"}, {"*", "factor", "termtail"},
    {"/", "factor", "termtail"}, {"%"}, {"identifier"}, {"number"}, {"(", "expr", ")"},
    {"sign", "digit", "numbertail"}, {"digit", "numbertail"}, {"%"}, {"+"}, {"-"}, {"%"}, {"0"}, {"1"},
    {"2"}, {"3"}, {"4"}, {"5"}, {"6"}, {"7"}, {"8"}, {"9"}, {"a"}, {"b"}, {"c"}};

int main(){
    
    string output = santizeInput();
    
    toFile(output);
    checkGrammar();
    
    return 0;
}

void toFile(string output){
    ofstream outfile;
    outfile.open("newdata.txt");
    outfile << output;
    outfile.close();
}

string santizeInput(){
    ifstream infile("finalv1.txt");

    vector<string> symbol;
    string line,sanitizedLine, temp = "";
    
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

        string check = &line[0] + line[1];
        if(line != "" && check != "\n"){
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
    
    return sanitizedLine;
}

void translate(){
    ifstream infile;
    infile.open("newdata.txt");
    string input;
    vector<string> line;
    
    string hllString = "#include <iostream> \nusing namespace std; \nint main(){\n";
    while(!infile.eof()){
        getline(infile,input);
        
        if (input.find("var") != std::string::npos){
            getline(infile,input);
            stringstream word;
            string w;
            word << input;
            line.clear();
            while(word >> w){
                line.push_back(w);
            }
            hllString += varType(line);
        }
        else if (input.find("begin") == std::string::npos && input.find("end") == std::string::npos){
            if (input.find("=") != std::string::npos){
                hllString += input + "\n";
            }
            else if(input.find("write") != std::string::npos){
                stringstream word;
                string w;
                word << input;
                line.clear();
                while(word >> w){
                    line.push_back(w);
                }
                hllString += outputStatement(line);
            }
        }
        
    }
    infile.close();
    hllString += "return 0; \n}";
    cout << hllString << endl;
    ofstream outfile;
    outfile.open("cplusplusout.cpp");
    outfile << hllString;
    outfile.close();
}


string outputStatement(vector<string> line){
    string hllLine = "";
    for(int i=2;i<line.size();i++){
        if(line[i] == ")")
            break;
        hllLine += "cout << " + line[i] + " << endl;\n";
    }
    return hllLine;
}

string varType(vector<string> line){
    string hllString = "";
    
    string varType =line[line.size()-2];
    if(varType == "integer"){
        hllString += "int ";
    }
    else if(varType == "string"){
        hllString += "string ";
    }
    else if(varType == "double"){
        hllString += "double ";
    }
    else if(varType == "char"){
        hllString += "char ";
    }
    
    for(int i =0; i < line.size();i++){
        if(line[i] == ":")
            break;
        hllString+=line[i];
    }
    hllString += ";";
   
    
    return hllString;
}

void checkGrammar(){
    stack<string> grammarStack;
    ifstream infile;
    infile.open("newdata.txt");
    string input;
    bool next;
    int i;
    int k =0;
    string token;
    string read;
    int fail = 0;
    int col,row;
    vector<string> parse;
    string letter = "";
    
    string inputTemp;
    vector<string> inputVector;
    grammarStack.push("$");
    grammarStack.push("P");
    displayStack(grammarStack);
    
    while(!infile.eof()){
        getline(infile, input);
        
        inputVector.clear();
        if (input.find(";") != std::string::npos) {
            stringstream inputstream;
            input = input.substr(0, input.find(";")+1);
            inputstream << input;
            while(inputstream >> inputTemp){
                inputVector.push_back(inputTemp);
            }
        }
        else{
            input = input.substr(0, input.find(" "));
            inputVector.push_back(input);
        }
        
        next = false;
        i = 0;
        parse.clear();
        
        while(!grammarStack.empty()){
            if(next)
                break;
            token = grammarStack.top();
            grammarStack.pop();
            read = inputVector[i];
            string ts = input;
            read.erase(std::remove(read.begin(), read.end(), ' '),
                        read.end());
            displayStack(grammarStack);
            parse.clear();
            if( token == "$" && read == ""){
                cout << "pass" <<endl;
                translate();
                return;
            }
            else if(existsIn(LETTERS,token) || existsIn(SYMBOLS,token)
                    ||existsIn(RESERVED_WORDS,token) || existsIn(DIGITS,token)){
                char letTemp = read[k];
                string letTempS = &letTemp;
                stringstream ss;
                ss << letTemp;
                ss >> letTempS;
         
                if(token == read){
                    i+=1;
                   
                }
                else if(token == letTempS){
                    k+=1;
                    if(k == read.size()){
                        i+=1;
                        
                        k=0;
                    }
                }
                else if(existsIn(SYMBOLS,token)||existsIn(RESERVED_WORDS,token)){
                    fail = 1;
                    break;
                }
                if(i == inputVector.size()){
                    next = true;
                }
            }
            else{
                letter = read[k];
                if((existsIn(LETTERS,letter) ||  existsIn(DIGITS,letter)) && read != "end."){
                    col = find(COLUMNS.begin(), COLUMNS.end(), letter) - COLUMNS.begin();
                }
                else{
                    if(existsIn(COLUMNS,read)){
                         col = find(COLUMNS.begin(), COLUMNS.end(), read) - COLUMNS.begin();
                    }
                    else{
                        fail = 1;
                        break;
                    }
                }
                    if(existsIn(ROWS, token))
                        row = find(ROWS.begin(), ROWS.end(), token) - ROWS.begin();
                    else
                        row = 0;
                    int temp = TABLE[row][col];
                
                
                    for (int t = 0; t < sizeof(PREDICTIVE_SET[temp])/sizeof(PREDICTIVE_SET[temp][0]);t++){
                        string f =PREDICTIVE_SET[temp][t];
                        if(PREDICTIVE_SET[temp][t] != "")
                            parse.push_back(PREDICTIVE_SET[temp][t]);
                    }
                    string ar[] ={"%"};
                
                    bool eq =true;
                for(int i = 0; i <sizeof(ar)/sizeof(*ar);i++){
                    
                    if(parse[i] != ar[i]){
                        eq = false;
                        break;
                    }
                }
                    if (!eq){
                        string tAr[] ={"null"};
                        bool tEQ =true;
                        for(int i = 0; i <sizeof(tAr)/sizeof(*tAr);i++){
                            
                            if(parse[i] != tAr[i]){
                                tEQ = false;
                                break;
                            }
                        }
                        if (tEQ){
                            fail = 1;
                            break;
                        }
                        for(int k = parse.size()-1;k>=0;k--){
                            if(parse[k] != "")
                                grammarStack.push(string(parse[k]));
                            
                        }
                        parse.clear();
                        displayStack(grammarStack);
                    }
                
            }
        }
        if(fail ==1){
            findError(token,read,grammarStack);
            return;
        }
        
        
        
        
    }
    infile.close();
}
void findError(string token,string read,stack<string> grammar){
    if(token == "P"){
        cout << "program was expected" << endl;
    }
    else if(existsIn(RESERVED_WORDS,token)){
        cout << token << " was expected" << endl;
    }
    else if(token == "stat-listtail"){
        if(read == "" || read == "end")
            cout << "end. was expected" << endl;
        else
            cout << "write was expected" << endl;
    }
    else if(existsIn(SYMBOLS,token)){
        cout << token << "is missing" << endl;
    }
    else if(grammar.size() > 1){
        cout << read << " was expected" << endl;
    }
    else{
        cout << "unknown error" << endl;
    }
    
}
bool areEqual(string parse[],string ar[]){
    return std::equal(parse, parse + sizeof parse / sizeof *parse, ar);
}
bool existsIn(vector<string> ar, string value){
    for(size_t i =0;i < ar.size();i++){
        if(value == ar[i])
            return true;
    }
    return false;
}

bool replace(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

string removeComment(string line){
    bool comm = false;
    string comment = "";
    for(int i=0;i<line.size();i++){
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
        replace(line, comment, "");
    }
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


void displayStack( stack<string> st){

    vector<string> print;
    while(!st.empty()){
        print.push_back(st.top());
        st.pop();
    }
    for(int k = print.size()-1; k >=0; k--){
        cout << print[k] << " ";
    }
    cout << endl;
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