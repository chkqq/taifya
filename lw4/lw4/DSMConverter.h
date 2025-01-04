#pragma once

#include "Machine.h"
#include "EmptyStateRemover.h"

class DSMConverter
{
public:
    client::Machine ConvertToDSM(const dev::Machine& originMachine);

private:
    NonEmptyTransitions m_notEmptyTransitions = {};
};
