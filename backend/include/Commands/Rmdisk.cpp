#include "Rmdisk.h"
#include <regex>
#include <algorithm>
#include <cctype>

using namespace std;

namespace Commands {

    static string aMinusculas(string s) {
        transform(s.begin(), s.end(), s.begin(), [](unsigned char c){ return tolower(c); });
        return s;
    }

    static string unirTokens(const vector<string>& tokens) {
        string resultado;
        for (size_t i=0; i< tokens.size(); ++i) {
            if (i >0) 
            resultado += " ";
            
            resultado += tokens[i];
        }
        return resultado;
    }

    CommandResult Rmdisk_Command(const vector<string>& tokens) {
        string atributos = unirTokens(tokens);
        //expresion 
        static const regex patron(R"(-path="[^"]+"|-path=[^\s]+)", regex::icase);

        //coincidencias
        vector<string> encontrados;
        auto begin = sregex_iterator(atributos.begin(), atributos.end(), patron);
        auto end = sregex_iterator();
        for (auto it = begin; it !=end; ++it) {
            encontrados.push_back(it->str());
        }

        if (encontrados.size() != tokens.size()){
            for (const auto& token : tokens){
                if (!regex_search(token, patron)) {
                    return {false, "ERROR: Parámetro no reconocido: " + token + " en comando RMDISK"};
                }
            }
        }
        bool tienePath=false;
        string ruta = "";

        for(const auto& parametro : encontrados) {
            size_t posIgual = parametro.find('=');
            if (posIgual==string::npos) {
                return {false, "ERROR: Parámtro inválido: " + parametro};
            }
            string clave= aMinusculas(parametro.substr(0, posIgual));
            string valor = parametro.substr(posIgual + 1);

            if (valor.size() >=2 && valor.front()=='"' && valor.back()=='"') {
                valor =valor.substr(1, valor.size()-2);
            }

            if (clave=="-path") {
                ruta= valor;

                tienePath=true;
            }
            else {
                return {false, "ERROR: Parámetro no reconocido: " + clave};
            }
        }

        if (!tienePath){
            return{false, "ERROR: Falta el parámetro obligatrio -path"};
        }

        return{true, "RMDISK ejecutado correctamente. Ruta: " + ruta};
    }

} 