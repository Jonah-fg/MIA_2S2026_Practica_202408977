#ifndef MKDISK_H
#define MKDISK_H

#include <vector>
#include <string>
#include "CommandResult.h"
using namespace std;

namespace Commands{
    CommandResult Mkdisk_Command(const vector<string>& tokens);
}
#endif