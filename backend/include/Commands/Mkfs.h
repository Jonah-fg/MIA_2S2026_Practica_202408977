#ifndef MKFS_H
#define MKFS_H

#include <vector>
#include <string>
#include "CommandResult.h"

namespace Commands{
    CommandResult Mkfs_Command(const std::vector<std::string>& tokens);
}
#endif