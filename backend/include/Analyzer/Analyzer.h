#ifndef ANALYZER_H
#define ANALYZER_H

#include <vector>
#include <string>
#include "../CommandResult.h"
using namespace std;

namespace Commands {
    CommandResult Mkdisk_Command(const vector<string>& tokens);
}

#endif  