#include "Analyzer.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include "../Commands/Mkdisk.h"
using namespace std;

namespace Analyzer {

    // Función para eliminar espacios en blanco al inicio y final
    static string recortar(const string& s) {
        size_t inicio = s.find_first_not_of(" \t\r\n");
        if (inicio == string::npos) return "";
        size_t fin = s.find_last_not_of(" \t\r\n");
        return s.substr(inicio, fin - inicio + 1);
    }

    // Función para dividir una línea en tokens (separados por espacios)
    static vector<string> campos(const string& s) {
        vector<string> tokens;
        istringstream iss(s);
        string tok;
        while (iss >> tok) {
            tokens.push_back(tok);
        }
        return tokens;
    }

    // Convierte un string a minúsculas
    static string aMinusculas(string s) {
        transform(s.begin(), s.end(), s.begin(),
                  [](unsigned char c){ return tolower(c); });
        return s;
    }

    void Analyze(const vector<string>& entradas) {
        if (entradas.empty()) {
            cout << "[ERROR] No se proporcionó ningún comando" << endl;
            return;
        }

        string entrada = recortar(entradas[0]);
        if (entrada.empty()) {
            cout << "\n" << entrada << "\n" << endl;
            return;
        }

        vector<string> tokens = campos(entrada);
        if (tokens.empty()) {
            cout << "[ERROR] No se proporcionó ningún comando válido" << endl;
            return;
        }

        // El primer token es el nombre del comando (en minúsculas)
        tokens[0] = aMinusculas(tokens[0]);
        vector<string> parametros(tokens.begin() + 1, tokens.end());

        Commands::CommandResult resultado;

        // Análisis sintáctico: identificar el comando y llamar a su validador
        if (tokens[0] == "mkdisk") {
            resultado = Commands::Mkdisk_Command(parametros);
        } else {
            resultado.exito = false;
            resultado.mensaje = "ERROR: Comando no reconocido: " + tokens[0];
        }

        // Mostrar resultado
        if (resultado.exito) {
            cout << "[OK] " << resultado.mensaje << endl;
        } else {
            cout << "[ERROR] " << resultado.mensaje << endl;
        }
    }

} // namespace Analyzer
