#include "Mkdisk.h"
#include <regex>
#include <algorithm>
#include <cctype>
#include <iostream>
using namespace std;

namespace Commands {

// minusculas
    static string aMinusculas(string s) {
        transform(s.begin(), s.end(), s.begin(), [](unsigned char c){ return tolower(c);});
        return s;
    }

    static string unirTokens(const vector<string>& tokens) {
        string resultado;
        for (size_t i=0; i <tokens.size(); ++i) {
            if (i>0) 
            resultado += " ";

            resultado += tokens[i];
        }
        return resultado;
    }

    CommandResult Mkdisk_Command(const vector<string>& tokens) {
        string atributos=unirTokens(tokens);

 // Expresión regular para reconocer cada parámetro 
        static const regex patron(R"(-size=\d+|-unit=[km]|-fit=[bfw]{2}|-path="[^"]+"|-path=[^\s]+)", regex::icase);

        vector<string> encontrados;
        auto begin=sregex_iterator(atributos.begin(), atributos.end(), patron);
        auto end =sregex_iterator();
        for (auto it=begin; it!=end; ++it) {
            encontrados.push_back(it->str());
        }

 // Si el número de coincidencias no coincide con el número de tokens, significa que hay un parámetro inválido
        if (encontrados.size()!= tokens.size()) {
            for (const auto& token : tokens) {
                if (!regex_search(token, patron)) {
                    return {false, "ERROR: Parámetro no reconocido: " +token + " en comando MKDISK"};
                }
            }
        }

        bool tieneSize= false;
        bool tienePath= false;
        int tamanio = 0;
        string unidad ="M";  
        string ajuste="FF";  
        string ruta="";

        for (const auto& param: encontrados) {
            size_t eqPos= param.find('=');
            if (eqPos==string::npos){
                return {false, "ERROR: Parámetro inválido: "+ param};
            }

            string clave= aMinusculas(param.substr(0, eqPos));
            string valor= param.substr(eqPos+1);

         // Si el valor está entre comillas, las removemos
            if (valor.size()>=2 && valor.front()=='"' && valor.back()=='"') {
                valor=valor.substr(1, valor.size()-2);
            }

            if (clave=="-size") {
                try{
                    size_t charsUsados=0;
                    int size=stoi(valor, &charsUsados);
                    if (charsUsados!= valor.size() || size<=0) {
                        return {false, "ERROR: El tamaño del disco debe ser un número entero positivo"};
                    }
                    tamanio=size;
                    tieneSize=true;
                } 
                catch (...){
                    return {false, "ERROR: El tamaño del disco debe ser un número entero positivo"};
                }
            } 
            else if (clave=="-unit"){
                string unidadUpper=valor;
                transform(unidadUpper.begin(), unidadUpper.end(), unidadUpper.begin(), [](unsigned char c) { return toupper(c); });
                if (unidadUpper =="K"|| unidadUpper=="M") {
                    unidad=unidadUpper;
                } 
                else{
                    return {false, "ERROR: Unidad inválida. Use K (Kilobytes) o M (Megabytes)"};
                }     
            } 
            else if (clave=="-fit") {
                string ajusteUpper=valor;
                transform(ajusteUpper.begin(), ajusteUpper.end(), ajusteUpper.begin(), [](unsigned char c){ return toupper(c); });
                if (ajusteUpper == "BF"|| ajusteUpper== "FF" || ajusteUpper=="WF") {
                    ajuste=ajusteUpper;
                } 
                else{
                    return {false, "ERROR: Ajuste inválido. Use BF, FF o WF"};
                }
            } 
            else if (clave=="-path") {
                ruta= valor;
                tienePath = true;
            } 
            else{
                return {false, "ERROR: Parámetro no reconocido: " + clave};
            }
        }

     //parametros obligatorios
        if (!tieneSize){
            return {false, "ERROR: Falta el parámetro obligatorio -size"};
        }
        if (!tienePath){
            return {false, "ERROR: Falta el parámetro obligatorio -path"};
        }

        string mensaje="MKDISK ejecutado correctamente. Tamaño: "+ to_string(tamanio) + unidad + ", Ajuste: " + ajuste + ", Ruta: " + ruta;
        return {true, mensaje};
    }
} 