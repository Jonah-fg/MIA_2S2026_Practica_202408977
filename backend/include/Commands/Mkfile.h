#ifndef MKFILE_H
#define MKFILE_H

#include <vector>
#include <string>
#include "CommandResult.h"

namespace Commands {
    CommandResult Mkfile_Command(const std::vector<std::string>& tokens);
}
#endif