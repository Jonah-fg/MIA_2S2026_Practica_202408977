#include <iostream>
#include <vector>
#include <sstream>
#include "Analyzer/Analyzer.h"
using namespace std;

static vector<string> splitLines(const string& texto) {
    vector<string> lineas;
    istringstream stream(texto);
    string linea;
    while (getline(stream, linea)) {
        lineas.push_back(linea);
    }
    return lineas;
}

int main() {
    string codigo = 
        "mkdisk -size=3000 -unit=M -path=/home/user/Disco1.mia\n"
        "mkdisk -size=5 -unit=K -path=/home/user/Disco2.mia\n"
        "mkdisk -size=10 -path=/home/user/Disco3.mia\n"
        "mkdisk -size=-5 -path=/home/user/Disco4.mia\n"
        "mkdisk -unit=X -size=100 -path=/home/user/Disco5.mia\n"
        "mkdisk -size=100 -path=/home/user/Disco6.mia -fit=BF\n"
        "mkdisk -path=/home/user/Disco7.mia\n";

    vector<string> lineas = splitLines(codigo);
    for (const auto& linea : lineas) {
        Analyzer::Analyze({linea});
    }
    return 0;
}