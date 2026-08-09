#ifndef FDISK_H
#define FDISK_H

#include <vector>
#include <string>
#include "CommandResult.h"

namespace Commands{
    CommandResult Fdisk_Command(const std::vector<std::string>& tokens);
}
#endif