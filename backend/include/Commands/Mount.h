#ifndef MOUNT_H
#define MOUNT_H

#include <vector>
#include <string>
#include "CommandResult.h"

namespace Commands {
    CommandResult Mount_Command(const std::vector<std::string>& tokens);
}
#endif