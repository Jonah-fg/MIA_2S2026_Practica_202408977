#include "Mount.h"
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
        for (size_t i=0; i<tokens.size(); ++i) {
            if (i > 0)
            resultado+=" ";

            resultado += tokens[i];
        }
        return resultado;
    }

    CommandResult Mount_Command(const vector<string>& tokens) {
        string atributos = unirTokens(tokens);

        static const regex expresion(R"(-path="[^"]+"|-path=[^\s]+|-name="[^"]+"|-name=[^\s]+)", regex::icase);

        vector<string> encontrados;
        auto begin = sregex_iterator(atributos.begin(), atributos.end(), expresion);
        auto end=sregex_iterator();
        for(auto it=begin; it !=end; ++it) {
            encontrados.push_back(it->str());
        }

        if (encontrados.size() != tokens.size()) {
            for (const auto& token : tokens) {
                if (!regex_search(token, expresion)) {
                    return {false, "ERROR: Parámetro no reconocido: " + token + " en comando MOUNT"};
                }
            }
        }

        bool tienePath= false;
        bool tieneName=false;
        string pathVal= "";
        string nameVal="";
        for (const auto& param: encontrados) {
            size_t eqPos = param.find('=');
            if (eqPos==string::npos){
                return {false, "ERROR: Parámetro inválido: "+ param};
            }
            string clave= aMinusculas(param.substr(0, eqPos));
            string valor= param.substr(eqPos+1);

            if (valor.size() >=2 && valor.front()=='"' && valor.back()=='"') {
                valor = valor.substr(1, valor.size()- 2);
            }

            if (clave=="-path") {
                pathVal =valor;
                tienePath=true;
            }
            else if (clave=="-name") {
                nameVal =valor;
                tieneName = true;
            }
            else {
                return {false, "ERROR: Parámetro no reconocido: " + clave};
            }
        }

        if (!tienePath) {
            return {false, "ERROR: Falta el parámetro obligatorio -path en MOUNT"};
        }
        if (!tieneName) {
            return {false, "ERROR: Falta el parámetro obligatorio -name en MOUNT"};
        }

        string idGenerado = "341A"; // Simulado
        return {true, "MOUNT ejecutado correctamente. Ruta: " + pathVal + ", Nombre: " + nameVal +", ID: " + idGenerado};
    }
} 