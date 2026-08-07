#include "Mkdisk.h"
#include <regex>
#include <algorithm>
#include <cctype>
#include <iostream>

namespace Commands {

    // Convierte a minúsculas
    static std::string toLowerStr(std::string s) {
        std::transform(s.begin(), s.end(), s.begin(),
                       [](unsigned char c){ return std::tolower(c); });
        return s;
    }

    // Une los tokens en un solo string
    static std::string joinTokens(const std::vector<std::string>& tokens) {
        std::string result;
        for (size_t i = 0; i < tokens.size(); ++i) {
            if (i > 0) result += " ";
            result += tokens[i];
        }
        return result;
    }

    CommandResult Mkdisk_Command(const std::vector<std::string>& tokens) {
        std::string atributos = joinTokens(tokens);

        // Expresión regular para reconocer cada parámetro válido
        // -size= número positivo
        // -unit= K o M (case insensitive)
        // -fit= BF, FF o WF (case insensitive)
        // -path= ruta sin espacios o con comillas
        static const std::regex lexic(
            R"(-size=\d+|-unit=[km]|-fit=[bfw]{2}|-path="[^"]+"|-path=[^\s]+)",
            std::regex::icase
        );

        // Buscar todas las coincidencias del patrón en la cadena
        std::vector<std::string> found;
        auto begin = std::sregex_iterator(atributos.begin(), atributos.end(), lexic);
        auto end = std::sregex_iterator();
        for (auto it = begin; it != end; ++it) {
            found.push_back(it->str());
        }

        // Si el número de coincidencias no coincide con el número de tokens,
        // significa que hay un parámetro inválido
        if (found.size() != tokens.size()) {
            for (const auto& token : tokens) {
                if (!std::regex_search(token, lexic)) {
                    return {false, "ERROR: Parámetro no reconocido: " + token + " en comando MKDISK"};
                }
            }
        }

        // Variables para almacenar los valores de los parámetros
        bool tieneSize = false;
        bool tienePath = false;
        int sizeVal = 0;
        std::string unitVal = "M";  // por defecto: Megabytes
        std::string fitVal = "FF";  // por defecto: First Fit
        std::string pathVal = "";

        // Procesar cada parámetro encontrado
        for (const auto& param : found) {
            size_t eqPos = param.find('=');
            if (eqPos == std::string::npos) {
                return {false, "ERROR: Parámetro inválido: " + param};
            }

            std::string key = toLowerStr(param.substr(0, eqPos));
            std::string value = param.substr(eqPos + 1);

            // Si el valor está entre comillas, las removemos
            if (value.size() >= 2 && value.front() == '"' && value.back() == '"') {
                value = value.substr(1, value.size() - 2);
            }

            if (key == "-size") {
                try {
                    size_t charsUsados = 0;
                    int size = std::stoi(value, &charsUsados);
                    // Verificar que se haya consumido todo el string y que sea positivo
                    if (charsUsados != value.size() || size <= 0) {
                        return {false, "ERROR: El tamaño del disco debe ser un número entero positivo"};
                    }
                    sizeVal = size;
                    tieneSize = true;
                } catch (...) {
                    return {false, "ERROR: El tamaño del disco debe ser un número entero positivo"};
                }
            } else if (key == "-unit") {
                std::string unitUpper = value;
                std::transform(unitUpper.begin(), unitUpper.end(), unitUpper.begin(),
                               [](unsigned char c){ return std::toupper(c); });
                if (unitUpper == "K" || unitUpper == "M") {
                    unitVal = unitUpper;
                } else {
                    return {false, "ERROR: Unidad inválida. Use K (Kilobytes) o M (Megabytes)"};
                }
            } else if (key == "-fit") {
                std::string fitUpper = value;
                std::transform(fitUpper.begin(), fitUpper.end(), fitUpper.begin(),
                               [](unsigned char c){ return std::toupper(c); });
                if (fitUpper == "BF" || fitUpper == "FF" || fitUpper == "WF") {
                    fitVal = fitUpper;
                } else {
                    return {false, "ERROR: Ajuste inválido. Use BF, FF o WF"};
                }
            } else if (key == "-path") {
                pathVal = value;
                tienePath = true;
            } else {
                return {false, "ERROR: Parámetro no reconocido: " + key};
            }
        }

        // Validar parámetros obligatorios
        if (!tieneSize) {
            return {false, "ERROR: Falta el parámetro obligatorio -size"};
        }
        if (!tienePath) {
            return {false, "ERROR: Falta el parámetro obligatorio -path"};
        }

        // Si todo es válido, retornamos éxito
        std::string mensaje = "MKDISK ejecutado correctamente. Tamaño: " + std::to_string(sizeVal) +
                              unitVal + ", Ajuste: " + fitVal + ", Ruta: " + pathVal;
        return {true, mensaje};
    }

} // namespace Commands