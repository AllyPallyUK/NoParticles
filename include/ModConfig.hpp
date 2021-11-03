#pragma once

#include "config-utils/shared/config-utils.hpp"

DECLARE_CONFIG(ModConfig,
    CONFIG_VALUE(notesEnabled, bool, "Enable note particle effect", false);
    CONFIG_VALUE(bombsEnabled, bool, "Enable bomb particle effect", false);
   
    CONFIG_INIT_FUNCTION(
        CONFIG_INIT_VALUE(notesEnabled);
        CONFIG_INIT_VALUE(bombsEnabled);
    )
);