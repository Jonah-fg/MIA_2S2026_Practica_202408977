#ifndef MKUSR_H
#define MKUSR_H

#include <vector>
#include <string>
#include "CommandResult.h"

namespace Commands {
    CommandResult Mkusr_Command(const std::vector<std::string>& tokens);
}
#endif