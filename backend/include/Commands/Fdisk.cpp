#include "Fdisk.h"
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
            if (i > 0) resultado +=" ";
            resultado+=tokens[i];
        }
        return resultado;
    }

    CommandResult Fdisk_Command(const vector<string>& tokens) {
        string atributos=unirTokens(tokens);

        static const regex expresion(R"(-size=\d+|-unit=[bkm]|-path="[^"]+"|-path=[^\s]+|-type=[pel]|-fit=[bfw]{2}|-name="[^"]+"|-name=[^\s]+)", regex::icase);

        vector<string> encontrados;
        auto begin =sregex_iterator(atributos.begin(), atributos.end(), expresion);
        auto end =sregex_iterator();
        for (auto it=begin; it!= end;++it) {
            encontrados.push_back(it->str());
        }
    //tokens validos
        if (encontrados.size()!= tokens.size()) {
            for (const auto& token: tokens) {
                if (!regex_search(token, expresion)){
                    return {false, "ERROR: Parámetro no reconocido: "+ token+" en comando FDISK"};
                }
            }
        }

        bool tieneSize =false;
        bool tienePath =false;
        bool tieneName= false;
        int sizeVal = 0;
        string unitVal ="K";      
        string pathVal ="";
        string typeVal= "P";      
        string fitVal= "FF";      
        string nameVal="";

        //parámetros
        for (const auto& param: encontrados) {
            size_t eqPos=param.find('=');
            if (eqPos ==string::npos){
                return {false, "ERROR: Parámetro inválido: "+ param};
            }
            string clave =aMinusculas(param.substr(0, eqPos));
            string valor= param.substr(eqPos + 1);

            if (valor.size()>=2 && valor.front() =='"'&& valor.back() == '"') {
                valor =valor.substr(1, valor.size()-2);
            }

            if (clave=="-size") {
                try{
                    size_t charsUsados = 0;
                    int size=stoi(valor, &charsUsados);
                    if (charsUsados != valor.size() || size <= 0) {
                        return {false, "ERROR: El tamaño de la partición debe ser un número entero positivo"};
                    }
                    sizeVal =size;
                    tieneSize= true;
                } 
                catch (...){
                    return {false, "ERROR: El tamaño de la partición debe ser un número entero positivo"};
                }
            }
            else if (clave=="-unit") {
                string unitUpper = valor;
                transform(unitUpper.begin(), unitUpper.end(), unitUpper.begin(), [](unsigned char c) {return toupper(c); });
                if (unitUpper=="B" || unitUpper == "K" || unitUpper== "M") {
                    unitVal = unitUpper;
                } 
                else {
                    return {false, "ERROR: Unidad inválida. Use B, K o M"};
                }
            }
            else if (clave =="-path") {
                pathVal= valor;
                tienePath=true;
            }
            else if (clave =="-type") {
                string typeUpper =valor;
                transform(typeUpper.begin(), typeUpper.end(), typeUpper.begin(), [](unsigned char c) { return toupper(c); });
                if (typeUpper== "P" || typeUpper == "E" || typeUpper == "L") {
                    typeVal=typeUpper;
                } else {
                    return {false, "ERROR: Tipo de partición inválido. Use P (Primaria), E (Extendida) o L (Lógica)"};
                }
            }
            else if (clave == "-fit") {
                string fitUpper = valor;
                transform(fitUpper.begin(), fitUpper.end(), fitUpper.begin(), [](unsigned char c) { return toupper(c); });
                if (fitUpper == "BF" || fitUpper == "FF" || fitUpper =="WF") {
                    fitVal = fitUpper;
                }
                else{
                    return {false, "ERROR: Ajuste inválido. Use BF, FF o WF"};
                }
            }
            else if (clave == "-name") {
                nameVal = valor;
                tieneName = true;
            }
            else {
                return {false, "ERROR: Parámetro no reconocido: " + clave};
            }
        }

        // obligatorios
        if (!tieneSize) {
            return {false,"ERROR: Falta el parámetro obliatorio -size en FDISK"};
        }
        if (!tienePath) {
            return {false, "ERROR: Falta el parámetro obligatorio -path en FDISK"};
        }
        if (!tieneName) {
            return {false, "ERROR: Falta el parámetro obligatorio -name en FDISK"};
        }

        string mensaje = "FDISK ejecutado correctamente. Tamaño: " + to_string(sizeVal) +unitVal + ", Ruta: " + pathVal + ", Tipo: " + typeVal + ", Ajuste: " + fitVal + ", Nombre: " + nameVal;
        return {true, mensaje};
    }

} // namespace Commands