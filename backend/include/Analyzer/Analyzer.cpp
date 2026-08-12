#include "Analyzer.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include "../Commands/Mkdisk.h"
#include "../Commands/Rmdisk.h"
#include "../Commands/Fdisk.h"
#include "../Commands/Mount.h"
#include "../Commands/Mkfs.h" 
#include "../Commands/Mkusr.h" 
#include "../Commands/Rmusr.h" 
#include "../Commands/Mkfile.h" 
using namespace std;

namespace Analyzer {

 // Función para eliminar espacios en blanco al inicio y final
    static string recortar(const string& s) {
        size_t inicio=s.find_first_not_of(" \t\r\n");
        if (inicio==string::npos) 
        return "";

        size_t fin=s.find_last_not_of("\t\r\n");
        return s.substr(inicio, fin-inicio +1);
    }

  //Función para dividir una línea en tokens 
    static vector<string> campos(const string& linea){
    vector<string> tokens;
    string token;
    bool dentroDeComillas = false;

    for (size_t i = 0; i < linea.size(); ++i) {
        char c = linea[i];
        if (c == '"') {
            dentroDeComillas = !dentroDeComillas;
            token += c;   // conservamos las comillas
        } else if (c == ' ' && !dentroDeComillas) {
            // Si no estamos dentro de comillas, el espacio separa tokens
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
        } else {
            token += c;
        }
    }
    if (!token.empty()) {
        tokens.push_back(token);
    }
    return tokens;
}

    static string aMinusculas(string s) {
        transform(s.begin(), s.end(), s.begin(), [](unsigned char c){return tolower(c);});
        return s;
    }

    void Analyze(const vector<string>& entradas) {
        if (entradas.empty()){
            cout << "[ERROR] No se proporcionó ningún comando"<<endl;
            return;
        }

        string entrada=recortar(entradas[0]);
        if (entrada.empty()) {
            cout << "\n" << entrada<< "\n" << endl;
            return;
        }

        vector<string> tokens= campos(entrada);
        if (tokens.empty()) {
            cout << "[ERROR] No se proporcionó ningún comando válido" <<endl;
            return;
        }

        tokens[0] =aMinusculas(tokens[0]);
        vector<string> parametros(tokens.begin() +1, tokens.end());
        Commands::CommandResult resultado;

        if (tokens[0]=="mkdisk") {
            resultado=Commands::Mkdisk_Command(parametros);
        } 
        else if (tokens[0] =="rmdisk") {    
            resultado= Commands::Rmdisk_Command(parametros);
        }
        else if (tokens[0]=="fdisk"){   
            resultado = Commands::Fdisk_Command(parametros);
        }
        else if (tokens[0]=="mount"){   
            resultado = Commands::Mount_Command(parametros);
        }
        else if (tokens[0]=="mkfs"){   
            resultado =Commands::Mkfs_Command(parametros);
        }
        else if (tokens[0]=="mkusr"){   
            resultado =Commands::Mkusr_Command(parametros);
        }
        else if (tokens[0]=="rmusr"){   
            resultado =Commands::Rmusr_Command(parametros);
        }
        else if (tokens[0]=="mkfile"){   
            resultado =Commands::Mkfile_Command(parametros);
        }
        else{
            resultado.exito=false;
            resultado.mensaje="ERROR: Comando no reconocido: " +tokens[0];
        }
        if (resultado.exito){
            cout <<"[OK]"<<resultado.mensaje<< endl;
        }
        else{
            cout <<"[ERROR] "<< resultado.mensaje << endl;
        }
    }
}
