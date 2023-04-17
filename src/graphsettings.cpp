#include "graphsettings.h"

GraphSettings& GraphSettings::instance()
{
    static GraphSettings settings;
    return settings;
}
