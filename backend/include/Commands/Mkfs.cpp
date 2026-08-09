#include "Mkfs.h"
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
        for (size_t i = 0; i < tokens.size();++i) {
            if (i > 0) 
            resultado +=" ";

            resultado+=tokens[i];
        }
        return resultado;
    }

    CommandResult Mkfs_Command(const vector<string>& tokens) {
        string atributos = unirTokens(tokens);

        // Parámetros: -id (obligatorio), -type (opcional, solo "full")
        static const regex expresion(
            R"(-id=[^\s]+|-type=full)",
            regex::icase
        );

        vector<string> encontrados;
        auto begin = sregex_iterator(atributos.begin(), atributos.end(), expresion);
        auto end = sregex_iterator();
        for (auto it = begin; it != end; ++it) {
            encontrados.push_back(it->str());
        }

        // Verificar que todos los tokens sean válidos
        if (encontrados.size() != tokens.size()) {
            for (const auto& token : tokens) {
                if (!regex_search(token, expresion)){
                    return {false, "ERROR: Parámetro no reconocido: " + token + " en comando MKFS"};
                }
            }
        }

        bool tieneId =false;
        string idVal ="";
        string typeVal ="full";  

        for (const auto& param : encontrados) {
            size_t eqPos = param.find('=');
            if (eqPos == string::npos) {
                return {false, "ERROR: Parámetro inválido: " + param};
            }

            string clave = aMinusculas(param.substr(0, eqPos));
            string valor = param.substr(eqPos + 1);

            if (clave=="-id") {
                idVal = valor;
                tieneId= true;
            }
            else if (clave == "-type") {
                string typeLower = aMinusculas(valor);
                if (typeLower == "full") {
                    typeVal = typeLower;
                } 
                else {
                    return {false, "ERROR: Tipo de formateo inválido. Use 'full'"};
                }
            }
            else {
                return {false, "ERROR: Parámetro no reconocido: " + clave};
            }
        }

        if (!tieneId) {
            return {false, "ERROR: Falta el parmetro obligatorio -id en MKFS"};
        }

        string mensaje = "MKFS ejecutado correctamente. ID: " + idVal + ", Tipo: " + typeVal;
        return {true, mensaje};
    }

} // namespace Commands