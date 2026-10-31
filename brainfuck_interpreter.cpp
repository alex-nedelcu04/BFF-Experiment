#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#define TAPE_MAX_SIZE 30000

using namespace std;

ifstream fc("code.in");
ifstream fin("input.in");

vector<unsigned long long> whiles;
int tapePos, codeLen, inputLen, inputPos;
unsigned char tape[TAPE_MAX_SIZE];
string code, input;

bool compile();

int main () {
    if (!compile()) { cout << '\n'; return 0; }

    ofstream fout("output.out");
    
    for (unsigned long long i = 0; i < codeLen; i++) {
        switch(code[i]) {
            case '<': { tapePos = max(0, tapePos - 1); break; }
            case '>': { tapePos = min(TAPE_MAX_SIZE - 1, tapePos + 1); break; }

            case '+': { tape[tapePos]++; break; }
            case '-': { tape[tapePos]--; break; }

            case ',': { tape[tapePos] = (inputPos == inputLen) ? 0 : input[inputPos++]; break; }
            case '.': { cout << tape[tapePos]; fout << tape[tapePos]; break; }

            case '[': { if (tape[tapePos] == 0) { i = whiles[i]; } break; }
            case ']': { if (tape[tapePos] != 0) { i = whiles[i]; } break; }
        }
    }
    
    cout << '\n';
    return 0;
}

bool compile() {
    stack<int> brackets;

    if (!fc.is_open()) { cout << "[ERROR]: Could not open code.in\n"; return false; }
    if (!fin.is_open()) { cout << "[ERROR]: Could not open input.in\n"; return false; }

    getline(fc, code, '\0'); getline(fin, input, '\0');
    codeLen = code.length(); inputLen = input.length();

    whiles.resize(codeLen);

    for (unsigned long long pos = 0; pos < codeLen; pos++) {
        switch(code[pos]) {
            case '[': { brackets.push(pos); break; }

            case ']': {
                if (brackets.empty()) { cout << "[ERROR]: Found \']\' without starting \'[\' at position " << (pos + 1) << ";\n"; return false; } 

                whiles[brackets.top()] = pos;
                whiles[pos] = brackets.top();

                brackets.pop();
                break; 
            }
        }
    }
    
    if (!brackets.empty()) {
        cout << "[ERROR]: Missing closing \']\' for \'[\'" << ((brackets.size() == 1) ? "" : "s") << " at position";

        if (brackets.size() > 1) {
            cout << "s: ";
            while (!brackets.empty()) {
                cout << (brackets.top() + 1) << ((brackets.size() > 2) ? ", " : (brackets.size() == 2) ? " and " : "");
                brackets.pop();    
            }
        }
        else { cout << " " << (brackets.top() + 1); }

        cout << ";\n";
        return false;
    }

    return true;
}