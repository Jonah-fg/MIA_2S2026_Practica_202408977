#ifndef COMMAND_RESULT_H
#define COMMAND_RESULT_H

#include <string>

namespace Commands {
    struct CommandResult {
        bool success = false;   // true si los parámetros son válidos
        std::string message;    // mensaje de éxito o de error
    };
}

#endif