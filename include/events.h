#pragma once
#include "view.h"

class Events
{
    bool KEY[256];
    public:
        bool *get_key();
};
