//Authors: Andrew Do, Cole Cambruzzi, Ruben Rosales
//Final Project
//CPSC 323
//Purpose: Construct a compiler that takes an input file of mangled grammar, fixes it, and ouputs it into c++ code

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

void displayStack(stack<string> st);
void translate();
string varType(vector<string> line);
string outputStatement(vector<string> line);
string santizeInput();
void toFile(string output);
void findError(string token, string read, stack<string> grammar);

const int TABLE[32][50] = {
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 0 },
    { 0, 0, 5, 0, 0, 0, 5, 5, 0, 0, 0, 5, 5, 5, 5, 5, 5, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 6, 6, 6, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7, 7, 7, 7, 0 },
    { 0, 0, 0, 0, 0, 0, 9, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11, 11, 11, 11, 11, 0 },
    { 0, 0, 0, 0, 0, 13, 0, 0, 0, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 12, 12, 12, 12, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 14, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 15, 15, 15, 15, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 17, 17, 17, 17, 17, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18, 0, 0, 18, 18, 0, 0, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 0 },
    { 0, 0, 21, 0, 0, 0, 0, 0, 0, 0, 0, 21, 0, 19, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 22, 0, 0, 22, 22, 0, 0, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 0 },
    { 0, 0, 25, 0, 0, 0, 0, 0, 0, 0, 0, 25, 0, 25, 25, 23, 24, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 28, 0, 0, 27, 27, 0, 0, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 26, 26, 26, 26, 26, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 29, 29, 0, 0, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 31, 0, 0, 0, 0, 0, 0, 0, 0, 31, 0, 31, 31, 31, 31, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 32, 33, 0, 0, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 45, 46, 47, 48, 49, 0 }
};
const vector<string> COL = { "null","program", ";", "var", "begin", "end.", ":", ",", "integer", "write", "(", ")", "=", "+", "-", "*", "/","0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "a", "b", "c", "$" };
const vector<string> ROW = { "null","P", "identifier", "identifiertail", "dec-list", "dec", "dectail", "type", "stat-list",
    "stat-listtail", "stat", "W", "assign", "expr", "exprtail", "term", "termtail", "factor", "number",
    "numbertail", "sign", "digit", "id" };
const vector<string> DIG = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9" };
const vector<string> LET = { "a", "b", "c" };
const vector<string> SYM = { ",", ";", ":", "(", ")", "-", "+", "-", "*", "/", "=", "." };
const vector<string> RESERVED_WORDS = { "program", "var", "begin", "end.", "integer", "write" };
const string PREDICTIVE_SET[50][40] = { { "null" },{ "program", "identifier", ";", "var", "dec-list", "begin", "stat-list", "end." },{ "id", "identifiertail" },{ "id", "identifiertail" },{ "digit", "identifiertail" },{ "%" },
    { "dec", ":", "type", ";" },{ "identifier", "dectail" },{ ",", "dec" },{ "%" },{ "integer" },
    { "stat", "stat-listtail" },{ "stat-list" },{ "%" },{ "W" },{ "assign" },
    { "write", "(", "identifier", ")", ";" },{ "identifier", "=", "expr", ";" },{ "term", "exprtail" },
    { "+", "expr" },{ "-", "expr" },{ "%" },{ "factor", "termtail" },{ "*", "factor", "termtail" },
    { "/", "factor", "termtail" },{ "%" },{ "identifier" },{ "number" },{ "(", "expr", ")" },
    { "sign", "digit", "numbertail" },{ "digit", "numbertail" },{ "%" },{ "+" },{ "-" },{ "%" },{ "0" },{ "1" },
    { "2" },{ "3" },{ "4" },{ "5" },{ "6" },{ "7" },{ "8" },{ "9" },{ "a" },{ "b" },{ "c" } };

// Function: main
// Purpose: process program until user exits
int main() {
    
    string output = santizeInput();
    
    toFile(output);
    checkGrammar();
    
    return 0;
}
// Function: toFile
// Purpose: output fixed text to new file
void toFile(string output) {
    ofstream outfile;
    outfile.open("finalv2.txt");
    outfile << output;
    outfile.close();
}

// Function: santizeInput
// Purpose: remove any unneccesary lines from original text file
string santizeInput() {
    ifstream infile("finalv1.txt");
    
    vector<string> symbol;
    string line, sanitizedLine, temp = "";
    
    while (!infile.eof()) {
        
        getline(infile, line);
        line.substr(0, line.find("//"));
        if (line.find("//") != std::string::npos) {
            if (line.find(";") == std::string::npos)
                line = "";
            else {
                
                line = removeComment(line);
                line = cleanWhiteSpaces(line);
            }
            
        }
        string check;
        if (line.size() > 2)
            check = &line[0] + line[1];
        else
            check = line;
        
        if (line != "" && check != "\n") {
            stringstream word(line);
            while (word >> temp) {
                if (hasSymbol(temp, symbol)) {
                    
                    for (size_t i = 0; i < symbol.size(); i++) {
                        temp = fixSymbol(temp, symbol.at(i));
                        
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

// Function: translate
// Purpose: remove any unneccesary lines from original text file
void translate() {
    ifstream infile;
    infile.open("finalv2.txt");
    string input;
    vector<string> line;
    
    string hllString = "#include <iostream> \nusing namespace std; \nint main(){\n";
    while (!infile.eof()) {
        getline(infile, input);
        
        if (input.find("var") != std::string::npos) {
            getline(infile, input);
            stringstream word;
            string w;
            word << input;
            line.clear();
            while (word >> w) {
                line.push_back(w);
            }
            hllString += varType(line);
        }
        else if (input.find("begin") == std::string::npos && input.find("end") == std::string::npos) {
            if (input.find("=") != std::string::npos) {
                hllString += input + "\n";
            }
            else if (input.find("write") != std::string::npos) {
                stringstream word;
                string w;
                word << input;
                line.clear();
                while (word >> w) {
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

// Function: outputStatement
// Purpose: convert write to cout
string outputStatement(vector<string> line) {
    string hllLine = "";
    for (int i = 2; i<line.size(); i++) {
        if (line[i] == ")")
            break;
        hllLine += "cout << " + line[i] + " << endl;\n";
    }
    return hllLine;
}

// Function: varType
// Purpose: determine variable type of line
string varType(vector<string> line) {
    string hllString = "";
    
    string varType = line[line.size() - 2];
    if (varType == "integer") {
        hllString += "int ";
    }
    else if (varType == "string") {
        hllString += "string ";
    }
    else if (varType == "double") {
        hllString += "double ";
    }
    else if (varType == "char") {
        hllString += "char ";
    }
    
    for (int i = 0; i < line.size(); i++) {
        if (line[i] == ":")
            break;
        hllString += line[i];
    }
    hllString += ";\n";
    
    
    return hllString;
}

// Function: checkGrammar
// Purpose: verify if grammar is valid
void checkGrammar() {
    stack<string> grammarStack;
    ifstream infile;
    infile.open("finalv2.txt");
    string input;
    bool nextStatement;
    int i;
    int k = 0;
    string charac;
    string currentTok;
    bool fail = false;
    int col, row;
    vector<string> currentLine;
    string letter = "";
    
    string inputTemp;
    vector<string> inputVector;
    grammarStack.push("$");
    grammarStack.push("P");
    displayStack(grammarStack);
    
    while (!infile.eof()) {
        getline(infile, input);
        
        inputVector.clear();
        if (input.find(";") != std::string::npos) {
            stringstream inputstream;
            input = input.substr(0, input.find(";") + 1);
            inputstream << input;
            while (inputstream >> inputTemp) {
                inputVector.push_back(inputTemp);
            }
        }
        else {
            input = input.substr(0, input.find(" "));
            inputVector.push_back(input);
        }
        
        nextStatement = false;
        i = 0;
        currentLine.clear();
        
        while (!grammarStack.empty()) {
            if (nextStatement)
                break;
            charac = grammarStack.top();
            grammarStack.pop();
            currentTok = inputVector[i];
            string ts = input;
            currentTok.erase(std::remove(currentTok.begin(), currentTok.end(), ' '),
                             currentTok.end());
            displayStack(grammarStack);
            currentLine.clear();
            if (charac == "$" && currentTok == "") {
                cout << "pass" << endl;
                translate();
                return;
            }
            else if (existsIn(LET, charac) || existsIn(SYM, charac)
                     || existsIn(RESERVED_WORDS, charac) || existsIn(DIG, charac)) {
                char letTemp = currentTok[k];
                string letTempS = &letTemp;
                stringstream ss;
                ss << letTemp;
                ss >> letTempS;
                
                if (charac == currentTok) {
                    i += 1;
                    
                }
                else if (charac == letTempS) {
                    k += 1;
                    if (k == currentTok.size()) {
                        i += 1;
                        
                        k = 0;
                    }
                }
                else if (existsIn(SYM, charac) || existsIn(RESERVED_WORDS, charac)) {
                    fail = true;
                    break;
                }
                if (i == inputVector.size()) {
                    nextStatement = true;
                }
            }
            else {
                letter = currentTok[k];
                if ((existsIn(LET, letter) || existsIn(DIG, letter)) && currentTok != "end.") {
                    col = find(COL.begin(), COL.end(), letter) - COL.begin();
                }
                else {
                    if (existsIn(COL, currentTok)) {
                        col = find(COL.begin(), COL.end(), currentTok) - COL.begin();
                    }
                    else {
                        fail = true;
                        break;
                    }
                }
                if (existsIn(ROW, charac))
                    row = find(ROW.begin(), ROW.end(), charac) - ROW.begin();
                else
                    row = 0;
                int temp = TABLE[row][col];
                
                
                for (int t = 0; t < sizeof(PREDICTIVE_SET[temp]) / sizeof(PREDICTIVE_SET[temp][0]); t++) {
                    string f = PREDICTIVE_SET[temp][t];
                    if (PREDICTIVE_SET[temp][t] != "")
                        currentLine.push_back(PREDICTIVE_SET[temp][t]);
                }
                string ar[] = { "%" };
                
                bool eq = true;
                for (int i = 0; i <sizeof(ar) / sizeof(*ar); i++) {
                    
                    if (currentLine[i] != ar[i]) {
                        eq = false;
                        break;
                    }
                }
                if (!eq) {
                    string tAr[] = { "null" };
                    bool tEQ = true;
                    for (int i = 0; i <sizeof(tAr) / sizeof(*tAr); i++) {
                        
                        if (currentLine[i] != tAr[i]) {
                            tEQ = false;
                            break;
                        }
                    }
                    if (tEQ) {
                        fail = true;
                        break;
                    }
                    for (int k = currentLine.size() - 1; k >= 0; k--) {
                        if (currentLine[k] != "")
                            grammarStack.push(string(currentLine[k]));
                        
                    }
                    currentLine.clear();
                    displayStack(grammarStack);
                }
                
            }
        }
        if (fail == true) {
            findError(charac, currentTok, grammarStack);
            return;
        }
        
        
        
        
    }
    infile.close();
}

// Function: findError
// Purpose: remove any unneccesary lines from original text file
void findError(string charac, string currStatement, stack<string> grammar) {
    if (charac == "P") {
        cout << "program was expected" << endl;
    }
    else if (existsIn(RESERVED_WORDS, charac)) {
        cout << charac << " was expected" << endl;
    }
    else if (charac == "stat-listtail") {
        if (currStatement == "" || currStatement == "end" || currStatement == ".")
            cout << "end. was expected" << endl;
        else
            cout << "write was expected" << endl;
    }
    else if (existsIn(SYM, charac)) {
        cout << charac << "is missing" << endl;
    }
    else if (grammar.size() > 1) {
        cout << currStatement << " was expected" << endl;
    }
    else {
        cout << "unknown error" << endl;
    }
    
}
// Function: existsIn
// Purpose: determine if variable exists in constant vectors
bool existsIn(vector<string> ar, string value) {
    for (size_t i = 0; i < ar.size(); i++) {
        if (value == ar[i])
            return true;
    }
    return false;
}

// Function: replace
// Purpose: replace all instances of substring in string
bool replace(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if (start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

// Function: removeComment
// Purpose: remove all comments from string
string removeComment(string line) {
    bool comm = false;
    string comment = "";
    for (int i = 0; i<line.size(); i++) {
        if (comm)
            comment += line[i];
        if (i < line.size() - 1) {
            if (line[i] == '/' && line[i + 1] == '/') {
                if (comm)
                    comm = false;
                else
                    comm = true;
                comment += line[i];
            }
        }
    }
    if (comment != "") {
        replace(line, comment, "");
    }
    return line;
}

// Function: cleanWhiteSpaces
// Purpose: remove any unncessary whitespaces and add where needed
string cleanWhiteSpaces(string line) {
    string cleanLine = "";
    
    for (int i = 0; i < line.size(); i++) {
        cleanLine += line[i];
        if (i < line.size() - 1) {
            if (isalpha(line[i]) && line[i + 1] == '(') {
                cleanLine += " ";
            }
            else if (line[i] == ')' && line[i + 1] != ' ') {
                cleanLine += " ";
            }
        }
    }
    return cleanLine;
}

// Function: fixSymbol
// Purpose: if string has operator fix it
string fixSymbol(string word, string symbol) {
    string  finalWord;
    
    if (word.find(symbol) != std::string::npos && symbol != "" && word.size() > 1) {
        for (int i = 0; i < word.size(); i++) {
            
            if (isSymbol(word[i])) {
                if (i == 0 && (isalpha(word[i + 1]) || isdigit(word[i + 1]))) {
                    finalWord += word[i];
                    finalWord += " ";
                }
                else if (i == word.size() - 1 && (isalpha(word[i - 1]) || isdigit(word[i - 1]))) {
                    finalWord += " ";
                    finalWord += word[i];
                }
                else if ((isalpha(word[i - 1]) || isdigit(word[i - 1])) && (isalpha(word[i + 1]) || isdigit(word[i + 1]))) {
                    finalWord += " ";
                    finalWord += word[i];
                    finalWord += " ";
                }
                else
                    finalWord += word[i];
            }
            else if (i <= word.size() - 2 && isStringSymbol(word[i], word[i + 1])) {
                string temp;
                
                if (i == 0) {
                    temp = word[i];
                    temp += word[i + 1];
                    finalWord += temp;
                    finalWord += " ";
                }
                else if (i == word.size() - 2) {
                    temp = word[i];
                    temp += word[i + 1];
                    finalWord += " ";
                    finalWord += temp;
                }
                else if ((isalpha(word[i - 1]) || isdigit(word[i - 1])) && (isalpha(word[i + 1]) || isdigit(word[i + 1]))) {
                    temp = word[i];
                    temp += word[i + 1];
                    finalWord += " ";
                    finalWord += temp;
                    finalWord += " ";
                    
                }
                else {
                    finalWord += word[i];
                    finalWord += word[i + 1];
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

// Function: displayStack
// Purpose: output stack to trace it
void displayStack(stack<string> st) {
    
    vector<string> print;
    while (!st.empty()) {
        print.push_back(st.top());
        st.pop();
    }
    for (int k = print.size() - 1; k >= 0; k--) {
        cout << print[k] << " ";
    }
    cout << endl;
}

// Function: isStringSymbol << operand
// Purpose: determine if string is
bool isStringSymbol(char symbol, char nextSymbol) {
    if (symbol == '<' && nextSymbol == '<')
        return true;
    return false;
}
// Function: isSymbol
// Purpose: determine if variable is special char
bool isSymbol(char symbol) {
    if (symbol == '+' || symbol == '=' || symbol == ',' || symbol == ';')
        return true;
    return false;
}

// Function: hasSymbol
// Purpose: determine if variable has special char
bool hasSymbol(string word, vector<string>& symbol) {
    if (word.find("+") != std::string::npos) {
        symbol.push_back("+");
    }
    if (word.find("=") != std::string::npos) {
        symbol.push_back("=");
    }
    if (word.find("<<") != std::string::npos) {
        symbol.push_back("<<");
    }
    if (word.find(",") != std::string::npos) {
        symbol.push_back(",");
    }
    if (word.find(";") != std::string::npos) {
        symbol.push_back(";");
    }
    if (!symbol.empty() && word.size() >1)
        return true;
    return false;
}