#include <iostream>
#include <sstream>
#include <string>
#include "httplib.h"                 
#include "Analyzer/Analyzer.h"

using namespace std;


string analizarComando(const string& comando) {
    // Redirigir la salida de cout a un stringstream para capturar lo que imprime Analyzer
    stringstream buffer;
    streambuf* viejo=cout.rdbuf(buffer.rdbuf());

    // Llamar al analizador con el comando
    Analyzer::Analyze({comando});

    // Restaurar la salida original
    cout.rdbuf(viejo);

    return buffer.str();
}

int main() {
    httplib::Server servidor;

    // Endpoint POST /analyze
    servidor.Post("/analyze", [&](const httplib::Request& req, httplib::Response& res) {
        string comando = req.body;

        if (comando.empty()) {
            res.status = 400;
            res.set_content("Error: No se recibió ningún comando", "text/plain");
            return;
        }

        string resultado = analizarComando(comando);
        res.set_content(resultado, "text/plain");
    });

    cout << "Servidor escuchando en http://localhost:8080" << endl;
    servidor.listen("localhost", 8080);

    return 0;
}