#include "Mkfile.h"
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
        for (size_t i= 0; i<tokens.size(); ++i) {
            if (i >0)
             resultado +=" ";

            resultado += tokens[i];
        }
        return resultado;
    }

    CommandResult Mkfile_Command(const vector<string>& tokens) {
        string atributos=unirTokens(tokens);
        static const regex expresion(R"(-path="[^"]+"|-path=[^\s]+|-f|-size=\d+|-cont="[^"]+"|-cont=[^\s]+)", regex::icase);

        vector<string> encontrados;
        auto begin =sregex_iterator(atributos.begin(), atributos.end(), expresion);
        auto end=sregex_iterator();
        for (auto it=begin; it!= end; ++it) {
            encontrados.push_back(it->str());
        }

        if (encontrados.size() != tokens.size()) {
            for (const auto& token :tokens) {
                if (!regex_search(token, expresion)) {
                    return {false, "ERROR: Paráetro no reconocido: " +token + " en comando MKFILE"};
                }
            }
        }
        bool tienePath= false;
        bool tieneF =false;
        bool tieneSize=false;
        bool tieneCont = false;
        string pathVal = "";
        int sizeVal =0;
        string contVal ="";

        for (const auto& param : encontrados) {
            size_t eqPos= param.find('=');
            if (eqPos==string::npos) {
                // Parámetro sin '=' (poaible -f)
                string clave = aMinusculas(param);
                if (clave=="-f"){
                    tieneF= true;
                } 
                else {
                    return {false, "ERROR: Parámetro inválido: "+ param};
                }
                continue;
            }
            string clave =aMinusculas(param.substr(0, eqPos));
            string valor= param.substr(eqPos + 1);

            if (valor.size()>=2 && valor.front() == '"' && valor.back()== '"') {
                valor = valor.substr(1, valor.size() -2);
            }

            if (clave== "-path") {
                pathVal =valor;
                tienePath =true;
            }
            else if (clave=="-size") {
                try{
                    size_t charsUsados = 0;
                    int size = stoi(valor, &charsUsados);
                    if (charsUsados != valor.size() || size < 0) {
                        return {false, "ERROR: El tamaño del archivo debe ser un número entero no negativo"};
                    }
                    sizeVal =size;
                    tieneSize = true;
                } 
                catch (...) {
                    return {false, "ERROR: El tamaño del archivo debe ser un número entero no negativo"};
                }
            }
            else if(clave == "-cont") {
                contVal=valor;
                tieneCont =true;
            }
            else{
                return {false, "ERROR: Parámetro no reconocido: " + clave};
            }
        }

        if (!tienePath){
            return {false, "ERROR: Falta el parámetro obligatorio -path en MKFILE"};
        }

        string mensaje = "MKFILE ejecutado corectamente. Ruta: " + pathVal;
        if (tieneF){
            mensaje += ", Crear carpetas si no existen (-f activado)";
        }
        if (tieneSize){
            mensaje +=", Tamaño: "+ to_string(sizeVal) + " bytes";
        }
        if (tieneCont){
            mensaje +=", Contenido desde: "+ contVal;
        }
        return{true, mensaje};
    }

} 