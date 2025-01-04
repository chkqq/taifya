#pragma once

#include "Machine.h"

class MachineSaver
{
public:
    static void Save(std::ostream& output, const client::Machine& machine);
};
