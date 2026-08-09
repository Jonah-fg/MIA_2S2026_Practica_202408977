#include "Rmusr.h"
#include <regex>
#include <algorithm>
#include <cctype>

using namespace std;

namespace Commands {

    static string aMinusculas(string s) {
        transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return tolower(c); });
        return s;
    }

    static string unirTokens(const vector<string>& tokens) {
        string resultado;
        for (size_t i = 0; i < tokens.size(); ++i) {
            if (i >0) 
            resultado +=" ";

            resultado += tokens[i];
        }
        return resultado;
    }

    CommandResult Rmusr_Command(const vector<string>& tokens) {
        string atributos = unirTokens(tokens);

        static const regex expresion(R"(-user=[^\s]+)", regex::icase);

        vector<string> encontrados;
        auto begin = sregex_iterator(atributos.begin(), atributos.end(), expresion);
        auto end=sregex_iterator();
        for (auto it=begin; it != end; ++it) {
            encontrados.push_back(it->str());
        }

        if (encontrados.size() != tokens.size()) {
            for (const auto& token :tokens) {
                if (!regex_search(token, expresion)) {
                    return {false, "ERROR: Parámetro no reconocido: " + token + " en comando RMUSR"};
                }
            }
        }

        bool tieneUser = false;
        string userVal = "";

        for (const auto& param : encontrados) {
            size_t eqPos = param.find('=');
            if (eqPos==string::npos) {
                return {false, "ERROR: Parámetro inválido: " + param};
            }

            string clave = aMinusculas(param.substr(0, eqPos));
            string valor = param.substr(eqPos + 1);

            if (clave == "-user") {
                userVal = valor;
                tieneUser = true;
            }
            else {
                return {false, "ERROR: Parámetro no reconocido: " + clave};
            }
        }
        if (!tieneUser) {
            return {false, "ERROR: Falta el parámetro obligatorio -user en RMUSR"};
        }
        return {true, "RMUSR ejecutado correctamente. Usuario eliminado: " + userVal};
    }

} 