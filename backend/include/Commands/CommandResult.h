#ifndef COMMAND_RESULT_H
#define COMMAND_RESULT_H

#include <string>
using namespace std;

namespace Commands {
    struct CommandResult {
        bool exito=false;
        string mensaje;
    };
}

#endif