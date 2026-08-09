#include <iostream>
#include <vector>
#include <sstream>
#include "Analyzer/Analyzer.h"

using namespace std;

static vector<string> splitLines(const string& text) {
    vector<string> lines;
    istringstream stream(text);
    string line;
    while (getline(stream, line)) {
        lines.push_back(line);
    }
    return lines;
}

int main() {
    string code= 
"mkfile -size=15 -path=/home/user/docs/a.txt -f\n"          // válido
"mkfile -path=\"/home/mis documentos/archivo 1.txt\"\n"      // válido con comillas
"mkfile -path=/home/user/docs/b.txt -f -cont=/home/Documents/b.txt\n"  // válido
"mkfile -size=15 -path=/home/user/docs/a.txt\n"             // válido
"mkfile -path=/home/user/docs/a.txt -size=abc\n"            // error: size no numérico
"mkfile -path=/home/user/docs/a.txt -size=-5\n"             // error: size negativo
"mkfile -path=/home/user/docs/a.txt -cont=/archivo.txt\n"   // válido
"mkfile -r -path=/home/user/a.txt\n"                        // error: -r no existe
"mkfile\n";

    vector<string> lines=splitLines(code);
    for (const auto& line: lines) {
        Analyzer::Analyze({line});
    }
    return 0;
}