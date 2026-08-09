#ifndef RMUSR_H
#define RMUSR_H

#include <vector>
#include <string>
#include "CommandResult.h"

namespace Commands {
    CommandResult Rmusr_Command(const std::vector<std::string>& tokens);
}
#endif