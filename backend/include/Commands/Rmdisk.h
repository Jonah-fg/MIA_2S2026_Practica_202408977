#ifndef RMDISK_H
#define RMDISK_H

#include <vector>
#include <string>
#include "CommandResult.h"

namespace Commands {
    //comando RMDISK
    CommandResult Rmdisk_Command(const std::vector<std::string>& tokens);
}
#endif