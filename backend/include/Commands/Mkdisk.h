#ifndef MKDISK_H
#define MKDISK_H

#include <vector>
#include <string>
#include "CommandResult.h"

namespace Commands {
    // Recibe un vector de strings (cada token es un parámetro)
    CommandResult Mkdisk_Command(const std::vector<std::string>& tokens);
}

#endif