#pragma once

#include "state.h"
#include "save.h"

namespace pacman
{    
    base_state& current_state();
    base_state& set_state(base_state* new_state);

    extern save current_save;
}