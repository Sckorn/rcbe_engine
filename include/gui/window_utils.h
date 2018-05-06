#ifndef RCBE_ENGINE_WINDOW_UTILS_H
#define RCBE_ENGINE_WINDOW_UTILS_H

#include "gui/Window.h"

#include <boost/filesystem.hpp>

namespace rcbe::toolkit::gui
{
Window makeWindowFromFile(const std::string &path);
}

#endif //RCBE_ENGINE_WINDOW_UTILS_H
